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

#include "t_nativeorientationtest.h"

_LIT(KT_NativeOrientationTestId,"testId");

CT_NativeOrientationTest::CT_NativeOrientationTest()
	{
	SetTestStepName(KT_NativeOrientationTestStep);
	}


CT_NativeOrientationTest::~CT_NativeOrientationTest()
	{
	}


enum TVerdict CT_NativeOrientationTest::doTestStepPreambleL()
	{
	TVerdict ret=CTestStep::doTestStepPreambleL();

	User::LeaveIfError(iWs.Connect()); // Setup session to wserv
	iWs.SetAutoFlush(EFalse);
	    
	if ( !GetIntFromConfig( ConfigSection(), KT_NativeOrientationTestId, iTestId ) )
		{
        INFO_PRINTF1(_L("ERROR: GetIntFromConfig() Failed!"));
		User::Leave(KErrNotFound);
		}
	return ret;
	}

enum TVerdict CT_NativeOrientationTest::doTestStepPostambleL()
	{
	return TestStepResult();
	}


/**
Starts test step
@internalComponent
@return TVerdict pass / fail
@pre N/A
@post N/A
*/
enum TVerdict CT_NativeOrientationTest::doTestStepL()
	{
    INFO_PRINTF1(_L("CT_NativeOrientation started"));
    RDebug::Printf("CT_NativeOrientation started");
    
    __UHEAP_MARK;
        
    switch (iTestId)
        {
        case 733:
            GRAPHICS_WSERV_0733L();
            break;
        case 734:
            GRAPHICS_WSERV_0734L();
            break;
        case 735:
            GRAPHICS_WSERV_0735L();
            break;
        case 736:
            GRAPHICS_WSERV_0736L();
            break;
        case 737:
            GRAPHICS_WSERV_0737L();
            break;
        case 738:
            GRAPHICS_WSERV_0738L();
            break;
        case 739:
            GRAPHICS_WSERV_0739L();
            break;
        case 740:
            GRAPHICS_WSERV_0740L();
            break;
        case 741:
            GRAPHICS_WSERV_0741L();
            break;
        case 742:
            GRAPHICS_WSERV_0742L();
            break;
        default:
            INFO_PRINTF1(_L("Bad Test Case Id"));
            User::Panic(_L("Bad Test Case Id"), 999);
            break;
        }

    iWs.Close();
    __UHEAP_MARKEND;

    INFO_PRINTF1(_L("CT_NativeOrientation completed!"));
    RDebug::Printf("CT_NativeOrientation completed");
    return TestStepResult();
	}

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0742
 @SYMTestCaseDesc   FixNativeOrientation() on a window before Construct()
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create screen device
                    2. Create group window
                    3. Create test window
                    4. Call FixNativeOrientation()
                    5. Construct test window
 @SYMTestExpectedResults    Works normally.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0742L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0742L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0742L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));

    RWindow myWin = RWindow(iWs);
    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    myWin.Activate();
    iWs.Finish();

    delete scrDevice;

    RDebug::Printf("Completed test GRAPHICS_WSERV_0742L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0742L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0741
 @SYMTestCaseDesc   FixNativeOrientation() on a window after Activate()
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create screen device
                    2. Create group window
                    3. Create test window
                    4. Activate test window
                    5. Call FixNativeOrientation()
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0741L()
	{
    RDebug::Printf("Entering test GRAPHICS_WSERV_0741L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0741L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));

    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    myWin.Activate();

    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    iWs.Finish();

    delete scrDevice;

    RDebug::Printf("Completed test GRAPHICS_WSERV_0741L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0741L"));
	}

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0740
 @SYMTestCaseDesc   SizeForEgl() returns same size as configured by SetSize()
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create screen device
                    2. Create group window
                    3. Create test window
                    4. Activate test window
                    5. Call SetSize(20,20)
                    6. Call SizeForEgl()
 @SYMTestExpectedResults    SizeForEgl() returns same size as configured by SetSize()
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0740L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0740L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0740L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));
    
    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    myWin.Activate();
    
    TSize setSize(20,20);
    myWin.SetSize(setSize);
    TSize eglSize = myWin.SizeForEgl();
    TEST(eglSize==setSize);
    iWs.Finish();

    delete scrDevice;
    RDebug::Printf("Completed test GRAPHICS_WSERV_0740L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0740L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0739
 @SYMTestCaseDesc   FixNativeOrientation() called twice on same window
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create screen device
                    2. Create group window
                    3. Create test window
                    4. Call FixNativeOrientation()
                    5. Call FixNativeOrientation()
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0739L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0739L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0739L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));
    
    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());

    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    iWs.Finish();
    nativeErr = myWin.FixNativeOrientation(); // Panic
    iWs.Finish();

    delete scrDevice;
    RDebug::Printf("Completed test GRAPHICS_WSERV_0739L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0739L"));
    }


