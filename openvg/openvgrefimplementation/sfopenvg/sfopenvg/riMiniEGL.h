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
#ifndef __MINI_EGL_H_
#define __MINI_EGL_H_

#include "egl.h"
#include "openvg.h"
#include "riArray.h"
#include "riMath.h"
#include "riContext.h"
#include "riImage.h"

#include "eglosnativewindowtype.h"

#ifdef BUILD_WITH_PRIVATE_EGL 
#include "eglinternal.h"
#endif

#ifdef BUILD_WITH_PRIVATE_OPENVG
#include "openvginternal.h"
#endif

#ifdef EGL_COMPOSITION
// Surface Manager and Update
#include <w32std.h>
#include <graphics/surfacemanager.h>
#include <graphics/surfaceconfiguration.h>
#include <graphics/suerror.h>
#include <graphics/surface_hints.h>
#include <graphics/surfaceupdateclient.h>
#include <e32hashtab.h>
#endif


//==============================================================================================
// EGL header
typedef enum
	{
	ESurfaceTypeWindow = 0,
	ESurfaceTypePbuffer = 1,
	ESurfaceTypePixmapFbsBitmap = 2,
	ESurfaceTypePixmapSgImage = 3,
	} TEglSurfaceType;

	
class CEglContext;

// client side info about a surface
struct TSurfaceInfo
    {
	//TSurfaceInfo& operator=( TSurfaceInfo& aRhs );
    TSize                   iSize;
    TEglSurfaceType         iSurfaceType; //Surface type
    EGLConfig               iConfigId;
    CFbsBitmap*             iFbsBitmap;       // Handle for CFbsBitmap

    EGLSurface              iSurface; 			// insted of HostSurfaceId
	// Note: most member data is only used by Window surfaces
    TSurfaceId              iSurfaceId;        /*<! Target system surface allocated by EGL.                                              */
    //
    RSurfaceManager			iSurfaceManager;
    RSurfaceUpdateSession	iSurfaceUpdateSession;
    void*                   iBuffer0;          /*<! Pointer to the first buffer pixels                                                   */
    void*                   iBuffer1;          /*<! Pointer to the second buffer pixels                                                  */
    //TInt                    iBuffer0Index;          /*<! Pointer to the first buffer pixels                                                   */
    //TInt                    iBuffer1Index;          /*<! Pointer to the second buffer pixels                                                  */
    RChunk                  iChunk;            /*<! chunk of the backbuffer surface memory                                               */
    TInt					iFrontBuffer;		// We assume zero based index, 0 -> iBuffer0, 1 -> iBuffer1 should be display
    TNativeWindowType		iNativeWindow;		// TNativeWindowType used now instead of RWindow directly
    TInt					iScreenNumber; 		// either provided by RWindow or default screen(0)
    EGLint                  iRedBits;
    EGLint                  iGreenBits;
    EGLint                  iBlueBits;
    EGLint                  iAlphaBits;
    EGLenum                 iAlphaFormat;
    EGLint                  iColorBits;
    EGLint                  iStride;
    //
    };


// Container for Symbian client side information about an EGL Display
NONSHARABLE_CLASS(CEglDisplayInfo) : public CBase
	{
public:
	TBool                              iInitialized;
//	RMutex                             iSurfaceMapLock;
    RHashMap<TInt, TSurfaceInfo*>      iSurfaceMap;
//private:
//	RMutex                             iContextMapLock;
    RHashMap<EGLContext, CEglContext*> iContextMap;
    };

// prototype to help eglGetProcAddress() API 
typedef void (*ProcPointer)(...);
//==============================================================================================

