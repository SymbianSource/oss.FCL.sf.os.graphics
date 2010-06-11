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
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code
*/


#include "egltest_surface.h"
#include "egltest_endpoint_images.h"
#include <graphics/surfaceconfiguration.h>

#define SURF_ASSERT(x) { if (!(x)) { RDebug::Printf("ASSERT(%s) failed at %s:%d", #x, __FILE__, __LINE__); User::Panic(_L("ASSERT SURF"), __LINE__); }}

// Macros for indicating what is what.
#define SIZE(x, y) x, y
#define Buffers(x) x
#define DefaultAlignment 32  // Pick some value that is valid.
#define Alignment(x) x
#define Stride(x) x
#define DefaultStride 0
// Note: Offset to first buffer.
#define Offset(x) x
#define WindowPos(x, y)  x, y
#define WindowMode(m) m

static const TSurfaceParamsCommon KSurfaceParams[] =
{
    {
        EStandardSurface,
        SIZE(100, 100),
        Buffers(2),
        DefaultAlignment,
        DefaultStride,
        Offset(0),
        EUidPixelFormatARGB_8888_PRE,
        EFalse,
        { 0 },
        WindowPos(0, 0),
        WindowMode(EColor16MAP)
    },
    {
        EBadAttribSurface,
        SIZE(100, 100),
        Buffers(2),
        DefaultAlignment,
        DefaultStride,
        Offset(0),
        EUidPixelFormatARGB_8888_PRE,
        ETrue,
        { 1, 1, EGL_NONE },
        WindowPos(0, 0),
        WindowMode(EColor16MAP)
    },
    {
        EEmptyAttribSurface,
        SIZE(100, 100),
        Buffers(2),
        DefaultAlignment,
        DefaultStride,
        Offset(0),
        EUidPixelFormatARGB_8888_PRE,
        ETrue,
        { EGL_NONE },
        WindowPos(0, 0),
        WindowMode(EColor16MAP)
    },
    {
        EStandard128sqSurface,
        SIZE(128, 128),
        Buffers(3),
        DefaultAlignment,
        DefaultStride,
        Offset(0),
        EUidPixelFormatARGB_8888_PRE,
        EFalse,
        { 0 },
        WindowPos(20, 20),
        WindowMode(EColor16MAP)
    },
    {
        EUnusualStrideSurface,
        SIZE(167,263),
        Buffers(2),
        Alignment(8),
        Stride(167*4+64),
        Offset(200),
        EUidPixelFormatARGB_8888_PRE,
        EFalse,
        { 0 },
        WindowPos(0, 0),
        WindowMode(EColor16MAP)
    },
    {
        EUnalignedPixelSizeSurface,
        SIZE(103, 107),
        Buffers(2),
        Alignment(8),
        Stride(103*4),
        Offset(0),
        EUidPixelFormatARGB_8888_PRE,
        EFalse,
        { 0 },
        WindowPos(0, 0),
        WindowMode(EColor16MAP)
    },
    {
        ELargeSurface,
        SIZE(800, 600),
        Buffers(2),
        DefaultAlignment,
        DefaultStride,
        Offset(0),
        EUidPixelFormatARGB_8888_PRE,
        EFalse,
        { 0 },
        WindowPos(0, 0),
        WindowMode(EColor16MAP)
    },
};


TInt KSurfaceMaxIndex = sizeof(KSurfaceParams) / sizeof(KSurfaceParams[0]);

CRawSurface* CRawSurface::NewL()
    {
    CRawSurface* obj = new (ELeave) CRawSurface();
    CleanupStack::PushL(obj);
    obj->ConstructL();
    CleanupStack::Pop(obj);
    return obj;
    }


CRawSurface::CRawSurface() : iDrawBuffer(0), iBuffers(0)
    {
    }


void CRawSurface::ConstructL()
    {
    iSurfaceId = TSurfaceId::CreateNullId();
    User::LeaveIfError(iSurfaceManager.Open());
    User::LeaveIfError(iSurfaceUpdate.Connect(5));
    }


