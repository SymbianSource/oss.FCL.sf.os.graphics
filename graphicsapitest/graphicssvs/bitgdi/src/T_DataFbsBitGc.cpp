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
#include <e32cmn.h>
#include <e32property.h>
#include "T_DataFbsBitGc.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdNewL,						"NewL");
_LIT(KCmdDestructor,				"~");
_LIT(KCmdActivate,					"Activate");
_LIT(KCmdActivateNoJustAutoUpdate,	"ActivateNoJustAutoUpdate");
_LIT(KCmdAlphaBlendBitmaps,			"AlphaBlendBitmaps");
_LIT(KCmdDisown,                    "disown");
_LIT(KCmdDrawPolyLineNoEndPoint,    "DrawPolyLineNoEndPoint");

_LIT(KCmdCancelClipping,            "CancelClipping");
_LIT(KCmdCopySettings,              "CopySettings");
_LIT(KCmdBitBlt,                    "BitBlt");

_LIT(KCmdDrawText,					"DrawText");
_LIT(KCmdDrawTextVertical,			"DrawTextVertical");
_LIT(KCmdUpdateJustification,		"UpdateJustification");
_LIT(KCmdUpdateJustificationVertical,"UpdateJustificationVertical");

_LIT(KCmdFadeArea,                   "FadeArea");
_LIT(KCmdIsBrushPatternUsed,         "IsBrushPatternUsed");
_LIT(KCmdIsFontUsed,                 "IsFontUsed");
_LIT(KCmdOrientationsAvailable,      "OrientationsAvailable");
_LIT(KCmdRectDrawnTo,                "RectDrawnTo");
_LIT(KCmdResized,                    "Resized");
_LIT(KCmdSetDitherOrigin,            "SetDitherOrigin");
_LIT(KCmdSetFadeMode,                "SetFadeMode");
_LIT(KCmdSetFadingParameters,        "SetFadingParameters");
_LIT(KCmdSetOrientation,             "SetOrientation");
_LIT(KCmdSetShadowMode,              "SetShadowMode");
_LIT(KCmdSetUserDisplayMode,         "SetUserDisplayMode");
_LIT(KCmdShadowArea,                 "ShadowArea");
_LIT(KCmdUseBrushPattern,            "UseBrushPattern");
_LIT(KCmdUseFont,                    "UseFont");
_LIT(KCmdUseFontNoDuplicate,         "UseFontNoDuplicate");
_LIT(KCmdGetAvailableOrientation,    "GetAvailableOrientation");
_LIT(KCmdPromptMessage,				 "PromptMessage");
///	Fields
_LIT(KFldAlphaBmp,					"alphabmp");
_LIT(KFldAlphaPt,					"alphapt");
_LIT(KFldDestPt,					"destpt");
_LIT(KFldDevice,					"device");
_LIT(KFldSrcBmp1,					"srcbmp1");
_LIT(KFldSrcBmp2,					"srcbmp2");
_LIT(KFldSrcPt,						"srcpt");
_LIT(KFldSrcRect,					"srcrect");
_LIT(KFldNumPoints,					"numpoints");
_LIT(KFldPointList,					"pointlist");

_LIT(KFldFbsBitGc,                  "fbsBitGc");
_LIT(KFldPoint,                     "point");

_LIT(KFldRect,						"rect");
_LIT(KFldTextAlign,					"textalign");
_LIT(KFldText,						"text");
_LIT(KFldLeftMargin,				"leftmargin");
_LIT(KFldBaselineOffset,			"baselineoffset");
_LIT(KFldTextWidth,					"textwidth");
_LIT(KFldUp,						"up");
_LIT(KFldParam,						"param");

_LIT(KFldRegion,					"region");
_LIT(KFldFadeMode,                  "fademode");
_LIT(KFldBlackMap,					"blackmap");
_LIT(KFldWhiteMap,					"whitemap");
_LIT(KFldGraphicsOrientation,       "orientation");
_LIT(KFldShadowMode,                "shadowmode");
_LIT(KFldDisplayMode,               "displaymode");
_LIT(KFldHandle,                    "handle");
_LIT(KFldFont,                      "font");
_LIT(KFldBitmap,					"bitmap");
_LIT(KFldUseHandle,					"usehandle");
_LIT(KFldPromptText,				"message");
_LIT(KFldExpectedKey,				"keyCode");

_LIT(KFldExpect,                    "expect");
_LIT(KFormatFieldNumber,	        "Orientation_%d=%d");
///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,			"Not expected value,get %d, expectd %d");
/*@}*/
_LIT(KPromptProcess,				"t_prompt.exe");


