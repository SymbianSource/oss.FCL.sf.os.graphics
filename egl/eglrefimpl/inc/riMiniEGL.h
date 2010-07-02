#ifndef __RIMINIEGL_H
#define __RIMINIEGL_H

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
#include "egl.h"
#include "riContext.h"

//#ifdef BUILD_WITH_PRIVATE_EGL 
#include "eglinternal.h"
//#endif

//#ifdef BUILD_WITH_PRIVATE_OPENVG
#include "openvginternal.h"
//#endif

//==============================================================================================

namespace OpenVGRI
{
class RIEGLSurface;
class RIEGLContext;
class RIEGLThread;

/*-------------------------------------------------------------------*//*!
* \brief	
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

class RIEGLConfig
{
public:
	RIEGLConfig() : m_desc(Color::formatToDescriptor(VG_sRGBA_8888)), m_configID(0)	{}
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

class EGL
{
public:
	EGL();
	~EGL();

	void	addReference()				{ m_referenceCount++; }
	int		removeReference()			{ m_referenceCount--; RI_ASSERT(m_referenceCount >= 0); return m_referenceCount; }

    void                addDisplay(RIEGLDisplay* display)           { RI_ASSERT(display); m_displays.push_back(display); }  //throws bad alloc
    void                removeDisplay(RIEGLDisplay* display)        { RI_ASSERT(display); bool res = m_displays.remove(display); RI_ASSERT(res); RI_UNREF(res); }
    RIEGLDisplay*       getDisplay(const EGLDisplay displayID) const;
    EGLDisplay          findDisplay(const EGLContext ctx) const;

    void                addCurrentThread(RIEGLThread* thread)       { RI_ASSERT(thread); m_currentThreads.push_back(thread); }  //throws bad alloc
    void                removeCurrentThread(RIEGLThread* thread)    { RI_ASSERT(thread); bool res = m_currentThreads.remove(thread); RI_ASSERT(res); RI_UNREF(res); }
    RIEGLThread*        getCurrentThread() const;

    RIEGLThread*        getThread();
    void                destroyThread();

    bool                isInUse(const void* image) const;

private:
	EGL(const EGL&);						// Not allowed.
	const EGL& operator=(const EGL&);		// Not allowed.

	Array<RIEGLThread*>		m_threads;			//threads that have called EGL
	Array<RIEGLThread*>		m_currentThreads;	//threads that have a bound context
	Array<RIEGLDisplay*>	m_displays;

	int                     m_referenceCount;
};

void* eglvgGetCurrentVGContext(void);
bool  eglvgIsInUse(void* image);

} //end of namespace

#endif /* __RIMINIEGL_H */


