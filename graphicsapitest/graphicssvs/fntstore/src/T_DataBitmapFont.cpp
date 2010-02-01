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

This contains CT_DataBitmapFont
*/

#include "T_DataBitmapFont.h"
#include "T_GraphicsUtil.h"

/*@{*/
/// Parameters
_LIT(KFldExpected,								"expected");
_LIT(KFldCode,									"code");
_LIT(KFldIsNull,								"isnull");
_LIT(KFldSessHandle,							"handle");
_LIT(KFldGlyphBitmapType,						"glyphtype");

_LIT(KFldCharMetrics,							"charmetrics");
_LIT(KFldOFCharMetrics,							"ofcharmetrics");
_LIT(KFldOFFaceAttrib,							"offaceattrib");
_LIT(KFldOFMetrics,								"ofmetrics");


/// Commands
_LIT(KCmdBitmapEncoding,						"BitmapEncoding");
_LIT(KCmdCharacterMetrics,						"CharacterMetrics");
_LIT(KCmdCharacterNeedsToBeRasterized,			"CharacterNeedsToBeRasterized");
_LIT(KCmdGetCharacterData,						"GetCharacterData");
_LIT(KCmdGetFaceAttrib,							"GetFaceAttrib");
_LIT(KCmdGetFontMetrics,						"GetFontMetrics");
_LIT(KCmdGlyphBitmapType,						"GlyphBitmapType");
_LIT(KCmdHasCharacterL,							"HasCharacterL");
_LIT(KCmdIsOpenFont,							"IsOpenFont");
_LIT(KCmdOpenFont,								"OpenFont");
_LIT(KCmdRasterize,								"Rasterize");
_LIT(KCmdUniqueFontId,							"UniqueFontId");

///	Constant Literals
_LIT(KTagCharMetricsAscent,						"ascent");
_LIT(KTagCharMetricsHeight,						"height");
_LIT(KTagCharMetricsMove,						"move");
_LIT(KTagCharMetricsLeftAdjust,					"leftadjust");
_LIT(KTagCharMetricsRightAdjust,				"rightadjust");

_LIT(KTagOFCharMetricsHeight,					"height");
_LIT(KTagOFCharMetricsGlyphType,				"glyphtype");

_LIT(KTagOFFaceAttribFamilyName,				"familyname");
_LIT(KTagOFFaceAttribMinSize,					"minsize");

_LIT(KTagOFMetricsMaxHeight,					"maxheight");
_LIT(KTagOFMetricsSize,							"size");

_LIT(KFormatEntryField,		"%S_%S");

/// Logging
_LIT(KLogError,									"Error=%d");
_LIT(KLogMissingParameter,						"Missing parameter '%S'");
_LIT(KLogMissingExpectedValue,					"Missing expected value '%S'");
_LIT(KLogNotExpectedValueInt,					"'%S' is not as expected=%d, actual=%d");
_LIT(KLogNotExpectedValueString,				"'%S' is not as expected='%S', actual='%S'");

/*@}*/