/**
* Two phase constructor
*/
CT_DataFbsBitGc* CT_DataFbsBitGc::NewL()
	{
	CT_DataFbsBitGc*	ret=new (ELeave) CT_DataFbsBitGc();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsBitGc::CT_DataFbsBitGc()
:	CT_DataBitmapContext()
,	iFbsBitGc(NULL)
	{
	}

/**
* Protected second phase construction
*/
void CT_DataFbsBitGc::ConstructL()
	{
	
	}

/**
* Destructor.
*/
CT_DataFbsBitGc::~CT_DataFbsBitGc()
	{

	DestroyData();
	}

void CT_DataFbsBitGc::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iFbsBitGc	= static_cast<CFbsBitGc*> (aAny);
	}

void CT_DataFbsBitGc::DisownObjectL()
	{
	iFbsBitGc = NULL;
	}

void CT_DataFbsBitGc::DestroyData()
	{
	delete iFbsBitGc;
	iFbsBitGc=NULL;
	}

/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsBitGc::GetObject()
	{
	return iFbsBitGc;
	}

//	CT_DataBitmapContext implementation
CBitmapContext* CT_DataFbsBitGc::GetBitmapContext() const
	{
	return iFbsBitGc;
	}

//	CT_DataGraphicsContext implementation
CGraphicsContext* CT_DataFbsBitGc::GetGraphicsContext() const
	{
	return iFbsBitGc;
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
TBool CT_DataFbsBitGc::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	/* Another work package will complete the implement of this */
	TBool	ret = ETrue;

	if ( aCommand==KCmdNewL )
		{
		DoCmdNewL();
		}
	else if ( aCommand==KCmdDestructor )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdActivate )
		{
		DoCmdActivateL(aSection);
		}
	else if ( aCommand == KCmdDisown )
		{
		DisownObjectL();
		}
	else if ( aCommand==KCmdActivateNoJustAutoUpdate )
		{
		DoCmdActivateNoJustAutoUpdateL(aSection);
		}
	else if ( aCommand==KCmdAlphaBlendBitmaps )
		{
		if ( !DoCmdAlphaBlendBitmapsL(aSection) )
			{
			ret=CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if ( aCommand==KCmdDrawPolyLineNoEndPoint )
		{
		DoCmdDrawPolyLineNoEndPointL(aSection);
		}
	else if ( aCommand==KCmdCancelClipping)
		{
		DoCmdCancelClipping(aSection);
		}
	else if ( aCommand==KCmdCopySettings)
		{
		DoCmdCopySettingsL(aSection);
		}
	else if ( aCommand==KCmdBitBlt)
		{
		if(!DoCmdBitBltL(aSection))
			{
			ret=CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if(aCommand==KCmdDrawText)
		{
		DoCmdDrawTextL(aSection);
		}
	else if(aCommand==KCmdDrawTextVertical)
		{
		DoCmdDrawTextVerticalL(aSection);
		}
	else if(aCommand==KCmdUpdateJustification)
		{
		DoCmdUpdateJustificationL(aSection);
		}
	else if(aCommand==KCmdUpdateJustificationVertical)
		{
		DoCmdUpdateJustificationVerticalL(aSection);
		}
	else if(aCommand==KCmdFadeArea)
		{
		DoCmdFadeArea(aSection);
		}
	else if(aCommand==KCmdIsBrushPatternUsed)
		{
		DoCmdIsBrushPatternUsed(aSection);
		}
	else if(aCommand==KCmdIsFontUsed)
		{
		DoCmdIsFontUsed(aSection);
		}
	else if(aCommand==KCmdOrientationsAvailable)
		{
		DoCmdOrientationsAvailable(aSection);
		}
	else if(aCommand==KCmdRectDrawnTo)
		{
		DoCmdRectDrawnTo(aSection);
		}
	else if(aCommand==KCmdResized)
		{
		DoCmdResized(aSection);
		}
	else if(aCommand==KCmdSetDitherOrigin)
		{
		DoCmdSetDitherOrigin(aSection);
		}
	else if(aCommand==KCmdSetFadeMode)
		{
		DoCmdSetFadeMode(aSection);
		}
	else if(aCommand==KCmdSetFadingParameters)
		{
		if(!DoCmdSetFadingParameters(aSection))
			{
			ret=CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if(aCommand==KCmdSetOrientation)
		{
		DoCmdSetOrientation(aSection);
		}
	else if(aCommand==KCmdSetShadowMode)
		{
		DoCmdSetShadowMode(aSection);
		}
	else if(aCommand==KCmdSetUserDisplayMode)
		{
		DoCmdSetUserDisplayMode(aSection);
		}
	else if(aCommand==KCmdShadowArea)
		{
		DoCmdShadowArea(aSection);
		}
	else if(aCommand==KCmdUseBrushPattern)
		{
		DoCmdUseBrushPatternL(aSection);
		}
	else if(aCommand==KCmdUseFont)
		{
		DoCmdUseFontL(aSection);
		}
	else if(aCommand==KCmdUseFontNoDuplicate)
		{
		DoCmdUseFontNoDuplicateL(aSection);
		}
	else if(aCommand==KCmdPromptMessage)
		{
		DoCmdPromptMessageL(aSection);
		}
	else
		{
		ret=CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataFbsBitGc::DoCmdNewL()
	{
	DestroyData();

	INFO_PRINTF1(_L("execute CFbsBitGc::NewL"));
	TRAPD( err, iFbsBitGc = CFbsBitGc::NewL() );
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataFbsBitGc::DoCmdDestructor()
	{
	DestroyData();
	}

void CT_DataFbsBitGc::DoCmdActivateL(const TDesC& aSection)
	{
	CFbsDevice*	device=NULL;
	if ( !CT_GraphicsUtil::GetFbsDeviceL(*this, aSection, KFldDevice, device) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldDevice());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Activate(CFbsDevice*)"));
		iFbsBitGc->Activate(device);
		}
	}

void CT_DataFbsBitGc::DoCmdActivateNoJustAutoUpdateL(const TDesC& aSection)
	{
	CFbsDevice*	device=NULL;
	if ( !CT_GraphicsUtil::GetFbsDeviceL(*this, aSection, KFldDevice, device) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldDevice());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute ActivateNoJustAutoUpdate(CFbsDevice*)"));
		iFbsBitGc->ActivateNoJustAutoUpdate(device);
		}
	}

TBool CT_DataFbsBitGc::DoCmdAlphaBlendBitmapsL(const TDesC& aSection)
	{
	CFbsBitmap*	fbsBitmap1=NULL;
	TBool		ret=CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldSrcBmp1, fbsBitmap1);
	if ( ret )
		{
		TBool		dataOk=ETrue;

		CFbsBitmap*	fbsBitmap2=NULL;
		if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldSrcBmp2, fbsBitmap2) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldSrcBmp2());
			SetBlockResult(EFail);
			}

		CFbsBitmap*	alphaBitmap=NULL;
		if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldAlphaBmp, alphaBitmap) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldAlphaBmp());
			SetBlockResult(EFail);
			}

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

		TPoint	sourcePoint;
		if ( !GetPointFromConfig(aSection, KFldSrcPt, sourcePoint) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldSrcPt());
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
			INFO_PRINTF1(_L("execute AlphaBlendBitmaps(const TPoint&,const CFbsBitmap*,const CFbsBitmap*,const TRect&,const TPoint&,const CFbsBitmap*,const TPoint&"));
			TInt	err=iFbsBitGc->AlphaBlendBitmaps(destinationPoint, fbsBitmap1, fbsBitmap2, sourceRect, sourcePoint, alphaBitmap, alphaPoint);
			if ( err!=KErrNone )
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			}
		}

	return ret;
	}

