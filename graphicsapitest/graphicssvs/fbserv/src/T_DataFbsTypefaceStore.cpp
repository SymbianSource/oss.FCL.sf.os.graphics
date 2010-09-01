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

This contains CT_DataFbsTypefaceStore
*/

#include "T_DataFbsTypefaceStore.h"
#include "T_GraphicsUtil.h"


/*@{*/
///	Parameters
_LIT(KAllFonts,										"all_fonts");
_LIT(KDriveName,									"drive_name");
_LIT(KFileName,										"file_name");
_LIT(KFont,											"font");
_LIT(KFontUid,										"font_uid");
_LIT(KFontNameAlias,								"font_name_alias");
_LIT(KFontName,										"font_name");
_LIT(KLanguage,										"language");
_LIT(KTypefaceId,									"typeface_id");
_LIT(KGraphicsDevice,								"graphics_device");
_LIT(KFontMaxHeight,								"font_max_height");
_LIT(KTypefaceIndex,								"typeface_index");
_LIT(KHeightIndex,									"height_index");
_LIT(KUseFileName,									"use_file_name");
_LIT(KUseDriveName,									"use_drive_name");
_LIT(KAlgStyle,										"alg_style");
_LIT(KCheckAgainstMaximum,							"check_against_maximum");
_LIT(KCheckHeightPositive,							"check_height_positive");
_LIT(KGlyphBitmapType,								"glyph_bitmap_type");

///	Commands
_LIT(KCmdNewL,										"NewL");
_LIT(KCmdAddFile,									"AddFile");
_LIT(KCmdInstallFile,								"InstallFile");
_LIT(KCmdRemoveFile,			 					"RemoveFile");
_LIT(KCmdGetNearestFontInPixels,					"GetNearestFontInPixels");
_LIT(KCmdGetNearestFontToDesignHeightInPixels,		"GetNearestFontToDesignHeightInPixels");
_LIT(KCmdGetNearestFontToMaxHeightInPixels,			"GetNearestFontToMaxHeightInPixels");
_LIT(KCmdGetFontById, 			 					"GetFontById");
_LIT(KCmdFontHeightInPixels,						"FontHeightInPixels");
_LIT(KCmdDefaultBitmapType, 						"DefaultBitmapType");
_LIT(KCmdSetDefaultBitmapType, 						"SetDefaultBitmapType");
_LIT(KCmdSetFontNameAliasL,							"SetFontNameAliasL");
_LIT(KCmdSetDefaultLanguageForMetrics,				"SetDefaultLanguageForMetrics");
_LIT(KCmdRemoveFontFileLocksL,						"RemoveFontFileLocksL");
_LIT(KCmdSetSystemDefaultTypefaceName,				"SetSystemDefaultTypefaceNameL");
_LIT(KCmdDestructor,								"~");

///	Logging
_LIT(KLogError,										"Error=%d");
_LIT(KLogMissingParameter,							"Missing parameter '%S'");
/*@}*/