/**
* Two phase constructor
*/
CT_DataBitmapFont* CT_DataBitmapFont::NewL()
	{
	CT_DataBitmapFont* ret = new (ELeave) CT_DataBitmapFont();
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataBitmapFont::CT_DataBitmapFont()
:	CT_DataFont(), iBitmapFont(NULL)
	{
	}

TAny* CT_DataBitmapFont::GetObject()
	{
	return iBitmapFont;
	}

void CT_DataBitmapFont::SetObjectL(TAny* aObject)
	{
	iBitmapFont	= static_cast<CBitmapFont*> (aObject);
	}
		
void CT_DataBitmapFont::DisownObjectL() 
	{
	iBitmapFont = NULL;
	}

CFont* CT_DataBitmapFont::GetFont() const
	{
	return iBitmapFont;
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
TBool CT_DataBitmapFont::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret = ETrue;

	if (aCommand == KCmdBitmapEncoding)
		{
		DoCmdBitmapEncoding(aSection);
		}
	else if (aCommand == KCmdCharacterMetrics)
		{
		DoCmdCharacterMetrics(aSection);
		}
	else if (aCommand == KCmdCharacterNeedsToBeRasterized)
		{
		DoCmdCharacterNeedsToBeRasterized(aSection);
		}
	else if (aCommand == KCmdGetCharacterData)
		{
		DoCmdGetCharacterData(aSection);
		}
	else if (aCommand == KCmdGetFaceAttrib)
		{
		DoCmdGetFaceAttrib(aSection);
		}
	else if (aCommand == KCmdGetFontMetrics)
		{
		DoCmdGetFontMetrics(aSection);
		}
	else if (aCommand == KCmdGlyphBitmapType)
		{
		DoCmdGlyphBitmapType(aSection);
		}
	else if (aCommand == KCmdHasCharacterL)
		{
		DoCmdHasCharacterL(aSection);
		}
	else if (aCommand == KCmdIsOpenFont)
		{
		DoCmdIsOpenFont(aSection);
		}
	else if (aCommand == KCmdOpenFont)
		{
		DoCmdOpenFont(aSection);
		}
	else if (aCommand == KCmdRasterize)
		{
		DoCmdRasterize(aSection);
		}
	else if (aCommand == KCmdUniqueFontId)
		{
		DoCmdUniqueFontId(aSection);
		}
	else
		{
		ret = CT_DataFont::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Calls CBitmapFont::DoCmdBitmapEncoding() */
void CT_DataBitmapFont::DoCmdBitmapEncoding(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::BitmapEncoding()"));
	TInt ret = iBitmapFont->BitmapEncoding();
	INFO_PRINTF2(_L("The actual bitmap encoding value is %d"), ret);
	
	TInt expected;
	if( GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		if (ret != expected)
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, ret, expected);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataBitmapFont::DoCmdCharacterMetrics(const TDesC& aSection)
	{
	TInt code;
	if( !GetIntFromConfig(aSection, KFldCode, code) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCode);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Calls CBitmapFont::CharacterMetrics(TInt, const TUint8*&)"));
		TCharacterMetrics metrics;
		const TUint8* ptr;
		metrics= iBitmapFont->CharacterMetrics(code, ptr);
		TBool failed = EFalse;
		
		TBool isNull;
		if( GetBoolFromConfig(aSection, KFldIsNull, isNull) )
			{
			if(isNull)
				{
				if(ptr != NULL)
					{
					ERR_PRINTF1(_L("Should return a NULL pointer") );
					failed = ETrue;
					}
				else
					{
					INFO_PRINTF1(_L("A NULL pointer is returned"));

					}
				}
			else
				{
				if(ptr == NULL)
					{
					ERR_PRINTF1(_L("Should return a pointer to the character bitmap") );
					failed = ETrue;
					}
				else
					{
					INFO_PRINTF1(_L("A pointer to the character bitmap is returned") );
					}
				}
			}

		TCharacterMetrics expected;
		if( GetCharacterMetricsFromConfig(*this, aSection, KFldCharMetrics, expected) )
			{
			if(metrics.iAscentInPixels != expected.iAscentInPixels)
				{
				
				ERR_PRINTF4(KLogNotExpectedValueInt, &KTagCharMetricsAscent, expected.iAscentInPixels, metrics.iAscentInPixels);
				failed = ETrue;
				}
			if(metrics.iHeightInPixels != expected.iHeightInPixels)
				{
				ERR_PRINTF4(KLogNotExpectedValueInt, &KTagCharMetricsHeight, expected.iHeightInPixels, metrics.iHeightInPixels);
				failed = ETrue;
				}
			if(metrics.iMoveInPixels != expected.iMoveInPixels)
				{				
				ERR_PRINTF4(KLogNotExpectedValueInt, &KTagCharMetricsMove, expected.iMoveInPixels, metrics.iMoveInPixels);	
				failed = ETrue;
				}
			if(metrics.iLeftAdjustInPixels != expected.iLeftAdjustInPixels)
				{
				ERR_PRINTF4(KLogNotExpectedValueInt, &KTagCharMetricsLeftAdjust, expected.iLeftAdjustInPixels, metrics.iLeftAdjustInPixels);	
				failed = ETrue;
				}
			if(metrics.iRightAdjustInPixels != expected.iRightAdjustInPixels)
				{
				ERR_PRINTF4(KLogNotExpectedValueInt, &KTagCharMetricsRightAdjust, expected.iRightAdjustInPixels, metrics.iRightAdjustInPixels);
				failed = ETrue;
				}
			}
		
		if(failed)
			{
			SetBlockResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("Character metrics is expected."));
			}
		}
	}

void CT_DataBitmapFont::CT_DataBitmapFont::DoCmdCharacterNeedsToBeRasterized(const TDesC& aSection)
	{
	TBool dataOK = ETrue;
	
	TInt handle;
	if( !GetIntFromConfig(aSection, KFldSessHandle, handle) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSessHandle);
		dataOK = EFalse;
		}
	
	TInt code;
	if( !GetIntFromConfig(aSection, KFldCode, code) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCode);
		dataOK = EFalse;
		}
	
	if(!dataOK)
		{
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Calls CBitmapFont::CharacterNeedsToBeRasterized(TInt, TInt)"));
		TBool ret = iBitmapFont->CharacterNeedsToBeRasterized(handle, code);
		
		TBool expected;
		if( GetBoolFromConfig(aSection, KFldExpected, expected) )
			{
			if(ret)
				{
				if(!expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: EFalse, actual: ETrue"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Return value is ETrue"));	
					}
				}
			else
				{
				if(expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: ETrue, actual: EFalse"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Return value is EFalse"));
					}
				}
			}
		}
	}


