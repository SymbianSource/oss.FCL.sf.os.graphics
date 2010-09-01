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

#include "T_DataWsScreenDevice.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname,							"CWsScreenDevice");

///	Fields
_LIT(KFldAlgStyle,								"alg_style");
_LIT(KFldBackLight,								"backlight");
_LIT(KFldBitmap,								"bitmap");
_LIT(KFldDefaultScreenNumber,					"defaultscreennumber");
_LIT(KFldExpected,								"expected");
_LIT(KFldExpectedCount,							"expectedcount");
_LIT(KFldFinish,								"finish");
_LIT(KFldFlags,									"flags");
_LIT(KFldFormatFieldNumber,						"%S%d");
_LIT(KFldFontUid,								"font_uid");
_LIT(KFldMode,									"mode");
_LIT(KFldPalette,								"palette");
_LIT(KFldPixel,									"pixel");
_LIT(KFldPixelSize,								"pixelsize");
_LIT(KFldRect,									"rect");
_LIT(KFldRect1,									"rect1");
_LIT(KFldRect2,									"rect2");
_LIT(KFldRotation,								"rotation");
_LIT(KFldStart,									"start");
_LIT(KFldTwips,									"twips");
_LIT(KFldTwipsSize,								"twipssize");
_LIT(KFldWindowGc,								"windowgc");
_LIT(KFldWs,									"ws");
_LIT(KFldFont,									"font");
_LIT(KFldContext,								"context");

///	Commands
_LIT(KCmdnew,									"new");
_LIT(KCmddisown,								"disown");
_LIT(KCmdDestructor,							"~CWsScreenDevice");
_LIT(KCmdDestructorGeneral,						"~");
_LIT(KCmdConstruct,								"Construct");
_LIT(KCmdCopyScreenToBitmap,					"CopyScreenToBitmap");
_LIT(KCmdCreateContext,							"CreateContext");
_LIT(KCmdCurrentScreenMode,						"CurrentScreenMode");
_LIT(KCmdGetCurrentScreenModeScale,				"GetCurrentScreenModeScale");
_LIT(KCmdGetCurrentScreenModeScaledOrigin,		"GetCurrentScreenModeScaledOrigin");
_LIT(KCmdGetDefaultScreenModeOrigin,			"GetDefaultScreenModeOrigin");
_LIT(KCmdGetDefaultScreenSizeAndRotation,		"GetDefaultScreenSizeAndRotation");
_LIT(KCmdGetFontById,							"GetFontById");
_LIT(KCmdGetRotationsList,						"GetRotationsList");
_LIT(KCmdGetScreenModeDisplayMode,				"GetScreenModeDisplayMode");
_LIT(KCmdGetScreenModeOrigin,					"GetScreenModeOrigin");
_LIT(KCmdGetScreenModeScale,					"GetScreenModeScale");
_LIT(KCmdGetScreenModeScaledOrigin,				"GetScreenModeScaledOrigin");
_LIT(KCmdGetScreenModeSizeAndRotation,			"GetScreenModeSizeAndRotation");
_LIT(KCmdGetScreenNumber,						"GetScreenNumber");
_LIT(KCmdGetScreenSizeModeList,					"GetScreenSizeModeList");
_LIT(KCmdNumScreenModes,						"NumScreenModes");
_LIT(KCmdRectCompare,							"RectCompare");
_LIT(KCmdPointerRect,							"PointerRect");
_LIT(KCmdScreenModeEnforcement,					"ScreenModeEnforcement");
_LIT(KCmdSetAppScreenMode,						"SetAppScreenMode");
_LIT(KCmdSetBackLight,							"SetBackLight");
_LIT(KCmdSetCustomPalette,						"SetCustomPalette");
_LIT(KCmdSetScreenSizeAndRotation,				"SetScreenSizeAndRotation");

_LIT(KCmdcheckPixels,							"checkPixels");
_LIT(KCmdcheckLineColor,						"checkLineColor");
_LIT(KCmdcheckRectColor,						"checkRectColor");

_LIT(KFormatFieldNumber,	"%S%d");

