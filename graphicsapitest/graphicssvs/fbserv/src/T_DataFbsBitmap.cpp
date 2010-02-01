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
v
This contains CT_DataFbsBitmap
*/

//	User includes
#include "T_DataFbsBitmap.h"
#include "T_GraphicsUtil.h"

//	EPOC includes
#include <graphics\bitmapuid.h>

/*@{*/
///	Parameters
_LIT(KAlways,								"always");
_LIT(KBitmapId,								"bitmap_id");
_LIT(KBuffLength,							"buff_length");
_LIT(KDitherOffsetX,						"dither_offset_x");
_LIT(KDitherOffsetY,						"dither_offset_y");
_LIT(KExpectedBool,							"expected_bool");
_LIT(KExpectedInt,							"expected_int");
_LIT(KFbsBitmapName,						"fbs_bitmap_name");
_LIT(KFileName,								"file_name");
_LIT(KFileOffset,							"file_offset");
_LIT(KHeight,								"height");
_LIT(KHandle,								"handle");
_LIT(KWidth,								"width");
_LIT(KLength,								"length");
_LIT(KNullExpected,							"null_expected");
_LIT(KDevice,								"device");
_LIT(KNumberOfSources,						"number_of_sources");
_LIT(KPixels,								"pixels");
_LIT(KPointX,								"point_x");
_LIT(KPointY,								"point_y");
_LIT(KRectTlX,								"rect_tl_x");
_LIT(KRectTlY,								"rect_tl_y");
_LIT(KRectBrX,								"rect_br_x");
_LIT(KRectBrY,								"rect_br_y");
_LIT(KScanLineCoord,						"scan_line_coord");
_LIT(KSourceFileName0,						"source_file_name_0"); // we don't need more than 3 sources for our test anyway.
_LIT(KSourceFileName1,						"source_file_name_1"); // So that this solution is ok
_LIT(KSourceFileName2,						"source_file_name_2");
_LIT(KSourceBitmapId0,						"source_bitmap_id_0");
_LIT(KSourceBitmapId1,						"source_bitmap_id_1");
_LIT(KSourceBitmapId2,						"source_bitmap_id_2");
_LIT(KShareIfLoaded,						"share_if_loaded");
_LIT(KTwips,								"twips");
_LIT(KUseDitherOffset,						"use_dither_offset");
_LIT(KUseRFile,								"use_rfile");
_LIT(KUseClosedRFile,						"use_closed_rfile");
_LIT(KUseClosedStream,						"use_closed_stream");
_LIT(KUseRequestStatus,						"use_request_status");
_LIT(KUid,									"uid");
_LIT(KPalette,								"palette");
_LIT(KModifiable,							"modifiable");
_LIT(KEntries,								"entries");
_LIT(KRed,									"red");
_LIT(KGreen,								"green");
_LIT(KBlue,									"blue");
_LIT(KDisplayMode,							"displaymode");
_LIT(KBitmapfileCompressionScheme,			"bitmap_compression_scheme");

///	Commands
_LIT(KCmdDisown,								"disown");
_LIT(KCmdNew,								"new");
_LIT(KCmdDestructor,						"~");
_LIT(KCmdCompress,							"Compress");
_LIT(KCmdCompressInBackground,				"CompressInBackground");
_LIT(KCmdCreate,							"Create");
_LIT(KCmdCreateHardwareBitmap,				"CreateHardwareBitmap");
_LIT(KCmdDataAddress,						"DataAddress");
_LIT(KCmdDataStride,						"DataStride");
_LIT(KCmdDisplayMode,						"DisplayMode");
_LIT(KCmdDuplicate,							"Duplicate");
_LIT(KCmdExternalizeL,						"ExternalizeL");
_LIT(KCmdExternalizeRectangleL,				"ExternalizeRectangleL");
_LIT(KCmdGetPalette,						"GetPalette");
_LIT(KCmdGetPixel,							"GetPixel");
_LIT(KCmdGetScanLine,						"GetScanLine");
_LIT(KCmdGetVerticalScanLine,				"GetVerticalScanLine");
_LIT(KCmdHandle,							"Handle");
_LIT(KCmdHardwareBitmapHandle,				"HardwareBitmapHandle");
_LIT(KCmdHeader,							"Header");
_LIT(KCmdHorizontalPixelsToTwips,			"HorizontalPixelsToTwips");
_LIT(KCmdHorizontalTwipsToPixels,			"HorizontalTwipsToPixels");
_LIT(KCmdInitialDisplayMode,				"InitialDisplayMode");
_LIT(KCmdInternalizeL,						"InternalizeL");
_LIT(KCmdIsCompressedInRAM,					"IsCompressedInRAM");
_LIT(KCmdIsFileInRom,						"IsFileInRom");
_LIT(KCmdIsLargeBitmap,						"IsLargeBitmap");
_LIT(KCmdIsMonochrome,						"IsMonochrome");
_LIT(KCmdIsRomBitmap,						"IsRomBitmap");
_LIT(KCmdLoad,								"Load");
_LIT(KCmdLoadAndCompress,					"LoadAndCompress");
_LIT(KCmdLockHeap,							"LockHeap");
_LIT(KCmdLockHeapLC,						"LockHeapLC");
_LIT(KCmdPaletteAttributes,					"PaletteAttributes");
_LIT(KCmdReset,								"Reset");
_LIT(KCmdResize,							"Resize");
_LIT(KCmdSave,								"Save");
_LIT(KCmdScanLineLength,					"ScanLineLength");
_LIT(KCmdSetDisplayMode,					"SetDisplayMode");
_LIT(KCmdSetPalette,						"SetPalette");
_LIT(KCmdSetRomBitmapL,						"SetRomBitmapL");
_LIT(KCmdSetScanLine,						"SetScanLine");
_LIT(KCmdSetSizeInTwips,					"SetSizeInTwips");
_LIT(KCmdSizeInPixels,						"SizeInPixels");
_LIT(KCmdSizeInTwips,						"SizeInTwips");
_LIT(KCmdStoreL,							"StoreL");
_LIT(KCmdSwapWidthAndHeight,				"SwapWidthAndHeight");
_LIT(KCmdUnlockHeap,						"UnlockHeap");
_LIT(KCmdVerticalPixelsToTwips,				"VerticalPixelsToTwips");
_LIT(KCmdVerticalTwipsToPixels,				"VerticalTwipsToPixels");

//??	SetRomBitmapL
//	Begin
//	End
//	GetDecompressionBuffer
//	GetAllBitmapHandles
/*@}*/

