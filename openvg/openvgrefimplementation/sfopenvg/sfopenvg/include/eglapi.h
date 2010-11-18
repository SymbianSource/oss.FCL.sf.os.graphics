// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Header file for guest egl implementation

#ifndef EGLAPI_H_
#define EGLAPI_H_


// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <w32std.h>
#include <graphics/surface.h>
#include <graphics/surfaceupdateclient.h>
#include <pixelformats.h>
#include <e32cmn.h>

#ifdef FAISALMEMON_S4_SGIMAGE
#include <sgresource/sgimage.h>
#endif

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>

class CEglSyncExtension;



/*
 Change value to control how much tracing is generated in UDEB builds
 0 = API traces only
 1 = API traces and parameter validation diagnostics
 2 = API traces, param validation, extra diagnostics
 */
#define EGL_TRACE_LEVEL 0

/*
EGL version info
*/
#define EGL_VERSION_MAJOR 1
#define EGL_VERSION_MINOR 4
#define MAKE_VERSION_STRING(major, minor) #major "." #minor

// tracing
#include <e32debug.h>
#ifdef _DEBUG
	#define EGL_TRACE(fmt, args...)                   RDebug::Printf(fmt, ##args)
	#define EGL_TRACE_DETAIL(level, fmt, args...)     if (level <= EGL_TRACE_LEVEL) RDebug::Printf(fmt, ##args)
	#define EGLPANIC_ASSERT(condition, panic)         if (!(condition)) { EglPanic(panic, #panic, #condition, __FILE__, __LINE__); }
	//#define EGLPANIC_ASSERT_DEBUG(condition, panic)   if (!(condition)) { EglPanic(panic, #panic, #condition, __FILE__, __LINE__); }
	//ToDo
	#define EGLPANIC_ASSERT_DEBUG(condition, panic)
	#define EGLPANIC_ALWAYS(panic)                    { EglPanic(panic, #panic, NULL, __FILE__, __LINE__); }
	#define EGLPANIC_DEBUG(panic)                     { EglPanic(panic, #panic, NULL, __FILE__, __LINE__); }
	#define EGL_TRACE_ATTRIB_LIST(aAttribList)        TAttribUtils::TraceAttribList(aAttribList)
	#define EGL_TRACE_GET_ATTRIB(N, T, D, O, A, V, R) TAttribUtils::TraceGetAttrib(N, T, D, O, A, V, R)
	#define EGL_TRACE_SET_ATTRIB(N, T, D, O, A, V)    TAttribUtils::TraceSetAttrib(N, T, D, O, A, V)
#else
	#define EGL_TRACE(fmt, args...)
	#define EGL_TRACE_DETAIL(level, fmt, args...)
	#define EGLPANIC_ASSERT(condition, panic)         if (!(condition)) { EglPanic(panic, NULL, NULL, NULL, __LINE__); }
	#define EGLPANIC_ASSERT_DEBUG(condition, panic)
	#define EGLPANIC_ALWAYS(panic)                    { EglPanic(panic, NULL, NULL, NULL, __LINE__); }
	#define EGLPANIC_DEBUG(panic)
	#define EGL_TRACE_ATTRIB_LIST(aAttribList)
	#define EGL_TRACE_GET_ATTRIB(N, T, D, O, A, V, R)
	#define EGL_TRACE_SET_ATTRIB(N, T, D, O, A, V)
#endif


// Guest EGL panic codes
typedef enum
	{
	EEglPanicCGuestEGLAllocFailed=1,
	EEglPanicDisplayMapLockCreateLocalFailed,
	EEglPanicGuestGraphicsAllocFailed,
	EEglPanicSgImageHandleInvalid,
	EEglPanicExtensionListAllocFailed,
	EEglPanicExtensionListCreationError,
	EEglPanicHostAndClientBoundApiOutOfSync,
	EEglPanicUnexpectedBoundApi,
	EEglPanicSymbianPixmapNotSetInSurface,
	EEglPanicSymbianWindowNotSetInSurface,           // 10
	EEglPanicHostAndClientEglInitOutOfSync,
	EEglPanicHostAndClientEglTerminateOutOfSync,
	EEglPanicErrorNotSet,
	EEglPanicThreadStateNotValidInInternalFunction,
	EEglPanicNotReplyOpcode,
	EEglPanicDisplayMapInsertFailed,
	EEglPanicEglSyncDisplayCreateFailed,
	EEglPanicReleaseInvalidDisplay,
	EEglPanicNullDisplayPointerInMap,
	EEglPanicLockedDisplayNotFound,                  // 20
	EEglPanicStrayDisplayUnlock,
	EEglPanicDisplayNotLockedForWriting,
	EEglPanicDisplayAlreadyLocked,
	EEglPanicReadLockExpected,
	EEglPanicReadFromUnlockedDisplay,
	EEglPanicDisplayAlreadyInMap,
	EEglPanicDisplayNotFound,
	EEglPanicSgDriverCreateLocalFailed,
	EEglPanicInvalidNativePixmap,
	EEglPanicEglImageLockCreateLocalFailed,          // 30
	EEglPanicBadSgDriverVersion,
	EEglPanicEglImageRefCountNonZero,
	EEglPanicEglImageIsDestroyed,
	EEglPanicAtribListLengthTooLong,
	EEglPanicTemp,							// temporary panic in development code
	} TEglPanic;

void EglPanic(TEglPanic aPanicCode, char* aPanicName, char* aCondition, char* aFile, TInt aLine);



#endif /* EGLAPI_H_ */
