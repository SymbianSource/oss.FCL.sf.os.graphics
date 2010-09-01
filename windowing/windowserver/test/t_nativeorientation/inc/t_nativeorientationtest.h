// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
@internalComponent
*/

#ifndef __T_NATIVEORIENTATIONTEST_H__
#define __T_NATIVEORIENTATIONTEST_H__

#include <w32std.h>
#include <test/testexecutestepbase.h>
#include <graphics/surfacemanager.h>
#include <graphics/surfaceupdateclient.h>
#include <graphics/surfaceconfiguration.h>

_LIT(KT_NativeOrientationTestStep,"T_NativeOrientationTestStep");

class CT_NativeOrientationTest : public CTestStep
	{
public:
	CT_NativeOrientationTest();
	~CT_NativeOrientationTest();
	// from CTestStep
	virtual enum TVerdict doTestStepPreambleL();
	virtual enum TVerdict doTestStepL();
	virtual enum TVerdict doTestStepPostambleL();

private:
	// Test cases
    void GRAPHICS_WSERV_0733L();
    void GRAPHICS_WSERV_0734L();
    void GRAPHICS_WSERV_0735L();
    void GRAPHICS_WSERV_0736L();
    void GRAPHICS_WSERV_0737L();
    void GRAPHICS_WSERV_0738L();
    void GRAPHICS_WSERV_0739L();
    void GRAPHICS_WSERV_0740L();
	void GRAPHICS_WSERV_0741L();
    void GRAPHICS_WSERV_0742L();
	void CreateSurfaceAttributes(RSurfaceManager::TSurfaceCreationAttributesBuf&, const TSize&);
	void FillSurfaceL(TSurfaceId&, const TRgb&, RSurfaceManager&, RSurfaceUpdateSession&);
	
private:	// Data members
	TInt iTestId;
	RWsSession iWs;
	};

#endif	// __T_NATIVEORIENTATIONTEST_H__
