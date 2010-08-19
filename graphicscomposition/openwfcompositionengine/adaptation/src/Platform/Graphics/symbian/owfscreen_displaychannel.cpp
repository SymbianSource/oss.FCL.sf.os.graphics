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
// Display Channel adaptation for owfscreen
// 
//

#include "owfscreen.h"

#include <assert.h>
#include <e32base.h>
#include <e32debug.h>
#include <pixelformats.h>
#include <hal.h>

#include "owfimage.h"

#include "owfdisplaycontext.h"

static const int KDefaultScreenNum = 0;
_LIT(KDisplayChannelLdd, "display0.ldd");

COwfcDisplayDevice* COwfcDisplayDevice::gDisplayDeviceInstancePtr = NULL;

static OWFboolean GetDisplayInfo(OWFint aScreenNumber, COWFScreenDisplayInfo * & aInfo)
    {
    if (!OWF_Screen_Valid(aScreenNumber))
        {
        return OWF_FALSE;
        }
    COwfcDisplayDevice *pInstance = COwfcDisplayDevice::SingletonInstance();
    if (!pInstance)
        {
        return OWF_FALSE;
        }
    TInt index = pInstance->iScreenDisplayInfo.Find((TInt)aScreenNumber, COwfcDisplayDevice::ScreenCompare);
    if (index == KErrNotFound)
        {
        return OWF_FALSE;
        }
    aInfo = pInstance->iScreenDisplayInfo[index];
    return OWF_TRUE;
    }

COwfcDisplayDevice* COwfcDisplayDevice::SingletonInstance()
    {
    if (!gDisplayDeviceInstancePtr)
        {
        TInt err = User::LoadLogicalDevice(KDisplayChannelLdd);
        if (err != KErrNone && err != KErrAlreadyExists)
            {
            DPRINT(("COwfcDisplayDevice::SingletonInstance: Error in LoadLogicalDevice."));
            return NULL;
            }
        gDisplayDeviceInstancePtr = new COwfcDisplayDevice;
        if (!gDisplayDeviceInstancePtr)
            {
            DPRINT(("Cannot create Display Device singleton instance."));
            return NULL;
            }
        }
    return gDisplayDeviceInstancePtr;
    }

TInt COwfcDisplayDevice::CloseScreen(TInt aScreen, OWFboolean *aCanDelete)
    {
    TInt index = iScreenDisplayInfo.Find(aScreen, ScreenCompare);
    if (index == KErrNotFound)
        {
        return KErrNotFound;
        }
    delete iScreenDisplayInfo[index];
    iScreenDisplayInfo.Remove(index);
    if (iScreenDisplayInfo.Count() == 0)
        {
        *aCanDelete = OWF_TRUE;
        }
    return KErrNone;
    }

COWFScreenDisplayInfo::COWFScreenDisplayInfo(TInt aScreen):iScreenNumber(aScreen)
    {
    }

COWFScreenDisplayInfo::~COWFScreenDisplayInfo()
    {
    TInt count = iCompositionBuffer.Count();
    for(TInt ii = 0; ii < count; ++ii)
        {
        TBufferAddress* addr = iCompositionBuffer[ii];
        if (addr)
            {
            addr->iChunk.Close();
            delete addr;
            }
        }
    iCompositionBuffer.Close();
    // We do not need to use this boolean
    TBool unused;
    // Reset the initial rotation for next time.
    iDispChan.SetRotation(iDefaultRotation, unused);
    iDispChan.Close();
    }