void CT_DataFbsBitGc::DoCmdDrawPolyLineNoEndPointL(const TDesC& aSection)
	{
	TInt	datNumPoints;
	if ( GetIntFromConfig(aSection, KFldNumPoints, datNumPoints) )
		{
		TPoint*	points=new (ELeave) TPoint[datNumPoints];
		
		if ( !GetPointListFromConfig(aSection, KFldPointList, datNumPoints, points) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawPolyLineNoEndPoint(TPoint*, TInt)"));
			iFbsBitGc->DrawPolyLineNoEndPoint(points, datNumPoints);
			}
		delete [] points;
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
			INFO_PRINTF1(_L("execute DrawPolyLineNoEndPoint(CArrayFix<TPoint>*)"));
			iFbsBitGc->DrawPolyLineNoEndPoint(points);
			}
		CleanupStack::PopAndDestroy(points);
		}
	}

void CT_DataFbsBitGc::DoCmdCancelClipping(const TDesC& /*aSection*/)
	{
	INFO_PRINTF1(_L("execute CancelClipping()"));
	iFbsBitGc->CancelClipping();
	}

void CT_DataFbsBitGc::DoCmdCopySettingsL(const TDesC& aSection)
	{
	CFbsBitGc* fbsBitGc=NULL;

	if ( !CT_GraphicsUtil::GetFbsBitGcL(*this, aSection, KFldFbsBitGc, fbsBitGc) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFbsBitGc());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute CopySettings(CFbsBitGc&)"));
		iFbsBitGc->CopySettings(*fbsBitGc);
		}
	}

