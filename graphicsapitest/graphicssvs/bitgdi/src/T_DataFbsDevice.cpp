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

This contains CT_DataFbsDevice
*/

#include "T_DataFbsDevice.h"
#include "T_GraphicsUtil.h"

/*@{*/
//Commands
_LIT(KCmdcheckPixels,				"checkPixels");
_LIT(KCmdcheckLineColor,			"checkLineColor");
_LIT(KCmdcheckRectColor,			"checkRectColor");
_LIT(KCmdSizeInPixels,				"SizeInPixels");
_LIT(KCmdGetFontById,							"GetFontById");
_LIT(KCmdGetNearestFontInPixels,				"GetNearestFontInPixels");
_LIT(KCmdGetNearestFontInTwips,					"GetNearestFontInTwips");
_LIT(KCmdGetNearestFontToDesignHeightInPixels,	"GetNearestFontToDesignHeightInPixels");
_LIT(KCmdGetNearestFontToDesignHeightInTwips,	"GetNearestFontToDesignHeightInTwips");
_LIT(KCmdGetNearestFontToMaxHeightInPixels,		"GetNearestFontToMaxHeightInPixels");
_LIT(KCmdGetNearestFontToMaxHeightInTwips,		"GetNearestFontToMaxHeightInTwips");
_LIT(KCmdCreateContext,				"CreateContext");
_LIT(KCmdRectCompare,				"RectCompare");
_LIT(KCmdSetCustomPalette,			"SetCustomPalette");
_LIT(KCmdGraphicsAccelerator,		"GraphicsAccelerator");
_LIT(KCmdOrientation,				"Orientation");
_LIT(KCmdDisplayMode16M,			"DisplayMode16M");
_LIT(KCmdSetScalingFactor,			"SetScalingFactor");
_LIT(KCmdGetDrawRect,				"GetDrawRect");
_LIT(KCmdSetDrawDeviceOffset,		"SetDrawDeviceOffset");
_LIT(KCmdRegisterLinkedTypeface,	"RegisterLinkedTypeface");

//Fields
_LIT(KFldAlgStyle,					"alg_style");
_LIT(KFldExpected,					"expected");
_LIT(KFldFinish,					"finish");
_LIT(KFldFormatFieldNumber,			"%S%d");
_LIT(KFldFont,						"font");
_LIT(KFldFbsFont,					"fbsfont");
_LIT(KFldFontUid,					"font_uid");
_LIT(KFldMaxHeight,					"maxheight");
_LIT(KFldPixel,						"pixel");
_LIT(KFldRect,						"rect");
_LIT(KFldStart,						"start");
_LIT(KFldBitGc,						"bitgc");
_LIT(KFldRect1,						"rect1");
_LIT(KFldRect2,						"rect2");
_LIT(KFldFbsDevice,					"fbsdevice");
_LIT(KFldExpectedOrientation,		"expectedorientation");
_LIT(KFldPalette,					"palette");
_LIT(KFldGraphicsAccelerator,		"graphicsaccelerator");
_LIT(KFldExpectedDisplayMode,		"expecteddisplaymode");
_LIT(KFldOrigin,					"origin");
_LIT(KFldFactorX,					"factorx");
_LIT(KFldFactorY,					"factory");
_LIT(KFldDivisorX,					"divisorx");
_LIT(KFldDivisorY,					"divisory");
_LIT(KFldExpectedDrawRect,			"expecteddrawrect");
_LIT(KFldOffset,					"offset");
_LIT(KFldLinkedTypefaceSpec, 		"linkedtypefacespec");
_LIT(KFldExpectedUid,		 		"expecteduid");

