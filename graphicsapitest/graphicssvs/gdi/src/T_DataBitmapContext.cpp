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

This contains CT_DataGraphicsContext
*/

#include "T_DataBitmapContext.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdAlphaBlendBitmaps,							"AlphaBlendBitmaps");
_LIT(KCmdBitBlt,									"BitBlt");
_LIT(KCmdBitBltMasked,								"BitBltMasked");
_LIT(KCmdClear,										"Clear");
_LIT(KCmdCopyRect,									"CopyRect");
_LIT(KCmdSetFaded,									"SetFaded");
_LIT(KCmdSetFadingParameters,						"SetFadingParameters");

///	Fields
_LIT(KFldAlphaBmp,									"alphabmp");
_LIT(KFldAlphaPt,									"alphapt");
_LIT(KFldBitmap,									"bitmap");
_LIT(KFldBlackMap,									"blackmap");
_LIT(KFldDestination,								"destination");
_LIT(KFldDestPt,									"destpt");
_LIT(KFldFaded,										"faded");
_LIT(KFldInvertMask,								"invertmask");
_LIT(KFldMaskBitmap,								"maskbitmap");
_LIT(KFldOffset,									"offset");
_LIT(KFldPoint,										"point");
_LIT(KFldRect,										"rect");
_LIT(KFldSource,									"source");
_LIT(KFldSourceRect,								"sourcerect");
_LIT(KFldSrcBmp,									"srcbmp");
_LIT(KFldSrcRect,									"srcrect");
_LIT(KFldSrcWsBmp,									"srcwsbmp");
_LIT(KFldWhiteMap,									"whitemap");

///	Logging
_LIT(KLogError,										"Error=%d");
_LIT(KLogMissingParameter,							"Missing parameter '%S'");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataBitmapContext::CT_DataBitmapContext()
:	CT_DataGraphicsContext()
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
TBool CT_DataBitmapContext::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret=ETrue;
	if ( aCommand==KCmdAlphaBlendBitmaps )
		{
		DoCmdAlphaBlendBitmapsL(aSection);
		}
	else if ( aCommand==KCmdBitBlt )
		{
		DoCmdBitBltL(aSection);
		}
	else if ( aCommand==KCmdBitBltMasked )
		{
		DoCmdBitBltMaskedL(aSection);
		}
	else if ( aCommand==KCmdClear )
		{
		DoCmdClear(aSection);
		}
	else if ( aCommand==KCmdCopyRect )
		{
		DoCmdCopyRect(aSection);
		}
	else if ( aCommand==KCmdSetFaded )
		{
		DoCmdSetFaded(aSection);
		}
	else if ( aCommand==KCmdSetFadingParameters )
		{
		DoCmdSetFadingParameters(aSection);
		}
	else
		{
		ret=CT_DataGraphicsContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataBitmapContext::DoCmdAlphaBlendBitmapsL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TPoint	destinationPoint;
	if ( !GetPointFromConfig(aSection, KFldDestPt, destinationPoint) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDestPt());
		SetBlockResult(EFail);
		}

	TRect	sourceRect;
	if ( !GetRectFromConfig(aSection, KFldSrcRect, sourceRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSrcRect());
		SetBlockResult(EFail);
		}

	TPoint	alphaPoint;
	if ( !GetPointFromConfig(aSection, KFldAlphaPt, alphaPoint) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldAlphaPt());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TInt		err=KErrNone;

		CWsBitmap*	wsBitmap=NULL;
		if ( CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldSrcWsBmp, wsBitmap) )
			{
			CWsBitmap*	alphaBitmap=NULL;
			if ( !CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldAlphaBmp, alphaBitmap) )
				{
				ERR_PRINTF2(KLogMissingParameter, &KFldAlphaBmp());
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("execute AlphaBlendBitmaps(TPoint, CWsBitmap*, TRect, CWsBitmap*, TPoint)"));
				err=GetBitmapContext()->AlphaBlendBitmaps(destinationPoint, wsBitmap, sourceRect, alphaBitmap, alphaPoint);
				}
			}
		else
			{
			CFbsBitmap*	fbsBitmap=NULL;
			if ( CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldSrcBmp, fbsBitmap) )
				{
				CFbsBitmap*	alphaBitmap=NULL;
				if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldAlphaBmp, alphaBitmap) )
					{
					ERR_PRINTF2(KLogMissingParameter, &KFldAlphaBmp());
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("execute AlphaBlendBitmaps(TPoint, CFbsBitmap*, TRect, CFbsBitmap*, TPoint)"));
					err=GetBitmapContext()->AlphaBlendBitmaps(destinationPoint, fbsBitmap, sourceRect, alphaBitmap, alphaPoint);
					}
				}
			else
				{
				ERR_PRINTF2(KLogMissingParameter, &KFldSrcWsBmp());
				ERR_PRINTF2(KLogMissingParameter, &KFldSrcBmp());
				SetBlockResult(EFail);
				}
			}
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataBitmapContext::DoCmdBitBltL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	CFbsBitmap*	bitmap=NULL;
	if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldBitmap());
		SetBlockResult(EFail);
		}

	TPoint		point;
	if ( GetPointFromConfig(aSection, KFldPoint, point) )
		{
		if ( dataOk )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute BitBlt(TPoint, CFbsBitmap*)"));
			GetBitmapContext()->BitBlt(point, bitmap);
			}
		}
	else
		{
		TPoint	destination;
		if ( !GetPointFromConfig(aSection, KFldDestination, destination) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldDestination());
			SetBlockResult(EFail);
			}

		TRect	source;
		if ( !GetRectFromConfig(aSection, KFldSource, source) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldSource());
			SetBlockResult(EFail);
			}

		if ( dataOk )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute BitBlt(TRect, CFbsBitmap*, TRect)"));
			GetBitmapContext()->BitBlt(destination, bitmap, source);
			}
		}
	}

