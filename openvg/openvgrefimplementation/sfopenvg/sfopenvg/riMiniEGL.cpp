/*------------------------------------------------------------------------
 *
 * EGL 1.3
 * -------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief	Simple implementation of EGL 1.3
 * \note	caveats:
			- always renders into the backbuffer and blits it to window (no single buffered rendering)
			- no native Windows or Mac OS X pixmap support
			- no power management events
			- no support for swap interval
 * \todo	what happens in egl functions when eglTerminate has been called but the context and surface are still in use?
 * \todo	OSDeinitMutex should be called in case getEGL fails.
 *//*-------------------------------------------------------------------*/
#include "eglapi.h"

#include "egl.h"
#include "openvg.h"
#include "riArray.h"
#include "riMath.h"
#include "riContext.h"
#include "riImage.h"
#include "riMiniEGL.h"
#include "riDefs.h"

#ifdef BUILD_WITH_PRIVATE_EGL 
#include "eglinternal.h"
#endif

#ifdef BUILD_WITH_PRIVATE_OPENVG
#include "openvginternal.h"
#endif

#ifdef EGL_COMPOSITION
#include "eglsync.h"
//
#include <w32std.h>
#include <graphics/surfacemanager.h>
#include <graphics/surfaceconfiguration.h>
#include <graphics/suerror.h>
#include <graphics/surface_hints.h>
#include <graphics/surfaceupdateclient.h>
#include <e32hashtab.h>
#endif

using namespace OpenVGRI;
//// class EGL
EGL::EGL() :
	m_threads(),
	m_currentThreads(),
	m_displays(),
	m_referenceCount(0)
{
}
EGL::~EGL()
{
	for(int i=0;i<m_displays.size();i++)
	{
		RI_DELETE(m_displays[i]);
	}
	for(int i=0;i<m_threads.size();i++)
	{
		RI_DELETE(m_threads[i]);
	}
	//currentThreads contain just pointers to threads we just deleted
}

// -----------------------------------------------------------------------------
// 2nd phase constructor
// ported from guestEGL
//-----------------------------------------------------------------------------
//
void EGL::Create()
{
	EGL_TRACE("  EGL::Create ");
    //OpenSgResources();
    TInt err;
	err = iDisplayMapLock.CreateLocal();
	EGLPANIC_ASSERT(err == KErrNone, EEglPanicDisplayMapLockCreateLocalFailed);

	//err = iEglImageLock.CreateLocal();
	//EGLPANIC_ASSERT(err == KErrNone, EEglPanicEglImageLockCreateLocalFailed);	

	InitialiseExtensions();

	//const char* initExtensionList = EglInternalFunction_QueryExtensionList();
	//EGL_TRACE("  CGuestEGL::Create  initExtensionList=0x%x (\"%s\") <--",
	//		initExtensionList, initExtensionList ? initExtensionList : "");
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/
namespace OpenVGRI
{
EGL* getEGL()
{


	//use TLS to store static global g_egl
	EGL* pEgl=NULL; 
	if((pEgl = static_cast<EGL*>(Dll::Tls()))==NULL)
		{
		//create TLS instance
		pEgl = RI_NEW(EGL, ());
		pEgl->Create();
		Dll::SetTls(pEgl);
		}
	return pEgl;

	
}
static void releaseEGL()
{
	EGL* pEgl = static_cast<EGL*>(Dll::Tls());
	if (pEgl)
		delete pEgl; 
	Dll::SetTls(NULL);

}
//helper functions
RIEGLContext* CastToRIEGLContext(EGLContext aCtxId)
  {
  return (RIEGLContext*)(aCtxId);
  }
EGLContext CastFromRIEGLContext(RIEGLContext* aCtx)
  {
  return (EGLContext)(aCtx);
  }

RIEGLSurface* CastToRIEGLSurface(EGLSurface aSurfaceId)
  {
  return (RIEGLSurface*)(aSurfaceId);
  }
EGLSurface CastFromRIEGLSurface(RIEGLSurface* aSurface)
  {
  return (EGLSurface)(aSurface);
  }

Image* CastToImage(EGLClientBuffer aBufferId)
  {
  return (Image*)(aBufferId);
  }

EGLClientBuffer CastFromImage(Image* aBUffer)
  {
  return (EGLClientBuffer)(aBUffer);
  }
////
} // namespace OpenVGRI
/*-------------------------------------------------------------------*//*!
* \brief	Given a display ID, return the corresponding object, or NULL
*			if the ID hasn't been initialized.
* \param	
* \return	
* \note		if egl has been initialized for this display, the display ID can
*			be found from egl->m_displays
*//*-------------------------------------------------------------------*/
RIEGLDisplay* EGL::getDisplay(const EGLDisplay displayID)
{
	for(int i=0;i<m_displays.size();i++)
	{
		if(displayID == m_displays[i]->getID())
		{
			if (CreateDisplayInfo(displayID))
			{
				return m_displays[i];		
			}
		}
	}
	return NULL;		//error: the display hasn't been eglInitialized
}

/*-------------------------------------------------------------------*//*!
* \brief	Creates a display given a display ID, including Display info
* \param	
* \return	
* \note		if egl has been initialized for this display, the display ID can
*			be found from egl->m_displays
*//*-------------------------------------------------------------------*/
void EGL::addDisplay(RIEGLDisplay* display)
{
	RI_ASSERT(display); 
	m_displays.push_back(display); //throws bad alloc
	CreateDisplayInfo(display->getID());
}  


/*
  Create an information object for an opened Display.
 */
TBool EGL::CreateDisplayInfo(const EGLDisplay aDisplayID)
{
	TBool result = EFalse;
	EGL_TRACE("EGL::CreateDisplayInfo begin aDisplay=%d", aDisplayID);
	iDisplayMapLock.WriteLock();
	
	if (NULL != iDisplayMap.Find( aDisplayID))
	{
		result = ETrue;
	}
	else
	{
		TInt err = KErrNoMemory; 
		CEglDisplayInfo* dispInfo = new CEglDisplayInfo;

		if (dispInfo)
		{
			err = iDisplayMap.Insert(aDisplayID, dispInfo);
			EGL_TRACE("EGL::CreateDisplayInfo - DisplayMap insert error %d", err);
			EGLPANIC_ASSERT_DEBUG(err == KErrNone, EEglPanicDisplayMapInsertFailed);
	
			//added for egl sync extension benefit
			if (iEglSyncExtension)
			{
				err = iEglSyncExtension->EglSyncDisplayCreate(aDisplayID);
				EGL_TRACE("EGL::CreateDisplayInfo - EglSyncDisplayCreate error %d", err);
				EGLPANIC_ASSERT_DEBUG(err == KErrNone, EEglPanicEglSyncDisplayCreateFailed);
	
				if (err)
				{
					iDisplayMap.Remove(aDisplayID);
				}
			}
		}

		if (err == KErrNone)
		{
			// mark as initialized here??
			dispInfo->iInitialized = ETrue;
			result = ETrue;
		}
	}
	iDisplayMapLock.Unlock();

	EGL_TRACE("EGL::CreateDisplayInfo end, result=%d", result);
	return result;
}

/*-------------------------------------------------------------------*//*!
 * \brief   Creates the EGL sync instance
 * \ingroup eglSync
*//*-------------------------------------------------------------------*/

EGLint EGL::InitialiseExtensions()
{
	iEglSyncExtension = CEglSyncExtension::Create(*this);
	return EGL_SUCCESS;
}

// API supporting EGL sync extension
/*-------------------------------------------------------------------*//*!
 * \brief   Query and request to lock a specified display
 * \ingroup eglSync
 * \param   aEglDisplay a display identifier
 * \return  EGL_SUCCESS if successful;
 *          EGL_BAD_DISPLAY is not a name of a valid EGLDisplay
 *          EGL_NOT_INITIALIZED if the display object associated
 *          with the <aEglDisplay> has not been initialized
*//*-------------------------------------------------------------------*/

EGLint EGL::FindAndLockDisplay(EGLDisplay aDisplayID)
{
	EGLint result = EGL_BAD_DISPLAY;
	EGL_TRACE("EGL::FindAndLockDisplay aDisplay=%d", aDisplayID);
	iDisplayMapLock.ReadLock();
	CEglDisplayInfo** pDisp = iDisplayMap.Find(aDisplayID);
	if (pDisp && *pDisp)
		{
		CEglDisplayInfo* disp = *pDisp;
		if (disp->iInitialized)
			{
			EGL_TRACE("EGL::FindAndLockDisplay display found");
			result = EGL_SUCCESS;
			}
		else
			{
			EGL_TRACE("EGL::FindAndLockDisplay display not initialized");
			result = EGL_NOT_INITIALIZED;
			}
		}
	else
		{
		EGL_TRACE("EGL::FindAndLockDisplay cannot find display");
		}
	if (result != EGL_SUCCESS)
		{
		iDisplayMapLock.Unlock();
		}
	return result;
}

/*-------------------------------------------------------------------*//*!
 * \brief   Releases the lock associated with a valid EGLDisplay
 * \ingroup eglSync
 * \param   aEglDisplay a display identifier
*//*-------------------------------------------------------------------*/

void EGL::ReleaseDisplayLock(EGLDisplay aDisplayID)
{
	EGL_TRACE("EGL::ReleaseDisplayLock aDisplay=%d", aDisplayID);
	iDisplayMapLock.Unlock();
}

/*-------------------------------------------------------------------*//*!
* \brief	return EGLDisplay for the current context
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

EGLDisplay EGL::findDisplay(EGLContext ctx) const
{
	for(int i=0;i<m_displays.size();i++)
	{
        if(m_displays[i]->contextExists(ctx))
            return m_displays[i]->getID();
	}
    return EGL_NO_DISPLAY;
}

/*-------------------------------------------------------------------*//*!
* \brief	return an EGL thread struct for the thread made current, or
*            NULL if there's no current context.
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

RIEGLThread* EGL::getCurrentThread() const
{
	void* currentThreadID = OSGetCurrentThreadID();
	for(int i=0;i<m_currentThreads.size();i++)
	{
		if(currentThreadID == m_currentThreads[i]->getThreadID())
			return m_currentThreads[i];
	}
	return NULL;		//thread is not current
}

/*-------------------------------------------------------------------*//*!
* \brief	return an EGL thread struct corresponding to current OS thread.
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

RIEGLThread* EGL::getThread()
{
	void* currentThreadID = OSGetCurrentThreadID();
	for(int i=0;i<m_threads.size();i++)
	{
		if(currentThreadID == m_threads[i]->getThreadID())
			return m_threads[i];
	}

	//EGL doesn't have a struct for the thread yet, add it to EGL's list
	RIEGLThread* newThread = NULL;
	try
	{
		newThread = RI_NEW(RIEGLThread, (OSGetCurrentThreadID()));	//throws bad_alloc
		m_threads.push_back(newThread);	//throws bad_alloc
		return newThread;
	}
	catch(std::bad_alloc)
	{
		RI_DELETE(newThread);
		return NULL;
	}
}

/*-------------------------------------------------------------------*//*!
* \brief	destroy an EGL thread struct
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

void EGL::destroyThread()
{
	void* currentThreadID = OSGetCurrentThreadID();
	for(int i=0;i<m_threads.size();i++)
	{
		if(currentThreadID == m_threads[i]->getThreadID())
        {
            RIEGLThread* thread = m_threads[i];
            bool res = m_threads.remove(thread);
            RI_ASSERT(res);
            RI_UNREF(res);
            RI_DELETE(thread);
            break;
        }
	}
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

bool EGL::isInUse(const void* image) const
{
    for(int i=0;i<m_currentThreads.size();i++)
    {
        RIEGLSurface* s = m_currentThreads[i]->getCurrentSurface();
        if(s && s->getDrawable() && s->getDrawable()->isInUse((Image*)image))
            return true;
    }
    return false;
}
//
RIEGLContext::RIEGLContext(OpenVGRI::VGContext* vgctx, const EGLConfig config) :
	m_vgContext(vgctx),
	m_config(config),
	m_referenceCount(0)
{
}
RIEGLContext::~RIEGLContext()
{
	RI_ASSERT(m_referenceCount == 0);
	RI_DELETE(m_vgContext);
}

RIEGLSurface::RIEGLSurface(void* OSWindowContext, const EGLConfig config, Drawable* drawable, bool largestPbuffer, int renderBuffer) :
    m_OSWindowContext(OSWindowContext),
	m_config(config),
	m_drawable(drawable),
	m_largestPbuffer(largestPbuffer),
	m_renderBuffer(renderBuffer),
	m_referenceCount(0)
{
    RI_ASSERT(m_renderBuffer == EGL_BACK_BUFFER);   //only back buffer rendering is supported
    m_drawable->addReference();
}

RIEGLSurface::~RIEGLSurface()
{
	RI_ASSERT(m_referenceCount == 0);
    OSDestroyWindowContext(m_OSWindowContext);
	if(m_drawable)
	{
		if(!m_drawable->removeReference())
			RI_DELETE(m_drawable);
	}
}

RIEGLDisplay::RIEGLDisplay(EGLDisplay id) :
	m_id(id),
	m_contexts(),
	m_surfaces()
{
	RI_ASSERT(EGL_NUMCONFIGS == 60);

	//sorted by RGB/LUMINANCE (exact), larger total number of color bits (at least), buffer size (at least), config ID (exact)
	//NOTE: 16 bit configs need to be sorted on the fly if the request ignores some channels
	//NOTE: config IDs start from 1
	//               R  B  G  A  L  bpp samples maskBits ID
	m_configs[0].set(8, 8, 8, 8, 0, 32, 1, 8, 1);	//EGL_RGB_BUFFER, buffer size = 32
	m_configs[1].set(8, 8, 8, 0, 0, 32, 1, 8, 2);	//EGL_RGB_BUFFER, buffer size = 24
	m_configs[2].set(5, 5, 5, 1, 0, 16, 1, 4, 3);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[3].set(5, 6, 5, 0, 0, 16, 1, 4, 4);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[4].set(4, 4, 4, 4, 0, 16, 1, 4, 5);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[5].set(0, 0, 0, 8, 0, 8,  1, 8, 6);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[6].set(0, 0, 0, 4, 0, 4,  1, 4, 7);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[7].set(0, 0, 0, 1, 0, 1,  1, 1, 8);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[8].set(0, 0, 0, 0, 8, 8,  1, 8, 9);	//EGL_LUMINANCE_BUFFER, buffer size = 8
	m_configs[9].set(0, 0, 0, 0, 1, 1,  1, 1, 10);	//EGL_LUMINANCE_BUFFER, buffer size = 1

	m_configs[10].set(8, 8, 8, 8, 0, 32, 4, 1, 11);	//EGL_RGB_BUFFER, buffer size = 32
	m_configs[11].set(8, 8, 8, 0, 0, 32, 4, 1, 12);	//EGL_RGB_BUFFER, buffer size = 24
	m_configs[12].set(5, 5, 5, 1, 0, 16, 4, 1, 13);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[13].set(5, 6, 5, 0, 0, 16, 4, 1, 14);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[14].set(4, 4, 4, 4, 0, 16, 4, 1, 15);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[15].set(0, 0, 0, 8, 0, 8,  4, 1, 16);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[16].set(0, 0, 0, 4, 0, 4,  4, 1, 17);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[17].set(0, 0, 0, 1, 0, 1,  4, 1, 18);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[18].set(0, 0, 0, 0, 8, 8,  4, 1, 19);	//EGL_LUMINANCE_BUFFER, buffer size = 8
	m_configs[19].set(0, 0, 0, 0, 1, 1,  4, 1, 20);	//EGL_LUMINANCE_BUFFER, buffer size = 1

	m_configs[20].set(8, 8, 8, 8, 0, 32, 32, 1, 21);	//EGL_RGB_BUFFER, buffer size = 32
	m_configs[21].set(8, 8, 8, 0, 0, 32, 32, 1, 22);	//EGL_RGB_BUFFER, buffer size = 24
	m_configs[22].set(5, 5, 5, 1, 0, 16, 32, 1, 23);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[23].set(5, 6, 5, 0, 0, 16, 32, 1, 24);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[24].set(4, 4, 4, 4, 0, 16, 32, 1, 25);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[25].set(0, 0, 0, 8, 0, 8,  32, 1, 26);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[26].set(0, 0, 0, 4, 0, 4,  32, 1, 27);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[27].set(0, 0, 0, 1, 0, 1,  32, 1, 28);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[28].set(0, 0, 0, 0, 8, 8,  32, 1, 29);	//EGL_LUMINANCE_BUFFER, buffer size = 8
	m_configs[29].set(0, 0, 0, 0, 1, 1,  32, 1, 30);	//EGL_LUMINANCE_BUFFER, buffer size = 1

    //configs without mask
	m_configs[30].set(8, 8, 8, 8, 0, 32, 1, 0, 31);	//EGL_RGB_BUFFER, buffer size = 32
	m_configs[31].set(8, 8, 8, 0, 0, 32, 1, 0, 32);	//EGL_RGB_BUFFER, buffer size = 24
	m_configs[32].set(5, 5, 5, 1, 0, 16, 1, 0, 33);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[33].set(5, 6, 5, 0, 0, 16, 1, 0, 34);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[34].set(4, 4, 4, 4, 0, 16, 1, 0, 35);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[35].set(0, 0, 0, 8, 0, 8,  1, 0, 36);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[36].set(0, 0, 0, 4, 0, 4,  1, 0, 37);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[37].set(0, 0, 0, 1, 0, 1,  1, 0, 38);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[38].set(0, 0, 0, 0, 8, 8,  1, 0, 39);	//EGL_LUMINANCE_BUFFER, buffer size = 8
	m_configs[39].set(0, 0, 0, 0, 1, 1,  1, 0, 40);	//EGL_LUMINANCE_BUFFER, buffer size = 1

	m_configs[40].set(8, 8, 8, 8, 0, 32, 4, 0, 41);	//EGL_RGB_BUFFER, buffer size = 32
	m_configs[41].set(8, 8, 8, 0, 0, 32, 4, 0, 42);	//EGL_RGB_BUFFER, buffer size = 24
	m_configs[42].set(5, 5, 5, 1, 0, 16, 4, 0, 43);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[43].set(5, 6, 5, 0, 0, 16, 4, 0, 44);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[44].set(4, 4, 4, 4, 0, 16, 4, 0, 45);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[45].set(0, 0, 0, 8, 0, 8,  4, 0, 46);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[46].set(0, 0, 0, 4, 0, 4,  4, 0, 47);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[47].set(0, 0, 0, 1, 0, 1,  4, 0, 48);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[48].set(0, 0, 0, 0, 8, 8,  4, 0, 49);	//EGL_LUMINANCE_BUFFER, buffer size = 8
	m_configs[49].set(0, 0, 0, 0, 1, 1,  4, 0, 50);	//EGL_LUMINANCE_BUFFER, buffer size = 1

	m_configs[50].set(8, 8, 8, 8, 0, 32, 32, 0, 51);	//EGL_RGB_BUFFER, buffer size = 32
	m_configs[51].set(8, 8, 8, 0, 0, 32, 32, 0, 52);	//EGL_RGB_BUFFER, buffer size = 24
	m_configs[52].set(5, 5, 5, 1, 0, 16, 32, 0, 53);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[53].set(5, 6, 5, 0, 0, 16, 32, 0, 54);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[54].set(4, 4, 4, 4, 0, 16, 32, 0, 55);	//EGL_RGB_BUFFER, buffer size = 16
	m_configs[55].set(0, 0, 0, 8, 0, 8,  32, 0, 56);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[56].set(0, 0, 0, 4, 0, 4,  32, 0, 57);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[57].set(0, 0, 0, 1, 0, 1,  32, 0, 58);	//EGL_RGB_BUFFER, buffer size = 8
	m_configs[58].set(0, 0, 0, 0, 8, 8,  32, 0, 59);	//EGL_LUMINANCE_BUFFER, buffer size = 8
	m_configs[59].set(0, 0, 0, 0, 1, 1,  32, 0, 60);	//EGL_LUMINANCE_BUFFER, buffer size = 1
/*
attrib                default        criteria order   priority
--------------------------------------------------------------
EGL_COLOR_BUFFER_TYPE EGL_RGB_BUFFER Exact    None    2 
EGL_RED_SIZE          0              AtLeast  Special 3 
EGL_GREEN_SIZE        0              AtLeast  Special 3 
EGL_BLUE_SIZE         0              AtLeast  Special 3 
EGL_LUMINANCE_SIZE    0              AtLeast  Special 3 
EGL_ALPHA_SIZE        0              AtLeast  Special 3 
EGL_BUFFER_SIZE       0              AtLeast  Smaller 4 
EGL_CONFIG_ID         EGL_DONT_CARE  Exact    Smaller 11
*/
}

