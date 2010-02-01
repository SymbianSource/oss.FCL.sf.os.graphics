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

This contains CT_DataFbsScreenDevice
*/

//	User includes
#include "T_DataFbsScreenDevice.h"
#include "T_GraphicsUtil.h"

/*@{*/
//Commands
_LIT(KCmdNewL,						"NewL");
_LIT(KCmdDestructor,				"~");

_LIT(KCmdChangeScreenDevice,		"ChangeScreenDevice");
_LIT(KCmdDrawSpriteBegin,		 	"DrawSpriteBegin");
_LIT(KCmdDrawSpriteEnd,		 		"DrawSpriteEnd");
_LIT(KCmdHardwareBitmap,		 	"HardwareBitmap");
_LIT(KCmdScreenNo,		 			"ScreenNo");
_LIT(KCmdSetAutoUpdate,		 		"SetAutoUpdate");
_LIT(KCmdUpdate,           			"Update");
_LIT(KCmdCancelSprite,				"CancelSprite");
_LIT(KCmdHideSprite,		 		"HideSprite");
_LIT(KCmdShowSprite,		 		"ShowSprite");

_LIT(KCmdDisown,           			"disown");

//Fields
_LIT(KFldExpected,					"expected");
_LIT(KLibName,						"libname");
_LIT(KScreenNumber,					"screen_number");
_LIT(KRgbColorRed,					"rgb_red");
_LIT(KRgbColorGreen,				"rgb_green");
_LIT(KRgbColorBlue,					"rgb_blue");
_LIT(KDisplayMode,					"displaymode");
_LIT(KAutoUpdate,					"autoupdate");
_LIT(KRegion,						"region");	
_LIT(KFbsScreenDevice, 				"FbsScrDevName");
_LIT(KFldRect,						"rect");
_LIT(KFldRegion,					"region");
_LIT(KSpriteBase,					"spritebase");

//Logging
_LIT(KLogMissingParameter,			"Missing parameter '%S'");

/*@}*/
/**
* Two phase constructor
*/
CT_DataFbsScreenDevice* CT_DataFbsScreenDevice::NewL()
	{
	CT_DataFbsScreenDevice*	ret = new (ELeave) CT_DataFbsScreenDevice();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsScreenDevice::CT_DataFbsScreenDevice()
:	CT_DataFbsDevice()
,	iFbsScreenDevice(NULL)
	{
	}

/**
* Protected second phase construction
*/
void CT_DataFbsScreenDevice::ConstructL()
	{
	}

/**
 * Set the object that the data wraps
 *
 * @param    aObject object that the wrapper is testing
 *
 */
void CT_DataFbsScreenDevice::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iFbsScreenDevice = static_cast<CFbsScreenDevice*> (aAny);
	}

void CT_DataFbsScreenDevice::DisownObjectL()
	{
	iFbsScreenDevice = NULL;
	}

/**
* Destructor. 
*/
CT_DataFbsScreenDevice::~CT_DataFbsScreenDevice()
	{
	DestroyData();
	}

void CT_DataFbsScreenDevice::DestroyData()
	{
	delete iFbsScreenDevice;
	iFbsScreenDevice=NULL;
	}

MGraphicsDeviceMap* CT_DataFbsScreenDevice::GetGraphicsDeviceMap() const
	{
	return iFbsScreenDevice;
	}

CGraphicsDevice* CT_DataFbsScreenDevice::GetGraphicsDevice() const
	{
	return iFbsScreenDevice;
	}

CBitmapDevice* CT_DataFbsScreenDevice::GetBitmapDevice() const
	{
	return iFbsScreenDevice;
	}

CFbsDevice* CT_DataFbsScreenDevice::GetFbsDevice() const
	{
	return iFbsScreenDevice;
	}