void CT_DataBitmapContext::DoCmdBitBltMaskedL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPoint	point;
	if ( !GetPointFromConfig(aSection, KFldPoint, point) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}

	TRect	sourceRect;
	if ( !GetRectFromConfig(aSection, KFldSourceRect, sourceRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSourceRect());
		SetBlockResult(EFail);
		}

	TBool	invertMask;
	if ( !GetBoolFromConfig(aSection, KFldInvertMask, invertMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldInvertMask());
		SetBlockResult(EFail);
		}

	CFbsBitmap*	bitmap=NULL;
	if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldBitmap());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		CFbsBitmap*	maskBitmap=NULL;
		CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldMaskBitmap, maskBitmap);

		// Execute command and log parameters
		INFO_PRINTF1(_L("execute BitBltMasked(TPoint, CFbsBitmap*, TRect, CFbsBitmap*, TBool)"));
		GetBitmapContext()->BitBltMasked(point, bitmap, sourceRect, maskBitmap, invertMask);
		}
	}

void CT_DataBitmapContext::DoCmdClear(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	rect;
	if ( GetRectFromConfig(aSection, KFldRect, rect) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Clear(TRect)"));
		GetBitmapContext()->Clear(rect);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Clear()"));
		GetBitmapContext()->Clear();
		}
	}

void CT_DataBitmapContext::DoCmdCopyRect(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPoint	offset;
	if ( !GetPointFromConfig(aSection, KFldOffset, offset) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldOffset());
		SetBlockResult(EFail);
		}

	TRect	rect;
	if ( !GetRectFromConfig(aSection, KFldRect, rect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute CopyRect(TPoint, TRect)"));
		GetBitmapContext()->CopyRect(offset, rect);
		}
	}

void CT_DataBitmapContext::DoCmdSetFaded(const TDesC& aSection)
	{
	TBool	faded=ETrue;
	if(	!GetBoolFromConfig(aSection, KFldFaded(), faded))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFaded());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetFaded(TBool)"));
		GetBitmapContext()->SetFaded(faded);
		}
	}

void CT_DataBitmapContext::DoCmdSetFadingParameters(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TInt	blackMap=0;
	if(	!GetIntFromConfig(aSection, KFldBlackMap(), blackMap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldBlackMap());
		SetBlockResult(EFail);
		}

	TInt	whiteMap=255;
	if(	!GetIntFromConfig(aSection, KFldWhiteMap(), whiteMap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldWhiteMap());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetFadingParameters(TUint8, TUint8)"));
		GetBitmapContext()->SetFadingParameters((TUint8)blackMap, (TUint8)whiteMap);
		}
	}