void CT_DataBitmapFont::DoCmdGetCharacterData(const TDesC& aSection)
	{
	TBool dataOK = ETrue;
	
	TInt handle;
	if( !GetIntFromConfig(aSection, KFldSessHandle, handle) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSessHandle);		
		dataOK = EFalse;
		}
	
	TInt code;	
	if( !GetIntFromConfig(aSection, KFldCode, code) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCode);
		dataOK = EFalse;
		}
	
	if(!dataOK)
		{
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Calls CBitmapFont::GetCharacterData(TInt aSessionHandle, TInt aCode, TOpenFontCharMetrics &aMetrics, const TUint8 *&aBitmap)"));
		TOpenFontCharMetrics metrics;
		const TUint8* ptr;
		TBool ret = iBitmapFont->GetCharacterData(handle, code, metrics, ptr);		
		
		TBool expected;
		if( GetBoolFromConfig(aSection, KFldExpected, expected) )
			{
			if(ret)
				{
				if(!expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: EFalse, actual: ETrue"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Return value is ETrue"));
										
					TBool isNull;
					if( GetBoolFromConfig(aSection, KFldIsNull, isNull) )
						{
						if(isNull)
							{
							if(ptr != NULL)
								{
								ERR_PRINTF1(_L("Should return a NULL pointer") );
								SetBlockResult(EFail);
								}
							else
								{
								INFO_PRINTF1(_L("A NULL pointer is returned") );
								}
							}
						else
							{
							if(ptr == NULL)
								{
								ERR_PRINTF1(_L("Should return a pointer to the character bitmap.") );
								SetBlockResult(EFail);
								}
							else
								{
								INFO_PRINTF1(_L("a pointer to the character bitmap is returned") );
								}
							}
						}					
					
					TBool failed = EFalse;
					TOpenFontCharMetrics expectedMetrics;
					if( GetOpenFontCharMetricsFromConfig(*this, aSection, KFldOFCharMetrics, expectedMetrics) )
						{
						if(metrics.GlyphType() != expectedMetrics.GlyphType())
							{
							ERR_PRINTF4(KLogNotExpectedValueInt, &KTagOFCharMetricsGlyphType, metrics.GlyphType(), expectedMetrics.GlyphType());
							failed = ETrue;
							}			
						if(metrics.Height() != expectedMetrics.Height())
							{
							ERR_PRINTF4(KLogNotExpectedValueInt, &KTagOFCharMetricsHeight, metrics.Height(), expectedMetrics.Height());
							failed = ETrue;
							}
						
						if(failed)
							{
							SetBlockResult(EFail);
							}
						else
							{
							INFO_PRINTF1(_L("Open font character metrics is expected"));
							}
						}
					}
				}
			else
				{
				if(expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: ETrue, actual: EFalse"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Return value is EFalse"));
					}
				}
			}
		}	
	}

