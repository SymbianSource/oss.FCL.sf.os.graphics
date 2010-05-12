// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Reference EGL implementation to support EGL sync objects and OpenWF extensions

#include "eglprivate.h"
#include <e32debug.h>

// Helper macros
//
#define GET_THREAD_SESSION(s,r) CEglThreadSession* s = CEglThreadSession::Static(); \
                                if (!s) \
                                    { \
                                    return r; \
                                    }

#define PANIC_NOT_SUPPORTED     RDebug::Printf("EGL Panic %s %s:%d\n",__func__,__FILE__,__LINE__);User::Panic(KEglPanicCategory, EEglPanicNotSupported); \
                                return 0;



// EGL API entrypoints
//
EXPORT_C EGLDisplay eglGetDisplay(NativeDisplayType display_id)
	{
	GET_THREAD_SESSION(session, EGL_NO_DISPLAY)
	return session->EglGetDisplay(display_id);
	}

EXPORT_C EGLBoolean eglInitialize(EGLDisplay dpy, EGLint* major, EGLint* minor)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglInitialize(dpy, major, minor);
	}

EXPORT_C EGLBoolean eglTerminate(EGLDisplay dpy)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglTerminate(dpy);
	}

EXPORT_C EGLBoolean eglReleaseThread(void)
	{
	// do not call CEglThreadSession::Static() here as it may create thread state if it does not exist
	//
	CEglThreadSession* session = reinterpret_cast<CEglThreadSession*>(Dll::Tls());
	if (session)
		{
		delete session;
		Dll::FreeTls();
		}

	return EGL_TRUE;
	}

EXPORT_C EGLint eglGetError(void)
	{
	// EGL spec section 3.11: GetError will create thread state object, 
	// it is recommended not to call this API immediately after calling ReleaseThread
	// for that reason.
	// If session creation fails, we returns BAD_ALLOC
	//
	GET_THREAD_SESSION(session, EGL_BAD_ALLOC)
	return session->EglGetError();
	}

EXPORT_C const char* eglQueryString(EGLDisplay dpy, EGLint name)
	{
	GET_THREAD_SESSION(session, NULL)
	return session->EglQueryString(dpy, name);
	}

typedef void (*eglfPtr)(...);
EXPORT_C TFuncPtrEglProc eglGetProcAddress (const char *procname)
	{
	GET_THREAD_SESSION(session, NULL)
	return session->EglGetProcAddress(procname);
	
	//eglfPtr pname = do_eglGetProcAddress(procname);
	//return pname;
	}

// Unsupported EGL APIs
//
EXPORT_C EGLBoolean eglGetConfigs(EGLDisplay dpy, EGLConfig *configs,
			 EGLint config_size, EGLint *num_config)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglGetConfigs(dpy, configs,config_size, num_config);
	}

EXPORT_C EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
			        EGLConfig *configs, EGLint config_size,EGLint *num_config)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglChooseConfig(dpy, attrib_list,configs, config_size,num_config);
	}
	
EXPORT_C EGLBoolean eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config,
									   EGLint attribute, EGLint *value)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglGetConfigAttrib(dpy, config,attribute,value);
	}

EXPORT_C EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win,
		                                   const EGLint *attrib_list)
	{
	GET_THREAD_SESSION(session, EGL_NO_SURFACE)
	return session->EglCreateWindowSurface(dpy, config,win,attrib_list);
	}

EXPORT_C EGLSurface eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config,
				   const EGLint *attrib_list)
	{
	GET_THREAD_SESSION(session, EGL_NO_SURFACE)
	return session->EglCreatePbufferSurface(dpy, config,attrib_list);
	}

EXPORT_C EGLSurface eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config,
				  EGLNativePixmapType pixmap,
				  const EGLint *attrib_list)
	{
	GET_THREAD_SESSION(session, EGL_NO_SURFACE)
	return session->EglCreatePixmapSurface(dpy, config,pixmap,attrib_list);
	}

EXPORT_C EGLBoolean eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglDestroySurface(dpy, surface);
	}

EXPORT_C EGLBoolean eglQuerySurface(EGLDisplay dpy, EGLSurface surface,
			   EGLint attribute, EGLint *value)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglQuerySurface(dpy, surface,attribute, value);
	}

EXPORT_C EGLBoolean eglBindAPI(EGLenum api)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglBindAPI(api);
	}

EXPORT_C EGLenum eglQueryAPI(void)
	{
	GET_THREAD_SESSION(session, EGL_NONE)
	return session->EglQueryAPI();
	}

EXPORT_C EGLBoolean eglWaitClient(void)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglWaitClient();
	}

EXPORT_C EGLSurface eglCreatePbufferFromClientBuffer(
	      EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer,
	      EGLConfig config, const EGLint *attrib_list)
	{
	GET_THREAD_SESSION(session, EGL_NO_SURFACE)
	return session->EglCreatePbufferFromClientBuffer(dpy, buftype, buffer,config, attrib_list);
	}