RIEGLDisplay::~RIEGLDisplay()
{
	//mark everything for deletion, but don't delete the current context and surface
	for(int i=0;i<m_contexts.size();i++)
	{
		if(!m_contexts[i]->removeReference())
			RI_DELETE(m_contexts[i]);
	}
	m_contexts.clear();	//remove all contexts from the list (makes further references to the current contexts invalid)

	for(int i=0;i<m_surfaces.size();i++)
	{
		if(!m_surfaces[i]->removeReference())
			RI_DELETE(m_surfaces[i]);
	}
	m_surfaces.clear();	//remove all surfaces from the list (makes further references to the current surfaces invalid)
}

EGLBoolean RIEGLDisplay::contextExists(const EGLContext ctx) const
{
	for(int i=0;i<m_contexts.size();i++)
	{
		if(m_contexts[i] == CastToRIEGLContext(ctx))
			return EGL_TRUE;
	}
	return EGL_FALSE;
}

EGLBoolean RIEGLDisplay::surfaceExists(const EGLSurface surf) const
{
	for(int i=0;i<m_surfaces.size();i++)
	{
		if(m_surfaces[i] == CastToRIEGLSurface(surf))
			return EGL_TRUE;
	}
	return EGL_FALSE;
}

RIEGLSurface* RIEGLDisplay::getSurface(const EGLSurface surf) const
{
	/*
	CEGLSurface* ret = FindObjectByPointer<CEGLSurface>( m_surfaces, surfaceId, NULL );
    if( ret && ret->IsTerminated() ) ret = NULL;
    return ret;
    */
    RIEGLSurface* ret = NULL;
    for(int i = 0; i < m_surfaces.size(); i++ )
    {
		if( m_surfaces[i] == CastToRIEGLSurface(surf) )
        {
			ret = m_surfaces[i];
        }
    }
    return ret;
}

EGLBoolean RIEGLDisplay::configExists(const EGLConfig config) const
{
    for(int i=0;i<EGL_NUMCONFIGS;i++)
    {
        if(m_configs[i].m_config == config)
		return EGL_TRUE;
    }
	return EGL_FALSE;
}

RIEGLThread::RIEGLThread(void* currentThreadID) :
	m_context(NULL),
	m_surface(NULL),
	m_error(EGL_SUCCESS),
	m_threadID(currentThreadID),
	m_boundAPI(EGL_NONE)
{
}

RIEGLThread::~RIEGLThread()
{
}