void CT_DataBitmapFont::DoCmdGetFaceAttrib(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::GetFaceAttrib(TOpenFontFaceAttrib&)"));
	TOpenFontFaceAttrib attrib;
	TBool ret = iBitmapFont->GetFaceAttrib(attrib);
	
	TBool expected;
	if( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if(ret)
			{
			if(!expected)
				{
				ERR_PRINTF1(_L("Return value is not as expected: EFalse, actual: ETrue"));
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("Return value is ETrue"));
				TBool failed = EFalse;				
				TOpenFontFaceAttrib expectedAttrib;
				if( GetOpenFontFaceAttribFromConfig(*this, aSection, KFldOFFaceAttrib, expectedAttrib) )
					{
					TPtrC actualName = attrib.FamilyName();
					TPtrC expectedName = expectedAttrib.FamilyName();
					if(actualName != expectedName)
						{	
						ERR_PRINTF4(KLogNotExpectedValueString, &KTagOFFaceAttribFamilyName, &expectedName, &actualName);
						failed = ETrue;
						}
					if(attrib.MinSizeInPixels() != expectedAttrib.MinSizeInPixels())
						{
						ERR_PRINTF4(KLogNotExpectedValueInt, &KTagOFFaceAttribMinSize, expectedAttrib.MinSizeInPixels(), attrib.MinSizeInPixels());
						failed = ETrue;
						}
					
					if(failed)
						{
						SetBlockResult(EFail);
						}
					else
						{
						INFO_PRINTF1(_L("Open font typeface attributes is expected"));
						}
					}
				}
			}
		else
			{
			if(expected)
				{
				ERR_PRINTF1(_L("Return value is not as expected: ETrue, actual: EFalse"));
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("Return value is EFalse"));
				}
			}
		}
	}

void CT_DataBitmapFont::DoCmdGetFontMetrics(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::GetFontMetrics(TOpenFontMetrics&)"));
	TOpenFontMetrics metrics;
	iBitmapFont->GetFontMetrics(metrics);
	
	TOpenFontMetrics expectedMetrics;
	if( GetOpenFontMetricsFromConfig(*this, aSection, KFldOFMetrics, expectedMetrics) )
		{
		TBool failed = EFalse;
		if(metrics.MaxHeight() != expectedMetrics.MaxHeight())
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KTagOFMetricsMaxHeight, expectedMetrics.MaxHeight(), metrics.MaxHeight());
			failed = ETrue;
			}
		if(metrics.Size() != expectedMetrics.Size())
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KTagOFMetricsSize, expectedMetrics.Size(), metrics.Size());
			failed = ETrue;
			}
		
		if(failed)
			{
			SetBlockResult(EFail);
			}
		else
			{
			INFO_PRINTF1(_L("Open font metrics is expected"));
			}
		}
	}

