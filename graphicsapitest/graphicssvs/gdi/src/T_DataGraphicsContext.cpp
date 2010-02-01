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

//	User includes
#include "T_DataGraphicsContext.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdCancelClippingRect,						"CancelClippingRect");
_LIT(KCmdCancelClippingRegion,						"CancelClippingRegion");
_LIT(KCmdDevice,									"Device");
_LIT(KCmdDiscardBrushPattern,						"DiscardBrushPattern");
_LIT(KCmdDiscardFont,								"DiscardFont");
_LIT(KCmdDrawArc,									"DrawArc");
_LIT(KCmdDrawBitmap,								"DrawBitmap");
_LIT(KCmdDrawBitmapMasked,							"DrawBitmapMasked");
_LIT(KCmdDrawEllipse,								"DrawEllipse");
_LIT(KCmdDrawLine,									"DrawLine");
_LIT(KCmdDrawLineBy,								"DrawLineBy");
_LIT(KCmdDrawLineTo,								"DrawLineTo");
_LIT(KCmdDrawPie,									"DrawPie");
_LIT(KCmdDrawPolygon,								"DrawPolygon");
_LIT(KCmdDrawPolyLine,								"DrawPolyLine");
_LIT(KCmdDrawRect,									"DrawRect");
_LIT(KCmdDrawRoundRect,								"DrawRoundRect");
_LIT(KCmdDrawText,									"DrawText");
_LIT(KCmdDrawTextExtended,							"DrawTextExtended");
_LIT(KCmdDrawTextVertical,							"DrawTextVertical");
_LIT(KCmdGetShadowColor,							"GetShadowColor");
_LIT(KCmdGetUnderlineMetrics,						"GetUnderlineMetrics");
_LIT(KCmdJustificationInPixels,						"JustificationInPixels");
_LIT(KCmdMapColors,									"MapColors");
_LIT(KCmdMoveBy,									"MoveBy");
_LIT(KCmdMoveTo,									"MoveTo");
_LIT(KCmdPlot,										"Plot");
_LIT(KCmdReserved,									"Reserved");
_LIT(KCmdReset,										"Reset");
_LIT(KCmdSetBrushColor,								"SetBrushColor");
_LIT(KCmdSetBrushOrigin,							"SetBrushOrigin");
_LIT(KCmdSetBrushStyle,								"SetBrushStyle");
_LIT(KCmdSetClippingRegion,							"SetClippingRegion");
_LIT(KCmdSetCharJustification,						"SetCharJustification");
_LIT(KCmdSetClippingRect,							"SetClippingRect");
_LIT(KCmdSetDrawMode,								"SetDrawMode");
_LIT(KCmdSetOrigin,									"SetOrigin");
_LIT(KCmdSetPenColor,								"SetPenColor");
_LIT(KCmdSetPenSize,								"SetPenSize");
_LIT(KCmdSetPenStyle,								"SetPenStyle");
_LIT(KCmdSetShadowColor,							"SetShadowColor");
_LIT(KCmdSetStrikethroughStyle,						"SetStrikethroughStyle");
_LIT(KCmdSetUnderlineStyle,							"SetUnderlineStyle");
_LIT(KCmdSetWordJustification,						"SetWordJustification");
_LIT(KCmdUseBrushPattern,							"UseBrushPattern");
_LIT(KCmdUseFont,									"UseFont");

///	Parameters
_LIT(KFldBaselineOffset,							"baselineoffset");
_LIT(KFldBitmap,									"bitmap");
_LIT(KFldBottom,									"bottom");
_LIT(KFldBrushStyle,								"brushstyle");
_LIT(KFldColor,										"color");
_LIT(KFldDestRect,									"destrect");
_LIT(KFldDevice,									"device");
_LIT(KFldDrawMode,									"drawmode");
_LIT(KFldExcessPixels,								"excessPixels");
_LIT(KFldExcessWidth,								"excesswidth");
_LIT(KFldExpected,									"expected");
_LIT(KFldFillRule,									"fillrule");
_LIT(KFldFirstUnit,									"firstUnit");
_LIT(KFldFont,										"font");
_LIT(KFldFontStrikethrough,							"fontstrikethrough");
_LIT(KFldFontUnderline,								"fontunderline");
_LIT(KFldEnd,										"end");
_LIT(KFldInvertMask,								"invertmask");
_LIT(KFldLeftMargin,								"leftmargin");
_LIT(KFldMapForwards,								"mapforwards");
_LIT(KFldMaskBitmap,								"maskbitmap");
_LIT(KFldNumChars,									"numchars");
_LIT(KFldNumColors,									"numcolors");
_LIT(KFldNumGaps,									"numgaps");
_LIT(KFldNumPoints,									"numpoints");
_LIT(KFldNumUnits,									"numunits");
_LIT(KFldPenStyle,									"penstyle");
_LIT(KFldPoint,										"point");
_LIT(KFldPoint1,									"point1");
_LIT(KFldPoint2,									"point2");
_LIT(KFldPointList,									"pointlist");
_LIT(KFldRect,										"rect");
_LIT(KFldRegion,									"region");
_LIT(KFldRgb,										"rgb");
_LIT(KFldRgbList,									"rgblist");
_LIT(KFldSourceRect,								"sourcerect");
_LIT(KFldSize,										"size");
_LIT(KFldStart,										"start");
_LIT(KFldText,										"text");
_LIT(KFldTextAlign,									"textalign");
_LIT(KFldTextExtendedParam,							"textextendedparam");
_LIT(KFldTop,										"top");
_LIT(KFldTopLeft,									"topleft");
_LIT(KFldTotalUnits,								"totalUnits");
_LIT(KFldUp,										"up");
_LIT(KFldWsBitmap,									"wsbitmap");

