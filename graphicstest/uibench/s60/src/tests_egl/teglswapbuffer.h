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
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/
 
#ifndef TEGLSWAPBUFFER_H
#define TEGLSWAPBUFFER_H


#include "te_graphicsperformanceSuiteStepBase.h"

#include <EGL/egl.h>


class CTEglSwapBuffer : public CTe_graphicsperformanceSuiteStepBase
	{
public:
    virtual ~CTEglSwapBuffer();
    CTEglSwapBuffer();
	
private:
	// From CTe_graphicsperformanceSuiteStepBase
    virtual TVerdict doTestStepPreambleL();
    virtual TVerdict doTestStepL();
	
	void EglSwapBufferL();
	void EglSwapBufferRegionL();
	void EglSwapBufferRegionStressL(EGLint aCount);
	
private:
    RWsSession iWs;
    RWindowGroup iWindowGroup;
    RWindow iWindow;
    
    EGLDisplay iEglDisplay;
    EGLSurface iEglSurface;
    EGLContext iEglContext;
	};
	
// Testname, is used by the test server and the test scripts to identify the test
_LIT(KTEglSwapBuffer,"teglswapbuffer");

#endif