void COWFScreenDisplayInfo::ConstructL()
    {
    User::LeaveIfError(iDispChan.Open(iScreenNumber));
// Temporary work-a-round for syborg
//#ifdef __WINS__
    iDefaultRotation = RDisplayChannel::ERotationNormal;
//#else
//    iDefaultRotation = RDisplayChannel::ERotation270CW;
//#endif
    
    switch (iDispChan.CurrentRotation())
        {
        case RDisplayChannel::ERotationNormal:
            iScreenInfo.currentRotation = OWF_ROTATION_0;
            break;
        case RDisplayChannel::ERotation90CW:
            iScreenInfo.currentRotation = OWF_ROTATION_90;
            break;
        case RDisplayChannel::ERotation180:
            iScreenInfo.currentRotation = OWF_ROTATION_180;
            break;
        case RDisplayChannel::ERotation270CW:
            iScreenInfo.currentRotation = OWF_ROTATION_270;
            break;
        default:
            DPRINT(("Unsupported rotation %d", rotation));
            User::Leave(KErrNotSupported);
        }
    
    TPckgBuf<RDisplayChannel::TDisplayInfo> pkgInfo;
    iDispChan.GetDisplayInfo(pkgInfo);
    RDisplayChannel::TDisplayInfo info = pkgInfo();
    switch(info.iPixelFormat)
        {
        case EUidPixelFormatXRGB_8888:
        case EUidPixelFormatARGB_8888:
        case EUidPixelFormatARGB_8888_PRE:
            iScreenInfo.pixelFormat = OWF_IMAGE_XRGB8888;
            break;
#ifdef SUPPORT_565_OUTPUT
        case EUidPixelFormatRGB_565:
            iScreenInfo.pixelFormat = OWF_IMAGE_RGB565;
            break;
#endif
        default:
            DPRINT(("Unsupported pixel format 0x%08.8x", info.iPixelFormat));
            User::Leave(KErrNotSupported);
        }
    iScreenInfo.supportedRotations = info.iAvailableRotations;
    TInt bytesPerPixel = 2;
    if( info.iBitsPerPixel == 24 || info.iBitsPerPixel == 32)
        {
        bytesPerPixel = 4;
        }
    
    switch (iDefaultRotation)
        {
        case RDisplayChannel::ERotation90CW:
            iRotationOffset = 1;
            break;
        case RDisplayChannel::ERotation180:
            iRotationOffset = 2;
            break;
        case RDisplayChannel::ERotation270CW:
            iRotationOffset = 3;
            break;
        case RDisplayChannel::ERotationNormal:
        default:
            OWF_ASSERT(iDefaultRotation == RDisplayChannel::ERotationNormal);
            iRotationOffset = 0;
        }
    
    if (iRotationOffset & 1)
        {
        // Either 90 or 270 degree rotation, so reverse the normal and flipped
        // sizes (which relate to zero degrees rotation).
        iScreenInfo.normal.width = info.iFlipped.iWidth;
        iScreenInfo.normal.height = info.iFlipped.iHeight;
        iScreenInfo.normal.stride = info.iFlipped.iOffsetBetweenLines;
        iScreenInfo.flipped.width = info.iNormal.iWidth;
        iScreenInfo.flipped.height = info.iNormal.iHeight;
        iScreenInfo.flipped.stride = info.iNormal.iOffsetBetweenLines;
        }
    else
        {
        iScreenInfo.normal.width = info.iNormal.iWidth;
        iScreenInfo.normal.height = info.iNormal.iHeight;
        iScreenInfo.normal.stride = info.iNormal.iOffsetBetweenLines;
        iScreenInfo.flipped.width = info.iFlipped.iWidth;
        iScreenInfo.flipped.height = info.iFlipped.iHeight;
        iScreenInfo.flipped.stride = info.iFlipped.iOffsetBetweenLines;
        }
        
    if (iScreenInfo.normal.stride < 0)
        {
        iScreenInfo.normal.stride = bytesPerPixel * iScreenInfo.normal.width;
        }
    if (iScreenInfo.flipped.stride < 0)
        {
        iScreenInfo.flipped.stride = bytesPerPixel * iScreenInfo.flipped.width;
        }
    
    for(TInt ii = 0; ii < info.iNumCompositionBuffers; ++ii)
        {
        iCompositionBuffer.AppendL(NULL);
        TBufferAddress* address = new(ELeave) TBufferAddress;
        iCompositionBuffer[ii] = address;
        User::LeaveIfError(iDispChan.GetCompositionBufferInfo(ii, address->iChunk, address->iOffset));
        address->iAddress = address->iChunk.Base() + address->iOffset;
        }
    }

