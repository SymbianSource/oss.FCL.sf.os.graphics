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

This contains CT_DataFbsFont
*/

#include "T_DataFbsFont.h"

/*@{*/
///	Parameters
_LIT(KText,									"text");
_LIT(KExpectedBool,							"expected_bool");
_LIT(KCharCode,								"char_code");
_LIT(KNullBitmapPointerExpected,			"null_bitmap_pointer_expected");

///	Commands
_LIT(KCmdGetFontMetrics,					"GetFontMetrics");
_LIT(KCmdTextWidthInPixelsWithCharWidth,	"TextWidthInPixelsWithCharWidth");
_LIT(KCmdHandle,							"Handle");
_LIT(KCmdCharacterMetrics,					"CharacterMetrics");
_LIT(KCmdRawTextWidthInPixels,				"RawTextWidthInPixels");
_LIT(KCmdGetFaceAttrib,						"GetFaceAttrib");
_LIT(KCmdIsOpenFont,						"IsOpenFont");
_LIT(KCmdHasCharacter,						"HasCharacter");
/*@}*/

/**
* Constructor
*/
CT_DataFbsFont* CT_DataFbsFont::NewL()
	{
	CT_DataFbsFont*	ret = new (ELeave) CT_DataFbsFont();
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsFont::CT_DataFbsFont()
:	CT_DataFont()
,	iFbsFont(NULL)
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
TBool CT_DataFbsFont::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret=ETrue;

	if (aCommand == KCmdGetFontMetrics)
		{
		DoCmdGetFontMetrics(aSection);
		}
	else if (aCommand == KCmdTextWidthInPixelsWithCharWidth)
		{
		DoCmdTextWidthInPixelsWithCharWidth(aSection);
		}
	else if (aCommand == KCmdHandle)
		{
		DoCmdHandle();
		}
	else if (aCommand == KCmdCharacterMetrics)
		{
		DoCmdCharacterMetrics(aSection);
		}
	else if (aCommand == KCmdRawTextWidthInPixels)
		{
		DoCmdRawTextWidthInPixels(aSection);
		}
	else if (aCommand == KCmdGetFaceAttrib)
		{
		DoCmdGetFaceAttrib(aSection);
		}
	else if (aCommand == KCmdIsOpenFont)
		{
		DoCmdIsOpenFont(aSection);
		}
	else if (aCommand == KCmdHasCharacter)
		{
		DoCmdHasCharacter(aSection);
		}
	else
		{
		ret=CT_DataFont::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


CFont* CT_DataFbsFont::GetFont() const
	{
	return iFbsFont;
	}


/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsFont::GetObject()
	{
	return iFbsFont;
	}


/**
* Sets a fbs font
*
* @param aNewFont	fbs font
*/
void CT_DataFbsFont::SetObjectL(TAny* aObject)
	{
	// no clean up needed, because fonts are managed by server side
	iFbsFont = static_cast<CFbsFont*>(aObject);
	}


/**
* Disown a fbs font
*
*/
void CT_DataFbsFont::DisownObjectL()
	{
	// no clean up needed, because fonts are managed by server side
	iFbsFont=NULL;
	}


/** Calls CFbsFont::GetFontMetrics() */
void CT_DataFbsFont::DoCmdGetFontMetrics(const TDesC& aSection)
	{
	TOpenFontMetrics	metrics;
	TBool				actual = iFbsFont->GetFontMetrics(metrics);
	INFO_PRINTF2(_L("Calls CFbsFont::GetFontMetrics() %d"), actual);

	// get if true expected flag from parameters
	TBool	expected;
	if ( GetBoolFromConfig(aSection, KExpectedBool(), expected) )
		{
		// check that the value is as expected
		if (actual != expected)
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFbsFont::TextWidthInPixels() using SCharWidth output paramter */
void CT_DataFbsFont::DoCmdTextWidthInPixelsWithCharWidth(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsFont::TextWidthInPixels() using SCharWidth output paramter"));

	// get text from parameters
	TPtrC	text;
	if (!GetStringFromConfig(aSection, KText(), text))
		{
		ERR_PRINTF2(_L("No %S"), &KText());
		SetBlockResult(EFail);
		}
	else
		{
		//	call TextWidthInPixels()
		SCharWidth	charWidth;
		iFbsFont->TextWidthInPixels(text, charWidth);

		// validate SCharWidth
		if (charWidth.iMove != charWidth.iLeftAdjust + charWidth.iWidth + charWidth.iRightAdjust)
			{
			ERR_PRINTF5(_L("Move is not equal to left adjust + width + right adjust!, move: %d, leftAdjust: %d, width: %d, rightAdjust: %d"), charWidth.iMove, charWidth.iLeftAdjust, charWidth.iWidth, charWidth.iRightAdjust);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFbsFont::Handle() */
void CT_DataFbsFont::DoCmdHandle()
	{
	// call Handle()
	TInt	handle = iFbsFont->Handle();
	INFO_PRINTF2(_L("Calls CFbsFont::Handle() %d"), handle);

	// validate hanle
	if (handle == 0)
		{
		ERR_PRINTF1(_L("Handle is zero!"));
		SetBlockResult(EFail);
		}
	}


/** Calls CFbsFont::CharacterMetrics() */
void CT_DataFbsFont::DoCmdCharacterMetrics(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsFont::CharacterMetrics()"));

	// get character code from parameters
	TInt	charCode = 0;
	if(!GetIntFromConfig(aSection, KCharCode(), charCode))
		{
		ERR_PRINTF2(_L("No %S"), &KCharCode());
		SetBlockResult(EFail);
		}
	else
		{
		// call CharacterMetrics()
		const TUint8*	bitmapPointer;
		iFbsFont->CharacterMetrics(charCode, bitmapPointer);

	   	// get if NULL bitmap pointer expected flag from parameters
	   	TBool	expectedNull = EFalse;
	   	if(GetBoolFromConfig(aSection, KNullBitmapPointerExpected(), expectedNull))
	   		{
			// check that on retirn the bitmap pointer is not NULL
			if ( expectedNull )
				{
				if ( bitmapPointer!=NULL )
					{
					ERR_PRINTF1(_L("Bitmap pointer is NOT null!"));
					SetBlockResult(EFail);
					}
				}
			else
				{
				if ( bitmapPointer==NULL )
					{
					ERR_PRINTF1(_L("Bitmap pointer is null!"));
					SetBlockResult(EFail);
					}
				}
			}
		}
	}


/** Calls CFbsFont::RawTextWidthInPixels() */
void CT_DataFbsFont::DoCmdRawTextWidthInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsFont::RawTextWidthInPixels()"));

	// get text from parameters
	TPtrC	text;
	if (!GetStringFromConfig(aSection, KText(), text))
		{
		ERR_PRINTF2(_L("No %S"), &KText());
		SetBlockResult(EFail);
		}
	else
		{
		// call RawTextWidthInPixels()
		TInt	width = iFbsFont->RawTextWidthInPixels(text);

		// check result against MeasureText()
		TInt	measureTextWidth = iFbsFont->MeasureText(text);
		if ( width != measureTextWidth)
			{
			ERR_PRINTF3(_L("The result of RawTextWidthInPixels() is not the same as for MeasureText(): %d, and %d"), width, measureTextWidth);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFbsFont::GetFaceAttrib() */
void CT_DataFbsFont::DoCmdGetFaceAttrib(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsFont::GetFaceAttrib()"));

	// call GetFaceAttrib()
	TOpenFontFaceAttrib	attrib;
	TBool 				actual = iFbsFont->GetFaceAttrib(attrib);

	// get if true expected flag from parameters
	TBool 	expected;
	if(GetBoolFromConfig(aSection, KExpectedBool(), expected))
		{
		// check that the value is as expected
		if (actual != expected)
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFbsFont::IsOpenFont() */
void CT_DataFbsFont::DoCmdIsOpenFont(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsFont::IsOpenFont()"));

	// call IsOpenFont()
	TBool	actual = iFbsFont->IsOpenFont();

	// check that the value is as expected if there is a expected bool parameter
	TBool	expected;
	if (GetBoolFromConfig(aSection, KExpectedBool(), expected) )
		{
		// check that the value is as expected
		if (actual != expected)
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFbsFont::HasCharacter() */
void CT_DataFbsFont::DoCmdHasCharacter(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsFont::HasCharacter()"));

	// get character code from parameters
	TInt	charCode = 0;
	if(!GetIntFromConfig(aSection, KCharCode(), charCode))
		{
		ERR_PRINTF2(_L("No %S"), &KCharCode());
		SetBlockResult(EFail);
		}
	else
		{
		// call HasCharacter()
		TBool	actual = iFbsFont->HasCharacter(charCode);

		TBool	expected;
		if(GetBoolFromConfig(aSection, KExpectedBool(), expected))
			{
		// check that the value is as expected
			if (actual != expected)
				{
				ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expected, actual);
				SetBlockResult(EFail);
				}
			}
		}
	}
