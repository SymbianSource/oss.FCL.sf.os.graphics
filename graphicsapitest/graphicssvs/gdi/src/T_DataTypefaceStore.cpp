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

This contains CT_DataTypefaceStore
*/

#include "T_DataTypefaceStore.h"

/*@{*/
///	Parameters
_LIT(KFont,											"font");
_LIT(KFontDataObjectName,							"font_data_object_name");
_LIT(KFontMaxHeight,								"font_max_height");
_LIT(KTypefaceIndex,								"typeface_index");
_LIT(KHeightIndex,									"height_index");
_LIT(KTypefaces,									"typefaces");
_LIT(KCheckHeightPositive,							"check_height_positive");

///	Commands
_LIT(KCmdFontHeightInTwips,					 		"FontHeightInTwips");
_LIT(KCmdGetNearestFontInTwips,						"GetNearestFontInTwips");
_LIT(KCmdGetNearestFontToDesignHeightInTwips,		"GetNearestFontToDesignHeightInTwips");
_LIT(KCmdGetNearestFontToMaxHeightInTwips,			"GetNearestFontToMaxHeightInTwips");
_LIT(KCmdNumTypefaces, 			 					"NumTypefaces");
_LIT(KCmdReleaseFont,								"ReleaseFont");
_LIT(KCmdTypefaceSupport,				 			"TypefaceSupport");

///	Logging
_LIT(KLogError,										"Error=%d");
_LIT(KLogMissingParameter,							"Missing parameter '%S'");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataTypefaceStore::CT_DataTypefaceStore()
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
TBool CT_DataTypefaceStore::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdGetNearestFontInTwips)
		{
		DoCmdGetNearestFontInTwipsL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToMaxHeightInTwips)
		{
		DoCmdGetNearestFontToMaxHeightInTwipsL(aSection);
		}
	else if (aCommand == KCmdNumTypefaces)
		{
		DoCmdNumTypefaces(aSection);
		}
	else if (aCommand == KCmdTypefaceSupport)
		{
		DoCmdTypefaceSupport(aSection);
		}
	else if (aCommand == KCmdFontHeightInTwips)
		{
		DoCmdFontHeightInTwips(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToDesignHeightInTwips)
		{
		DoCmdGetNearestFontToDesignHeightInTwipsL(aSection);
		}
	else if (aCommand == KCmdReleaseFont)
		{
		DoCmdReleaseFontL(aSection);
		}
	else
		{
		retVal = EFalse;
		}

	return retVal;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////


/** Call CTypefaceStore::GetNearestFontInTwips() */
void CT_DataTypefaceStore::DoCmdGetNearestFontInTwipsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CTypefaceStore::GetNearestFontInTwips()"));

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFont, fontSpec) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFont());
		SetBlockResult(EFail);
		}
	else
		{
		// call CTypefaceStore::GetNearestFontInTwips()
		CFont*		font = NULL;
		TInt		err = GetTypefaceStore()->GetNearestFontInTwips(font, fontSpec);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			// set font data object of provided
			SetFontDataObjectIfPassedAsParameterL(font, aSection);
			}
		}
	}


/** Call CTypefaceStore::GetNearestFontToDesignHeightInTwips() */
void CT_DataTypefaceStore::DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CTypefaceStore::GetNearestFontToDesignHeightInTwips()"));

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFont, fontSpec) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFont());
		SetBlockResult(EFail);
		}
	else
		{
		// call CTypefaceStore::GetNearestFontToDesignHeightInTwips()
		CFont*		font = NULL;
		TInt		err = GetTypefaceStore()->GetNearestFontToDesignHeightInTwips(font, fontSpec);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			// set font data object of provided
			SetFontDataObjectIfPassedAsParameterL(font, aSection);
			}
		}
	}


/** Call CTypefaceStore::GetNearestFontToMaxHeightInTwips() */
void CT_DataTypefaceStore::DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CTypefaceStore::GetNearestFontToMaxHeightInTwips()"));

	TBool		dataOk=ETrue;

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFont, fontSpec) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFont());
		SetBlockResult(EFail);
		}

	// get font maximum height from parameters
	TInt	fontMaxHeight;
	if(!GetIntFromConfig(aSection, KFontMaxHeight(), fontMaxHeight))
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFontMaxHeight());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// call CTypefaceStore::GetNearestFontToMaxHeightInTwips()
		CFont*		font = NULL;
		TInt		err = GetTypefaceStore()->GetNearestFontToMaxHeightInTwips(font, fontSpec, fontMaxHeight);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Get nearest font to max height in twips error: %d"), err);
			SetError(err);
			}
		else
			{
			// set font data object of provided
			SetFontDataObjectIfPassedAsParameterL(font, aSection);

			// check that the height of the returned font is not greater that the maximum specified
			TFontSpec	fontSpecInTwips = font->FontSpecInTwips();
			TInt		actualHeight = fontSpecInTwips.iHeight;
			if (actualHeight > fontMaxHeight)
				{
				ERR_PRINTF3(_L("Font doesn't match in maximum specified, height: %d, maximum: %d"), actualHeight, fontMaxHeight);
				SetBlockResult(EFail);
				}
			}
		}
	}