CRawSurface::~CRawSurface()
    {
    iSurfaceUpdate.Close();
    if(!iSurfaceId.IsNull())
        {
        iSurfaceManager.CloseSurface(iSurfaceId);
        }
    iSurfaceManager.Close();
    }


TInt CRawSurface::PixelSize(TUidPixelFormat aPixelFormat)
    {
    switch(aPixelFormat)
        {
        case EUidPixelFormatARGB_8888_PRE:
        case EUidPixelFormatARGB_8888:
        case EUidPixelFormatABGR_8888:
        case EUidPixelFormatABGR_8888_PRE:
            return 4;

        case EUidPixelFormatARGB_4444:
        case EUidPixelFormatRGB_565:
            return 2;

        default:
            SURF_ASSERT(0);
            break;
        }
    return 0; // Make sure no compiler moans about "not all paths return a value".
    }


void CRawSurface::GetSurfAttribs(RSurfaceManager::TSurfaceCreationAttributesBuf &aSurfaceAttribs, TInt aIndex)
    {
    SURF_ASSERT(aIndex < KSurfaceMaxIndex);
    SURF_ASSERT(aIndex == KSurfaceParams[aIndex].iIndex);
    iParamIndex = aIndex;
    aSurfaceAttribs().iSize = TSize(KSurfaceParams[aIndex].iXSize, KSurfaceParams[aIndex].iYSize);
    iBuffers = KSurfaceParams[aIndex].iBuffers;
    aSurfaceAttribs().iBuffers = iBuffers;
    aSurfaceAttribs().iPixelFormat = KSurfaceParams[aIndex].iPixelFormat;
    TInt stride = KSurfaceParams[aIndex].iStrideInBytes;
    if (stride == 0)
        {
        stride = KSurfaceParams[aIndex].iXSize * PixelSize(KSurfaceParams[aIndex].iPixelFormat);
        }
    aSurfaceAttribs().iStride = stride;
    aSurfaceAttribs().iOffsetToFirstBuffer = KSurfaceParams[aIndex].iOffsetToFirstBuffer;
    aSurfaceAttribs().iAlignment = KSurfaceParams[aIndex].iAlignment;
    aSurfaceAttribs().iContiguous = EFalse;
    aSurfaceAttribs().iCacheAttrib = RSurfaceManager::ECached;
    aSurfaceAttribs().iOffsetBetweenBuffers = 0;
    aSurfaceAttribs().iSurfaceHints = NULL;
    aSurfaceAttribs().iHintCount = 0;
    aSurfaceAttribs().iMappable = ETrue;
    }


//From CSurface.
void CRawSurface::CreateL(TInt aIndex)
    {
    RSurfaceManager::TSurfaceCreationAttributesBuf surfaceAttribs;

    GetSurfAttribs(surfaceAttribs, aIndex);
    TInt err = iSurfaceManager.CreateSurface(surfaceAttribs, iSurfaceId);
    User::LeaveIfError(err);
    }


TUint8* CRawSurface::MapSurfaceAndGetInfoLC(RSurfaceManager::TSurfaceInfoV01& aInfo)
    {
    SURF_ASSERT(!iSurfaceId.IsNull());
    User::LeaveIfError(iSurfaceManager.MapSurface(iSurfaceId, iSurfaceChunk));
    CleanupClosePushL(iSurfaceChunk);
    RSurfaceManager::TInfoBuf infoBuf;
    User::LeaveIfError(iSurfaceManager.SurfaceInfo(iSurfaceId, infoBuf));
    aInfo = infoBuf();
    TInt offset = -1000;  // So we hopefully detect when it goes horribly wrong.
    User::LeaveIfError(iSurfaceManager.GetBufferOffset(iSurfaceId, iDrawBuffer, offset));
    return iSurfaceChunk.Base() + offset;
    }