namespace OpenVGRI
{

void* OSGetCurrentThreadID(void);
void OSAcquireMutex(void);
void OSReleaseMutex(void);
void OSDeinitMutex(void);

EGLDisplay OSGetDisplay(EGLNativeDisplayType display_id);
void* OSCreateWindowContext(EGLNativeWindowType window);
void OSDestroyWindowContext(void* context);
bool OSIsWindow(const void* context);
void OSGetWindowSize(const void* context, int& width, int& height);
void OSBlitToWindow(void* context, const Drawable* drawable);
EGLBoolean OSGetNativePixmapInfo(NativePixmapType pixmap, int* width, int* height, int* stride, VGImageFormat* format, int** data);


/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

class RIEGLConfig
{
public:
	RIEGLConfig() : m_desc(Color::formatToDescriptor(VG_sRGBA_8888)), m_configID(0)	{} //why VG_sRGBA_8888 and not VG_sARGB_8888 by default?
	~RIEGLConfig()							{}
	void		set(int r, int g, int b, int a, int l, int bpp, int samples, int maskBits, int ID)
	{
		m_desc.redBits = r;
		m_desc.greenBits = g;
		m_desc.blueBits = b;
		m_desc.alphaBits = a;
		m_desc.luminanceBits = l;
		m_desc.alphaShift = 0;
		m_desc.luminanceShift = 0;
		m_desc.blueShift = b ? a : 0;
		m_desc.greenShift = g ? a + b : 0;
		m_desc.redShift = r ? a + b + g : 0;
		m_desc.format = (VGImageFormat)-1;
		m_desc.internalFormat = l ? Color::sLA : Color::sRGBA;
		m_desc.bitsPerPixel = bpp;
		RI_ASSERT(Color::isValidDescriptor(m_desc));
		m_samples = samples;
        m_maskBits = maskBits;
		m_configID = ID;
        m_config = (EGLConfig)ID;
	}

    Color::Descriptor configToDescriptor(bool sRGB, bool premultiplied) const
    {
        Color::Descriptor desc = m_desc;
        unsigned int f = m_desc.luminanceBits ? Color::LUMINANCE : 0;
        f |= sRGB ? Color::NONLINEAR : 0;
        f |= premultiplied ? Color::PREMULTIPLIED : 0;
        desc.internalFormat = (Color::InternalFormat)f;
        return desc;
    }

	//EGL RED SIZE bits of Red in the color buffer
	//EGL GREEN SIZE bits of Green in the color buffer
	//EGL BLUE SIZE bits of Blue in the color buffer
	//EGL ALPHA SIZE bits of Alpha in the color buffer
	//EGL LUMINANCE SIZE bits of Luminance in the color buffer
	Color::Descriptor	m_desc;
	int					m_samples;
    int                 m_maskBits;
	EGLint				m_configID;			//EGL CONFIG ID unique EGLConfig identifier
    EGLConfig           m_config;
	//EGL BUFFER SIZE depth of the color buffer (sum of channel bits)
	//EGL ALPHA MASK SIZE number alpha mask bits (always 8)
	//EGL BIND TO TEXTURE RGB boolean True if bindable to RGB textures. (always EGL_FALSE)
	//EGL BIND TO TEXTURE RGBA boolean True if bindable to RGBA textures. (always EGL_FALSE)
	//EGL COLOR BUFFER TYPE enum color buffer type (EGL_RGB_BUFFER, EGL_LUMINANCE_BUFFER)
	//EGL CONFIG CAVEAT enum any caveats for the configuration (always EGL_NONE)
	//EGL DEPTH SIZE integer bits of Z in the depth buffer (always 0)
	//EGL LEVEL integer frame buffer level (always 0)
	//EGL MAX PBUFFER WIDTH integer maximum width of pbuffer (always INT_MAX)
	//EGL MAX PBUFFER HEIGHT integer maximum height of pbuffer (always INT_MAX)
	//EGL MAX PBUFFER PIXELS integer maximum size of pbuffer (always INT_MAX)
	//EGL MAX SWAP INTERVAL integer maximum swap interval (always 1)
	//EGL MIN SWAP INTERVAL integer minimum swap interval (always 1)
	//EGL NATIVE RENDERABLE boolean EGL TRUE if native rendering APIs can render to surface (always EGL_FALSE)
	//EGL NATIVE VISUAL ID integer handle of corresponding native visual (always 0)
	//EGL NATIVE VISUAL TYPE integer native visual type of the associated visual (always EGL_NONE)
	//EGL RENDERABLE TYPE bitmask which client rendering APIs are supported. (always EGL_OPENVG_BIT)
	//EGL SAMPLE BUFFERS integer number of multisample buffers (always 0)
	//EGL SAMPLES integer number of samples per pixel (always 0)
	//EGL STENCIL SIZE integer bits of Stencil in the stencil buffer (always 0)
	//EGL SURFACE TYPE bitmask which types of EGL surfaces are supported. (always EGL WINDOW BIT | EGL PIXMAP BIT | EGL PBUFFER BIT)
	//EGL TRANSPARENT TYPE enum type of transparency supported (always EGL_NONE)
	//EGL TRANSPARENT RED VALUE integer transparent red value (undefined)
	//EGL TRANSPARENT GREEN VALUE integer transparent green value (undefined)
	//EGL TRANSPARENT BLUE VALUE integer transparent blue value (undefined)
};

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

class RIEGLContext
{
public:
	RIEGLContext(OpenVGRI::VGContext* vgctx, const EGLConfig config);
	~RIEGLContext();
	void	addReference()				{ m_referenceCount++; }
	int		removeReference()			{ m_referenceCount--; RI_ASSERT(m_referenceCount >= 0); return m_referenceCount; }

