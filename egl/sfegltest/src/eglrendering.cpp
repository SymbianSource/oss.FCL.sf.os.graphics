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

#include <string.h>
#include "eglrendering.h"
#include "openvgengine.h"
#include <hal.h>

const TInt KTimerDelay = 0;


/** Attributes to be passed into eglChooseConfig */
const EGLint	KColorRGBA8888AttribList[] =
		{
		EGL_RED_SIZE,			8,
		EGL_GREEN_SIZE,			8,
		EGL_BLUE_SIZE,			8,
        EGL_ALPHA_SIZE,         8,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, 	EGL_OPENVG_BIT,
		EGL_NONE
		};


CEGLRendering* CEGLRendering::NewL(RWindow& aWindow, TBool aQhd)
	{
	CEGLRendering* self = CEGLRendering::NewLC(aWindow, aQhd);
	CleanupStack::Pop(self);
	return self;
	}


CEGLRendering* CEGLRendering::NewLC(RWindow& aWindow, TBool aQhd)
	{
	CEGLRendering* self = new(ELeave) CEGLRendering(aWindow);
	CleanupStack::PushL(self);
	self->ConstructL(aQhd);
	return self;
	}

CEGLRendering::~CEGLRendering()
	{
	Stop();
	
	delete iTimer;

	if (iContextVG!=EGL_NO_CONTEXT)
		{
		EGLCheckReturnError(eglDestroyContext(iDisplay,iContextVG));
		}
	
	if (iSurface!=EGL_NO_SURFACE)
		{
		EGLCheckReturnError(eglDestroySurface(iDisplay,iSurface));
		}
	
	// Call eglMakeCurrent() to ensure the surfaces and contexts are truly destroyed. 
	EGLCheckReturnError(eglMakeCurrent(iDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

	EGLCheckReturnError(eglTerminate(iDisplay));
	EGLCheckReturnError(eglReleaseThread());

	delete iBitmap;
	}

void CEGLRendering::Start()
	{
	// Start drawing the screen periodically
	iTimer->Start(0, KTimerDelay, TCallBack(TimerCallBack,this));
	}

void CEGLRendering::Stop()
	{
	if(iTimer)
		{
		iTimer->Cancel();
		}	
	}

void CEGLRendering::EGLCheckError()
	{
	EGLint error = eglGetError();
	if(error != EGL_SUCCESS)
		{
		User::Panic(_L("EGL error"), error);
		}
	}

void CEGLRendering::VGCheckError()
	{
	VGint error = vgGetError();
	if(error != VG_NO_ERROR)
		{
		User::Panic(_L("OpenVG error"), error);
		}
	}

void CEGLRendering::EGLCheckReturnError(EGLBoolean aBool)
	{
	if (!aBool)
		{
		User::Panic(_L("EGL return error"),eglGetError());
 		}
	}


CEGLRendering::CEGLRendering(RWindow& aWindow)
	: iWindow(aWindow),iCount(0)
	{
	}

/*
 * Construct EGL objects, and OpenVG binding.
 * 
 * Here we collaborate with EGL to associate a session, pick and configuration, assign
 * it to the window we have, and then bind the OpenVG rendering API to our newly created
 * context.
 * 
 * In bring up terms, here is where the first EGL code entry points are called from.  Its
 * the natural point where an EGL bringup starts debugging from, assuming the core EGL
 * works in terms of supporting EGL sync objects (needed for boot before we get to the
 * ESHELL command prompt).
 */
void CEGLRendering::ConstructL(TBool aQhd)
    {
	RDebug::Printf("CEGLRendering::ConstructL");
	
	// Refresh timer	
	iTimer = CPeriodic::NewL(CActive::EPriorityIdle);
	
	const TDisplayMode dispMode = iWindow.DisplayMode();
	const TSize windowSize(iWindow.Size());

	// Create display object
	iDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	RDebug::Printf("CEGLRendering::ConstructL 1");
	EGLCheckError();

	// Initialize display object
	EGLCheckReturnError(eglInitialize(iDisplay, NULL, NULL));
	RDebug::Printf("CEGLRendering::ConstructL 2");
	
	RDebug::Printf("Vendor string %s", eglQueryString(iDisplay, EGL_VENDOR));
	RDebug::Printf("Version string %s", eglQueryString(iDisplay, EGL_VERSION));
	RDebug::Printf("Version string %s", eglQueryString(iDisplay, EGL_EXTENSIONS));

		
	// Check that EGL provides the capabilities for this app.
	TInt error = KErrNone;
	if ( NULL == strstr(eglQueryString(iDisplay, EGL_CLIENT_APIS), "OpenVG") ) 
		{
		RDebug::Printf("OpenVG not listed in supported client APIs %s", eglQueryString(iDisplay, EGL_CLIENT_APIS));
		error = KErrNotSupported;
		}
		

	if ( NULL == strstr(eglQueryString(iDisplay, EGL_EXTENSIONS), "EGL_SYMBIAN_COMPOSITION") ) 
		{
		RDebug::Printf("EGL_SYMBIAN_COMPOSITION not listed in extension string %s", eglQueryString(iDisplay, EGL_EXTENSIONS));
		error = KErrNotSupported;
		}
	if (error != KErrNone)
		{
		User::Leave(error);
		}

	EGLint numConfigs;
	EGLConfig chosenConfig = 0;

	// Choose the config to use
	EGLCheckReturnError(eglChooseConfig(iDisplay, KColorRGBA8888AttribList, &chosenConfig, 1, &numConfigs));
	RDebug::Printf("CEGLRendering::ConstructL 3");
	if (numConfigs == 0)
		{
		RDebug::Printf("No matching configs found", eglQueryString(iDisplay, EGL_EXTENSIONS));
		User::Leave(KErrNotSupported);
		}
	
	// Create window surface to draw direct to.
	EGLCheckReturnError(eglBindAPI(EGL_OPENVG_API));
	RDebug::Printf("CEGLRendering::ConstructL 4");
	iSurface = eglCreateWindowSurface(iDisplay, chosenConfig, &iWindow, NULL);
	RDebug::Printf("CEGLRendering::ConstructL 5");
	EGLCheckError();

	TInt redSize, greenSize, blueSize, alphaSize;

	EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_ALPHA_SIZE, &alphaSize));
	EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_RED_SIZE, &redSize));
	EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_GREEN_SIZE, &greenSize));
	EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_BLUE_SIZE, &blueSize));
	RDebug::Print(_L("EGLConfig id:%d alpha:%d red:%d green:%d blue:%d"), chosenConfig,
			alphaSize, redSize, greenSize, blueSize);
	RDebug::Printf("CEGLRendering::ConstructL 6");

	// Create context to store surface settings
	iContextVG = eglCreateContext(iDisplay, chosenConfig, EGL_NO_CONTEXT, NULL);
	RDebug::Printf("CEGLRendering::ConstructL 7");
	EGLCheckError();
	
	CEGLRendering::EGLCheckReturnError(eglMakeCurrent(iDisplay, iSurface, iSurface, iContextVG));
	RDebug::Printf("CEGLRendering::ConstructL 8");

    // Now try and draw a line (blue)
    static VGubyte const starSegments[] = 
        {
        VG_MOVE_TO_ABS,
        VG_LINE_TO_REL,
        VG_CLOSE_PATH
        };
    
    static VGfloat const starCoords[] = 
        {
        110, 35,
        50, 160,
        };
        
        
    VGfloat strokeColor[4]  = {1.f, 0.f, 0.f, 1.f};
    
    VGPaint strokePaint = vgCreatePaint();

    vgSetParameteri(strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    vgSetParameterfv(strokePaint, VG_PAINT_COLOR, 4, strokeColor);
    
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                            VG_PATH_DATATYPE_F,
                            1.0f, // scale
                            0.0f, // bias
                            3,    // segmentCapacityHint
                            4,   // coordCapacityHint
                            VG_PATH_CAPABILITY_ALL);
                            
    ASSERT(vgGetError() == VG_NO_ERROR);
    RDebug::Printf("vgCreatePath");
    
                            
    vgAppendPathData(path, sizeof(starSegments), starSegments, starCoords);
    ASSERT(vgGetError() == VG_NO_ERROR);
    RDebug::Printf("vgAppendPathData");
    
    vgSetPaint(strokePaint, VG_STROKE_PATH);
    
    // Draw the star directly using the OpenVG API.
    vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);
    ASSERT(vgGetError() == VG_NO_ERROR);
    RDebug::Printf("vgDrawPath");
    
    eglSwapBuffers(iDisplay, iSurface);
    RDebug::Printf("eglSwapBuffers");
    EGLCheckError();
	}


/** Update the display */
void CEGLRendering::UpdateDisplay()
	{
	// Flush colour buffer to the window surface
	//CEGLRendering::EGLCheckReturnError(eglSwapBuffers(iDisplay, iSurface));
	}

/** Callback called by refresh timer */
TInt CEGLRendering::TimerCallBack(TAny* aDemo)
	{
	
	static_cast<CEGLRendering*>(aDemo)->UpdateDisplay();
	return KErrNone;
	}