namespace OpenVGRI
{
////
// Note: egl error handling model differs from OpenVG. The latest error is stored instead of the oldest one.
//static void eglSetError(EGL* egl, EGLint error)
void eglSetError(EGL* egl, EGLint error)
{
	RIEGLThread* thread = egl->getThread();
	if(thread)
		thread->setError(error);
}

/*-------------------------------------------------------------------*//*!
* \brief	Returns the OpenVG context current to the calling thread.
* \param	
* \return	
* \note		This function is always called from a mutexed API function
*//*-------------------------------------------------------------------*/

void* eglvgGetCurrentVGContext(void)
{
	EGL* egl = OpenVGRI::getEGL();
    if(egl)
    {
        RIEGLThread* thread = egl->getCurrentThread();
        if(thread)
        {
            RI_ASSERT(thread->getCurrentContext() && thread->getCurrentSurface());
            return thread->getCurrentContext()->getVGContext();
        }
    }
	return NULL;	//not initialized or made current
}

/*-------------------------------------------------------------------*//*!
* \brief	Check if the image is current in any of the displays
* \param	
* \return	
* \note		This function is always called from a mutexed API function
*//*-------------------------------------------------------------------*/

bool eglvgIsInUse(void* image)
{
	EGL* egl = OpenVGRI::getEGL();
    if(egl)
    {
        return egl->isInUse(image);
    }
	return false;
}

} // namespace OpenVGRI


/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLint do_eglGetError(void)
#else
EGLint eglGetError()
#endif
{
    OSAcquireMutex();
    EGLint ret = EGL_SUCCESS;
	EGL* egl = OpenVGRI::getEGL();
    if(egl)
    {
        RIEGLThread* thread = egl->getThread();
        if(thread)
            ret = thread->getError();	//initialized, return error code
    }
    else ret = EGL_NOT_INITIALIZED;
    OSReleaseMutex();
    return ret;
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLDisplay do_eglGetDisplay(EGLNativeDisplayType display_id)
#else
EGLDisplay eglGetDisplay(EGLNativeDisplayType display_id)
#endif
{
    return OSGetDisplay(display_id);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
#else
EGLBoolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(display, EGL_SUCCESS, EGL_TRUE);	//already initialized

	//create the current display
	//if a context and a surface are bound by the time of eglTerminate, they remain bound until eglMakeCurrent is called
	RIEGLDisplay* newDisplay = NULL;
	try
	{
		newDisplay = RI_NEW(RIEGLDisplay, (dpy));	//throws bad_alloc
		egl->addDisplay(newDisplay);	//throws bad_alloc
		display = newDisplay;
		RI_ASSERT(display);
	}
	catch(std::bad_alloc)
	{
		RI_DELETE(newDisplay);
		EGL_RETURN(EGL_BAD_ALLOC, EGL_FALSE);
	}

	if(major) *major = 1;
	if(minor) *minor = 2;
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglTerminate(EGLDisplay dpy)
#else
EGLBoolean eglTerminate(EGLDisplay dpy)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_SUCCESS, EGL_TRUE);
    egl->removeDisplay(display);
    RI_DELETE(display);
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY const char *do_eglQueryString(EGLDisplay dpy, EGLint name)
#else
const char *eglQueryString(EGLDisplay dpy, EGLint name)
#endif
{
	EGL_GET_DISPLAY(dpy, NULL);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, NULL);

	static const char apis[] = "OpenVG OpenGL_ES OpenGL_ES2";
	static const char extensions[] = "KHR_reusable_sync EGL_SYMBIAN_COMPOSITION";
	static const char vendor[] = "Symbian Foundation";
	static const char version[] = "1.4 mergeegl";

	const char* ret = NULL;
	switch(name)
	{
	case EGL_CLIENT_APIS:
		ret = apis;
		break;

	case EGL_EXTENSIONS:
		ret = extensions;
		break;

	case EGL_VENDOR:
		ret = vendor;
		break;

	case EGL_VERSION:
		ret = version;
		break;

	default:
		EGL_RETURN(EGL_BAD_PARAMETER, NULL);
	}
	EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
#else
EGLBoolean eglGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!num_config, EGL_BAD_PARAMETER, EGL_FALSE);
	if(!configs)
	{
		*num_config = display->getNumConfigs();
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
	}
	*num_config = RI_INT_MIN(config_size, display->getNumConfigs());
	for(int i=0;i<*num_config;i++)
		configs[i] = display->getConfigByIdx(i).m_config;

	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

static bool smaller(EGLint c, EGLint filter)
{
	return (filter != EGL_DONT_CARE) && (c < filter);
}

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
#else
EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!num_config, EGL_BAD_PARAMETER, EGL_FALSE);

	if(!configs)
	{
		*num_config = display->getNumConfigs();
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
	}
	*num_config = 0;
	if(!config_size)
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);

	int bufferSize = 0;
	int redSize = 0;
	int greenSize = 0;
	int blueSize = 0;
	int luminanceSize = 0;
	int alphaSize = 0;
	int colorBufferType = EGL_RGB_BUFFER;
	int configID = EGL_DONT_CARE;
	int sampleBuffers = 0;
	int samples = 0;
	if(attrib_list)
	{
		for(int i=0;attrib_list[i] != EGL_NONE;i+=2)
		{
			switch(attrib_list[i])
			{
			case EGL_BUFFER_SIZE:				//depth of the color buffer
				bufferSize = attrib_list[i+1];
				break;
			case EGL_RED_SIZE:					//bits of Red in the color buffer
				redSize = attrib_list[i+1];
				break;
			case EGL_GREEN_SIZE:				//bits of Green in the color buffer
				greenSize = attrib_list[i+1];
				break;
			case EGL_BLUE_SIZE:					//bits of Blue in the color buffer
				blueSize = attrib_list[i+1];
				break;
			case EGL_LUMINANCE_SIZE:			//bits of Luminance in the color buffer
				luminanceSize = attrib_list[i+1];
				break;
			case EGL_ALPHA_SIZE:				//bits of Alpha in the color buffer
				alphaSize = attrib_list[i+1];
				break;
			case EGL_ALPHA_MASK_SIZE:			//bits of Alpha in the alpha mask buffer
				if(attrib_list[i+1] > 8)
					EGL_RETURN(EGL_SUCCESS, EGL_TRUE);	//not supported
				break;
			case EGL_COLOR_BUFFER_TYPE:			//enum color buffer type (EGL_RGB_BUFFER, EGL_LUMINANCE_BUFFER)
				EGL_IF_ERROR(attrib_list[i+1] != EGL_RGB_BUFFER && attrib_list[i+1] != EGL_LUMINANCE_BUFFER && attrib_list[i+1] != EGL_DONT_CARE, EGL_BAD_ATTRIBUTE, EGL_FALSE);
				colorBufferType = attrib_list[i+1];
				break;
			case EGL_CONFIG_ID:					//unique EGLConfig identifier
				configID = attrib_list[i+1];
				break;

			case EGL_SAMPLE_BUFFERS:			//integer number of multisample buffers
				sampleBuffers = attrib_list[i+1];
				break;
			case EGL_SAMPLES:					//integer number of samples per pixel
				samples = attrib_list[i+1];
				break;

			case EGL_BIND_TO_TEXTURE_RGB:		//boolean True if bindable to RGB textures. (always EGL_FALSE)
			case EGL_BIND_TO_TEXTURE_RGBA:		//boolean True if bindable to RGBA textures. (always EGL_FALSE)
			case EGL_DEPTH_SIZE:				//integer bits of Z in the depth buffer (always 0)
			case EGL_LEVEL:						//integer frame buffer level (always 0)
			case EGL_NATIVE_RENDERABLE:			//boolean EGL TRUE if native rendering APIs can render to surface (always EGL_FALSE)
			case EGL_STENCIL_SIZE:				//integer bits of Stencil in the stencil buffer (always 0)
				if(attrib_list[i+1])
					EGL_RETURN(EGL_SUCCESS, EGL_TRUE);	//not supported
				break;

			case EGL_CONFIG_CAVEAT:				//enum any caveats for the configuration (always EGL_NONE)
			case EGL_NATIVE_VISUAL_TYPE:		//integer native visual type of the associated visual (always EGL_NONE)
				if(attrib_list[i+1] != EGL_NONE)
					EGL_RETURN(EGL_SUCCESS, EGL_TRUE);	//not supported
				break;

			case EGL_MAX_SWAP_INTERVAL:			//integer maximum swap interval (always 1)
			case EGL_MIN_SWAP_INTERVAL:			//integer minimum swap interval (always 1)
				if(attrib_list[i+1] != 1)
					EGL_RETURN(EGL_SUCCESS, EGL_TRUE);	//not supported
				break;

			case EGL_RENDERABLE_TYPE:			//bitmask which client rendering APIs are supported. (always EGL_OPENVG_BIT)
				if(!(attrib_list[i+1] & EGL_OPENVG_BIT))
					EGL_RETURN(EGL_SUCCESS, EGL_TRUE);	//not supported
				break;

			case EGL_SURFACE_TYPE:				//bitmask which types of EGL surfaces are supported. (always EGL_WINDOW_BIT | EGL_PIXMAP_BIT | EGL_PBUFFER_BIT | EGL_VG_COLORSPACE_LINEAR_BIT | EGL_VG_ALPHA_FORMAT_PRE_BIT)
				break;	//all types are always supported

			case EGL_TRANSPARENT_TYPE:			//enum type of transparency supported (always EGL_NONE)
			case EGL_NATIVE_VISUAL_ID:			//integer handle of corresponding native visual (always 0)
			case EGL_MAX_PBUFFER_WIDTH:			//integer maximum width of pbuffer (always INT_MAX)
			case EGL_MAX_PBUFFER_HEIGHT:		//integer maximum height of pbuffer (always INT_MAX)
			case EGL_MAX_PBUFFER_PIXELS:		//integer maximum size of pbuffer (always INT_MAX)
			case EGL_TRANSPARENT_RED_VALUE:		//integer transparent red value (undefined)
			case EGL_TRANSPARENT_GREEN_VALUE:	//integer transparent green value (undefined)
			case EGL_TRANSPARENT_BLUE_VALUE:	//integer transparent blue value (undefined)
				break;	//ignored

			default:
				EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_FALSE);	//unknown attribute
			}
		}
	}

	if(configID && configID != EGL_DONT_CARE)
	{	//if CONFIG_ID is defined, ignore the rest of the attribs
        for(int i=0;i<EGL_NUMCONFIGS;i++)
        {
            if(display->getConfigByIdx(i).m_configID == configID)
            {
                *num_config = 1;
                *configs = display->getConfigByIdx(i).m_config;
            }
        }
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
	}

	//go through all configs, add passed configs to return list
    //TODO take alpha mask size into account
	EGLConfig found[EGL_NUMCONFIGS];
    int keys[EGL_NUMCONFIGS];
	int numFound = 0;
	for(int i=0;i<display->getNumConfigs();i++)
	{
		const RIEGLConfig& c = display->getConfigByIdx(i);

		int colorBits = c.m_desc.redBits + c.m_desc.greenBits + c.m_desc.blueBits;
		int luminanceBits = c.m_desc.luminanceBits;
		int configBufferSize;
		if(colorBits)
		{
			RI_ASSERT(!luminanceBits);
			colorBits += c.m_desc.alphaBits;
			configBufferSize = colorBits;
		}
		else if(luminanceBits)
		{
			luminanceBits += c.m_desc.alphaBits;
			configBufferSize = luminanceBits;
		}
		else
		{	//alpha only surface
			colorBits = c.m_desc.alphaBits;
			luminanceBits = c.m_desc.alphaBits;
			configBufferSize = colorBits;
		}

		if (smaller(configBufferSize, bufferSize))
			continue;

		int configSampleBuffers = c.m_samples == 1 ? 0 : 1;
		if (smaller(configSampleBuffers, sampleBuffers))
			continue;
		if (smaller(c.m_samples, samples))
			continue;

		if (smaller(c.m_desc.redBits, redSize)
			|| smaller(c.m_desc.greenBits, greenSize)
			|| smaller(c.m_desc.blueBits, blueSize) 
			|| smaller(c.m_desc.alphaBits, alphaSize) )
			continue;

		if (smaller(c.m_desc.luminanceBits, luminanceSize))
			continue;

		if ((colorBufferType == EGL_RGB_BUFFER && !colorBits) ||
			(colorBufferType == EGL_LUMINANCE_BUFFER && !luminanceBits))
			continue;

		int sortKey = c.m_configID;	//sort from smaller to larger
		int sortBits = 0;
		if(redSize != 0 && redSize != EGL_DONT_CARE)
			sortBits += c.m_desc.redBits;
		if(greenSize != 0 && greenSize != EGL_DONT_CARE)
			sortBits += c.m_desc.greenBits;
		if(blueSize != 0 && blueSize != EGL_DONT_CARE)
			sortBits += c.m_desc.blueBits;
		if(alphaSize != 0 && alphaSize != EGL_DONT_CARE)
			sortBits += c.m_desc.alphaBits;
		if(luminanceSize != 0 && luminanceSize != EGL_DONT_CARE)
			sortBits += c.m_desc.luminanceBits;
		RI_ASSERT(c.m_configID <= EGL_NUMCONFIGS);	//if there are more configs, increase the shift value
		RI_ASSERT(sortBits <= 32);
		sortKey += (32-sortBits) << 4;	//sort from larger to smaller

		found[numFound] = c.m_config;
		keys[numFound++] = sortKey;
	}
	if(!numFound)
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);

	//sort return list into increasing order
	for(int e=0;e<numFound-1;e++)
	{
		for(int f=e+1;f<numFound;f++)
		{
			if(keys[e] > keys[f])
			{
                EGLConfig tmp = found[e];
                found[e] = found[f];
                found[f] = tmp;
				RI_INT_SWAP(keys[e], keys[f]);
			}
		}
	}

	//write configs into return array
	numFound = RI_INT_MIN(numFound, config_size);
	for(int i=0;i<numFound;i++)
	{
		configs[i] = found[i];
	}
	*num_config = numFound;

	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