void CT_DataBitmapFont::DoCmdGlyphBitmapType(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::GlyphBitmapType()"));
	TGlyphBitmapType ret = iBitmapFont->GlyphBitmapType();
	INFO_PRINTF2(_L("Glyph bitmap type is %d."), (TInt)ret);
	
	TGlyphBitmapType expected;
	if( CT_GraphicsUtil::ReadGlyphBitmapType(*this, aSection, KFldGlyphBitmapType(), expected) )
		{
		if(ret != expected)
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, (TInt)ret, (TInt)expected);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataBitmapFont::DoCmdHasCharacterL(const TDesC& aSection)
	{
	TInt code;
	if( !GetIntFromConfig(aSection, KFldCode, code) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCode);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Calls CBitmapFont::HasCharacterL(TInt)"));
		TBool ret = iBitmapFont->HasCharacterL(code);
		
		TBool expected;
		if( GetBoolFromConfig(aSection, KFldExpected, expected) )
			{
			if(ret)
				{
				if(!expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: EFalse, actual: ETrue"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Return value is ETrue"));
					}
				}
			else
				{
				if(expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: ETrue, actual: EFalse"));
					SetBlockResult(EFail);
					}
				else
					{
					INFO_PRINTF1(_L("Return value is EFalse"));
					}
				}
			}
		}
	}

void CT_DataBitmapFont::DoCmdIsOpenFont(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::IsOpenFont()"));
	TBool ret = iBitmapFont->IsOpenFont();
	
	TBool expected;
	if( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if(ret)
			{
			if(!expected)
				{
				ERR_PRINTF1(_L("Return value is not as expected: EFalse, actual: ETrue"));
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("Return value is ETrue"));
				}
			}
		else
			{
			if(expected)
				{
				ERR_PRINTF1(_L("Return value is not as expected: ETrue, actual: EFalse"));
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("Return value is EFalse"));
				}
			}
		}
	}

void CT_DataBitmapFont::DoCmdOpenFont(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::OpenFont()"));
	COpenFont* ptr = iBitmapFont->OpenFont();
	
	TBool isNull;
	if( GetBoolFromConfig(aSection, KFldIsNull, isNull) )
		{
		if(isNull)
			{
			if(ptr != NULL)
				{
				ERR_PRINTF1(_L("Should return a NULL pointer") );
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("A NULL pointer is returned") );
				}
			}
		else
			{
			if(ptr == NULL)
				{
				ERR_PRINTF1(_L("Should return a pointer to COpenFont") );
				SetBlockResult(EFail);
				}
			else
				{
				INFO_PRINTF1(_L("A pointer to COpenFont is returned") );
				}
			}
		}
	}

void CT_DataBitmapFont::DoCmdRasterize(const TDesC& aSection)
	{
	TBool dataOK = ETrue;
	
	TInt handle;
	if( !GetIntFromConfig(aSection, KFldSessHandle, handle) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSessHandle);	
		dataOK = EFalse;
		}
	
	TInt code;
	if( !GetIntFromConfig(aSection, KFldCode, code) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCode);
		dataOK = EFalse;
		}
	
	if(!dataOK)
		{
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("Calls CBitmapFont::Rasterize(TInt, TInt, TOpenFontGlyphData*)"));
		TOpenFontGlyphData* ptr = NULL;
		TBool ret = iBitmapFont->Rasterize(handle, code, ptr);
		
		TBool expected;
		if( GetBoolFromConfig(aSection, KFldExpected, expected) )
			{
			if(ret)
				{
				if(!expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: EFalse, actual: ETrue"));
					SetBlockResult(EFail);
					}
				else
					{
					// The character was successfully rasterized.
					INFO_PRINTF1(_L("Return value is ETrue"));	
					TBool isNull;
					if( GetBoolFromConfig(aSection, KFldIsNull, isNull) )
						{
						if(isNull)
							{
							if(ptr != NULL)
								{
								ERR_PRINTF1(_L("Should return a NULL pointer") );
								SetBlockResult(EFail);
								}
							else
								{
								INFO_PRINTF1(_L("A NULL pointer is returned"));
								}
							}
						else
							{
							if(ptr == NULL)
								{
								ERR_PRINTF1(_L("Should return a pointer to TOpenFontGlyphData") );
								SetBlockResult(EFail);
								}
							else
								{
								INFO_PRINTF1(_L("A pointer to TOpenFontGlyphData is returned"));
								}
							}
						}
					}
				}
			// ret = EFalse
			else
				{
				if(expected)
					{
					ERR_PRINTF1(_L("Return value is not as expected: ETrue, actual: EFalse"));
					SetBlockResult(EFail);				
					}
				else
					{
					INFO_PRINTF1(_L("Return value is EFalse"));
					}
				}
			}
		}
	}

