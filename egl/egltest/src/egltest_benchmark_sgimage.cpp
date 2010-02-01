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
*/

#include <test/tefunit.h> // for ASSERT macros
#include <e32msgqueue.h>

#include <test/tprofiler.h>
#include <test/egltestcommonprocess.h>
#include <test/egltestcommonconversion.h>
#include <test/egltestcommoninisettings.h>

#include "egltest_benchmark_sgimage.h"

//In general, there is no checking of the returned errors for the API we will be profiling
//we assume that the basic operations will just work, as they have been tested in many other places
//we don’t want error checking to interfere with the profiling itself 
//to enable error checking, uncomment the macro below 
//#define ENABLE_CHECKING_WHILST_PROFILING    1 

_LIT(KBenchmarkSection, "Benchmark");

//data will be used to cleanup VgImages if leaving occurs
NONSHARABLE_CLASS(CVgImageDeleteData) : public CBase
    {
public:
    CVgImageDeleteData(VGImage*& aVgImages, TInt aNumVgImages) :
        iVgImages(aVgImages),
        iNumVgImages(aNumVgImages)
        {
        }
    ~CVgImageDeleteData();
private:    
    VGImage*& iVgImages;//the ownership will be transferred and cleanup function will destroy this array
    TInt iNumVgImages;
    };

CVgImageDeleteData::~CVgImageDeleteData()
    {
    if(iVgImages)
        {
        for(TInt count=iNumVgImages-1; count >= 0; --count)
            {
            vgDestroyImage(iVgImages[count]);
            }
        delete [] iVgImages;
        }
    }

//data will be used to cleanup RSgImages if leaving occurs
NONSHARABLE_CLASS(CSgImageDeleteData) : public CBase
    {
public:
    CSgImageDeleteData(RSgImage*& aSgImages, TInt aNumSgImages) :
        iSgImages(aSgImages),
        iNumSgImages(aNumSgImages)
        {
        }
    ~CSgImageDeleteData();
private:    
    RSgImage*& iSgImages;//the ownership will be transferred and cleanup function will destroy this array
    TInt iNumSgImages;
    };

CSgImageDeleteData::~CSgImageDeleteData()
    {
    if(iSgImages)
        {
        for(TInt count=iNumSgImages-1; count >= 0; --count)
            {
            iSgImages[count].Close();
            }
        delete [] iSgImages;
        }
    }

//data will be used to cleanup EGLImages if leaving occurs
NONSHARABLE_CLASS(CEglImageDeleteData) : public CBase
    {
public:
    CEglImageDeleteData(CEglTestStep& aImageBaseStep, 
            EGLImageKHR*& aEglImages, EGLDisplay& aDisplay, TInt aNumEglImages) : 
        iImageBaseStep(aImageBaseStep), iEglImages(aEglImages), iDisplay(aDisplay), iNumEglImages(aNumEglImages)
        {
        }
    ~CEglImageDeleteData();
private:    
	CEglTestStep& iImageBaseStep; //not owned by this object
    EGLImageKHR*& iEglImages; //the ownership will be transferred and cleanup function will destroy this array
    EGLDisplay iDisplay; //not owned by this object
    TInt iNumEglImages;
    };

CEglImageDeleteData::~CEglImageDeleteData()
    {
    if(iEglImages)
        {
        CTestEglSession* eglSession = iImageBaseStep.GetEglSess(); 
        for(TInt count=iNumEglImages-1; count >= 0; --count)
            {    
            eglSession->DestroyEGLImage( iDisplay, iEglImages[count]);
            }
        delete [] iEglImages;
        }
    }

CEglTest_Benchmark_Base::~CEglTest_Benchmark_Base()
    {
    delete iProfiler;
    }