COWFScreenDisplayInfo* COWFScreenDisplayInfo::NewL(TInt aScreen)
    {
    COWFScreenDisplayInfo* sdi = new(ELeave) COWFScreenDisplayInfo(aScreen);
    CleanupStack::PushL(sdi);
    sdi->ConstructL();
    CleanupStack::Pop(sdi);
    return sdi;
    }

COwfcDisplayDevice::~COwfcDisplayDevice()
    {
    COwfcDisplayDevice::gDisplayDeviceInstancePtr = NULL;
    iScreenDisplayInfo.Close();
    }

TInt COwfcDisplayDevice::OpenScreen(TInt aScreen)
    {
    if (!OWF_Screen_Valid(aScreen))
        {
        return KErrNotSupported;
        }
    TInt err = iScreenDisplayInfo.Find(aScreen, ScreenCompare);
    if (err != KErrNotFound)
        {
        return KErrInUse;
        }
    COWFScreenDisplayInfo *info = NULL;
    TRAP(err, info = COWFScreenDisplayInfo::NewL(aScreen));
    if (err == KErrNone)
        {
        err = iScreenDisplayInfo.Append(info);
        if (err != KErrNone)
            {
            delete info;
            }
        }
    return err;
    }

OWFboolean COWFScreenDisplayInfo::Blit(void* buffer, OWF_ROTATION rotation)
    {
    TInt err;
    
    if (iScreenInfo.currentRotation != rotation)
        {
        TInt totalRotation;
        RDisplayChannel::TDisplayRotation dcRotation;

        // Change the rotation to a numeric index from zero
        switch (rotation)
            {
            case OWF_ROTATION_90:
                totalRotation = 1;
                break;
            case OWF_ROTATION_180:
                totalRotation = 2;
                break;
            case OWF_ROTATION_270:
                totalRotation = 3;
                break;
            case OWF_ROTATION_0:
            default:
                OWF_ASSERT(rotation == OWF_ROTATION_0);
                totalRotation = 0;
                break;
            }

        // Add the initial rotation of the device and wrap to get final one.
        totalRotation = (totalRotation + iRotationOffset) % 4;

        // Now change the final rotation to the display channel rotation
        switch (totalRotation)
            {
            case 1:
                dcRotation = RDisplayChannel::ERotation90CW;
                break;
            case 2:
                dcRotation = RDisplayChannel::ERotation180;
                break;
            case 3:
                dcRotation = RDisplayChannel::ERotation270CW;
                break;
            case 0:
            default:
                OWF_ASSERT(totalRotation == 0);
                dcRotation = RDisplayChannel::ERotationNormal;
                break;
            }

        if (!(iScreenInfo.supportedRotations & dcRotation))
            {
            return OWF_FALSE;
            }
        // We do not need to use this boolean
        TBool displayConfigChanged;
        err = iDispChan.SetRotation(dcRotation, displayConfigChanged);
        OWF_ASSERT(err == KErrNone);
        iScreenInfo.currentRotation = rotation;    
        }
    
    TUint bufferIndex;
	TRequestStatus status;
	iDispChan.GetCompositionBuffer(bufferIndex, status);
	User::WaitForRequest(status);
	
	err = status.Int();
	if (err != KErrNone || bufferIndex >= iCompositionBuffer.Count()) 
		{
		return OWF_FALSE;
		}
	
	TUint8* firstPixelAddr = iCompositionBuffer[bufferIndex]->iAddress;
	
	// Only 32 or 16bpp supported and assume stride == width * pixel size
	TInt size = 0;
	if (rotation == OWF_ROTATION_0 || rotation == OWF_ROTATION_180)
	    {
	    size = iScreenInfo.normal.height * iScreenInfo.normal.stride; 
	    }
	else
	    {
        size = iScreenInfo.flipped.height * iScreenInfo.flipped.stride; 
	    }
	Mem::Move(firstPixelAddr, buffer, size);
	RDisplayChannel::TPostCount postCount;
	err = iDispChan.PostCompositionBuffer(NULL, postCount);
	if (err == KErrNone)
		{
		// This will throttle composition more than necessary,
		// since it will wait until buffer is being displayed
		iDispChan.WaitForPost(postCount, status);
		User::WaitForRequest(status);
		err = status.Int();
		}
	return (err == KErrNone) ? OWF_TRUE : OWF_FALSE;
	}