///	Logging
_LIT(KLogColor,										"Color Red=%d Green=%d Blue=%d Alpha=%d");
_LIT(KLogError,										"Error=%d");
_LIT(KLogMetrics,									"Top=%d Bottom=%d");
_LIT(KLogMissingParameter,							"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,							"Not expected value");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataGraphicsContext::CT_DataGraphicsContext()
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
TBool CT_DataGraphicsContext::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret=ETrue;
	if ( aCommand==KCmdCancelClippingRect )
		{
		DoCmdCancelClippingRect();
		}
	else if ( aCommand==KCmdCancelClippingRegion )
		{
		DoCmdCancelClippingRegion();
		}
	else if ( aCommand==KCmdDevice )
		{
		DoCmdDeviceL(aSection);
		}
	else if ( aCommand==KCmdDiscardBrushPattern )
		{
		DoCmdDiscardBrushPattern();
		}
	else if ( aCommand==KCmdDiscardFont )
		{
		DoCmdDiscardFont();
		}
	else if ( aCommand==KCmdDrawArc )
		{
		DoCmdDrawArc(aSection);
		}
	else if ( aCommand==KCmdDrawBitmap )
		{
		DoCmdDrawBitmapL(aSection);
		}
	else if ( aCommand==KCmdDrawBitmapMasked )
		{
		DoCmdDrawBitmapMaskedL(aSection);
		}
	else if ( aCommand==KCmdDrawEllipse )
		{
		DoCmdDrawEllipse(aSection);
		}
	else if ( aCommand==KCmdDrawLine )
		{
		DoCmdDrawLine(aSection);
		}
	else if ( aCommand==KCmdDrawLineBy )
		{
		DoCmdDrawLineBy(aSection);
		}
	else if ( aCommand==KCmdDrawLineTo )
		{
		DoCmdDrawLineTo(aSection);
		}
	else if ( aCommand==KCmdDrawPie )
		{
		DoCmdDrawPie(aSection);
		}
	else if ( aCommand==KCmdDrawPolygon )
		{
		DoCmdDrawPolygonL(aSection);
		}
	else if ( aCommand==KCmdDrawPolyLine )
		{
		DoCmdDrawPolyLineL(aSection);
		}
	else if ( aCommand==KCmdDrawRect )
		{
		DoCmdDrawRect(aSection);
		}
	else if ( aCommand==KCmdDrawRoundRect )
		{
		DoCmdDrawRoundRect(aSection);
		}
	else if ( aCommand==KCmdDrawText )
		{
		DoCmdDrawText(aSection);
		}
	else if ( aCommand==KCmdDrawTextExtended )
		{
		DoCmdDrawTextExtendedL(aSection);
		}
	else if ( aCommand==KCmdDrawTextVertical )
		{
		DoCmdDrawTextVertical(aSection);
		}
	else if ( aCommand==KCmdGetShadowColor )
		{
		DoCmdGetShadowColor(aSection);
		}
	else if ( aCommand==KCmdGetUnderlineMetrics )
		{
		DoCmdGetUnderlineMetrics(aSection);
		}
	else if ( aCommand==KCmdJustificationInPixels )
		{
		DoCmdJustificationInPixels(aSection);
		}
	else if ( aCommand==KCmdMapColors )
		{
		DoCmdMapColorsL(aSection);
		}
	else if ( aCommand==KCmdMoveBy )
		{
		DoCmdMoveBy(aSection);
		}
	else if ( aCommand==KCmdMoveTo )
		{
		DoCmdMoveTo(aSection);
		}
	else if ( aCommand==KCmdPlot )
		{
		DoCmdPlot(aSection);
		}
	else if ( aCommand==KCmdReserved )
		{
		DoCmdReserved();
		}
	else if ( aCommand==KCmdReset )
		{
		DoCmdReset();
		}
	else if ( aCommand==KCmdSetBrushColor )
		{
		DoCmdSetBrushColor(aSection);
		}
	else if ( aCommand==KCmdSetBrushOrigin )
		{
		DoCmdSetBrushOrigin(aSection);
		}
	else if ( aCommand==KCmdSetBrushStyle )
		{
		DoCmdSetBrushStyle(aSection);
		}
	else if ( aCommand==KCmdSetClippingRegion )
		{
		DoCmdSetClippingRegion(aSection);
		}
	else if ( aCommand==KCmdSetCharJustification )
		{
		DoCmdSetCharJustification(aSection);
		}
	else if ( aCommand==KCmdSetClippingRect )
		{
		DoCmdSetClippingRect(aSection);
		}
	else if ( aCommand==KCmdSetDrawMode )
		{
		DoCmdSetDrawMode(aSection);
		}
	else if ( aCommand==KCmdSetOrigin )
		{
		DoCmdSetOrigin(aSection);
		}
	else if ( aCommand==KCmdSetPenColor )
		{
		DoCmdSetPenColor(aSection);
		}
	else if ( aCommand==KCmdSetPenSize )
		{
		DoCmdSetPenSize(aSection);
		}
	else if ( aCommand==KCmdSetPenStyle )
		{
		DoCmdSetPenStyle(aSection);
		}
	else if ( aCommand==KCmdSetShadowColor )
		{
		DoCmdSetShadowColor(aSection);
		}
	else if ( aCommand==KCmdSetStrikethroughStyle )
		{
		DoCmdSetStrikethroughStyle(aSection);
		}
	else if ( aCommand==KCmdSetUnderlineStyle )
		{
		DoCmdSetUnderlineStyle(aSection);
		}
	else if ( aCommand==KCmdSetWordJustification )
		{
		DoCmdSetWordJustification(aSection);
		}
	else if ( aCommand==KCmdUseBrushPattern )
		{
		DoCmdUseBrushPatternL(aSection);
		}
	else if ( aCommand==KCmdUseFont )
		{
		DoCmdUseFontL(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CT_DataGraphicsContext::DoCmdCancelClippingRect()
	{
	INFO_PRINTF1(_L("execute CancelClippingRect()"));
	GetGraphicsContext()->CancelClippingRect();
	}

void CT_DataGraphicsContext::DoCmdCancelClippingRegion()
	{
	INFO_PRINTF1(_L("execute CancelClippingRegion()"));
	GetGraphicsContext()->CancelClippingRegion();
	}

void CT_DataGraphicsContext::DoCmdDeviceL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute Device()"));
	CGraphicsDevice*	device = GetGraphicsContext()->Device();
	TPtrC				deviceName;
	if ( GetStringFromConfig(aSection, KFldDevice(), deviceName) )
		{
		// check that the data object was found
		CDataWrapper*	dataWrapper=GetDataWrapperL(deviceName);
		dataWrapper->SetObjectL(device);
		}
	}

void CT_DataGraphicsContext::DoCmdDiscardBrushPattern()
	{
	INFO_PRINTF1(_L("execute DiscardBrushPattern()"));
	GetGraphicsContext()->DiscardBrushPattern();
	}

void CT_DataGraphicsContext::DoCmdDiscardFont()
	{
	INFO_PRINTF1(_L("execute DiscardFont()"));
	GetGraphicsContext()->DiscardFont();
	}

void CT_DataGraphicsContext::DoCmdDrawArc(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TRect	datRect;
	if ( !GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}

	TPoint	datStart;
	if ( !GetPointFromConfig(aSection, KFldStart, datStart) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldStart());
		SetBlockResult(EFail);
		}

	TPoint	datEnd;
	if ( !GetPointFromConfig(aSection, KFldEnd, datEnd) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldEnd());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute DrawArc(TRect, TPoint, TPoint)"));
		GetGraphicsContext()->DrawArc(datRect, datStart, datEnd);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawBitmapL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	CFbsBitmap*	bitmap=NULL;
	if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldBitmap());
		SetBlockResult(EFail);
		}

	TPoint	datTopLeft;
	if ( GetPointFromConfig(aSection, KFldTopLeft, datTopLeft) )
		{
		if ( dataOk )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawBitmap(TPoint, CFbsBitmap*)"));
			GetGraphicsContext()->DrawBitmap(datTopLeft, bitmap);
			}
		}
	else
		{
		TRect	datDestRect;
		if ( !GetRectFromConfig(aSection, KFldDestRect, datDestRect) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldDestRect());
			SetBlockResult(EFail);
			}

		TRect	datSourceRect;
		if ( GetRectFromConfig(aSection, KFldSourceRect, datSourceRect) )
			{
			if ( dataOk )
				{
				// Execute command and log parameters
				INFO_PRINTF1(_L("execute DrawBitmap(TRect, CFbsBitmap*, TRect)"));
				GetGraphicsContext()->DrawBitmap(datDestRect, bitmap, datSourceRect);
				}
			}
		else
			{
			if ( dataOk )
				{
				// Execute command and log parameters
				INFO_PRINTF1(_L("execute DrawBitmap(TRect, CFbsBitmap*)"));
				GetGraphicsContext()->DrawBitmap(datDestRect, bitmap);
				}
			}
		}
	}

