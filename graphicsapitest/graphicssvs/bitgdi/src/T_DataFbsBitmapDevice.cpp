/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

/**
@test
@internalComponent

This contains CT_DataFbsBitmapDevice
*/

//	User includes
#include "T_DataFbsBitmapDevice.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdNewL,							"NewL");
_LIT(KCmdDestructor,					"~");
_LIT(KCmdDrawingBegin, 					"DrawingBegin");
_LIT(KCmdDrawingEnd,					"DrawingEnd");
_LIT(KCmdResize,						"Resize");
_LIT(KCmdSwapWidthAndHeight,			"SwapWidthAndHeight");

///	Parameters
_LIT(KFldBitmap,						"bitmap");
_LIT(KFldLibName,						"libname");
_LIT(KFldSize,							"size");
_LIT(KFldResize,						"resize");
_LIT(KFldAlways,						"Always");

// Logs
_LIT(KLogMissingParameter,				"Missing parameter '%S'");
_LIT(KLogErrNum,						"Execute failed with error = %d!");	
/*@}*/


/**
* Two phase constructor
*/
CT_DataFbsBitmapDevice* CT_DataFbsBitmapDevice::NewL()
	{
	CT_DataFbsBitmapDevice*	ret = new (ELeave) CT_DataFbsBitmapDevice();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsBitmapDevice::CT_DataFbsBitmapDevice()
:	CT_DataFbsDevice()
,	iFbsBitmapDevice(NULL)
	{
	}

/**
* Protected second phase construction
*/
void CT_DataFbsBitmapDevice::ConstructL()
	{
	}

/**
* Destructor.
*/
CT_DataFbsBitmapDevice::~CT_DataFbsBitmapDevice()
	{
	DestroyData();
	}

void CT_DataFbsBitmapDevice::DestroyData()
	{
	delete iFbsBitmapDevice;
	iFbsBitmapDevice=NULL;
	}

MGraphicsDeviceMap* CT_DataFbsBitmapDevice::GetGraphicsDeviceMap() const
	{
	return iFbsBitmapDevice;
	}

CGraphicsDevice* CT_DataFbsBitmapDevice::GetGraphicsDevice() const
	{
	return iFbsBitmapDevice;
	}

CBitmapDevice* CT_DataFbsBitmapDevice::GetBitmapDevice() const
	{
	return iFbsBitmapDevice;
	}

CFbsDevice* CT_DataFbsBitmapDevice::GetFbsDevice() const
	{
	return iFbsBitmapDevice;
	}
/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsBitmapDevice::GetObject()
	{
	return iFbsBitmapDevice;
	}

void CT_DataFbsBitmapDevice::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iFbsBitmapDevice = static_cast<CFbsBitmapDevice*> (aObject);
	}

void CT_DataFbsBitmapDevice::DisownObjectL() 
	{
	iFbsBitmapDevice = NULL;
	}

/**
* Process a command read from the ini file
*
* @param aDataWrapper	test step requiring command to be processed
* @param aCommand	the command to process
* @param aSection		the entry in the ini file requiring the command to be processed
*
* @return ETrue if the command is processed
*/
TBool CT_DataFbsBitmapDevice::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	/* Another work package will complete the implement of this */
	TBool	retVal = ETrue;

	if ( aCommand == KCmdNewL )
		{
		DoCmdNewL(aSection);
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else if (aCommand == KCmdDrawingBegin)
		{
		DoCmdDrawingBegin(aSection);
		}
	else if (aCommand == KCmdDrawingEnd)
		{
		DoCmdDrawingEnd(aSection);
		}
	else if (aCommand == KCmdResize)
		{
		DoCmdResize(aSection);
		}
	else if (aCommand == KCmdSwapWidthAndHeight)
		{
		DoCmdSwapWidthAndHeight();
		}
	else
		{
		retVal=CT_DataFbsDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}
	return retVal;
	}

void CT_DataFbsBitmapDevice::DoCmdNewL(const TDesC& aSection)
	{
	DestroyData();
	CFbsBitmap*	bitmap = NULL;
	
	if (!CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBitmap);
		SetBlockResult(EFail);
		return;
		}
	TInt	err = KErrNone;
	TPtrC	libName;
	if ( GetStringFromConfig(aSection, KFldLibName(), libName) )
		{
		INFO_PRINTF1(_L("execute NewL(CFbsBitmap*, const TDesC&)."));
		TRAP(err, iFbsBitmapDevice=CFbsBitmapDevice::NewL(bitmap, libName));
		}
	else
		{
		INFO_PRINTF1(_L("execute NewL(CFbsBitmap*)."));
		TRAP(err, iFbsBitmapDevice=CFbsBitmapDevice::NewL(bitmap));
		}
	
	if ( err != KErrNone )
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

void CT_DataFbsBitmapDevice::DoCmdDestructor()
	{
	DestroyData();
	}

void CT_DataFbsBitmapDevice::DoCmdDrawingBegin(const TDesC& aSection)
	{
	TBool Always = EFalse;
	GetBoolFromConfig(aSection, KFldAlways, Always);
	INFO_PRINTF2(_L("execute DrawingBegin function with the Always parameter=%d."), Always);
	iFbsBitmapDevice->DrawingBegin(Always);
	}

void CT_DataFbsBitmapDevice::DoCmdDrawingEnd(const TDesC& aSection)
	{
	TBool Always = EFalse;
	GetBoolFromConfig(aSection, KFldAlways, Always);
	INFO_PRINTF2(_L("execute DrawingEnd function with the Always parameter=%d."), Always);
	iFbsBitmapDevice->DrawingEnd(Always);
	}

void CT_DataFbsBitmapDevice::DoCmdResize(const TDesC& aSection)
	{
	TSize	datSize;
	TInt 	err = KErrNone;
	INFO_PRINTF1(_L("execute Resize(const TSize&)"));
	if (!GetSizeFromConfig(aSection, KFldResize, datSize))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldResize);
		SetBlockResult(EFail);
		}
	else
		{
		err = iFbsBitmapDevice->Resize(datSize);
		if (KErrNone != err)
			{
			ERR_PRINTF2(KLogErrNum, err);
			SetError(err);
			}
		}
	}

void CT_DataFbsBitmapDevice::DoCmdSwapWidthAndHeight()
	{
	INFO_PRINTF1(_L("execute SwapWidthAndHeight()"));
	TInt err = iFbsBitmapDevice->SwapWidthAndHeight();
	if (KErrNone != err)
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