/**
* Two phase constructor
*/
CT_DataFbsBitmap* CT_DataFbsBitmap::NewL()
	{
	CT_DataFbsBitmap* ret = new (ELeave) CT_DataFbsBitmap();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsBitmap::CT_DataFbsBitmap()
:	CDataWrapperBase()
,	iFbsBitmap(NULL)
,	iDisplayMode(ENone)
,	iPopWhenUnlocking(EFalse)
,	iCompressFailed(EFalse)
,	iActiveCompressInBackground(NULL)
	{
	}

/**
* Protected constructor. Second phase construction
*/
void CT_DataFbsBitmap::ConstructL()
	{
	iActiveCompressInBackground=CActiveCallback::NewL(*this);
	}

/**
* Destructor.
*/
CT_DataFbsBitmap::~CT_DataFbsBitmap()
	{
	DestroyData();
	delete iActiveCompressInBackground;
	iActiveCompressInBackground=NULL;
	}

/**
* Contains cleanup implementation
*/
void CT_DataFbsBitmap::DestroyData()
	{
	delete iFbsBitmap;
	iFbsBitmap=NULL;
	}

/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsBitmap::GetObject()
	{
	return iFbsBitmap;
	}

void CT_DataFbsBitmap::SetObjectL(TAny* aAny)
    {
    DestroyData();
    iFbsBitmap=static_cast<CFbsBitmap*> (aAny);
    }

void CT_DataFbsBitmap::DisownObjectL()
	{
	iFbsBitmap=NULL;
	}

CFbsBitmap* CT_DataFbsBitmap::GetFbsBitmap() const
	{
	return iFbsBitmap;
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
TBool CT_DataFbsBitmap::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNewL();
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else if (aCommand == KCmdCompress)
		{
		DoCmdCompress(aSection);
		}
	else if (aCommand == KCmdCompressInBackground)
		{
		DoCmdCompressInBackground(aSection, aAsyncErrorIndex);
		}
	else if (aCommand == KCmdCreate)
		{
		DoCmdCreate(aSection);
		}
	else if (aCommand == KCmdCreateHardwareBitmap)
		{
		DoCmdCreateHardwareBitmap(aSection);
		}
	else if (aCommand == KCmdDataAddress)
		{
		DoCmdDataAddress(aSection);
		}
	else if (aCommand == KCmdDataStride)
		{
		DoCmdDataStride(aSection);
		}
	else if (aCommand == KCmdDisplayMode)
		{
		DoCmdDisplayMode(aSection);
		}
	else if (aCommand == KCmdDuplicate)
		{
		DoCmdDuplicateL(aSection);
		}
	else if (aCommand == KCmdExternalizeL)
		{
		DoCmdExternalizeL(aSection);
		}
	else if (aCommand == KCmdExternalizeRectangleL)
		{
		DoCmdExternalizeRectangleL(aSection);
		}
	else if (aCommand == KCmdGetPalette)
		{
		DoCmdGetPaletteL(aSection);
		}
	else if (aCommand == KCmdGetPixel)
		{
		DoCmdGetPixel(aSection);
		}
	else if (aCommand == KCmdGetScanLine)
		{
		DoCmdGetScanLineL(aSection);
		}
	else if (aCommand == KCmdGetVerticalScanLine)
		{
		DoCmdGetVerticalScanLineL(aSection);
		}
	else if (aCommand == KCmdHandle)
		{
		DoCmdHandle(aSection);
		}
	else if (aCommand == KCmdHardwareBitmapHandle)
		{
		DoCmdHardwareBitmapHandle(aSection);
		}
	else if (aCommand == KCmdHeader)
		{
		DoCmdHeader();
		}
	else if (aCommand == KCmdHorizontalPixelsToTwips)
		{
		DoCmdHorizontalPixelsToTwips(aSection);
		}
	else if (aCommand == KCmdHorizontalTwipsToPixels)
		{
		DoCmdHorizontalTwipsToPixels(aSection);
		}
	else if (aCommand == KCmdInitialDisplayMode)
		{
		DoCmdInitialDisplayMode(aSection);
		}
	else if (aCommand == KCmdInternalizeL)
		{
		DoCmdInternalizeL(aSection);
		}
	else if (aCommand == KCmdIsCompressedInRAM)
		{
		DoCmdIsCompressedInRAM(aSection);
		}
	else if (aCommand == KCmdIsFileInRom)
		{
		DoCmdIsFileInRom(aSection);
		}
	else if (aCommand == KCmdIsLargeBitmap)
		{
		DoCmdIsLargeBitmap(aSection);
		}
	else if (aCommand == KCmdIsMonochrome)
		{
		DoCmdIsMonochrome(aSection);
		}
	else if (aCommand == KCmdIsRomBitmap)
		{
		DoCmdIsRomBitmap(aSection);
		}
	else if (aCommand == KCmdLoad)
		{
		DoCmdLoadL(aSection);
		}
	else if (aCommand == KCmdLoadAndCompress)
		{
		DoCmdLoadAndCompressL(aSection);
		}
	else if (aCommand == KCmdLockHeap)
		{
		DoCmdLockHeap(aSection);
		}
	else if (aCommand == KCmdLockHeapLC)
		{
		DoCmdLockHeapLC(aSection);
		}
	else if (aCommand == KCmdPaletteAttributes)
		{
		DoCmdPaletteAttributes(aSection);
		}
	else if (aCommand == KCmdReset)
		{
		DoCmdReset();
		}
	else if (aCommand == KCmdResize)
		{
		DoCmdResize(aSection);
		}
	else if (aCommand == KCmdSave)
		{
		DoCmdSave(aSection);
		}
	else if (aCommand == KCmdScanLineLength)
		{
		DoCmdScanLineLength(aSection);
		}
	else if (aCommand == KCmdSetDisplayMode)
		{
		DoCmdSetDisplayMode(aSection);
		}
	else if (aCommand == KCmdSetPalette)
		{
		DoCmdSetPaletteL(aSection);
		}
	else if (aCommand == KCmdSetRomBitmapL)
		{
		DoCmdSetRomBitmapL(aSection);
		}
	else if (aCommand == KCmdSetScanLine)
		{
		DoCmdSetScanLineL(aSection);
		}
	else if (aCommand == KCmdSetSizeInTwips)
		{
		DoCmdSetSizeInTwipsL(aSection);
		}
	else if (aCommand == KCmdSizeInPixels)
		{
		DoCmdSizeInPixels(aSection);
		}
	else if (aCommand == KCmdSizeInTwips)
		{
		DoCmdSizeInTwips(aSection);
		}
	else if (aCommand == KCmdStoreL)
		{
		DoCmdStoreL(aSection);
		}
	else if (aCommand == KCmdSwapWidthAndHeight)
		{
		DoCmdSwapWidthAndHeight();
		}
	else if (aCommand == KCmdUnlockHeap)
		{
		DoCmdUnlockHeapL(aSection);
		}
	else if (aCommand == KCmdVerticalPixelsToTwips)
		{
		DoCmdVerticalPixelsToTwips(aSection);
		}
	else if (aCommand == KCmdVerticalTwipsToPixels)
		{
		DoCmdVerticalTwipsToPixels(aSection);
		}
	else if (aCommand == KCmdDisown)
   {
   DisownObjectL();
   }
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}


/** Creates new CFbsBitmap class instance */
void CT_DataFbsBitmap::DoCmdNewL()
	{
	INFO_PRINTF1(_L("Creates new CFbsBitmap class instance"));

	// cleanup if already created
    DestroyData();
	iFbsBitmap=new (ELeave) CFbsBitmap();
	}

/** Calls static CFbsBitmap destructor */
void CT_DataFbsBitmap::DoCmdDestructor()
	{
	DestroyData();
	}

/** Calls CFbsBitmap::Compress() */
void CT_DataFbsBitmap::DoCmdCompress(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Compress()"));

	TInt							err=KErrNone;
	TBitmapfileCompressionScheme	compressionScheme;
	if ( CT_GraphicsUtil::ReadBitmapfileCompressionScheme(*this, aSection, KBitmapfileCompressionScheme(), compressionScheme) )
		{
		err=GetFbsBitmap()->Compress(compressionScheme);
		}
	else
		{
		// call Compress()
		err=GetFbsBitmap()->Compress();
		}
	// check error code
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("Compress error: %d"), err);
		SetError(err);
		iCompressFailed = ETrue;
		}
	else
	    {
	    iCompressFailed = EFalse;
	    };
	}

/** Calls CFbsBitmap::CompressInBackground() */
void CT_DataFbsBitmap::DoCmdCompressInBackground(const TDesC& aSection, const TInt aAsyncErrorIndex)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::CompressInBackground()"));

	// get a flag if to use requset status from parameters
	TBool	useRequestStatus = EFalse;
	GetBoolFromConfig(aSection, KUseRequestStatus(), useRequestStatus);

	TBitmapfileCompressionScheme	compressionScheme;
	TBool							hasCompressionScheme=CT_GraphicsUtil::ReadBitmapfileCompressionScheme(*this, aSection, KBitmapfileCompressionScheme(), compressionScheme);

	// call CompressInBackground()
	if ( useRequestStatus )
		{
		if ( hasCompressionScheme )
			{
			GetFbsBitmap()->CompressInBackground(iActiveCompressInBackground->iStatus, compressionScheme);
			}
		else
			{
			GetFbsBitmap()->CompressInBackground(iActiveCompressInBackground->iStatus);
			}
		iActiveCompressInBackground->Activate(aAsyncErrorIndex);
		IncOutstanding();
		}
	else
		{
		TInt	err = KErrNone;

		if ( hasCompressionScheme )
			{
			err=GetFbsBitmap()->CompressInBackground(compressionScheme);
			}
		else
			{
			err=GetFbsBitmap()->CompressInBackground();
			}

		// check error code
		if ( err != KErrNone )
			{
			ERR_PRINTF2(_L("Compress in background error: %d"), err);
			SetError(err);
			iCompressFailed = ETrue;
			}
		else
		    {
		    iCompressFailed = EFalse;
		    }
		}
	}

