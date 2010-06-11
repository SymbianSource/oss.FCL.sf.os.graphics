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

#ifndef __EGLTEST_SURFACE_H__
#define __EGLTEST_SURFACE_H__


#include <e32base.h>
#include <w32std.h>
#include <graphics/surface.h>
#include <graphics/surfacemanager.h>
#include <graphics/surfaceupdateclient.h>
#include <graphics/suerror.h>
#include <EGL/egl.h>

enum TSurfaceType
    {
    ESurfTypeRaw,
    ESurfTypeEglWindow,
    ESurfTypeRawSingleBuffered,
    ESurfTypeMaxTypes  // Should be the last enum.
    };

// Index to Surface Parameters.
enum TSurfaceIndex
    {
    EStandardSurface,
    EBadAttribSurface,
    EEmptyAttribSurface,
    EStandard128sqSurface,
    EUnusualStrideSurface,
    EUnalignedPixelSizeSurface,
    ELargeSurface,
    };

// Notification enums - allows us to use one function to request several notifications.
enum TNotification
    {
    ENotifyWhenAvailable,
    ENotifyWhenDisplayed,
    ENotifyWhenDispXTimes,
    };

const TInt KNumAttribs = 3;

// Common between local and remote
struct TSurfaceParamsCommon
    {
    TSurfaceIndex iIndex;   // Check that it matches - or we've got the table wrong.
    TInt iXSize, iYSize;
    TInt iBuffers;
    TInt iAlignment;
    TInt iStrideInBytes;
    TInt iOffsetToFirstBuffer;
    TUidPixelFormat iPixelFormat;
    TBool iUseAttribList; //when false use NULL in at attribute parameter
    EGLint iAttribs[KNumAttribs];
    // Window info.
    TInt iXPos, iYPos;
    TDisplayMode iDisplayMode;
    };

struct TSurfaceParamsRemote
    {
    TSurfaceId           iSurfaceId;
    TSurfaceParamsCommon iCommonParams;
    TSurfaceType         iSurfaceType;
    };


//Interface for creating, drawing to and submitting content for a surface
//Derived classes can implement different types of surface, that can be
//used in the same test code. Ie surface manager surface (raw), egl window
//surface, video surface, viewfinder surface.
class CSurface: public CBase
    {
public:
    virtual void CreateL(TInt aIndex) = 0;
    virtual void DrawContentL(TInt aImageIndex) = 0;
    virtual void DrawContentL(const TRgb& aColour) = 0;
    virtual void SubmitContentL(TBool aShouldWaitForDisplay, TInt aRectIndex = 0) = 0;
    virtual TSurfaceId SurfaceId() const = 0;
    virtual void GetSurfaceParamsL(TSurfaceParamsRemote &aParams) = 0;
    virtual const TText *GetSurfaceTypeStr() const = 0;
    virtual TInt Notify(TNotification aWhen, TRequestStatus &aStatus, TUint32 aXTimes) = 0;
    virtual TInt WaitFor(TNotification aWhen, TRequestStatus &aStatus, TInt aTimeoutInMicroseconds, TUint32& aTimeStamp) = 0;

public:
    // Factory function to create a surface of aSurfType.
    static CSurface *SurfaceFactoryL(TSurfaceType aSurfType);
protected:
    TInt iParamIndex;
    };


//Raw surface implements CSurface for basic surface manager surface.
class CRawSurface : public CSurface
    {
public:
    static CRawSurface* NewL();
    ~CRawSurface();

    //From CSurface.
    void CreateL(TInt aIndex);
    void GetSurfAttribs(RSurfaceManager::TSurfaceCreationAttributesBuf &aSurfaceAttribs, TInt aIndex);
    void DrawContentL(TInt aIndex);
    void DrawContentL(const TRgb& aColour);
    void SubmitContentL(TBool aShouldWaitForDisplay, TInt aRectIndex = 0);
    TSurfaceId SurfaceId() const;
    void GetSurfaceParamsL(TSurfaceParamsRemote &aParams);
    virtual const TText *GetSurfaceTypeStr() const;
    virtual TInt Notify(TNotification aWhen, TRequestStatus &aStatus, TUint32 aXTimes);
    virtual TInt WaitFor(TNotification aWhen, TRequestStatus &aStatus, TInt aTimeoutInMicroseconds, TUint32& aTimeStamp);

protected:
    CRawSurface();
    void ConstructL();
    TUint8* MapSurfaceAndGetInfoLC(RSurfaceManager::TSurfaceInfoV01& aInfo);

private:
    static TInt PixelSize(TUidPixelFormat aPixelFormat);
    static TRequestStatus *GetRequestPtr(TNotification aWhen);

private:
    RSurfaceUpdateSession iSurfaceUpdate;
    TRequestStatus        iAvailable;
    TRequestStatus        iDisplayed;
    TRequestStatus        iDisplayedXTimes;
    TTimeStamp            iTimeStamp;
    TInt                  iDrawBuffer;
    RChunk                iSurfaceChunk;

protected:
    TInt                  iBuffers;
    RSurfaceManager       iSurfaceManager;
    TSurfaceId            iSurfaceId;
    };

// Simple wrapper to enable a list of "singlebuffer" in the list of buffer types.
class CRawSingleBufferSurface : public CRawSurface
    {
public:
    static CRawSingleBufferSurface* NewL();
    ~CRawSingleBufferSurface();

    //From CSurface.
    void CreateL(TInt aIndex);
    virtual const TText *GetSurfaceTypeStr() const;
    };

// Forward declare classes that we do not need to know the content of.
class CWindow;
class CTestVgImage;

class CEglWindowSurface : public CSurface
    {
public:
    static CEglWindowSurface* NewL();
    ~CEglWindowSurface();

    void CreateL(TInt aIndex);
    void DrawContentL(TInt aImageIndex);
    void DrawContentL(const TRgb& aColour);
    void SubmitContentL(TBool aShouldWaitForDisplay, TInt aRectIndex = 0);
    void ActivateL();
    TSurfaceId SurfaceId() const;
    void GetSurfaceParamsL(TSurfaceParamsRemote &aParams);
    virtual const TText *GetSurfaceTypeStr() const;
    virtual TInt Notify(TNotification aWhen, TRequestStatus &aStatus, TUint32 aXTimes);
    virtual TInt WaitFor(TNotification aWhen, TRequestStatus &aStatus, TInt aTimeoutInMicroseconds, TUint32& aTimeStamp);

private:
    CEglWindowSurface();
    void ConstructL();

private:
    EGLContext iContext;
    EGLSurface iSurface;
    EGLDisplay iDisplay;
    CWindow*   iWindow;
    };



#endif