/**
 @SYMTestCaseID     GRAPHICS-WSERV-0738
 @SYMTestCaseDesc   SizeForEgl() returns same size as configured by SetSize()
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create screen device
                    2. Create group window
                    3. Create test window
                    4. Activate test window
                    5. Call SetSize(60,40)
                    6. Call SizeForEgl() (check size is (60,40)
                    7. Call FixNativeOrientation()
                    8. Call SetSize(40,60)
                    9. Call SizeForEgl() (check size is (60,40)
 @SYMTestExpectedResults    SizeForEgl() returns same size as configured by SetSize()
                            prior to FixNativeOrientation(), i.e. subsequent SetSize()
                            does not affect SizeForEgl()
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0738L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0738L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0738L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));
    
    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    
    TSize setSize(60,40);
    myWin.SetSize(setSize);
    TSize eglSize = myWin.SizeForEgl();
    TEST(eglSize==setSize);
    TSize winSize = myWin.Size();
    TEST(winSize==setSize);

    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    
    eglSize = myWin.SizeForEgl();
    TEST(eglSize==setSize);
    winSize = myWin.Size();
    TEST(winSize==setSize);
    
    TSize setSize2(40,60);
    myWin.SetSize(setSize2);
    eglSize = myWin.SizeForEgl();
    TEST(eglSize==setSize);
    winSize = myWin.Size();
    TEST(winSize==setSize2);
    
    iWs.Finish();
    myWin.Close();
    grpWin.Close();
    delete scrDevice;
    RDebug::Printf("Completed test GRAPHICS_WSERV_0738L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0738L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0737
 @SYMTestCaseDesc   FixNativeOrientation() without primary screen device
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create group window
                    2. Create test window
                    3. Call FixNativeOrientation()
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0737L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0737L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0737L")); 

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));
    
    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));

    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    iWs.Finish();

    RDebug::Printf("Completed test GRAPHICS_WSERV_0737L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0737L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0736
 @SYMTestCaseDesc   SetBackgroundSurface() without primary screen device
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create group window
                    2. Create test window
                    3. Call FixNativeOrientation()
                    4. Create Surface
                    5. Delete screen device
                    6. SetBackgroundSurface()
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0736L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0736L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0736L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));

    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());

    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    myWin.Activate();
    iWs.Finish();
    
    RSurfaceManager surfaceManager;
    User::LeaveIfError(surfaceManager.Open());
    
    RSurfaceManager::TSurfaceCreationAttributesBuf backgroundBuf;
    CreateSurfaceAttributes(backgroundBuf,myWin.Size());
          
    RSurfaceUpdateSession surfaceUpdateSession;
    User::LeaveIfError(surfaceUpdateSession.Connect());
    
    TSurfaceId surfaceId = TSurfaceId::CreateNullId();
    User::LeaveIfError(surfaceManager.CreateSurface(backgroundBuf, surfaceId));
   
    TSurfaceConfiguration surfaceConfig;
    surfaceConfig.SetSurfaceId(surfaceId);
    surfaceConfig.SetViewport(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetExtent(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetOrientation(CFbsBitGc::EGraphicsOrientationNormal);

    TRgb backgroundColor = TRgb(255,255,255,255);
    FillSurfaceL(surfaceId, backgroundColor, surfaceManager, surfaceUpdateSession);

    delete scrDevice;
    User::LeaveIfError(myWin.SetBackgroundSurface(surfaceConfig, ETrue));

    RDebug::Printf("Completed test GRAPHICS_WSERV_0736L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0736L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0735
 @SYMTestCaseDesc   Trigger background color draw without primary screen device
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create group window
                    2. Create test window
                    3. Call FixNativeOrientation()
                    4. Create Surface
                    5. SetBackgroundSurface()
                    6. Delete screen device
                    7. Trigger background color draw
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0735L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0735L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0735L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));

    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    
    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    myWin.Activate();
    iWs.Finish();
    
    RSurfaceManager surfaceManager;
    User::LeaveIfError(surfaceManager.Open());
    
    RSurfaceManager::TSurfaceCreationAttributesBuf backgroundBuf;
    CreateSurfaceAttributes(backgroundBuf,myWin.Size());
          
    RSurfaceUpdateSession surfaceUpdateSession;
    User::LeaveIfError(surfaceUpdateSession.Connect());
    
    TSurfaceId surfaceId = TSurfaceId::CreateNullId();
    User::LeaveIfError(surfaceManager.CreateSurface(backgroundBuf, surfaceId));
   
    TSurfaceConfiguration surfaceConfig;
    surfaceConfig.SetSurfaceId(surfaceId);
    surfaceConfig.SetViewport(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetExtent(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetOrientation(CFbsBitGc::EGraphicsOrientationNormal);

    TRgb backgroundColor = TRgb(255,255,255,255);
    FillSurfaceL(surfaceId, backgroundColor, surfaceManager, surfaceUpdateSession);
    User::LeaveIfError(myWin.SetBackgroundSurface(surfaceConfig, ETrue));

    delete scrDevice;
    myWin.Invalidate();
    myWin.BeginRedraw();
    myWin.EndRedraw();
    iWs.Finish();
    
    RDebug::Printf("Completed test GRAPHICS_WSERV_0735L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0735L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0734
 @SYMTestCaseDesc   Change window position without primary screen device
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create group window
                    2. Create test window
                    3. Call FixNativeOrientation()
                    4. Create Surface
                    5. SetBackgroundSurface()
                    6. Delete screen device
                    7. Change window position
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0734L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0734L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0734L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));

    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    
    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    myWin.Activate();
    iWs.Finish();
    
    RSurfaceManager surfaceManager;
    User::LeaveIfError(surfaceManager.Open());
    
    RSurfaceManager::TSurfaceCreationAttributesBuf backgroundBuf;
    CreateSurfaceAttributes(backgroundBuf,myWin.Size());
          
    RSurfaceUpdateSession surfaceUpdateSession;
    User::LeaveIfError(surfaceUpdateSession.Connect());
    
    TSurfaceId surfaceId = TSurfaceId::CreateNullId();
    User::LeaveIfError(surfaceManager.CreateSurface(backgroundBuf, surfaceId));
   
    TSurfaceConfiguration surfaceConfig;
    surfaceConfig.SetSurfaceId(surfaceId);
    surfaceConfig.SetViewport(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetExtent(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetOrientation(CFbsBitGc::EGraphicsOrientationNormal);

    TRgb backgroundColor = TRgb(255,255,255,255);
    FillSurfaceL(surfaceId, backgroundColor, surfaceManager, surfaceUpdateSession);
    User::LeaveIfError(myWin.SetBackgroundSurface(surfaceConfig, ETrue));

    delete scrDevice;
    myWin.SetPosition(TPoint(40,40));
    iWs.Finish();
    
    RDebug::Printf("Completed test GRAPHICS_WSERV_0734L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0734L"));
    }

/**
 @SYMTestCaseID     GRAPHICS-WSERV-0733
 @SYMTestCaseDesc   Change window size without primary screen device
 @SYMPREQ           66527  
 @SYMREQ            417-66527
 @SYMPrerequisites  
 @SYMTestType       CT (Component Test)
 @SYMTestActions    1. Create group window
                    2. Create test window
                    3. Call FixNativeOrientation()
                    4. Create Surface
                    5. SetBackgroundSurface()
                    6. Delete screen device
                    7. Change window size
 @SYMTestExpectedResults    EWsWinOpFixNativeOrientation panic.
*/
void CT_NativeOrientationTest::GRAPHICS_WSERV_0733L()
    {
    RDebug::Printf("Entering test GRAPHICS_WSERV_0733L");
    INFO_PRINTF1(_L("Entering test GRAPHICS_WSERV_0733L")); 
    CWsScreenDevice* scrDevice = new (ELeave) CWsScreenDevice(iWs);
    User::LeaveIfError(scrDevice->Construct());

    RWindowGroup grpWin = RWindowGroup(iWs);
    User::LeaveIfError(grpWin.Construct((TUint32)&grpWin));

    RWindow myWin = RWindow(iWs);
    User::LeaveIfError(myWin.Construct(grpWin, (TUint32)&myWin));
    myWin.SetExtent(TPoint(0,0), scrDevice->SizeInPixels());
    
    TInt nativeErr = myWin.FixNativeOrientation();
    TEST(nativeErr==KErrNone);
    myWin.Activate();
    iWs.Finish();
    
    RSurfaceManager surfaceManager;
    User::LeaveIfError(surfaceManager.Open());
    
    RSurfaceManager::TSurfaceCreationAttributesBuf backgroundBuf;
    CreateSurfaceAttributes(backgroundBuf,myWin.Size());
          
    RSurfaceUpdateSession surfaceUpdateSession;
    User::LeaveIfError(surfaceUpdateSession.Connect());
    
    TSurfaceId surfaceId = TSurfaceId::CreateNullId();
    User::LeaveIfError(surfaceManager.CreateSurface(backgroundBuf, surfaceId));
   
    TSurfaceConfiguration surfaceConfig;
    surfaceConfig.SetSurfaceId(surfaceId);
    surfaceConfig.SetViewport(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetExtent(TRect(TPoint(0,0),scrDevice->SizeInPixels()));
    surfaceConfig.SetOrientation(CFbsBitGc::EGraphicsOrientationNormal);

    TRgb backgroundColor = TRgb(255,255,255,255);
    FillSurfaceL(surfaceId, backgroundColor, surfaceManager, surfaceUpdateSession);
    User::LeaveIfError(myWin.SetBackgroundSurface(surfaceConfig, ETrue));

    delete scrDevice;
    myWin.SetSize(TSize(27,29));
    iWs.Finish();
    
    RDebug::Printf("Completed test GRAPHICS_WSERV_0733L");
    INFO_PRINTF1(_L("Completed test GRAPHICS_WSERV_0733L"));
    }

void CT_NativeOrientationTest::CreateSurfaceAttributes(RSurfaceManager::TSurfaceCreationAttributesBuf& aBuffer , const TSize& aSize)
    {
    RSurfaceManager::TSurfaceCreationAttributes& b = aBuffer();
                
    b.iSize.iWidth = aSize.iWidth;
    b.iSize.iHeight = aSize.iHeight;
    b.iBuffers = 1;              // number of buffers in the surface
    b.iPixelFormat = EUidPixelFormatARGB_8888;
    b.iStride = aSize.iWidth * 4;        // Number of bytes between start of one line and start of next
    b.iOffsetToFirstBuffer = 0; // way of reserving space before the surface pixel data
    b.iAlignment = 4;           // alignment, 1,2,4,8 byte aligned
    b.iContiguous = EFalse;
    b.iMappable = ETrue;
    }

void CT_NativeOrientationTest::FillSurfaceL(TSurfaceId& aSurface, const TRgb& aColor, RSurfaceManager& surfaceManager, RSurfaceUpdateSession& surfaceUpdateSession)
    {
    RSurfaceManager::TInfoBuf infoBuf;
    RSurfaceManager::TSurfaceInfoV01& info = infoBuf();

    User::LeaveIfError(surfaceManager.SurfaceInfo(aSurface, infoBuf));
    TUint32 color = 0;
    TBool use16 = EFalse;

    if (info.iSize.iHeight<0 || info.iSize.iWidth<0 || info.iStride<0)
        {
        User::Leave(KErrCorrupt);
        }
    if (info.iSize.iHeight==0 || info.iSize.iWidth==0 || info.iStride==0)
        {
        User::Leave(KErrNotReady);
        }

    switch (info.iPixelFormat)
        {
        case EUidPixelFormatXRGB_8888:
            {
            color = aColor.Color16MU();
#ifdef ALPHA_FIX_24BIT
            color |= ((ALPHA_FIX_24BIT)&0xff)<<24;
#endif
            break;
            }
        case EUidPixelFormatARGB_8888:
            {
            color = aColor.Color16MA();
            break;
            }
        case EUidPixelFormatARGB_8888_PRE:
            {
            color = aColor.Color16MAP();
            break;
            }
        case EUidPixelFormatXRGB_4444:
        case EUidPixelFormatARGB_4444:
            {
            color = aColor.Color4K();
            use16 = ETrue;
            break;
            }
        case EUidPixelFormatRGB_565:
            {
            color = aColor.Color64K();
            use16 = ETrue;
            break;
            }
        default:
            {
            User::Leave(KErrNotSupported);
            break;
            }
        }

    RChunk chunk;
    User::LeaveIfError(surfaceManager.MapSurface(aSurface, chunk));
    CleanupClosePushL(chunk);

    TInt offsetToFirstBuffer;
    User::LeaveIfError(surfaceManager.GetBufferOffset(aSurface, 0, offsetToFirstBuffer));
    TUint8* surfacePtr = chunk.Base() + offsetToFirstBuffer;
    TUint8* linePtr = surfacePtr;

    if (use16)
        {
        if ( info.iSize.iWidth*2>info.iStride)
            {
            User::Leave(KErrOverflow);
            }
        TUint16* ptr = reinterpret_cast<TUint16*>(surfacePtr);

        // Fill first line
        for (TInt xx = 0; xx < info.iSize.iWidth; xx++)
            {
            ptr[xx] = (TUint16)color;
            }
        }
    else
        {
        if ( info.iSize.iWidth*4>info.iStride)
            {
            User::Leave(KErrOverflow);
            }
        TUint32* ptr = reinterpret_cast<TUint32*>(surfacePtr);

        // Fill first line
        for (TInt xx = 0; xx < info.iSize.iWidth; xx++)
            {
            ptr[xx] = color;
            }
        }

    // Now copy that to the other lines
    for (TInt yy = 1; yy < info.iSize.iHeight; yy++)
        {
        linePtr += info.iStride;
        Mem::Copy(linePtr, surfacePtr, info.iSize.iWidth * 4);
        }
    
    TInt err = surfaceUpdateSession.SubmitUpdate(0, aSurface, 0, NULL);
    if (err!=KErrNone)
        RDebug::Printf("Error submitting update: 0x%X\n", err);

    CleanupStack::PopAndDestroy(/* chunk */);
    }