/** Calls CFbsBitmap::Create() */
void CT_DataFbsBitmap::DoCmdCreate(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Create()"));

	// get size height from parameters
	TInt	height;
	if(!GetIntFromConfig(aSection, KHeight(), height))
		{
		ERR_PRINTF2(_L("No %S"), &KHeight());
		SetBlockResult(EFail);
		}

	// get size width from parameters
	TInt	width;
	if(!GetIntFromConfig(aSection, KWidth(), width))
		{
		ERR_PRINTF2(_L("No %S"), &KWidth());
		SetBlockResult(EFail);
		}

	// get display mode from parameters

	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		}
	else
		{
		// call Create()
		TInt	err = GetFbsBitmap()->Create(TSize(width, height), displayMode);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Creation error: %d"), err);
			SetError(err);
			}
		}
	}

/** Calls CFbsBitmap::CreateHardwareBitmap() */
void CT_DataFbsBitmap::DoCmdCreateHardwareBitmap(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::CreateHardwareBitmap()"));

	// get size height from parameters
	TInt	height;
	if(!GetIntFromConfig(aSection, KHeight(), height))
		{
		ERR_PRINTF2(_L("No %S"), &KHeight());
		SetBlockResult(EFail);
		}

	// get size width from parameters
	TInt	width;
	if(!GetIntFromConfig(aSection, KWidth(), width))
		{
		ERR_PRINTF2(_L("No %S"), &KWidth());
		SetBlockResult(EFail);
		}

	// get display mode from parameters
	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		}

	// get application uid from parameters
	TInt	uidInt;
	if(!GetIntFromConfig(aSection, KUid(), uidInt))
		{
		ERR_PRINTF2(_L("No %S"), &KUid());
		SetBlockResult(EFail);
		}

	// call Create()
	TInt	err = GetFbsBitmap()->CreateHardwareBitmap(TSize(width, height), displayMode, TUid::Uid(uidInt));

	// check error code
	if ( err != KErrNone )
		{
		ERR_PRINTF2(_L("Hardware bitmap creation error: %d"), err);
		SetError(err);
		}
	}

/** Calls CFbsBitmap::DataAddress() locking and unlocking heap by defined in parameters means */
void CT_DataFbsBitmap::DoCmdDataAddress(const TDesC& aSection)
	{
	// call DataAddress()
	TUint32*	address=GetFbsBitmap()->DataAddress();
	INFO_PRINTF2(_L("DataAddress %d"), address);

	// validate returned value if needed
	TInt	expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != (TInt)address)
			{
			ERR_PRINTF1(_L("The returned value is not as expected"));
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::DataStride() */
void CT_DataFbsBitmap::DoCmdDataStride(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::DataStride()"));
	
	// call DataStride()
	TInt value = GetFbsBitmap()->DataStride();
	INFO_PRINTF2(_L("Data stride: %d"), value);

	// validate returned value if needed
	TInt expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, value);
			SetBlockResult(EFail);
			}
		}

	// check the value is not NULL
	if (value <= 0)
		{
		ERR_PRINTF2(_L("The data stride is not expected zero or less, value: %d"), value);
		SetBlockResult(EFail);
		}
	}

/** Checks CFbsBitmap::DisplayMode() value */
void CT_DataFbsBitmap::DoCmdDisplayMode(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Checks CFbsBitmap::DisplayMode() value"));

	// call DisplayMode()
	iDisplayMode=GetFbsBitmap()->DisplayMode();
	INFO_PRINTF2(_L("Display Mode %d"), iDisplayMode);

	// get display mode from parameters
	TDisplayMode	expectedValue;
	if ( CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), expectedValue) )
		{
		// check value against NULL
		if ( iDisplayMode!=expectedValue )
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expectedValue, iDisplayMode);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::Duplicate() */
void CT_DataFbsBitmap::DoCmdDuplicateL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Duplicate()"));
	TInt	handle=GetHandle();
	TPtrC	objectName;
	if ( GetStringFromConfig(aSection, KFbsBitmapName(), objectName) )
		{
		CDataWrapperBase*	wrapper=static_cast<CDataWrapperBase*>(GetDataWrapperL(objectName));
		handle=wrapper->GetHandle();
		}
	else
		{
		GetIntFromConfig(aSection, KHandle(), handle);
		}

	// call Duplicate()
	TInt	err = GetFbsBitmap()->Duplicate(handle);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Duplicate error: %d"), err);
		SetError(err);
		}
	}

/** Calls CFbsBitmap::ExternalizeL() */
void CT_DataFbsBitmap::DoCmdExternalizeL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::ExternalizeL()"));
	ExternalizeImplL(aSection, TRect(), EFalse);
	}

/** Calls CFbsBitmap::ExternalizeRectangleL() */
void CT_DataFbsBitmap::DoCmdExternalizeRectangleL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::ExternalizeRectangleL()"));

	// get rectangle top left x-coordinate from parameters
	TInt	tlx;
	if(!GetIntFromConfig(aSection, KRectTlX(), tlx))
		{
		ERR_PRINTF2(_L("No %S"), &KRectTlX());
		SetBlockResult(EFail);
		}

	// get rectangle top left x-coordinate from parameters
	TInt	tly;
	if(!GetIntFromConfig(aSection, KRectTlY(), tly))
		{
		ERR_PRINTF2(_L("No %S"), &KRectTlY());
		SetBlockResult(EFail);
		}

	// get rectangle top left x-coordinate from parameters
	TInt	brx;
	if(!GetIntFromConfig(aSection, KRectBrX(), brx))
		{
		ERR_PRINTF2(_L("No %S"), &KRectBrX());
		SetBlockResult(EFail);
		}

	// get rectangle top left x-coordinate from parameters
	TInt	bry;
	if(!GetIntFromConfig(aSection, KRectBrY(), bry))
		{
		ERR_PRINTF2(_L("No %S"), &KRectBrY());
		SetBlockResult(EFail);
		}

	ExternalizeImplL(aSection, TRect(tlx, tly, brx, bry), ETrue);
	}

/** Calls CFbsBitmap::GetPalette() */
void CT_DataFbsBitmap::DoCmdGetPaletteL(const TDesC& aSection)
	{
	// call GetPalette()
	CPalette*	palette = NULL;
	TInt		err = GetFbsBitmap()->GetPalette(palette);
	INFO_PRINTF2(_L("Calls CFbsBitmap::GetPalette() %d"), palette);

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("GetPalette error: %d"), err);
		SetError(err);
		}
	else
		{
		//	Verify palette
		CPalette*	expectedPalette = NULL;

		if ( CT_GraphicsUtil::GetPaletteL(*this, aSection, KPalette(), expectedPalette) )
			{
			if ( palette != expectedPalette )
				{
				ERR_PRINTF3(_L("GetPalette palette %d not the expected palette %d"), palette, expectedPalette);
				SetBlockResult(EFail);
				}
			}
		}
	}

