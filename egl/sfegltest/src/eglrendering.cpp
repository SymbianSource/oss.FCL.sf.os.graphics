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

#include "eglrendering.h"
#include <string.h>

const TInt KTimerDelay = 0;

/** Attributes to be passed into eglChooseConfig */
const EGLint KAttribList[] =
		{
		EGL_RED_SIZE,			8,
		EGL_GREEN_SIZE,			8,
		EGL_BLUE_SIZE,			8,
        EGL_ALPHA_SIZE,         8,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENVG_BIT,
		EGL_NONE
		};

void CEGLRendering::EGLCheckError()
    {
    EGLint error = eglGetError();
    if(EGL_SUCCESS != error)
        {
        RDebug::Printf("[EBT] CEglRendering::EGLCheckError error %d", error);
        User::Panic(_L("EBT-EGL"), error);
        }
    }

void CEGLRendering::EGLCheckReturnError(EGLBoolean aBool)
    {
    if(!aBool)
        {
        RDebug::Printf("[EBT] CEglRendering::EGLCheckReturnError false");
        User::Panic(_L("EGL-EGL-RTN"), eglGetError());
        }
    }

void CEGLRendering::VGCheckError()
    {
    VGint error = vgGetError();
    if(VG_NO_ERROR != error)
        {
        RDebug::Printf("[EBT] CEglRendering::VGCheckError error %d", error);
        User::Panic(_L("EBT-VG"), error);
        }
    }

CEGLRendering* CEGLRendering::NewL(RWindow& aWindow)
	{
	CEGLRendering* self = new (ELeave) CEGLRendering(aWindow);
	CleanupStack::PushL(self);
	self->ConstructL();
    CleanupStack::Pop(self);
	return self;
	}

CEGLRendering::CEGLRendering(RWindow& aWindow)
	:  iWindow(aWindow)
	,  iCount(0)
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
void CEGLRendering::ConstructL()
    {
	RDebug::Printf("[EBT] CEGLRendering::ConstructL");
	iTimer = CPeriodic::NewL(CActive::EPriorityIdle);
	EglSetupL();
	VgSetup();
	VgPaint();
	}

void CEGLRendering::EglSetupL()
    {
    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglGetDisplay");
    iDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLCheckError();

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglInitialize");
    EGLCheckReturnError(eglInitialize(iDisplay, NULL, NULL));

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL vendor %s", eglQueryString(iDisplay, EGL_VENDOR));
    RDebug::Printf("[EBT] CEGLRendering::EglSetupL version %s", eglQueryString(iDisplay, EGL_VERSION));
    RDebug::Printf("[EBT] CEGLRendering::EglSetupL extensions %s", eglQueryString(iDisplay, EGL_EXTENSIONS));

    // Check that EGL provides the capabilities for this app.
    if(NULL == strstr(eglQueryString(iDisplay, EGL_CLIENT_APIS), "OpenVG"))
        {
        RDebug::Printf("[EBT] CEGLRendering::EglSetupL OpenVG not listed in supported client APIs %s", eglQueryString(iDisplay, EGL_CLIENT_APIS));
        User::Leave(KErrNotSupported);
        }

    if(NULL == strstr(eglQueryString(iDisplay, EGL_EXTENSIONS), "EGL_SYMBIAN_COMPOSITION") )
        {
        RDebug::Printf("[EBT] CEGLRendering::EglSetupL EGL_SYMBIAN_COMPOSITION not listed in extension string %s", eglQueryString(iDisplay, EGL_EXTENSIONS));
        User::Leave(KErrNotSupported);
        }

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglChooseConfig");
    EGLint numConfigs = 0;
    EGLConfig chosenConfig = 0;
    EGLCheckReturnError(eglChooseConfig(iDisplay, KAttribList, &chosenConfig, 1, &numConfigs));
    if (0 == numConfigs)
        {
        RDebug::Printf("[EBT] No matching configs found", eglQueryString(iDisplay, EGL_EXTENSIONS));
        User::Leave(KErrNotSupported);
        }

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglBindApi");
    EGLCheckReturnError(eglBindAPI(EGL_OPENVG_API));
    
    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglCreateWindowSurface");
    iSurface = eglCreateWindowSurface(iDisplay, chosenConfig, &iWindow, NULL);
    EGLCheckError();

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglGetConfigAttrib");
    TInt redSize, greenSize, blueSize, alphaSize;
    EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_ALPHA_SIZE, &alphaSize));
    EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_RED_SIZE, &redSize));
    EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_GREEN_SIZE, &greenSize));
    EGLCheckReturnError(eglGetConfigAttrib(iDisplay, chosenConfig, EGL_BLUE_SIZE, &blueSize));
    RDebug::Printf("[EBT] CEGLRendering::EglSetupL id:%d alpha:%d red:%d green:%d blue:%d",
                   chosenConfig, alphaSize, redSize, greenSize, blueSize);

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglCreateContext");
    iContext = eglCreateContext(iDisplay, chosenConfig, EGL_NO_CONTEXT, NULL);
    EGLCheckError();

    RDebug::Printf("[EBT] CEGLRendering::EglSetupL eglMakeCurrent");
    CEGLRendering::EGLCheckReturnError(eglMakeCurrent(iDisplay, iSurface, iSurface, iContext));
    }

