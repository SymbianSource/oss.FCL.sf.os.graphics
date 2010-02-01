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

This contains CT_DataGraphicsDeviceMap
*/

#include "T_DataGraphicsDeviceMap.h"
#include "T_DataFont.h"

/*@{*/
///	Commands
_LIT(KCmdGetNearestFontInTwips,						"GetNearestFontInTwips");
_LIT(KCmdGetNearestFontToDesignHeightInTwips,		"GetNearestFontToDesignHeightInTwips");
_LIT(KCmdGetNearestFontToMaxHeightInTwips,			"GetNearestFontToMaxHeightInTwips");
_LIT(KCmdHorizontalPixelsToTwips,					"HorizontalPixelsToTwips");
_LIT(KCmdHorizontalTwipsToPixels,					"HorizontalTwipsToPixels");
_LIT(KCmdPixelsToTwips,								"PixelsToTwips");
_LIT(KCmdReleaseFont,								"ReleaseFont");
_LIT(KCmdTwipsToPixels,								"TwipsToPixels");
_LIT(KCmdVerticalPixelsToTwips,						"VerticalPixelsToTwips");
_LIT(KCmdVerticalTwipsToPixels,						"VerticalTwipsToPixels");

///	Fields
_LIT(KFldExpected,									"expected");
_LIT(KFldFont,										"font");
_LIT(KFldMaxHeight,									"maxheight");
_LIT(KFldPixels,									"pixels");
_LIT(KFldPoint,										"point");
_LIT(KFldRect,										"rect");
_LIT(KFldTwips,										"twips");
_LIT(KFldDisown,									"disown");

///	Logging
_LIT(KLogError,										"Error=%d");
_LIT(KLogMissingParameter,							"Missing parameter '%S'");
_LIT(KLogMissingWrapper,							"Missing wrapper for '%S'");
_LIT(KLogNotExpectedValue,							"Not expected value");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataGraphicsDeviceMap::CT_DataGraphicsDeviceMap()
:	CDataWrapperBase()
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
TBool CT_DataGraphicsDeviceMap::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret=ETrue;
	if ( aCommand==KCmdGetNearestFontInTwips )
		{
		DoCmdGetNearestFontInTwipsL(aSection);
		}
	else if ( aCommand==KCmdGetNearestFontToDesignHeightInTwips )
		{
		DoCmdGetNearestFontToDesignHeightInTwipsL(aSection);
		}
	else if ( aCommand==KCmdGetNearestFontToMaxHeightInTwips )
		{
		DoCmdGetNearestFontToMaxHeightInTwipsL(aSection);
		}
	else if ( aCommand==KCmdHorizontalPixelsToTwips )
		{
		DoCmdHorizontalPixelsToTwips(aSection);
		}
	else if ( aCommand==KCmdHorizontalTwipsToPixels )
		{
		DoCmdHorizontalTwipsToPixels(aSection);
		}
	else if ( aCommand==KCmdPixelsToTwips )
		{
		DoCmdPixelsToTwips(aSection);
		}
	else if ( aCommand==KCmdReleaseFont )
		{
		DoCmdReleaseFontL(aSection);
		}
	else if ( aCommand==KCmdTwipsToPixels )
		{
		DoCmdTwipsToPixels(aSection);
		}
	else if ( aCommand==KCmdVerticalPixelsToTwips )
		{
		DoCmdVerticalPixelsToTwips(aSection);
		}
	else if ( aCommand==KCmdVerticalTwipsToPixels )
		{
		DoCmdVerticalTwipsToPixels(aSection);
		}
	else
		{
		ret=EFalse;
		}
	return ret;
	}

