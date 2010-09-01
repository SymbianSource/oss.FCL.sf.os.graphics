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
// This class does performance tests for eglSwapBuffers() and eglSwapBuffersRegionNOK().
// The function eglSwapBuffersRegionNOK() is a vendor specific EGL extension and allows users to 
// perform region based surface updates. The test should show how the performance of the
// extension function compares to the default one. 
//


/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/


#include "teglswapbuffer.h"

#include <VG/openvg.h>


// If the condition that is passed to the macro is false
// the macro leaves with the EGL error code
#define EGL_CHECK_ERRORL(PASS) { if (!(PASS)) { SetTestStepResult(EFail); ERR_PRINTF2(KErrEgl, eglGetError()); User::Leave(eglGetError()); } }

// Configuration for a Color16MAP - Window
static const EGLint KConfigAttribs[] =
    {
    EGL_BUFFER_SIZE,     32,
    EGL_RED_SIZE,        8,
    EGL_GREEN_SIZE,      8,
    EGL_BLUE_SIZE,       8,
    EGL_ALPHA_SIZE,      8,
    EGL_RENDERABLE_TYPE, EGL_OPENVG_BIT,
    EGL_SURFACE_TYPE,    EGL_WINDOW_BIT | EGL_VG_ALPHA_FORMAT_PRE_BIT,    
    EGL_NONE
    };

// Additional attributes for the window surface
static const EGLint KWindowSurfaceAttribs[] = 
    {
    EGL_VG_ALPHA_FORMAT, EGL_VG_ALPHA_FORMAT_PRE,
    EGL_NONE
    };

// The test draws alternating backgrounds to show the affect
// of different swapBuffer functions 
static const TInt KMaxClearColors = 2;
static const VGfloat KClearColors[KMaxClearColors][4] = 
    {
        {0.5f, 0.5f, 0.5f, 1.0f}, // gray
        {0.1f, 0.2f, 0.4f, 1.0f}  // blue
    };

// Datatype for a function pointer to eglSwapBuffersRegionNOK()
typedef EGLBoolean (*TFPtrEglSwapBuffersRegionNok) (EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint* rects);

// Number of iterations, it defines how often the swapBuffer function is called
const TInt KIterationsToTest = 1000;

// Maximum number of rectangles for eglSwapBuffersRegionNOK() stress test
const TInt KStressTestMaxNoRects = 100;
// Defines the increase of number of rectangles for each iteration
const TInt KStressTestNoRectsStepSize = 5;
// Size of the dirty rectangles for the stress test
const TSize KStressTestRectSize(10, 10);
// Gap between the dirty Rectangles
const TInt KStressTestRectGap = 3;

// Window dimensions
const TSize KWindowSize(200, 200);
const TPoint KWindowPosition(30, 30);
// Region dimensions (top left hand corner and bottom right hand corner)
const TRect KRegionRect(50, 50, 60, 60);

// This test step meassures the performance of eglSwapBuffers()
_LIT(KTestStep0018,"GRAPHICS-UI-BENCH-S60-0018");
// This test step meassures the performance of eglSwapBuffersRegionNOK()
_LIT(KTestStep0019,"GRAPHICS-UI-BENCH-S60-0019");
// This test step meassures the performance of eglSwapBuffersRegionNOK() with a lot of dirty rectangles
_LIT(KTestStep0020,"GRAPHICS-UI-BENCH-S60-0020");

_LIT(KErrEgl, "EGL error 0x%x");
_LIT(KErrEglConfigNotSupported, "EGL config is not supported.");
_LIT(KInfoRectangles, "Number of dirty rectangles: %d");
_LIT(KWarnStressTestRectCount, "Dirty rectangles for stress test don't fit onto window surface (%d of %d).");


CTEglSwapBuffer::CTEglSwapBuffer()
	{
	SetTestStepName(KTEglSwapBuffer);
	}
	