void CEGLRendering::VgSetup()
    {
    static VGubyte const Segments[] =
        {
        VG_MOVE_TO_ABS,
        VG_LINE_TO_REL,
        VG_CLOSE_PATH
        };

    static VGfloat const Coords[] =
        {
        110, 35,
        50, 160,
        };

    VGfloat strokeColor[4]  = {1.f, 0.f, 0.f, 1.f};

    RDebug::Printf("[EBT] CEGLRendering::VgSetup vgCreatePaint");
    iVGPaint = vgCreatePaint();
    VGCheckError();

    RDebug::Printf("[EBT] CEGLRendering::VgSetup vgSetParameterX");
    vgSetParameteri(iVGPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    VGCheckError();
    vgSetParameterfv(iVGPaint, VG_PAINT_COLOR, 4, strokeColor);
    VGCheckError();
    
    RDebug::Printf("[EBT] CEGLRendering::VgSetup vgCreatePath");
    iVGPath = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                            VG_PATH_DATATYPE_F,
                            1.0f, // scale
                            0.0f, // bias
                            3,    // segmentCapacityHint
                            4,    // coordCapacityHint
                            VG_PATH_CAPABILITY_ALL);
    VGCheckError();
    
    RDebug::Printf("[EBT] CEGLRendering::VgSetup vgAppendPathData");
    vgAppendPathData(iVGPath, sizeof(Segments), Segments, Coords);
    VGCheckError();
    }

void CEGLRendering::VgPaint()
    {
    RDebug::Printf("[EBT] CEGLRendering::VgPaint vgSetPaint");
    vgSetPaint(iVGPaint, VG_STROKE_PATH);
    VGCheckError();
    
    RDebug::Printf("[EBT] CEGLRendering::VgPaint vgDrawPath");
    vgDrawPath(iVGPath, VG_STROKE_PATH);
    VGCheckError();

    RDebug::Printf("[EBT] CEGLRendering::VgPaint eglSwapBuffers");
    eglSwapBuffers(iDisplay, iSurface);
    EGLCheckError();
    }

CEGLRendering::~CEGLRendering()
    {
    RDebug::Printf("[EBT] CEGLRendering::~CEGLRendering");
    
    Stop();

    delete iTimer;

    if (EGL_NO_CONTEXT != iContext)
        {
        EGLCheckReturnError(eglDestroyContext(iDisplay, iContext));
        }

    if (EGL_NO_SURFACE != iSurface)
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
    iTimer->Start(0, KTimerDelay, TCallBack(TimerCallBack, this));
    }

void CEGLRendering::Stop()
    {
    if(iTimer)
        {
        iTimer->Cancel();
        }
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