void CRawSurface::DrawContentL(TInt aImageIndex)
    {
    CTestCFbsImage *image = CTestCFbsImage::NewL(aImageIndex);
    CleanupStack::PushL(image);

    RSurfaceManager::TSurfaceInfoV01 info;
    TUint8 *dataAddress = MapSurfaceAndGetInfoLC(info);
    TInt stride = info.iStride;

    CFbsBitmap *bitmap = image->Bitmap();
    TDisplayMode displaymode = bitmap->DisplayMode();
    TInt pixelStride = stride / CFbsBitmap::ScanLineLength(1, displaymode);
    for(TInt y = 0; y < image->Size().iHeight; y++)
        {
        TPtr8 buf(dataAddress + y * stride, stride);

        // TODO: We need to check that the bitsperpixel matches between the surface and bitmap.
        bitmap->GetScanLine(buf, TPoint(0, y), pixelStride, displaymode);
        }

    CleanupStack::PopAndDestroy(2, image);
    }


void CRawSurface::DrawContentL(const TRgb& aColour)
    {
    //Map the surface and get its info.
    RSurfaceManager::TSurfaceInfoV01 surfaceInfo;
    TUint32* buffer = (TUint32*)MapSurfaceAndGetInfoLC(surfaceInfo);

    //Currently this function only supports drawing into ARGB_8888_PRE surfaces.
    //This is because the only test that uses this function uses this type of surface.
    //If this functionallity needs expanding, you must correctly convert the TRgb colour
    //and pack it into the surface buffer correctly.
    SURF_ASSERT(surfaceInfo.iPixelFormat == EUidPixelFormatARGB_8888_PRE);

    TUint32 fillColour = aColour._Color16MAP();

    //Loop over each pixel in the surface and colour it.
    //This is deliberately slow since it is only used for the tearing test
    //and we want to spend most of our time drawing so that the chances of the other thread
    //picking up a buffer in the middle of drawing is increased.
    for(TInt y=0; y < surfaceInfo.iSize.iHeight; ++y)
        {
        for(TInt x=0; x < surfaceInfo.iSize.iWidth; ++x)
            {
            buffer[x] = fillColour;
            }
        buffer += surfaceInfo.iStride >> 2;
        }

    CleanupStack::PopAndDestroy();
    }


void CRawSurface::SubmitContentL(TBool aShouldWaitForDisplay, TInt /* aRectsIndex */)
    {
    TRequestStatus displayNotify = KRequestPending;
    TTimeStamp timeStamp;

    if(aShouldWaitForDisplay)
        {
        Notify(ENotifyWhenDisplayed, displayNotify, 0);
        }

    TInt err = iSurfaceUpdate.SubmitUpdate(KAllScreens, iSurfaceId, iDrawBuffer, NULL);
    User::LeaveIfError(err);
    iDrawBuffer = (iDrawBuffer + 1) % iBuffers;

    if(aShouldWaitForDisplay)
        {
        TUint32 dummy;
        TInt err = WaitFor(ENotifyWhenDisplayed, displayNotify, 100 * 1000, dummy);
        if (err != KErrNone && err != KErrNotVisible && err != KErrOverflow)
            {
            RDebug::Printf("%s:%d: NotifyWhenDisplayed gave unexpected error %d", __FILE__, __LINE__, err);
            User::Leave(err);
            }
        }
    }


TSurfaceId CRawSurface::SurfaceId() const
    {
    return iSurfaceId;
    }


void CRawSurface::GetSurfaceParamsL(TSurfaceParamsRemote &aParams)
    {
    aParams.iCommonParams = KSurfaceParams[iParamIndex];
    aParams.iCommonParams.iBuffers = iBuffers; // May have been changed if it's a single buffered surface...
    aParams.iSurfaceId = SurfaceId();
    }

const TText *CRawSurface::GetSurfaceTypeStr() const
    {
    return _S("CRawSurface");
    }