    VGContext*      getVGContext() const      { return m_vgContext; }
    EGLConfig getConfig() const         { return m_config; }
private:
	RIEGLContext(const RIEGLContext&);
	RIEGLContext& operator=(const RIEGLContext&);
	VGContext*		m_vgContext;
	const EGLConfig	m_config;
	int				m_referenceCount;
};

RIEGLContext* CastToRIEGLContext(EGLContext aCtxId);
EGLContext CastFromRIEGLContext(RIEGLContext* aCtx);



/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

class RIEGLSurface
{
public:
    RIEGLSurface(void* OSWindowContext, const EGLConfig config, Drawable* drawable, bool largestPbuffer, int renderBuffer);
	~RIEGLSurface();
	void	addReference()				{ m_referenceCount++; }
	int		removeReference()			{ m_referenceCount--; RI_ASSERT(m_referenceCount >= 0); return m_referenceCount; }

    void*           getOSWindowContext() const { return m_OSWindowContext; }
    EGLConfig       getConfig() const          { return m_config; }
    Drawable*       getDrawable() const        { return m_drawable; }
    bool            isLargestPbuffer() const   { return m_largestPbuffer; }
    int             getRenderBuffer() const    { return m_renderBuffer; }
    
    void 			Lock()						{iSurfaceLock.ReadLock();}
    void 			Unlock()					{iSurfaceLock.Unlock();}

private:
	RIEGLSurface(const RIEGLSurface&);
	RIEGLSurface& operator=(const RIEGLSurface&);
    void*            m_OSWindowContext;
	const EGLConfig	 m_config;
	Drawable*        m_drawable;
	bool			 m_largestPbuffer;
	int				 m_renderBuffer;		//EGL_BACK_BUFFER or EGL_SINGLE_BUFFER
	int				 m_referenceCount;
	
	RReadWriteLock	 iSurfaceLock; 			// protects the surface for multi-threaded clients
};

RIEGLSurface* CastToRIEGLSurface(EGLSurface aSurfaceId);
EGLSurface CastFromRIEGLSurface(RIEGLSurface* aSurface);



/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#define EGL_NUMCONFIGS		60

class RIEGLDisplay
{
public:
	RIEGLDisplay(EGLDisplay id);
	~RIEGLDisplay();

	int                getNumConfigs() const              { return EGL_NUMCONFIGS; }
    const RIEGLConfig& getConfigByIdx(int i) const             { RI_ASSERT(i >= 0 && i < EGL_NUMCONFIGS); return m_configs[i]; }
    const RIEGLConfig& getConfig(const EGLConfig config) const        { for(int i=0;i<EGL_NUMCONFIGS;i++) { if(m_configs[i].m_config == config) return m_configs[i]; } RI_ASSERT(0); return m_configs[0]; }