/** Calls CFbsBitmap::GetPixel() */
void CT_DataFbsBitmap::DoCmdGetPixel(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::GetPixel()"));

	// get point x-coordinate from parameters
	TInt	pointX;
	if(!GetIntFromConfig(aSection, KPointX(), pointX))
		{
		ERR_PRINTF2(_L("No %S"), &KPointX());
		SetBlockResult(EFail);
		}

	// get point y-coordinate from parameters
	TInt	pointY;
	if(!GetIntFromConfig(aSection, KPointY(), pointY))
		{
		ERR_PRINTF2(_L("No %S"), &KPointY());
		SetBlockResult(EFail);
		}

	// call GetPixel()
	TRgb	color;
	GetFbsBitmap()->GetPixel(color, TPoint(pointX, pointY));

	INFO_PRINTF2(_L("Red    %d"), color.Red());
	INFO_PRINTF2(_L("Green  %d"), color.Green());
	INFO_PRINTF2(_L("Blue   %d"), color.Blue());

	TInt	actual;
	TInt	expected;

	if(GetIntFromConfig(aSection, KRed(), expected))
		{
		actual=color.Red();
		if ( expected != actual )
			{
			ERR_PRINTF3(_L("Expected Red %d does not match actual %d"), expected, actual);
			SetBlockResult(EFail);
			}
		}

	if(GetIntFromConfig(aSection, KGreen(), expected))
		{
		actual=color.Green();
		if ( expected != actual )
			{
			ERR_PRINTF3(_L("Expected Green %d does not match actual %d"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	
	if(GetIntFromConfig(aSection, KBlue(), expected))
		{
		actual=color.Blue();
		if ( expected != actual )
			{
			ERR_PRINTF3(_L("Expected Blue %d does not match actual %d"), expected, actual);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::GetScanLine() */
void CT_DataFbsBitmap::DoCmdGetScanLineL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::GetScanLine()"));
	TBool								dataOk=ETrue;

	// get point x coordinate from parameters
	TInt	pointX = 0;
	if(!GetIntFromConfig(aSection, KPointX(), pointX))
		{
		ERR_PRINTF2(_L("No %S"), &KPointX());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get point y coordinate from parameters
	TInt	pointY = 0;
	if(!GetIntFromConfig(aSection, KPointY(), pointY))
		{
		ERR_PRINTF2(_L("No %S"), &KPointY());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get length from parameters
	TInt	length = 0;
	if(!GetIntFromConfig(aSection, KLength(), length))
		{
		ERR_PRINTF2(_L("No %S"), &KLength());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get buffer length from parameters
	TInt	buffLength = 0;
	if(!GetIntFromConfig(aSection, KBuffLength(), buffLength))
		{
		ERR_PRINTF2(_L("No %S"), &KBuffLength());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get display mode from parameters
	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		// call GetScanLine()
		TUint8*	buffer = new (ELeave) TUint8[buffLength];
		TPtr8	scanline(buffer, buffLength, buffLength);

		GetFbsBitmap()->GetScanLine(scanline, TPoint(pointX, pointY), length, displayMode);

		delete [] buffer;
		}
	}

/** Calls CFbsBitmap::GetVerticalScanLine() */
void CT_DataFbsBitmap::DoCmdGetVerticalScanLineL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::GetVerticalScanLine()"));
	TBool								dataOk=ETrue;

	// get point x coordinate from parameters
	TInt	pointX = 0;
	if(!GetIntFromConfig(aSection, KPointX(), pointX))
		{
		ERR_PRINTF2(_L("No %S"), &KPointX());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get if to use dither offset flag from parameters
	TBool	useDitherOffset = EFalse;
	if(!GetBoolFromConfig(aSection, KUseDitherOffset(), useDitherOffset))
		{
		ERR_PRINTF2(_L("No %S"), &KUseDitherOffset());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get dither offset x coordinate from parameters
	TInt	ditherOffsetX = 0;
	if(useDitherOffset && !GetIntFromConfig(aSection, KDitherOffsetX(), ditherOffsetX))
		{
		ERR_PRINTF2(_L("No %S"), &KDitherOffsetX());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get dither offset y coordinate from parameters
	TInt	ditherOffsetY = 0;
	if(useDitherOffset && !GetIntFromConfig(aSection, KDitherOffsetY(), ditherOffsetY))
		{
		ERR_PRINTF2(_L("No %S"), &KDitherOffsetY());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get buffer length from parameters
	TInt	buffLength = 0;
	if(!GetIntFromConfig(aSection, KBuffLength(), buffLength))
		{
		ERR_PRINTF2(_L("No %S"), &KBuffLength());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get display mode from parameters
	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		// call GetVerticalScanLine()
		TUint8*	buffer = new (ELeave) TUint8[buffLength];
		TPtr8 	scanline(buffer, buffLength, buffLength);

		if (useDitherOffset)
			{
			GetFbsBitmap()->GetVerticalScanLine(scanline, pointX, displayMode);
			}
		else
			{
			GetFbsBitmap()->GetVerticalScanLine(scanline, pointX, TPoint(ditherOffsetX, ditherOffsetY), displayMode);
			}

		delete [] buffer;
		}
	}

/** Calls CFbsBitmap::Handle() */
void CT_DataFbsBitmap::DoCmdHandle(const TDesC& aSection)
	{
	
	// call Handle()
	TInt	handle=GetFbsBitmap()->Handle();
	INFO_PRINTF2(_L("Calls CFbsBitmap::Handle() %d"), handle);
	SetHandle(handle);

	// validate returned value if needed
	TInt	expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != handle)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, handle);
			SetBlockResult(EFail);
			}
		}
	}

/** Checks CFbsBitmap::HardwareBitmapHandle() */
void CT_DataFbsBitmap::DoCmdHardwareBitmapHandle(const TDesC& aSection)
	{
	// call HardwareBitmapHandle()
	TInt	handle=GetFbsBitmap()->HardwareBitmapHandle();
	INFO_PRINTF2(_L("CFbsBitmap::HardwareBitmapHandle() %d"), handle);
	SetHandle(handle);

	// get if null expected flag from parameters
	TBool	nullExpected = EFalse;
	if ( GetBoolFromConfig(aSection, KNullExpected(), nullExpected) )
		{
		// check value againts NULL
		if ( (handle == NULL) != nullExpected )
			{
			ERR_PRINTF2(_L("The value is not as expected! value: %d"), handle);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::Header() */
void CT_DataFbsBitmap::DoCmdHeader()
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Header()"));

	// call Header()
	SEpocBitmapHeader	header=GetFbsBitmap()->Header();
	INFO_PRINTF2(_L("Header iBitmapSize     &d"), header.iBitmapSize);
	INFO_PRINTF2(_L("Header iBitsPerPixel   &d"), header.iBitsPerPixel);
	INFO_PRINTF2(_L("Header iColor          &d"), header.iColor);
	INFO_PRINTF2(_L("Header iCompression    &d"), header.iCompression);
	INFO_PRINTF2(_L("Header iPaletteEntries &d"), header.iPaletteEntries);
	INFO_PRINTF3(_L("Header iSizeInPixels   &d %d"), header.iSizeInPixels.iWidth, header.iSizeInPixels.iHeight);
	INFO_PRINTF3(_L("Header iSizeInTwips    &d %d"), header.iSizeInTwips.iWidth, header.iSizeInTwips.iHeight);
	INFO_PRINTF2(_L("Header iStructSize     &d"), header.iStructSize);
	}

/** Calls CFbsBitmap::HorizontalPixelsToTwips() */
void CT_DataFbsBitmap::DoCmdHorizontalPixelsToTwips(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::HorizontalPixelsToTwips()"));

	// get pixels from parameters
	TInt	pixels;
	if(!GetIntFromConfig(aSection, KPixels(), pixels))
		{
		ERR_PRINTF2(_L("No %S"), &KPixels());
		SetBlockResult(EFail);
		}

	// call HorizontalPixelsToTwips()
	TInt	value = GetFbsBitmap()->HorizontalPixelsToTwips(pixels);
	INFO_PRINTF2(_L("Twips: %d"), value);

	// validate returned value if needed
	TInt	expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, value);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::HorizontalTwipsToPixels() */
void CT_DataFbsBitmap::DoCmdHorizontalTwipsToPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::HorizontalTwipsToPixels()"));

	// get twips from parameters
	TInt	twips;
	if(!GetIntFromConfig(aSection, KTwips(), twips))
		{
		ERR_PRINTF2(_L("No %S"), &KTwips());
		SetBlockResult(EFail);
		}

	// call HorizontalTwipsToPixels()
	TInt	value = GetFbsBitmap()->HorizontalTwipsToPixels(twips);
	INFO_PRINTF2(_L("Pixels: %d"), value);

	// validate returned value if needed
	TInt	expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, value);
			SetBlockResult(EFail);
			}
		}
	}

/** Checks CFbsBitmap::InitialDisplayMode() value */
void CT_DataFbsBitmap::DoCmdInitialDisplayMode(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Checks CFbsBitmap::InitialDisplayMode() value"));

	// call InitialDisplayMode()
	TInt	actualValue = GetFbsBitmap()->InitialDisplayMode();
	INFO_PRINTF2(_L("Initial Display Mode %d"), actualValue);

	// get display mode from parameters
	TDisplayMode	expectedValue;
	if ( CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), expectedValue) )
		{
		// check value againts NULL
		if (actualValue != expectedValue)
			{
			ERR_PRINTF3(_L("The value is not as expected! expecyed: %d, actual: %d"), expectedValue, actualValue);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::InternalizeL() */
void CT_DataFbsBitmap::DoCmdInternalizeL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::InternalizeL()"));

	// get file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// get a flag if to use closed stream from parameters
	TBool	useClosedStream = EFalse;
	if(!GetBoolFromConfig(aSection, KUseClosedStream(), useClosedStream))
		{
		ERR_PRINTF2(_L("No %S"), &KUseClosedStream());
		SetBlockResult(EFail);
		}

	// ReadStore creation
	CDirectFileStore*	readstore = CDirectFileStore::OpenL(FileServer(), fileName, EFileStream | EFileRead);
	CleanupStack::PushL(readstore);
	TInt				popCount=1;
	TStreamId			headerid = readstore->Root();
	RStoreReadStream	readstrm;

	// close stream if defined by parameters
	if (!useClosedStream)
		{
		readstrm.OpenL(*readstore, headerid);
		CleanupClosePushL(readstrm);
		popCount=2;
		}

	// call InternalizeL()
	TRAPD(err, GetFbsBitmap()->InternalizeL(readstrm));
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Internalize error: %d"), err);
		SetError(err);
		}

	// cleanup
	CleanupStack::PopAndDestroy(popCount, readstore);
	}

/** Calls CFbsBitmap::IsCompressedInRAM() to check againt expected value */
void CT_DataFbsBitmap::DoCmdIsCompressedInRAM(const TDesC& aSection)
	{
	// call IsCompressedInRAM() to get actual value
	TBool	actualValue = GetFbsBitmap()->IsCompressedInRAM();
	INFO_PRINTF2(_L("CFbsBitmap::IsCompressedInRAM() %d"), actualValue);

	// get expected value from parameters
	TBool	expectedValue = EFalse;
	if ( GetBoolFromConfig(aSection, KExpectedBool(), expectedValue) )
		{
		if (actualValue != expectedValue)
			{
			ERR_PRINTF3(_L("Expected value doesn't match to actual! expected: %d, actual: %d"), expectedValue, actualValue);
			if (!iCompressFailed)
			    {
			    SetBlockResult(EFail);
			    };
			}
		}
	}

/** Calls CFbsBitmap::IsFileInRom() and check against expectation */
void CT_DataFbsBitmap::DoCmdIsFileInRom(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::IsFileInRom() and check against expectation"));

	// get file name from parameters

	TPtrC	fileName;
	if ( !GetStringFromConfig(aSection, KFileName(), fileName) )
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// get flag if use RFile instance in Save() call from parameters
	TBool	useRFile = EFalse;
	GetBoolFromConfig(aSection, KUseRFile(), useRFile);

	TInt		err = KErrNone;
	TBool		isFileInRom = EFalse;
	TUint32*	addressInRom = NULL;
	if (useRFile)
		{
		// open rfile
		RFile file;
		err = file.Open(FileServer(), fileName, EFileShareReadersOnly);

		// check if failed to open file
		if ( err != KErrNone )
			{
			ERR_PRINTF2(_L("File open error: %d"), err);
			SetBlockResult(EFail);
			}
		else
			{
			// call IsFileInRom()
			isFileInRom = CFbsBitmap::IsFileInRom(file, addressInRom);

			// close rfile
			file.Close();
			}
		}
	else
		{
		isFileInRom = CFbsBitmap::IsFileInRom(fileName, addressInRom);
		}

	// get expected value if is in ROM from parameters
	TBool	expectedIsFileInRom = EFalse;
	if ( GetBoolFromConfig(aSection, KExpectedBool(), expectedIsFileInRom) )
		{
		// check the expectation
		if (expectedIsFileInRom != isFileInRom)
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expectedIsFileInRom, isFileInRom);
			SetBlockResult(EFail);
			}
		}

	// check that address is not null if in rom
	if ( isFileInRom && addressInRom == NULL )
		{
		ERR_PRINTF1(_L("The file is in ROM but returned address is NULL"));
		SetBlockResult(EFail);
		}
	}

/** Calls CFbsBitmap::IsLargeBitmap() */
void CT_DataFbsBitmap::DoCmdIsLargeBitmap(const TDesC& aSection)
	{
	// call IsLargeBitmap()
	TBool	value = GetFbsBitmap()->IsLargeBitmap();
	INFO_PRINTF2(_L("Is large bitmap: %d"), value);
	
	// validate returned value if needed
	TBool	expectedBool;
	if(GetIntFromConfig(aSection, KExpectedBool(), expectedBool))
		{
		if (expectedBool != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedBool, value);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::IsMonochrome() */
void CT_DataFbsBitmap::DoCmdIsMonochrome(const TDesC& aSection)
	{

	// call IsMonochrome()
	TBool	value = GetFbsBitmap()->IsMonochrome();
	INFO_PRINTF2(_L("Is monochrome: %d"), value);

	// validate returned value if needed
	TBool	expectedBool;
	if(GetIntFromConfig(aSection, KExpectedBool(), expectedBool))
		{
		if (expectedBool != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedBool, value);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::IsRomBitmap() and check against expectation */
void CT_DataFbsBitmap::DoCmdIsRomBitmap(const TDesC& aSection)
	{
	// call IsRomBitmap()
	TBool	actualValue = GetFbsBitmap()->IsRomBitmap();
	INFO_PRINTF2(_L("CFbsBitmap::IsRomBitmap() %d"), actualValue);

	// get expected value if is in ROM from parameters
	TBool	expectedIsRomBitmap = EFalse;
	if ( GetBoolFromConfig(aSection, KExpectedBool(), expectedIsRomBitmap) )
		{
		// check the expectation
		if (expectedIsRomBitmap != actualValue)
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expectedIsRomBitmap, actualValue);
			SetBlockResult(EFail);
			}
		}
	}

/** CFbsBitmap::Load() Call */
void CT_DataFbsBitmap::DoCmdLoadL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("CFbsBitmap::Load() Call"));
	LoadOrLoadAndCompressL(aSection, EFalse);
	}

/** CFbsBitmap::LoadAndCompress() Call */
void CT_DataFbsBitmap::DoCmdLoadAndCompressL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("CFbsBitmap::LoadAndCompress() Call"));
	LoadOrLoadAndCompressL(aSection, ETrue);
	}

/** Calls static CFbsBitmap destructor */
void CT_DataFbsBitmap::DoCmdLockHeap(const TDesC& aSection)
	{
	TBool	always=EFalse;
	GetBoolFromConfig(aSection, KAlways(), always);
	GetFbsBitmap()->LockHeap(always);
	}

/** Calls static CFbsBitmap destructor */
void CT_DataFbsBitmap::DoCmdLockHeapLC(const TDesC& aSection)
	{
	TBool	always=EFalse;
	GetBoolFromConfig(aSection, KAlways(), always);

	GetFbsBitmap()->LockHeapLC(always);
	iPopWhenUnlocking=ETrue;
	}

/** Calls CFbsBitmap::PaletteAttributes() */
void CT_DataFbsBitmap::DoCmdPaletteAttributes(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::PaletteAttributes()"));

	// call PaletteAttributes()
	TBool	modifiable;
	TInt	entries;
	GetFbsBitmap()->PaletteAttributes(modifiable, entries);

	INFO_PRINTF3(_L("Modifiable: %d, Number of entries: %d"), modifiable, entries);

	// validate returned value if needed
	TBool	expectedModifiable;
	if(GetBoolFromConfig(aSection, KModifiable(), expectedModifiable))
		{
		if (expectedModifiable != modifiable)
			{
			ERR_PRINTF3(_L("The returned modifaiable value is not as expected, expected: %d, actual: %d"), expectedModifiable, modifiable);
			SetBlockResult(EFail);
			}
		}

	// validate returned value if needed
	TBool	expectedEntries;
	if(GetIntFromConfig(aSection, KEntries(), expectedEntries))
		{
		if (expectedEntries != entries)
			{
			ERR_PRINTF3(_L("The returned modifaiable value is not as expected, expected: %d, actual: %d"), expectedEntries, entries);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::Reset() */
void CT_DataFbsBitmap::DoCmdReset()
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Reset()"));

	// call Reset()
	GetFbsBitmap()->Reset();
	}

/** Calls CFbsBitmap::Resize() */
void CT_DataFbsBitmap::DoCmdResize(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Resize()"));

	// get size height from parameters
	TInt	height;
	if(!GetIntFromConfig(aSection, KHeight(), height))
		{
		ERR_PRINTF2(_L("No %S"), &KHeight());
		SetBlockResult(EFail);
		}

	// get size width from parameters
	TInt	width;
	if(!GetIntFromConfig(aSection, KWidth(), width))
		{
		ERR_PRINTF2(_L("No %S"), &KWidth());
		SetBlockResult(EFail);
		}

	// call Resize()
	TInt	err = GetFbsBitmap()->Resize(TSize(width, height));

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Resize error: %d"), err);
		SetError(err);
		}
	}

/** Calls CFbsBitmap::Save() */
void CT_DataFbsBitmap::DoCmdSave(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Save()"));

	// get file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// get flag if use RFile instance in Save() call from parameters
	TBool	useRFile = EFalse;
	if(!GetBoolFromConfig(aSection, KUseRFile(), useRFile))
		{
		ERR_PRINTF2(_L("No %S"), &KUseRFile());
		SetBlockResult(EFail);
		}

	// get flag if use RFile instance in Load() call from parameters
	TBool	useClosedRFile = EFalse;
	if(GetBoolFromConfig(aSection, KUseClosedRFile(), useClosedRFile))
		{
		if (useClosedRFile && !useRFile)
			{
			ERR_PRINTF3(_L("Cannot use %S parameter if %S is not set to TRUE"), &KUseClosedRFile(), &KUseRFile());
			SetBlockResult(EFail);
			}
		}

	TInt	err = KErrNone;
	if (useRFile)
		{
		// required to allow file handle to be adopted by another process
		FileServer().ShareProtected();

		// open or create rfile
		RFile	file;

		if (!useClosedRFile)
			{
			err = file.Open(FileServer(), fileName, EFileWrite);

			if (err == KErrNotFound)
				{
				err = file.Create(FileServer(), fileName, EFileWrite);

				// check if failed to create file
				if (err != KErrNone)
					{
					ERR_PRINTF2(_L("File create error: %d"), err);
					SetBlockResult(EFail);
					}
				}
			else if (err != KErrNone)
				{
				// if failed to open file
				ERR_PRINTF2(_L("File open error: %d"), err);
				SetBlockResult(EFail);
				}
			}

		// call Save()
		err = GetFbsBitmap()->Save(file);

		// close rfile
		file.Close();
		}
	else
		{
		err = GetFbsBitmap()->Save(fileName);
		}

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Save error: %d"), err);
		SetError(err);
		}
	}

/** Calls CFbsBitmap::ScanLineLength() */
void CT_DataFbsBitmap::DoCmdScanLineLength(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::ScanLineLength()"));

	// get length from parameters
	TInt length = 0;
	if(!GetIntFromConfig(aSection, KLength(), length))
		{
		ERR_PRINTF2(_L("No %S"), &KLength());
		SetBlockResult(EFail);
		}

	// get display mode from parameters
	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		}
	else
		{
		TInt actualValue = CFbsBitmap::ScanLineLength(length, displayMode);
		INFO_PRINTF2(_L("CFbsBitmap::ScanLineLength() %d"), actualValue);
	
		TInt expectedValue = 0;
		if ( GetIntFromConfig(aSection, KExpectedInt(), expectedValue) )
			{
			if (actualValue != expectedValue)
				{
				ERR_PRINTF3(_L("Expected value doesn't match to actual! expected: %d, actual: %d"), expectedValue, actualValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

/** Calls CFbsBitmap::SetDisplayMode() */
void CT_DataFbsBitmap::DoCmdSetDisplayMode(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::SetDisplayMode()"));

	// get display mode from parameters
	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		}
	else
		{
		// call SetDisplayMode()
		TInt	err = GetFbsBitmap()->SetDisplayMode(displayMode);

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Set display mode error: %d"), err);
			SetError(err);
			}
		}
	}

/** Calls CFbsBitmap::SetPalette() by previously created palette */
void CT_DataFbsBitmap::DoCmdSetPaletteL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::SetPalette() by previously created palette"));

	// get CPalette object passed as a parameter
	CPalette*	palette = NULL;

	CT_GraphicsUtil::GetPaletteL(*this, aSection, KPalette(), palette);

	// call SetPalette()
	GetFbsBitmap()->SetPalette(palette);
	}

/** Calls CFbsBitmap::SetRomBitmapL() by another bitmap */
void CT_DataFbsBitmap::DoCmdSetRomBitmapL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::SetRomBitmapL() by another bitmap"));

	// get fbsBitmap passed as a parameter
	CFbsBitmap*		otherFbsBitmap=NULL;

	CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFbsBitmapName(), otherFbsBitmap);

	// get romPointer. If the bitmap is ROM bitmap then Handle() returns ROM pointer
	CBitwiseBitmap*	bitmapRomAddress=NULL;
	if ( otherFbsBitmap!=NULL )
		{
		bitmapRomAddress=REINTERPRET_CAST(CBitwiseBitmap*, otherFbsBitmap->Handle());
		}

	// call SetRomBitmapL()
	TInt	bitmapSizeInBytes=0;
	TRAPD(err, GetFbsBitmap()->SetRomBitmapL(bitmapRomAddress, bitmapSizeInBytes));

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Set ROM bitmap error: %d"), err);
		SetError(err);
		}
	}