TInt CRawSurface::Notify(TNotification aWhen, TRequestStatus& aStatus, TUint32 aXTimes)
    {
    aStatus = KRequestPending;
    switch(aWhen)
        {
        case ENotifyWhenAvailable:
            iSurfaceUpdate.NotifyWhenAvailable(aStatus);
            break;
        case ENotifyWhenDisplayed:
            iSurfaceUpdate.NotifyWhenDisplayed(aStatus, iTimeStamp);
            break;
        case ENotifyWhenDispXTimes:
            iSurfaceUpdate.NotifyWhenDisplayedXTimes(aXTimes, aStatus);
            break;
        default:
            RDebug::Printf("%s:%d: Invalid notification: %d. Panicking...", __FILE__, __LINE__, aWhen);
            User::Panic(_L("CRawSurface::Notify()"), __LINE__);
            break;
        }
    return KErrNone;
    }


TInt CRawSurface::WaitFor(TNotification aWhen, TRequestStatus& aStatus, TInt aTimeoutInMicroSeconds, TUint32& aTimeStamp)
    {
    RTimer timer;
    TInt err = timer.CreateLocal();
    if (err != KErrNone)
        {
        return err;
        }
    TRequestStatus timerStatus = KRequestPending;
#if __WINS__
    // Windows timer isn't very precise - add some "fuzz" to the timeout to ensure we do not wait "too little".
    const TInt KTimeOutExtra = 20000;
#else
    // On hardware, we should be able to run with less "fuzz".
    const TInt KTimeOutExtra = 10000;
#endif
    timer.HighRes(timerStatus, aTimeoutInMicroSeconds + KTimeOutExtra);
    User::WaitForRequest(timerStatus, aStatus);
    if (aStatus == KRequestPending)
        {
        if (aWhen == ENotifyWhenDisplayed)
            {
            aTimeStamp = User::FastCounter();
            }
        return KErrTimedOut;
        }
    if (aWhen == ENotifyWhenDisplayed)
        {
        aTimeStamp = iTimeStamp();
        }
    timer.Cancel();
    timer.Close();
    TInt result = aStatus.Int();
    aStatus = KRequestPending;
    return result;
    }



const TText *CRawSingleBufferSurface::GetSurfaceTypeStr() const
    {
    return _S("CRawSingleBufferedSurface");
    }

CRawSingleBufferSurface *CRawSingleBufferSurface::NewL()
    {
    CRawSingleBufferSurface* obj = new (ELeave) CRawSingleBufferSurface();
    CleanupStack::PushL(obj);
    obj->ConstructL();
    CleanupStack::Pop(obj);
    return obj;
    }


void CRawSingleBufferSurface::CreateL(TInt aIndex)
    {
    RSurfaceManager::TSurfaceCreationAttributesBuf surfaceAttribs;

    GetSurfAttribs(surfaceAttribs, aIndex);

    iBuffers = 1;
    surfaceAttribs().iBuffers = 1;

    TInt err = iSurfaceManager.CreateSurface(surfaceAttribs, iSurfaceId);
    User::LeaveIfError(err);
    }

CRawSingleBufferSurface::~CRawSingleBufferSurface()
    {
    }

class CWindow: public CBase
    {
public:
    static CWindow *NewL(TInt aIndex);
    RWindow& Window();
    ~CWindow();
private:
    void ConstructL(TInt aIndex);
    CWindow();

private:
    RWindow iWindow;
    RWindowGroup iWindowGroup;
    RWsSession iWsSession;
    };


CWindow* CWindow::NewL(TInt aIndex)
    {
    CWindow *self = new (ELeave) CWindow;
    CleanupStack::PushL(self);
    self->ConstructL(aIndex);
    CleanupStack::Pop(self);
    return self;
    }


void CWindow::ConstructL(TInt aIndex)
    {
    RFbsSession::Connect();
    if (aIndex >= KSurfaceMaxIndex)
        {
        User::Leave(KErrOverflow);
        }
    User::LeaveIfError(iWsSession.Connect());
    iWindowGroup = RWindowGroup(iWsSession);
    User::LeaveIfError(iWindowGroup.Construct((TUint32)this));
    iWindow = RWindow(iWsSession);
    User::LeaveIfError(iWindow.Construct(iWindowGroup, -1U));
    const TSurfaceParamsCommon& winAttrib = KSurfaceParams[aIndex];
    iWindow.SetExtent(TPoint(winAttrib.iXPos, winAttrib.iYPos), TSize(winAttrib.iXSize, winAttrib.iYSize));
    iWindow.SetRequiredDisplayMode(winAttrib.iDisplayMode);
    iWindow.Activate();
    }