void CT_DataBitmapFont::DoCmdUniqueFontId(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CBitmapFont::UniqueFontId()"));
	TUint32 ret = iBitmapFont->UniqueFontId();
	INFO_PRINTF2(_L("Unique font id is %d"), (TInt)ret);
	
	TInt temp;
	if( GetIntFromConfig(aSection, KFldExpected, temp) )
		{
		TUint32 expected = (TUint32)temp;
		if(ret != expected)
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, ret, expected);
			SetBlockResult(EFail);
			}
		}
	}

TBool CT_DataBitmapFont::GetCharacterMetricsFromConfig(CDataWrapper& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TCharacterMetrics& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TInt temp;
	TBool ret = EFalse;
	
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagCharMetricsAscent);
	if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, temp) )
		{
		aResult.iAscentInPixels = temp;
		ret = ETrue;
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagCharMetricsHeight);
	if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, temp) )
		{
		aResult.iHeightInPixels = temp;
		ret = ETrue;
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagCharMetricsLeftAdjust);
	if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, temp) )
		{
		aResult.iLeftAdjustInPixels = temp;
		ret = ETrue;
		}
	
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagCharMetricsMove);
	if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, temp) )
		{
		aResult.iMoveInPixels = temp;
		ret = ETrue;
		}
	
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagCharMetricsRightAdjust);
	if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, temp) )
		{
		aResult.iRightAdjustInPixels = temp;
		ret = ETrue;
		}
	
	return ret;
	}


TBool CT_DataBitmapFont::GetOpenFontCharMetricsFromConfig(CDataWrapperBase& aDataWrapper,const TDesC& aSectName,const TDesC& aKeyName,TOpenFontCharMetrics& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TGlyphBitmapType type;
	TInt height;
	
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagOFCharMetricsGlyphType);
	if( CT_GraphicsUtil::ReadGlyphBitmapType(aDataWrapper, aSectName, tempStore, type) )
		{		
		tempStore.Format(KFormatEntryField, &aKeyName, &KTagOFCharMetricsHeight);
		if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, height) )
			{
			aResult.SetHeight(height);
			aResult.SetGlyphType(type);
			return ETrue;
			}
		}
	
	return EFalse;
	}

TBool CT_DataBitmapFont::GetOpenFontFaceAttribFromConfig(CDataWrapper& aDataWrapper,const TDesC& aSectName,const TDesC& aKeyName,TOpenFontFaceAttrib& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagOFFaceAttribFamilyName);
	
	TPtrC familyname;
	TInt minsize;
	if( aDataWrapper.GetStringFromConfig(aSectName, tempStore, familyname) )
		{
		tempStore.Format(KFormatEntryField, &aKeyName, &KTagOFFaceAttribMinSize);
		if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, minsize) )
			{
			aResult.SetFamilyName(familyname);						
			aResult.SetMinSizeInPixels(minsize);
			return ETrue;
			}
		}
	
	return EFalse;
	}

TBool CT_DataBitmapFont::GetOpenFontMetricsFromConfig(CDataWrapper& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName, TOpenFontMetrics& aResult)
	{ 
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagOFMetricsMaxHeight);
	
	TInt maxheight;
	TInt size;
	if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, maxheight) )
		{
		tempStore.Format(KFormatEntryField, &aKeyName, &KTagOFMetricsSize);
		if( aDataWrapper.GetIntFromConfig(aSectName, tempStore, size) )
			{
			aResult.SetMaxHeight(maxheight);
			aResult.SetSize(size);
			return ETrue;
			}
		}
	
	return EFalse;
	}