#else
EGLBoolean eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->configExists(config), EGL_BAD_CONFIG, EGL_FALSE);
	const RIEGLConfig& c = display->getConfig(config);
	switch(attribute)
	{
	case EGL_BUFFER_SIZE:
		*value = RI_INT_MAX(c.m_desc.redBits + c.m_desc.greenBits + c.m_desc.blueBits + c.m_desc.alphaBits, c.m_desc.luminanceBits + c.m_desc.alphaBits);
		break;

	case EGL_RED_SIZE:
		*value = c.m_desc.redBits;
		break;

	case EGL_GREEN_SIZE:
		*value = c.m_desc.greenBits;
		break;

	case EGL_BLUE_SIZE:
		*value = c.m_desc.blueBits;
		break;

	case EGL_LUMINANCE_SIZE:
		*value = c.m_desc.luminanceBits;
		break;

	case EGL_ALPHA_SIZE:
		*value = c.m_desc.alphaBits;
		break;

	case EGL_ALPHA_MASK_SIZE:
		*value = c.m_maskBits;
		break;

	case EGL_BIND_TO_TEXTURE_RGB:
	case EGL_BIND_TO_TEXTURE_RGBA:
		*value = EGL_FALSE;
		break;

	case EGL_COLOR_BUFFER_TYPE:
		if(c.m_desc.redBits)
			*value = EGL_RGB_BUFFER;
		else
			*value = EGL_LUMINANCE_BUFFER;
		break;

	case EGL_CONFIG_CAVEAT:
		*value = EGL_NONE;
		break;

	case EGL_CONFIG_ID:
		*value = c.m_configID;
		break;

	case EGL_DEPTH_SIZE:
		*value = 0;
		break;

	case EGL_LEVEL:
		*value = 0;
		break;

	case EGL_MAX_PBUFFER_WIDTH:
	case EGL_MAX_PBUFFER_HEIGHT:
		*value = 16384;			//NOTE arbitrary maximum
		break;
		
	case EGL_MAX_PBUFFER_PIXELS:
		*value = 16384*16384;	//NOTE arbitrary maximum
		break;

	case EGL_MAX_SWAP_INTERVAL:
	case EGL_MIN_SWAP_INTERVAL:
		*value = 1;
		break;

	case EGL_NATIVE_RENDERABLE:
		*value = EGL_FALSE;
		break;

	case EGL_NATIVE_VISUAL_ID:
		*value = 0;
		break;

	case EGL_NATIVE_VISUAL_TYPE:
		*value = EGL_NONE;
		break;

	case EGL_RENDERABLE_TYPE:
		*value = EGL_OPENVG_BIT;
		break;

	case EGL_SAMPLE_BUFFERS:
		*value = c.m_samples > 1 ? 1 : 0;
		break;

	case EGL_SAMPLES:
		*value = c.m_samples > 1 ? c.m_samples : 0;
		break;

	case EGL_STENCIL_SIZE:
		*value = 0;
		break;

	case EGL_SURFACE_TYPE:
		*value = EGL_WINDOW_BIT | EGL_PIXMAP_BIT | EGL_PBUFFER_BIT | EGL_VG_COLORSPACE_LINEAR_BIT | EGL_VG_ALPHA_FORMAT_PRE_BIT;
		break;

	case EGL_TRANSPARENT_TYPE:
		*value = EGL_NONE;
		break;

	case EGL_TRANSPARENT_RED_VALUE:
	case EGL_TRANSPARENT_GREEN_VALUE:
	case EGL_TRANSPARENT_BLUE_VALUE:
		*value = 0;
		break;

    case EGL_CONFORMANT:
        *value = EGL_OPENVG_BIT;  //TODO return proper value
        break;

	default:
		EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_FALSE);
	}
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLSurface do_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
#else
EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display->configExists(config), EGL_BAD_CONFIG, EGL_NO_SURFACE);

	int renderBuffer = EGL_BACK_BUFFER;
	int colorSpace = EGL_COLORSPACE_sRGB;
	int alphaFormat = EGL_ALPHA_FORMAT_NONPRE;
   

	if(attrib_list)
	{
		for(int i=0;attrib_list[i] != EGL_NONE;i+=2)
		{
			switch(attrib_list[i])
			{
			case EGL_RENDER_BUFFER:
				renderBuffer = attrib_list[i+1];
				break;

			case EGL_VG_COLORSPACE:
				colorSpace = attrib_list[i+1];
				break;

			case EGL_VG_ALPHA_FORMAT:
				alphaFormat = attrib_list[i+1];
				break;

			default:
				EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);
			}
		}
	}
	//we ignore the renderBuffer parameter since we can only render to double buffered surfaces

	//TODO If the attributes of win do not correspond to config, then an EGL BAD MATCH error is generated.
	//TODO If there is already an EGLConfig associated with win (as a result of a previous eglCreateWindowSurface call), then an EGL BAD ALLOC error is generated
	// 
	void* wc = NULL;
	Drawable* d = NULL;
	RIEGLSurface* s = NULL;
	try
	{
        wc = OSCreateWindowContext(win);
		RI_ASSERT(wc);
		//TODO what should happen if window width or height is zero?
		int windowWidth = 0, windowHeight = 0;
		OSGetWindowSize(wc, windowWidth, windowHeight);
        bool isWindow = OSIsWindow(wc);
		if(windowWidth <= 0 || windowHeight <= 0 || !isWindow)
		{
            OSDestroyWindowContext(wc);
            EGL_IF_ERROR(!isWindow, EGL_BAD_NATIVE_WINDOW, EGL_NO_SURFACE);
			EGL_IF_ERROR(windowWidth <= 0 || windowHeight <= 0, EGL_BAD_NATIVE_WINDOW, EGL_NO_SURFACE);
		}
		d = RI_NEW(Drawable, (display->getConfig(config).configToDescriptor((colorSpace == EGL_VG_COLORSPACE_LINEAR) ? false : true, (alphaFormat == EGL_VG_ALPHA_FORMAT_PRE) ? true : false), windowWidth, windowHeight, display->getConfig(config).m_samples, display->getConfig(config).m_maskBits));	//throws bad_alloc
		RI_ASSERT(d);
		s = RI_NEW(RIEGLSurface,(wc, config, d, false, renderBuffer));	//throws bad_alloc
		RI_ASSERT(s);
		s->addReference();
		display->addSurface(s);	//throws bad_alloc
	}
	catch(std::bad_alloc)
	{
        OSDestroyWindowContext(wc);
        RI_DELETE(d);
        RI_DELETE(s);
		EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
	}
#ifdef EGL_COMPOSITION
	RIEGLThread* thread = egl->getThread();
	RI_ASSERT(thread);
	
    
	//egl->iDisplayMapLock.WriteLock();
	CEglDisplayInfo** pDispInfo;
	pDispInfo = egl->iDisplayMap.Find(dpy); // make sure iDisplayInfo is populated in CreateDisplay!!
	
	if (pDispInfo && *pDispInfo)
	{
		TSurfaceInfo& surfaceInfo = egl->GetSurfaceInfo();
		// so we don't have to pass alphaFormat to CreateSurface we set it here... not sure if needed anyway
		surfaceInfo.iAlphaFormat = alphaFormat;
		
		if(EGL_TRUE != egl->EglInternalFunction_CreateSurface(/*aThreadState,*/ dpy, (EGLSurface)s, config, win, surfaceInfo))
			EGL_RETURN(EGL_BAD_SURFACE, NULL);
			
		(*pDispInfo)->iSurfaceMap.Insert(surfaceInfo.iSurface, &surfaceInfo);		
		
		TSurfaceConfiguration surfaceConfig;
		surfaceConfig.SetSurfaceId(surfaceInfo.iSurfaceId);
			
		if(surfaceInfo.iNativeWindow.IsWindow())
		{
			RWindow* window = (RWindow*)(win);				
			RWsSession& session = *window->Session();
			session.RegisterSurface(surfaceInfo.iScreenNumber, surfaceInfo.iSurfaceId);
			surfaceConfig.SetFlip(ETrue); // don't ask :o
			window->SetBackgroundSurface(surfaceConfig, ETrue);
		}
		else
		{
			// Now what?? Nothing is good enough?
			// Simply no composition if not RWindow?
			EGL_TRACE("eglCreateWindowSurface - Native Window NOT RWindow - no composition");
		}
		
		TRequestStatus status = KRequestPending;
		TTimeStamp timestampLocalToThread;
		// session needs to be established... and it is in InternalFunction_CreateSurface
		/*
		 * When double-buffering is used, the client renders to one buffer (called A in this example) while the other buffer (B) is on the screen and vice versa. In order for this to work correctly and to be free of tearing artefacts, the client must use the following sequence:

		   1. Render the graphics content to buffer A, and call NotifyWhenAvailable() followed by SubmitUpdate() for buffer A.
		   2. Render the graphics content to buffer B, and call NotifyWhenAvailable() followed by SubmitUpdate() for buffer B.
		   3. Wait for notification that buffer A is available. When it becomes available, render further content to it and call NotifyWhenAvailable() followed by SubmitUpdate() for buffer A.
		   4. Wait for notification that buffer B is available. When it becomes available, render further content to it and call NotifyWhenAvailable() followed by SubmitUpdate() for buffer B.
		   5. Repeat steps 3 and 4 for as long as required.

		After sending the first two buffers to the composition engine, the client waits for notification before sending a further buffer. The composition engine always returns notification after receiving a new buffer even if an error condition is detected. 
		*/
		surfaceInfo.iSurfaceUpdateSession.NotifyWhenDisplayed(status, timestampLocalToThread);
		surfaceInfo.iSurfaceUpdateSession.SubmitUpdate(surfaceInfo.iScreenNumber,surfaceInfo.iSurfaceId, 0);
		surfaceInfo.iSurfaceUpdateSession.SubmitUpdate(surfaceInfo.iScreenNumber,surfaceInfo.iSurfaceId, 1);
		User::WaitForRequest(status);
	}
	
	
	
	//egl->iDisplayMapLock.Unlock();
		
	//thread->setError(EGL_SUCCESS); // done by EGL_RETURN macro?