    EGLDisplay        getID() const                       { return m_id; }

    void              addContext(RIEGLContext* ctx)       { RI_ASSERT(ctx); m_contexts.push_back(ctx); }  //throws bad_alloc
    void              removeContext(RIEGLContext* ctx)    { RI_ASSERT(ctx); bool res = m_contexts.remove(ctx); RI_ASSERT(res); RI_UNREF(res); }

    void              addSurface(RIEGLSurface* srf)       { RI_ASSERT(srf); m_surfaces.push_back(srf); }  //throws bad_alloc
    void              removeSurface(RIEGLSurface* srf)    { RI_ASSERT(srf); bool res = m_surfaces.remove(srf); RI_ASSERT(res); RI_UNREF(res); }

    EGLBoolean        contextExists(const EGLContext ctx) const;
    EGLBoolean        surfaceExists(const EGLSurface srf) const;
    EGLBoolean        configExists(const EGLConfig cfg) const;

    RIEGLSurface* 	  getSurface(const EGLSurface surf) const;
private:
	RIEGLDisplay(const RIEGLDisplay& t);
	RIEGLDisplay& operator=(const RIEGLDisplay&t);

	EGLDisplay              m_id;

	Array<RIEGLContext*>	m_contexts;
	Array<RIEGLSurface*>	m_surfaces;

	RIEGLConfig             m_configs[EGL_NUMCONFIGS];
};



/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

class RIEGLThread
{
public:
	RIEGLThread(void* currentThreadID);
	~RIEGLThread();

    void*           getThreadID() const       { return m_threadID; }

    void            makeCurrent(RIEGLContext* c, RIEGLSurface* s)       { m_context = c; m_surface = s; }
	RIEGLContext*	getCurrentContext() const   { return m_context; }
	RIEGLSurface*	getCurrentSurface() const   { return m_surface; }

    void            setError(EGLint error)      { m_error = error; }
    EGLint          getError() const            { return m_error; }

    void            bindAPI(EGLint api)         { m_boundAPI = api; }
    EGLint          getBoundAPI() const         { return m_boundAPI; }

private:
	RIEGLThread(const RIEGLThread&);
	RIEGLThread operator=(const RIEGLThread&);

	RIEGLContext*		m_context;
	RIEGLSurface*		m_surface;
	EGLint              m_error;
	void*               m_threadID;
	EGLint              m_boundAPI;
};


Image* CastToImage(EGLClientBuffer aBufferId);
EGLClientBuffer CastFromImage(Image* aBUffer);



/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

class EGL
{
public:
	EGL();
	~EGL();

	void	addReference()				{ m_referenceCount++; }
	int		removeReference()			{ m_referenceCount--; RI_ASSERT(m_referenceCount >= 0); return m_referenceCount; }

    void                addDisplay(RIEGLDisplay* display)           { RI_ASSERT(display); m_displays.push_back(display); }  //throws bad alloc
    void                removeDisplay(RIEGLDisplay* display)        { RI_ASSERT(display); bool res = m_displays.remove(display); RI_ASSERT(res); RI_UNREF(res); }
    RIEGLDisplay*       getDisplay(const EGLDisplay displayID);
    EGLDisplay          findDisplay(const EGLContext ctx) const;

    void                addCurrentThread(RIEGLThread* thread)       { RI_ASSERT(thread); m_currentThreads.push_back(thread); }  //throws bad alloc
    void                removeCurrentThread(RIEGLThread* thread)    { RI_ASSERT(thread); bool res = m_currentThreads.remove(thread); RI_ASSERT(res); RI_UNREF(res); }
    RIEGLThread*        getCurrentThread() const;

    RIEGLThread*        getThread();
    void                destroyThread();

    bool                isInUse(const void* image) const;
    
    // Following functions ported from guestEGL
    TBool 				CreateDisplayInfo(const EGLDisplay aDisplayID);
	TBool 				InitializeDisplayInfo(EGLDisplay aDisplay);
	TBool 				IsDisplayInitialized(EGLDisplay aDisplay);
	TBool 				DestroyDisplayInfo(EGLDisplay aDisplay);
	