/**
* Two phase constructor
*/
CT_DataFbsTypefaceStore* CT_DataFbsTypefaceStore::NewL()
	{
	CT_DataFbsTypefaceStore* ret = new (ELeave) CT_DataFbsTypefaceStore();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataFbsTypefaceStore::CT_DataFbsTypefaceStore()
:	CT_DataTypefaceStore()
,	iFbsTypefaceStore(NULL)
,	iLastTypefaceFileId(0)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataFbsTypefaceStore::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataFbsTypefaceStore::~CT_DataFbsTypefaceStore()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataFbsTypefaceStore::DestroyData()
	{
	delete iFbsTypefaceStore;
	iFbsTypefaceStore = NULL;
	}


/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsTypefaceStore::GetObject()
	{
	return iFbsTypefaceStore;
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
TBool CT_DataFbsTypefaceStore::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret=ETrue;

	if (aCommand == KCmdNewL)
		{
		DoCmdNewL(aSection);
		}
	else if (aCommand == KCmdAddFile)
		{
		DoCmdAddFile(aSection);
		}
	else if (aCommand == KCmdInstallFile)
		{
		DoCmdInstallFile(aSection);
		}
	else if (aCommand == KCmdRemoveFile)
		{
		DoCmdRemoveFile(aSection);
		}
	else if (aCommand == KCmdGetNearestFontInPixels)
		{
		DoCmdGetNearestFontInPixelsL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToDesignHeightInPixels)
		{
		DoCmdGetNearestFontToDesignHeightInPixelsL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToMaxHeightInPixels)
		{
		DoCmdGetNearestFontToMaxHeightInPixelsL(aSection);
		}
	else if (aCommand == KCmdGetFontById)
		{
		DoCmdGetFontByIdL(aSection);
		}
	else if (aCommand == KCmdFontHeightInPixels)
		{
		DoCmdFontHeightInPixels(aSection);
		}
	else if (aCommand == KCmdDefaultBitmapType)
		{
		DoCmdDefaultBitmapType(aSection);
		}
	else if (aCommand == KCmdSetDefaultBitmapType)
		{
		DoCmdSetDefaultBitmapType(aSection);
		}
	else if (aCommand == KCmdSetFontNameAliasL)
		{
		DoCmdSetFontNameAlias(aSection);
		}
	else if (aCommand == KCmdSetDefaultLanguageForMetrics)
		{
		DoCmdSetDefaultLanguageForMetrics(aSection);
		}
	else if (aCommand == KCmdRemoveFontFileLocksL)
		{
		DoCmdRemoveFontFileLocks(aSection);
		}
	else if (aCommand == KCmdSetSystemDefaultTypefaceName)
		{
		DoCmdSetSystemDefaultTypefaceName(aSection);
		}
	else if (aCommand == KCmdDestructor)
		{
		DestroyData();
		}
	else
		{
		ret=CT_DataTypefaceStore::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


CTypefaceStore* CT_DataFbsTypefaceStore::GetTypefaceStore() const
	{
	return iFbsTypefaceStore;
	}



////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** CFbsTypefaceStore::NewL() call */
void CT_DataFbsTypefaceStore::DoCmdNewL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("CFbsTypefaceStore::NewL() call"));

	// cleanup if any
	DestroyData();


	// get FbsBitmapData object from parameters
	CGraphicsDevice*	device=NULL;
	TPtrC	deviceName;
	if ( GetStringFromConfig(aSection, KGraphicsDevice(), deviceName) )
		{
		// check that the data object was found
		device=static_cast<CGraphicsDevice*>(GetDataObjectL(deviceName));
		}

	// do create
	TRAPD(err, iFbsTypefaceStore = CFbsTypefaceStore::NewL(device));

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("CFbsTypefaceStore creation error: %d"), err);
		SetError(err);
		}
	}


/** Call CFbsTypefaceStore::GetNearestFontInPixels() */
void CT_DataFbsTypefaceStore::DoCmdGetNearestFontInPixelsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::GetNearestFontInPixels()"));

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFont, fontSpec) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFont());
		SetBlockResult(EFail);
		}
	else
		{
		// call CFbsTypefaceStore::GetNearestFontInPixels()
		CFont*		font = NULL;
		TInt		err = iFbsTypefaceStore->GetNearestFontInPixels(font, fontSpec);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			// set fbs font data object of provided
			SetFontDataObjectIfPassedAsParameterL(font, aSection);
			}
		}
	}


/** Call CFbsTypefaceStore::GetNearestFontToDesignHeightInPixels() */
void CT_DataFbsTypefaceStore::DoCmdGetNearestFontToDesignHeightInPixelsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::GetNearestFontToDesignHeightInPixels()"));

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFont, fontSpec) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFont());
		SetBlockResult(EFail);
		}
	else
		{
		// call CFbsTypefaceStore::GetNearestFontToDesignHeightInPixels()
		CFont*		fbsFont = NULL;
		TInt		err = iFbsTypefaceStore->GetNearestFontToDesignHeightInPixels(fbsFont, fontSpec);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			// set fbs font data object of provided
			SetFontDataObjectIfPassedAsParameterL(fbsFont, aSection);
			}
		}
	}


/** Call CFbsTypefaceStore::GetNearestFontToMaxHeightInPixels() */
void CT_DataFbsTypefaceStore::DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::GetNearestFontToMaxHeightInPixels()"));

	TBool		dataOk=ETrue;

	TFontSpec	fontSpec;
	if ( !GetFontSpecFromConfig(aSection, KFont, fontSpec) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFont());
		SetBlockResult(EFail);
		}

	TInt	fontMaxHeight;
	if ( !GetIntFromConfig(aSection, KFontMaxHeight(), fontMaxHeight) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFontMaxHeight());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// call CFbsTypefaceStore::GetNearestFontToMaxHeightInPixels()
		CFont*		fbsFont = NULL;
		TInt		err = iFbsTypefaceStore->GetNearestFontToMaxHeightInPixels(fbsFont, fontSpec, fontMaxHeight);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Get nearest font to max height in twips error: %d"), err);
			SetError(err);
			}
		else
			{
			// set fbs font data object of provided
			SetFontDataObjectIfPassedAsParameterL(fbsFont, aSection);

			// get check against maximum parameter from parameters
			TBool	checkAgainstMaximum = EFalse;
			GetBoolFromConfig(aSection, KCheckAgainstMaximum(), checkAgainstMaximum);

			if ( checkAgainstMaximum )
				{
				// check that the height of the returned font is not greater that the maximum specified
				if (fbsFont->HeightInPixels() > fontMaxHeight)
					{
					ERR_PRINTF3(_L("Font doesn't match in maximum specified, height: %d, maximum: %d"), fbsFont->HeightInPixels(), fontMaxHeight);
					SetBlockResult(EFail);
					}
				}
			}
		}
	}