TVerdict CEglTest_Benchmark_Base::doTestStepPreambleL()
    {
    TVerdict verdict = CEglTestStep::doTestStepPreambleL();
    iProfiler = CTProfiler::NewL(*this);
    //read all parameters from config
    CEglTestCommonIniSettings* iniParser = CEglTestCommonIniSettings::NewL();
    CleanupStack::PushL(iniParser);
    iNumIterations = iniParser->GetNumberOfIterations(KBenchmarkSection);
    if(!iNumIterations)
        {
        ERR_PRINTF1(_L("The number iterations is not specified in INI file, the test will not be executed!"));
        User::Leave(KErrArgument);      
        }
    
    iImageSize = iniParser->GetImageSize(KBenchmarkSection);
    if(iImageSize == TSize(0,0))
        {
        ERR_PRINTF1(_L("The image size whether is not specified in INI file or is TSize(0,0), the test will not be executed!"));
        User::Leave(KErrArgument);      
        }
    iPixelFormat = iniParser->GetPixelFormat(KBenchmarkSection, 0);
    if(iPixelFormat == EUidPixelFormatUnknown)
        {
        ERR_PRINTF1(_L("The image pixel format is not specified in INI file, the test will not be executed!"));
        User::Leave(KErrArgument);      
        }
    CleanupStack::PopAndDestroy(iniParser);
    INFO_PRINTF5(_L("**** The test will be run in following configuration: number of iterations %d, image size (%d, %d), image pixel format 0X%X"), iNumIterations, iImageSize.iWidth, iImageSize.iHeight, iPixelFormat);
    return verdict;
    }

TVerdict CEglTest_Benchmark_Base::doTestStepPostambleL()
    {
    delete iProfiler; //we need to delete here to keep happy heap checking in the base class 
    iProfiler = NULL;
    return CEglTestStep::doTestStepPostambleL();
    }

//-------

