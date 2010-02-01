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

This contains CT_DataFont
*/

//	User includes
#include "T_DataFont.h"

//	Epoc includes
#include <gdi.h>

/*@{*/
///	Parameters
_LIT(KText,									"text");
_LIT(KWidthAvailable,						"width_available");
_LIT(KCalculateExcess,						"calculate_excess");
_LIT(KExpectedExcess,						"expected_excess");
_LIT(KExpectedResult,						"expected_result");
_LIT(KExpectedFont,							"expected_font");	
_LIT(KCharCode,								"char_code");
_LIT(KUid,									"uid");
_LIT(KMeasureTextOutput,					"measure_text_output");
_LIT(KMeasureTextInput,						"measure_text_input");

///	Commands
_LIT(KCmdFontCapitalAscent,					"FontCapitalAscent");
_LIT(KCmdFontMaxAscent,						"FontMaxAscent");
_LIT(KCmdFontStandardDescent,				"FontStandardDescent");
_LIT(KCmdFontMaxDescent,					"FontMaxDescent");
_LIT(KCmdFontLineGap,						"FontLineGap");
_LIT(KCmdFontMaxHeight,						"FontMaxHeight");
_LIT(KCmdTypeUid,							"TypeUid");
_LIT(KCmdHeightInPixels,					"HeightInPixels");
_LIT(KCmdAscentInPixels,					"AscentInPixels");
_LIT(KCmdDescentInPixels,					"DescentInPixels");
_LIT(KCmdCharWidthInPixels,					"CharWidthInPixels");
_LIT(KCmdTextCount,							"TextCount");
_LIT(KCmdMaxCharWidthInPixels,				"MaxCharWidthInPixels");
_LIT(KCmdMaxNormalCharWidthInPixels,		"MaxNormalCharWidthInPixels");
_LIT(KCmdFontSpecInTwips,					"FontSpecInTwips");
_LIT(KCmdWidthZeroInPixels,					"WidthZeroInPixels");
_LIT(KCmdBaselineOffsetInPixels,			"BaselineOffsetInPixels");
_LIT(KCmdGetCharacterData,					"GetCharacterData");
_LIT(KCmdMeasureText,						"MeasureText");
_LIT(KCmdExtendedFunction,					"ExtendedFunction");
_LIT(KCmdTextWidthInPixels,					"TextWidthInPixels");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataFont::CT_DataFont()
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
TBool CT_DataFont::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool ret=ETrue;

	if (aCommand == KCmdFontCapitalAscent)
		{
		DoCmdFontCapitalAscent(aSection);
		}
	else if (aCommand == KCmdFontMaxAscent)
		{
		DoCmdFontMaxAscent(aSection);
		}
	else if (aCommand == KCmdFontStandardDescent)
		{
		DoCmdFontStandardDescent(aSection);
		}
	else if (aCommand == KCmdFontMaxDescent)
		{
		DoCmdFontMaxDescent(aSection);
		}
	else if (aCommand == KCmdFontLineGap)
		{
		DoCmdFontLineGap(aSection);
		}
	else if (aCommand == KCmdFontMaxHeight)
		{
		DoCmdFontMaxHeight(aSection);
		}
	else if (aCommand == KCmdTypeUid)
		{
		DoCmdTypeUid(aSection);
		}
	else if (aCommand == KCmdHeightInPixels)
		{
		DoCmdHeightInPixels(aSection);
		}
	else if (aCommand == KCmdAscentInPixels)
		{
		DoCmdAscentInPixels(aSection);
		}
	else if (aCommand == KCmdDescentInPixels)
		{
		DoCmdDescentInPixels(aSection);
		}
	else if (aCommand == KCmdCharWidthInPixels)
		{
		DoCmdCharWidthInPixels(aSection);
		}
	else if (aCommand == KCmdTextCount)
		{
		DoCmdTextCount(aSection);
		}
	else if (aCommand == KCmdMaxCharWidthInPixels)
		{
		DoCmdMaxCharWidthInPixels(aSection);
		}
	else if (aCommand == KCmdMaxNormalCharWidthInPixels)
		{
		DoCmdMaxNormalCharWidthInPixels(aSection);
		}
	else if (aCommand == KCmdFontSpecInTwips)
		{
		DoCmdFontSpecInTwips(aSection);
		}
	else if (aCommand == KCmdWidthZeroInPixels)
		{
		DoCmdWidthZeroInPixels(aSection);
		}
	else if (aCommand == KCmdBaselineOffsetInPixels)
		{
		DoCmdBaselineOffsetInPixels(aSection);
		}
	else if (aCommand == KCmdGetCharacterData)
		{
		DoCmdGetCharacterData(aSection);
		}
	else if (aCommand == KCmdMeasureText)
		{
		DoCmdMeasureTextL(aSection);
		}
	else if (aCommand == KCmdExtendedFunction)
		{
		DoCmdExtendedFunction(aSection);
		}
	else if (aCommand == KCmdTextWidthInPixels)
		{
		DoCmdTextWidthInPixels(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////


/** Calls CFont::TextCount() */
void CT_DataFont::DoCmdTextCount(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::TextCount()"));
	// get text from parameters
	TPtrC	text;
	if ( !GetStringFromConfig(aSection, KText(), text) )
		{
		ERR_PRINTF2(_L("No %S"), &KText());
		SetBlockResult(EFail);
		}

	// get available width from parameters
	TInt	widthAvailable = 0;
	if(!GetIntFromConfig(aSection, KWidthAvailable(), widthAvailable))
		{
		ERR_PRINTF2(_L("No %S"), &KWidthAvailable());
		SetBlockResult(EFail);
		}

	// get if to use version of TextCount() that calculates excess from parameters
	TBool	calculateExcess = EFalse;
	GetBoolFromConfig(aSection, KCalculateExcess(), calculateExcess);

	// call TextCount()
	TInt	result = KErrNone;
	if ( calculateExcess )
		{
		TInt	excess = 0;
		result = GetFont()->TextCount(text, widthAvailable, excess);

		// get expected excess from parameters
		TInt	expectedExcess = 0;
		if ( GetIntFromConfig(aSection, KExpectedExcess(), expectedExcess) )
			{
			ERR_PRINTF1(_L("An expected excess should not be provided if calculate excess is set to false."));
			SetBlockResult(EFail);
			}
		}
	else
		{
		result = GetFont()->TextCount(text, widthAvailable);
		}

	// get expected result from parameters
	TInt	expectedResult = 0;
	if ( GetIntFromConfig(aSection, KExpectedResult(), expectedResult) )
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::TextWidthInPixels() */
void CT_DataFont::DoCmdTextWidthInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::TextWidthInPixels()"));
	// get text from parameters
	TPtrC	text;
	if (!GetStringFromConfig(aSection, KText(), text))
		{
		ERR_PRINTF2(_L("No %S"), &KText());
		SetBlockResult(EFail);
		}

	// call TextWidthInPixels()
	TInt	result = GetFont()->TextWidthInPixels(text);

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::CharWidthInPixels() */
void CT_DataFont::DoCmdCharWidthInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::CharWidthInPixels()"));
	// get character code from parameters
	TInt	charCode = 0;
	if(!GetIntFromConfig(aSection, KCharCode(), charCode))
		{
		ERR_PRINTF2(_L("No %S"), &KCharCode());
		SetBlockResult(EFail);
		}

	// call CharWidthInPixels()
	TInt	result = GetFont()->CharWidthInPixels(charCode);

	// get expected result from parameters
	TInt 	expectedResult = 0;
	if ( GetIntFromConfig(aSection, KExpectedResult(), expectedResult) )
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontCapitalAscent() */
void CT_DataFont::DoCmdFontCapitalAscent(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontCapitalAscent()"));
	// call FontCapitalAscent()
	TInt	result = GetFont()->FontCapitalAscent();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if ( GetIntFromConfig(aSection, KExpectedResult(), expectedResult) )
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontMaxAscent() */
void CT_DataFont::DoCmdFontMaxAscent(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontMaxAscent()"));
	// call FontMaxAscent()
	TInt	result = GetFont()->FontMaxAscent();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if ( GetIntFromConfig(aSection, KExpectedResult(), expectedResult) )
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontStandardDescent() */
void CT_DataFont::DoCmdFontStandardDescent(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontStandardDescent()"));
	// call FontStandardDescent()
	TInt	result = GetFont()->FontStandardDescent();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontMaxDescent() */
void CT_DataFont::DoCmdFontMaxDescent(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontMaxDescent()"));
	// call FontMaxDescent()
	TInt	result = GetFont()->FontMaxDescent();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontLineGap() */
void CT_DataFont::DoCmdFontLineGap(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontLineGap()"));
	// call FontLineGap()
	TInt	result = GetFont()->FontLineGap();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontMaxHeight() */
void CT_DataFont::DoCmdFontMaxHeight(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontMaxHeight()"));
	// call FontMaxHeight()
	TInt	result = GetFont()->FontMaxHeight();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::TypeUid() */
void CT_DataFont::DoCmdTypeUid(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::TypeUid()"));
	// call TypeUid()
	TUid	result = GetFont()->TypeUid();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != TUid::Uid(expectedResult))
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result.iUid);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::HeightInPixels() */
void CT_DataFont::DoCmdHeightInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::HeightInPixels()"));
	// call HeightInPixels()
	TInt	result = GetFont()->HeightInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::AscentInPixels() */
void CT_DataFont::DoCmdAscentInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::AscentInPixels()"));
	// call AscentInPixels()
	TInt	result = GetFont()->AscentInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::DescentInPixels() */
void CT_DataFont::DoCmdDescentInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::DescentInPixels()"));
	// call DescentInPixels()
	TInt	result = GetFont()->DescentInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::MaxCharWidthInPixels() */
void CT_DataFont::DoCmdMaxCharWidthInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::MaxCharWidthInPixels()"));
	// call MaxCharWidthInPixels()
	TInt	result = GetFont()->MaxCharWidthInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::MaxNormalCharWidthInPixels() */
void CT_DataFont::DoCmdMaxNormalCharWidthInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::MaxNormalCharWidthInPixels()"));
	// call MaxNormalCharWidthInPixels()
	TInt	result = GetFont()->MaxNormalCharWidthInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::FontSpecInTwips() */
void CT_DataFont::DoCmdFontSpecInTwips(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::FontSpecInTwips()"));

	// call FontSpecInTwips()
	TFontSpec	fontSpec=GetFont()->FontSpecInTwips();
	INFO_PRINTF3(_L("iName	%S, iHeight    %d"), &fontSpec.iTypeface.iName,fontSpec.iHeight);
	
	TFontSpec expectedFontSpec;
	if(GetFontSpecFromConfig(aSection, KExpectedFont(), expectedFontSpec))
		{
		// just check font name
		if( fontSpec.iTypeface.iName!=expectedFontSpec.iTypeface.iName)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected font: %S, actual font: %S"),
					&expectedFontSpec.iTypeface.iName, &fontSpec.iTypeface.iName);
			SetError(-1);			
			}
		}
	}


/** Calls CFont::WidthZeroInPixels() */
void CT_DataFont::DoCmdWidthZeroInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::WidthZeroInPixels()"));
	// call WidthZeroInPixels()
	TInt	result = GetFont()->WidthZeroInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::BaselineOffsetInPixels() */
void CT_DataFont::DoCmdBaselineOffsetInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::BaselineOffsetInPixels()"));
	// call BaselineOffsetInPixels()
	TInt	result = GetFont()->BaselineOffsetInPixels();

	// get expected result from parameters
	TInt	expectedResult = 0;
	if(GetIntFromConfig(aSection, KExpectedResult(), expectedResult))
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFont::GetCharacterData() */
void CT_DataFont::DoCmdGetCharacterData(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::GetCharacterData()"));
	// get character code from parameters
	TInt	charCode = 0;
	if(!GetIntFromConfig(aSection, KCharCode(), charCode))
		{
		ERR_PRINTF2(_L("No %S"), &KCharCode());
		SetBlockResult(EFail);
		}

	// call GetCharacterData()
	TUint								code = charCode;
	TOpenFontCharMetrics				metrics;
	const TUint8*						bitmapPointer;
	TSize								bitmapSize;
	CFont::TCharacterDataAvailability	ret=GetFont()->GetCharacterData(code, metrics, bitmapPointer, bitmapSize);
	INFO_PRINTF2(_L("CharacterDataAvailability %d"), ret);
	INFO_PRINTF2(_L("Height                    %d"), bitmapSize.iHeight);
	INFO_PRINTF2(_L("Width                     %d"), bitmapSize.iWidth);
	}


/** Calls CFont::ExtendedFunction() */
void CT_DataFont::DoCmdExtendedFunction(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFont::ExtendedFunction()"));
	// get function UID from parameters
	TInt	uidInt = 0;
	if(!GetHexFromConfig(aSection, KUid(), uidInt))
		{
		ERR_PRINTF2(_L("No %S"), &KUid());
		SetBlockResult(EFail);
		}

	// call ExtendedFunction()
	TInt	result=GetFont()->ExtendedFunction(TUid::Uid(uidInt), NULL);

	// get expected result from parameters
	TInt	expectedResult = 0;
	if ( GetIntFromConfig(aSection, KExpectedResult(), expectedResult) )
		{
		if (result != expectedResult)
			{
			ERR_PRINTF3(_L("The result is not as expected! expected: %d, actual: %d"), expectedResult, result);
			SetBlockResult(EFail);
			}
		}
	}


/** Calls CFbsFont::MeasureText() */
void CT_DataFont::DoCmdMeasureTextL(const TDesC& aSection)
	{
	CFont::TMeasureTextInput*	measureTextInputPointer = NULL;
	GetMeasureTextInputL(*this, KMeasureTextInput(), aSection, measureTextInputPointer);
	if ( measureTextInputPointer==NULL )
		{
		INFO_PRINTF1(_L("measureTextInputPointer NULL"));
		}

	CFont::TMeasureTextOutput*	measureTextOutputPointer = NULL;
	GetMeasureTextOutputL(*this, KMeasureTextOutput(), aSection, measureTextOutputPointer);
	if ( measureTextOutputPointer==NULL )
		{
		INFO_PRINTF1(_L("measureTextOutputPointer NULL"));
		}

	// get text from parameters
	TPtrC	text;
	if (!GetStringFromConfig(aSection, KText(), text))
		{
		ERR_PRINTF2(_L("No %S"), &KText());
		SetBlockResult(EFail);
		}
	else
		{
		TInt	width=GetFont()->MeasureText(text, measureTextInputPointer, measureTextOutputPointer);
		INFO_PRINTF2(_L("MeasureText %d"), width);
		}
	}


////////////////// UTIL METHODS ////////////////////////


/**
* Utility method that fetches TMeasureTextInput pointer by command parameter name from INI-file.
*/
TBool CT_DataFont::GetMeasureTextInputL(CDataWrapper& aDataWrapper, const TDesC& aParameterName, const TDesC& aSection, CFont::TMeasureTextInput*& aMeasureTextInput)
	{
	// get MeasureTextInputData object from parameters
	TPtrC	measureTextInputDataObjectName;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSection, aParameterName, measureTextInputDataObjectName);
	if ( ret )
		{
		// check that the data object was found
		aMeasureTextInput=static_cast<CFont::TMeasureTextInput*>(aDataWrapper.GetDataObjectL(measureTextInputDataObjectName));
		}

	return ret;
	}


/**
* Utility method that fetches TMeasureTextOutput pointer by command parameter name from INI-file.
*/
TBool CT_DataFont::GetMeasureTextOutputL(CDataWrapper& aDataWrapper, const TDesC& aParameterName, const TDesC& aSection, CFont::TMeasureTextOutput*& aMeasureTextOutput)
	{
	// get MeasureTextOutputData object from parameters
	TPtrC	measureTextOutputDataObjectName;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSection, aParameterName, measureTextOutputDataObjectName);
	if ( ret )
		{
		// check that the data object was found
		aMeasureTextOutput=static_cast<CFont::TMeasureTextOutput*>(aDataWrapper.GetDataObjectL(measureTextOutputDataObjectName));
		}

	return ret;
	}