void CT_DataGraphicsContext::DoCmdDrawBitmapMaskedL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TRect	datDestRect;
	if ( !GetRectFromConfig(aSection, KFldDestRect, datDestRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDestRect());
		SetBlockResult(EFail);
		}

	TRect	datSourceRect;
	if ( !GetRectFromConfig(aSection, KFldSourceRect, datSourceRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSourceRect());
		SetBlockResult(EFail);
		}

	TBool	datInvertMask;
	if ( !GetBoolFromConfig(aSection, KFldInvertMask, datInvertMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldInvertMask());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		CWsBitmap*	wsBitmap = NULL;
		if ( CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldWsBitmap, wsBitmap) )
			{
			CWsBitmap*	mask=NULL;
			CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldMaskBitmap, mask);

			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawBitmapMasked(TRect, CWsBitmap*, TRect, CWsBitmap*, TBool)"));
			GetGraphicsContext()->DrawBitmapMasked(datDestRect, wsBitmap, datSourceRect, mask, datInvertMask);
			}
		else
			{
			CFbsBitmap*	fbsBitmap = NULL;
			if ( CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, fbsBitmap) )
				{
				CFbsBitmap*	mask=NULL;
				CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldMaskBitmap, mask);

				// Execute command and log parameters
				INFO_PRINTF1(_L("execute DrawBitmapMasked(TRect, CFbsBitmap*, TRect, CFbsBitmap*, TBool)"));
				GetGraphicsContext()->DrawBitmapMasked(datDestRect, fbsBitmap, datSourceRect, mask, datInvertMask);
				}
			else
				{
				ERR_PRINTF2(KLogMissingParameter, &KFldWsBitmap());
				ERR_PRINTF2(KLogMissingParameter, &KFldBitmap());
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsContext::DoCmdDrawEllipse(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	datRect;
	if ( !GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute DrawEllipse(TRect)"));
		GetGraphicsContext()->DrawEllipse(datRect);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawLine(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPoint	datPoint1;
	if ( !GetPointFromConfig(aSection, KFldPoint1, datPoint1) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint1());
		SetBlockResult(EFail);
		}

	TPoint	datPoint2;
	if ( !GetPointFromConfig(aSection, KFldPoint2, datPoint2) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint2());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DrawLine(TPoint, TPoint)"));
		GetGraphicsContext()->DrawLine(datPoint1, datPoint2);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawLineBy(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	datPoint;
	if ( !GetPointFromConfig(aSection, KFldPoint, datPoint) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DrawLineBy(TPoint)"));
		GetGraphicsContext()->DrawLineBy(datPoint);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawLineTo(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	datPoint;
	if ( !GetPointFromConfig(aSection, KFldPoint, datPoint) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DrawLineTo(TPoint)"));
		GetGraphicsContext()->DrawLineTo(datPoint);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawPie(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TRect	datRect;
	if ( !GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}

	TPoint	datStart;
	if ( !GetPointFromConfig(aSection, KFldStart, datStart) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldStart());
		SetBlockResult(EFail);
		}

	TPoint	datEnd;
	if ( !GetPointFromConfig(aSection, KFldEnd, datEnd) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldEnd());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DrawPie(TRect, TPoint, TPoint)"));
		GetGraphicsContext()->DrawPie(datRect, datStart, datEnd);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawPolygonL(const TDesC& aSection)
	{
	TInt	err=KErrNone;

	// Get test data for command input parameter(s)
	CGraphicsContext::TFillRule	datFillRule=CGraphicsContext::EAlternate;
	CT_GraphicsUtil::ReadFillRule(*this, aSection, KFldFillRule, datFillRule);

	TInt	datNumPoints;
	if ( GetIntFromConfig(aSection, KFldNumPoints, datNumPoints) )
		{
		TPoint*	points=new (ELeave) TPoint[datNumPoints];
		CleanupArrayDeletePushL(points);

		if ( !GetPointListFromConfig(aSection, KFldPointList, datNumPoints, points) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawPolygon(TPoint*, TInt, TFillRule)"));
			err=GetGraphicsContext()->DrawPolygon(points, datNumPoints, datFillRule);
			}
		CleanupStack::PopAndDestroy(points);
		}
	else
		{
		CArrayFix<TPoint>*	points=new (ELeave) CArrayFixFlat<TPoint>(1);
		CleanupStack::PushL(points);
		TBool	hasPointList=GetPointListFromConfigL(aSection, KFldPointList, *points);

		if ( !hasPointList )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawPolygon(CArrayFix<TPoint>*, TFillRule)"));
			err=GetGraphicsContext()->DrawPolygon(points, datFillRule);
			}
		CleanupStack::PopAndDestroy(points);
		}

	// Check the command return code, if !=KErrNone then stop this command
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawPolyLineL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	numPoints;
	if ( GetIntFromConfig(aSection, KFldNumPoints, numPoints) )
		{
		TPoint*	points=new (ELeave) TPoint[numPoints];
		CleanupArrayDeletePushL(points);

		if ( !GetPointListFromConfig(aSection, KFldPointList, numPoints, points) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawPolyLine(TPoint*, TInt)"));
			GetGraphicsContext()->DrawPolyLine(points, numPoints);
			}
		CleanupStack::PopAndDestroy(points);
		}
	else
		{
		CArrayFix<TPoint>*	points=new (ELeave) CArrayFixFlat<TPoint>(1);
		CleanupStack::PushL(points);
		TBool	hasPointList=GetPointListFromConfigL(aSection, KFldPointList, *points);

		if ( !hasPointList )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawPolyLine(CArrayFix<TPoint>*)"));
			GetGraphicsContext()->DrawPolyLine(points);
			}
		CleanupStack::PopAndDestroy(points);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawRect(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	rect;
	if ( !GetRectFromConfig(aSection, KFldRect, rect) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DrawRect(TRect)"));
		GetGraphicsContext()->DrawRect(rect);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawRoundRect(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TRect	rect;
	if ( !GetRectFromConfig(aSection, KFldRect, rect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}

	TSize	size;
	if ( !GetSizeFromConfig(aSection, KFldSize, size) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSize());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DrawRoundRect(TRect, TSize)"));
		GetGraphicsContext()->DrawRoundRect(rect, size);
		}
	}

void CT_DataGraphicsContext::DoCmdDrawText(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	TPtrC	text;
	if( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		}

	TPoint	point;
	if(	GetPointFromConfig(aSection, KFldPoint(), point))
		{
		if ( dataOk )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawText(const TDesC&, const TPoint &)"));
			GetGraphicsContext()->DrawText(text, point);
			}
		}
	else
		{
		TRect	rect;
		if ( GetRectFromConfig(aSection, KFldRect(), rect) )
			{
			TInt	baselineOffset=0;
			if ( !GetIntFromConfig(aSection, KFldBaselineOffset(), baselineOffset))
				{
				dataOk=EFalse;
				ERR_PRINTF2(_L("Missing parameter %S"), &KFldBaselineOffset());
				SetBlockResult(EFail);
				}

			CGraphicsContext::TTextAlign	textAlign=CGraphicsContext::ELeft;
			CT_GraphicsUtil::ReadTextAlign(*this, aSection, KFldTextAlign, textAlign);

			TInt	leftMargin=0;
			GetIntFromConfig(aSection, KFldLeftMargin(), leftMargin);
			if( dataOk )
				{
				// Execute command and log parameters
				INFO_PRINTF1(_L("execute DrawText(const TDesC&, const TRect&, TInt, TTextAlign, TInt)"));
				GetGraphicsContext()->DrawText(text, rect, baselineOffset, textAlign, leftMargin);
				}
			}
		else
			{
			dataOk=EFalse;
			ERR_PRINTF2(_L("Missing parameter %S"), &KFldRect());
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsContext::DoCmdDrawTextExtendedL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	TPtrC	text;
	if( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		}

	TPoint	point;
	if(	!GetPointFromConfig(aSection, KFldPoint(), point))
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldPoint());
		SetBlockResult(EFail);
		}

	CGraphicsContext::TDrawTextExtendedParam*	drawTextExtendedParam=NULL;
	if ( !CT_GraphicsUtil::GetDrawTextExtendedParamL(*this, aSection, KFldTextExtendedParam(), drawTextExtendedParam) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldTextExtendedParam());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute DrawTextExtended(const TDesC&, TPoint, CGraphicsContext::TDrawTextExtendedParam)"));
		TInt	err=GetGraphicsContext()->DrawTextExtended(text, point, *drawTextExtendedParam);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataGraphicsContext::DoCmdDrawTextVertical(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	TPtrC	text;
	if( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		}

	TBool	up=EFalse;
	if(	!GetBoolFromConfig(aSection, KFldUp(), up))
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldUp());
		SetBlockResult(EFail);
		}

	TPoint	point;
	if(	GetPointFromConfig(aSection, KFldPoint(), point))
		{
		if ( dataOk )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC&, const TPoint &, TBool)"));
			GetGraphicsContext()->DrawTextVertical(text, point, up);
			}
		}
	else
		{
		// Another overload DrawTextVertical function
		TRect	rect;
		if(	!GetRectFromConfig(aSection, KFldRect(), rect) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(_L("Missing parameter %S"), &KFldRect());
			SetBlockResult(EFail);
			}

		TInt	baselineOffset=0;
		if ( !GetIntFromConfig(aSection, KFldBaselineOffset(), baselineOffset))
			{
			dataOk=EFalse;
			ERR_PRINTF2(_L("Missing parameter %S"), &KFldBaselineOffset());
			SetBlockResult(EFail);
			}

		CGraphicsContext::TTextAlign	textAlign=CGraphicsContext::ELeft;
		CT_GraphicsUtil::ReadTextAlign(*this, aSection, KFldTextAlign, textAlign);

		TInt	leftMargin=0;
		GetIntFromConfig(aSection, KFldLeftMargin(), leftMargin);

		if ( dataOk )
			{
			INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC&, const TRect&, TInt, TBool, TTextAlign, TInt)"));
			GetGraphicsContext()->DrawTextVertical(text, rect, baselineOffset, up, textAlign, leftMargin);
			}
		}
	}

void CT_DataGraphicsContext::DoCmdGetShadowColor(const TDesC& aSection)
	{
	TRgb	color;
	INFO_PRINTF1(_L("execute GetShadowColor(TRgb)"));
	TInt	err=GetGraphicsContext()->GetShadowColor(color);
	INFO_PRINTF5(KLogColor, color.Red(), color.Green(), color.Blue(), color.Alpha());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	else
		{
		TRgb	expected;
		if ( GetRgbFromConfig(aSection, KFldRgb(), expected) )
			{
			if ( color!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsContext::DoCmdGetUnderlineMetrics(const TDesC& aSection)
	{
	TInt	top;
	TInt	bottom;
	INFO_PRINTF1(_L("execute GetUnderlineMetrics(TInt, TInt)"));
	TInt	err=GetGraphicsContext()->GetUnderlineMetrics(top, bottom);
	INFO_PRINTF3(KLogMetrics, top, bottom);

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	else
		{
		TInt	expected;
		if ( GetIntFromConfig(aSection, KFldTop(), expected) )
			{
			if ( top!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		if ( GetIntFromConfig(aSection, KFldBottom(), expected) )
			{
			if ( bottom!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsContext::DoCmdJustificationInPixels(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TInt	excessPixels;
	if(	!GetIntFromConfig(aSection, KFldExcessPixels(), excessPixels) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldExcessPixels());
		SetBlockResult(EFail);
		}

	TInt	totalUnits;
	if(	!GetIntFromConfig(aSection, KFldTotalUnits(), totalUnits) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldTotalUnits());
		SetBlockResult(EFail);
		}

	TInt	firstUnit;
	if(	!GetIntFromConfig(aSection, KFldFirstUnit(), firstUnit) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFirstUnit());
		SetBlockResult(EFail);
		}

	TInt	numUnits;
	if(	!GetIntFromConfig(aSection, KFldNumUnits(), numUnits) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldNumUnits());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute JustificationInPixels(TInt, TInt, TInt, TInt)"));
		TInt	actual=GetGraphicsContext()->JustificationInPixels(excessPixels, totalUnits, firstUnit, numUnits);
		INFO_PRINTF2(_L("JustificationInPixels=%d"), actual);

		TInt	expected;
		if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsContext::DoCmdMapColorsL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TRect	rect;
	if(	!GetRectFromConfig(aSection, KFldRect(), rect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}

	TInt	numColors;
	if ( !GetIntFromConfig(aSection, KFldNumColors, numColors) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldNumColors());
		SetBlockResult(EFail);
		}

	TBool	mapForwards=EFalse;
	if(	!GetBoolFromConfig(aSection, KFldMapForwards(), mapForwards))
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldMapForwards());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TRgb*	colors=new (ELeave) TRgb[numColors];
		CleanupArrayDeletePushL(colors);

		if ( !GetRgbListFromConfig(aSection, KFldRgbList, numColors, colors) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldRgbList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute MapColors(const TRect&, const TRgb*, TInt, TBool)"));
			GetGraphicsContext()->MapColors(rect, colors, numColors, mapForwards);
			}
		CleanupStack::PopAndDestroy(colors);
		}
	}

void CT_DataGraphicsContext::DoCmdMoveBy(const TDesC& aSection)
	{
	TPoint	point;
	if ( !GetPointFromConfig(aSection, KFldPoint, point) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute MoveBy(TPoint)"));
		GetGraphicsContext()->MoveBy(point);
		}
	}

void CT_DataGraphicsContext::DoCmdMoveTo(const TDesC& aSection)
	{
	TPoint	point;
	if ( !GetPointFromConfig(aSection, KFldPoint, point) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute MoveTo(TPoint)"));
		GetGraphicsContext()->MoveTo(point);
		}
	}

void CT_DataGraphicsContext::DoCmdPlot(const TDesC& aSection)
	{
	TPoint	point;
	if ( !GetPointFromConfig(aSection, KFldPoint, point) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Plot(TPoint)"));
		GetGraphicsContext()->Plot(point);
		}
	}

void CT_DataGraphicsContext::DoCmdReserved()
	{
	INFO_PRINTF1(_L("execute Reserved()"));
	GetGraphicsContext()->Reserved();
	}

void CT_DataGraphicsContext::DoCmdReset()
	{
	INFO_PRINTF1(_L("execute Reset()"));
	GetGraphicsContext()->Reset();
	}

void CT_DataGraphicsContext::DoCmdSetBrushColor(const TDesC& aSection)
	{
	TRgb	rgb;
	if(	!GetRgbFromConfig(aSection, KFldColor(), rgb))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldColor());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetBrushColor(TRgb)"));
		GetGraphicsContext()->SetBrushColor(rgb);
		}
	}

void CT_DataGraphicsContext::DoCmdSetBrushOrigin(const TDesC& aSection)
	{
	TPoint	point;
	if ( !GetPointFromConfig(aSection, KFldPoint, point) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetBrushOrigin(TPoint)"));
		GetGraphicsContext()->SetBrushOrigin(point);
		}
	}

void CT_DataGraphicsContext::DoCmdSetBrushStyle(const TDesC& aSection)
	{
	CGraphicsContext::TBrushStyle	brushStyle;
	if(	!CT_GraphicsUtil::ReadBrushStyle(*this, aSection, KFldBrushStyle(), brushStyle))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBrushStyle());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetBrushStyle(CGraphicsContext::TBrushStyle)"));
		GetGraphicsContext()->SetBrushStyle(brushStyle);
		}
	}

void CT_DataGraphicsContext::DoCmdSetClippingRegion(const TDesC& aSection)
	{
	RRegion	region;
	CleanupClosePushL(region);
	if ( !GetRegionFromConfig(aSection, KFldRegion, region) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRegion());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetClippingRegion(TRegion)"));
		TInt	err=GetGraphicsContext()->SetClippingRegion(region);

		// Check the command return code, if !=KErrNone then stop this command
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	CleanupStack::PopAndDestroy(&region);
	}

void CT_DataGraphicsContext::DoCmdSetCharJustification(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TInt	excessWidth=0;
	if(	!GetIntFromConfig(aSection, KFldExcessWidth(), excessWidth) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldExcessWidth());
		SetBlockResult(EFail);
		}

	TInt	numChars=0;
	if(	!GetIntFromConfig(aSection, KFldNumChars(), numChars) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldNumChars());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute SetCharJustification(TInt, TInt)"));
		GetGraphicsContext()->SetCharJustification(excessWidth, numChars);
		}
	}

void CT_DataGraphicsContext::DoCmdSetClippingRect(const TDesC& aSection)
	{
	TRect	rect;
	if ( !GetRectFromConfig(aSection, KFldRect, rect) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRect());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetClippingRect(TRect)"));
		GetGraphicsContext()->SetClippingRect(rect);
		}
	}

void CT_DataGraphicsContext::DoCmdSetDrawMode(const TDesC& aSection)
	{
	CGraphicsContext::TDrawMode	drawMode;
	if(	!CT_GraphicsUtil::ReadDrawMode(*this, aSection, KFldDrawMode(), drawMode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldDrawMode());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetDrawMode(CGraphicsContext::TDrawMode)"));
		GetGraphicsContext()->SetDrawMode(drawMode);
		}
	}

void CT_DataGraphicsContext::DoCmdSetOrigin(const TDesC& aSection)
	{
	TPoint	point;
	if ( !GetPointFromConfig(aSection, KFldPoint, point) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetOrigin(TPoint)"));
		GetGraphicsContext()->SetOrigin(point);
		}
	}

void CT_DataGraphicsContext::DoCmdSetPenColor(const TDesC& aSection)
	{
	TRgb	rgb;
	if(	!GetRgbFromConfig(aSection, KFldColor(), rgb))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldColor());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetPenColor(TRgb)"));
		GetGraphicsContext()->SetPenColor(rgb);
		}
	}