/**
@SYMTestCaseID GRAPHICS-EGL-0434

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    Performance test - Creating and destroying VGImage
@SYMTestActions
Creating regular VGImage
Init EGL, create context and surface
For i = 0 to N
    Start timer
    Create VGImage[i] with size: 50x50 and format:  VG_sARGB_8888_PRE
    Stop timer and record time
End loop
Record average time of creating VGImage

Closing regular VGImage
For i = N to 0
    Start timer
    Destroy VGImage[i]
    Stop timer and record time
End loop
Record average time of destroying VGImage
Destroy context, surface and terminate EGL

Creating VGImage from uninitialized RSgImage
Open RSgDriver
Construct TSgImageInfo object with
•   Size: 50x50
•   PixelFormat: ESgPixelFormatARGB_8888_PRE
•   Usage: ESgUsageBitOpenVgImage;

For i = 0 to N
    Start timer
    Create RSgImage[i] with arguments TSgImageInfo but without initialized data
    Stop timer and record time

    Start timer
    Create an EGLImage[i] from the RSgImage[i]
    Stop timer and record time

    Start timer
    Create a VGImage[i] from the EGLImage[i]
    Stop timer and record time

    Record total time of  RSgImage[i], EGLImage[i] and VGImage[i] creation
End loop
Record average creation time of RSgImage, EGLImage, VGImage and the whole RSgImage-EGLImage-VGImage chain

Destroying VGImage from RSgImage
For i = N to 0
    Start timer
    Close VGImage[i]
    Stop timer and record time

    Start timer
    Close EGLImage[i]
    Stop timer and record time

    Start timer
    Close RSgImage[i]
    Stop timer and record time

    Record total time for closing VGImage[i], EGLImage[i] and RSgImage[i]
End loop
Record average closing time of RSgImage, EGLImage, VGImage, and the whole 
VGImage-EGLImage-RSgImage chain

Creating VGImage from initialized RSgImage
For i = 0 to N
    Start timer
    Create RSgImage[i] with arguments TSgImageInfo a stride of 200 and a  pointer to a 
    data array of size 10000 bytes
    Stop timer and record time

    Start timer
    Create an EGLImage[i] from the RSgImage[i]
    Stop timer and record time

    Start timer
    Create a VGImage[i] from the EGLImage[i]
    Stop timer and record time

    Record total time for RSgImage[i], EGLImage[i] and VGImage[i] creation
End loop
Record average creation time of RSgImage, EGLImage, VGImage and the whole 
RSgImage-EGLImage-VGImage chain
Close all VGImages, EGLImages and RSgImages
Close RSgDriver

@SYMTestExpectedResults
The creation of RSgImage, EGLImage and VGImage must return no error. 
The average time shall be made available in an easy-to-use format for further 
analysis and comparison.
*/
TVerdict CEglTest_Benchmark_CreateCloseImage::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0434"));
    INFO_PRINTF1(_L("CEglTest_Benchmark_CreateCloseImage::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
	INFO_PRINTF1(_L("CEglTest_Benchmark_CreateCloseImage can only be run with SgImage-Lite"));
	RecordTestResultL();
	CloseTMSGraphicsStep();
	return TestStepResult();
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(!ret)
        {
        // The extension is not supported
        RecordTestResultL();
        CloseTMSGraphicsStep();
        return TestStepResult();
        }
 
    ASSERT_TRUE(iDisplay == EGL_NO_DISPLAY);
    GetDisplayL();
    CreateEglSessionL();
    iEglSess->InitializeL();
    iEglSess->OpenSgDriverL();
 //----create pixmap and make context curent
    TSgImageInfo imageInfo;
    imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
    imageInfo.iPixelFormat = iPixelFormat;
    imageInfo.iSizeInPixels = iImageSize;
    //create a dummy surface and context for the purpose of enabling use of VG
    iEglSess->CreatePixmapSurfaceAndMakeCurrentAndMatchL(imageInfo, CTestEglSession::EResourceCloseSgImageLate);

    //Creating regular VGImages
    //Create an array of VGImages and push them into cleanup stack
    //vgImageDeleteData takes ownership of the VGImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    VGImageFormat vgPixelFormat = EglTestConversion::PixelFormatToVgFormat(iPixelFormat);
    VGImage* vgImages = NULL;
    CVgImageDeleteData* vgImageDeleteData = new (ELeave) CVgImageDeleteData(vgImages, iNumIterations);
    CleanupStack::PushL(vgImageDeleteData);    
    vgImages = new (ELeave)VGImage[iNumIterations];
    //-------   start profiling
    iProfiler->InitResults();
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        //we will use VG_IMAGE_QUALITY_NONANTIALIASED flag to avoid OpenVG making the quality improvements
        //at the expense of performance (for instance to create an extra buffer) 
        vgImages[count] = vgCreateImage(vgPixelFormat, iImageSize.iWidth, iImageSize.iHeight, VG_IMAGE_QUALITY_NONANTIALIASED);
        //we will check the images later, to avoid interfering with the profiler
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Creating regular VGImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
    
    //check that we created all VgImages
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        ASSERT_VG_TRUE(vgImages[count] !=  VG_INVALID_HANDLE);
        }    
    //Closing regular VGImage
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        vgDestroyImage(vgImages[count]);
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Closing regular VGImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   

//---  Creating VGImage from uninitialized RSgImage  
    //Create an array of SgImages and push them into cleanup stack
    //sgImageData takes ownership of the SgImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    imageInfo.iUsage = ESgUsageBitOpenVgImage;
    RSgImage* sgImages = NULL;
    CSgImageDeleteData* sgImageData = new (ELeave)CSgImageDeleteData(sgImages, iNumIterations);
    CleanupStack::PushL(sgImageData);    
    sgImages = new (ELeave) RSgImage[iNumIterations];
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        const TInt res = sgImages[count].Create(imageInfo, NULL);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
        TESTL(res == KErrNone);
#endif
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Creating uninitialized RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
    
    //check that we created all sgImages successfully
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        TESTL(!sgImages[count].IsNull());
        }    
    
    //Create an array of EglImages and push them into cleanup stack
    //eglImageDeleteData takes ownership of the EglImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    EGLImageKHR* eglImages = NULL;
    CEglImageDeleteData* eglImageDeleteData = new (ELeave)CEglImageDeleteData(*this, eglImages, iDisplay, iNumIterations); 
    CleanupStack::PushL(eglImageDeleteData);    
    eglImages = new (ELeave) EGLImageKHR[iNumIterations];
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {//we will use preserved flag as sgImage supposedly has some content
        eglImages[count]= iEglSess->eglCreateImageKhrL(iDisplay,EGL_NO_CONTEXT,EGL_NATIVE_PIXMAP_KHR,&sgImages[count],const_cast<EGLint *> (KEglImageAttribsPreservedTrue));
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Creating EGLImage from uninitialized RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   

    //check that we created all EglImages successfully
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        ASSERT_EGL_TRUE(eglImages[count] != EGL_NO_IMAGE_KHR)
        }
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        vgImages[count] = iEglSess->vgCreateImageTargetKHR((VGeglImageKHR)eglImages[count]);
        iProfiler->MarkResultSetL();
        }   
    iProfiler->ResultsAnalysis(_L("Creating VGImage from EGLImage, which in turn based on uninitialized RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
    
    //check that we created all sgImages successfully
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        ASSERT_VG_TRUE(vgImages[count] != VG_INVALID_HANDLE);
        }
    
    //Destroying VGImage/EGLImage/RSgImages
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        vgDestroyImage(vgImages[count]);
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Destroying VGImage which based on RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        const TBool res = iEglSess->DestroyEGLImage(iDisplay, eglImages[count]);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
        ASSERT_EGL_TRUE(res)
#endif
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Destroying EGLImage which in turn based on RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        sgImages[count].Close();
        iProfiler->MarkResultSetL();
        }   
    iProfiler->ResultsAnalysis(_L("Closing RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
    
    //Creating initialized VGImage from initialized RSgImage
    const TInt KDataStride = iImageSize.iWidth * EglTestConversion::BytePerPixel(iPixelFormat);
    const TInt KDataSizeInByte = KDataStride *  iImageSize.iHeight;
    TInt* data = new (ELeave) TInt[KDataSizeInByte];
    User::LeaveIfNull(data);
    CleanupStack::PushL(data);
    
    Mem::Fill(data, KDataSizeInByte / 2, 0xff);
    Mem::FillZ(data + KDataSizeInByte / 2, KDataSizeInByte / 2);
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        const TInt res = sgImages[count].Create(imageInfo, data, KDataStride);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
        TESTL(res == KErrNone);
#endif
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Creating initialized RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
    
    //check that we created all sgImages successfully
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        TESTL(!sgImages[count].IsNull());
        }
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {//we will use preserved flag as sgImage supposedly has some content
        eglImages[count]= iEglSess->eglCreateImageKhrL(iDisplay, EGL_NO_CONTEXT, EGL_NATIVE_PIXMAP_KHR, 
                &sgImages[count], const_cast<EGLint *> (KEglImageAttribsPreservedTrue));
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Creating EGLImage from initialized RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    

    //check that we created all EglImages successfully
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        ASSERT_EGL_TRUE(eglImages[count] != EGL_NO_IMAGE_KHR)
        }
    
    iProfiler->InitResults();
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        vgImages[count] = iEglSess->vgCreateImageTargetKHR((VGeglImageKHR)eglImages[count]);
        iProfiler->MarkResultSetL();
        }   
    iProfiler->ResultsAnalysis(_L("Creating VGImage from EGLImage which in turn based on initialized RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    

    //check that we created all VgImages successfully
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        ASSERT_VG_TRUE(vgImages[count] != VG_INVALID_HANDLE);
        }
    
    CleanupStack::PopAndDestroy(data);    
    //Destroying VGImage/EGLImage/RSgImages
    //no need to profile as we have already done this before
    CleanupStack::PopAndDestroy(3, vgImageDeleteData);  // vgImageDeleteData, sgImageData, eglImageDeleteData  
    
    CleanAll();
    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    }