///	Logging
_LIT(KLogColor,									"Color Red=%d Green=%d Blue=%d Alpha=%d");
_LIT(KLogPoint,									"Point X=%d Y=%d");
_LIT(KLogError,									"Error=%d");
_LIT(KLogMissingParameter,						"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,						"Not expected value");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsScreenDevice* CT_DataWsScreenDevice::NewL()
/**
 * Two phase constructor
 */
	{
	CT_DataWsScreenDevice*	ret=new (ELeave) CT_DataWsScreenDevice();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsScreenDevice::CT_DataWsScreenDevice()
/**
 * Protected constructor. First phase construction
 */
:	CT_DataBitmapDevice()
,	iWsScreenDevice(NULL)
	{
	}

void CT_DataWsScreenDevice::ConstructL()
/**
 * Second phase construction
 */
	{
	}

CT_DataWsScreenDevice::~CT_DataWsScreenDevice()
/**
 * Public destructor
 */
	{
	DestroyData();
	}

TAny* CT_DataWsScreenDevice::GetObject()
/**
 * Return a pointer to the object that the data wraps
 *
 * @return	pointer to the object that the data wraps
 */
	{
	return iWsScreenDevice;
	}

void CT_DataWsScreenDevice::SetObjectL(TAny* aAny)
/**
 * Set the object that the data wraps
 *
 * @param	aObject object that the wrapper is testing
 *
 */
	{
	DestroyData();
	iWsScreenDevice = static_cast<CWsScreenDevice*> (aAny);
	}

void CT_DataWsScreenDevice::DisownObjectL()
/**
 * The object will no longer be owned by this
 *
 * @leave	KErrNotSupported if the the function is not supported
 */
	{
	iWsScreenDevice = NULL;
	}

void CT_DataWsScreenDevice::DestroyData()
	{
	delete iWsScreenDevice;
	iWsScreenDevice=NULL;
	}

MWsClientClass* CT_DataWsScreenDevice::GetClientClass() const
	{
	return iWsScreenDevice;
	}

MGraphicsDeviceMap*	CT_DataWsScreenDevice::GetGraphicsDeviceMap() const
	{
	return iWsScreenDevice;
	}

CGraphicsDevice*	CT_DataWsScreenDevice::GetGraphicsDevice() const
	{
	return iWsScreenDevice;
	}

CBitmapDevice*	CT_DataWsScreenDevice::GetBitmapDevice() const
	{
	return iWsScreenDevice;
	}


/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @param aAsyncErrorIndex	index of command. used for async calls
*
* @return ETrue if the command is processed
*/
TBool CT_DataWsScreenDevice::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor || aCommand==KCmdDestructorGeneral )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnewL(aSection);
		}
	else if ( aCommand==KCmddisown )
		{
		DisownObjectL();
		}	
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstruct(aSection);
		}
	else if ( aCommand==KCmdCopyScreenToBitmap )
		{
		DoCmdCopyScreenToBitmapL(aSection);
		}
	else if ( aCommand==KCmdCreateContext )
		{
		DoCmdCreateContextL(aSection);
		}
	else if ( aCommand==KCmdCurrentScreenMode )
		{
		DoCmdCurrentScreenMode(aSection);
		}
	else if ( aCommand==KCmdGetCurrentScreenModeScale )
		{
		DoCmdGetCurrentScreenModeScale(aSection);
		}
	else if ( aCommand==KCmdGetCurrentScreenModeScaledOrigin )
		{
		DoCmdGetCurrentScreenModeScaledOrigin(aSection);
		}
	else if ( aCommand==KCmdGetDefaultScreenModeOrigin )
		{
		DoCmdGetDefaultScreenModeOrigin(aSection);
		}
	else if ( aCommand==KCmdGetDefaultScreenSizeAndRotation )
		{
		DoCmdGetDefaultScreenSizeAndRotation(aSection);
		}
	else if ( aCommand==KCmdGetFontById )
		{
		DoCmdGetFontByIdL(aSection);
		}
	else if ( aCommand==KCmdGetRotationsList )
		{
			DoCmdGetRotationsListL(aSection);
		}
	else if ( aCommand==KCmdGetScreenModeDisplayMode )
		{
		DoCmdGetScreenModeDisplayMode(aSection);
		}
	else if ( aCommand==KCmdGetScreenModeOrigin )
		{
		DoCmdGetScreenModeOrigin(aSection);
		}
	else if ( aCommand==KCmdGetScreenModeScale )
		{
		DoCmdGetScreenModeScale(aSection);
		}
	else if ( aCommand==KCmdGetScreenModeScaledOrigin )
		{
		DoCmdGetScreenModeScaledOrigin(aSection);
		}
	else if ( aCommand==KCmdGetScreenModeSizeAndRotation )
		{
		DoCmdGetScreenModeSizeAndRotation(aSection);
		}
	else if ( aCommand==KCmdGetScreenNumber )
		{
		DoCmdGetScreenNumber(aSection);
		}
	else if ( aCommand==KCmdGetScreenSizeModeList )
		{
		DoCmdGetScreenSizeModeListL(aSection);
		}
	else if ( aCommand==KCmdNumScreenModes )
		{
		DoCmdNumScreenModes(aSection);
		}
	else if ( aCommand==KCmdPointerRect )
		{
		DoCmdPointerRect(aSection);
		}
	else if ( aCommand==KCmdRectCompare )
		{
		DoCmdRectCompare(aSection);
		}
	else if ( aCommand==KCmdScreenModeEnforcement )
		{
		DoCmdScreenModeEnforcement(aSection);
		}
	else if ( aCommand==KCmdSetAppScreenMode )
		{
		DoCmdSetAppScreenMode(aSection);
		}
	else if ( aCommand==KCmdSetBackLight )
		{
		DoCmdSetBackLight(aSection);
		}
	else if ( aCommand==KCmdSetCustomPalette )
		{
		DoCmdSetCustomPaletteL(aSection);
		}
	else if ( aCommand==KCmdSetScreenSizeAndRotation )
		{
		DoCmdSetScreenSizeAndRotation(aSection);
		}
	else if ( aCommand==KCmdcheckPixels )
		{
		DoCmdcheckPixels(aSection);
		}
	else if ( aCommand==KCmdcheckLineColor )
		{
		DoCmdcheckLineColor(aSection);
		}
	else if ( aCommand==KCmdcheckRectColor )
		{
		DoCmdcheckRectColor(aSection);
		}
	else if ( CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex) )
		{
		}
	else
		{
		ret=CT_DataWsClientClass::DoCommandL(*this, aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

/**
Test Constructor
*/
void CT_DataWsScreenDevice::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	RWsSession*	ws=NULL;
	TPtrC		wsName;
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}

	TInt	err;
	if ( ws!= NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new CWsScreenDevice(RWsSession)"));
		TRAP( err, iWsScreenDevice = new (ELeave) CWsScreenDevice(*ws) );
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new CWsScreenDevice()"));
		TRAP(err, iWsScreenDevice = new (ELeave) CWsScreenDevice());
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

/**
Test Destructor
*/
void CT_DataWsScreenDevice::DoCmdDestructor()
	{
	INFO_PRINTF1(_L("execute ~CWsScreenDevice()"));
	DestroyData();
	}

/**
Test Construct()
*/
void CT_DataWsScreenDevice::DoCmdConstruct(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	err=KErrNone;

	TInt	datDefaultScreenNumber;
	if ( GetIntFromConfig(aSection, KFldDefaultScreenNumber, datDefaultScreenNumber) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Construct(TInt)"));
		err=iWsScreenDevice->Construct(datDefaultScreenNumber);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Construct()"));
		err=iWsScreenDevice->Construct();
		}

	// Check the command return code.
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

/**
Test CopyScreenToBitmap()
*/
void CT_DataWsScreenDevice::DoCmdCopyScreenToBitmapL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	CFbsBitmap*	bitmap=NULL;
	TBool	hasBitmap=CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap);
	if ( !hasBitmap )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBitmap);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		TInt	err=KErrNone;

		TRect	datRect;
		if ( GetRectFromConfig(aSection, KFldRect, datRect) )
			{
			INFO_PRINTF1(_L("execute CopyScreenToBitmap(CFbsBitmap, TRect)"));
			err=iWsScreenDevice->CopyScreenToBitmap(bitmap, datRect);
			}
		else
			{
			INFO_PRINTF1(_L("execute CopyScreenToBitmap(CFbsBitmap)"));
			err=iWsScreenDevice->CopyScreenToBitmap(bitmap);
			}

		// Check the command return code.
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

/**
Test CreateContext()
*/
void CT_DataWsScreenDevice::DoCmdCreateContextL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPtrC	datWindowGc;
	if ( !GetStringFromConfig(aSection, KFldWindowGc, datWindowGc) )
		{
		//CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		
		// get BitmapUtilData object from parameters
		if ( !GetStringFromConfig(aSection, KFldContext, datWindowGc) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldContext());
			SetBlockResult(EFail);
			}
		else
			{
			CGraphicsContext*	context=NULL;
			INFO_PRINTF1(_L("execute CreateContext(CGraphicsContext*&)"));
			TInt	returnCode=GetGraphicsDevice()->CreateContext(context);
			// Check the command return code.
			if ( returnCode!=KErrNone )
				{
				ERR_PRINTF2(KLogError, returnCode);
				SetError(returnCode);
				}
			else
				{
				SetDataObjectL(datWindowGc, context);
				}
			}
		}
	else
		{
		// Execute command and log parameters
		CWindowGc*	windowGc;

		INFO_PRINTF1(_L("execute CreateContext(CWindowGc)"));
		TInt	returnCode = iWsScreenDevice->CreateContext(windowGc);

		// Check the command return code.
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}
		else
			{
			SetDataObjectL(datWindowGc, windowGc);
			}
		}
	}