TBool CT_DataFbsBitGc::DoCmdBitBltL(const TDesC& aSection)
	{
	TBool ret=ETrue;
	CFbsBitGc* fbsBitGc=NULL;
	
	if ( !CT_GraphicsUtil::GetFbsBitGcL(*this, aSection, KFldFbsBitGc, fbsBitGc) )
		{
		ret=EFalse;
		}
	else
		{
		// Execute command and log parameters
		TPoint	point;
		if ( !GetPointFromConfig(aSection, KFldPoint, point) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
			SetBlockResult(EFail);
			//Case failed, needn't execute other step.
			return ret;
			}
		
		TRect	sourceRect;
		if(!GetRectFromConfig(aSection, KFldSrcRect, sourceRect))
			{
			INFO_PRINTF1(_L("execute BitBlt(const TPoint&, const CFbsBitGc&)"));
			iFbsBitGc->BitBlt(point,*fbsBitGc);
			}
		else
			{
			INFO_PRINTF1(_L("execute BitBlt(const TPoint&, const CFbsBitGc&,const TRect &)"));
			iFbsBitGc->BitBlt(point,*fbsBitGc,sourceRect);
			}
		}
	
	return ret;
	}

void CT_DataFbsBitGc::DoCmdDrawTextL(const TDesC& aSection)
	{
	TRect rect;
	TInt baselineOffset=0;
	TPtrC text;
	TInt datTextWidth;
	CGraphicsContext::TTextParameters* param=NULL;
	
	if( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		return;
		}
	
	if (GetRectFromConfig(aSection, KFldRect, rect) )
		{
			if (GetIntFromConfig(aSection, KFldBaselineOffset, baselineOffset))
				{
				TInt datMargin=0;
				CGraphicsContext::TTextAlign textAlign=CGraphicsContext::ELeft;
				CT_GraphicsUtil::ReadTextAlign(*this, aSection, KFldTextAlign,textAlign);
				if(!GetIntFromConfig(aSection, KFldLeftMargin, datMargin))
					{
					INFO_PRINTF1(_L("Using the default value aMargin=0"));
					}
				
				if (GetIntFromConfig(aSection, KFldTextWidth, datTextWidth))
					{
					if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
						{
						INFO_PRINTF1(_L("execute DrawText(const TDesC &,const TTextParameters *,const TRect &,TInt,TInt,TTextAlign,TInt)"));
						iFbsBitGc->DrawText(text, param, rect, baselineOffset, datTextWidth, textAlign, datMargin);
						}
					else
						{
						INFO_PRINTF1(_L("execute DrawText(const TDesC &,const TRect &,TInt,TInt,TTextAlign,TInt)"));
						iFbsBitGc->DrawText(text, rect, baselineOffset, datTextWidth, textAlign, datMargin);
						}

					}
				else
					{
					if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
						{
						INFO_PRINTF1(_L("execute DrawText(const TDesC &,const TTextParameters *,const TRect &,TInt,TTextAlign,TInt)"));
						iFbsBitGc->DrawText(text, param, rect, baselineOffset, textAlign, datMargin);
						}
					else
						{
						INFO_PRINTF1(_L("execute DrawText(const TDesC &,const TRect &,TInt,TTextAlign,TInt)"));
						iFbsBitGc->DrawText(text, rect, baselineOffset, textAlign, datMargin);
						}
					}
				}
			else
				{
				if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
					{
					INFO_PRINTF1(_L("execute DrawText(const TDesC&,const TTextParameters *,const TRect&)"));
					iFbsBitGc->DrawText(text, param, rect);
					}
				else
					{
					INFO_PRINTF1(_L("execute DrawText(const TDesC&,const TRect&)"));
					iFbsBitGc->DrawText(text, rect);
					}
				}
			}
	else
		{
		TPoint	point;
		if(!GetPointFromConfig(aSection, KFldPoint, point))
			{
			if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
				{
				INFO_PRINTF1(_L("execute DrawText(const TDesC&, const TTextParameters *)"));
				iFbsBitGc->DrawText(text,param);
				}
			else
				{
				INFO_PRINTF1(_L("execute DrawText(const TDesC&)"));
				iFbsBitGc->DrawText(text);
				}
			}
		else
			{
			if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
				{
				INFO_PRINTF1(_L("execute DrawText(const TDesC&,const TTextParameters *,const TPoint&)"));
	            iFbsBitGc->DrawText(text,param,point);	
				}
			else
				{
				INFO_PRINTF1(_L("execute DrawText(const TDesC&,const TPoint&)"));
	            iFbsBitGc->DrawText(text,point);	
				}
			}
		}
	delete param;
	}