//Logs
_LIT(KLogColor,						"Color Red=%d Green=%d Blue=%d Alpha=%d");
_LIT(KLogPoint,						"Point X=%d Y=%d");
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogMissingWrapper,			"Missing wrapper for '%S'");
_LIT(KLogNotExpectedValue,			"Not expected value");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataFbsDevice::CT_DataFbsDevice()
:	CT_DataBitmapDevice()
	{
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
TBool CT_DataFbsDevice::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	/* Another work package will implement this */
	TBool	retVal = ETrue;
	
	if (aCommand == KCmdcheckPixels)
		{
		DoCmdCheckPixels(aSection);
		}
	else if (aCommand == KCmdcheckLineColor)
		{
		DoCmdCheckLineColor(aSection);
		}
	else if (aCommand == KCmdcheckRectColor)
		{
		DoCmdCheckRectColor(aSection);
		}
	else if (aCommand == KCmdSizeInPixels)
		{
		DoCmdSizeInPixels(aSection);
		}
	else if (aCommand == KCmdGetFontById)
		{
		DoCmdGetFontByIdL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontInPixels)
		{
		if( !DoCmdGetNearestFontInPixelsL(aSection) )
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdGetNearestFontInTwips)
		{
		if( !DoCmdGetNearestFontInTwipsL(aSection) )
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdGetNearestFontToDesignHeightInPixels)
		{
		if( !DoCmdGetNearestFontToDesignHeightInPixelsL(aSection) )
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdGetNearestFontToDesignHeightInTwips)
		{
		if( !DoCmdGetNearestFontToDesignHeightInTwipsL(aSection) )
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdGetNearestFontToMaxHeightInPixels)
		{
		if( !DoCmdGetNearestFontToMaxHeightInPixelsL(aSection) )
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdGetNearestFontToMaxHeightInTwips)
		{
		if( !DoCmdGetNearestFontToMaxHeightInTwipsL(aSection) )
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdCreateContext)
		{
		if (!DoCmdCreateContextL(aSection))
			{
			retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdRectCompare)
		{
		DoCmdRectCompareL(aSection);
		}
	else if (aCommand == KCmdOrientation)
		{
		DoCmdOrientation(aSection);
		}
	else if (aCommand == KCmdSetCustomPalette)
		{
		DoCmdSetCustomPaletteL(aSection);
		}
	else if (aCommand == KCmdGraphicsAccelerator)
		{
		DoCmdGraphicsAccelerator();
		}
	else if (aCommand == KCmdDisplayMode16M)
		{
		DoCmdDisplayMode16M(aSection);
		}
	else if (aCommand == KCmdSetScalingFactor)
		{
		DoCmdSetScalingFactor(aSection);
		}
	else if (aCommand == KCmdGetDrawRect)
		{
		DoCmdGetDrawRect(aSection);
		}
	else if (aCommand == KCmdRegisterLinkedTypeface)
		{
		DoCmdRegisterLinkedTypefaceL(aSection);
		}
	else if (aCommand == KCmdSetDrawDeviceOffset)
		{
		DoCmdSetDrawDeviceOffset(aSection);
		}
	else
		{
		retVal = CT_DataBitmapDevice::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}
	return retVal;
	}


void CT_DataFbsDevice::DoCmdSizeInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute SizeInPixels()"));
	TSize actualsize;
	TInt expectheight = 0;
	TInt expectwidth = 0;
	TSize expectsize;
	
	if (!GetSizeFromConfig(aSection, KFldExpected, expectsize))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		actualsize = GetFbsDevice()->SizeInPixels();
		if (actualsize != expectsize)
			{
			ERR_PRINTF3(_L("The actual size is width=%d, height=%d"), actualsize.iWidth, actualsize.iHeight);
			ERR_PRINTF3(_L("The expect size is width=%d, height=%d"), expectsize.iWidth, expectsize.iHeight);
			SetBlockResult(EFail);
			}
		}
	}