/** Calls CFbsBitmap::SetScanLine() */
void CT_DataFbsBitmap::DoCmdSetScanLineL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::SetScanLine()"));

	// get scanline coordinate from parameters
	TInt	coordinate = 0;
	if(!GetIntFromConfig(aSection, KScanLineCoord(), coordinate))
		{
		ERR_PRINTF2(_L("No %S"), &KScanLineCoord());
		SetBlockResult(EFail);
		}

	// get file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// open file
	RFile	file;
	User::LeaveIfError(file.Open(FileServer(), fileName, EFileRead | EFileShareAny));
	CleanupClosePushL(file);

	// get size
	TInt	size = 0;
	User::LeaveIfError(file.Size(size));

	// read file
    HBufC8*	fileDataBuff = HBufC8::NewLC(size);
    TPtr8	fileData = fileDataBuff->Des();

	User::LeaveIfError(file.Read(fileData));

	GetFbsBitmap()->SetScanLine(fileData, coordinate);

    CleanupStack::PopAndDestroy(2, &file);  // fileDataBuff, file
	}

/** Calls CFbsBitmap::SetSizeInTwips() by size parameter */
void CT_DataFbsBitmap::DoCmdSetSizeInTwipsL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::SetSizeInTwips()"));

	// get if to NULL device pointer flag from parameters
	TPtrC	device;
	if ( GetStringFromConfig(aSection, KDevice(), device) )
		{
		CFbsBitmapDevice*	bitmapDevice=static_cast<CFbsBitmapDevice*>(GetDataObjectL(device));
		GetFbsBitmap()->SetSizeInTwips(bitmapDevice);
		}
	else
		{
		// get size height from parameters
		TInt	height;
		if(!GetIntFromConfig(aSection, KHeight(), height))
			{
			ERR_PRINTF2(_L("No %S"), &KHeight());
			SetBlockResult(EFail);
			}

		// get size width from parameters
		TInt	width;
		if(!GetIntFromConfig(aSection, KWidth(), width))
			{
			ERR_PRINTF2(_L("No %S"), &KWidth());
			SetBlockResult(EFail);
			}

		// call SetSizeInTwips()
		GetFbsBitmap()->SetSizeInTwips(TSize(width, height));
		}
	}

