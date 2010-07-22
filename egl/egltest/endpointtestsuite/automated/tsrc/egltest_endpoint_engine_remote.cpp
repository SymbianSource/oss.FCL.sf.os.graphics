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

#include <e32std.h>
#include "egltest_endpoint_engine.h"
#include "egltest_endpoint_images.h"
#include "egltest_surface.h"
#include "egltest_parameters.h"

CEgltest_Remote_Engine::CEgltest_Remote_Engine()
    : CRemoteTestStepBase(ETestUidEndpointEngine), iTestVerdict(ERtvPass), iLogging(EFalse), iSurface(0)
    {
    for (TInt i = 0; i < KMaxEndpoints; i++)
        {
        iEndpoints[i] = EGL_NO_ENDPOINT_NOK;
        iEglImage[i] = EGL_NO_IMAGE_KHR;
        iVgImage[i] = NULL;
        iRequestStatus[i] = KRequestPending;
        }
    }

CEgltest_Remote_Engine::~CEgltest_Remote_Engine()
    {
    // Note: This is run from a different thread an on a different heap than the one
    // used during the execution of DoRunRemoteTestCaseL(). So any allocation done
    // during DoRunRemoteTestCaseL must be de-allocated in DoEndRemoteTestStepL()
    }

TRemoteTestVerdict CEgltest_Remote_Engine::DoStartRemoteTestStepL(
        const TRemoteTestParams& /* aParams */)
    {
    iLogging = EFalse;
    return ERtvPass;
    }

TRemoteTestVerdict CEgltest_Remote_Engine::DoEndRemoteTestStepL(
        const TRemoteTestParams& /* aParams */)
    {
    delete iSurface;
    return ERtvPass;
    }


void CEgltest_Remote_Engine::CheckReturn(TInt aRetval,
        const TEngineTestCase& aEngineTestCase, TInt aFailValue,
        const TText* aFailSymbol, const TText* aFunction)
    {
    TBool isEqual = (aRetval == aFailValue);
    TBool expectFail = (aEngineTestCase.iFlags & EExpectFailureMask) != 0;

    EGLint err = eglGetError();
    if (err != aEngineTestCase.iErrorExpected)
        {
        REMOTE_ERR_PRINTF3(_L("testcase failed: expected %04x, got %04x"), aEngineTestCase.iErrorExpected, err);
        iTestVerdict = ERtvFail;
        }

    if (!isEqual && expectFail)
        {
        REMOTE_ERR_PRINTF5(
                _L("return value when failing from %s is not expected fail value %s (%d). Value returned is %d"),
                aFunction, aFailSymbol, aFailValue, aRetval);
        iTestVerdict = ERtvFail;
        }
    else if (isEqual && !expectFail)
        {
        REMOTE_ERR_PRINTF5(
                _L("return value when succeeding from %s is equal to expected fail value %s (%d). Value returned is %d"),
                aFunction, aFailSymbol, aFailValue, aRetval);
        iTestVerdict = ERtvFail;
        }
    if (isEqual != expectFail)
        {
        RDebug::Printf(
                "%s:%d: Called for %s, with expected fail %s, flags = %d",
                __FILE__, __LINE__, aFunction, aFailSymbol,
                aEngineTestCase.iFlags);
        iTestVerdict = ERtvFail;
        }
    // Now check
    if (expectFail && err == EGL_SUCCESS)
        {
        REMOTE_ERR_PRINTF2(_L("Got EGL_SUCCESS in error when calling %s, when we expected an error"),
                aFunction);
        iTestVerdict = ERtvFail;
        }
    // Didn't expect to fail, so we
    else if (!expectFail && err != EGL_SUCCESS)
        {
        REMOTE_ERR_PRINTF3(_L("Got an error (%x) on successful call to %s, when expecting EGL_SUCCESS"),
                err, aFunction);
        iTestVerdict = ERtvFail;
        }
    }

