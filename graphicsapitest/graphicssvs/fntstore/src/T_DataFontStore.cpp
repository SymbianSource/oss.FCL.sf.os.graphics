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

 This contains CT_DataFontStore
 */
#include "T_DataFontStore.h"
#include "T_GraphicsUtil.h"
#include "T_TestOpenFont.h"

/*@{*/
///	Parameters
_LIT(KFldTypefaceName,				"typefacename");
_LIT(KFldHeapSize,					"heapsize");
_LIT(KFldFontFile,					"fontfile");
_LIT(KFldId,						"id");
_LIT(KFldExpectedGlyphBitmapType,	"expected_bmptype");
_LIT(KFldGlyphBitmapType,			"bmptype");
_LIT(KFldSessionHandle,				"sessionhandle");
_LIT(KFldTypefaceIndex,				"typeface_index");
_LIT(KFldHeightIndex,				"height_index");
_LIT(KFldExpectedHeight,			"expected_height");
_LIT(KFldFontUid,					"fontuid");
_LIT(KFldAlgStyle,					"alg_style");
_LIT(KFldFont,						"font");
_LIT(KFldOpenFont,					"openfont");
_LIT(KFldMaxHeight,					"maxheight");
_LIT(KFldUseTestClass,				"usetestclass");
_LIT(KFldRasterizer,				"rasterizer");
_LIT(KFldShaperFactory,				"shaperfactory");

///	Commands
_LIT(KCmdNewL,						"NewL");
_LIT(KCmdDestructor,				"~");
_LIT(KCmdAddFileL,					"AddFileL");
_LIT(KCmdRemoveFile,				"RemoveFile");
_LIT(KCmdDefaultBitmapType,			"DefaultBitmapType");
_LIT(KCmdDeleteSessionCache,		"DeleteSessionCache");
_LIT(KCmdFontHeightInPixels,		"FontHeightInPixels");
_LIT(KCmdGetFontById,				"GetFontById");
_LIT(KCmdGetNearestFontToDesignHeightInPixels,	"GetNearestFontToDesignHeightInPixels");
_LIT(KCmdGetNearestFontToDesignHeightInTwips,	"GetNearestFontToDesignHeightInTwips");
_LIT(KCmdGetNearestFontToMaxHeightInPixels,		"GetNearestFontToMaxHeightInPixels");
_LIT(KCmdGetNearestFontToMaxHeightInTwips,		"GetNearestFontToMaxHeightInTwips");
_LIT(KCmdInstallRasterizerL,					"InstallRasterizerL");
_LIT(KCmdInstallShaperFactoryL,		"InstallShaperFactoryL");
_LIT(KCmdSetDefaultBitmapType,		"SetDefaultBitmapType");

/// Logs
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingFilename,			"File '%S' not found in array");
_LIT(KLogFontSpec,					"font_spec or openfont_spec");
/*@}*/

/**
 * Two phase constructor
 */
CT_DataFontStore* CT_DataFontStore::NewL()
	{
	CT_DataFontStore* ret = new (ELeave) CT_DataFontStore();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
 * Protected second phase construction
 */
void CT_DataFontStore::ConstructL()
	{
	}

/**
 * Protected constructor. First phase construction
 */
CT_DataFontStore::CT_DataFontStore() :
	CT_DataTypefaceStore(), iFntStore(NULL)
	{
	}

/**
 * Destructor.
 */
CT_DataFontStore::~CT_DataFontStore()
	{
	DestroyData();
	iFile.Reset();
	iFile.Close();
	iId.Reset();
	iId.Close();
	}

/**
 * cleanup implementation.
 */
void CT_DataFontStore::DestroyData()
	{
	delete iFntStore;
	iFntStore = NULL;
	}

TAny* CT_DataFontStore::GetObject()
	{
	return iFntStore;
	}

void CT_DataFontStore::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iFntStore = static_cast<CFontStore*> (aObject);
	}

void CT_DataFontStore::DisownObjectL()
	{
	iFntStore = NULL;
	}

