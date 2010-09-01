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

This contains CT_DataGraphicsDevice
*/

#include "T_DataGraphicsDevice.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdCreateContext,								"CreateContext");
_LIT(KCmdDisplayMode,								"DisplayMode");
_LIT(KCmdFontHeightInTwips,							"FontHeightInTwips");
_LIT(KCmdGetPalette,								"GetPalette");
_LIT(KCmdNumTypefaces,								"NumTypefaces");
_LIT(KCmdPaletteAttributes,							"PaletteAttributes");
_LIT(KCmdSetPalette,								"SetPalette");
_LIT(KCmdSizeInPixels,								"SizeInPixels");
_LIT(KCmdSizeInTwips,								"SizeInTwips");
_LIT(KCmdTypefaceSupport,							"TypefaceSupport");

///	Fields
_LIT(KFldContext,									"context");
_LIT(KFldDisplayMode,								"displaymode");
_LIT(KFldExpected,									"expected");
_LIT(KFldExpectedEntries,							"expected_entries");
_LIT(KFldExpectedModifiable,						"expected_modifiable");
_LIT(KFldHeightIndex,								"height_index");
_LIT(KFldIndex,										"index");
_LIT(KFldPalette,									"palette");
_LIT(KFldTypefaceIndex,								"typeface_index");

///	Logging
_LIT(KLogError,										"Error=%d");
_LIT(KLogMissingParameter,							"Missing parameter '%S'");
_LIT(KLogMissingWrapper,							"Missing wrapper for '%S'");
_LIT(KLogNotExpectedValue,							"Not expected value");
/*@}*/


