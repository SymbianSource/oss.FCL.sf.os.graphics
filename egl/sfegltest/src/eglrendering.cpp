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

CEGLRendering::CEGLRendering(RWindow& aWindow)
	:  iWindow(aWindow)
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
	iRedrawTimer = CPeriodic::NewL(CActive::EPriorityIdle);
	EglSetupL();
	KhrSetup();
	KhrPaint();
	EglSwapBuffers();
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

void CEGLRendering::EglSwapBuffers()
    {
    RDebug::Printf("[EBT] CEGLRendering::EglSwapBuffers");
    eglSwapBuffers(iDisplay, iSurface);
    EGLCheckError();
    }

CEGLRendering::~CEGLRendering()
    {
    RDebug::Printf("[EBT] CEGLRendering::~CEGLRendering");

    StopRedrawTimer();
    delete iRedrawTimer;

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
    }

void CEGLRendering::StartRedrawTimer()
    {
    // Start drawing the screen periodically
    iRedrawTimer->Start(0, KTimerDelay, TCallBack(TimerCallBack, this));
    }

void CEGLRendering::StopRedrawTimer()
    {
    if(iRedrawTimer)
        {
        iRedrawTimer->Cancel();
        }
    }

TInt CEGLRendering::TimerCallBack(TAny* aDemo)
	{
	static_cast<CEGLRendering*>(aDemo)->Redraw();
	return KErrNone;
	}

void CEGLRendering::Redraw()
    {
    RDebug::Printf("[EBT] CEGLRendering::Redraw");
    KhrPaint();
    EglSwapBuffers();
    }