/** Calls CFbsBitmap::SizeInPixels() and checks the size */
void CT_DataFbsBitmap::DoCmdSizeInPixels(const TDesC& aSection)
	{
	// call SizeInTwips()
	TSize	actualSize = GetFbsBitmap()->SizeInPixels();
	INFO_PRINTF3(_L("CFbsBitmap::SizeInPixels() height %d, width %d"), actualSize.iHeight, actualSize.iWidth);

	// get size height from parameters
	TInt	height;
	if ( GetIntFromConfig(aSection, KHeight(), height) )
		{
		if ( actualSize.iHeight != height )
			{
			ERR_PRINTF3(_L("Height mismatch expected: %d, actual: %d"), height, actualSize.iHeight);
			SetBlockResult(EFail);
			}
		}

	// get size width from parameters
	TInt	width;
	if ( GetIntFromConfig(aSection, KWidth(), width) )
		{
		// check that value is as expected
		if ( actualSize.iWidth != width )
			{
			ERR_PRINTF3(_L("Width mismatch expected: %d, actual: %d"), width, actualSize.iWidth);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::SizeInTwips() and compares the result with expected */
void CT_DataFbsBitmap::DoCmdSizeInTwips(const TDesC& aSection)
	{
	// call SizeInTwips()
	TSize	actualSize = GetFbsBitmap()->SizeInTwips();
	INFO_PRINTF3(_L("CFbsBitmap::SizeInTwips() height %d, width %d"), actualSize.iHeight, actualSize.iWidth);

	// get size height from parameters
	TInt	height;
	if ( GetIntFromConfig(aSection, KHeight(), height) )
		{
		if ( actualSize.iHeight != height )
			{
			ERR_PRINTF3(_L("Height mismatch expected: %d, actual: %d"), height, actualSize.iHeight);
			SetBlockResult(EFail);
			}
		}

	// get size width from parameters
	TInt	width;
	if ( GetIntFromConfig(aSection, KWidth(), width) )
		{
		// check that value is as expected
		if ( actualSize.iWidth != width )
			{
			ERR_PRINTF3(_L("Width mismatch expected: %d, actual: %d"), width, actualSize.iWidth);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::StoreL() */
void CT_DataFbsBitmap::DoCmdStoreL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::StoreL()"));

	TInt	numberOfFiles = 0;
	TInt	numberOfBitmapIds = 0;
	TPtrC	sourceFileName1;
	TPtrC	sourceFileName2;
	TInt	sourceBitmapId0 = -1;
	TInt	sourceBitmapId1 = -1;
	TInt	sourceBitmapId2 = -1;

	// get file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// get flag if use RFile instance in Save() call from parameters
	TBool	useRFile = EFalse;
	if(!GetBoolFromConfig(aSection, KUseRFile(), useRFile))
		{
		ERR_PRINTF2(_L("No %S"), &KUseRFile());
		SetBlockResult(EFail);
		}

	// get flag if use RFile instance in Load() call from parameters
	TBool	useClosedRFile = EFalse;
	if(GetBoolFromConfig(aSection, KUseClosedRFile(), useClosedRFile))
		{
		if (useClosedRFile && !useRFile)
			{
			ERR_PRINTF3(_L("Cannot use %S parameter if %S is not set to TRUE"), &KUseClosedRFile(), &KUseRFile());
			SetBlockResult(EFail);
			}
		}

	// get up to 3 source file names from parameters
	TPtrC	sourceFileName0;
	if (GetStringFromConfig(aSection, KSourceFileName0(), sourceFileName0))
		{
		numberOfFiles++;
		if (GetStringFromConfig(aSection, KSourceFileName1(), sourceFileName1))
			{
			numberOfFiles++;
			if (GetStringFromConfig(aSection, KSourceFileName2(), sourceFileName2))
				{
				numberOfFiles++;
				}
			}
		}

	// get up to 3 source bitmap ids from parameters
	if (GetIntFromConfig(aSection, KSourceBitmapId0(), sourceBitmapId0))
		{
		numberOfBitmapIds++;
		if (GetIntFromConfig(aSection, KSourceBitmapId1(), sourceBitmapId1))
			{
			numberOfBitmapIds++;
			if (GetIntFromConfig(aSection, KSourceBitmapId2(), sourceBitmapId2))
				{
				numberOfBitmapIds++;
				}
			}
		}

	// get number of sources argument from parameters
	TInt	numberOfSources;
	if(!GetIntFromConfig(aSection, KNumberOfSources(), numberOfSources))
		{
		ERR_PRINTF2(_L("No %S"), &KNumberOfSources());
		SetBlockResult(EFail);
		}

	// create array of file names
 	TFileName** sourceFiles = new (ELeave) TFileName*[numberOfFiles];
 	CleanupStack::PushL(sourceFiles);
	if (numberOfFiles > 0) sourceFiles[0] = new (ELeave) TFileName(sourceFileName0);
	if (numberOfFiles > 1) sourceFiles[1] = new (ELeave) TFileName(sourceFileName1);
	if (numberOfFiles > 2) sourceFiles[2] = new (ELeave) TFileName(sourceFileName2);

	// create array of bitmap ids
	TInt32* bitmapIds = new (ELeave) TInt32[numberOfBitmapIds];
	CleanupStack::PushL(bitmapIds);

	if (numberOfBitmapIds > 0) bitmapIds[0] = sourceBitmapId0;
	if (numberOfBitmapIds > 1) bitmapIds[1] = sourceBitmapId1;
	if (numberOfBitmapIds > 2) bitmapIds[2] = sourceBitmapId2;

	TInt err = KErrNone;
	if (useRFile)
		{
		// required to allow file handle to be adopted by another process
		FileServer().ShareProtected();

		// open rfile
		TBool	openSuccess = EFalse;

		RFile file;
		err = file.Open(FileServer(), fileName, EFileWrite);

		// check if ok, if not fount => try to create new file
		if (err == KErrNone)
			{
			openSuccess = ETrue;
			}
		else if (err == KErrNotFound)
			{
			INFO_PRINTF1(_L("File doesn't exist, trying to create it."));

			err = file.Create(FileServer(), fileName, EFileWrite);

			// check if failed to create file
			if (err == KErrNone)
				{
				openSuccess = ETrue;
				}
			else
				{
				ERR_PRINTF2(_L("File create error: %d"), err);
				SetBlockResult(EFail);
				}
			}
		else
			{
			ERR_PRINTF2(_L("File open error: %d"), err);
			SetBlockResult(EFail);
			}

		// if ok => do store
		if (openSuccess)
			{
			// if we use invalid RFile handle in our test
			if (useClosedRFile)
				{
				file.Close();
				}

			// call StoreL()
			TRAP(err, CFbsBitmap::StoreL(file, numberOfSources, (const TDesC**) sourceFiles, bitmapIds));

			// check error code
			if (err != KErrNone)
				{
				ERR_PRINTF2(_L("Store error: %d"), err);
				SetError(err);
				}

			// close rfile
			file.Close();
			}
		}
	else
		{
		// call StoreL()
		TRAP(err, CFbsBitmap::StoreL(fileName, numberOfSources, (const TDesC**) sourceFiles, bitmapIds));

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Store error: %d"), err);
			SetError(err);
			}
		}

 	// Iterate over the file name pointer array and cleanup
 	for (TInt i = 0; i < numberOfFiles; ++i)
 		{
		delete sourceFiles[i];
   		}

 	// Cleanup the arrays allocated on the heap
 	CleanupStack::PopAndDestroy(2, sourceFiles);
	}

/** Calls CFbsBitmap::SwapWidthAndHeight() */
void CT_DataFbsBitmap::DoCmdSwapWidthAndHeight()
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::SwapWidthAndHeight()"));

	// call SwapWidthAndHeight()
	TInt	err = GetFbsBitmap()->SwapWidthAndHeight();

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Swap width and height error: %d"), err);
		SetError(err);
		}
	}

/** Calls static CFbsBitmap destructor */
void CT_DataFbsBitmap::DoCmdUnlockHeapL(const TDesC& aSection)
	{

	CFbsBitmap*	bitmap=NULL;
	if ( CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFbsBitmapName(), bitmap) )
		{
		CFbsBitmap::UnlockHeap(bitmap);
		}
	else
		{
		TBool	always=EFalse;
		GetBoolFromConfig(aSection, KAlways(), always);

		GetFbsBitmap()->UnlockHeap(always);
		}

	if ( iPopWhenUnlocking )
		{
		iPopWhenUnlocking=EFalse;
		CleanupStack::Pop();
		}
	}

/** Calls CFbsBitmap::VerticalPixelsToTwips() */
void CT_DataFbsBitmap::DoCmdVerticalPixelsToTwips(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::VerticalPixelsToTwips()"));

	// get pixels from parameters
	TInt	pixels;
	if(!GetIntFromConfig(aSection, KPixels(), pixels))
		{
		ERR_PRINTF2(_L("No %S"), &KPixels());
		SetBlockResult(EFail);
		}

	// call VerticalPixelsToTwips()
	TInt	value = GetFbsBitmap()->VerticalPixelsToTwips(pixels);
	INFO_PRINTF2(_L("Twips: %d"), value);

	// validate returned value if needed
	TInt	expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, value);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::VerticalTwipsToPixels() */
void CT_DataFbsBitmap::DoCmdVerticalTwipsToPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::VerticalTwipsToPixels()"));

	// get twips from parameters
	TInt	twips;
	if(!GetIntFromConfig(aSection, KTwips(), twips))
		{
		ERR_PRINTF2(_L("No %S"), &KTwips());
		SetBlockResult(EFail);
		}

	// call VerticalTwipsToPixels()
	TInt	value = GetFbsBitmap()->VerticalTwipsToPixels(twips);
	INFO_PRINTF2(_L("Pixels: %d"), value);

	// validate returned value if needed
	TInt expectedValue;
	if(GetIntFromConfig(aSection, KExpectedInt(), expectedValue))
		{
		if (expectedValue != value)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, value);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls CFbsBitmap::ExternalizeL() or CFbsBitmap::ExternalizeRectangleL(). Used by DoCmdExternalize() and DoCmdExternalizeRectangle() methods */
void CT_DataFbsBitmap::ExternalizeImplL(const TDesC& aSection, const TRect& aRect, TBool aUseRect)
	{
	// get file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// get a flag if to use closed stream from parameters
	TBool	useClosedStream = EFalse;
	if(!GetBoolFromConfig(aSection, KUseClosedStream(), useClosedStream))
		{
		ERR_PRINTF2(_L("No %S"), &KUseClosedStream());
		SetBlockResult(EFail);
		}

	// create write store
	CDirectFileStore*	writestore = CDirectFileStore::ReplaceL(FileServer(), fileName, EFileStream | EFileWrite);
	CleanupStack::PushL(writestore);
	TInt				popCount=1;
	TUidType 			uidtype(KDirectFileStoreLayoutUid, KMultiBitmapFileImageUid);

	writestore->SetTypeL(uidtype);

	// create write stream
	RStoreWriteStream	writestrm;

	// close stream set by parameters
	if (!useClosedStream)
		{
		TStreamId	headerid=writestrm.CreateL(*writestore);
		CleanupClosePushL(writestrm);

		++popCount;
		writestore->SetRootL(headerid);
		}

	if (aUseRect)
		{
		// call ExternalizeRectangleL()
		TRAPD(err, GetFbsBitmap()->ExternalizeRectangleL(writestrm, aRect));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Externalize rectangle error: %d"), err);
			SetError(err);
			}
		}
	else
		{
		// call ExternalizaL()
		TRAPD(err, GetFbsBitmap()->ExternalizeL(writestrm));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("Externalize error: %d"), err);
			SetError(err);
			}
		}

	// cleanup
	CleanupStack::PopAndDestroy(popCount, writestore);
	}

