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

#include <test/egltestcommonprocess.h>
#include <test/egltestcommonconversion.h>
#include <test/egltestcommoninisettings.h>
#include "egltest_oom_sgimage.h"


_LIT(KOOMSection, "OOM");


//Since we want to exhaust the memory it probably makes sense to use any 32bpp mode  
//There is no need to put it into INI file
const TUidPixelFormat KOOMPixelFormat = EUidPixelFormatARGB_8888; 


CEglTest_OOM_Base::~CEglTest_OOM_Base()
    {
    CleanGraphicsResources();
    CleanAll();
    }

TVerdict CEglTest_OOM_Base::doTestStepPreambleL()
    {
    TVerdict verdict = CEglTestStep::doTestStepPreambleL();
    //read all parameters from config
    CEglTestCommonIniSettings* iniParser = CEglTestCommonIniSettings::NewL();
    CleanupStack::PushL(iniParser);
    iNumIterations = iniParser->GetNumberOfIterations(KOOMSection);
    if(!iNumIterations)
        {
        ERR_PRINTF1(_L("The number iterations is not specified in INI file, the test will not be executed!"));
        User::Leave(KErrArgument);      
        }
    
    iImageSize = iniParser->GetImageSize(KOOMSection);
    if(iImageSize == TSize(0,0))
        {
        ERR_PRINTF1(_L("The image size whether is not specified in INI file or is TSize(0,0), the test will not be executed!"));
        User::Leave(KErrArgument);      
        }
    iPixelFormat = KOOMPixelFormat;
    
    iThresholdGPUUsedMemory = iniParser->GetThresholdGPUUsedMemory(KOOMSection);
    if(iThresholdGPUUsedMemory == 0)
        {
        ERR_PRINTF1(_L("Threshold GPU used memory whether is not specified in INI file or is 0, the test will not be executed!"));
        User::Leave(KErrArgument);      
        }

    iThresholdLastIteration = iniParser->GetThresholdLastIteration(KOOMSection);
    if(iThresholdLastIteration == 0)
        {
        ERR_PRINTF1(_L("Threshold last iteration whether is not specified in INI file or is 0, the test will not be executed!"));
        User::Leave(KErrArgument);      
        }
    
    CleanupStack::PopAndDestroy(iniParser);

    INFO_PRINTF4(_L("**** The test will be run in following configuration: number of iterations %d, image size (%d, %d)"), iNumIterations, iImageSize.iWidth, iImageSize.iHeight);
    INFO_PRINTF3(_L("**** Threshold GPU used memory %d, threshold last iteration %d"), iThresholdGPUUsedMemory, iThresholdLastIteration);
    
    PrintUsedPixelConfiguration();    
    return verdict;
    }

TVerdict CEglTest_OOM_Base::doTestStepPostambleL()
    {
    //to keep heap checking happy we have to clean up before destructor 
    CleanGraphicsResources();
    return CEglTestStep::doTestStepPostambleL();
    }

//receive last successful index from the client process
void CEglTest_OOM_Base::ReceiveMessageFromClient(RMsgQueue<TEglStepMessageBuffer>& aMessageQueueClientProcParam) 
    {
    TEglStepMessageBuffer param;
    aMessageQueueClientProcParam.ReceiveBlocking(param);
    iLastIterationNumber = *(TInt*) (param.iBuf);
    }

//send last successful index to the main process for analysis
void CEglTest_OOM_Base::SendIndexToMainProcessL(TInt aIndex)
    {
    TEglStepMessageBuffer param;
    RMsgQueue<TEglStepMessageBuffer> messageQueueClientProcParam;
    CleanupClosePushL(messageQueueClientProcParam);
    User::LeaveIfError(messageQueueClientProcParam.Open(EProcSlotCustomClientParam, EOwnerProcess));
    *((TInt*)param.iBuf) = aIndex;
    messageQueueClientProcParam.SendBlocking(param);
    CleanupStack::PopAndDestroy(&messageQueueClientProcParam);
    }

//clean Sg/Egl/Vg images allocated. Reset arrays for various deviation variables.
void CEglTest_OOM_Base::CleanGraphicsResources()
    {
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    while(iSurfaces.Count() > 0)
        {
        eglDestroySurface(iDisplay, iSurfaces[0]);
        iSurfaces.Remove(0);
        }
    iSurfaces.Reset();

    while(iVgImages.Count() > 0)
        {
        vgDestroyImage(iVgImages[0]);
        iVgImages.Remove(0);
        }
    iVgImages.Reset();

    while(iEglImages.Count() > 0)
        {
        iEglSess->DestroyEGLImage(iDisplay, iEglImages[0]);
        iEglImages.Remove(0);
        }
    iEglImages.Reset();
    
    while(iSgImages.Count() > 0)
        {
        iSgImages[0].Close();
        iSgImages.Remove(0);
        }
    iSgImages.Reset();

    iGPUUsedMemory.Reset();
    iLastIterations.Reset();
#endif
    }