void CT_DataGraphicsContext::DoCmdSetPenSize(const TDesC& aSection)
	{
	TSize	size;
	if(	!GetSizeFromConfig(aSection, KFldSize(), size))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSize());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetPenSize(TSize)"));
		GetGraphicsContext()->SetPenSize(size);
		}
	}

void CT_DataGraphicsContext::DoCmdSetPenStyle(const TDesC& aSection)
	{
	CGraphicsContext::TPenStyle	penStyle;
	if(	!CT_GraphicsUtil::ReadPenStyle(*this, aSection, KFldPenStyle(), penStyle))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPenStyle());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetPenStyle(CGraphicsContext::TPenStyle)"));
		GetGraphicsContext()->SetPenStyle(penStyle);
		}
	}

void CT_DataGraphicsContext::DoCmdSetShadowColor(const TDesC& aSection)
	{
	TRgb	color;
	if ( !GetRgbFromConfig(aSection, KFldRgb(), color) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRgb());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetShadowColor(TRgb)"));
		TInt	err=GetGraphicsContext()->SetShadowColor(color);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataGraphicsContext::DoCmdSetStrikethroughStyle(const TDesC& aSection)
	{
	TFontStrikethrough	fontStrikethrough;
	if(	!CT_GraphicsUtil::ReadFontStrikethrough(*this, aSection, KFldFontStrikethrough(), fontStrikethrough) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFontStrikethrough());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetStrikethroughStyle(TFontStrikethrough)"));
		GetGraphicsContext()->SetStrikethroughStyle(fontStrikethrough);
		}
	}