void CT_DataFbsBitGc::DoCmdDrawTextVerticalL(const TDesC& aSection)
	{
	TPtrC	text;
	TInt datTextWidth;
	TBool	up=EFalse;
	CGraphicsContext::TTextParameters* param=NULL;
	
	if( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		return;
		}
	
	if(	!GetBoolFromConfig(aSection, KFldUp, up))
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldUp());
		SetBlockResult(EFail);
		return;
		}
	
	TRect	rect;
	TInt baselineOffset=0;
	if (GetRectFromConfig(aSection, KFldRect, rect) )
		{
		if (GetIntFromConfig(aSection, KFldBaselineOffset, baselineOffset))
			{
			TInt datMargin=0;
			CGraphicsContext::TTextAlign textAlign=CGraphicsContext::ELeft;
			CT_GraphicsUtil::ReadTextAlign(*this, aSection, KFldTextAlign,textAlign);
			if(!GetIntFromConfig(aSection, KFldLeftMargin, datMargin))
				{
				INFO_PRINTF1(_L("Using the default value aMargin=0"));
				}
			
			if (GetIntFromConfig(aSection, KFldTextWidth, datTextWidth))
				{
				if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
					{
					INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TTextParameters *,const TRect &,TInt,TInt,TBool,TTextAlign,TInt)"));
					iFbsBitGc->DrawTextVertical(text,param,rect,baselineOffset,datTextWidth,up,textAlign,datMargin);					
					}
				else
					{
					INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TRect &,TInt,TInt,TBool,TTextAlign,TInt)"));
					iFbsBitGc->DrawTextVertical(text,rect,baselineOffset,datTextWidth,up,textAlign,datMargin);					
					}
				}
			else 
				{
				if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
					{
					INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TTextParameters *,const TRect &,TInt,TBool,TTextAlign,TInt)"));
					iFbsBitGc->DrawTextVertical(text,param,rect,baselineOffset,up,textAlign,datMargin);
					}
				else
					{
					INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TRect &,TInt,TBool,TTextAlign,TInt)"));
					iFbsBitGc->DrawTextVertical(text,rect,baselineOffset,up,textAlign,datMargin);					
					}
				}
			}
		else
			{
			if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
				{
				INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TTextParameters *,const TRect &,TBool)"));
				iFbsBitGc->DrawTextVertical(text,param,rect,up);
				}
			else
				{
				INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TRect &,TBool)"));
				iFbsBitGc->DrawTextVertical(text,rect,up);				
				}
			}
		}
	else
		{
		TPoint	point;
		if(!GetPointFromConfig(aSection, KFldPoint, point))
			{
			if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
				{
				INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TTextParameters *,TBool)"));
				iFbsBitGc->DrawTextVertical(text,param,up);
				}
			else
				{
				INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,TBool)"));
				iFbsBitGc->DrawTextVertical(text,up);				
				}
			}
		else
			{
			if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
				{
				INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TTextParameters *,const TPoint &,TBool)"));
				iFbsBitGc->DrawTextVertical(text,param,point,up);
				}
			else
				{
				INFO_PRINTF1(_L("execute DrawTextVertical(const TDesC &,const TPoint &,TBool)"));
				iFbsBitGc->DrawTextVertical(text,point,up);
				}
			}
		}
	delete param;
	}

void CT_DataFbsBitGc::DoCmdUpdateJustificationL(const TDesC& aSection)
	{
	TPtrC text;
	CGraphicsContext::TTextParameters* param=NULL;
	if ( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		return;
		}
	if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
		{
		INFO_PRINTF1(_L("execute UpdateJustification(const TDesC &,const TTextParameters *)"));
		iFbsBitGc->UpdateJustification(text,param);		
		}
	else
		{
		INFO_PRINTF1(_L("execute UpdateJustification(const TDesC &)"));
		iFbsBitGc->UpdateJustification(text);		
		}
	delete param;
	}