EXPORT_C EGLBoolean eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface,
			    EGLint attribute, EGLint value)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglSurfaceAttrib(dpy, surface,attribute, value);
	}

EXPORT_C EGLBoolean eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglBindTexImage(dpy, surface, buffer);
	}

EXPORT_C EGLBoolean eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglReleaseTexImage(dpy, surface, buffer);
	}

EXPORT_C EGLBoolean eglSwapInterval(EGLDisplay dpy, EGLint interval)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglSwapInterval(dpy, interval);
	}

EXPORT_C EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config,
			    EGLContext share_context,
			    const EGLint *attrib_list)
	{
	GET_THREAD_SESSION(session, EGL_NO_CONTEXT)
	return session->EglCreateContext(dpy, config,share_context,attrib_list);
	}

EXPORT_C EGLBoolean eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglDestroyContext(dpy, ctx);
	}

EXPORT_C EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw,
			  EGLSurface read, EGLContext ctx)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglMakeCurrent(dpy, draw,read, ctx);
	}

EXPORT_C EGLContext eglGetCurrentContext(void)
	{
	GET_THREAD_SESSION(session, EGL_NO_CONTEXT)
	return session->EglGetCurrentContext();
	}

EXPORT_C EGLSurface eglGetCurrentSurface(EGLint readdraw)
	{
	GET_THREAD_SESSION(session, EGL_NO_SURFACE)
	return session->EglGetCurrentSurface(readdraw);
	}

EXPORT_C EGLDisplay eglGetCurrentDisplay(void)
	{
	GET_THREAD_SESSION(session, EGL_NO_DISPLAY)
	return session->EglGetCurrentDisplay();
	}

EXPORT_C EGLBoolean eglQueryContext(EGLDisplay dpy, EGLContext ctx,
			   EGLint attribute, EGLint *value)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglQueryContext(dpy, ctx,attribute, value);
	}

EXPORT_C EGLBoolean eglWaitGL(void)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglWaitGL();
	}

EXPORT_C EGLBoolean eglWaitNative(EGLint engine)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglWaitNative(engine);
	}

EXPORT_C EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglSwapBuffers(dpy, surface);
	}

EXPORT_C EGLBoolean eglCopyBuffers(EGLDisplay dpy, EGLSurface surface,
			  EGLNativePixmapType target)
	{
	GET_THREAD_SESSION(session, EGL_FALSE)
	return session->EglCopyBuffers(dpy, surface,target);
	
	}

// EGL_KHR_reusable_sync APIs
//
EGLSyncKHR eglCreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list)
    {
    GET_THREAD_SESSION(session, EGL_NO_SYNC_KHR)
    return session->EglCreateSyncKhr(dpy, type, attrib_list);
    }

EGLBoolean eglDestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync)
    {
    GET_THREAD_SESSION(session, EGL_FALSE)
    return session->EglDestroySyncKhr(dpy, sync);
    }

EGLint eglClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout)
    {
    GET_THREAD_SESSION(session, EGL_FALSE)
    return session->EglClientWaitSyncKhr(dpy, sync, flags, timeout);
    }

EGLBoolean eglSignalSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode)
    {
    GET_THREAD_SESSION(session, EGL_FALSE)
    return session->EglSignalSyncKhr(dpy, sync, mode);
    }

EGLBoolean eglGetSyncAttribKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint *value)
    {
    GET_THREAD_SESSION(session, EGL_FALSE)
    return session->EglGetSyncAttribKhr(dpy, sync, attribute, value);
    }

// Private extensions
//
EGLint egl_Private_SignalSyncNOK(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode)
    {
    GET_THREAD_SESSION(session, EGL_BAD_ALLOC)
    return session->EglSignalSyncInternal(dpy, sync, mode);
    }

// Debug APIs
//
#ifdef _DEBUG

#define GET_THREAD_SESSION_ASSERT(s)    CEglThreadSession* s = CEglThreadSession::Static(); \
                                        __ASSERT_DEBUG(s, User::Panic(KEglPanicCategory, EEglPanicInvalidSession));

void egliDebugHeapMarkStart()
    {
    GET_THREAD_SESSION_ASSERT(session)
    session->EglHeapMarkStart();
    }

EGLint egliDebugHeapMarkEnd(EGLint count)
    {
    GET_THREAD_SESSION_ASSERT(session)    
    return session->EglHeapMarkEnd(count);
    }

void egliDebugSetBurstAllocFail(EGLenum type, EGLint rate, EGLint burst)
    {
    GET_THREAD_SESSION_ASSERT(session)
    session->EglHeapSetBurstAllocFail(type, rate, burst);
}
#endif //_DEBUG