    // API supporting EGL sync extension
    EGLint FindAndLockDisplay(EGLDisplay aDisplayID);
    void ReleaseDisplayLock(EGLDisplay aDisplayID);
    EGLint InitialiseExtensions();
    CEglSyncExtension* 	EGLSyncExtension() {return iEglSyncExtension;};
    //
    TSurfaceInfo&	GetSurfaceInfo() {return iSurfaceInfo;};
// Following should be really private... if there were 'external' equivalents defined in EGL class...
//private:
public:
    TBool EglInternalFunction_CreateSurface(EGLDisplay aDisplay, EGLSurface aSurface, EGLConfig aConfig, EGLNativeWindowType aNativeWindow,
    		TSurfaceInfo& aSurfaceInfo);
    TBool EglInternalFunction_CallSetSurfaceParams(EGLDisplay aDisplay, EGLSurface aSurface,TSurfaceInfo& aSurfaceInfo);
    TBool EglInternalFunction_SwapBuffers(EGLDisplay aDisplay, EGLSurface aSurface);
    TSurfaceInfo* EglInternalFunction_GetPlatformSurface( EGLDisplay display, EGLSurface surface );
    EGLBoolean EglInternalFunction_SurfaceResized(TSurfaceInfo&, int, int);
private:
    friend EGL* getEGL();
	EGL(const EGL&);						// Not allowed.
	const EGL& operator=(const EGL&);		// Not allowed.
	void Create();
	
	Array<RIEGLThread*>		m_threads;			//threads that have called EGL
	Array<RIEGLThread*>		m_currentThreads;	//threads that have a bound context
	Array<RIEGLDisplay*>	m_displays;

	int                     m_referenceCount;
	
	//data members supporting EGL Sync extension
	CEglSyncExtension* 		iEglSyncExtension;
	TSurfaceInfo 			iSurfaceInfo;
	// public for now
public:
	RReadWriteLock			iDisplayMapLock; // protects the iDisplayMap for multi-threaded clients
	RHashMap<TInt, CEglDisplayInfo*> iDisplayMap;
};

// Note: egl error handling model differs from OpenVG. The latest error is stored instead of the oldest one.
static void eglSetError(EGL* egl, EGLint error);

EGL* getEGL();

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

#define EGL_GET_DISPLAY(DISPLAY, RETVAL) \
	OSAcquireMutex(); \
	EGL* egl = OpenVGRI::getEGL(); \
    if(!egl) \
    { \
		OSReleaseMutex(); \
		return RETVAL; \
    } \
	RIEGLDisplay* display = egl->getDisplay(DISPLAY); \

#define EGL_GET_EGL(RETVAL) \
	OSAcquireMutex(); \
	EGL* egl = getEGL(); \
    if(!egl) \
    { \
		OSReleaseMutex(); \
		return RETVAL; \
    } \

#define EGL_IF_ERROR(COND, ERRORCODE, RETVAL) \
	if(COND) { OpenVGRI::eglSetError(egl, ERRORCODE); OSReleaseMutex(); return RETVAL; } \

#define EGL_RETURN(ERRORCODE, RETVAL) \
	{ \
		OpenVGRI::eglSetError(egl, ERRORCODE); \
		OSReleaseMutex(); \
		return RETVAL; \
	}

/*-------------------------------------------------------------------*//*!
* \brief	Returns the OpenVG context current to the calling thread.
* \param	
* \return	
* \note		This function is always called from a mutexed API function
*//*-------------------------------------------------------------------*/

void* eglvgGetCurrentVGContext(void);

/*-------------------------------------------------------------------*//*!
* \brief	Check if the image is current in any of the displays
* \param	
* \return	
* \note		This function is always called from a mutexed API function
*//*-------------------------------------------------------------------*/

bool eglvgIsInUse(void* image);


//==============================================================================================

}	//namespace OpenVGRI

#endif //__MINI_EGL_H_