CTypefaceStore* CT_DataFontStore::GetTypefaceStore() const
	{
	return iFntStore;
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
TBool CT_DataFontStore::DoCommandL(const TTEFFunction& aCommand,
		const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNewL)
		{
		DoCmdNewL(aSection);
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else if (aCommand == KCmdAddFileL)
		{
		DoCmdAddFileL(aSection);
		}
	else if (aCommand == KCmdRemoveFile)
		{
		DoCmdRemoveFile(aSection);
		}
	else if (aCommand == KCmdDefaultBitmapType)
		{
		DoCmdDefaultBitmapType(aSection);
		}
	else if (aCommand == KCmdDeleteSessionCache)
		{
		DoCmdDeleteSessionCache(aSection);
		}
	else if (aCommand == KCmdFontHeightInPixels)
		{
		DoCmdFontHeightInPixels(aSection);
		}
	else if (aCommand == KCmdGetFontById)
		{
		DoCmdGetFontByIdL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToDesignHeightInPixels)
		{
		DoCmdGetNearestFontToDesignHeightInPixelsL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToDesignHeightInTwips)
		{
		if (!DoCmdGetNearestFontToDesignHeightInTwipsL(aSection))
			{
			retVal = CT_DataTypefaceStore::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdGetNearestFontToMaxHeightInPixels)
		{
		DoCmdGetNearestFontToMaxHeightInPixelsL(aSection);
		}
	else if (aCommand == KCmdGetNearestFontToMaxHeightInTwips)
		{
		if (!DoCmdGetNearestFontToMaxHeightInTwipsL(aSection))
			{
			retVal = CT_DataTypefaceStore::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
			}
		}
	else if (aCommand == KCmdInstallRasterizerL)
		{
		DoCmdInstallRasterizerL(aSection);
		}
	else if (aCommand == KCmdInstallShaperFactoryL)
		{
		DoCmdInstallShaperFactoryL(aSection);
		}
	else if (aCommand == KCmdSetDefaultBitmapType)
		{
		DoCmdSetDefaultBitmapType(aSection);
		}
	else
		{
		retVal = CT_DataTypefaceStore::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return retVal;
	}

////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Creates an instance of CT_DataFontStore class */
void CT_DataFontStore::DoCmdNewL(const TDesC& aSection)
	{
	// cleanup if any
	DestroyData();

	TInt heapsize;
	TInt err;
	
	INFO_PRINTF1(_L("execute CFontStore::NewL(RHeap*)"));
	if (GetIntFromConfig(aSection, KFldHeapSize, heapsize))
		{
		RHeap* heap = NULL;
		if (heapsize != 0)
			{
			heap = UserHeap::ChunkHeap(NULL, heapsize, heapsize);
			}
		TRAP(err, iFntStore = CFontStore::NewL(heap));
		}
	else
		{
		// Use heap of the thread.
		TRAP(err, iFntStore = CFontStore::NewL(&User::Heap()));
		}
	
	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataFontStore::DoCmdDestructor()
	{
	// cleanup if any
	DestroyData();
	}

void CT_DataFontStore::DoCmdAddFileL(const TDesC& aSection)
	{
	TPtrC name;
	
	if (!GetStringFromConfig(aSection, KFldFontFile, name))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFontFile());
		SetBlockResult(EFail);
		}
	else
		{
		TUid id;
		
		INFO_PRINTF1(_L("execute CFontStore::AddFileL(const TDesC&)"));
		TRAPD(err, id = iFntStore->AddFileL(name));
		
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			iFile.AppendL(name);
			iId.AppendL(id.iUid);
			}
		}
	}

TBool CT_DataFontStore::FindIdByName(const TDesC& aName, TInt& aId)
	{
	TBool found = EFalse;
	for (TInt index = 0; index < iFile.Count(); ++index)
		{
		if (iFile[index] == aName)
			{
			aId = iId[index];
			iFile.Remove(index);
			iId.Remove(index);
			found = ETrue;
			break;
			}
		}
	return found;
	}

void CT_DataFontStore::DoCmdRemoveFile(const TDesC& aSection)
	{
	TBool dataOk = ETrue;
	TInt id;
	
	// Get the uid
	if (!GetIntFromConfig(aSection, KFldId(), id))
		{
		TPtrC file;
		
		// No uid provided. Get file name instead.
		if (!GetStringFromConfig(aSection, KFldFontFile(), file))
			{
			dataOk = EFalse;
			}
		else
			{
			// Get uid by file name.
			if (!FindIdByName(file, id))
				{
				dataOk = EFalse;
				}
			}
		}

	if (dataOk)
		{
		INFO_PRINTF1(_L("execute CFontStore::RemoveFile(TUid)"));
		iFntStore->RemoveFile(TUid::Uid(id));
		}
	else
		{
		INFO_PRINTF1(_L("execute CFontStore::RemoveFile(TUid::Null())"));
		iFntStore->RemoveFile(TUid::Null());
		}
	}

void CT_DataFontStore::DoCmdDefaultBitmapType(const TDesC& aSection)
	{
	TGlyphBitmapType actual = iFntStore->DefaultBitmapType();
	INFO_PRINTF2(_L("execute CFontStore::DefaultBitmapType() = %d"), actual);
	
	TGlyphBitmapType expected;
	if(CT_GraphicsUtil::ReadGlyphBitmapType(*this, aSection, KFldExpectedGlyphBitmapType(), expected))
		{
		if (actual != expected)
			{
			ERR_PRINTF3(_L("The expected result %d is not equal to the actual result %d!"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataFontStore::DoCmdDeleteSessionCache(const TDesC& aSection)
	{
	TInt handle;

	if (!GetIntFromConfig(aSection, KFldSessionHandle, handle))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSessionHandle());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute CFontStore::DeleteSessionCache(TInt)"));
		iFntStore->DeleteSessionCache(handle);
		}
	}

void CT_DataFontStore::DoCmdFontHeightInPixels(const TDesC& aSection)
	{
	TInt tfindex;
	TInt heightindex;
	TBool dataOk = ETrue;
	
	if (!GetIntFromConfig(aSection, KFldTypefaceIndex, tfindex))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldTypefaceIndex());
		SetBlockResult(EFail);
		}
	
	if (!GetIntFromConfig(aSection, KFldHeightIndex, heightindex))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldHeightIndex());
		SetBlockResult(EFail);
		}
	
	if (dataOk)
		{
		TInt actual = iFntStore->FontHeightInPixels(tfindex, heightindex);
		INFO_PRINTF2(_L("execute CFontStore::FontHeightInPixels(TInt, TInt) = %d"), actual);
		
		TInt expected;
		if (GetIntFromConfig(aSection, KFldExpectedHeight, expected))
			{
			if (expected != actual)
				{
				ERR_PRINTF3(_L("The expected result %d is not equal to the actual result %d!"), expected, actual);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataFontStore::DoCmdGetFontByIdL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;
	TAlgStyle* algStyle = NULL;
	TPtrC wrapperName;
	TInt fontUid;

	// get font uid
	if (!GetIntFromConfig(aSection, KFldFontUid(), fontUid))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFontUid());
		SetBlockResult(EFail);
		}
	// get algorithmic style
	if	(!CT_GraphicsUtil::GetAlgStyleL(*this, aSection, KFldAlgStyle(), algStyle) && NULL == algStyle)
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldAlgStyle());
		SetBlockResult(EFail);
		}
	if (!GetStringFromConfig(aSection, KFldFont, wrapperName))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	if (dataOk)
		{
		CFont* font = NULL;
		INFO_PRINTF1(_L("execute CFontStore::GetFontById(CFont *&, TUid, const TAlgStyle&)"));
		TInt err = iFntStore->GetFontById(font, TUid::Uid(fontUid), *algStyle);

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

void CT_DataFontStore::DoCmdGetNearestFontToDesignHeightInPixelsL(
		const TDesC& aSection)
	{
	TPtrC wrapperName;
	
	if (!GetStringFromConfig(aSection, KFldFont, wrapperName))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	else
		{
		TFontSpec fontSpec;
		TInt err;
		CFont* font;
		TBool fxnCalled = EFalse;
		
		if (GetFontSpecFromConfig(aSection, KFldFont, fontSpec))
			{
			fxnCalled = ETrue;
			INFO_PRINTF1(_L("execute CFontStore::GetNearestFontToDesignHeightInPixels(CFont*&, const TFontSpec&)"));
			err = iFntStore->GetNearestFontToDesignHeightInPixels(font, fontSpec);
			}
		else if (GetFontSpecFromConfig(aSection, KFldOpenFont, fontSpec))
			{
			fxnCalled = ETrue;
			INFO_PRINTF1(_L("execute CFontStore::GetNearestFontToDesignHeightInPixels(CFont*&, const TOpenFontSpec&)"));
			err = iFntStore->GetNearestFontToDesignHeightInPixels(font, TOpenFontSpec(fontSpec));
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KLogFontSpec());
			SetBlockResult(EFail);
			}
		
		if (fxnCalled)
			{
			if (err != KErrNone)
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
	}

TBool CT_DataFontStore::DoCmdGetNearestFontToDesignHeightInTwipsL(
		const TDesC& aSection)
	{
	TPtrC wrapperName;

	if (!GetStringFromConfig(aSection, KFldFont, wrapperName))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	else
		{
		TFontSpec fontSpec;
		TInt err;
		CFont* font;

		if (!GetFontSpecFromConfig(aSection, KFldOpenFont, fontSpec))
			{
			return EFalse;
			}
		else
			{
			INFO_PRINTF1(_L("execute CFontStore::GetNearestFontToDesignHeightInTwips(CFont*&, const TOpenFontSpec&)"));
			err = iFntStore->GetNearestFontToDesignHeightInTwips(font, TOpenFontSpec(fontSpec));
			if (err != KErrNone)
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
	return ETrue;
	}

void CT_DataFontStore::DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;

	TPtrC wrapperName;
	if (!GetStringFromConfig(aSection, KFldFont, wrapperName))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}
	
	TInt maxHeight;
	if (!GetIntFromConfig(aSection, KFldMaxHeight, maxHeight))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldMaxHeight());
		SetBlockResult(EFail);
		}
	
	if (dataOk)
		{
		TFontSpec fontSpec;
		TInt err;
		CFont* font;
		TBool fxnCalled = EFalse;

		if (GetFontSpecFromConfig(aSection, KFldFont, fontSpec))
			{
			fxnCalled = ETrue;
			INFO_PRINTF1(_L("execute CFontStore::GetNearestFontToMaxHeightInPixels(CFont*&, const TFontSpec&, TInt)"));
			err = iFntStore->GetNearestFontToMaxHeightInPixels(font, fontSpec, maxHeight);
			}
		else if (GetFontSpecFromConfig(aSection, KFldOpenFont, fontSpec))
			{
			fxnCalled = ETrue;
			INFO_PRINTF1(_L("execute CFontStore::GetNearestFontToMaxHeightInPixels(CFont*&, const TOpenFontSpec&, TInt)"));
			err = iFntStore->GetNearestFontToMaxHeightInPixels(font, TOpenFontSpec(fontSpec), maxHeight);
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KLogFontSpec());
			SetBlockResult(EFail);
			}

		if (fxnCalled)
			{
			if (err != KErrNone)
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
	}

TBool CT_DataFontStore::DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection)
	{
	TBool dataOk = ETrue;
	TPtrC wrapperName;
	if (!GetStringFromConfig(aSection, KFldFont, wrapperName))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFont());
		SetBlockResult(EFail);
		}

	TInt maxHeight;
	if (!GetIntFromConfig(aSection, KFldMaxHeight, maxHeight))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldMaxHeight());
		SetBlockResult(EFail);
		}
	
	TFontSpec fontSpec;
	if (!GetFontSpecFromConfig(aSection, KFldOpenFont, fontSpec))
		{
		return EFalse;
		}
	
	if (dataOk)
		{
		TInt err;
		CFont* font;

		INFO_PRINTF1(_L("execute CFontStore::GetNearestFontToMaxHeightInTwips(CFont*&, const TOpenFontSpec&, TInt)"));
		err = iFntStore->GetNearestFontToMaxHeightInTwips(font, TOpenFontSpec(fontSpec), maxHeight);
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			SetDataObjectL(wrapperName, font);
			}
		}
	return ETrue;
	}

