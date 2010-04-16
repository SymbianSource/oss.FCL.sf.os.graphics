// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Tests for manual execution.

/**
 @file
 @test
 @internalComponent - Graphics Resource API Conformance Test Suite
*/

#include "tsggenericmanual.h"

CTSgGenericManual::CTSgGenericManual(TBool aConformanceTests) :
	CTSgTestStepBase(aConformanceTests)
	{
	INFO_PRINTF1(_L("Graphics resource component test - Generic Manual Tests.\r\n"));
	}

CTSgGenericManual::~CTSgGenericManual()
	{
	}

/**
This is intented to be used for TestStressResourceLeakL (GRAPHICS-RESOURCE-0050) test. 
It creates images until the memory full situation. The images are kept in the passed RArray of RSgImage. 
The returned error code is expected to be either KErrNoMemory or KErrNoGraphicsMemory.
*/
TInt CTSgGenericManual::CreateImages(const TSgImageInfo& aInfo, RArray<RSgImage>& aTestImages)
    {
    TInt err = KErrNone;
    while(err == KErrNone)
        {
        RSgImage image;
        err = image.Create(aInfo);
        if(err == KErrNone)
            {
            err = aTestImages.Append(image);
            }
        }    
    return err;
    }

void CTSgGenericManual::DestroyImages(RArray<RSgImage>& aTestImages)
    {
    TInt count = aTestImages.Count();
    for(TInt i=0; i<count; ++i)
        {
        aTestImages[i].Close();
        }   
    aTestImages.Reset();    
    }

/** 
@leave Gets system wide error code
@return TVerdict code
*/
TVerdict CTSgGenericManual::doTestStepL()
	{	
    SetTestStepID(_L("GRAPHICS-RESOURCE-0050"));
    INFO_PRINTF1(_L("RSgImage generic resource leak stress test.\r\n"));
    TestStressResourceLeakL();
	RecordTestResultL();
	
	return TestStepResult();
	}

/**
@SYMTestCaseID          GRAPHICS-RESOURCE-0050
@SYMTestCaseDesc        RSgImage exhaustive and resource leak test
@SYMPREQ                PREQ2637
@SYMFssID               RSgImage::Create(const TSgImageInfo&, const TAny*, TInt)
                        RSgImage::Close() 
@SYMTestPriority        Medium
@SYMTestType            CT
@SYMTestPurpose         To ensure no resource leaks while creating and destroying RSgImage multiple times
@SYMTestActions         Create images until it returns no memomy error. Close the created images and          
                        create as many images as possible until memory is full. Test the number of images
                        created and also for each iteration the number of images created to be the same.
@SYMTestExpectedResults There should be no panics or leaves.
 */
void CTSgGenericManual::TestStressResourceLeakL()
    {
    TestOpenDriverL();
    
    const TInt KNumIterations = 100000;
    TSgImageInfo info;
    info.iPixelFormat = EUidPixelFormatRGB_565;
    info.iSizeInPixels = TSize(1, 1);
    info.iUsage = ESgUsageBitOpenVgImage;
    
    RArray<RSgImage> testImages;    
    
    TInt count(0);
    TInt err = KErrNone;
    for(TInt i=0; i<KNumIterations && (err == KErrNone); ++i) 
        {
        err = CreateImages(info, testImages);
        TInt thisCount = testImages.Count();
        DestroyImages(testImages);

        if (err == KErrNoMemory || err == KErrNoGraphicsMemory)
        	{
        	err = KErrNone;
        	}
        else if (err != KErrNone)
        	{
        	ERR_PRINTF2(_L("Create images error [%d]"), err);
        	SetTestStepResult(EFail);
        	}
      
        if(i > 0 && count != thisCount)
            {
            ERR_PRINTF4(_L("Mismatch @ iteration %d : Was %d, now %d"), i,  count, thisCount);
            }        
        count = thisCount;
        }
    
    INFO_PRINTF2(_L("%d images created \r\n"), count);
    TestCloseDriver();
    }