/**
Test CurrentScreenMode()
*/
void CT_DataWsScreenDevice::DoCmdCurrentScreenMode(const TDesC& aSection)
	{
	TInt	actual=iWsScreenDevice->CurrentScreenMode();
	INFO_PRINTF2(_L("CurrentScreenMode()=%d"), actual);

	TInt	expected;
	if ( !GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test GetCurrentScreenModeScale()
*/
void CT_DataWsScreenDevice::DoCmdGetCurrentScreenModeScale(const TDesC& aSection)
	{
	TSize	actual=iWsScreenDevice->GetCurrentScreenModeScale();
	INFO_PRINTF3(_L("GetCurrentScreenModeScale(TInt) height=%d width=%d"), actual.iHeight, actual.iWidth);

	TSize	expected;
	if ( !GetSizeFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test GetCurrentScreenModeScaledOrigin()
*/
void CT_DataWsScreenDevice::DoCmdGetCurrentScreenModeScaledOrigin(const TDesC& aSection)
	{
	TPoint	actual=iWsScreenDevice->GetCurrentScreenModeScaledOrigin();
	INFO_PRINTF3(_L("GetCurrentScreenModeScaledOrigin() x=%d y=%d"), actual.iX, actual.iY);

	TPoint	expected;
	if ( !GetPointFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test GetDefaultScreenModeOrigin()
*/
void CT_DataWsScreenDevice::DoCmdGetDefaultScreenModeOrigin(const TDesC& aSection)
	{
	TPoint	actual=iWsScreenDevice->GetDefaultScreenModeOrigin();
	INFO_PRINTF3(_L("GetDefaultScreenModeOrigin() x=%d y=%d"), actual.iX, actual.iY);

	TPoint	expected;
	if ( !GetPointFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test GetDefaultScreenSizeAndRotation()
*/
void CT_DataWsScreenDevice::DoCmdGetDefaultScreenSizeAndRotation(const TDesC& aSection)
	{
	CFbsBitGc::TGraphicsOrientation	rotation;
	TBool							checkRotation=CT_GraphicsUtil::ReadGraphicsOrientation(*this, aSection, KFldRotation, rotation);

	TSize	pixelSize;
	TBool	checkPixelSize=GetSizeFromConfig(aSection, KFldPixelSize, pixelSize);

	TBool	twips=EFalse;
	GetBoolFromConfig(aSection, KFldTwips, twips);
	if ( twips )
		{
		TPixelsTwipsAndRotation	param;
		iWsScreenDevice->GetDefaultScreenSizeAndRotation(param);
		INFO_PRINTF2(_L("GetDefaultScreenSizeAndRotation(TPixelsTwipsAndRotation) Rotation=%d"), param.iRotation);
		INFO_PRINTF3(_L("Pixel height=%d width=%d"), param.iPixelSize.iHeight, param.iPixelSize.iWidth);
		INFO_PRINTF3(_L("Twips height=%d width=%d"), param.iTwipsSize.iHeight, param.iTwipsSize.iWidth);

		if ( checkRotation )
			{
			if ( rotation!=param.iRotation )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}

		if ( checkPixelSize )
			{
			if ( pixelSize!=param.iPixelSize )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}

		TSize	twipsSize;
		if ( GetSizeFromConfig(aSection, KFldTwipsSize, twipsSize) )
			{
			if ( twipsSize!=param.iTwipsSize )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	else
		{
		TPixelsAndRotation	param;
		iWsScreenDevice->GetDefaultScreenSizeAndRotation(param);
		INFO_PRINTF2(_L("GetDefaultScreenSizeAndRotation(TPixelsAndRotation) Rotation=%d"), param.iRotation);
		INFO_PRINTF3(_L("Pixel height=%d width=%d"), param.iPixelSize.iHeight, param.iPixelSize.iWidth);

		if ( checkRotation )
			{
			if ( rotation!=param.iRotation )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}

		if ( checkPixelSize )
			{
			if ( pixelSize!=param.iPixelSize )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test GetFontById()
*/
void CT_DataWsScreenDevice::DoCmdGetFontByIdL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call GetFontById()"));
	TBool	dataOk=ETrue;

	// get font uid from parameters
	TInt	fontUid;
	if(!GetIntFromConfig(aSection, KFldFontUid(), fontUid))
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("No %S"), &KFldFontUid());
		SetBlockResult(EFail);
		}
	// get TAlgStyle from parameters
	TAlgStyle*	algStyle=NULL;
	CT_GraphicsUtil::GetAlgStyleL(*this, aSection, KFldAlgStyle(), algStyle);
	if ( algStyle==NULL )
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("No %S"), &KFldAlgStyle());
		SetBlockResult(EFail);
		}
	// get font wrapper name
	TPtrC datFbsFont;
	if( !GetStringFromConfig(aSection, KFldFont, datFbsFont) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	// call GetFontById()
	else
		{
		if ( dataOk )
			{
			CFont* fbsFont=NULL;
			INFO_PRINTF1(_L("execute GetFontById(CFont *&aFont, TUid aUid, const TAlgStyle &aAlgStyle)"));
			TInt err = iWsScreenDevice->GetFontById(fbsFont, TUid::Uid(fontUid), *algStyle);
			
			if ( err!=KErrNone )
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(datFbsFont, fbsFont);
				}
			}
		}
	}

/**
Test GetRotationsList()
*/
void CT_DataWsScreenDevice::DoCmdGetRotationsListL(const TDesC& aSection)
	{
	CArrayFixFlat<TInt>*	rotations=new(ELeave) CArrayFixFlat<TInt>(1);
	CleanupStack::PushL(rotations);

	TInt	mode=0;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}
	else
		{
		TInt	err=iWsScreenDevice->GetRotationsList(mode, rotations);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			INFO_PRINTF2(_L("GetRotationsList count=%d"), rotations->Count());
			TInt	expectedCount;
			if ( GetIntFromConfig(aSection, KFldExpectedCount, expectedCount) )
				{
				if ( expectedCount!=rotations->Count() )
					{
					ERR_PRINTF1(KLogNotExpectedValue);
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Check elements in the rotations list"));
					TBuf<KMaxTestExecuteCommandLength>	tempStore;

					TBool	ok=ETrue;
					for ( TInt index = 0; (index < expectedCount) && (ok); )
						{
						tempStore.Format(KFormatFieldNumber, &KFldRotation, ++index);
						
						CFbsBitGc::TGraphicsOrientation	rotation;
						ok = CT_GraphicsUtil::ReadGraphicsOrientation(*this, aSection, tempStore, rotation);
						if ( !ok )
							{
							ERR_PRINTF2(KLogMissingParameter, &tempStore);
							SetBlockResult(EFail);
							}
						else
							{
							if( rotation != (*rotations)[index-1])
								{
								ERR_PRINTF1(KLogNotExpectedValue);
								SetBlockResult(EFail);
								ok = EFalse;
								}
							}
						}
					}
				}
			}
		}

	CleanupStack::PopAndDestroy();
	}

/**
Test GetScreenModeDisplayMode()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenModeDisplayMode(const TDesC& aSection)
	{
	TInt	mode=0;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}
	else
		{
		TDisplayMode	actual=iWsScreenDevice->GetScreenModeDisplayMode(mode);
		INFO_PRINTF2(_L("GetScreenModeDisplayMode(TInt)=%d"), actual);

		TDisplayMode	expected;
		if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldExpected, expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test GetScreenModeOrigin()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenModeOrigin(const TDesC& aSection)
	{
	TInt	mode=0;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}
	else
		{
		TPoint	actual=iWsScreenDevice->GetScreenModeOrigin(mode);
		INFO_PRINTF3(_L("GetScreenModeOrigin(TInt) x=%d y=%d"), actual.iX, actual.iY);

		TPoint	expected;
		if ( !GetPointFromConfig(aSection, KFldExpected, expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test GetScreenModeScale()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenModeScale(const TDesC& aSection)
	{
	TInt	mode=0;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}
	else
		{
		TSize	actual=iWsScreenDevice->GetScreenModeScale(mode);
		INFO_PRINTF3(_L("GetScreenModeScale(TInt) height=%d width=%d"), actual.iHeight, actual.iWidth);

		TSize	expected;
		if ( !GetSizeFromConfig(aSection, KFldExpected, expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test GetScreenModeScaledOrigin()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenModeScaledOrigin(const TDesC& aSection)
	{
	TInt	mode=0;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}
	else
		{
		TPoint	actual=iWsScreenDevice->GetScreenModeScaledOrigin(mode);
		INFO_PRINTF3(_L("GetScreenModeScaledOrigin(TInt) x=%d y=%d"), actual.iX, actual.iY);

		TPoint	expected;
		if ( !GetPointFromConfig(aSection, KFldExpected, expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test GetScreenModeSizeAndRotation()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenModeSizeAndRotation(const TDesC& aSection)
	{
	TInt	mode=0;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}

	CFbsBitGc::TGraphicsOrientation	rotation;
	TBool							checkRotation=CT_GraphicsUtil::ReadGraphicsOrientation(*this, aSection, KFldRotation, rotation);

	TSize	pixelSize;
	TBool	checkPixelSize=GetSizeFromConfig(aSection, KFldPixelSize, pixelSize);

	TBool	twips=EFalse;
	GetBoolFromConfig(aSection, KFldTwips, twips);
	if ( twips )
		{
		TPixelsTwipsAndRotation	param;
		iWsScreenDevice->GetScreenModeSizeAndRotation(mode, param);
		INFO_PRINTF2(_L("GetScreenModeSizeAndRotation(TInt, TPixelsTwipsAndRotation) Rotation=%d"), param.iRotation);
		INFO_PRINTF3(_L("Pixel height=%d width=%d"), param.iPixelSize.iHeight, param.iPixelSize.iWidth);
		INFO_PRINTF3(_L("Twips height=%d width=%d"), param.iTwipsSize.iHeight, param.iTwipsSize.iWidth);

		if ( checkRotation )
			{
			if ( rotation!=param.iRotation )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}

		if ( checkPixelSize )
			{
			if ( pixelSize!=param.iPixelSize )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}

		TSize	twipsSize;
		if ( GetSizeFromConfig(aSection, KFldTwipsSize, twipsSize) )
			{
			if ( twipsSize!=param.iTwipsSize )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	else
		{
		TPixelsAndRotation	param;
		iWsScreenDevice->GetScreenModeSizeAndRotation(mode, param);
		INFO_PRINTF2(_L("GetScreenModeSizeAndRotation(TInt, TPixelsAndRotation) Rotation=%d"), param.iRotation);
		INFO_PRINTF3(_L("Pixel height=%d width=%d"), param.iPixelSize.iHeight, param.iPixelSize.iWidth);

		if ( checkRotation )
			{
			if ( rotation!=param.iRotation )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}

		if ( checkPixelSize )
			{
			if ( pixelSize!=param.iPixelSize )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test GetScreenNumber()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenNumber(const TDesC& aSection)
	{
	// Execute command and log parameters
	TInt	actual=iWsScreenDevice->GetScreenNumber();
	INFO_PRINTF2(_L("execute GetScreenNumber()=%d"), actual);

	// Diaplay command return value, check if it matches the expected value
	TInt	expected;
	if ( !GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test GetScreenSizeModeList()
*/
void CT_DataWsScreenDevice::DoCmdGetScreenSizeModeListL(const TDesC& aSection)
	{
	RArray<TInt>	screenModes;
	CleanupClosePushL(screenModes);

	TInt	number = iWsScreenDevice->GetScreenSizeModeList(&screenModes);

	if (number < 0)
		{
		ERR_PRINTF2(KLogError, number);
		SetError(number);
		}
	else
		{
		INFO_PRINTF2(_L("GetScreenSizeModeList count=%d"), screenModes.Count());
		TInt	expectedCount;
		if ( GetIntFromConfig(aSection, KFldExpectedCount, expectedCount) )
			{
			if ( expectedCount!=screenModes.Count() || expectedCount != number)
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("Check elements in the modes list"));
				TBuf<KMaxTestExecuteCommandLength>	tempStore;

				TBool	ok=ETrue;
				for ( TInt index = 0; (index < number) && (ok); )
					{
					tempStore.Format(KFormatFieldNumber, &KFldMode, ++index);
					
					TInt expectedMode = 0;
					ok=GetIntFromConfig(aSection, tempStore, expectedMode);
					if ( !ok )
						{
						ERR_PRINTF2(KLogMissingParameter, &tempStore);
						SetBlockResult(EFail);
						}
					else
						{
						if( expectedMode != screenModes[index-1])
							{
							ERR_PRINTF1(KLogNotExpectedValue);
							SetBlockResult(EFail);
							ok = EFalse;
							}
						}
					}
				}
			}
		}

	CleanupStack::PopAndDestroy(&screenModes);
	}

/**
Test NumScreenModes()
*/
void CT_DataWsScreenDevice::DoCmdNumScreenModes(const TDesC& aSection)
	{
	TInt	actual=iWsScreenDevice->NumScreenModes();
	INFO_PRINTF2(_L("NumScreenModes()=%d"), actual);

	TInt	expected;
	if ( GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test PointerRect()
*/
void CT_DataWsScreenDevice::DoCmdPointerRect(const TDesC& aSection)
	{
	TRect	actual=iWsScreenDevice->PointerRect();
	LogRect(_L("PointerRect()"), actual);

	TRect	expected;
	if ( GetRectFromConfig(aSection, KFldExpected, expected) )
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test RectCompare()
*/
void CT_DataWsScreenDevice::DoCmdRectCompare(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TRect	datRect1;
	if ( !GetRectFromConfig(aSection, KFldRect1, datRect1) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect1);
		SetBlockResult(EFail);
		}

	TRect	datRect2;
	if ( !GetRectFromConfig(aSection, KFldRect2, datRect2) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect2);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TBool	actual=EFalse;

		// Execute command and log parameters
		TUint	datFlags;
		if ( CT_GraphicsUtil::ReadSpriteInCompare(*this, aSection, KFldFlags, datFlags) )
			{
			actual=iWsScreenDevice->RectCompare(datRect1, datRect2, datFlags);
			INFO_PRINTF2(_L("execute RectCompare(TRect, TRect, TUint)=%d"), actual);
			}
		else
			{
			actual=iWsScreenDevice->RectCompare(datRect1, datRect2);
			INFO_PRINTF2(_L("execute RectCompare(TRect, TRect)=%d"), actual);
			}

		// Diaplay command return value, check if it matches the expected value
		TBool	expected;
		if ( !GetBoolFromConfig(aSection, KFldExpected, expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/**
Test ScreenModeEnforcement()
*/
void CT_DataWsScreenDevice::DoCmdScreenModeEnforcement(const TDesC& aSection)
	{
	TScreenModeEnforcement	actual=iWsScreenDevice->ScreenModeEnforcement();
	INFO_PRINTF2(_L("execute ScreenModeEnforcement()=%d"), actual);

	TScreenModeEnforcement	expected;
	if ( CT_GraphicsUtil::ReadScreenModeEnforcement(*this, aSection, KFldExpected, expected) )
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

/**
Test SetAppScreenMode()
*/
void CT_DataWsScreenDevice::DoCmdSetAppScreenMode(const TDesC& aSection)
	{
	TInt	mode;
	if ( !GetIntFromConfig(aSection, KFldMode, mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRect2);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("SetAppScreenMode()"));
		iWsScreenDevice->SetAppScreenMode(mode);
		}
	}

/**
Test SetBackLight()
*/
void CT_DataWsScreenDevice::DoCmdSetBackLight(const TDesC& aSection)
	{
	TBool	backLight;
	if ( !GetBoolFromConfig(aSection, KFldBackLight, backLight) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBackLight);
		SetBlockResult(EFail);
		}
	else
		{
		TInt	err=iWsScreenDevice->SetBackLight(backLight);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

/**
Test SetCustomPalette()
*/
void CT_DataWsScreenDevice::DoCmdSetCustomPaletteL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CWsScreenDevice::SetCustomPalette() by previously created palette"));

	// get CPalette object passed as a parameter
	CPalette*	palette = NULL;
	
	if(!CT_GraphicsUtil::GetPaletteL(*this, aSection, KFldPalette(), palette))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPalette);
		SetBlockResult(EFail);
		}
	else
		{
		// call SetPalette()
		TInt err = iWsScreenDevice->SetCustomPalette(palette);
		if(err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}	
	}

/**
Test SetScreenSizeAndRotation()
*/
void CT_DataWsScreenDevice::DoCmdSetScreenSizeAndRotation(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	CFbsBitGc::TGraphicsOrientation	rotation;
	if ( !CT_GraphicsUtil::ReadGraphicsOrientation(*this, aSection, KFldRotation, rotation) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRotation);
		SetBlockResult(EFail);
		}

	TSize	pixelSize;
	if ( !GetSizeFromConfig(aSection, KFldPixelSize, pixelSize) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPixelSize);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TSize	twipsSize;
		if ( GetSizeFromConfig(aSection, KFldTwipsSize, twipsSize) )
			{
			TPixelsTwipsAndRotation	param;
			param.iPixelSize=pixelSize;
			param.iRotation=rotation;
			param.iTwipsSize=twipsSize;
			INFO_PRINTF1(_L("execute SetScreenSizeAndRotation(const TPixelsTwipsAndRotation&)"));
			iWsScreenDevice->SetScreenSizeAndRotation(param);
			}
		else
			{
			TPixelsAndRotation	param;
			param.iPixelSize=pixelSize;
			param.iRotation=rotation;
			INFO_PRINTF1(_L("execute SetScreenSizeAndRotation(const TPixelsAndRotation&)"));
			iWsScreenDevice->SetScreenSizeAndRotation(param);
			}
		}
	}

/**
* The source codes hereafter are NOT simple wrapper functions of CWsScreenDevice class,
* but use combinations of CWsScreenDevice (and other) class functions to check if what
* take from the CWsScreenDevice matches with the expected value specified in TEF testdata.
*
*/

/**
* DoCmdcheckPixels: to check the color of specified pixels on the screen device, and compare
* with the expected color specidied in the TEF testdata.
*		pixel<n>: specify the position of the screen device to check
*		expected<n>: specify the expected color to be compared with
*
*/
void CT_DataWsScreenDevice::DoCmdcheckPixels(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TBool	moreData=ETrue;
	for ( TInt index=1; moreData; ++index )
		{
		// get the pixel position on the screen to check/verify
		tempStore.Format(KFldFormatFieldNumber, &KFldPixel, index);
		TPoint	point;
		if ( !GetPointFromConfig(aSection, tempStore, point) )
			{
			moreData=EFalse;
			}
		else
			{
			TRgb	actual;
			iWsScreenDevice->GetPixel(actual, point);

			tempStore.Format(KFldFormatFieldNumber, &KFldExpected, index);
			TRgb	expected;
			if ( GetRgbFromConfig(aSection, tempStore, expected) )
				{
				if ( actual!=expected )
					{
					ERR_PRINTF5(KLogColor, actual.Red(), actual.Green(), actual.Blue(), actual.Alpha());
					ERR_PRINTF5(KLogColor, expected.Red(), expected.Green(), expected.Blue(), expected.Alpha());
					SetBlockResult(EFail);
					}
				}
			}
		}
	}

/**
* DoCmdcheckLineColor: to check the color of specified lines on the screen device, and compare
* with the expected color specidied in the TEF testdata.
*		start<n>: specify the starting position of the line on the screen device to check
*		finish<n>: specify the finish position of the line on the screen device to check
*		expected<n>: specify the expected color to be compared with
* Note: the straight line (from start to the finish points) is constructed by TLinearDDA
*
*/
void CT_DataWsScreenDevice::DoCmdcheckLineColor(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	CPalette*	palette=NULL;
	iWsScreenDevice->GetPalette(palette);

	TBool	moreData=ETrue;
	for ( TInt index=1; moreData; ++index )
		{
		// get the the start and finish points of the line to check/verify
		tempStore.Format(KFldFormatFieldNumber, &KFldStart, index);
		TPoint	startPoint;
		if ( !GetPointFromConfig(aSection, tempStore, startPoint) )
			{
			moreData=EFalse;
			}

		tempStore.Format(KFldFormatFieldNumber, &KFldFinish, index);
		TPoint	endPoint;
		if ( !GetPointFromConfig(aSection, tempStore, endPoint) )
			{
			moreData=EFalse;
			}

		if ( moreData )
			{
			// get the expected color from testdata
			tempStore.Format(KFldFormatFieldNumber, &KFldExpected, index);
			TRgb	expected;
			if ( !GetRgbFromConfig(aSection, tempStore, expected) )
				{
				ERR_PRINTF2(KLogMissingParameter, &tempStore);
				SetBlockResult(EFail);
				}
			else
				{
				if ( palette!=NULL )
					{
					expected=palette->NearestEntry(expected);
					}

				TLinearDDA	lineDDA;
				lineDDA.Construct(startPoint, endPoint, TLinearDDA::ECenter);

				// for each pixels on the line (from start to finish points), check if its color matches expected
				TPoint	point;
				while ( !lineDDA.SingleStep(point) )
					{
					TRgb	actual;
					iWsScreenDevice->GetPixel(actual, point);

					if ( actual!=expected )
						{
						ERR_PRINTF5(KLogColor, actual.Red(), actual.Green(), actual.Blue(), actual.Alpha());
						ERR_PRINTF5(KLogColor, expected.Red(), expected.Green(), expected.Blue(), expected.Alpha());
						ERR_PRINTF3(KLogPoint, point.iX, point.iY);
						SetBlockResult(EFail);
						}
					}
				}
			}
		}

	delete palette;
	}

/**
* DoCmdcheckRectColor: to check the color of specified rectangle on the screen device, and
* compare with the expected color specidied in the TEF testdata.
*		rect<n>: specify the rectangle area on the screen device to check
*		expected<n>: specify the expected color to be compared with
*
*/
void CT_DataWsScreenDevice::DoCmdcheckRectColor(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	CPalette*	palette=NULL;
	iWsScreenDevice->GetPalette(palette);

	TBool	moreData=ETrue;
	for ( TInt index=1; moreData; ++index )
		{
		// get the the rectangle to check/verify
		tempStore.Format(KFldFormatFieldNumber, &KFldRect, index);
		TRect	rect;
		if ( !GetRectFromConfig(aSection, tempStore, rect) )
			{
			moreData=EFalse;
			}
		else
			{
			// get the expected color from testdata
			tempStore.Format(KFldFormatFieldNumber, &KFldExpected, index);
			TRgb	expected;
			if ( !GetRgbFromConfig(aSection, tempStore, expected) )
				{
				ERR_PRINTF2(KLogMissingParameter, &tempStore);
				SetBlockResult(EFail);
				}
			else
				{
				if ( palette!=NULL )
					{
					expected=palette->NearestEntry(expected);
					}

				TPoint	point;
				for ( point.iX=rect.iTl.iX; point.iX<rect.iBr.iX; ++point.iX)
					{
					for ( point.iY=rect.iTl.iY; point.iY<rect.iBr.iY; ++point.iY )
						{
						TRgb	actual;
						iWsScreenDevice->GetPixel(actual, point);

						if ( actual!=expected )
							{
							ERR_PRINTF5(KLogColor, actual.Red(), actual.Green(), actual.Blue(), actual.Alpha());
							ERR_PRINTF5(KLogColor, expected.Red(), expected.Green(), expected.Blue(), expected.Alpha());
							ERR_PRINTF3(KLogPoint, point.iX, point.iY);
							SetBlockResult(EFail);
							}
						}
					}
				}
			}
		}

	delete palette;
	}