void CT_DataFontStore::DoCmdInstallRasterizerL(const TDesC& aSection)
	{
	COpenFontRasterizer* rasterizer = NULL;
	TInt err;
	TBool useDummy;
	
	if (!GetBoolFromConfig(aSection, KFldUseTestClass, useDummy))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldUseTestClass());
		SetBlockResult(EFail);
		}
	else
		{
		if (useDummy)
			{
			TRAP(err, rasterizer = CTestRasterizer::NewL());
			if (err != KErrNone)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				return;
				}
			}
		else
			{
			CT_GraphicsUtil::GetRasterizerL(*this, aSection, KFldRasterizer, rasterizer);
			}
		
		if (rasterizer == NULL)
			{
			INFO_PRINTF1(_L("execute CFontStore::InstallRasterizerL(NULL)"));
			}
		else
			{
			INFO_PRINTF1(_L("execute CFontStore::InstallRasterizerL(COpenFontRasterizer*)"));
			}
		TRAP(err, iFntStore->InstallRasterizerL(rasterizer));
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataFontStore::DoCmdInstallShaperFactoryL(const TDesC& aSection)
	{
	CShaperFactory* shaper = NULL;
	TInt err;
	TBool useDummy;
	
	if (!GetBoolFromConfig(aSection, KFldUseTestClass, useDummy))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldUseTestClass());
		SetBlockResult(EFail);
		}
	else
		{
		if (useDummy)
			{
			TRAP(err, shaper = CTestShaperFactory::NewL());
			if (err != KErrNone)
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				return;
				}
			}
		else
			{
			CT_GraphicsUtil::GetShaperFactoryL(*this, aSection, KFldShaperFactory, shaper);
			}

		if (shaper == NULL)
			{
			INFO_PRINTF1(_L("execute CFontStore::InstallShaperFactoryL(NULL)"));
			}
		else
			{
			INFO_PRINTF1(_L("execute CFontStore::InstallShaperFactoryL(CShaperFactory*)"));
			}
		TRAP(err, iFntStore->InstallShaperFactoryL(shaper));
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataFontStore::DoCmdSetDefaultBitmapType(const TDesC& aSection)
	{
	TGlyphBitmapType bmptype;
	
	if (!CT_GraphicsUtil::ReadGlyphBitmapType(*this, aSection, KFldGlyphBitmapType(), bmptype))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldGlyphBitmapType());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute CFontStore::SetDefaultBitmapType(TGlyphBitmapType)"));
		iFntStore->SetDefaultBitmapType(bmptype);
		}
	}