TInt COWFScreenDisplayInfo::ScreenNumber() const
    {
    return iScreenNumber;
    }
    
void COWFScreenDisplayInfo::GetScreenInfo(OWF_SCREEN *header) const
    {
    *header = iScreenInfo;
    }

//

OWF_API_CALL OWFint OWF_Screen_GetDefaultNumber()
    {
    return KDefaultScreenNum;
    }

OWF_API_CALL OWFboolean OWF_Number_Of_Screens(OWFint *pNumberOfScreens)
    {
    TInt screens = 0;
    TInt err = HAL::Get(HAL::EDisplayNumberOfScreens, screens);
    if (err != KErrNone)
        {
        DPRINT(("Cannot get number of screens from HAL, error: %d", err));
        return OWF_FALSE;
        }
    *pNumberOfScreens = (OWFint)screens;
    return OWF_TRUE;
    }

OWF_API_CALL OWFboolean OWF_Screen_Valid(OWFint screen)
    {
    OWFint numberOfScreens = 0;
    if (!OWF_Number_Of_Screens(&numberOfScreens))
        {
        return OWF_FALSE;
        }
    if (screen < 0 || screen >= numberOfScreens)
        {
        return OWF_FALSE;
        }
    return OWF_TRUE;
    }

OWF_API_CALL OWFboolean OWF_Screen_Valid_And_Available(OWFint screen)
    {
    if (!OWF_Screen_Valid(screen))
        {
        return OWF_FALSE;
        }
    COwfcDisplayDevice *pInstance = COwfcDisplayDevice::SingletonInstance();
    if (!pInstance)
        {
        return OWF_TRUE;
        }
    TInt index = pInstance->iScreenDisplayInfo.Find((TInt)screen, COwfcDisplayDevice::ScreenCompare);
    if (index != KErrNotFound)
        {
        return OWF_FALSE; 
        }
    return OWF_TRUE;
    }

OWF_API_CALL OWFboolean OWF_Screen_GetHeader(OWFint screen, OWF_SCREEN* header)
    {
    COWFScreenDisplayInfo* info;
    if (!GetDisplayInfo(screen, info))
        {
        return OWF_FALSE;
        }
    info->GetScreenInfo(header);
    return OWF_TRUE;
    }

OWF_API_CALL OWFboolean OWF_Screen_Blit(OWFint screenNumber, void* buffer, OWF_ROTATION rotation)
    {
    COWFScreenDisplayInfo* info;
    if (!GetDisplayInfo(screenNumber, info))
        {
        return OWF_FALSE;
        }
    return info->Blit(buffer, rotation);
    }

OWF_API_CALL OWFboolean
OWF_Screen_Rotation_Supported(OWFint screen)
    {
    COWFScreenDisplayInfo* info;
    if (!GetDisplayInfo(screen, info))
        {
        return OWF_FALSE;
        }
    OWF_SCREEN header;
    info->GetScreenInfo(&header);
    return (header.supportedRotations & 0xF == 0xF) ? OWF_TRUE : OWF_FALSE;
    }