void CT_DataGraphicsContext::DoCmdSetUnderlineStyle(const TDesC& aSection)
	{
	TFontUnderline	fontUnderline;
	if(	!CT_GraphicsUtil::ReadFontUnderline(*this, aSection, KFldFontUnderline(), fontUnderline) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFontUnderline());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetUnderlineStyle(TFontUnderline)"));
		GetGraphicsContext()->SetUnderlineStyle(fontUnderline);
		}
	}

void CT_DataGraphicsContext::DoCmdSetWordJustification(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TInt	excessWidth=0;
	if(	!GetIntFromConfig(aSection, KFldExcessWidth(), excessWidth))
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldExcessWidth());
		SetBlockResult(EFail);
		}

	TInt	numGaps=0;
	if(	!GetIntFromConfig(aSection, KFldNumGaps(), numGaps))
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldNumGaps());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute SetWordJustification(TInt, TInt)"));
		GetGraphicsContext()->SetWordJustification(excessWidth, numGaps);
		}
	}

void CT_DataGraphicsContext::DoCmdUseBrushPatternL(const TDesC& aSection)
	{
	CFbsBitmap*	bitmap=NULL;
	if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBitmap());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute UseBrushPattern(CFbsBitmap*)"));
		GetGraphicsContext()->UseBrushPattern(bitmap);
		}
	}

void CT_DataGraphicsContext::DoCmdUseFontL(const TDesC& aSection)
	{
	CFont*	font=NULL;

	CT_GraphicsUtil::GetFontL(*this, aSection, KFldFont, font);
	INFO_PRINTF1(_L("execute UseFont(CFont*)"));
	GetGraphicsContext()->UseFont(font);
	}