void CT_DataFbsBitGc::DoCmdUpdateJustificationVerticalL(const TDesC& aSection)
	{
	TPtrC	text;
	TBool	up=EFalse;
	CGraphicsContext::TTextParameters *param=NULL;
	if( !GetStringFromConfig(aSection, KFldText(), text) )
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldText());
		SetBlockResult(EFail);
		return;
		}
	
	if(	!GetBoolFromConfig(aSection, KFldUp, up))
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldUp());
		SetBlockResult(EFail);
		return;
		}
	
	if(CT_GraphicsUtil::GetTextParametersL(*this,aSection,KFldParam,param))
		{
		INFO_PRINTF1(_L("execute UpdateJustificationVertical(const TDesC &,const TTextParameters *,TBool)"));
		iFbsBitGc->UpdateJustificationVertical(text,param,up);			
		}
	else
		{
		INFO_PRINTF1(_L("execute UpdateJustificationVertical(const TDesC &,TBool)"));
		iFbsBitGc->UpdateJustificationVertical(text,up);		
		}
	delete param;
	}

void CT_DataFbsBitGc::DoCmdFadeArea(const TDesC& aSection)
	{
	RRegion reg;
	if(GetRegionFromConfig(aSection,KFldRegion,reg))
		{
		INFO_PRINTF1(_L("execute FadeArea(const TRegion *)"));
		iFbsBitGc->FadeArea(&reg);
		}
	else
		{
		INFO_PRINTF2(_L("Missing parameter %S"), &KFldRegion());
		INFO_PRINTF1(_L("execute FadeArea(NULL)"));
		iFbsBitGc->FadeArea(NULL);
		}
	}