CTEglSwapBuffer::~CTEglSwapBuffer()	
	{
    // Make sure that this EGL status is active
    eglMakeCurrent(iEglDisplay, iEglSurface, iEglSurface, iEglContext);
    if (iEglContext != EGL_NO_CONTEXT)
        {
        eglDestroyContext(iEglDisplay, iEglContext);
        }
    if (iEglSurface != EGL_NO_SURFACE)
        {
        eglDestroySurface(iEglDisplay, iEglSurface);
        }   
    // Call eglMakeCurrent() to ensure the surfaces and contexts are truly destroyed
    eglMakeCurrent(iEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(iEglDisplay);
    eglReleaseThread();
    
    iWindow.Close();
    iWindowGroup.Close();
    iWs.Close();
	}

/**
 * It's called by the test framework before the actual test. It's used
 * to do the preparation for the test. It's important to call the
 * baseclass implementation also.
 * 
 * @return TVerdict code
 */
TVerdict CTEglSwapBuffer::doTestStepPreambleL()
    {
    CTe_graphicsperformanceSuiteStepBase::doTestStepPreambleL();
    
    // Establish the connection to the window server and create
    // a WindowGroup and a Window object
    TESTNOERRORL(iWs.Connect());    
    iWindowGroup = RWindowGroup(iWs);
    TESTNOERRORL(iWindowGroup.Construct(0));
    iWindow = RWindow(iWs);
    // The window is automatically fullscreen if it's a child of a window group
    TESTNOERRORL(iWindow.Construct(iWindowGroup, reinterpret_cast<TUint32>(this)));
    iWindow.SetSize(KWindowSize);
    iWindow.SetPosition(KWindowPosition);
    iWindow.Activate();
        
    // Create display object
    iEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGL_CHECK_ERRORL(iEglDisplay != EGL_NO_DISPLAY);
    EGL_CHECK_ERRORL(eglInitialize(iEglDisplay, 0, 0));
        
    // Choose EGL config
    EGLConfig matchingConfigs[1];
    EGLint numConfigs = 0;
    eglChooseConfig(iEglDisplay, KConfigAttribs, matchingConfigs, 1, &numConfigs);
    if (numConfigs <= 0) // Abort the test if the EGL config is not supported
        {
        ERR_PRINTF1(KErrEglConfigNotSupported);
        SetTestStepError(KErrNotSupported);
        return TestStepResult();
        }
    
    // Use OpenVG to draw
    EGL_CHECK_ERRORL(eglBindAPI(EGL_OPENVG_API));
    
    // Create the window surface and the egl context and make them current
    iEglSurface = eglCreateWindowSurface(iEglDisplay, matchingConfigs[0], &iWindow, KWindowSurfaceAttribs);
    EGL_CHECK_ERRORL(iEglSurface != EGL_NO_SURFACE);
    iEglContext = eglCreateContext(iEglDisplay, matchingConfigs[0], EGL_NO_CONTEXT, NULL);
    EGL_CHECK_ERRORL(iEglContext != EGL_NO_CONTEXT);
    EGL_CHECK_ERRORL(eglMakeCurrent(iEglDisplay, iEglSurface, iEglSurface, iEglContext));
    
    return TestStepResult();
    }

/**
 * Override of base class pure virtual function.
 * This implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value should be EPass.
 *
 * @return TVerdict code
 */
TVerdict CTEglSwapBuffer::doTestStepL()
    {
    // Tests  the performance of eglSwapBuffers()
    SetTestStepID(KTestStep0018);    
    TRAPD(err, EglSwapBufferL());
    if (err != KErrNone)
        {
        SetTestStepResult(EAbort);
        }    
    RecordTestResultL();
    
    // Tests the maximum performance of eglSwapBuffersRegionNOK()
    SetTestStepID(KTestStep0019);    
    TRAP(err, EglSwapBufferRegionL());
    if (err != KErrNone)
        {
        SetTestStepResult(EAbort);
        }    
    RecordTestResultL();
    
    // Stress tests the performance of eglSwapBuffersRegionNOK()
    SetTestStepID(KTestStep0020);    
    for (TInt noRects = KStressTestNoRectsStepSize; noRects <= KStressTestMaxNoRects; noRects += KStressTestNoRectsStepSize)
        {
        // TRAP here is on purpose, normally you shouldn't use it in loops
        TRAP(err, EglSwapBufferRegionStressL(noRects));
        if (err != KErrNone)
            {
            SetTestStepResult(EAbort);
            }
        }
    RecordTestResultL();
    
    // Close the test and return result to the testframework
    CloseTMSGraphicsStep();    
    return TestStepResult();
	}

/**
@SYMTestCaseID GRAPHICS-UI-BENCH-S60-0018

@SYMTestPriority 1

@SYMPREQ 2677

@SYMTestCaseDesc
Tests how long it takes to swap window surface buffers if the whole surface is updated.

@SYMTestActions
Clear the window surface with alternating background colors, swap the surface buffers
and measure how long it takes.

@SYMTestExpectedResults
Test should pass and print the average framerate to a log file.
*/
void CTEglSwapBuffer::EglSwapBufferL()
    {    
    // Initialise uibench and reset the timer
    iProfiler->InitResults();
    // Perform the test
    for(TInt i = KIterationsToTest; i > 0; --i)
        {
        // Clean the surface with the background color
        vgSetfv(VG_CLEAR_COLOR, 4, KClearColors[i % KMaxClearColors]);
        vgClear(0, 0, KWindowSize.iWidth, KWindowSize.iHeight);
        // Swap the surface buffers
        EGL_CHECK_ERRORL(eglSwapBuffers(iEglDisplay, iEglSurface));
        }
    // Mark the time and print the results to the log file
    iProfiler->MarkResultSetL();
    iProfiler->ResultsAnalysisFrameRate(KTestStep0018, 0, 0, 0,
            KIterationsToTest, KWindowSize.iWidth * KWindowSize.iHeight);   
    }

/**
@SYMTestCaseID GRAPHICS-UI-BENCH-S60-0019

@SYMTestPriority 1

@SYMPREQ 2677

@SYMTestCaseDesc
Tests how long it takes to swap window surface buffers if only a small region is updated. This
test should show the maximum possible performance increase.

@SYMTestActions
Clear the window surface with alternating background colors, swap the surface buffers
and measure how long it takes.

@SYMTestExpectedResults
Test should pass and print the average framerate to a log file.
*/
void CTEglSwapBuffer::EglSwapBufferRegionL()
    {
    // Number of rectangles
    EGLint count = 1;
    // Rectangle for partial swap buffer function
    EGLint rects[] = {KRegionRect.iTl.iX, KRegionRect.iTl.iY, KRegionRect.Width(), KRegionRect.Height()};
    // Get the function pointer for eglSwapBuffersRegionNOK()
    TFPtrEglSwapBuffersRegionNok pfnEglSwapBuffersRegionNok = reinterpret_cast<TFPtrEglSwapBuffersRegionNok>(eglGetProcAddress("eglSwapBuffersRegionNOK"));
    EGL_CHECK_ERRORL(pfnEglSwapBuffersRegionNok);
    
    // Clear the surface
    vgSetfv(VG_CLEAR_COLOR, 4, KClearColors[0]);
    vgClear(0, 0, KWindowSize.iWidth, KWindowSize.iHeight);
    EGL_CHECK_ERRORL(eglSwapBuffers(iEglDisplay, iEglSurface));
    
    // Initialise uibench and reset the timer
    iProfiler->InitResults();
    // Perform the test
    for(TInt i = KIterationsToTest; i > 0; --i)
        {
        // Clean the surface with the background color
        vgSetfv(VG_CLEAR_COLOR, 4, KClearColors[i % KMaxClearColors]);
        vgClear(0, 0, KWindowSize.iWidth, KWindowSize.iHeight);
        // Swap the surface buffers
        EGL_CHECK_ERRORL(pfnEglSwapBuffersRegionNok(iEglDisplay, iEglSurface, count, rects));
        }
    // Mark the time and print the results to the log file
    iProfiler->MarkResultSetL();
    iProfiler->ResultsAnalysisFrameRate(KTestStep0019, 0, 0, 0,
            KIterationsToTest, KWindowSize.iWidth * KWindowSize.iHeight);
    }

/**
@SYMTestCaseID GRAPHICS-UI-BENCH-S60-0020

@SYMTestPriority 1

@SYMPREQ 2677

@SYMTestCaseDesc
Tests how long it takes to swap window surface buffers if only a small region is updated. This
test should show the maximum possible performance increase.

@SYMTestActions
Clear the window surface with alternating background colors, swap the surface buffers
and measure how long it takes.

@SYMTestExpectedResults
Test should pass and print the average framerate to a log file.
*/
void CTEglSwapBuffer::EglSwapBufferRegionStressL(EGLint aCount)
    {
    TInt* rects = static_cast<TInt*>(User::AllocLC(sizeof(TInt) * 4 * aCount));
    TInt actualRectCount = 0;
    TInt idx = 0;
    for (TInt y = 0; (y < KWindowSize.iHeight - KStressTestRectSize.iHeight - 1) && (actualRectCount < aCount); y += KStressTestRectSize.iHeight + KStressTestRectGap)
        {
        for (TInt x = 0; (x < KWindowSize.iWidth - KStressTestRectSize.iWidth - 1) && (actualRectCount < aCount); x += KStressTestRectSize.iWidth + KStressTestRectGap)
            {
            rects[idx++] = x;
            rects[idx++] = y;
            rects[idx++] = KStressTestRectSize.iWidth;
            rects[idx++] = KStressTestRectSize.iHeight;
            actualRectCount++;
            }
        }
    TESTL(actualRectCount > 0);
    if (actualRectCount != aCount)
        {
        WARN_PRINTF3(KWarnStressTestRectCount, actualRectCount, aCount);
        }
    
    // Get the function pointer for eglSwapBuffersRegionNOK()
    TFPtrEglSwapBuffersRegionNok pfnEglSwapBuffersRegionNok = reinterpret_cast<TFPtrEglSwapBuffersRegionNok>(eglGetProcAddress("eglSwapBuffersRegionNOK"));
    EGL_CHECK_ERRORL(pfnEglSwapBuffersRegionNok);
    
    // Clear the surface
    vgSetfv(VG_CLEAR_COLOR, 4, KClearColors[0]);
    vgClear(0, 0, KWindowSize.iWidth, KWindowSize.iHeight);
    EGL_CHECK_ERRORL(eglSwapBuffers(iEglDisplay, iEglSurface));
    
    // Initialise uibench and reset the timer
    iProfiler->InitResults();
    // Perform the test
    for(TInt i = KIterationsToTest; i > 0; --i)
        {
        // Clean the surface with the background color
        vgSetfv(VG_CLEAR_COLOR, 4, KClearColors[i % KMaxClearColors]);
        vgClear(0, 0, KWindowSize.iWidth, KWindowSize.iHeight);
        // Swap the surface buffers
        EGL_CHECK_ERRORL(pfnEglSwapBuffersRegionNok(iEglDisplay, iEglSurface, actualRectCount, rects));
        }
    // Mark the time and print the results to the log file
    iProfiler->MarkResultSetL();
    INFO_PRINTF2(KInfoRectangles, aCount);
    iProfiler->ResultsAnalysisFrameRate(KTestStep0020, 0, 0, 0,
            KIterationsToTest, KWindowSize.iWidth * KWindowSize.iHeight);
    CleanupStack::PopAndDestroy(rects);
    }