//if an array is empty, it returns zero for both aMin and aMax
void CEglTest_OOM_Base::GetMinMax(const RArray<TInt>& aArray, TInt& aMin, TInt& aMax) const
    {
    aMin = 0;
    aMax = 0;
    if(aArray.Count() == 0)
        return;
    aMax = aArray[0];
    aMin = aArray[0];
    for(TInt ii = 1; ii < aArray.Count(); ii++)
        {
        if(aMin > aArray[ii])
            {
            aMin = aArray[ii];
            }
        if(aMax < aArray[ii])
            {
            aMax = aArray[ii];
            }
        }
    }

TInt CEglTest_OOM_Base::Deviation(const RArray<TInt>& aArray) const
    {
    TInt min = 0;
    TInt max = 0;
    
    GetMinMax(aArray, min, max);
    if(max == 0) 
        return 0; // to avoid division by zero
    return (max - min) / (((TReal)(min + max)) / 2) * 100;  
    }

//Calculate and output deviation of various parameters. 
//If the measurement for particular parameter doesn’t take place, for instance, 
//due to absence of the extension, the output will be skipped. 
void CEglTest_OOM_Base::CheckDeviation()
    {
    TInt res = KErrNone;
    
    if(iGPUUsedMemory.Count() > 0)
        {
        res = Deviation(iGPUUsedMemory);
        TEST(iThresholdGPUUsedMemory >= res);
        INFO_PRINTF3(_L("GPU used memory deviation %d %%, threshold %d %%"), res, iThresholdGPUUsedMemory);
        }
    
    if(iLastIterations.Count() > 0)
        {
        res = Deviation(iLastIterations);
        TEST(iThresholdLastIteration >= res);
        INFO_PRINTF3(_L("Last iteration deviation %d %%, threshold %d %%"), res, iThresholdLastIteration);
        }
    }

void CEglTest_OOM_Base::RetrieveExtensionDataL()
    {
    PrintEglResourceProfilingInfoL();
    INFO_PRINTF2(_L("Nember iterations before the failure occurs, %d"), iLastIterationNumber);
    iLastIterations.Append(iLastIterationNumber);
    iLastIterationNumber = -1;
    }

//Print GPU information provided be NOK_resource_profiling2 egl extension
//Some data, like GPU usage, will be memorized for further comparison
//This extension is optional and may not be present in the system
void CEglTest_OOM_Base::PrintEglResourceProfilingInfoL()
    {
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
#ifdef EGL_PROF_MEMORY_USAGE_THRESHOLD_NOK    

    GetDisplayL();
    CreateEglSessionL();
    iEglSess->InitializeL();    
    
    if(!iPFnEglQueryProfilingDataNOK)
        {
        iPFnEglQueryProfilingDataNOK = (PFNEGLQUERYPROFILINGDATANOKPROC) eglGetProcAddress("eglQueryProfilingDataNOK");
    
        if(!iPFnEglQueryProfilingDataNOK)
            {
            CleanAll();
            WARN_PRINTF1(_L("NOK_resource_profiling2 extension is not available"));
            return;
            }
        }
        
    EGLint data_count;
    // Find out how much profiling data is available 
    iPFnEglQueryProfilingDataNOK(iDisplay, 
                             EGL_PROF_QUERY_GLOBAL_BIT_NOK | 
                             EGL_PROF_QUERY_MEMORY_USAGE_BIT_NOK,
                             NULL,
                             0,
                             &data_count);

    // Allocate room for the profiling data
    EGLint* prof_data = (EGLint*)User::AllocL(data_count * sizeof(EGLint));

    CleanupStack::PushL(prof_data);
    
    // Retrieve the profiling data 
    iPFnEglQueryProfilingDataNOK(iDisplay, 
                             EGL_PROF_QUERY_GLOBAL_BIT_NOK | 
                             EGL_PROF_QUERY_MEMORY_USAGE_BIT_NOK,
                             prof_data,
                             data_count,
                             &data_count);

    // Iterate over the returned data 
    EGLint i = 0;
    while (i < data_count)
    {
        switch (prof_data[i++])
        {
        case EGL_PROF_TOTAL_MEMORY_NOK:
            INFO_PRINTF2(_L("Total memory: %d"), prof_data[i++]);
            break;
        case EGL_PROF_USED_MEMORY_NOK:
            iGPUUsedMemory.AppendL(prof_data[i]);
            INFO_PRINTF2(_L("Used memory: %d"), prof_data[i++]);
            break;
        case EGL_PROF_PROCESS_ID_NOK:    
            if(sizeof(EGLNativeProcessIdTypeNOK) == 4)
                {
                INFO_PRINTF2(_L("Process ID(4 bytes), 0x%08X"), prof_data[i++]);
                }
            else if(sizeof(EGLNativeProcessIdTypeNOK) == 8)
                {
                EGLNativeProcessIdTypeNOK processId = ((EGLNativeProcessIdTypeNOK)(prof_data[i])) + (((EGLNativeProcessIdTypeNOK)(prof_data[i + 1]))<<32);
                RProcess process;
                TProcessId pid(processId);
                TInt err = process.Open(pid);
                if (err == KErrNone)
                    {
                    TPtrC ptr(process.FullName());
                    INFO_PRINTF4(_L("Process ID, %lu - 0x%lu - %S"), processId, processId, &ptr);
                    process.Close();
                    }
                else
                    {//this parameter is not in use in the test, thus is no point to set an error.
                    WARN_PRINTF4(_L("Process ID, %lu - 0x%lx, fail to open process with error %d"), processId, processId, err);
                    }                    
                i += 2;
                }
            else
                {//this parameter is for information only. It doesn't impact our measurement. So there is no need to set an error.
                WARN_PRINTF1(_L("Unknown EGLNativeProcessIdTypeNOK"));
                }
            break;
        case EGL_PROF_PROCESS_USED_PRIVATE_MEMORY_NOK:
            INFO_PRINTF2(_L("Process used private memory: %d"), prof_data[i++]);
            break;
        case EGL_PROF_PROCESS_USED_SHARED_MEMORY_NOK:
            INFO_PRINTF2(_L("Process used shared memory: %d"), prof_data[i++]);
            break;
        }
    }

    // Free allocated memory 
    CleanupStack::PopAndDestroy(prof_data);
    CleanAll();
    
#endif //EGL_PROF_MEMORY_USAGE_THRESHOLD_NOK    
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE   
    }