/** Call CFbsTypefaceStore::AddFile(), remeber typeface id */
void CT_DataFbsTypefaceStore::DoCmdAddFile(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::AddFile(), remeber typeface id"));

	// get font file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}
	else
		{
		// call AddFile()
		TInt	err = iFbsTypefaceStore->AddFile(fileName, iLastTypefaceFileId);
		INFO_PRINTF2(_L("Id %d"), iLastTypefaceFileId);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Add file error: %d"), err);
			SetError(err);
			}
		}
	}


/** Call CFbsTypefaceStore::InstallFile(), remeber typeface id */
void CT_DataFbsTypefaceStore::DoCmdInstallFile(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::InstallFile(), remeber typeface id"));

	// get font file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}
	else
		{
		// call InstallFile()
		TInt	err = iFbsTypefaceStore->InstallFile(fileName, iLastTypefaceFileId);
		INFO_PRINTF2(_L("Id %d"), iLastTypefaceFileId);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Install file error: %d"), err);
			SetError(err);
			}
		}
	}


/** Call CFbsTypefaceStore::RemoveFile() */
void CT_DataFbsTypefaceStore::DoCmdRemoveFile(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::RemoveFile()"));

	// get typeface id from parameters if provided
	TInt	typefaceId=iLastTypefaceFileId;
	GetIntFromConfig(aSection, KTypefaceId(), typefaceId);
	INFO_PRINTF2(_L("Typeface id provided : %d"), typefaceId);

	// call RemoveFile()
	iFbsTypefaceStore->RemoveFile(typefaceId);
	}


/** Call CFbsTypefaceStore::GetFontById() */
void CT_DataFbsTypefaceStore::DoCmdGetFontByIdL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::GetFontById()"));

	// get font uid from parameters
	TInt	fontUid;
	if(!GetIntFromConfig(aSection, KFontUid(), fontUid))
		{
		ERR_PRINTF2(_L("No %S"), &KFontUid());
		SetBlockResult(EFail);
		}

	// call GetFontById()
	TAlgStyle*	algStylePointer = NULL;
	CT_GraphicsUtil::GetAlgStyleL(*this, aSection, KAlgStyle(), algStylePointer);

	CFont* 	fbsFont = NULL;
	TInt	err = KErrNone;
	if (algStylePointer == NULL)
		{
		TAlgStyle	algStyle;
		err = iFbsTypefaceStore->GetFontById(fbsFont, TUid::Uid(fontUid), algStyle);
		}
	else
		{
		err = iFbsTypefaceStore->GetFontById(fbsFont, TUid::Uid(fontUid), *algStylePointer);
		}

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Get font by id error: %d"), err);
		SetError(err);
		}
	}


/** Call CFbsTypefaceStore::FontHeightInPixels() */
void CT_DataFbsTypefaceStore::DoCmdFontHeightInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::FontHeightInPixels()"));
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
		TInt	typefaceHeight = iFbsTypefaceStore->FontHeightInPixels(typefaceIndex, heightIndex);

		// get positive height check flag from parameters
		TBool	checkHeightPositive = EFalse;
		GetBoolFromConfig(aSection, KCheckHeightPositive(), checkHeightPositive);

		// check that TTypefaceSupport fields are set
		if ( checkHeightPositive && (typefaceHeight<=0) )
			{
			ERR_PRINTF2(_L("Typeface height is not greater than 0, height: %d"), typefaceHeight);
			SetBlockResult(EFail);
			}
		}
	}


/** Call CFbsTypefaceStore::DefaultBitmapType() and compare value with expected */
void CT_DataFbsTypefaceStore::DoCmdDefaultBitmapType(const TDesC& aSection)
	{
	// get actual value of default bitmap type
	TGlyphBitmapType	actualValue = iFbsTypefaceStore->DefaultBitmapType();

	INFO_PRINTF2(_L("CFbsTypefaceStore::DefaultBitmapType() %d"), actualValue);


	// get expected value of default bitmap type from parameters
	TGlyphBitmapType	expectedType;
	if (!CT_GraphicsUtil::ReadGlyphBitmapType(*this, aSection, KGlyphBitmapType(), expectedType))
		{
		ERR_PRINTF1(_L("No glyph_bitmap_type"));
		SetBlockResult(EFail);
		}

	// compare
	if (actualValue != expectedType)
		{
		ERR_PRINTF3(_L("Value of default bitmap type is not as expected, expected: %d, actual: %d"), expectedType, actualValue);
		SetBlockResult(EFail);
		}
	}