/**
* DoCmdcheckPixels: to check the color of specified pixels on the screen device, and compare
* with the expected color specidied in the TEF testdata.
*		pixel<n>: specify the position of the screen device to check
*		expected<n>: specify the expected color to be compared with
*
*/
void CT_DataFbsDevice::DoCmdCheckPixels(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TBool moreData = ETrue;
	for (TInt index=1; moreData; ++index)
		{
		// get the pixel position on the screen to check/verify
		tempStore.Format(KFldFormatFieldNumber, &KFldPixel, index);
		TPoint	point;
		if ( !GetPointFromConfig(aSection, tempStore, point) )
			{
			moreData = EFalse;
			}
		else
			{
			TRgb actual;
			GetFbsDevice()->GetPixel(actual, point);

			tempStore.Format(KFldFormatFieldNumber, &KFldExpected, index);
			TRgb expected;
			if (GetRgbFromConfig(aSection, tempStore, expected))
				{
				if (actual != expected)
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
void CT_DataFbsDevice::DoCmdCheckLineColor(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	CPalette* palette = NULL;
	GetFbsDevice()->GetPalette(palette);

	TBool moreData = ETrue;
	for (TInt index=1; moreData; ++index)
		{
		// get the the start and finish points of the line to check/verify
		tempStore.Format(KFldFormatFieldNumber, &KFldStart, index);
		TPoint	startPoint;
		if (!GetPointFromConfig(aSection, tempStore, startPoint))
			{
			moreData = EFalse;
			}

		tempStore.Format(KFldFormatFieldNumber, &KFldFinish, index);
		TPoint endPoint;
		if (!GetPointFromConfig(aSection, tempStore, endPoint))
			{
			moreData = EFalse;
			}

		if (moreData)
			{
			// get the expected color from testdata
			tempStore.Format(KFldFormatFieldNumber, &KFldExpected, index);
			TRgb expected;
			if (!GetRgbFromConfig(aSection, tempStore, expected))
				{
				ERR_PRINTF2(KLogMissingParameter, &tempStore);
				SetBlockResult(EFail);
				}
			else
				{
				if (palette!=NULL)
					{
					expected = palette->NearestEntry(expected);
					}

				TLinearDDA lineDDA;
				lineDDA.Construct(startPoint, endPoint, TLinearDDA::ECenter);

				// for each pixels on the line (from start to finish points), check if its color matches expected
				TPoint point;
				while (!lineDDA.SingleStep(point))
					{
					TRgb actual;
					GetFbsDevice()->GetPixel(actual, point);

					if (actual != expected)
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
void CT_DataFbsDevice::DoCmdCheckRectColor(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength> tempStore;

	CPalette* palette=NULL;
	GetFbsDevice()->GetPalette(palette);

	TBool moreData = ETrue;
	for (TInt index=1; moreData; ++index)
		{
		// get the the rectangle to check/verify
		tempStore.Format(KFldFormatFieldNumber, &KFldRect, index);
		TRect rect;
		if (!GetRectFromConfig(aSection, tempStore, rect))
			{
			moreData = EFalse;
			}
		else
			{
			// get the expected color from testdata
			tempStore.Format(KFldFormatFieldNumber, &KFldExpected, index);
			TRgb expected;
			if (!GetRgbFromConfig(aSection, tempStore, expected))
				{
				ERR_PRINTF2(KLogMissingParameter, &tempStore);
				SetBlockResult(EFail);
				}
			else
				{
				if (palette!=NULL)
					{
					expected = palette->NearestEntry(expected);
					}

				TPoint point;
				for (point.iX=rect.iTl.iX; point.iX<rect.iBr.iX; ++point.iX)
					{
					for (point.iY=rect.iTl.iY; point.iY<rect.iBr.iY; ++point.iY)
						{
						TRgb actual;
						GetFbsDevice()->GetPixel(actual, point);

						if (actual!=expected)
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

void CT_DataFbsDevice::DoCmdGetFontByIdL(const TDesC& aSection)
	{
	TBool dataOk=ETrue;

	// get font uid
	TInt fontUid;
	if( !GetIntFromConfig(aSection, KFldFontUid(), fontUid) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFontUid());
		SetBlockResult(EFail);
		}
	
	// get algorithmic style
	TAlgStyle* algStyle = NULL;
	CT_GraphicsUtil::GetAlgStyleL(*this, aSection, KFldAlgStyle(), algStyle);
	if (NULL == algStyle)
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldAlgStyle());
		SetBlockResult(EFail);
		}
	
	TPtrC wrapperName;
	if( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )
		{
		// "CFbsFont*&" type
		if ( dataOk )
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute GetFontById(CFbsFont *&aFont, TUid aFileId, const TAlgStyle &aAlgStyle)"));
			TInt err = GetFbsDevice()->GetFontById(fbsFont, TUid::Uid(fontUid), *algStyle);
			
			if (KErrNone != err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		if( dataOk )
			{
			CFont* font = NULL;
			INFO_PRINTF1(_L("execute GetFontById(CFont *&aFont, TUid aFileId, const TAlgStyle &aAlgStyle)"));
			TInt err = GetFbsDevice()->GetFontById(font, TUid::Uid(fontUid), *algStyle);
			
			if (KErrNone != err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, font);
				}
			}
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	}

TBool CT_DataFbsDevice::DoCmdGetNearestFontInPixelsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// get font specification
	TFontSpec fontSpec;	
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TPtrC wrapperName;
	if ( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )
		{
		// "CFbsFont*&" type
		if(dataOk)
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute GetNearestFontInPixels(CFbsFont*&, TFontSpec)"));
			TInt err = GetFbsDevice()->GetNearestFontInPixels(fbsFont, fontSpec);
			if (KErrNone !=  err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		// hand over the ownership to parent class
		return EFalse;
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	return ETrue;
	}

TBool CT_DataFbsDevice::DoCmdGetNearestFontInTwipsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// get font specification
	TFontSpec fontSpec;	
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TPtrC wrapperName;
	if ( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )
		{
		// "CFbsFont*&" type
		if(dataOk)
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute GetNearestFontInTwips(CFbsFont*&, TFontSpec)"));
			TInt err = GetFbsDevice()->GetNearestFontInTwips(fbsFont, fontSpec);
			if (KErrNone !=  err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		// hand over the ownership to parent class
		return EFalse;
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	return ETrue;
	}

TBool CT_DataFbsDevice::DoCmdGetNearestFontToDesignHeightInPixelsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// get font specification
	TFontSpec fontSpec;	
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TPtrC wrapperName;
	if ( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )
		{
		// "CFbsFont*&" type
		if(dataOk)
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute DoCmdGetNearestFontToDesignHeightInPixels(CFbsFont*&, TFontSpec)"));
			TInt err = GetFbsDevice()->GetNearestFontToDesignHeightInPixels(fbsFont, fontSpec);
			if (KErrNone !=  err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		// hand over the ownership to parent class
		return EFalse;
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	return ETrue;
	}

TBool CT_DataFbsDevice::DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// get font specification
	TFontSpec fontSpec;	
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TPtrC wrapperName;
	if ( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )
		{
		// "CFbsFont*&" type
		if(dataOk)
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute DoCmdGetNearestFontToDesignHeightInTwips(CFbsFont*&, TFontSpec)"));
			TInt err = GetFbsDevice()->GetNearestFontToDesignHeightInTwips(fbsFont, fontSpec);
			if (KErrNone !=  err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		// hand over the ownership to parent class
		return EFalse;
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	return ETrue;
	}

TBool CT_DataFbsDevice::DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// get font specification
	TFontSpec fontSpec;	
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	// get maximum height in pixels
	TInt maxHeight;
	if ( !GetIntFromConfig(aSection, KFldMaxHeight, maxHeight) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldMaxHeight());
		SetBlockResult(EFail);
		}
	
	TPtrC wrapperName;
	if ( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )	
		{
		// "CFbsFont*&" type
		if(dataOk)
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute DoCmdGetNearestFontToMaxHeightInPixels(CFbsFont*&, TFontSpec)"));
			TInt err = GetFbsDevice()->GetNearestFontToMaxHeightInPixels(fbsFont, fontSpec, maxHeight);
			if (KErrNone !=  err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		// hand over the ownership to parent class
		return EFalse;
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	return ETrue;
	}

TBool CT_DataFbsDevice::DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// get font specification
	TFontSpec fontSpec;	
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	// get maximum height in twips
	TInt maxHeight;
	if ( !GetIntFromConfig(aSection, KFldMaxHeight, maxHeight) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldMaxHeight());
		SetBlockResult(EFail);
		}
	
	TPtrC wrapperName;
	if ( GetStringFromConfig(aSection, KFldFbsFont, wrapperName) )
		{
		// "CFbsFont*&" type
		if(dataOk)
			{
			CFbsFont* fbsFont = NULL;
			INFO_PRINTF1(_L("execute DoCmdGetNearestFontToMaxHeightInTwips(CFbsFont*&, TFontSpec)"));
			TInt err = GetFbsDevice()->GetNearestFontToMaxHeightInTwips(fbsFont, fontSpec, maxHeight);
			if (KErrNone !=  err)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				SetDataObjectL(wrapperName, fbsFont);
				}
			}
		}
	else if( GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		// "CFont*&" type
		// hand over the ownership to parent class
		return EFalse;
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	return ETrue;
	}

TBool CT_DataFbsDevice::DoCmdCreateContextL(const TDesC& aSection)
	{
	TPtrC wrapperName;

	if (!GetStringFromConfig(aSection, KFldBitGc, wrapperName))
		{
		return EFalse;
		}
	else
		{
		CDataWrapper* wrapper = GetDataWrapperL(wrapperName);
		if (wrapper == NULL)
			{
			ERR_PRINTF2(KLogMissingWrapper, &wrapperName);
			SetBlockResult(EFail);
			}
		else
			{
			CFbsBitGc* context = NULL;
			INFO_PRINTF1(_L("execute CreateContext(CFbsBitGc*&)"));
			TInt err = GetFbsDevice()->CreateContext(context);
			if (err != KErrNone)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				wrapper->SetObjectL(context);
				}
			}
		return ETrue;
		}
	}

void CT_DataFbsDevice::DoCmdRectCompareL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	// Get test data for command input parameter(s)
	TRect datRect1;
	if ( !GetRectFromConfig(aSection, KFldRect1, datRect1) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect1);
		SetBlockResult(EFail);
		}

	TRect datRect2;
	if ( !GetRectFromConfig(aSection, KFldRect2, datRect2) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect2);
		SetBlockResult(EFail);
		}

	TBool expected;
	if ( !GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	
	if (dataOk)
		{
		TBool actual;
		CFbsDevice* device = NULL;
		CFbsDevice* self = GetFbsDevice();
		if (!CT_GraphicsUtil::GetFbsDeviceL(*this, aSection, KFldFbsDevice, device) )
			{
			device = self;
			}
		actual = self->RectCompare(datRect1, *device, datRect2);
		INFO_PRINTF2(_L("execute RectCompare(const TRect&, const CFbsDevice&, const TRect&)=%d"), actual);

		if (actual != expected)
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsDevice::DoCmdSetCustomPaletteL(const TDesC& aSection)
	{
	CPalette* palette= NULL;
	
	if ( !CT_GraphicsUtil::GetPaletteL(*this, aSection, KFldPalette(), palette) )
		{
		INFO_PRINTF1(_L("execute SetCustomPalette(NULL)"));
		}
	else
		{
		INFO_PRINTF1(_L("execute SetCustomPalette(const CPalette*)"));
		}
	
	TInt err = GetFbsDevice()->SetCustomPalette(palette);
	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataFbsDevice::DoCmdGraphicsAccelerator()
	{
	CGraphicsAccelerator* ga= NULL;
	INFO_PRINTF1(_L("execute GraphicsAccelerator()"));
	ga = GetFbsDevice()->GraphicsAccelerator();
	if (ga != NULL)
		{
		INFO_PRINTF1(_L("GraphicsAccelerator Supported"));
		}
	else
		{
		INFO_PRINTF1(_L("GraphicsAccelerator Not Supported"));
		}
	}

void CT_DataFbsDevice::DoCmdOrientation(const TDesC& aSection)
	{
	CFbsBitGc::TGraphicsOrientation actual = GetFbsDevice()->Orientation();
	INFO_PRINTF2(_L("execute Orientation()=%d"), actual);

	CFbsBitGc::TGraphicsOrientation expected;
	if (!CT_GraphicsUtil::GetGraphicsOrientation(*this, aSection, KFldExpectedOrientation, expected))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpectedOrientation);
		SetBlockResult(EFail);
		}
	else
		{
		if (actual != expected)
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsDevice::DoCmdDisplayMode16M(const TDesC& aSection)
	{
	TDisplayMode actual = GetFbsDevice()->DisplayMode16M();
	INFO_PRINTF2(_L("execute DisplayMode16M()=%d"), actual);

	// get display mode from parameters
	TDisplayMode expected;
	if (CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldExpectedDisplayMode(), expected) )
		{
		if (actual != expected)
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsDevice::DoCmdSetScalingFactor(const TDesC& aSection)
	{
	TBool dataOk = ETrue;
	TPoint point;
	
	if ( !GetPointFromConfig(aSection, KFldOrigin, point) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldOrigin);
		SetBlockResult(EFail);
		}
	
	TInt factorX;
	if ( !GetIntFromConfig(aSection, KFldFactorX, factorX) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFactorX);
		SetBlockResult(EFail);
		}

	TInt factorY;
	if ( !GetIntFromConfig(aSection, KFldFactorY, factorY) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFactorY);
		SetBlockResult(EFail);
		}

	TInt divisorX;
	if ( !GetIntFromConfig(aSection, KFldDivisorX, divisorX) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDivisorX);
		SetBlockResult(EFail);
		}

	TInt divisorY;
	if ( !GetIntFromConfig(aSection, KFldDivisorY, divisorY) )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDivisorY);
		SetBlockResult(EFail);
		}
	
	if (dataOk)
		{
		INFO_PRINTF1(_L("execute SetScalingFactor(const TPoint&, TInt, TInt, TInt, TInt)"));
		TInt err = GetFbsDevice()->SetScalingFactor(point, factorX, factorY, divisorX, divisorY);
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataFbsDevice::DoCmdGetDrawRect(const TDesC& aSection)
	{
	TRect actual;
	
	INFO_PRINTF1(_L("execute GetDrawRect(TRect&)"));
	GetFbsDevice()->GetDrawRect(actual);
	INFO_PRINTF5(_L("DrawRect = (top.x=%d, top.y=%d, bottom.x=%d, bottom.y=%d)"), actual.iTl.iX, actual.iTl.iY, actual.iBr.iX, actual.iBr.iY);
	
	TRect expected;
	if ( GetRectFromConfig(aSection, KFldExpectedDrawRect, expected) )
		{
		if (actual != expected)
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsDevice::DoCmdRegisterLinkedTypefaceL(const TDesC& aSection)
	{
	CLinkedTypefaceSpecification* spec = NULL;
	if (!CT_GraphicsUtil::GetLinkedTypefaceSpecificationL(*this, aSection, KFldLinkedTypefaceSpec, spec))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldLinkedTypefaceSpec);
		SetBlockResult(EFail);
		}
	else
		{
		TInt actual;
		INFO_PRINTF1(_L("execute RegisterLinkedTypeface(const CLinkedTypefaceSpecification&, TInt&)"));
		TInt err = GetFbsDevice()->RegisterLinkedTypeface(*spec, actual);
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			return;
			}

		TInt expected;
		if (GetIntFromConfig(aSection, KFldExpectedUid, expected))
			{
			if (actual != expected)
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataFbsDevice::DoCmdSetDrawDeviceOffset(const TDesC& aSection)
	{
	TPoint point;

	if ( !GetPointFromConfig(aSection, KFldOffset, point) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldOrigin);
		SetBlockResult(EFail);
		}
	else
		{
		TInt err = GetFbsDevice()->SetDrawDeviceOffset(point);
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}