#define CHECK_RETURN(retval, failval, func) \
    CheckReturn((retval), si, (failval), _S(#failval), func)

#define CHECK_RETURN_CAST(retval, failval, func) \
    CheckReturn(reinterpret_cast<int>(retval), si, reinterpret_cast<int>(failval), _S(#failval), func)

#define CHECK_BOOL_RET(func, funcName) \
{  \
    EGLBoolean ret = EglEndpoint().func(dpy, endpoint);     \
    CheckReturn(ret, si, EGL_FALSE, _S("EGL_FALSE"), _S(funcName));  \
}

void CEgltest_Remote_Engine::LogDump(const TEngineTestCase& aCase)
    {
    const TText *caseName = EngineCaseName(aCase.iCase);

    Log(((TText8*)__FILE__), __LINE__, ESevrInfo,
                    _L("Performing subcase %d (%s), with flags=%d, err=%04x endpointidx=%d, image=%d, args=(%d, %d)"),
                    aCase.iCase,
                    caseName,
                    aCase.iFlags,
                    aCase.iErrorExpected,
                    aCase.iEndpointIndex,
                    aCase.iImageIndex,
                    aCase.iArg1, aCase.iArg2);
    }

TRemoteTestVerdict CEgltest_Remote_Engine::DoRunRemoteTestCaseL(
        TInt aTestCase, const TRemoteTestParams &aParams)
    {
    TRemoteTestArgs args;
    iTestVerdict = ERtvPass;
    const TEngineTestCase &si = aParams.iEndpointEngine.iEngineTestCase;

    CDisplayParams* displayParams = CDisplayParams::NewLC(!!(si.iFlags & EUseBadDisplay), eglGetDisplay(EGL_DEFAULT_DISPLAY));
    TInt dpyParamsCount = displayParams->Count();
    CEndpointParams* endpointParams = CEndpointParams::NewLC(!!(si.iFlags & EUseBadEndpoint), iEndpoints, KMaxEndpoints, si.iEndpointIndex);
    TInt endpointCount  = endpointParams->Count();
    CImageParams* imageParams = CImageParams::NewLC(!!(si.iFlags & EUseBadEglImage), iEglImage, KMaxEndpoints, si.iImageIndex);
    TInt imageCount = imageParams->Count();

    for(TInt dpyIter = 0; dpyIter < dpyParamsCount; dpyIter++)
        {
        args.iDisplay = (*displayParams)[dpyIter];
        for(TInt epIter = 0; epIter < endpointCount; epIter++)
            {
            args.iEndpoint = (*endpointParams)[epIter];

            for(TInt imageIter = 0; imageIter < imageCount; imageIter++)
                {
                args.iImage = (*imageParams)[imageIter];

                RunCaseL(aTestCase, aParams, args);
                if (iTestVerdict != ERtvPass || iLogging)
                    {
                    if (iTestVerdict != ERtvPass)
                        {
                        REMOTE_INFO_PRINTF1(_L("Test failed:"));
                        }
                    LogDump(si);
                    REMOTE_INFO_PRINTF4(_L("Using values: display: %d, endpoint: %d, image: %d"),
                            args.iDisplay, args.iEndpoint, args.iImage);
                    }
                }
            }
        }
    CleanupStack::PopAndDestroy(3);
    return iTestVerdict;
    }


void CEgltest_Remote_Engine::ActivateVgContextL()
    {
    if (!iSurface)
        {
        iSurface = CEglWindowSurface::NewL();
        RDebug::Printf("%s:%d: iSurface = %08x", __FILE__, __LINE__, iSurface);
        iSurface->CreateL(EStandardSurface);
        }
    iSurface->ActivateL();
    }


void CEgltest_Remote_Engine::RunCaseL(TInt aTestCase, const TRemoteTestParams &aParams, const TRemoteTestArgs& aArgs)
    {
    const TEngineTestCase &si = aParams.iEndpointEngine.iEngineTestCase;
    EGLDisplay dpy = aArgs.iDisplay;
    EGLImageKHR image = aArgs.iImage;
    EGLEndpointNOK endpoint = aArgs.iEndpoint;

    switch (aTestCase)
        {
        case EInitializeCase:
            REMOTE_INFO_PRINTF1(_L("calling EglStartL()"));
            EglStartL();
            break;

        case ECreateEndpointCase:
            CreateEndpointCaseL(aParams, aArgs);
            break;

        case EAcquireImageCase:
            image = EglEndpoint().AcquireImage(dpy, endpoint);
            CHECK_RETURN(image, EGL_NO_IMAGE_KHR, _S("eglAcquireImage"));
            if (image != EGL_NO_IMAGE_KHR)
                {
                iEglImage[si.iEndpointIndex] = image;
                }
            break;

        case ECompareImageCase:
            if (iEglImage[si.iEndpointIndex] == EGL_NO_IMAGE_KHR)
                {
                REMOTE_ERR_PRINTF2(_L("Image at index %d is not a valid eglImage"), si.iEndpointIndex);
                iTestVerdict = ERtvFail;
                }
            else
                {
                ActivateVgContextL();

                CTestCFbsImage *image = CTestCFbsImage::NewL(si.iImageIndex);
                CleanupStack::PushL(image);
                CTestVgEglImage *vgImage = CTestVgEglImage::NewL(iEglImage[si.iEndpointIndex]);
                CleanupStack::PushL(vgImage);
                TBool res = vgImage->CompareImageL(image, !!(si.iFlags & EExpectError));
                if (res != !(si.iFlags & EExpectError))
                    {
                    REMOTE_ERR_PRINTF1(_L("Pixel comparison failed...."));
                    iTestVerdict = ERtvFail;
                    }
                else if (!(si.iFlags & EExpectError))
                    {
                    TInt imageIndex2 = (si.iImageIndex + 1) % CTestImage::KImageCount;
                    CTestCFbsImage *image2 = CTestCFbsImage::NewL(imageIndex2);
                    CleanupStack::PushL(image2);
                    res = vgImage->CompareImageL(image2, !(si.iFlags & EExpectError));
                    if (res == !(si.iFlags & EExpectError))
                        {
                        REMOTE_ERR_PRINTF1(_L("Pixel comparison didn't fail - two images the same?...."));
                        iTestVerdict = ERtvFail;
                        }
                    CleanupStack::PopAndDestroy(image2);
                    }
                CleanupStack::PopAndDestroy(2, image);
                }
            break;

        case EReleaseImageCase:
            ReleaseImageCaseL(aParams, aArgs);
            break;

        case EBeginStreamingCase:
            CHECK_BOOL_RET(EndpointBeginStreaming, "eglEndpointBeginStreaming");
            break;

        case EEndStreamingCase:
            CHECK_BOOL_RET(EndpointEndStreaming, "eglEndpointEndStreaming");
            break;

        case EDestroyEndpointCase:
            CHECK_BOOL_RET(DestroyEndpoint, "eglDestroyEndpoint");
            break;

        case EGetAttribCase:
            {
            TInt value = EglEndpoint().GetEndpointAttrib(dpy, endpoint, si.iArg1);
            // We can't use the macro CHECK_RETURN_L here, as the return value for
            // "success" can be any integer value, including "EGL_FALSE". So we can
            // only check when we expect failure.
            if (si.iFlags & EExpectFailureMask)
                {
                CheckReturn(value, si, EGL_FALSE,_S("EGL_FALSE") ,_S("eglGetEndpointAttrib"));
                }
            else
                {
                EGLint err = eglGetError();
                if (err != EGL_SUCCESS)
                    {
                    REMOTE_ERR_PRINTF2(_L("Got an error (%x) on successful call to eglGetEndpointAttrib, when expecting EGL_SUCCESS in error"),
                            err);
                    iTestVerdict = ERtvFail;
                    }
                }
            if (value != si.iArg2)
                {
                REMOTE_ERR_PRINTF4(_L("GetEndpointAttrib(%04x), got %d, expected %d"), si.iArg1, value, si.iArg2);
                iTestVerdict = ERtvFail;
                }
            }
            break;

        case ESetAttribCase:
            {
            EGLBoolean ret = EglEndpoint().SetEndpointAttrib(dpy, endpoint, si.iArg1, si.iArg2);
            CHECK_RETURN(ret, EGL_FALSE, _S("eglSetEndpointAttrib"));
            }
            break;

        case EDestroyEglImageCase:
            {
            EGLBoolean ret = EglEndpoint().DestroyImage(dpy, image);
            CHECK_RETURN(ret, EGL_FALSE, _S("eglDestroyImageKHR"));
            }
			break;

        case ECreateVgImageCase:
            {
            // For a VgImage to be possible to create, we need to have a EglSurface.
            ActivateVgContextL();

            TRAPD(err, iVgImage[si.iImageIndex] = CTestVgEglImage::NewL(iEglImage[si.iEndpointIndex]));
            if ((si.iFlags & EExpectFailureMask) && err == KErrNone)
                {
                REMOTE_ERR_PRINTF1(_L("Successfully created VGImage when we expected an error"));
                iTestVerdict = ERtvFail;
                delete iVgImage[si.iImageIndex];
                iVgImage[si.iImageIndex] = NULL;
                }
            else if (!(si.iFlags & EExpectFailureMask) && err != KErrNone)
                {
                REMOTE_ERR_PRINTF1(_L("Failed to create VGImage when we expected to succeed"));
                iTestVerdict = ERtvFail;
                }
            }
            break;

        // Test that a vgImage can be used. We do NOT test the content for anything in particular, since
        // the current usage of this is to do checking on a vgImage after endpoint is destroyed, and the
        // specification is that the vgImage is undefined under this condition.
        case ETestVgImageValidCase:
            {
            ActivateVgContextL();
            CTestVgEglImage *vgImage = iVgImage[si.iImageIndex];
            if (!vgImage)
                {
                // Image not usable!
                REMOTE_ERR_PRINTF1(_L("VGImage is not present"));
                iTestVerdict = ERtvFail;
                }
            else
                {
                vgDrawImage(vgImage->VGImage());
                VGint err = vgGetError();
                if (err != VG_NO_ERROR)
                    {
                    iTestVerdict = ERtvFail;
                    }
#if 0
                TSize size = vgImage->Size();
                // Now read the pixels in four corners and the middle to check if the image is still "working".
                vgImage->Pixel(0, 0);
                vgImage->Pixel(size.iWidth-1, size.iHeight-1);
                vgImage->Pixel(0, size.iHeight-1);
                vgImage->Pixel(size.iWidth-1, 0);
                vgImage->Pixel(size.iWidth >> 1, size.iHeight >> 1);
#endif
                }
            // If we get here, the image is "working" - we expect to panic or crash if it's not...
            }
            break;

        case EDestroyVgImageCase:
            delete iVgImage[si.iImageIndex];
            iVgImage[si.iImageIndex] = NULL;
            break;

        case ERequestNotificationCase:
            RequestNotificationL(aParams, aArgs);
            break;

        case ECancelNotificationCase:
            CHECK_BOOL_RET(EndpointCancelNotification, "eglEndpointCancelNotification");
            break;

        case EWaitForNotificationCase:
            {
            RTimer timer;
            TInt err = timer.CreateLocal();
            if (err != KErrNone)
                {
                REMOTE_INFO_PRINTF2(_L("Could not create timer. Error=%d"), err);
                iTestVerdict = ERtvFail;
                }
            else
                {
                TRequestStatus timerStatus = KRequestPending;
                timer.HighRes(timerStatus, si.iArg1);
                // Note that the requeststatus is set to KRequestPending by
                // eglEndpointRequestNotificationNOK(), so we don't do that
                // before waiting. See below for more comments.
                TRequestStatus *requestStatus = &iRequestStatus[si.iEndpointIndex];
                User::WaitForRequest(timerStatus, *requestStatus);
                TInt result = KErrNotReady;  // Give it some ERROR value that is unlikely to happen later.
                timer.Cancel();
                timer.Close();
                if (timerStatus == KErrNone && *requestStatus == KRequestPending)
                    {
                    result = KErrTimedOut;
                    }
                else
                    {
                    result = requestStatus->Int();
                    }
                // Reset the request - this allows us to (ab-)use this request to
                // wait for things that aren't completing, etc.
                *requestStatus = KRequestPending;
                if ((si.iFlags & EExpectError) && result >= KErrNone)
                    {
                    iTestVerdict = ERtvFail;
                    REMOTE_INFO_PRINTF1(_L("Expected failure, but result was a success"));
                    }
                else if (!(si.iFlags & EExpectError) && result < KErrNone)
                    {
                    iTestVerdict = ERtvFail;
                    REMOTE_INFO_PRINTF1(_L("Expected success, but result was a failure"));
                    }
                if (result != si.iErrorExpected)
                    {
                    iTestVerdict = ERtvFail;
                    REMOTE_INFO_PRINTF3(_L("EWaitForNotificationCase: Expected error %d, got %d"), si.iErrorExpected, result);
                    }
                }
            }
            break;

        case EGetEndpointDirtyAreaCase:
            GetEndpointDirtyAreaL(aParams, aArgs);
            break;

        case ETerminateCase:
            REMOTE_INFO_PRINTF1(_L("calling EglEndL()"));
            EglEndL();
            break;

        /*
         * Debug cases
         */

        case EBreakPointCase:
            if (si.iFlags & EDebugRemote)
                {
                __BREAKPOINT();
                }
            break;

        case ELogEnableCase:
            if (si.iFlags & EDebugRemote)
                {
                iLogging = ETrue;
                }
            break;
            
        case EPanicCase:
            if (si.iFlags & EDebugRemote)
                {
                User::Panic(_L("EPanicCase"), -1);
                }
            break;

        default:
            REMOTE_ERR_PRINTF2(_L("Invalid testcase %d"), aTestCase);
            User::Invariant();
            break;
        }
    }


void CEgltest_Remote_Engine::CreateEndpointCaseL(const TRemoteTestParams &aParams, const TRemoteTestArgs& aArgs)
    {
    const TEngineTestCase &si = aParams.iEndpointEngine.iEngineTestCase;
    EGLDisplay dpy = aArgs.iDisplay;
    EGLEndpointNOK endpoint = aArgs.iEndpoint;
    const TSurfaceParamsRemote& cp = aParams.iEndpointEngine.iSurfaceParams;

    CEnumParams* endpointTypeParams = CEnumParams::NewLC(!!(si.iFlags & EUseBadEndpointType),
                                                            EGL_ENDPOINT_TYPE_CONSUMER_NOK);
    CEnumParams* sourceParams = CEnumParams::NewLC(!!(si.iFlags & EUseBadSourceType),
                                                   EGL_TSURFACEID_NOK);
    CSurfaceIdParams *surfParams = CSurfaceIdParams::NewLC(!!(si.iFlags & EUseBadSurfaceId),
                                                           cp.iSurfaceId);

    TInt endpointTypeCount = endpointTypeParams->Count();
    TInt sourceCount = sourceParams->Count();
    TInt surfCount = surfParams->Count();

    for(TInt typeIter = 0; typeIter < endpointTypeCount; typeIter++)
        {
        EGLenum type = (*endpointTypeParams)[typeIter];
        for(TInt sourceIter = 0; sourceIter < sourceCount; sourceIter++)
            {
            EGLenum source_type = (*sourceParams)[sourceIter];

            for(TInt surfIter = 0; surfIter < surfCount; surfIter++)
                {
                EGLEndpointSourceNOK source = (EGLEndpointSourceNOK)(&(*surfParams)[surfIter]);
                EGLint *attrib_list = cp.iCommonParams.iUseAttribList?
                    const_cast<EGLint *>(cp.iCommonParams.iAttribs):NULL;

                endpoint = EglEndpoint().CreateEndpoint(dpy, type, source_type, source, attrib_list);
                CHECK_RETURN_CAST(endpoint, EGL_NO_ENDPOINT_NOK, _S("eglCreateEndpoint"));
                if (endpoint != EGL_NO_ENDPOINT_NOK)
                    {
                    iEndpoints[si.iEndpointIndex] = endpoint;
                    }
                }
            }
        }
    CleanupStack::PopAndDestroy(3);
    }


void CEgltest_Remote_Engine::ReleaseImageCaseL(const TRemoteTestParams& aParams, const TRemoteTestArgs& aArgs)
    {
    const TEngineTestCase &si = aParams.iEndpointEngine.iEngineTestCase;
    EGLDisplay dpy = aArgs.iDisplay;
    EGLImageKHR image = aArgs.iImage;
    EGLEndpointNOK endpoint = aArgs.iEndpoint;

    static const EGLenum validAPIs[] = { EGL_OPENVG_API, EGL_OPENGL_API, EGL_OPENGL_ES_API };
    const TInt validAPIcount = sizeof(validAPIs) / sizeof(validAPIs[0]);

    CEnumParams* enumParams = CEnumParams::NewLC(!!(si.iFlags & EUseBadApi),
            validAPIs, validAPIcount, 0);
    for(TInt enumIter = 0; enumIter < enumParams->Count(); enumIter++)
        {
        EGLenum api = (*enumParams)[enumIter];
        EGLBoolean ret = EglEndpoint().ReleaseImage(dpy, endpoint, image, api);
        CHECK_RETURN(ret, EGL_FALSE, _S("eglReleaseImage"));
        }
    CleanupStack::PopAndDestroy(enumParams);
    }

void CEgltest_Remote_Engine::RequestNotificationL(const TRemoteTestParams& aParams, const TRemoteTestArgs &aArgs)
    {
    const TEngineTestCase &si = aParams.iEndpointEngine.iEngineTestCase;
    EGLDisplay dpy = aArgs.iDisplay;
    EGLEndpointNOK endpoint = aArgs.iEndpoint;

    CSyncParams* enumParams = CSyncParams::NewLC(!!(si.iFlags & EUseBadSync), &iRequestStatus[si.iEndpointIndex]);
    for(TInt enumIter = 0; enumIter < enumParams->Count(); enumIter++)
        {
        TRequestStatus* sync = (*enumParams)[enumIter];
        EGLBoolean ret = EglEndpoint().EndpointRequestNotification(dpy, endpoint, sync);
        CHECK_RETURN(ret, EGL_FALSE, _S("eglEndpointRequestNotification"));
        }
    CleanupStack::PopAndDestroy(enumParams);
    }

// Mark either side of "rects" with something that we can detect.
// Must not be a valid rect coordinate - which is unlikely for this
// number (regardless of endianness), since it's roughly 0x40000000.
static const EGLint KMarker = 'NTCH';
// Allow space for this number of rectangles either side of the actual buffer.
static const TInt KBufferArea = 2;

void CEgltest_Remote_Engine::DoCheckRectsL(EGLint *aRectsBuffer, EGLint aRectCount, EGLint aMaxRects,
                                           TInt aRectsIndex, const TRect aSurfaceRect)
    {
    // Right now, this testing only supports "full surface" rectangles.
    ASSERT(aRectsIndex == 0);
    EGLint *rects = aRectsBuffer+KBufferArea * 4;

    // First, check the rects returned by the call. Should not be equal to KMarker.
    // For example, if we ask for 4 rects, and only two rects are filled in, index
    // 0 and 1 are checked that they are properly filled in.
    for (TInt i = 0; i < aRectCount * 4; i++)
        {
        if (rects[i] == KMarker)
            {
            iTestVerdict = ERtvFail;
            REMOTE_INFO_PRINTF3(_L("Seems the dirty area wasn't filled in properly! Got 0x%08x at %d"), rects[i], i);
            }
        }
    // Check the area not supposed to be filled in! All this should contain KMArker!
    // Check that the dirty area get call didn't fill in any memory
    // beyond the rectangles returned. Say we asked for 4 rectangles,
    // and two were returned, this will check that index 2 & 3 were
    // not modified. If we ask for 4 rects and get 4 rects back, nothing
    // is done here.
    for(TInt i = aRectCount * 4; i < aMaxRects * 4; i++)
        {
        if (rects[i] != KMarker)
            {
            iTestVerdict = ERtvFail;
            REMOTE_INFO_PRINTF3(_L("Seems the dirty area filled beyond the number of rects that it returned! Got 0x%08x at %d"), rects[i], i);
            }
        }
    // Check the "bufferaea" before the actual rects - MUST not be touched.
    for(TInt i = 0; i < KBufferArea * 4; i++)
        {
        if (aRectsBuffer[i] != KMarker)
            {
            iTestVerdict = ERtvFail;
            REMOTE_INFO_PRINTF3(_L("Seems the dirty area walked outside it's allowed memory! Got 0x%08x at %d"), rects[i], i);
            }
        }
    // Check the buffer area AFTER the buffer we gave - again, the
    // production code should ABSOLUTELY not write here.
    for(TInt i = (aMaxRects + KBufferArea) * 4; i < (aMaxRects + KBufferArea * 2) * 4; i++)
        {
        if (aRectsBuffer[i] != KMarker)
            {
            iTestVerdict = ERtvFail;
            REMOTE_INFO_PRINTF3(_L("Seems the dirty area walked outside it's allowed memory! Got 0x%08x at %d"), rects[i], i);
            }
        }
    if (aRectsIndex == 0)
        {
        // Check that rectangle matches the full surface extent.
        // We should only have ONE rectangle in this case!
        if (aRectCount != 1)
            {
            REMOTE_INFO_PRINTF2(_L("Expected 1 rectangle returned - got %d"), aRectCount);
            iTestVerdict = ERtvFail;
            }
        else
            {
            TRect returnedRect = TRect(rects[0], rects[1], rects[2], rects[3]);
            if (returnedRect != aSurfaceRect)
                {
                REMOTE_INFO_PRINTF1(_L("rectangles do not match!"));
                }
            }
        }
    // TODO: To support flexible sets of dirty area we need an else on the
    // above if-statement. However, with the current reference and the planned
    // third party known at this point, only "full surface" will ever be
    // returned.
    }

void CEgltest_Remote_Engine::GetEndpointDirtyAreaL(const TRemoteTestParams& aParams, const TRemoteTestArgs& aArgs)
    {
    const TEngineTestCase &si = aParams.iEndpointEngine.iEngineTestCase;
    EGLDisplay dpy = aArgs.iDisplay;
    EGLEndpointNOK endpoint = aArgs.iEndpoint;

    EGLint *rects;
    EGLint *rectsBuffer = NULL;
    const TInt actualRectsSize = (si.iArg2 + KBufferArea * 2) * 4;
    // We don't use the "parameter expansion" for bad rects value.
    // This is because it's so easy to just add it here, and there is only one bad
    // value that is recognisable.
    if (si.iFlags & (EUseNullRects | EUseBadRects))
        {
        rects = NULL;
        }
    else
        {
        rectsBuffer = new EGLint[actualRectsSize];
        CleanupStack::PushL(rectsBuffer);
        rects = rectsBuffer + (KBufferArea * 4);
        for(TInt i = 0; i < actualRectsSize; i++)
            {
            rectsBuffer[i] = KMarker;
            }
        }
    EGLBoolean collapse = (si.iFlags & EUseCollapseArea)?EGL_TRUE:EGL_FALSE;
    EGLint ret = EglEndpoint().GetEndpointDirtyArea(dpy, endpoint, rects, si.iArg1, si.iArg2, collapse);
    if (!(si.iFlags & EExpectFailureMask))
        {
        if (rectsBuffer)
            {
            TInt rectsIndex = si.iImageIndex;  // ImageIndex is used for rects!
            const TSurfaceParamsRemote &surfParams = aParams.iEndpointEngine.iSurfaceParams;
            // TODO: If the surface has been downscaled, we need to modify this rectangle.
            // We can only know if it's downsampled by getting the image, converting to a VGImage,
            // and getting the size of the VGImage. It can be done, but we would need to make
            // sure the imageindex matches the endpointindex, as imageindex is used by the
            // rectsindex (above).
            TRect surfaceRect = TRect(0, 0, surfParams.iCommonParams.iXSize-1, surfParams.iCommonParams.iYSize-1);
            DoCheckRectsL(rectsBuffer, ret, si.iArg2, rectsIndex, surfaceRect);
            }
        }
    if (rectsBuffer)
        {
        CleanupStack::PopAndDestroy(rectsBuffer);
        }
    if (ret != 0 || (si.iFlags & EExpectFailureMask))
        {
        CHECK_RETURN(ret, EGL_FALSE, _S("eglGetEndpointDirtyArea"));
        }
    }