/** Call CFbsTypefaceStore::SetDefaultBitmapType() */
void CT_DataFbsTypefaceStore::DoCmdSetDefaultBitmapType(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::SetDefaultBitmapType()"));

	// get expected value of default bitmap type from parameters
	TGlyphBitmapType	glyphBitmapType;
	if (!CT_GraphicsUtil::ReadGlyphBitmapType(*this, aSection, KGlyphBitmapType(), glyphBitmapType))
		{
		ERR_PRINTF1(_L("No glyph_bitmap_type"));
		SetBlockResult(EFail);
		}

	// get actual value of default bitmap type
	iFbsTypefaceStore->SetDefaultBitmapType(glyphBitmapType);
	}


/** Calls SetFontNameAliasL() for the given font name and alias name */
void CT_DataFbsTypefaceStore::DoCmdSetFontNameAlias(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls SetFontNameAliasL() for the given font name and alias name"));
	TBool								dataOk=ETrue;

	// get font name from parameters
	TPtrC	fontName;
	if (!GetStringFromConfig(aSection, KFontName(), fontName))
		{
		ERR_PRINTF2(_L("No %S"), &KFontName());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get font name alias from parameters
	TPtrC	fontNameAlias;
	if (!GetStringFromConfig(aSection, KFontNameAlias(), fontNameAlias))
		{
		ERR_PRINTF2(_L("No %S"), &KFontNameAlias());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		// set font name alias
		TRAPD(err, iFbsTypefaceStore->SetFontNameAliasL(fontNameAlias, fontName));

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Set font name alias error: %d"), err);
			SetError(err);
			}
		}
	}


/** Call CFbsTypefaceStore::SetDefaultLanguageForMetrics() */
void CT_DataFbsTypefaceStore::DoCmdSetDefaultLanguageForMetrics(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Call CFbsTypefaceStore::SetDefaultLanguageForMetrics()"));

	// get language from parameters
	TInt	language;
	if(!GetIntFromConfig(aSection, KLanguage(), language))
		{
		ERR_PRINTF2(_L("No %S"), &KLanguage());
		SetBlockResult(EFail);
		}
	else
		{
		// call SetDefaultLanguageForMetrics for the given language
		iFbsTypefaceStore->SetDefaultLanguageForMetrics((TLanguage) language);
		}
	}


/** Call CFbsTypefaceStore::RemoveFontFileLocksL() */
void CT_DataFbsTypefaceStore::DoCmdRemoveFontFileLocks(const TDesC& aSection)
	{
	// get if to call by file name flag from parameters
	TBool	useFileName = EFalse;
	GetBoolFromConfig(aSection, KUseFileName(), useFileName);

	// get if to call by drive name flag from parameters
	TBool	useDriveName = EFalse;
	GetBoolFromConfig(aSection, KUseDriveName(), useDriveName);

	TInt	err = KErrNone;

	if (useDriveName && useFileName)
		{
		ERR_PRINTF1(_L("Cannot all by both file and drive name"));
		SetBlockResult(EFail);
		}
	else if (useDriveName)
		{
		// get drive name from parameters
		TPtrC	driveName;
		if (!GetStringFromConfig(aSection, KDriveName(), driveName))
			{
			ERR_PRINTF2(_L("No %S"), &KDriveName());
			SetBlockResult(EFail);
			}

		// get all-fonts boolean value from parameters
		TBool	allFonts = EFalse;
		GetBoolFromConfig(aSection, KAllFonts(), allFonts);

		// call RemoveFontFileLocksL()
		TRAP(err, iFbsTypefaceStore->RemoveFontFileLocksL(driveName, allFonts));
		}
	else if (useFileName)
		{
		// get file name from parameters
		TPtrC	fileName;
		if (!GetStringFromConfig(aSection, KFileName(), fileName))
			{
			ERR_PRINTF2(_L("No %S"), &KFileName());
			SetBlockResult(EFail);
			}
		else
			{
			// call RemoveFontFileLocksL()
			TRAP(err, iFbsTypefaceStore->RemoveFontFileLocksL(fileName));
			}
		}
	else
		{
		TRAP(err, iFbsTypefaceStore->RemoveFontFileLocksL());
		}

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Remove font file lock error: %d"), err);
		SetError(err);
		}
	}


/** Call CFbsTypefaceStore::SetSystemDefaultTypefaceNameL() */
void CT_DataFbsTypefaceStore::DoCmdSetSystemDefaultTypefaceName(const TDesC& aSection)
	{

	TPtrC	name;
	if ( GetStringFromConfig(aSection, KFontName(), name) )
		{
		TRAPD(err, iFbsTypefaceStore->SetSystemDefaultTypefaceNameL(name));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("SetSystemDefaultTypefaceNameL error: %d"), err);
			SetError(err);
			}
		}
	else
		{
		ERR_PRINTF2(_L("No %S"), &KFontName());
		SetBlockResult(EFail);
		}
	}