CWindow::~CWindow()
    {
    iWindow.Close();
    iWindowGroup.Close();
    iWsSession.Close();
    RFbsSession::Disconnect();
    }


CWindow::CWindow()
    {
    }


RWindow& CWindow::Window()
    {
    return iWindow;
    }


CEglWindowSurface* CEglWindowSurface::NewL()
    {
    CEglWindowSurface* self = new (ELeave) CEglWindowSurface;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


void CEglWindowSurface::ConstructL()
    {
    }


CEglWindowSurface::CEglWindowSurface()
    {
    }


void CEglWindowSurface::CreateL(TInt aIndex)
    {
    SURF_ASSERT(aIndex < KSurfaceMaxIndex);
    SURF_ASSERT(aIndex == KSurfaceParams[aIndex].iIndex);
    iParamIndex = aIndex;
    iWindow = CWindow::NewL(aIndex);
    iDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint err;
    if (iDisplay == EGL_NO_DISPLAY)
        {
        err = eglGetError();
        RDebug::Printf("%s:%d: err = %d", __FILE__, __LINE__, err);
        User::Leave(KErrNotSupported);
        }

    EGLConfig config;
    EGLint nConfigs = 0;

    // TODO: Need to use differnet config attribs based on aIndex.
    static const EGLint KConfigAttribs[] =
    {
        EGL_BUFFER_SIZE,    32,
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,EGL_OPENVG_BIT,
        EGL_NONE
    };

    // Need some way to configure the attribs ...
    eglChooseConfig(iDisplay, KConfigAttribs, &config, 1, &nConfigs);
    if (!nConfigs)
        {
        err = eglGetError();
        RDebug::Printf("%s:%d: err = %d", __FILE__, __LINE__, err);
        User::Leave(KErrNotSupported);
        }

    if (!eglBindAPI(EGL_OPENVG_API))
        {
        err = eglGetError();
        RDebug::Printf("%s:%d: err = %d", __FILE__, __LINE__, err);
        User::Leave(KErrNotSupported);
        }
    iContext = eglCreateContext(iDisplay, config, 0, NULL);
    if (iContext == EGL_NO_CONTEXT)
        {
        err = eglGetError();
        RDebug::Printf("%s:%d: err = %d", __FILE__, __LINE__, err);
        User::Leave(KErrNotSupported);
        }

    iSurface = eglCreateWindowSurface(iDisplay, config, &iWindow->Window(), NULL);
    if (iSurface == EGL_NO_SURFACE)
        {
        err = eglGetError();
        RDebug::Printf("%s:%d: err = %d", __FILE__, __LINE__, err);
        User::Leave(KErrNotSupported);
        }
    }

void CEglWindowSurface::SubmitContentL(TBool aShouldWaitForDisplay, TInt /* aRectsIndex */)
    {
    ActivateL();
    if (!eglSwapBuffers(iDisplay, iSurface))
        {
        User::Leave(KErrBadHandle);
        }
    if (aShouldWaitForDisplay)
        {
        // We are cheating: We just wait for a bit to ensure that the swapbuffer is actually finished.
        // There is no way to determine how long this takes, so we just grab a number that should be
        // large enough...
        User::After(100 * 1000);  // Wait 100ms.
        }
    }

void CEglWindowSurface::ActivateL()
    {
    if (!eglMakeCurrent(iDisplay, iSurface, iSurface, iContext))
        {
        User::Leave(KErrBadHandle);
        }
    }

CEglWindowSurface::~CEglWindowSurface()
    {
    eglMakeCurrent(iDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (iSurface != EGL_NO_SURFACE)
        {
        eglDestroySurface(iDisplay, iSurface);
        iSurface = EGL_NO_SURFACE;
        }

    if (iDisplay != EGL_NO_DISPLAY)
        {
        eglDestroyContext(iDisplay, iContext);
        }

    delete iWindow;
    }

TSurfaceId CEglWindowSurface::SurfaceId() const
    {
    // Default constructor for id sets it to a NULL-value, so if no window is created, we get
    // a defined surface id value that is invalid.
    TSurfaceId id;
    if (iWindow)
        {
        TSurfaceConfiguration surfConfig;
        iWindow->Window().GetBackgroundSurface(surfConfig);
        surfConfig.GetSurfaceId(id);
        }
    return id;
    }


void CEglWindowSurface::DrawContentL(const TRgb& aColour)
    {
    ActivateL();

    TSize size;
    eglQuerySurface(iDisplay, iSurface, EGL_WIDTH, &size.iWidth);
    eglQuerySurface(iDisplay, iSurface, EGL_HEIGHT, &size.iHeight);

    VGfloat fillColour[4];
    fillColour[0] = (VGfloat)aColour.Red() / 255.0f;
    fillColour[1] = (VGfloat)aColour.Green() / 255.0f;
    fillColour[2] = (VGfloat)aColour.Blue() / 255.0f;
    fillColour[3] = (VGfloat)aColour.Alpha() / 255.0f;

    vgSetfv(VG_CLEAR_COLOR, 4, fillColour);
    vgClear(0, 0, size.iWidth, size.iHeight);
    }


void CEglWindowSurface::DrawContentL(TInt aIndex)
    {
    ActivateL();
    CTestVgImage *vgImage = CTestVgImage::NewL(aIndex);
    CleanupStack::PushL(vgImage);
    vgDrawImage(vgImage->VGImage());
    CleanupStack::PopAndDestroy(vgImage);
    }

void CEglWindowSurface::GetSurfaceParamsL(TSurfaceParamsRemote &aParams)
    {
    RSurfaceManager surfaceManager;
    User::LeaveIfError(surfaceManager.Open());
    RSurfaceManager::TInfoBuf infoBuf;
    TInt err = surfaceManager.SurfaceInfo(SurfaceId(), infoBuf);
    User::LeaveIfError(err);
    surfaceManager.Close();
    RSurfaceManager::TSurfaceInfoV01& info = infoBuf();
    aParams.iSurfaceId = SurfaceId();
    aParams.iCommonParams.iAlignment = -1;  // N/A
    aParams.iCommonParams.iBuffers = info.iBuffers;
    aParams.iCommonParams.iOffsetToFirstBuffer = -1;
    aParams.iCommonParams.iPixelFormat = info.iPixelFormat;
    aParams.iCommonParams.iStrideInBytes = info.iStride;
    aParams.iCommonParams.iXSize = info.iSize.iWidth;
    aParams.iCommonParams.iYSize = info.iSize.iHeight;
    aParams.iCommonParams.iUseAttribList = KSurfaceParams[iParamIndex].iUseAttribList;
    for(TInt i = 0; i < KNumAttribs; i++)
        {
        aParams.iCommonParams.iAttribs[i] = KSurfaceParams[iParamIndex].iAttribs[i];
        }
    }

const TText *CEglWindowSurface::GetSurfaceTypeStr() const
    {
    return _S("CEglWindowSurface");
    }

TInt CEglWindowSurface::Notify(TNotification /*aWhen*/, TRequestStatus& /*aStatus*/, TUint32 /*aXTImes*/)
    {
    return KErrNotSupported;
    }

TInt CEglWindowSurface::WaitFor(TNotification /*aWhen*/, TRequestStatus& /*aStatus*/,
        TInt /*aTimeoutinMicroseconds*/, TUint32 & /*aTimeStamp*/)
    {
    return KErrNotSupported;
    }


// Factory function.
CSurface *CSurface::SurfaceFactoryL(TSurfaceType aSurfaceType)
    {
    switch(aSurfaceType)
        {
        case ESurfTypeRaw:
            return CRawSurface::NewL();
        case ESurfTypeEglWindow:
            return CEglWindowSurface::NewL();
        case ESurfTypeRawSingleBuffered:
            return CRawSingleBufferSurface::NewL();
        default:
            SURF_ASSERT(0);
            return NULL;
        }
    }