void CT_DataFbsBitGc::DoCmdIsBrushPatternUsed(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute IsBrushPatternUsed()"));
	TBool bUsed=iFbsBitGc->IsBrushPatternUsed();
	
	TBool expect;
	if(GetBoolFromConfig(aSection,KFldExpect,expect))
		{
		if(expect!=bUsed)
			{
			ERR_PRINTF3(KLogNotExpectedValue, bUsed,expect);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsBitGc::DoCmdIsFontUsed(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute IsFontUsed()"));
	TBool bUsed=iFbsBitGc->IsFontUsed();
		
	TBool expect;
	if(GetBoolFromConfig(aSection,KFldExpect,expect))
		{
		if(expect!=bUsed)
			{
			ERR_PRINTF3(KLogNotExpectedValue, bUsed,expect);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsBitGc::DoCmdOrientationsAvailable(const TDesC& /*aSection*/)
	{
	INFO_PRINTF1(_L("execute OrientationsAvailable(TBool aOrientation[4])"));

	//iGraphicsOrientation defined in CDataWrapperBase
	iFbsBitGc->OrientationsAvailable(iGraphicsOrientation);
	
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	
	for(TInt i=0;i<4;i++)
		{
		tempStore.Format(KFormatFieldNumber, i+1,iGraphicsOrientation[i]);
		INFO_PRINTF1(KFormatFieldNumber);
		}
	}

void CT_DataFbsBitGc::DoCmdRectDrawnTo(const TDesC& aSection)
	{
	TRect	rect;
	if(GetRectFromConfig(aSection, KFldRect, rect))
		{
		INFO_PRINTF1(_L("execute RectDrawnTo(TRect&)"));
		iFbsBitGc->RectDrawnTo(rect);
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldRect());
		SetBlockResult(EFail);
		}

	}

void CT_DataFbsBitGc::DoCmdResized(const TDesC& /*aSection*/)
	{
	INFO_PRINTF1(_L("execute Resized()"));
	iFbsBitGc->Resized();
	
	}

void CT_DataFbsBitGc::DoCmdSetDitherOrigin(const TDesC& aSection)
	{
	TPoint	point;
	if (GetPointFromConfig(aSection, KFldPoint, point))
		{
		INFO_PRINTF1(_L("execute SetDitherOrigin(const TPoint &)"));
		iFbsBitGc->SetDitherOrigin(point);
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
		SetBlockResult(EFail);
		}
	}

void CT_DataFbsBitGc::DoCmdSetFadeMode(const TDesC& aSection)
	{
	TBool fade;
	if(GetBoolFromConfig(aSection,KFldFadeMode,fade))
		{
		INFO_PRINTF1(_L("execute SetFadeMode(TBool)"));
		iFbsBitGc->SetFadeMode(fade);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetFadeMode(TBool aFadeMode=EFalse)"));
		iFbsBitGc->SetFadeMode();
		}
	}

TBool CT_DataFbsBitGc::DoCmdSetFadingParameters(const TDesC& aSection)
	{
	TInt blackmap=0;
	TInt whitemap=255;
	if(GetIntFromConfig(aSection,KFldWhiteMap,whitemap))
		{
		//let base class handle this
		return EFalse;
		}
	else
		{
		if(GetBoolFromConfig(aSection,KFldBlackMap,blackmap))
			{
			INFO_PRINTF1(_L("execute SetFadingParameters(TUint8)"));
			iFbsBitGc->SetFadingParameters(blackmap);
			}
		else
			{
			INFO_PRINTF1(_L("execute SetFadingParameters(TUint8 aBlackMap=0)"));
			iFbsBitGc->SetFadingParameters();
			}
		}
	return ETrue;
	}

void CT_DataFbsBitGc::DoCmdSetOrientation(const TDesC& aSection)
	{
	CFbsBitGc::TGraphicsOrientation orientation;
	TBool ret=EFalse;
	if(CT_GraphicsUtil::GetGraphicsOrientation(*this,aSection,KFldGraphicsOrientation,orientation))
		{
		INFO_PRINTF1(_L("execute SetOrientation(TGraphicsOrientation)"));
		ret=iFbsBitGc->SetOrientation(orientation);
		if(ret!=iGraphicsOrientation[orientation])
			{
			INFO_PRINTF1(_L("SetOrientation and OrientationsAvailable return values are not match!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldGraphicsOrientation());
		SetBlockResult(EFail);
		}
	}

void CT_DataFbsBitGc::DoCmdSetShadowMode(const TDesC& aSection)
	{
	TBool mode;
	if(GetBoolFromConfig(aSection,KFldShadowMode,mode))
		{
		INFO_PRINTF1(_L("execute SetShadowMode(TBool)"));
		iFbsBitGc->SetShadowMode(mode);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetShadowMode(TBool aShadowMode=EFalse)"));
		iFbsBitGc->SetShadowMode();
		}
	}

void CT_DataFbsBitGc::DoCmdSetUserDisplayMode(const TDesC& aSection)
	{
	TDisplayMode mode;
	if(CT_GraphicsUtil::ReadDisplayMode(*this,aSection,KFldDisplayMode,mode))
		{
		INFO_PRINTF1(_L("execute SetUserDisplayMode(TDisplayMode)"));
		iFbsBitGc->SetUserDisplayMode(mode);
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldDisplayMode());
		SetBlockResult(EFail);
		}
	}

void CT_DataFbsBitGc::DoCmdShadowArea(const TDesC& aSection)
	{
	RRegion reg;
	if(GetRegionFromConfig(aSection,KFldRegion,reg))
		{
		INFO_PRINTF1(_L("execute ShadowArea(const TRegion *)"));
		iFbsBitGc->ShadowArea(&reg);
		}
	else
		{
		INFO_PRINTF2(_L("Missing parameter %S"), &KFldRegion());
		INFO_PRINTF1(_L("execute ShadowArea(NULL)"));
		iFbsBitGc->ShadowArea(NULL);
		}
	}

void CT_DataFbsBitGc::DoCmdUseBrushPatternL(const TDesC& aSection)
	{
	TInt handle;
	if(GetIntFromConfig(aSection,KFldHandle,handle))
		{
		INFO_PRINTF1(_L("execute UseBrushPattern(TInt) using the specified handle."));
		TInt err=iFbsBitGc->UseBrushPattern(handle);
		if(KErrNone!=err)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	else
		{
		CFbsBitmap*	bitmap=NULL;
		if (CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap))
			{
			TBool usehandle;
			if(GetBoolFromConfig(aSection,KFldUseHandle,usehandle))
				{
				INFO_PRINTF1(_L("execute UseBrushPattern(TInt) using the bitmap object's handle."));
				TInt err=iFbsBitGc->UseBrushPattern(bitmap->Handle());
				if(KErrNone!=err)
					{
					ERR_PRINTF2(KLogError, err);
					SetError(err);
					}
				}
			else
				{
				INFO_PRINTF1(_L("execute UseBrushPattern(CFbsBitmap*)"));
				iFbsBitGc->UseBrushPattern(bitmap);
				}
			}
		else
			{
			ERR_PRINTF2(_L("Missing parameter %S"), &KFldBitmap());
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsBitGc::DoCmdUseFontL(const TDesC& aSection)
	{
	TInt handle;
	if(GetIntFromConfig(aSection,KFldHandle,handle))
		{
		INFO_PRINTF1(_L("execute UseFont(TInt) using the specified handle"));
		TInt err=iFbsBitGc->UseFont(handle);
		if(KErrNone!=err)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	else
		{
		CFont* font=NULL;
		if(CT_GraphicsUtil::GetFontL(*this,aSection,KFldFont,font))
			{
			TBool usehandle;
			if(GetBoolFromConfig(aSection,KFldUseHandle,usehandle))
				{
				INFO_PRINTF1(_L("execute UseFont(TInt) using the font object's handle"));
				TInt err=iFbsBitGc->UseFont(((CFbsFont*)font)->Handle());
				if(KErrNone!=err)
					{
					ERR_PRINTF2(KLogError, err);
					SetError(err);
					}
				}
			else
				{
				INFO_PRINTF1(_L("execute UseFont(CFont*)"));
				iFbsBitGc->UseFont(font);
				}
			}
		else
			{
			ERR_PRINTF2(_L("Missing parameter %S"), &KFldFont());
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFbsBitGc::DoCmdUseFontNoDuplicateL(const TDesC& aSection)
	{
	CFont* font=NULL;
	if(CT_GraphicsUtil::GetFontL(*this,aSection,KFldFont,font))
		{
		INFO_PRINTF1(_L("execute UseFontNoDuplicate(const CFbsBitGcFont*)"));
		iFbsBitGc->UseFontNoDuplicate((CFbsBitGcFont*)font);
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldFont());
		SetBlockResult(EFail);
		}
	}

void  CT_DataFbsBitGc::DoCmdPromptMessageL(const TDesC& aSection)
	{
	TPtrC text;
	//get prompt text to display in console in order to help user verify. 
	if(!GetStringFromConfig(aSection, KFldPromptText(), text))
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldPromptText());
		SetBlockResult(EFail);
		}
	TInt expectedKey;
	if(!GetIntFromConfig(aSection, KFldExpectedKey(), expectedKey))
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldExpectedKey());
		SetBlockResult(EFail);
		}
	TInt actualKey;
	RProperty property;
	const TUid KMyPropertyCat={0x10012345};
	TUint key=1;
	TSecurityPolicy read(TSecurityPolicy::EAlwaysPass); 
	TSecurityPolicy write(TSecurityPolicy::EAlwaysPass); 
	TInt err=property.Define(KMyPropertyCat,key,RProperty::EInt,read,write);
	if (err==KErrAlreadyExists)
		{
		ERR_PRINTF1(_L("RProperty has existed!"));
		SetBlockResult(EFail);
		User::LeaveIfError(err);
		}
	err=property.Attach(KMyPropertyCat,key);
	TRequestStatus substatus;
	//issue a request to get property change information.
	property.Subscribe(substatus);
	RProcess promptProcess;
	err = promptProcess.Create(KPromptProcess, text);
	if (err == KErrNone)
		{
		CleanupClosePushL(promptProcess);
		TRequestStatus promptStatus;
		promptProcess.Rendezvous(promptStatus);
		if (promptStatus == KRequestPending)
			{
			promptProcess.Resume();
			User::WaitForRequest(promptStatus);
			}
		else
			{
			promptProcess.RendezvousCancel(promptStatus);
			promptProcess.Kill(0);
			ERR_PRINTF2(_L("Executing of prompt process failed with error %d"), promptStatus.Int());
			SetBlockResult(EFail);
			}
		CleanupStack::PopAndDestroy();
		}
	else
		{
		ERR_PRINTF2(_L("Failed to create prompt process with error %d"), err);
		SetBlockResult(EFail);
		}
	//get property change information. 
	User::WaitForRequest(substatus);
	if (substatus!=KRequestPending)
		{
		property.Get(KMyPropertyCat,key,actualKey);
		INFO_PRINTF2(_L("property.Get %d"), actualKey);
		}
		err=RProperty::Delete(KMyPropertyCat,key);
		if (err!=KErrNotFound)
		User::LeaveIfError(err);
		property.Close();
		if(actualKey != expectedKey)
		{
		ERR_PRINTF3(_L("expectedKey=%d pressed key=%d"), expectedKey, actualKey);
		SetBlockResult(EFail);
		}
	}