void CT_DataGraphicsDeviceMap::DoCmdGetNearestFontInTwipsL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TPtrC		wrapperName;
	// get BitmapUtilData object from parameters
	if ( !GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		CDataWrapper*	wrapper=GetDataWrapperL(wrapperName);
		if ( wrapper==NULL )
			{
			ERR_PRINTF2(KLogMissingWrapper, &wrapperName);
			SetBlockResult(EFail);
			}
		else
			{
			CFont*	font=NULL;
			INFO_PRINTF1(_L("execute GetNearestFontInTwips(CFont*&, TFontSpec)"));
			TInt	err=GetGraphicsDeviceMap()->GetNearestFontInTwips(font, fontSpec);
			if ( err!=KErrNone )
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				wrapper->SetObjectL(font);
				}
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TPtrC		wrapperName;
	// get BitmapUtilData object from parameters
	if ( !GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		CDataWrapper*	wrapper=GetDataWrapperL(wrapperName);
		if ( wrapper==NULL )
			{
			ERR_PRINTF2(KLogMissingWrapper, &wrapperName);
			SetBlockResult(EFail);
			}
		else
			{
			CFont*	font=NULL;
			INFO_PRINTF1(_L("execute GetNearestFontToDesignHeightInTwips(CFont*&, TFontSpec)"));
			TInt	err=GetGraphicsDeviceMap()->GetNearestFontToDesignHeightInTwips(font, fontSpec);
			if ( err!=KErrNone )
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				wrapper->SetObjectL(font);
				}
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFldFont, fontSpec) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TInt	maxHeight;
	if ( !GetIntFromConfig(aSection, KFldMaxHeight, maxHeight) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldMaxHeight());
		SetBlockResult(EFail);
		}

	TPtrC		wrapperName;
	// get BitmapUtilData object from parameters
	if ( !GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		CDataWrapper*	wrapper=GetDataWrapperL(wrapperName);
		if ( wrapper==NULL )
			{
			ERR_PRINTF2(KLogMissingWrapper, &wrapperName);
			SetBlockResult(EFail);
			}
		else
			{
			CFont*	font=NULL;
			INFO_PRINTF1(_L("execute GetNearestFontToMaxHeightInTwips(CFont*&, TFontSpec, TInt)"));
			TInt	err=GetGraphicsDeviceMap()->GetNearestFontToMaxHeightInTwips(font, fontSpec, maxHeight);
			if ( err!=KErrNone )
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				wrapper->SetObjectL(font);
				}
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdHorizontalPixelsToTwips(const TDesC& aSection)
	{
	TInt	value;
	if ( !GetIntFromConfig(aSection, KFldPixels, value) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPixels());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute HorizontalPixelsToTwips(TInt)"));
		TInt	pixels=GetGraphicsDeviceMap()->HorizontalPixelsToTwips(value);
		INFO_PRINTF2(_L("HorizontalPixelsToTwips=%d"), pixels);

		if ( GetIntFromConfig(aSection, KFldExpected, value) )
			{
			if ( value!=pixels )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdHorizontalTwipsToPixels(const TDesC& aSection)
	{
	TInt	value;
	if ( !GetIntFromConfig(aSection, KFldTwips, value) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldTwips());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute HorizontalTwipsToPixels(TInt)"));
		TInt	pixels=GetGraphicsDeviceMap()->HorizontalTwipsToPixels(value);
		INFO_PRINTF2(_L("HorizontalTwipsToPixels=%d"), pixels);

		if ( GetIntFromConfig(aSection, KFldExpected, value) )
			{
			if ( value!=pixels )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdPixelsToTwips(const TDesC& aSection)
	{
	TPoint	point;
	if ( GetPointFromConfig(aSection, KFldPoint, point) )
		{
		INFO_PRINTF1(_L("execute PixelsToTwips(const TPoint&)"));
		iPixelsToTwipsPoint=GetGraphicsDeviceMap()->PixelsToTwips(point);
		INFO_PRINTF3(_L("PixelsToTwips x=%d y=%d"), iPixelsToTwipsPoint.iX, iPixelsToTwipsPoint.iY);

		if ( GetPointFromConfig(aSection, KFldExpected, point) )
			{
			if ( point!=iPixelsToTwipsPoint )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	else
		{
		TRect	rect;
		if ( GetRectFromConfig(aSection, KFldRect, rect) )
			{
			INFO_PRINTF1(_L("execute PixelsToTwips(const TRect&)"));
			iPixelsToTwipsRect=GetGraphicsDeviceMap()->PixelsToTwips(rect);
			LogRect(_L("PixelsToTwips(TRect)"), iPixelsToTwipsRect);
			if ( GetRectFromConfig(aSection, KFldExpected, rect) )
				{
				if ( rect!=iPixelsToTwipsRect )
					{
					ERR_PRINTF1(KLogNotExpectedValue);
					SetBlockResult(EFail);
					}
				}
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
			ERR_PRINTF2(KLogMissingParameter, &KFldRect());
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdReleaseFontL(const TDesC& aSection)
	{
	TPtrC		wrapperName;
	// get BitmapUtilData object from parameters
	if ( !GetStringFromConfig(aSection, KFldFont, wrapperName) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	else
		{
		CT_DataFont*	wrapper=static_cast<CT_DataFont*>(GetDataWrapperL(wrapperName));
		if ( wrapper==NULL )
			{
			ERR_PRINTF2(KLogMissingWrapper, &wrapperName);
			SetBlockResult(EFail);
			}
		else
			{
			CFont*	font=wrapper->GetFont();
			INFO_PRINTF1(_L("execute ReleaseFont(CFont*)"));
			GetGraphicsDeviceMap()->ReleaseFont(font);
			
			// check if we need to disown object
			TBool	disOwn=ETrue, Own;
			if ( GetBoolFromConfig(aSection, KFldDisown, Own))
				disOwn=Own;
			if (disOwn)
				wrapper->DisownObjectL();
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdTwipsToPixels(const TDesC& aSection)
	{
	TPoint	point;
	if ( GetPointFromConfig(aSection, KFldPoint, point) )
		{
		INFO_PRINTF1(_L("execute TwipsToPixels(const TPoint&)"));
		iTwipsToPixelsPoint=GetGraphicsDeviceMap()->TwipsToPixels(point);
		INFO_PRINTF3(_L("TwipsToPixels x=%d y=%d"), iTwipsToPixelsPoint.iX, iTwipsToPixelsPoint.iY);

		if ( GetPointFromConfig(aSection, KFldExpected, point) )
			{
			if ( point!=iTwipsToPixelsPoint )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	else
		{
		TRect	rect;
		if ( GetRectFromConfig(aSection, KFldRect, rect) )
			{
			INFO_PRINTF1(_L("execute TwipsToPixels(const TRect&)"));
			iTwipsToPixelsRect=GetGraphicsDeviceMap()->TwipsToPixels(rect);
			LogRect(_L("TwipsToPixels(const TRect&)"), iTwipsToPixelsRect);
			if ( GetRectFromConfig(aSection, KFldExpected, rect) )
				{
				if ( rect!=iTwipsToPixelsRect )
					{
					ERR_PRINTF1(KLogNotExpectedValue);
					SetBlockResult(EFail);
					}
				}
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
			ERR_PRINTF2(KLogMissingParameter, &KFldRect());
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdVerticalPixelsToTwips(const TDesC& aSection)
	{
	TInt	value;
	if ( !GetIntFromConfig(aSection, KFldPixels, value) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPixels());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute VerticalPixelsToTwips(TInt)"));
		TInt	pixels=GetGraphicsDeviceMap()->VerticalPixelsToTwips(value);
		INFO_PRINTF2(_L("VerticalPixelsToTwips=%d"), pixels);

		if ( GetIntFromConfig(aSection, KFldExpected, value) )
			{
			if ( value!=pixels )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsDeviceMap::DoCmdVerticalTwipsToPixels(const TDesC& aSection)
	{
	TInt	value;
	if ( !GetIntFromConfig(aSection, KFldTwips, value) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldTwips());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute VerticalTwipsToPixels(TInt)"));
		TInt	pixels=GetGraphicsDeviceMap()->VerticalTwipsToPixels(value);
		INFO_PRINTF2(_L("VerticalTwipsToPixels=%d"), pixels);

		if ( GetIntFromConfig(aSection, KFldExpected, value) )
			{
			if ( value!=pixels )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}