/**
@SYMTestCaseID GRAPHICS-EGL-0435

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    Performance test – Creating and destroying VGImage from an RSgImage via Open
@SYMTestActions
Environmental settings:
•   Image Size: w50 h50
•   Simulated Load: 0%
•   Pixel format ESgPixelFormatARGB_8888_PRE
•   Number of benchmark iteration: N

The method of creation of the RSgImage is similar to the case taken from GRAPHICS-EGL-RSGIMAGE_LITE-0406

From Process A:
Open the RSgDriver
Create an RSgImage
Spawn a client process B, passing in the drawable ID of the RSgImage
Wait for process B to exit

From process B:
Open RSgDriver
For i = 0 to N
    Start timer
    Open RSgImage[i]
    Stop timer and record time

    Start timer
    Create an EGLImage[i] from the RSgImage[i]
    Stop timer and record time

    Start timer
    Create a VGImage[i] from the EGLImage[i]
    Stop timer and record time

    Record total time of RSgImage[i] open and EGLImage[i] and VGImage[i] creation
End loop
Record average opening time of RSgImage, and average creation time of EGLImage, 
VGImage and the whole RSgImage-EGLImage-VGImage chain
For i = N to 0
    Start timer
    Close VGImage[i]
    Stop timer and record time

    Start timer
    Close EGLImage[i]
    Stop timer and record time

    Start timer
    Close RSgImage[i]
    Stop timer and record time

    Record total time for closing VGImage[i], EGLImage[i] and RSgImage[i]
End loop
Record average closing time of RSgImage, EGLImage, VGImage, and the whole VGImage-EGLImage-RSgImage chain

Close RSgDriver
Exit

@SYMTestExpectedResults
    The creation/opening of RSgImage, EGLImage and VGImage must return no error. 
    The average time shall be made available in an easy-to-use format 
    for further analysis and comparison. All allocated image memory should be freed
*/
TVerdict CEglTest_Benchmark_Multi_Process_CreateCloseImage::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0435"));
    SetTestStepName(KBenchmark_Multi_Process_CreateCloseImage);
    INFO_PRINTF1(_L("CEglTest_Benchmark_Multi_Process_CreateCloseImage::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
	INFO_PRINTF1(_L("CEglTest_Benchmark_Multi_Process_CreateCloseImage can only be run with SgImage-Lite"));
	RecordTestResultL();
	CloseTMSGraphicsStep();
	return TestStepResult();
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(!ret)
        {
        // The extension is not supported
        RecordTestResultL();
        CloseTMSGraphicsStep();
        return TestStepResult();
        }

    // launch 2 processes
    Test_MultiProcessL(KEglTestStepDllName, 2, TestStepName());
    
    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    }

void CEglTest_Benchmark_Multi_Process_CreateCloseImage::doProcessFunctionL(TInt aIdx)
    {
    INFO_PRINTF2(_L("CEglTest_Benchmark_Multi_Process_CreateCloseImage::doProcessFunctionL, Process %d"),aIdx);
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    GetDisplayL();
    CreateEglSessionL(aIdx);
    iEglSess->InitializeL();    
    iEglSess->OpenSgDriverL();

    TSgImageInfo imageInfo;
    imageInfo.iUsage = ESgUsageBitOpenVgImage;
    imageInfo.iPixelFormat = iPixelFormat;
    imageInfo.iSizeInPixels = iImageSize;
    RArray<TSgDrawableId> sgIdImageList; 
    
    //Create an array of SgImages and push them into cleanup stack
    //sgImageData takes ownership of the SgImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    RSgImage* sgImages = NULL;
    CSgImageDeleteData* sgImageData = new (ELeave)CSgImageDeleteData(sgImages, iNumIterations);
    CleanupStack::PushL(sgImageData);    
    sgImages = new (ELeave) RSgImage[iNumIterations];
    
    RMsgQueue<TSgDrawableId> messageQueue;
    User::LeaveIfError(messageQueue.Open(EProcSlotMsgQueueSgId, EOwnerProcess));
    CleanupClosePushL(messageQueue);

    //create  iNumIterations number of SgImages in one process and send them over to the second process
    INFO_PRINTF3(_L("Process %d, Start sending %d SgImage IDs to other process..."), aIdx, iNumIterations);
    if(aIdx == 0)
        {
        for(TInt count=0; count < iNumIterations; ++count)
            {
            const TInt res = sgImages[count].Create(imageInfo, NULL);
            TESTL(res == KErrNone);
            messageQueue.SendBlocking(sgImages[count].Id());
            }
        }
    else if(aIdx == 1)
        {
        for(TInt count=0; count < iNumIterations; ++count)
            {
            TSgDrawableId sgImageId;
            messageQueue.ReceiveBlocking(sgImageId);
            sgIdImageList.AppendL(sgImageId);
            }
        }
    INFO_PRINTF3(_L("Process %d, Finish sending %d SgImage IDs to other process..."), aIdx, iNumIterations);
    
    CleanupStack::PopAndDestroy(&messageQueue); 
    
    // Wait for both processes to reach this point
    // This is to be sure that there is no context 
    //switching whilst the profiling occurs
    Rendezvous(aIdx);
    if(aIdx == 1)
        {
        imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
        //create a dummy surface and context for the purpose of enabling use of VG
        iEglSess->CreatePixmapSurfaceAndMakeCurrentAndMatchL(imageInfo, CTestEglSession::EResourceCloseSgImageLate);
        
        iProfiler->InitResults();
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            const TInt res = sgImages[count].Open(sgIdImageList[count]);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
            TESTL(res == KErrNone);
#endif            
            iProfiler->MarkResultSetL();
            }
        iProfiler->ResultsAnalysis(_L("Open RSgImage from another process"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
        
        //check that we created all sgImages successfully
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            TESTL(!sgImages[count].IsNull());
            }
        
        //Create an array of EglImages and push them into cleanup stack
        //eglImageDeleteData takes ownership of EglImages array. 
        //If leaving occurs or this object is destroyed from the cleanup stack 
        //it will delete all images and then the array
        EGLImageKHR* eglImages = NULL;
        CEglImageDeleteData* eglImageDeleteData = new (ELeave)CEglImageDeleteData(*this, eglImages, iDisplay, iNumIterations); 
        CleanupStack::PushL(eglImageDeleteData);    
        eglImages = new (ELeave) EGLImageKHR[iNumIterations];
        
        iProfiler->InitResults();
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {//we will use preserved flag as sgImage supposedly has some content
            eglImages[count]= iEglSess->eglCreateImageKhrL(iDisplay,EGL_NO_CONTEXT,EGL_NATIVE_PIXMAP_KHR,&sgImages[count],const_cast<EGLint *> (KEglImageAttribsPreservedTrue));
            iProfiler->MarkResultSetL();
            }
        iProfiler->ResultsAnalysis(_L("Creating EGLImage from RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
        
        //check that we created all EglImages successfully
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            ASSERT_EGL_TRUE(eglImages[count] != EGL_NO_IMAGE_KHR)
            }
        
        //Create an array of VGImages and push them into cleanup stack
        //vgImageDeleteData takes ownership of VgImages array. 
        //If leaving occurs or this object is destroyed from the cleanup stack 
        //it will delete all images and then the array
        VGImage* vgImages = NULL;
        CVgImageDeleteData* vgImageDeleteData = new (ELeave) CVgImageDeleteData(vgImages, iNumIterations);
        CleanupStack::PushL(vgImageDeleteData);
        vgImages = new (ELeave) VGImage[iNumIterations];
        
        iProfiler->InitResults();
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            vgImages[count] = iEglSess->vgCreateImageTargetKHR((VGeglImageKHR)eglImages[count]);
            iProfiler->MarkResultSetL();
            }   
        iProfiler->ResultsAnalysis(_L("Creating VGImage from EGLImage, which in turn based on SgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   

        //check that we created all VgImages successfully
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            ASSERT_VG_TRUE(vgImages[count] != VG_INVALID_HANDLE);
            }   
        
        sgIdImageList.Reset();
        
        //Destroying VGImage/EglImage/RSgImages
        iProfiler->InitResults();
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            vgDestroyImage(vgImages[count]);
            iProfiler->MarkResultSetL();
            }
        iProfiler->ResultsAnalysis(_L("Destroying VGImage based on EGLImage which in turn based on RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
        
        iProfiler->InitResults();
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            const TBool res = iEglSess->DestroyEGLImage(iDisplay, eglImages[count]);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
            ASSERT_EGL_TRUE(res);
#endif            
            iProfiler->MarkResultSetL();
            }
        iProfiler->ResultsAnalysis(_L("Destroying EGLImage which based on RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
        
        iProfiler->InitResults();
        for(TInt count=iNumIterations-1; count >= 0; --count)
            {
            sgImages[count].Close();
            iProfiler->MarkResultSetL();
            }   
        iProfiler->ResultsAnalysis(_L("Closing RSgImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);    
        CleanupStack::PopAndDestroy(2, eglImageDeleteData); //eglImageDeleteData, vgImageDeleteData     
        }

    // Wait for both processes to reach this point
    Rendezvous(aIdx);
    CleanupStack::PopAndDestroy(sgImageData);
    CleanAll();
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    }


/**
@SYMTestCaseID GRAPHICS-EGL-0436

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    Performance test - Drawing VGImage
@SYMTestActions
Environmental settings:
•   Image Size: w50 h50
•   Simulated Load: 0%
•   Pixel format ESgPixelFormatARGB_8888_PRE
•   Number of benchmark iteration: N (may vary depending on hardware capacity)

Creating regular VGImage
Init EGL, create context and surface
For i = 0 to N
    Create VGImage[i] with size: 50x50 and format:  VG_sARGB_8888_PRE
    SetSubData for VGImage[i] 
End loop

Draw VGImage
For i = N to 0
    Start timer
    Draw VGImage[i]
    Stop timer and record time
End loop
Record average time of drawing VGImage

Closing regular VGImage
For i = N to 0
    Destroy VGImage[i]
End loop

Creating VGImage from initialized RSgImage
Open RSgDriver
Construct TSgImageInfo object with
•   Size: 50x50
•   PixelFormat: ESgPixelFormatARGB_8888_PRE
•   Usage: ESgUsageBitOpenVgImage;

For i = N to 0
    Create RSgImage[i] with arguments TSgImageInfo with initialized data which were supplied to regular VGImage 
    Create an EGLImage[i] from the RSgImage[i]
    Create a VGImage[i] from the EGLImage[i]
End loop

Draw VGImage which is based on RSgImage
For i = N to 0
    Start timer
    Draw VGImage[i]
    Stop timer and record time
End loop
Record average time of drawing VGImage

Close all VGImages, EGLImages and RSgImages
Close RSgDriver

@SYMTestExpectedResults
The creation of RSgImage, EGLImage and VGImage must return no error. 
The average drawing time of regular VGImage and VGImage with underlying RSgImage is 
made available in an easy-to-use format for further analysis and comparison.
*/
TVerdict CEglTest_Benchmark_DrawImage::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0436"));
    INFO_PRINTF1(_L("CEglTest_Benchmark_DrawImage::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    INFO_PRINTF1(_L("CEglTest_Benchmark_DrawImage can only be run with SgImage-Lite"));
    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(!ret)
        {
        // The extension is not supported
        RecordTestResultL();
        CloseTMSGraphicsStep();
        return TestStepResult();
        }
 
    ASSERT_TRUE(iDisplay == EGL_NO_DISPLAY);
    GetDisplayL();
    CreateEglSessionL();
    iEglSess->InitializeL();
    iEglSess->OpenSgDriverL();
 //----create pixmap and make context curent
    TSgImageInfo imageInfo;
    imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
    imageInfo.iPixelFormat = iPixelFormat;
    imageInfo.iSizeInPixels = iImageSize;
    //create a dummy surface and context for the purpose of enabling use of VG
    iEglSess->CreatePixmapSurfaceAndMakeCurrentAndMatchL(imageInfo, CTestEglSession::EResourceCloseSgImageLate);

    const TInt KDataStride = iImageSize.iWidth * EglTestConversion::BytePerPixel(iPixelFormat);
    const TInt KDataSizeInByte = KDataStride *  iImageSize.iHeight;
    TInt* data = new (ELeave) TInt[KDataSizeInByte];
    User::LeaveIfNull(data);
    CleanupStack::PushL(data);
    
    Mem::Fill(data, KDataSizeInByte / 2, 0xff);
    Mem::FillZ(data + KDataSizeInByte / 2, KDataSizeInByte / 2);

    //Creating regular VGImages
    //Create an array of VGImages and push them into cleanup stack
    //vgImageDeleteData takes ownership of the VGImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    VGImageFormat vgPixelFormat = EglTestConversion::PixelFormatToVgFormat(iPixelFormat);
    VGImage* vgImages = NULL;
    CVgImageDeleteData* vgImageDeleteData = new (ELeave) CVgImageDeleteData(vgImages, iNumIterations);
    CleanupStack::PushL(vgImageDeleteData);    
    vgImages = new (ELeave)VGImage[iNumIterations];
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        //we will use VG_IMAGE_QUALITY_NONANTIALIASED flag to avoid OpenVG making the quality improvements
        //at the expense of performance (for instance to create an extra buffer) 
        vgImages[count] = vgCreateImage(vgPixelFormat, iImageSize.iWidth, iImageSize.iHeight, VG_IMAGE_QUALITY_NONANTIALIASED);
        ASSERT_VG_TRUE(vgImages[count] !=  VG_INVALID_HANDLE);
        
        vgImageSubData(vgImages[count],
                data, KDataStride,
                vgPixelFormat,
                0, 0, iImageSize.iWidth, iImageSize.iHeight);
        ASSERT_VG_ERROR(VG_NO_ERROR);
        }
    
    //--------- start profiling
    iProfiler->InitResults();
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        vgDrawImage(vgImages[count]);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
        ASSERT_VG_ERROR(VG_NO_ERROR);
#endif            
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Drawing regular VGImage"), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
    //--------- stop profiling
    
    //destroy vgImages
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        vgDestroyImage(vgImages[count]);
        ASSERT_VG_ERROR(VG_NO_ERROR);
        vgImages[count]=VG_INVALID_HANDLE;
        }
    
    //Create an array of SgImages and push them into cleanup stack
    //sgImageData takes ownership of the SgImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    imageInfo.iUsage = ESgUsageBitOpenVgImage;
    RSgImage* sgImages = NULL;
    CSgImageDeleteData* sgImageData = new (ELeave)CSgImageDeleteData(sgImages, iNumIterations);
    CleanupStack::PushL(sgImageData);    
    sgImages = new (ELeave) RSgImage[iNumIterations];
    
    //Create an array of EglImages and push them into cleanup stack
    //eglImageDeleteData takes ownership of the EglImages array. 
    //If leaving occurs or this object is destroyed from the cleanup stack 
    //it will delete all images and then the array
    EGLImageKHR* eglImages = NULL;
    CEglImageDeleteData* eglImageDeleteData = new (ELeave)CEglImageDeleteData(*this, eglImages, iDisplay, iNumIterations); 
    CleanupStack::PushL(eglImageDeleteData);    
    eglImages = new (ELeave) EGLImageKHR[iNumIterations];
    
    //Creating VGImage from initialized RSgImage
    for(TInt count=iNumIterations-1; count >= 0; --count)
        {
        const TInt res = sgImages[count].Create(imageInfo, data, KDataStride);
        TESTL(res == KErrNone);
        TESTL(!sgImages[count].IsNull());

        eglImages[count]= iEglSess->eglCreateImageKhrL(iDisplay,EGL_NO_CONTEXT,EGL_NATIVE_PIXMAP_KHR,&sgImages[count],const_cast<EGLint *> (KEglImageAttribsPreservedTrue));
        ASSERT_EGL_TRUE(eglImages[count] != EGL_NO_IMAGE_KHR)

        vgImages[count] = iEglSess->vgCreateImageTargetKHR((VGeglImageKHR)eglImages[count]);
        ASSERT_VG_TRUE(vgImages[count] != VG_INVALID_HANDLE);
        }
    
    //---------------start profiling
    iProfiler->InitResults();
    for(TInt count=iNumIterations - 1; count >= 0; --count)
        {
        vgDrawImage(vgImages[count]);
#ifdef ENABLE_CHECKING_WHILST_PROFILING
        ASSERT_VG_ERROR(VG_NO_ERROR);
#endif            
        iProfiler->MarkResultSetL();
        }
    iProfiler->ResultsAnalysis(_L("Drawing VGImage with underlying RSgImage "), 0, EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), EglTestConversion::PixelFormatToDisplayMode(iPixelFormat), iNumIterations);   
    //----------------stop profiling
    
    //Destroying VGImage/EGLImage/RSgImages
    //no need to profile as we have already done this before
    CleanupStack::PopAndDestroy(4, data);  // data, vgImageDeleteData, sgImageData, eglImageDeleteData  
    
    CleanAll();
    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    }