/** Call CTypefaceStore::NumTypefaces() */
void CT_DataTypefaceStore::DoCmdNumTypefaces(const TDesC& aSection)
	{
	TInt	actual=GetTypefaceStore()->NumTypefaces();
	INFO_PRINTF2(_L("Call CTypefaceStore::NumTypefaces() %d"), actual);
	TInt	expected;
	if ( GetIntFromConfig(aSection, KTypefaces(), expected) )
		{
		if (  actual != expected )
			{
			ERR_PRINTF3(_L("Actual typfaces %d does not match expected %d"), actual, expected);
			SetBlockResult(EFail);
			}
		}
	}


/** Call CTypefaceStore::TypefaceSupport() for the given index */
void CT_DataTypefaceStore::DoCmdTypefaceSupport(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CTypefaceStore::TypefaceSupport() for the given index"));
	// get typeface index from parameters
	TInt	typefaceIndex;
	if(!GetIntFromConfig(aSection, KTypefaceIndex(), typefaceIndex))
		{
		ERR_PRINTF2(_L("No %S"), &KTypefaceIndex());
		SetBlockResult(EFail);
		}
	else
		{
		// call TypefaceSupport()
		TTypefaceSupport	typefaceSupport;
		GetTypefaceStore()->TypefaceSupport(typefaceSupport, typefaceIndex);
		INFO_PRINTF2(_L("iIsScalable       %d"), typefaceSupport.iIsScalable);
		INFO_PRINTF2(_L("iMaxHeightInTwips %d"), typefaceSupport.iMaxHeightInTwips);
		INFO_PRINTF2(_L("iMinHeightInTwips %d"), typefaceSupport.iMinHeightInTwips);
		INFO_PRINTF2(_L("iNumHeights       %d"), typefaceSupport.iNumHeights);
		}
	}


/** Call CTypefaceStore::FontHeightInTwips() */
void CT_DataTypefaceStore::DoCmdFontHeightInTwips(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CTypefaceStore::FontHeightInTwips()"));
	TBool								dataOk=ETrue;

	// get typeface index from parameters
	TInt	typefaceIndex;
	if(!GetIntFromConfig(aSection, KTypefaceIndex(), typefaceIndex))
		{
		ERR_PRINTF2(_L("No %S"), &KTypefaceIndex());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get height index from parameters
	TInt	heightIndex;
	if(!GetIntFromConfig(aSection, KHeightIndex(), heightIndex))
		{
		ERR_PRINTF2(_L("No %S"), &KHeightIndex());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		TInt	typefaceHeight = GetTypefaceStore()->FontHeightInTwips(typefaceIndex, heightIndex);
		INFO_PRINTF2(_L("Typeface height %d"), typefaceHeight);

		// get positive height check flag from parameters
		TBool	checkHeightPositive = EFalse;
		GetBoolFromConfig(aSection, KCheckHeightPositive(), checkHeightPositive);

		// check that typeface height is positive
		if ( checkHeightPositive && (typefaceHeight<=0) )
			{
			ERR_PRINTF2(_L("Typeface height is not greater than 0, height: %d"), typefaceHeight);
			SetBlockResult(EFail);
			}
		}
	}


/** CTypefaceStore::ReleaseFont() call */
void CT_DataTypefaceStore::DoCmdReleaseFontL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("CTypefaceStore::ReleaseFont() call"));

	CT_DataFont*	fontDataObject=GetFontDataObjectFromParameterL(KFontDataObjectName(), aSection);

	if (fontDataObject == NULL)
		{
		ERR_PRINTF1(_L("Font data wrapper was not found"));
		SetBlockResult(EFail);
		}
	else
		{
		GetTypefaceStore()->ReleaseFont(static_cast<CFont*>(fontDataObject->GetObject()));
		}
	}


///////////////////////////////// UTIL METHODS //////////////////////////////////////


/**
* Utility method that sets CT_DataFont with a new font if the name of the data object was passed as a parameter from INI-file.
*/
void CT_DataTypefaceStore::SetFontDataObjectIfPassedAsParameterL(CFont* aFont, const TDesC& aSection)
	{
	// fetch data object
	CT_DataFont*	fontDataObject = GetFontDataObjectFromParameterL(KFontDataObjectName(), aSection);
	if ( fontDataObject != NULL )
		{
		// set obtained font for the data object
		fontDataObject->SetObjectL(aFont);
		}
	}


/**
* Utility method that fetches CT_DataFont pointer by command parameter name from INI-file.
*/
CT_DataFont* CT_DataTypefaceStore::GetFontDataObjectFromParameterL(const TDesC& aParameterName, const TDesC& aSection)
	{
	CT_DataFont*	ret = NULL;

	// get FontData object from parameters
	TPtrC	fontDataObjectName;
	if ( GetStringFromConfig(aSection, aParameterName, fontDataObjectName) )
		{
		// check that the data object was found
		ret = static_cast<CT_DataFont*>(GetDataWrapperL(fontDataObjectName));
		}

	return ret;
	}