/** Calls CFbsBitmap::Load() or CFbsBitmap::LoadAndCompress(). Used by DoCmdLoad() and DoCmdLoadAndCompress() methods */
void CT_DataFbsBitmap::LoadOrLoadAndCompressL(const TDesC& aSection, TBool aCompress)
	{

	// get bitmap id from parameters
	TInt	bitmapId;
	if(!GetIntFromConfig(aSection, KBitmapId(), bitmapId))
		{
		ERR_PRINTF2(_L("No %S"), &KBitmapId());
		SetBlockResult(EFail);
		}

	// get file name from parameters
	TPtrC	fileName;
	if (!GetStringFromConfig(aSection, KFileName(), fileName))
		{
		ERR_PRINTF2(_L("No %S"), &KFileName());
		SetBlockResult(EFail);
		}

	// get file offset from parameters
	TInt	fileOffset;
	TBool	useOffset = GetIntFromConfig(aSection, KFileOffset(), fileOffset);

	// get share if loaded from parameters
	TBool	shareIfLoaded = EFalse;
	GetBoolFromConfig(aSection, KShareIfLoaded(), shareIfLoaded);

	// get flag if use RFile instance in Load() call from parameters
	TBool	useRFile = EFalse;
	GetBoolFromConfig(aSection, KUseRFile(), useRFile);

	// get flag if use RFile instance in Load() call from parameters
	TBool	useClosedRFile = EFalse;
	GetBoolFromConfig(aSection, KUseClosedRFile(), useClosedRFile);

	if (useClosedRFile && !useRFile)
		{
		ERR_PRINTF3(_L("Cannot use %S parameter if %S is not set to TRUE"), &KUseClosedRFile(), &KUseRFile());
		SetBlockResult(EFail);
		}

	// load bitmap
	TInt	err = KErrNone;
	if (useRFile)
		{
		// required to allow file handle to be adopted by another process
		FileServer().ShareProtected();

		// open rfile
		RFile	file;

		// if we use invalid RFile handle in our test
		if (!useClosedRFile)
			{
			User::LeaveIfError(file.Open(FileServer(), fileName, EFileShareReadersOnly));
			CleanupClosePushL(file);
			}

		// do load
		if (aCompress)
			{
			if (useOffset)
				{
				err = GetFbsBitmap()->LoadAndCompress(file, bitmapId, shareIfLoaded, fileOffset);
				}
			else
				{
				err = GetFbsBitmap()->LoadAndCompress(file, bitmapId, shareIfLoaded);
				}
			}
		else
			{
			if (useOffset)
				{
				err = GetFbsBitmap()->Load(file, bitmapId, shareIfLoaded, fileOffset);
				}
			else
				{
				err = GetFbsBitmap()->Load(file, bitmapId, shareIfLoaded);
				}
			}

		if (!useClosedRFile)
			{
			CleanupStack::PopAndDestroy(&file);
			}
		}
	else
		{
		if (aCompress)
			{
			if (useOffset)
				{
				err = GetFbsBitmap()->LoadAndCompress(fileName, bitmapId, shareIfLoaded, fileOffset);
				}
			else
				{
				err = GetFbsBitmap()->LoadAndCompress(fileName, bitmapId, shareIfLoaded);
				}
			}
		else
			{
			if (useOffset)
				{
				err = GetFbsBitmap()->Load(fileName, bitmapId, shareIfLoaded, fileOffset);
				}
			else
				{
				err = GetFbsBitmap()->Load(fileName, bitmapId, shareIfLoaded);
				}
			}
		}

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Load error: %d"), err);
		SetError(err);
		}
	}