#endif
	EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool EGL::EglInternalFunction_CreateSurface(/*TEglThreadState& aThreadState,*/ EGLDisplay aDisplay, EGLSurface aSurface, EGLConfig aConfig, 
		EGLNativeWindowType aNativeWindow, TSurfaceInfo& aSurfaceInfo)
{
	
	aSurfaceInfo.iNativeWindow.SetNativeWindow(aNativeWindow);    
	aSurfaceInfo.iSurfaceType = ESurfaceTypeWindow;
	aSurfaceInfo.iSize = aSurfaceInfo.iNativeWindow.SizeInPixels();
	
	aSurfaceInfo.iConfigId = aConfig;    
	aSurfaceInfo.iSurface = aSurface;
	
	aSurfaceInfo.iStride = 4 * aSurfaceInfo.iSize.iWidth;          // Number of bytes between start of one line and start of next   
	    
	
	RSurfaceManager::TSurfaceCreationAttributesBuf buf;
	RSurfaceManager::TSurfaceCreationAttributes& attributes = buf();
	// Set surafce attributes
    attributes.iSize = aSurfaceInfo.iSize;
    attributes.iBuffers = 2;           // double-buffering is used
    attributes.iPixelFormat = EUidPixelFormatARGB_8888;  // this is a guess; either query or hardcode to match syborg
    //attributes.iPixelFormat = EUidPixelFormatARGB_8888_PRE;  // this is a guess; either query or hardcode
    /** Minimum or required number of bytes between start of one line andstart of next. */
    attributes.iStride = aSurfaceInfo.iStride;
    /** Minimum or required offset to the first buffer from the base of the chunk. 
     * Typically this will be set to 0. The value specified for the offset must comply with the alignment specified in iAlignment.
     * If iAlignment is page aligned, this value will be rounded up to a multiple of the page size when the surface is created, 
     * therefore the surface info must be queried for the actual value used. */
    attributes.iOffsetToFirstBuffer = 0;
    /** Alignment applied to the base address of each buffer in the surface:
     * 1, 2, 4, 8 ,16, 32, 64 bytes or EPageAligned. */
    attributes.iAlignment = RSurfaceManager::EPageAligned;                      // alignment, 1,2,4,8,16,32,64 byte aligned or EPageAligned
    /** Number of hints in the array iSurfaceHints. 
     * The number should not exceed the maximum number supported by the surface manager, 
     * see GetSurfaceManagerAttrib(EMaxNumberOfHints). */
    attributes.iHintCount=0;
    /** Array of hints which should be associated with the surface. 
     * This array must not contain duplicate hint keys. */
     attributes.iSurfaceHints = NULL;
    /** Minimum or required offset between the start of one buffer and the
	start of the next one in bytes. When set to 0 the surface manager will
	choose how buffers are laid out within the chunk. If it is too small
	and doesn't fit with the alignment, CreateSurface() will return KErrArgument. */
    attributes.iOffsetBetweenBuffers = 0;
    /** Require physically contiguous memory. 
     * This value will be ignored if using a chunk which already exists. */
    attributes.iContiguous = ETrue;
    /** Caching attribute to create chunk memory. 
     * This value will be ignored if using a chunk which already exists. */
    attributes.iCacheAttrib = RSurfaceManager::ECached;      // Cache attributes
    /** Should the surface be mappable. 
     * If EFalse any call to MapSurface() will fail with KErrNotSupported
     * Note, some architectures may not support mappable surfaces. */
    attributes.iMappable = ETrue;	
	//
    aSurfaceInfo.iScreenNumber = aSurfaceInfo.iNativeWindow.ScreenNumber();
				
	// Create the surface
	
	aSurfaceInfo.iSurfaceManager.Open();
	aSurfaceInfo.iSurfaceManager.CreateSurface(buf, aSurfaceInfo.iSurfaceId);
	
	TInt err =  aSurfaceInfo.iSurfaceManager.MapSurface(aSurfaceInfo.iSurfaceId, aSurfaceInfo.iChunk);
		
	EGL_TRACE("EGL::EglInternalFunction_CreateSurface surface manager returned chunk %x and ret val %d", aSurfaceInfo.iChunk, err);
				


    if ( !EglInternalFunction_CallSetSurfaceParams(/*aThreadState,*/ aDisplay, aSurface, aSurfaceInfo) )
        {
        EGL_TRACE("CGuestEGL::EglInternalFunction_CreateSurface end failure");

        return EGL_FALSE;
        }
    aSurfaceInfo.iSurfaceUpdateSession.Connect();
    EGL_TRACE("CGuestEGL::EglInternalFunction_CreateSurface end success");

    return EGL_TRUE;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool EGL::EglInternalFunction_CallSetSurfaceParams(/*aThreadState,*/EGLDisplay aDisplay,EGLSurface aSurface,TSurfaceInfo& aSurfaceInfo)
{
	EGL_GET_EGL(EGL_FALSE);
/*
    EGL_GET_DISPLAY(aDisplay, EGL_NO_SURFACE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
    //RI_ASSERT( display->ProcessId() == process->Id() );
    RIEGLSurface* surface = display->getSurface( aSurface );
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_BAD_SURFACE);
 */
    RI_ASSERT( aSurfaceInfo.iSurfaceType == ESurfaceTypeWindow );
    
    //TODO: resizing?
    
#ifdef BUILD_WITH_PRIVATE_EGL
	if(!do_eglQuerySurface(aDisplay, aSurface, EGL_VG_ALPHA_FORMAT, &aSurfaceInfo.iAlphaFormat))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!do_eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_BUFFER_SIZE, &aSurfaceInfo.iColorBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!do_eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_RED_SIZE, &aSurfaceInfo.iRedBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!do_eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_GREEN_SIZE, &aSurfaceInfo.iGreenBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!do_eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_BLUE_SIZE, &aSurfaceInfo.iBlueBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!do_eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_ALPHA_SIZE, &aSurfaceInfo.iAlphaBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
#else
	if(!eglQuerySurface(aDisplay, aSurface, EGL_VG_ALPHA_FORMAT, &aSurfaceInfo.iAlphaFormat))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_BUFFER_SIZE, &aSurfaceInfo.iColorBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_RED_SIZE, &aSurfaceInfo.iRedBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_GREEN_SIZE, &aSurfaceInfo.iGreenBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_BLUE_SIZE, &aSurfaceInfo.iBlueBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	if(!eglGetConfigAttrib(aDisplay, aSurfaceInfo.iConfigId, EGL_ALPHA_SIZE, &aSurfaceInfo.iAlphaBits))
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
#endif

    
    EGL_TRACE("EGL::EglInternalFunction_CallSetSurfaceParams  Win surface details: width=%d height=%d colorbits=%d red=%d green=%d blue=%d alpha=%d alphaformat=0x%x",
    		aSurfaceInfo.iSize.iWidth, aSurfaceInfo.iSize.iHeight, aSurfaceInfo.iColorBits, aSurfaceInfo.iRedBits,
    		aSurfaceInfo.iGreenBits, aSurfaceInfo.iBlueBits, aSurfaceInfo.iAlphaBits, aSurfaceInfo.iAlphaFormat);
    TInt err;
    TSize size;

    TInt offsetToFirstBuffer = -1;
    TInt offsetToSecondBuffer = -1;
	err = aSurfaceInfo.iSurfaceManager.GetBufferOffset(aSurfaceInfo.iSurfaceId,0,offsetToFirstBuffer);
	if(err!=KErrNone)
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
	err = aSurfaceInfo.iSurfaceManager.GetBufferOffset(aSurfaceInfo.iSurfaceId,1,offsetToSecondBuffer);
	if(err!=KErrNone)
		EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE );
    EGL_TRACE("EGL::EglInternalFunction_CallSetSurfaceParams offsetToFirstBuffer=0x%x offsetToSecondBuffer=0x%x",
    		offsetToFirstBuffer,offsetToSecondBuffer);

   
    /* Store the pointer to the pixel data */
    aSurfaceInfo.iBuffer0 = aSurfaceInfo.iChunk.Base() + offsetToFirstBuffer;
    aSurfaceInfo.iBuffer1 = aSurfaceInfo.iChunk.Base() + offsetToSecondBuffer;
    
    aSurfaceInfo.iFrontBuffer = 0; // start rendering buffer 0 as a front buffer
    
    //TODO: what else? 
    EGL_RETURN(EGL_SUCCESS,EGL_TRUE);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TSurfaceInfo* EGL::EglInternalFunction_GetPlatformSurface( EGLDisplay display, EGLSurface surface )
{
	EGL_TRACE( "EGL::EglInternalFunction_GetPlatformSurface");
	TSurfaceInfo* result = NULL;

	iDisplayMapLock.ReadLock();

	CEglDisplayInfo** pDispInfo = iDisplayMap.Find( display );
	if (pDispInfo && *pDispInfo)
		{
		TSurfaceInfo** pSurfaceInfo = (*pDispInfo)->iSurfaceMap.Find( surface );
		if (pSurfaceInfo)
			{
			result = *pSurfaceInfo;
			}
		}

	// TODO on success should probably Unlock() the surface in the caller
	iDisplayMapLock.Unlock();

	/* TODO review calling code, to see if this suggestion makes sense
	if (result == NULL)
		{
		EGL_RAISE_ERROR( EGL_BAD_SURFACE, NULL); //Enable this when all surfaces are in surface map
		}
		*/
	return result;
}

EGLBoolean EGL::EglInternalFunction_SurfaceResized(/*TEglThreadState&,*/ TSurfaceInfo&, int, int) 
	{
	return EFalse; // stub code
	}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLSurface do_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
#else
EGLSurface eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display->configExists(config), EGL_BAD_CONFIG, EGL_NO_SURFACE);

	int width = 0, height = 0;
	bool largestPbuffer = false;
	int colorSpace = EGL_VG_COLORSPACE_sRGB;
	int alphaFormat = EGL_VG_ALPHA_FORMAT_NONPRE;
	if(attrib_list)
	{
		for(int i=0;attrib_list[i] != EGL_NONE;i+=2)
		{
			switch(attrib_list[i])
			{
			case EGL_WIDTH:
				width = attrib_list[i+1];
				break;

			case EGL_HEIGHT:
				height = attrib_list[i+1];
				break;

			case EGL_LARGEST_PBUFFER:
				largestPbuffer = attrib_list[i+1] ? true : false;
				break;

			case EGL_VG_COLORSPACE:
				colorSpace = attrib_list[i+1];
				break;

			case EGL_VG_ALPHA_FORMAT:
				alphaFormat = attrib_list[i+1];
				break;

			case EGL_TEXTURE_FORMAT:	//config doesn't support OpenGL ES
			case EGL_TEXTURE_TARGET:	//config doesn't support OpenGL ES
			case EGL_MIPMAP_TEXTURE:	//config doesn't support OpenGL ES
			default:
				EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);
			}
		}
	}
	EGL_IF_ERROR(width <= 0 || height <= 0, EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);

    Drawable* d = NULL;
	RIEGLSurface* s = NULL;
	try
	{
		d = RI_NEW(Drawable, (display->getConfig(config).configToDescriptor((colorSpace == EGL_VG_COLORSPACE_LINEAR) ? false : true, (alphaFormat == EGL_VG_ALPHA_FORMAT_PRE) ? true : false), width, height, display->getConfig(config).m_samples, display->getConfig(config).m_maskBits));	//throws bad_alloc
		RI_ASSERT(d);
		s = RI_NEW(RIEGLSurface,(NULL, config, d, largestPbuffer, EGL_BACK_BUFFER));	//throws bad_alloc
		RI_ASSERT(s);
		s->addReference();
		display->addSurface(s);	//throws bad_alloc
	}
	catch(std::bad_alloc)
	{
        RI_DELETE(d);
        RI_DELETE(s);
		EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
	}
	EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLSurface do_eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