/**
@SYMTestCaseID GRAPHICS-EGL-0438

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    OOM test – Free VG/Egl/Sg Images while the process which owns them is terminated

@SYMTestActions
Environmental settings:
•   Image Size: w50 h50
•   List of simulated load: 0%
•   List of pixel formats
ESgPixelFormatARGB_8888
•   Client process priorities - all the same
•   Client process random parameters:
-   None

The creation of RSgImages and launching of processes is along the lines of the method outlined in GRAPHICS-EGL-RSGIMAGE_LITE-0406

    From the main process:
    For i = 0 to N
    Spawn 2 client processes A and B.
        Signal all client processes to start by use of a semaphore
        Wait until client processes exit
    If the test fails not due to the memory allocation record an error code to the log file then set a test result as a failure and skip further actions.
    End loop
    Exit

    From client process A:
    Get EGL display
    Initialize EGL
    Open RSgDriver
    Create context, pbuffer surface.
    Make pbuffer surface current for the given context  
    Loop until exit condition met
    Start loop:
Create SgImage
Create EglImage with underlying SgImage
    Create VgImage with underlying EglImage
    Exit condition – Sg/Egl/Vg image creation has failed.
End loop:
    Destroy the pbuffer surface
Log the last iteration number and exact operation which precedes a failure. 
In the environment supporting NOK_resource_profiling2 extension retrieve for further analyzes the following GPU profiling data (if available):
•   Total memory
•   Used memory
•   Process ID
•   Process used private memory
•   Process used shared memory

Make the process busy by putting it into the indefinite loop.
    
    From client process B:
    Wait until process A fails with the image creation.
    Terminate the process A.

@SYMTestExpectedResults
For each step from 0 to N in the main process, 
-   Image allocation failure must happen at approximately the same iteration in process A. 
    MaxIterationNumber – MinIterationNumber < Threashold, where Treashold will not 
    exceeds 5 and exact value to be defined during implementation. 
-   GPU memory usage retrieved through NOK_resource_profiling2 extension, if available, 
    is consistent and doesn’t decrease over the time. 
    MaxGPUMemoryUsage – MinGPUMemoryUsage < Threshold, where Threshold will not exceed 
    5 and exact value to be defined during implementation. 
*/
TVerdict CEglTest_OOM_CloseVGImageWithTermination::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0438"));
    SetTestStepName(KOOM_CloseVGImageWithTermination);
    INFO_PRINTF1(_L("CEglTest_Benchmark_Multi_Process_CreateCloseImage::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
	INFO_PRINTF1(_L("CEglTest_OOM_CloseVGImageWithTermination can only be run with SgImage-Lite"));
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(ret)
        {
        // The extension is supported
        // if the test fails not due to the memory allocation, then skip further actions
        for(TInt index = 0; (index < iNumIterations) && (TestStepResult()== EPass); index++)
            {
            // launch 2 processes
            Test_MultiProcessL(KEglTestStepDllName, 2, TestStepName());
            RetrieveExtensionDataL();
            }
        CheckDeviation();
        }
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE

    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
    }

void CEglTest_OOM_CloseVGImageWithTermination::doProcessFunctionL(TInt aIdx)
    {
    INFO_PRINTF2(_L("CEglTest_OOM_CloseVGImageWithTermination::doProcessFunctionL, Process %d"),aIdx);
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE

    if(aIdx == 0)
        {
        GetDisplayL();
        CreateEglSessionL(aIdx);
        iEglSess->InitializeL();    
        iEglSess->OpenSgDriverL();

        //create a dummy surface and context for the purpose of enabling use of VG
        TEglTestConfig pbufferFormat = EglTestConversion::VgFormatToPBufferSurfaceFormat(EglTestConversion::PixelFormatToVgFormat(iPixelFormat));
        EGLConfig currentConfig = 0;
        TRAPD(res, currentConfig = iEglSess->GetConfigExactMatchL( pbufferFormat ));
        User::LeaveIfError(res);
        
        iEglSess->CreatePbufferSurfaceAndMakeCurrentL(currentConfig, iImageSize, EGL_OPENVG_API);
        TInt index = 0;
        TSgImageInfo imageInfo;
        imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
        imageInfo.iPixelFormat = iPixelFormat;
        imageInfo.iSizeInPixels = iImageSize;
        for(;;++index)
            {
            RSgImage sgImage;
            TInt res = sgImage.Create(imageInfo, NULL);
            if(res != KErrNone || sgImage.IsNull())
                {
                INFO_PRINTF5(_L("***Fail to create RSgImage after %d attempts, error: %d, expected %d or %d"), index, res, KErrNoMemory, KErrNoGraphicsMemory);
                TEST((res == KErrNoMemory) || (res == KErrNoGraphicsMemory));
                break;
                }
            EGLImageKHR eglImages = iEglSess->eglCreateImageKhrL(iDisplay,EGL_NO_CONTEXT,EGL_NATIVE_PIXMAP_KHR,&sgImage,const_cast<EGLint *> (KEglImageAttribsPreservedTrue));
            EGLint eglError = eglGetError();
            if((eglImages == EGL_NO_IMAGE_KHR) || (eglError != EGL_SUCCESS))
                {
                INFO_PRINTF4(_L("***Fail to create EGLImage after %d attempts, error: %d, expected: %d"), index, eglError, EGL_BAD_ALLOC);
                TEST(eglError == EGL_BAD_ALLOC);
                break;
                }

            VGImage vgImage = iEglSess->vgCreateImageTargetKHR((VGeglImageKHR)eglImages);
            VGErrorCode vgError = vgGetError();
            if(vgImage == VG_INVALID_HANDLE || (vgError != VG_NO_ERROR))
                {
                INFO_PRINTF4(_L("***Fail to create VGImage after %d attempts, error: %d, expected: %d"), index, vgError, VG_OUT_OF_MEMORY_ERROR);
                TEST(vgError == VG_OUT_OF_MEMORY_ERROR);
                break;
                }
            } //for
        SendIndexToMainProcessL(index);
        }
    Rendezvous(aIdx);
    
    //create the queue to send/receive Process ID between processes
    RMsgQueue<TProcessId> messageQueueProcId;
    User::LeaveIfError(messageQueueProcId.Open(EProcSlotMsgQueueProcId, EOwnerProcess));
    CleanupClosePushL(messageQueueProcId);
    
    if(aIdx == 0)
        {
        // Sending Process ID to other process... so that the other process can kill it.
        TProcessId procId = RProcess().Id();
        messageQueueProcId.SendBlocking(procId);
        CleanupStack::PopAndDestroy(&messageQueueProcId);
        //go into indefinite loop which will be terminated by the second process
        for(;;) { }
        }
    else
        {
        TProcessId procId;
        messageQueueProcId.ReceiveBlocking(procId);
        CleanupStack::PopAndDestroy(&messageQueueProcId);

        RProcess process;
        TESTL(process.Open(procId) == KErrNone);
        process.Kill(KErrNone);
        process.Close();
        
        // small delay to ensure the kernel finishes the clean-up
        User::After(1*1000*1000); // 1 second
        }
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    }

/**
@SYMTestCaseID GRAPHICS-EGL-0439

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    OOM test – Free VG/Egl/Sg Images while the process which owns them exits gracefully

@SYMTestActions
Environmental settings:
•   Image Size: w50 h50
•   List of simulated load: 0%
•   List of pixel formats
ESgPixelFormatARGB_8888
•   Client process priorities - all the same
•   Client process random parameters:
-   None

The creation of RSgImages and launching of processes is along the lines of the method outlined in GRAPHICS-EGL-RSGIMAGE_LITE-0406

    From the main process:
    For i = 0 to N
    Spawn 1 client process.
        Signal client process to start by use of a semaphore
        Wait until client process exits
    If the test fails not due to the memory allocation record an error code to the log file then set a test result as a failure and skip further actions.
    End loop
    Exit

    From client process A:
    Get EGL display
    Initialize EGL
    Open RSgDriver
    Create context, pbuffer surface.
    Make pbuffer surface current for the given context  
    Loop until exit condition met
    Start loop:
Create SgImage
Create EglImage with underlying SgImage
    Create VgImage with underlying EglImage
    Exit condition – Sg/Egl/Vg image creation has failed.
End loop:
    Destroy the pbuffer surface
Log the last iteration number and exact operation which precedes a failure. 
Close all allocated graphics resources (Sg/Egl/Vg images)
In the environment supporting NOK_resource_profiling2 extension, retrieve for further analyzes the following GPU profiling data (if available):
•   Total memory
•   Used memory
•   Process ID
•   Process used private memory
•   Process used shared memory

Terminate EGL
Close RSgDriver

@SYMTestExpectedResults
For each step from 0 to N in the main process, 
-   Image allocation failure must happen at approximately the same iteration in process A. 
    MaxIterationNumber – MinIterationNumber < Threashold, where Treashold will not 
    exceeds 5 and exact value to be defined during implementation. 
-   GPU memory usage retrieved through NOK_resource_profiling2 extension, if available, 
    is consistent and doesn’t decrease over the time.
    MaxGPUMemoryUsage – MinGPUMemoryUsage < Threshold, where Threshold will not exceed 
    5 and exact value to be defined during implementation. 
*/
TVerdict CEglTest_OOM_CloseVGImage::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0439"));
    SetTestStepName(KOOM_CloseVGImage);
    INFO_PRINTF1(_L("CEglTest_OOM_CloseVGImage::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    INFO_PRINTF1(_L("CEglTest_OOM_CloseVGImage can only be run with SgImage-Lite"));
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(ret)
        {
        // if the test fails not due to the memory allocation, then skip further actions
        for(TInt index = 0; (index < iNumIterations) && (TestStepResult()== EPass); index++)
            {
            // launch 1 process
            Test_MultiProcessL(KEglTestStepDllName, 1, TestStepName());
            RetrieveExtensionDataL();
            }
        CheckDeviation();
        }
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE

    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
    }

void CEglTest_OOM_CloseVGImage::doProcessFunctionL(TInt aIdx)
    {
    INFO_PRINTF2(_L("CEglTest_OOM_CloseVGImageWithTermination::doProcessFunctionL, Process %d"),aIdx);
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    GetDisplayL();
    CreateEglSessionL(aIdx);
    iEglSess->InitializeL();    
    iEglSess->OpenSgDriverL();

    //create a dummy surface and context for the purpose of enabling use of VG
    TEglTestConfig pbufferFormat = EglTestConversion::VgFormatToPBufferSurfaceFormat(EglTestConversion::PixelFormatToVgFormat(iPixelFormat));
    EGLConfig currentConfig = 0;
    TRAPD(res, currentConfig = iEglSess->GetConfigExactMatchL( pbufferFormat ));
    User::LeaveIfError(res);

    iEglSess->CreatePbufferSurfaceAndMakeCurrentL(currentConfig, iImageSize, EGL_OPENVG_API);
    TInt index = 0;
    TSgImageInfo imageInfo;
    imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
    imageInfo.iPixelFormat = iPixelFormat;
    imageInfo.iSizeInPixels = iImageSize;

    for(;;++index)
        {
        RSgImage sgImage;
        TInt res = sgImage.Create(imageInfo, NULL);
        if(res != KErrNone || sgImage.IsNull())
            {
            INFO_PRINTF5(_L("***Fail to create RSgImage after %d attempts, error: %d, expected: %d or %d"), index, res, KErrNoMemory, KErrNoGraphicsMemory);
            TEST((res == KErrNoMemory) || (res == KErrNoGraphicsMemory));
            break;
            }
        iSgImages.AppendL(sgImage);
        
        EGLImageKHR eglImage = iEglSess->eglCreateImageKhrL(iDisplay,EGL_NO_CONTEXT,EGL_NATIVE_PIXMAP_KHR,&sgImage,const_cast<EGLint *> (KEglImageAttribsPreservedTrue));
        EGLint eglError = eglGetError();
        if((eglImage == EGL_NO_IMAGE_KHR) || (eglError != EGL_SUCCESS))
            {
            INFO_PRINTF4(_L("***Fail to create EGLImage after %d attempts, error: %d, expected: %d"), index, eglError, EGL_BAD_ALLOC);
            TEST(eglError == EGL_BAD_ALLOC);
            break;
            }
        iEglImages.AppendL(eglImage);
        
        VGImage vgImage = iEglSess->vgCreateImageTargetKHR((VGeglImageKHR)eglImage);
        VGErrorCode vgError = vgGetError();
        if(vgImage == VG_INVALID_HANDLE || (vgError != VG_NO_ERROR))
            {
            INFO_PRINTF4(_L("***Fail to create VGImage after %d attempts, error: %d, expected: %d"), index, vgError, VG_OUT_OF_MEMORY_ERROR);
            TEST(vgError == VG_OUT_OF_MEMORY_ERROR);
            break;
            }
        iVgImages.AppendL(vgImage);
        }

    SendIndexToMainProcessL(index);
    
    //now clean everything
    CleanGraphicsResources();
    iEglSess->CloseSgDriver();
    CleanAll();
#endif    
    }    

/**
@SYMTestCaseID GRAPHICS-EGL-0440

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    OOM test – Free SgImages/Pixmap surfaces while the process which owns them is terminated

@SYMTestActions
Environmental settings:
•   Image Size: w50 h50
•   List of simulated load: 0%
•   List of pixel formats
ESgPixelFormatARGB_8888
•   Client process priorities - all the same
•   Client process random parameters:
-   None

The creation of RSgImages and launching of processes is along the lines of the method outlined in GRAPHICS-EGL-RSGIMAGE_LITE-0406

    From the main process:
    For i = 0 to N
        Spawn 2 client processes A and B.
        Signal all client processes to start by use of a semaphore
        Wait until client processes exit
    If the test fails not due to the memory allocation record an error code to the log file then set a test result as a failure and skip further actions.
    End loop
    Exit

    From client process A:
    Get EGL display
    Initialize EGL
    Open RSgDriver
    Loop until exit condition met
    Start loop:
        Create SgImage
        Create Pixmap surface with underlying SgImage
        Exit condition – SgImage/Pixmap surface creation has failed.
    End loop:
    Log the last iteration number and exact operation which precedes a failure. 
    In the environment supporting NOK_resource_profiling2 extension retrieve for further analyzes the following GPU profiling data (if available):
        •   Total memory
        •   Used memory
        •   Process ID
        •   Process used private memory
        •   Process used shared memory
    Make the process busy by putting it into the indefinite loop.
    
    From client process B:
    Wait until process A fails with the image/surface creation.
    Terminate the process A.

@SYMTestExpectedResults
For each step from 0 to N in the main process, 
-   Image or surface allocation failure must happen at approximately the same iteration 
    in process A. MaxIterationNumber – MinIterationNumber < Threashold, 
    where Treashold will not exceeds 5 and exact value to be defined during implementation. 
-   GPU memory usage retrieved through NOK_resource_profiling2 extension, if available, 
    is consistent and doesn’t decrease over the time.
    MaxGPUMemoryUsage – MinGPUMemoryUsage < Threshold, where Threshold will not exceed 
    5 and exact value to be defined during implementation. 
*/
TVerdict CEglTest_OOM_ClosePixmapSurfaceWithTermination::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0438"));
    SetTestStepName(KOOM_ClosePixmapSurfaceWithTermination);
    INFO_PRINTF1(_L("CEglTest_OOM_ClosePixmapSurfaceWithTermination::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    INFO_PRINTF1(_L("CEglTest_OOM_ClosePixmapSurfaceWithTermination can only be run with SgImage-Lite"));
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(ret)
        {
        // if the test fails not due to the memory allocation, then skip further actions
        for(TInt index = 0; (index < iNumIterations) && (TestStepResult()== EPass); index++)
            {
            // launch 2 processes
            Test_MultiProcessL(KEglTestStepDllName, 2, TestStepName());
            RetrieveExtensionDataL();
            }
        CheckDeviation();
        }
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE

    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
    }

void CEglTest_OOM_ClosePixmapSurfaceWithTermination::doProcessFunctionL(TInt aIdx)
    {
    INFO_PRINTF2(_L("CEglTest_OOM_ClosePixmapSurfaceWithTermination::doProcessFunctionL, Process %d"),aIdx);
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE

    if(aIdx == 0)
        {
        GetDisplayL();
        CreateEglSessionL(aIdx);
        iEglSess->InitializeL();    
        iEglSess->OpenSgDriverL();

        TInt index = 0;
        TSgImageInfo imageInfo;
        imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
        imageInfo.iPixelFormat = iPixelFormat;
        imageInfo.iSizeInPixels = iImageSize;

        for(;;++index)
            {
            RSgImage sgImage;
            TInt res = sgImage.Create(imageInfo, NULL);
            if(res != KErrNone || sgImage.IsNull())
                {
                INFO_PRINTF5(_L("***Fail to create RSgImage after %d attempts, error: %d, expected: %d or %d"), index, res, KErrNoMemory, KErrNoGraphicsMemory);
                TEST((res == KErrNoMemory) || (res == KErrNoGraphicsMemory));
                break;
                }
            
            EGLConfig currentConfig = 0;
            const EGLint KAttrib[] = { EGL_MATCH_NATIVE_PIXMAP,   (TInt)&sgImage,
                                       EGL_RENDERABLE_TYPE,       EGL_OPENVG_BIT,
                                       EGL_SURFACE_TYPE,          EGL_PIXMAP_BIT,
                                       EGL_NONE };
            
            EGLint config_size;
            ASSERT_EGL_TRUE(eglChooseConfig(iDisplay,KAttrib,&currentConfig,1,&config_size));
            ASSERT_TRUE(currentConfig!=0);
            
            // Create a pixmap surface from the native image
            EGLSurface surface = eglCreatePixmapSurface(iDisplay, currentConfig, &sgImage, NULL);
            EGLint eglError = eglGetError();
            if((surface == EGL_NO_SURFACE) || (eglError != EGL_SUCCESS))
                {
                INFO_PRINTF4(_L("***Fail to create Pixmap surface after %d attempts, error: %d, expected: %d"), index, eglError, EGL_BAD_ALLOC);
                TEST(eglError == EGL_BAD_ALLOC);
                break;
                }            
            } //for
        SendIndexToMainProcessL(index);
        }
    Rendezvous(aIdx);
    
    //create the queue to send/receive Process ID between processes
    RMsgQueue<TProcessId> messageQueueProcId;
    User::LeaveIfError(messageQueueProcId.Open(EProcSlotMsgQueueProcId, EOwnerProcess));
    CleanupClosePushL(messageQueueProcId);
    
    if(aIdx == 0)
        {
        // Sending Process ID to other process... so that the other process can kill it.
        TProcessId procId = RProcess().Id();
        messageQueueProcId.SendBlocking(procId);
        CleanupStack::PopAndDestroy(&messageQueueProcId);
        //go into indefinite loop which will be terminated by the second process
        for(;;) { }
        }
    else
        {
        TProcessId procId;
        messageQueueProcId.ReceiveBlocking(procId);
        CleanupStack::PopAndDestroy(&messageQueueProcId);

        RProcess process;
        TESTL(process.Open(procId) == KErrNone);
        process.Kill(KErrNone);
		process.Close();

        // small delay to ensure the kernel finishes the clean-up
        User::After(1*1000*1000); // 1 second
        }
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    }

/**
@SYMTestCaseID GRAPHICS-EGL-0441

@SYMTestPriority 1

@SYMPREQ 2637

@SYMTestCaseDesc
    OOM test – Free SgImages/Pixmap surfaces while the process which owns them exits gracefully

@SYMTestActions
Environmental settings:
•   Image Size: w50 h50
•   List of simulated load: 0%
•   List of pixel formats
ESgPixelFormatARGB_8888
•   Client process priorities - all the same
•   Client process random parameters:
-   None

The creation of RSgImages and launching of processes is along the lines of the method outlined in GRAPHICS-EGL-RSGIMAGE_LITE-0406

    From the main process:
    For i = 0 to N
        Spawn 1 client process.
        Signal client process to start by use of a semaphore
        Wait until client process exits
    If the test fails not due to the memory allocation record an error code to the log file then set a test result as a failure and skip further actions.
    End loop
    Exit

    From client process A:
    Get EGL display
    Initialize EGL
    Open RSgDriver
    Loop until exit condition met
    Start loop:
        Create SgImage
        Create Pixmap surface with underlying SgImage
        Exit condition – SgImage/Pixmap surface creation has failed.
    End loop:
    Log the last iteration number and exact operation which precedes a failure.
    CLose all allocated graphics resources (SgImages/Pixmap surfaces) 
    In the environment supporting NOK_resource_profiling2 extension retrieve for further analyzes the following GPU profiling data (if available):
        •   Total memory
        •   Used memory
        •   Process ID
        •   Process used private memory
        •   Process used shared memory
    Terminate EGL
    Close RSgDriver

@SYMTestExpectedResults
For each step from 0 to N in the main process, 
-   Image or surface allocation failure must happen at approximately the same iteration in process A. 
    MaxIterationNumber – MinIterationNumber < Threashold, 
    where Treashold will not exceeds 5 and exact value to be defined during implementation. 
-   GPU memory usage retrieved through NOK_resource_profiling2 extension, 
    if available, is consistent and doesn’t decrease over the time. 
*/
TVerdict CEglTest_OOM_ClosePixmapSurface::doTestStepL()
    {
    SetTestStepID(_L("GRAPHICS-EGL-0441"));
    SetTestStepName(KOOM_ClosePixmapSurface);
    INFO_PRINTF1(_L("CEglTest_OOM_ClosePixmapSurface::doTestStepL"));

#ifndef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    INFO_PRINTF1(_L("CEglTest_OOM_ClosePixmapSurface can only be run with SgImage-Lite"));
#else
    TBool ret = CheckForExtensionL(KEGL_RSgimage | KEGL_KHR_image_base | KVG_KHR_EGL_image | KEGL_KHR_image_pixmap);
    if(ret)
        {
        // if the test fails not due to the memory allocation, then skip further actions
        for(TInt index = 0; (index < iNumIterations) && (TestStepResult()== EPass); index++)
            {
            // launch 1 process
            Test_MultiProcessL(KEglTestStepDllName, 1, TestStepName());
            RetrieveExtensionDataL();
            } //for
        CheckDeviation();
        }
#endif //SYMBIAN_GRAPHICS_EGL_SGIMAGELITE

    RecordTestResultL();
    CloseTMSGraphicsStep();
    return TestStepResult();
    }

void CEglTest_OOM_ClosePixmapSurface::doProcessFunctionL(TInt aIdx)
    {
    INFO_PRINTF2(_L("CEglTest_OOM_ClosePixmapSurface::doProcessFunctionL, Process %d"),aIdx);
#ifdef SYMBIAN_GRAPHICS_EGL_SGIMAGELITE
    GetDisplayL();
    CreateEglSessionL(aIdx);
    iEglSess->InitializeL();    
    iEglSess->OpenSgDriverL();

    TInt index = 0;
    TSgImageInfo imageInfo;
    imageInfo.iUsage = ESgUsageBitOpenVgImage | ESgUsageBitOpenVgSurface;
    imageInfo.iPixelFormat = iPixelFormat;
    imageInfo.iSizeInPixels = iImageSize;

    for(;;++index)
        {
        RSgImage sgImage;
        TInt res = sgImage.Create(imageInfo, NULL);
        if(res != KErrNone || sgImage.IsNull())
            {
            INFO_PRINTF5(_L("***Fail to create RSgImage after %d attempts, error: %d, expected: %d or %d"), index, res, KErrNoMemory, KErrNoGraphicsMemory);
            TEST((res == KErrNoMemory) || (res == KErrNoGraphicsMemory));
            break;
            }
        iSgImages.AppendL(sgImage);
        
        EGLConfig currentConfig = 0;
        const EGLint KAttrib[] = { EGL_MATCH_NATIVE_PIXMAP,   (TInt)&sgImage,
                                   EGL_RENDERABLE_TYPE,       EGL_OPENVG_BIT,
                                   EGL_SURFACE_TYPE,          EGL_PIXMAP_BIT,
                                   EGL_NONE };
        
        EGLint config_size;
        ASSERT_EGL_TRUE(eglChooseConfig(iDisplay,KAttrib,&currentConfig,1,&config_size));
        ASSERT_TRUE(currentConfig!=0);
        
        // Create a pixmap surface from the native image
        EGLSurface surface = eglCreatePixmapSurface(iDisplay, currentConfig, &sgImage, NULL);
        EGLint eglError = eglGetError();
        if((surface == EGL_NO_SURFACE) || (eglError != EGL_SUCCESS))
            {
            INFO_PRINTF4(_L("***Fail to create Pixmap surface after %d attempts, error: %d, expected: %d "), index, eglError, EGL_BAD_ALLOC);
            TEST(eglError == EGL_BAD_ALLOC);
            break;
            }       
        iSurfaces.AppendL(surface);
        } //for
    SendIndexToMainProcessL(index);
    //now clean everything
    CleanGraphicsResources();
    iEglSess->CloseSgDriver();
    CleanAll();
#endif    
    }    