/**
 Virtual RunL - Called on completion of an asynchronous command
 @see MTPActiveCallback
 @param aActive Active Object that RunL has been called on
 @pre N/A
 @post N/A
 @leave system wide error code
*/
void CT_DataFbsBitmap::RunL(CActive* aActive, const TInt aIndex)
	{
	if ( aActive==iActiveCompressInBackground )
		{
		RunCompressInBackground(aIndex);
		}
	else
		{
		ERR_PRINTF1(_L("Stray signal"));
		SetBlockResult(EFail);
		}
	DecOutstanding();
	}

/**
 Virtual DoCancel - Request to cancel the asynchronous command
 @see - MTPActiveCallback
 @param aActive Active Object that DoCancel has been called on
 @pre - N/A
 @post - N/A
 @leave system wide error code
*/
void CT_DataFbsBitmap::DoCancel(CActive* aActive, const TInt aIndex)
	{
	if ( aActive==iActiveCompressInBackground )
		{
		DoCancelCompressInBackground(aIndex);
		}
	else
		{
		ERR_PRINTF1(_L("Stray signal"));
		SetBlockResult(EFail);
		}
	DecOutstanding();
	}

void CT_DataFbsBitmap::RunCompressInBackground(const TInt aIndex)
	{
	TInt	err=iActiveCompressInBackground->iStatus.Int();
	INFO_PRINTF2(_L("RunCompressInBackground %d"), err);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("RunL Error %d"), err);
		SetAsyncError(aIndex, err);
		}
	}

void CT_DataFbsBitmap::DoCancelCompressInBackground(const TInt /*aIndex*/)
	{
	}