#else
EGLSurface eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
	EGL_IF_ERROR(buftype != EGL_OPENVG_IMAGE, EGL_BAD_PARAMETER, EGL_NO_SURFACE);
	EGL_IF_ERROR(!buffer, EGL_BAD_PARAMETER, EGL_NO_SURFACE);	//TODO should also check if buffer really is a valid VGImage object (needs VG context for that)
    Image* image = (Image*)buffer;
	EGL_IF_ERROR(image->isInUse(), EGL_BAD_ACCESS, EGL_NO_SURFACE);	//buffer is in use by OpenVG
	EGL_IF_ERROR(!display->configExists(config), EGL_BAD_CONFIG, EGL_NO_SURFACE);
	EGL_IF_ERROR(attrib_list && attrib_list[0] != EGL_NONE, EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);	//there are no valid attribs for OpenVG
	const Color::Descriptor& bc = ((Image*)buffer)->getDescriptor();
	const Color::Descriptor& cc = display->getConfig(config).m_desc;
	EGL_IF_ERROR(bc.redBits != cc.redBits || bc.greenBits != cc.greenBits || bc.blueBits != cc.blueBits ||
				 bc.alphaBits != cc.alphaBits || bc.luminanceBits != cc.luminanceBits, EGL_BAD_MATCH, EGL_NO_SURFACE);

	//TODO If buffer is already bound to another pbuffer, an EGL BAD ACCESS error is generated.

    Drawable* d = NULL;
	RIEGLSurface* s = NULL;
	try
	{
		d = RI_NEW(Drawable, (image, display->getConfig(config).m_maskBits));
		RI_ASSERT(d);
		s = RI_NEW(RIEGLSurface,(NULL, config, d, false, EGL_BACK_BUFFER));	//throws bad_alloc
		RI_ASSERT(s);
		s->addReference();
		display->addSurface(s);	//throws bad_alloc
	}
	catch(std::bad_alloc)
	{
        RI_DELETE(d);
        RI_DELETE(s);
		EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
	}
	EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLSurface do_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
#else
EGLSurface eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
#endif
{
    EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
	EGL_IF_ERROR(!display->configExists(config), EGL_BAD_CONFIG, EGL_NO_SURFACE);
	//EGL_IF_ERROR(!pixmap || !isValidImageFormat(pixmap->format) || !pixmap->data || pixmap->width <= 0 || pixmap->height <= 0, EGL_BAD_NATIVE_PIXMAP, EGL_NO_SURFACE);

	RI_UNREF(attrib_list);
	EGL_IF_ERROR(display->getConfig(config).m_samples != 1, EGL_BAD_MATCH, EGL_NO_SURFACE);

	//TODO If there is already an EGLSurface associated with pixmap (as a result of a previous eglCreatePixmapSurface call), then a EGL BAD ALLOC error is generated.
	
	
	EGLint  width  = -1;
	EGLint  height = -1;
	EGLint  stride = -1;
	VGImageFormat format;
	int* data = NULL;
	EGLBoolean err = OSGetNativePixmapInfo(pixmap, &width, &height, &stride,&format, &data);
	
    Drawable* d = NULL;
	RIEGLSurface* s = NULL;
	try
	{
		d = RI_NEW(Drawable, (Color::formatToDescriptor(format), width, height, stride, (RIuint8*)data, display->getConfig(config).m_maskBits));	//throws bad_alloc
		RI_ASSERT(d);
		s = RI_NEW(RIEGLSurface,(NULL, config, d, false, EGL_BACK_BUFFER));	//throws bad_alloc
		RI_ASSERT(s);
		s->addReference();
		display->addSurface(s);	//throws bad_alloc
	}
	catch(std::bad_alloc)
	{
        RI_DELETE(d);
        RI_DELETE(s);
		EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
	}
	EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
	
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
#else
EGLBoolean eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->surfaceExists(surface), EGL_BAD_SURFACE, EGL_FALSE);

    display->removeSurface((RIEGLSurface*)surface);
	if(!((RIEGLSurface*)surface)->removeReference())
		RI_DELETE((RIEGLSurface*)surface);

	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
#else
EGLBoolean eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->surfaceExists(surface), EGL_BAD_SURFACE, EGL_FALSE);
	RI_UNREF(attribute);
	RI_UNREF(value);
	//do nothing
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
#else
EGLBoolean eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->surfaceExists(surface), EGL_BAD_SURFACE, EGL_FALSE);
	//TODO give an error if value is NULL?

	RIEGLSurface* s = (RIEGLSurface*)surface;
	switch(attribute)
	{
	case EGL_VG_ALPHA_FORMAT:
		*value = (s->getDrawable()->getDescriptor().isPremultiplied()) ? EGL_VG_ALPHA_FORMAT_PRE : EGL_VG_ALPHA_FORMAT_NONPRE;
		break;

	case EGL_VG_COLORSPACE:
		*value = (s->getDrawable()->getDescriptor().isNonlinear()) ? EGL_VG_COLORSPACE_sRGB : EGL_VG_COLORSPACE_LINEAR;
		break;

	case EGL_CONFIG_ID:
		*value = display->getConfig(s->getConfig()).m_configID;
		break;

	case EGL_HEIGHT:
		*value = s->getDrawable()->getHeight();
		break;

	case EGL_HORIZONTAL_RESOLUTION:
		*value = EGL_UNKNOWN;			//TODO Horizontal dot pitch
		break;

	case EGL_LARGEST_PBUFFER:
		if(!s->getOSWindowContext())
			*value = s->isLargestPbuffer() ? EGL_TRUE : EGL_FALSE;
		break;

	case EGL_MIPMAP_TEXTURE:
		if(!s->getOSWindowContext())
			*value = EGL_FALSE;
		break;

	case EGL_MIPMAP_LEVEL:
		if(!s->getOSWindowContext())
			*value = 0;
		break;

	case EGL_PIXEL_ASPECT_RATIO:
		*value = EGL_UNKNOWN;			//TODO Display aspect ratio
		break;

	case EGL_RENDER_BUFFER:
		*value = s->getRenderBuffer();
		break;

	case EGL_SWAP_BEHAVIOR:
		*value = EGL_BUFFER_PRESERVED;
		break;

	case EGL_TEXTURE_FORMAT:
		if(!s->getOSWindowContext())
			*value = EGL_NO_TEXTURE;
		break;

	case EGL_TEXTURE_TARGET:
		if(!s->getOSWindowContext())
			*value = EGL_NO_TEXTURE;
		break;

	case EGL_VERTICAL_RESOLUTION:
		*value = EGL_UNKNOWN;			//TODO Vertical dot pitch
		break;

	case EGL_WIDTH:
		*value = s->getDrawable()->getWidth();
		break;

	default:
		EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_FALSE);
	}
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLContext do_eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
#else
EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_NO_CONTEXT);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_CONTEXT);
	EGL_IF_ERROR(!display->configExists(config), EGL_BAD_CONFIG, EGL_NO_CONTEXT);
	RI_UNREF(attrib_list);

	RIEGLThread* thread = egl->getThread();
	if(!thread)
		EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_CONTEXT);

	//creation of OpenGL ES contexts is not allowed in this implementation
	if(thread->getBoundAPI() != EGL_OPENVG_API)
		EGL_RETURN(EGL_BAD_MATCH, EGL_NO_CONTEXT);

    OpenVGRI::VGContext* vgctx = NULL;
	RIEGLContext* c = NULL;
	try
	{
		vgctx = RI_NEW(OpenVGRI::VGContext, (share_context ? ((RIEGLContext*)share_context)->getVGContext() : NULL));	//throws bad_alloc
		c = RI_NEW(RIEGLContext, (vgctx, config));	//throws bad_alloc
		c->addReference();
		display->addContext(c);	//throws bad_alloc
	}
	catch(std::bad_alloc)
	{
        RI_DELETE(vgctx);
        RI_DELETE(c);
		EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_CONTEXT);
	}

	EGL_RETURN(EGL_SUCCESS, (EGLContext)c);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
#else
EGLBoolean eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->contextExists(ctx), EGL_BAD_CONTEXT, EGL_FALSE);

	RIEGLContext* context = (RIEGLContext*)ctx;
    display->removeContext(context);
	if(!context->removeReference() )
		RI_DELETE(context);

	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