/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsScreenDevice::GetObject()
	{
	return iFbsScreenDevice;
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
TBool CT_DataFbsScreenDevice::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	/* Another work package will complete the implement of this */
	TBool	retVal = ETrue;

	if ( aCommand == KCmdNewL )
		{
		DoCmdNew(aSection);
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else if (aCommand == KCmdChangeScreenDevice)
		{
		DoCmdChangeScreenDeviceL(aSection);
		}
	else if (aCommand == KCmdDrawSpriteBegin)
		{
		DoCmdDrawSpriteBegin();
		}
	else if (aCommand == KCmdDrawSpriteEnd)
		{
		DoCmdDrawSpriteEnd();
		}
	else if (aCommand == KCmdHardwareBitmap)
		{
		DoCmdHardwareBitmap();
		}
	else if (aCommand == KCmdScreenNo)
		{
		DoCmdScreenNo(aSection);
		}
	else if (aCommand == KCmdSetAutoUpdate)
		{
		DoCmdSetAutoUpdate(aSection);
		}
	else if (aCommand == KCmdUpdate)
		{
		DoCmdUpdateL(aSection);
		}
	else if (aCommand == KCmdCancelSprite)
		{
		DoCmdCancelSprite();
		}
	else if (aCommand == KCmdHideSprite)
		{
		DoCmdHideSpriteL(aSection);
		}
	else if (aCommand == KCmdShowSprite)
		{
		DoCmdShowSpriteL(aSection);
		}
	else if (aCommand == KCmdDisown)
		{
		DisownObjectL();
		}
	else
		{
		retVal = CT_DataFbsDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return retVal;
	}

void CT_DataFbsScreenDevice::DoCmdNew(const TDesC& aSection)
	{
	DestroyData();

	// get display mode from parameters
	TDisplayMode	displayMode=ENone;
	TBool			hasDisplayMode=CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode);

	// get lib name from parameters

	TPtrC	libName;
	TBool	hasLibName=GetStringFromConfig(aSection, KLibName(), libName);

	TBool	useRgb = EFalse;

	TInt	red = 0;
	if(GetIntFromConfig(aSection, KRgbColorRed(), red))
		{
		useRgb=ETrue;
		}

	TInt	green = 0;
	if(GetIntFromConfig(aSection, KRgbColorGreen(), green))
		{
		useRgb=ETrue;
		}

	TInt	blue = 0;
	if(GetIntFromConfig(aSection, KRgbColorBlue(), blue))
		{
		useRgb = ETrue;
		}

	// get screen number from parameters
	TInt	screenNumber = 0;
	GetIntFromConfig(aSection, KScreenNumber(), screenNumber);

	TInt	err=KErrNone;
	if ( !hasDisplayMode )
		{
		//	Find the best display mode
		for ( TDisplayMode testMode=TDisplayMode(ENone+1); testMode<EColorLast; testMode=TDisplayMode(testMode+1) )
			{
			if ( hasLibName ) // if lib name is given
				{		
				if ( useRgb )
					{
					TRAP(err, iFbsScreenDevice=CFbsScreenDevice::NewL(libName, testMode, TRgb(red, green, blue)));
					}
				else
					{
					TRAP(err, iFbsScreenDevice=CFbsScreenDevice::NewL(libName, testMode));
					}
				}
			else // if lib name is not given
				{		
				TRAP(err, iFbsScreenDevice=CFbsScreenDevice::NewL(screenNumber, testMode));
				}

			//	Test if Mode OK
			if ( err==KErrNone )
				{
				displayMode=testMode;
				hasDisplayMode=ETrue;
				delete iFbsScreenDevice;
				iFbsScreenDevice=NULL;
				INFO_PRINTF2(_L("Mode %d supported"), testMode);
				}
			}
		}

	if ( hasDisplayMode )
		{
		if ( hasLibName ) // if lib name is given
			{		
			if ( useRgb )
				{
				TRAP(err, iFbsScreenDevice=CFbsScreenDevice::NewL(libName, displayMode, TRgb(red, green, blue)));
				}
			else
				{
				TRAP(err, iFbsScreenDevice=CFbsScreenDevice::NewL(libName, displayMode));
				}
			}
		else // if lib name is not given
			{		
			TRAP(err, iFbsScreenDevice=CFbsScreenDevice::NewL(screenNumber, displayMode));
			}

		if ( err!=KErrNone )
			{
			ERR_PRINTF2(_L("NewL failed with error %d"), err);
			SetError(err);
			}
		}
	else
		{
		ERR_PRINTF1(_L("No Display Mode"));
		SetBlockResult(EFail);
		}
	}

void CT_DataFbsScreenDevice::DoCmdDestructor()
	{
	DestroyData();
	}

void CT_DataFbsScreenDevice::DoCmdChangeScreenDeviceL(const TDesC& aSection)
	{
	TPtrC	FbsScreenDevNameOld;
	CFbsScreenDevice*	FbsScreenDevOld = NULL;
	
	if (GetStringFromConfig(aSection, KFbsScreenDevice, FbsScreenDevNameOld))
		{
		FbsScreenDevOld = static_cast<CFbsScreenDevice*>(GetDataObjectL(FbsScreenDevNameOld));
		INFO_PRINTF1(_L("FbsScreenDevice::ChangeScreenDevice(CFbsScreenDevice *aOldDevice) is called!"));
		iFbsScreenDevice->ChangeScreenDevice(FbsScreenDevOld);
		}
	else
		{
		INFO_PRINTF2(KLogMissingParameter, &KFbsScreenDevice);
		SetBlockResult(EFail);
		}
	}

void CT_DataFbsScreenDevice::DoCmdDrawSpriteBegin()
	{
	INFO_PRINTF1(_L("FbsScreenDevice::DrawSpriteBegin() is called!"));
	iFbsScreenDevice->DrawSpriteBegin();
	}

void CT_DataFbsScreenDevice::DoCmdDrawSpriteEnd()
	{
	INFO_PRINTF1(_L("FbsScreenDevice::DrawSpriteEnd() is called!"));
	iFbsScreenDevice->DrawSpriteEnd();
	}

void CT_DataFbsScreenDevice::DoCmdHardwareBitmap()
	{	
	INFO_PRINTF1(_L("FbsScreenDevice::HardwareBitmap() is called!"));
	RHardwareBitmap handle = iFbsScreenDevice->HardwareBitmap();
	INFO_PRINTF2(_L("The actual handle from HardwareBitmap() is %d"), handle.iHandle);
	}

void CT_DataFbsScreenDevice::DoCmdScreenNo(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("FbsScreenDevice::ScreenNo() is called!"));
	TInt actual = iFbsScreenDevice->ScreenNo();
	INFO_PRINTF2(_L("The actual result of ScreenNo() is %d"), actual);

	// Diaplay command return value, check if it matches the expected value
	TInt expected;
	if (!GetIntFromConfig(aSection, KFldExpected, expected))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if (actual!=expected)
			{
			ERR_PRINTF3(_L("The expected result %d is not equal to the actual result %d!"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsScreenDevice::DoCmdSetAutoUpdate(const TDesC& aSection)
	{
	TBool autoupdate;
	if (!GetBoolFromConfig(aSection, KAutoUpdate, autoupdate))
		{
		ERR_PRINTF2(KLogMissingParameter, &KAutoUpdate);
		SetBlockResult(EFail);
		}
	else
		{
		iFbsScreenDevice->SetAutoUpdate(autoupdate);
		INFO_PRINTF2(_L("FbsScreenDevice::SetAutoUpdate(%d) has been called."), autoupdate);
		}
	}

void CT_DataFbsScreenDevice::DoCmdUpdateL(const TDesC& aSection)
	{
	RRegion region(1);
	CleanupClosePushL(region);
	
	if (GetRegionFromConfig(aSection, KRegion(), region))
		{
		INFO_PRINTF1(_L("FbsScreenDevice::Update(const TRegion &aRegion) has been called."));
		iFbsScreenDevice->Update(region);
		}
	else
		{
		INFO_PRINTF1(_L("FbsScreenDevice::Update() has been called."));
		iFbsScreenDevice->Update();
		}
	CleanupStack::PopAndDestroy(&region);
	}

void CT_DataFbsScreenDevice::DoCmdCancelSprite()
	{
	INFO_PRINTF1(_L("execute CancelSprite()!"));
	iFbsScreenDevice->CancelSprite();
	}

void CT_DataFbsScreenDevice::DoCmdHideSpriteL(const TDesC& aSection)
	{
	TRect datRect;
	RRegion	datRegion(1);
	CleanupClosePushL(datRegion);

	if (GetRectFromConfig(aSection, KFldRect, datRect))
		{
		if (GetRegionFromConfig(aSection, KFldRegion, datRegion))
			{
			INFO_PRINTF1(_L("execute HideSprite(const TRect &aRect, const TRegion *aClippingRegion)!"));
			TSpriteBase* spritebase = iFbsScreenDevice->HideSprite(datRect, &datRegion);
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldRegion);
			SetBlockResult(EFail);
			}
		}
	else
		{
		INFO_PRINTF1(_L("execute HideSprite(void)!"));
		TSpriteBase* spritebase = iFbsScreenDevice->HideSprite();
		}
	
	CleanupStack::PopAndDestroy(&datRegion);
	}


void CT_DataFbsScreenDevice::DoCmdShowSpriteL(const TDesC& aSection)
	{
	TSpriteBase* spritebase = NULL;
	TPtrC SpriteBase;
	TRect datRect;
	RRegion	datRegion(1);
	CleanupClosePushL(datRegion);
			
	if (GetStringFromConfig(aSection, KSpriteBase, SpriteBase))
		{
		spritebase = static_cast<TSpriteBase*>(GetDataObjectL(SpriteBase));
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KSpriteBase);
		SetBlockResult(EFail);
		}
		
	if (GetRectFromConfig(aSection, KFldRect, datRect))
		{
		if (GetRegionFromConfig(aSection, KFldRegion, datRegion))
			{
			INFO_PRINTF1(_L("execute ShowSprite(TSpriteBase*, const TRect&, const TRegion*)!"));
			iFbsScreenDevice->ShowSprite(spritebase, datRect, &datRegion);
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldRegion);
			SetBlockResult(EFail);
			}
		}
	else
		{
		INFO_PRINTF1(_L("execute ShowSprite(TSpriteBase*)!"));
		iFbsScreenDevice->ShowSprite(spritebase);
		}
	CleanupStack::PopAndDestroy(&datRegion);
	}