/**
* Constructor. First phase construction
*/
CT_DataGraphicsDevice::CT_DataGraphicsDevice()
:	CT_DataGraphicsDeviceMap()
,	iDisplayMode(ENone)
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
TBool CT_DataGraphicsDevice::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret=ETrue;
	if ( aCommand==KCmdCreateContext )
		{
		DoCmdCreateContextL(aSection);
		}
	else if ( aCommand==KCmdDisplayMode )
		{
		DoCmdDisplayMode(aSection);
		}
	else if ( aCommand==KCmdFontHeightInTwips )
		{
		DoCmdFontHeightInTwips(aSection);
		}
	else if ( aCommand==KCmdGetPalette )
		{
		DoCmdGetPaletteL(aSection);
		}
	else if ( aCommand==KCmdNumTypefaces )
		{
		DoCmdNumTypefaces(aSection);
		}
	else if ( aCommand==KCmdPaletteAttributes )
		{
		DoCmdPaletteAttributes(aSection);
		}
	else if ( aCommand==KCmdSetPalette )
		{
		DoCmdSetPaletteL(aSection);
		}
	else if ( aCommand==KCmdSizeInPixels )
		{
		DoCmdSizeInPixels(aSection);
		}
	else if ( aCommand==KCmdSizeInTwips )
		{
		DoCmdSizeInTwips(aSection);
		}
	else if ( aCommand==KCmdTypefaceSupport )
		{
		DoCmdTypefaceSupport(aSection);
		}
	else
		{
		ret=CT_DataGraphicsDeviceMap::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataGraphicsDevice::DoCmdCreateContextL(const TDesC& aSection)
	{
	TPtrC	wrapperName;
	// get BitmapUtilData object from parameters
	if ( !GetStringFromConfig(aSection, KFldContext, wrapperName) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldContext());
		SetBlockResult(EFail);
		}
	else
		{
		CDataWrapper*	wrapper=GetDataWrapperL(wrapperName);
		if ( wrapper==NULL )
			{
			ERR_PRINTF2(KLogMissingWrapper, &wrapperName);
			SetBlockResult(EFail);
			}
		else
			{
			CGraphicsContext*	context=NULL;
			INFO_PRINTF1(_L("execute CreateContext(CGraphicsContext*&)"));
			TInt	err=GetGraphicsDevice()->CreateContext(context);
			if ( err!=KErrNone )
				{
				ERR_PRINTF2(KLogError, err);
				SetError(err);
				}
			else
				{
				wrapper->SetObjectL(context);
				}
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdDisplayMode(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute DisplayMode()"));
	iDisplayMode=GetGraphicsDevice()->DisplayMode();
	INFO_PRINTF2(_L("Display Mode %d"), iDisplayMode);

	// get display mode from parameters
	TDisplayMode	expectedValue;
	if ( CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldDisplayMode(), expectedValue) )
		{
		// check value against NULL
		if ( iDisplayMode!=expectedValue )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdFontHeightInTwips(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// get typeface index from parameters
	TInt	typefaceIndex;
	if ( !GetIntFromConfig(aSection, KFldTypefaceIndex(), typefaceIndex) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldTypefaceIndex());
		SetBlockResult(EFail);
		}

	// get height index from parameters
	TInt	heightIndex;
	if ( !GetIntFromConfig(aSection, KFldHeightIndex(), heightIndex) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldHeightIndex());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute FontHeightInTwips(TInt, TInt)"));
		TInt	height=GetGraphicsDevice()->FontHeightInTwips(typefaceIndex, heightIndex);
		INFO_PRINTF2(_L("Height %d"), height);

		// get positive height check flag from parameters
		TInt	expected;
		if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
			{
			if ( height!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdGetPaletteL(const TDesC& aSection)
	{
	CPalette*	pallet=NULL;
	INFO_PRINTF1(_L("execute GetPalette(CPalette*&)"));
	TInt	err=GetGraphicsDevice()->GetPalette(pallet);

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	else
		{
		CPalette*	expected=NULL;
		if ( CT_GraphicsUtil::GetPaletteL(*this, aSection, KFldPalette(), expected) )
			{
			if ( pallet!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdNumTypefaces(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute NumTypefaces()"));
	TInt	actual=GetGraphicsDevice()->NumTypefaces();
	INFO_PRINTF2(_L("NumTypefaces=%d"), actual);

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

void CT_DataGraphicsDevice::DoCmdPaletteAttributes(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute PaletteAttributes(TBool&, TInt&)"));
	TBool	modifiable;
	TInt	entries;
	GetGraphicsDevice()->PaletteAttributes(modifiable, entries);
	INFO_PRINTF2(_L("modifiable %d"), modifiable);
	INFO_PRINTF2(_L("entries    %d"), entries);

	TBool	expectedModifiable;
	if ( GetBoolFromConfig(aSection, KFldExpectedModifiable(), expectedModifiable) )
		{
		if ( modifiable!=expectedModifiable )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}

	TInt	expectedEntries;
	if ( GetIntFromConfig(aSection, KFldExpectedEntries(), expectedEntries) )
		{
		if ( entries!=expectedEntries )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdSetPaletteL(const TDesC& aSection)
	{
	CPalette*	pallet=NULL;
	if ( !CT_GraphicsUtil::GetPaletteL(*this, aSection, KFldPalette(), pallet) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPalette());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetPalette(CPalette*)"));
		GetGraphicsDevice()->SetPalette(pallet);
		}
	}

void CT_DataGraphicsDevice::DoCmdSizeInPixels(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute SizeInPixels()"));
	iSizeInPixels=GetGraphicsDevice()->SizeInPixels();
	INFO_PRINTF3(_L("CGraphicsDevice::SizeInPixels() height %d, width %d"), iSizeInPixels.iHeight, iSizeInPixels.iWidth);
	
	TSize	expected;
	if ( GetSizeFromConfig(aSection, KFldExpected(), expected) )
		{
		if ( iSizeInPixels!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdSizeInTwips(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute SizeInTwips()"));
	iSizeInTwips=GetGraphicsDevice()->SizeInTwips();
	INFO_PRINTF3(_L("CGraphicsDevice::SizeInTwips() height %d, width %d"), iSizeInTwips.iHeight, iSizeInTwips.iWidth);
	
	TSize	expected;
	if ( GetSizeFromConfig(aSection, KFldExpected(), expected) )
		{
		if ( iSizeInTwips!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataGraphicsDevice::DoCmdTypefaceSupport(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// get typeface index from parameters
	TInt	index;
	if ( !GetIntFromConfig(aSection, KFldIndex(), index) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldIndex());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute TypefaceSupport(TTypefaceSupport&, TInt)"));
		GetGraphicsDevice()->TypefaceSupport(iTypefaceSupport, index);
		INFO_PRINTF2(_L("iIsScalable       %d"), iTypefaceSupport.iIsScalable);
		INFO_PRINTF2(_L("iMaxHeightInTwips %d"), iTypefaceSupport.iMaxHeightInTwips);
		INFO_PRINTF2(_L("iMinHeightInTwips %d"), iTypefaceSupport.iMinHeightInTwips);
		INFO_PRINTF2(_L("iNumHeights       %d"), iTypefaceSupport.iNumHeights);
		
		TTypefaceSupport expectedTypeface;
		if (CT_GraphicsUtil::ReadTypefaceSupport(*this, aSection, KFldExpected(),expectedTypeface ))
			{
			// just check font name
			if (expectedTypeface.iTypeface.iName!=iTypefaceSupport.iTypeface.iName )
				{
				ERR_PRINTF3(_L("DoCmdTypefaceSupport expected=%S actual=%S"), 
						&expectedTypeface.iTypeface.iName,&iTypefaceSupport.iTypeface.iName);
				SetBlockResult(EFail);
				}
			
			}
		}
	}