#else
EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	// From guestEGL - eglMakeCurrent
	if (ctx == EGL_NO_CONTEXT)
	{
		// what now??
		EGL_TRACE("eglMakeCurrent context NO_CONTEXT");
		if ( (draw != EGL_NO_SURFACE) || (read != EGL_NO_SURFACE) )
		{
			EGL_RETURN(EGL_BAD_SURFACE, EGL_FALSE);
		}	
	}
	else
	{
		if ( (draw == EGL_NO_SURFACE) || (read == EGL_NO_SURFACE) )
		{
			EGL_RETURN(EGL_BAD_SURFACE, EGL_FALSE);
		
		}
		// ToDo use new CEglContext code
		const TInt KMaxSurfaces( 2 );
		EGLSurface surfaces[KMaxSurfaces];
		TSurfaceInfo* surfaceInfo[KMaxSurfaces] = {NULL, NULL};
		surfaces[0] = draw;
		if (draw != read)
		{
			surfaces[1] = read;
		}
		else
		{
			surfaces[1] = EGL_NO_SURFACE;
		}

		for ( TInt i = 0; i < KMaxSurfaces; i++ )
		{
			if ( EGL_NO_SURFACE != surfaces[i] )
			{
				EGL_TRACE("eglMakeCurrent check surface %d", surfaces[i] );
				surfaceInfo[i] = getEGL()->EglInternalFunction_GetPlatformSurface( dpy, surfaces[i] );
				EGL_TRACE("eglMakeCurrent surfaces[%d] is %x", i, surfaces[i]);
				//EGL_CHECK_ERROR( surfaceInfo, EGL_BAD_SURFACE , EGL_FALSE );
				if ( surfaceInfo[i] )
				{
					TSize newSize;
					switch (surfaceInfo[i]->iSurfaceType)
					{
						case ESurfaceTypePixmapFbsBitmap:
							EGLPANIC_ASSERT_DEBUG(surfaceInfo[i]->iFbsBitmap, EEglPanicTemp);
							newSize = surfaceInfo[i]->iFbsBitmap->SizeInPixels();
							break;
						case ESurfaceTypeWindow:
							EGLPANIC_ASSERT_DEBUG(surfaceInfo[i]->iNativeWindow, EEglPanicTemp);
							newSize = surfaceInfo[i]->iNativeWindow.SizeInPixels();
							break;
						default:
							// size cannot change for other surface types
							newSize = surfaceInfo[i]->iSize;
							break;
					}
					if (newSize != surfaceInfo[i]->iSize)
					{
						EGL_TRACE("eglMakeCurrent resize surface");
						if ( !getEGL()->EglInternalFunction_SurfaceResized(/*aThreadState,*/ *surfaceInfo[i], dpy, surfaces[i] ) )
						{
							return EGL_FALSE;
						}
						surfaceInfo[i]->iSize = newSize;
					}
				}
			}
		}

		// adapt to only some surfaces having CEglSurfaceInfo objects so far 
		EGLSurface drawId = surfaceInfo[0] ? surfaceInfo[0]->iSurface : draw;
		EGLSurface readId = read;
		if ((read == draw) && surfaceInfo[0])
		{
			readId = surfaceInfo[0]->iSurface;
		}
		else if (surfaceInfo[1])
		{
			readId = surfaceInfo[1]->iSurface;
		}

		EGL_TRACE("  eglMakeCurrent surfaces[0]=0x%x, surfaces[1]=0x%x", surfaces[0], surfaces[1]);
		EGL_TRACE("  eglMakeCurrent surfacesInfo[0]=0x%x, surfacesInfo[0].iSurface=0x%x",
					surfaceInfo[0], surfaceInfo[0] ? surfaceInfo[0]->iSurface : NULL);
		EGL_TRACE("  eglMakeCurrent surfacesInfo[1]=0x%x, surfacesInfo[1].iSurface=0x%x",
					surfaceInfo[1], surfaceInfo[1] ? surfaceInfo[1]->iSurface : NULL);

		EGL_TRACE("CGuestEGL::eglMakeCurrent call host");
		// now we need to use readId and drawId
		draw = drawId;
		read = readId;
	}
	// end from guestEGL
	/* following is original EGL... very much different to hostEGL!
	 * is that an issue?
	 * Works only with one buffer... draw only used from now on?
	 */
	EGL_IF_ERROR(ctx != EGL_NO_CONTEXT && !display->contextExists(ctx), EGL_BAD_CONTEXT, EGL_FALSE);
	EGL_IF_ERROR(draw != EGL_NO_SURFACE && !display->surfaceExists(draw), EGL_BAD_SURFACE, EGL_FALSE);
	EGL_IF_ERROR(read != EGL_NO_SURFACE && !display->surfaceExists(read), EGL_BAD_SURFACE, EGL_FALSE);
	EGL_IF_ERROR(draw != read, EGL_BAD_MATCH, EGL_FALSE);	//TODO what's the proper error code?
	EGL_IF_ERROR((draw != EGL_NO_SURFACE && ctx == EGL_NO_CONTEXT) || (draw == EGL_NO_SURFACE && ctx != EGL_NO_CONTEXT), EGL_BAD_MATCH, EGL_FALSE);

	RIEGLSurface* s = NULL;
	RIEGLContext* c = NULL;
	if(draw != EGL_NO_SURFACE && ctx != EGL_NO_CONTEXT)
	{
		EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);

		s = (RIEGLSurface*)draw;
		c = (RIEGLContext*)ctx;

        //If either draw or read are pbuffers created with eglCreatePbufferFromClientBuffer, and the underlying bound client API buffers
        //are in use by the client API that created them, an EGL BAD ACCESS error is generated.
		EGL_IF_ERROR(s->getDrawable()->isInUse(), EGL_BAD_ACCESS, EGL_FALSE);


		//TODO properly check compatibility of surface and context:
		//-both have RGB or LUMINANCE configs
		//-buffer bit depths match
		//-configs support OpenVG
		//-both have the same display
		EGL_IF_ERROR(s->getConfig() != c->getConfig(), EGL_BAD_MATCH, EGL_FALSE);
		//TODO check if context or surfaces are already bound to another thread

		//If a native window underlying either draw or read is no longer valid, an EGL BAD NATIVE WINDOW error is generated.
        EGL_IF_ERROR(s->getOSWindowContext() && !OSIsWindow(s->getOSWindowContext()), EGL_BAD_NATIVE_WINDOW, EGL_FALSE);

		//TODO If the previous context of the calling display has unflushed commands, and the previous surface is no longer valid, an EGL BAD CURRENT SURFACE error is generated. (can this happen?)
		//TODO If the ancillary buffers for draw and read cannot be allocated, an EGL BAD ALLOC error is generated. (mask buffer?)
	}

	//check if the thread is current
	RIEGLThread* thread = egl->getCurrentThread();
	if(thread)
	{	//thread is current, release the old bindinds and remove the thread from the current thread list
		RIEGLContext* pc = thread->getCurrentContext();
		RIEGLSurface* ps = thread->getCurrentSurface();
		if(pc)
		{
#ifdef BUILD_WITH_PRIVATE_OPENVG
			/*do_vg*/vgFlush();
#else
			vgFlush();
#endif
			pc->getVGContext()->setDefaultDrawable(NULL);
			if(!pc->removeReference())
				RI_DELETE(pc);
		}
		if(ps)
		{
			if(!ps->removeReference())
				RI_DELETE(ps);
		}

        egl->removeCurrentThread(thread);
	}

	if( c && s )
	{
		//bind context and surface to the current display
		RIEGLThread* newThread = egl->getThread();
		if(!newThread)
			EGL_RETURN(EGL_BAD_ALLOC, EGL_FALSE);
        newThread->makeCurrent(c, s);
		c->getVGContext()->setDefaultDrawable(s->getDrawable());

		try
		{
			egl->addCurrentThread(newThread);	//throws bad_alloc
		}
		catch(std::bad_alloc)
		{
			EGL_RETURN(EGL_BAD_ALLOC, EGL_FALSE);
		}

		c->addReference();
		s->addReference();
	}
	
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLContext do_eglGetCurrentContext()
#else
EGLContext eglGetCurrentContext()
#endif
{
    EGL_GET_EGL(EGL_NO_CONTEXT);
	EGLContext ret = EGL_NO_CONTEXT;
	RIEGLThread* thread = egl->getCurrentThread();
	if(thread && thread->getBoundAPI() == EGL_OPENVG_API)
    {
        ret = CastFromRIEGLContext(thread->getCurrentContext());
        RI_ASSERT(ret);
    }
	EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLSurface do_eglGetCurrentSurface(EGLint readdraw)
#else
EGLSurface eglGetCurrentSurface(EGLint readdraw)
#endif
{
    EGL_GET_EGL(EGL_NO_SURFACE);
	EGL_IF_ERROR(readdraw != EGL_READ && readdraw != EGL_DRAW, EGL_BAD_PARAMETER, EGL_NO_SURFACE);
	EGLContext ret = EGL_NO_SURFACE;
	RIEGLThread* thread = egl->getCurrentThread();
	if(thread && thread->getBoundAPI() == EGL_OPENVG_API)
    {
        ret = CastFromRIEGLSurface(thread->getCurrentSurface());
        RI_ASSERT(ret);
    }
	EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief	Returns the current display
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLDisplay do_eglGetCurrentDisplay(void)
#else
EGLDisplay eglGetCurrentDisplay(void)
#endif
{
    EGL_GET_EGL(EGL_NO_DISPLAY);

	RIEGLThread* thread = egl->getCurrentThread();
	if(!thread || thread->getBoundAPI() != EGL_OPENVG_API)
		EGL_RETURN(EGL_SUCCESS, EGL_NO_DISPLAY);

	RIEGLContext* ctx = thread->getCurrentContext();
    RI_ASSERT(ctx);
    EGLDisplay ret = egl->findDisplay(CastFromRIEGLContext(ctx));
	EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value)
#else
EGLBoolean eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->contextExists(ctx), EGL_BAD_CONTEXT, EGL_FALSE);
	EGL_IF_ERROR(attribute != EGL_CONFIG_ID && attribute != EGL_CONTEXT_CLIENT_TYPE, EGL_BAD_ATTRIBUTE, EGL_FALSE);
	if(attribute == EGL_CONFIG_ID)
		*value = display->getConfig(((RIEGLContext*)ctx)->getConfig()).m_configID;
	if(attribute == EGL_CONTEXT_CLIENT_TYPE)
		*value = EGL_OPENVG_API;
	// \todo [kalle 05/Jul/05] Handling of EGL_RENDER_BUFFER attribute is missing.
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglBindAPI(EGLenum api)
#else
EGLBoolean eglBindAPI(EGLenum api)
#endif
{
    EGL_GET_EGL(EGL_FALSE);
	EGL_IF_ERROR(api != EGL_OPENVG_API && api != EGL_OPENGL_ES_API, EGL_BAD_PARAMETER, EGL_FALSE);
	RIEGLThread* thread = egl->getThread();
	if(thread)
		thread->bindAPI(api);
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLenum do_eglQueryAPI(void)
#else
EGLenum eglQueryAPI(void)
#endif
{
    EGL_GET_EGL(EGL_NONE);
	RIEGLThread* thread = egl->getThread();
	if(thread)
		EGL_RETURN(EGL_SUCCESS, thread->getBoundAPI());
	EGL_RETURN(EGL_SUCCESS, EGL_NONE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglWaitClient()
#else
EGLBoolean eglWaitClient()
#endif
{
    EGL_GET_EGL(EGL_FALSE);
	RIEGLThread* thread = egl->getCurrentThread();
	if(thread && thread->getBoundAPI() == EGL_OPENVG_API)
#ifdef BUILD_WITH_PRIVATE_OPENVG
		/*do_vg*/vgFinish();
#else
		vgFinish();
#endif
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	Waits for OpenGL ES
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglWaitGL(void)
#else
EGLBoolean eglWaitGL(void)
#endif
{
	return EGL_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		We don't support native rendering
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglWaitNative(EGLint engine)
#else
EGLBoolean eglWaitNative(EGLint engine)
#endif
{
	RI_UNREF(engine);
	return EGL_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/
/*
 * guestEGL swap buffers
 * 

	 * PSEUDO CODE
	 * serialization.eglSwapBuffers
	 * (maybe finish currently bound api)
	 * surfaceupdatesession.notifywhenavailable
	 *   .whendisplayed()  (alternative choice from above)
	 * surfaceupdatesession.submitupdated()
	 * user:waitforrequestl
 */
#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
#else
EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->surfaceExists(surface), EGL_BAD_SURFACE, EGL_FALSE);

	
	TSurfaceInfo* surfaceInfo = egl->EglInternalFunction_GetPlatformSurface( dpy, surface );
	EGL_IF_ERROR(!surfaceInfo, EGL_BAD_SURFACE, EGL_FALSE);
	
	RIEGLSurface* s = (RIEGLSurface*)surface;

	RIEGLThread* currentThread = egl->getCurrentThread();
	EGL_IF_ERROR(!currentThread || currentThread->getCurrentSurface() != s, EGL_BAD_SURFACE, EGL_FALSE);
	EGL_IF_ERROR(!OSIsWindow(s->getOSWindowContext()), EGL_BAD_NATIVE_WINDOW, EGL_FALSE);

#ifdef BUILD_WITH_PRIVATE_OPENVG
	/*do_vg*/vgFlush();
#else
	vgFlush();
#endif
	
	EGLint currentApi = currentThread->getBoundAPI();
	RIEGLContext* context = currentThread->getCurrentContext();
	switch( currentApi )
	{
		case EGL_OPENVG_API:
	    {
			//if( thread->CurrentVGContext() != surface->BoundContext() )
			if( currentThread->getCurrentContext()->getVGContext() != s->getOSWindowContext() ) // getOSWindowContext??
			{
				//EGLI_LEAVE_RET( EGL_FALSE, EGL_BAD_SURFACE );
				EGL_TRACE("eglSwapBuffers EGL_BAD_SURFACE for EGL_OPENVG_API ");
				//EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE); // commnted out as the check against OSWindowContext seems wrong
			}
			break;
	    }
	    case EGL_OPENGL_ES_API:
	    {
			/*
	        if( thread->CurrentGLESContext() != surface->BoundContext() )
	        {
				 EGLI_LEAVE_RET( EGL_FALSE, EGL_BAD_SURFACE );
	        }
	        */
	        EGL_RETURN(EGL_BAD_SURFACE,EGL_FALSE);
	        // \todo other GLES stuff?
	     }
	}
	
	
	
	/*
	 * ToDo
	 * from hostegl
	 if( !(CEGLOs::IsValidNativeWindow(((CEGLWindowSurface*)surface)->NativeType())) )
	 */
	if(!s->getOSWindowContext())
	{	//do nothing for other than window surfaces (NOTE: single-buffered window surfaces should return immediately as well)
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
	}
	if( surfaceInfo->iSurfaceType != ESurfaceTypeWindow || s->getRenderBuffer() == EGL_SINGLE_BUFFER )
	{
		EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
	}
	
	int windowWidth = 0, windowHeight = 0;
    OSGetWindowSize(s->getOSWindowContext(), windowWidth, windowHeight);
    /*
    * TODO
    * what we need to do for resize to work?
    */
	if(windowWidth != s->getDrawable()->getWidth() || windowHeight != s->getDrawable()->getHeight())
	{	//resize the back buffer
		RIEGLContext* c = currentThread->getCurrentContext();
		RI_ASSERT(c);
		/* from hostEGL
		if( !(surface->Resize(w, h)) )
		{
			EGLI_LEAVE_RET( EGL_FALSE, EGL_BAD_ALLOC );
		}
		*/
		try
		{
			s->getDrawable()->resize(windowWidth, windowHeight);	//throws bad_alloc
		}
		catch(std::bad_alloc)
		{
			c->getVGContext()->setDefaultDrawable(NULL);
			EGL_RETURN(EGL_BAD_ALLOC, EGL_FALSE);
		}
		if( windowWidth <= 0 || windowHeight <= 0 )
		{
			// invisible window => do nothing
			EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
		}
		/*
		 * what is this for? From hostEgl...
		 
		if( surface->VGClientSurface() && state->VGInterface() )
		{
			if( !(state->VGInterface()->ResizeSurface(thread->CurrentVGContext()->ClientContext(),
					surface->VGClientSurface(), w, h, surface->VGBuffers())) )
		    {
				EGLI_LEAVE_RET( EGL_FALSE, EGL_BAD_ALLOC );
		    }
		}
		*/
	}
	
	
	//
	
	void* buf = NULL;
	if (surfaceInfo->iFrontBuffer == 0)
	{
		buf = surfaceInfo->iBuffer1;
	}
	else
	{
		buf = surfaceInfo->iBuffer0;
	}
	
	//
	switch( currentApi )
	{
		case EGL_OPENVG_API:
		{
			//VGImageFormat format = VG_sARGB_8888_PRE; // hack!!
			VGImageFormat format = VG_sARGB_8888; // hack!!
			//VGImageFormat format = VG_sRGBA_8888; // hack!!
			//
			vgReadPixels(buf,surfaceInfo->iStride,format,0,0,windowWidth,windowHeight);
		    break;
		}
		case EGL_OPENGL_ES_API:
		{
		/*
			if( surface->BoundContext() && !(state->GLESInterface(surface->BoundContext()->ClientVersion())) )
			{
				// \todo error code?
				EGLI_LEAVE_RET( EGL_FALSE, EGL_BAD_ACCESS );
			}
			*/
			EGL_RETURN(EGL_BAD_ACCESS, EGL_FALSE);
		}
	}
	//
    OSBlitToWindow(s->getOSWindowContext(), s->getDrawable());
    getEGL()->EglInternalFunction_SwapBuffers(dpy, surface);
    
	
    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

TBool EGL::EglInternalFunction_SwapBuffers(EGLDisplay aDisplay, EGLSurface aSurface)
{
	TSize size = iSurfaceInfo.iNativeWindow.SizeInPixels();
	
	TRequestStatus status;
	TTimeStamp timestampLocalToThread;
	// session needs to be established
	iSurfaceInfo.iSurfaceUpdateSession.NotifyWhenDisplayed(status, timestampLocalToThread);
	// what to do if native window is not RWindow???
	if (iSurfaceInfo.iFrontBuffer == 0)
	{
		iSurfaceInfo.iFrontBuffer = 1;
	}
	else
	{
		iSurfaceInfo.iFrontBuffer = 0;
	}
	iSurfaceInfo.iSurfaceUpdateSession.SubmitUpdate(iSurfaceInfo.iScreenNumber,iSurfaceInfo.iSurfaceId, iSurfaceInfo.iFrontBuffer);
	User::WaitForRequest(status);
	    
	if (size != iSurfaceInfo.iSize)
	{
		EGL_TRACE("EGL::EglInternalFunction_SwapBuffers Surface Resized size=%d,%d, iSurfaceInfo.iSize=%d,%d",
	    			size.iHeight, size.iWidth, iSurfaceInfo.iSize.iHeight, iSurfaceInfo.iSize.iWidth);
	    			
	    return EglInternalFunction_SurfaceResized(/*aThreadState,*/ iSurfaceInfo, aDisplay, aSurface); // TODO handling of resize
	}
	return EGL_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
#else
EGLBoolean eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	EGL_IF_ERROR(!display->surfaceExists(surface), EGL_BAD_SURFACE, EGL_FALSE);
	//EGL_IF_ERROR(!target || !isValidImageFormat(target->format) || !target->data || target->width <= 0 || target->height <= 0, EGL_BAD_NATIVE_PIXMAP, EGL_FALSE);
	EGLint  width  = -1;
	EGLint  height = -1;
	EGLint  stride = -1;
	VGImageFormat format;
	int* data = NULL;
	EGLBoolean err = OSGetNativePixmapInfo(target, &width, &height, &stride,&format, &data);
	
	TUint* fdata = (TUint*)((TUint)data + ( stride * ( height - 1  ) ) );	
	try
	{
		Image output(Color::formatToDescriptor(format), width, height, -stride, (RIuint8*)fdata);
        output.addReference();
		output.blit(((RIEGLSurface*)surface)->getDrawable()->getColorBuffer(), 0, 0, 0, 0, width, height);	//throws bad_alloc
        output.removeReference();
	}
	catch(std::bad_alloc)
	{
	}
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
	
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		We support only swap interval one
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglSwapInterval(EGLDisplay dpy, EGLint interval)
#else
EGLBoolean eglSwapInterval(EGLDisplay dpy, EGLint interval)
#endif
{
	EGL_GET_DISPLAY(dpy, EGL_FALSE);
	EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
	RI_UNREF(interval);
	EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/
/**
	Obtain extension function pointer.

	Provide support for the Symbian Graphics SHAI for EGL Extensions
	@param aName	Name of extension
	@return			Function pointer, or NULL if not available.

	@note  Known extensions and their directly associated functions:
	   1. EGL_KHR_image 
			None
	   2. EGL_KHR_image_base 
			eglCreateImageKHR,
			eglDestroyImageKHR
	   3. EGL_KHR_image_pixmap
			None
	   4. EGL_KHR_vg_parent_image
			None
	   5. EGL_KHR_gl_texture_2D_image
			None
	   6. EGL_KHR_gl_texture_cubemap_image
			EGLImageTargetTexture2DOES,
			EGLImageTargetRenderbufferStorageOES
	   7. EGL_KHR_lock_surface2
			eglLockSurfaceKHR,
			eglUnlockSurfaceKHR
	   8. EGL_NOK_resource_profiling2
			eglQueryProfilingDataNOK
	   9. EGL_SYMBIAN_composition
			None
	  10. EGL_NOK_image_endpoint
			eglCreateEndpointNOK,
			eglDestroyEndpointNOK,
			eglGetEndpointAttribNOK,
			eglSetEndpointAttribNOK,
			eglEndpointBeginStreamingNOK,
			eglEndpointEndStreamingNOK,
			eglAcquireImageNOK,
			eglReleaseImageNOK,
			eglGetEndpointDirtyAreaNOK,
			eglEndpointRequestNotificationNOK,
			eglEndpointCancelNotificationNOK,
			eglCreateEndpointNOK
	  11. NOK_pixmap_type_rsgimage
			None
	  12. EGL_NOK_swap_region
			eglSwapBuffersRegionNOK

	@see Khronos EGL 1.4 Specification, eglGetProcAddress()
	@author Faisal Memon Community EGL project
	ported to Merge EGL
	*/
typedef void RI_Proc();

//EGLAPI void (* EGLAPIENTRY      eglGetProcAddress(const char *procname))(void);

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY void (*do_eglGetProcAddress(const char *aName))(...)
#else
void (*eglGetProcAddress(const char *aName))(...)
#endif
{
	/*
	if(!procname)
		return NULL;
	return NULL;
	*/
	
/*ProcPointer CGuestEGL::eglGetProcAddress(const char* aName)*/
	if (strncmp("eglCreateImageKHR", aName, strlen("eglCreateImageKHR")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglDestroyImageKHR", aName, strlen("eglDestroyImageKHR")) == 0)
	{
		return NULL;
	}
	else if (strncmp("EGLImageTargetTexture2DOES", aName, strlen("EGLImageTargetTexture2DOES")) == 0)
	{
		return NULL;
	}
	else if (strncmp("EGLImageTargetRenderbufferStorageOES", aName, strlen("EGLImageTargetRenderbufferStorageOES")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglLockSurfaceKHR", aName, strlen("eglLockSurfaceKHR")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglUnlockSurfaceKHR", aName, strlen("eglUnlockSurfaceKHR")) == 0)
	{
		return NULL;
	}
	else if (strncmp("EGL_KHR_lock_surface", aName, strlen("EGL_KHR_lock_surface")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglQueryProfilingDataNOK", aName, strlen("eglQueryProfilingDataNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglCreateEndpointNOK", aName, strlen("eglCreateEndpointNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglDestroyEndpointNOK", aName, strlen("eglDestroyEndpointNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglGetEndpointAttribNOK", aName, strlen("eglGetEndpointAttribNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglEndpointBeginStreamingNOK", aName, strlen("eglEndpointBeginStreamingNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglEndpointEndStreamingNOK", aName, strlen("eglEndpointEndStreamingNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglAcquireImageNOK", aName, strlen("eglAcquireImageNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglReleaseImageNOK", aName, strlen("eglReleaseImageNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglGetEndpointDirtyAreaNOK", aName, strlen("eglGetEndpointDirtyAreaNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglEndpointRequestNotificationNOK", aName, strlen("eglEndpointRequestNotificationNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglEndpointCancelNotificationNOK", aName, strlen("eglEndpointCancelNotificationNOK")) == 0)
	{
		return NULL;
	}
	else if (strncmp("eglCreateEndpointNOK", aName, strlen("eglCreateEndpointNOK")) == 0)
	{
	    return NULL;
	}
	else if (strncmp("eglSwapBuffersRegionNOK", aName, strlen("eglSwapBuffersRegionNOK")) == 0)
	{
	    return NULL;
	}
	else if (strncmp("egl_Private_SignalSyncNOK", aName, strlen("egl_Private_SignalSyncNOK")) == 0)
	{
		return (ProcPointer)CEglSyncExtension::egl_Private_SignalSyncNOK;
	}
	else if (strncmp("eglCreateSyncKHR", aName, strlen("eglCreateSyncKHR")) == 0)
	{
		return (ProcPointer)CEglSyncExtension::eglCreateSyncKHR;
	}
	else if (strncmp("eglDestroySyncKHR", aName, strlen("eglDestroySyncKHR")) == 0)
	{
		return (ProcPointer)CEglSyncExtension::eglDestroySyncKHR;
	}
	else if (strncmp("eglClientWaitSyncKHR", aName, strlen("eglClientWaitSyncKHR")) == 0)
	{
		return (ProcPointer)CEglSyncExtension::eglClientWaitSyncKHR;
	}
	else if (strncmp("eglSignalSyncKHR", aName, strlen("eglSignalSyncKHR")) == 0)
	{
		return (ProcPointer)CEglSyncExtension::eglSignalSyncKHR;
	}
	else if (strncmp("eglGetSyncAttribKHR", aName, strlen("eglGetSyncAttribKHR")) == 0)
	{
		return (ProcPointer)CEglSyncExtension::eglGetSyncAttribKHR;
	}
	else
	{
		return NULL;
	}
		
}



/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglReleaseThread(void)
#else
EGLBoolean eglReleaseThread(void)
#endif
{
    EGL_GET_EGL(EGL_FALSE);

	//check if the thread is current
	RIEGLThread* thread = egl->getCurrentThread();
	if(thread)
	{	//thread is current, release the old bindings and remove the thread from the current thread list
		RIEGLContext* pc = thread->getCurrentContext();
		RIEGLSurface* ps = thread->getCurrentSurface();
		if(pc)
		{
#ifdef BUILD_WITH_PRIVATE_OPENVG
			/*do_vg*/vgFlush();
#else
			vgFlush();
#endif
			pc->getVGContext()->setDefaultDrawable(NULL);
			if(!pc->removeReference())
				RI_DELETE(pc);
		}
		if(ps)
		{
			if(!ps->removeReference())
				RI_DELETE(ps);
		}

        egl->removeCurrentThread(thread);
	}

    //destroy EGL's thread struct
    egl->destroyThread();

	//destroy the EGL instance
	releaseEGL();

	OSReleaseMutex();
    OSDeinitMutex();

	return EGL_SUCCESS;
}

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
#else
EGLBoolean eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
#endif
	{
	//not implemented
	RI_ASSERT(0);
	return false;
	}

#ifdef BUILD_WITH_PRIVATE_EGL
RI_APIENTRY EGLBoolean do_eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
#else 
EGLBoolean eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
#endif
	{
	//not implemented
	RI_ASSERT(0);
	return false;
	}
#undef EGL_NUMCONFIGS




