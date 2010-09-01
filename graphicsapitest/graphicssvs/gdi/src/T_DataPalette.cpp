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

This contains CT_DataPalette
*/

#include "T_DataPalette.h"
#include "T_GraphicsUtil.h"


/*@{*/
///	Parameters
_LIT(KNumberOfColors,						"number_of_colors");
_LIT(KDisplayMode,							"displaymode");

///	Commands
_LIT(KCmdNewL,								"NewL");
_LIT(KCmdNewDefaultL,						"NewDefaultL");
_LIT(KCleanup,								"~");
/*@}*/


/**
* Two phase constructor
*/
CT_DataPalette* CT_DataPalette::NewL()
	{
	CT_DataPalette* ret = new (ELeave) CT_DataPalette();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataPalette::CT_DataPalette()
:	CDataWrapperBase()
,	iPalette(NULL)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataPalette::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataPalette::~CT_DataPalette()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataPalette::DestroyData()
	{
	delete iPalette;
	iPalette = NULL;
	}


/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataPalette::GetObject()
	{
	return iPalette;
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
TBool CT_DataPalette::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNewL)
		{
		DoCmdNew(aSection);
		}
	else if (aCommand == KCmdNewDefaultL)
		{
		DoCmdNewDefault(aSection);
		}
	else if (aCommand == KCleanup)
		{
		DestroyData();
		}
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Creates a default palette for the display mode specified */
void CT_DataPalette::DoCmdNew(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CPalette::NewL() creates a new palette with a given number of colors"));
	// cleanup if any
	delete iPalette;
	iPalette = NULL;

	// get number of colors from parameters
	TInt	numberOfColors;
	if(!GetIntFromConfig(aSection, KNumberOfColors(), numberOfColors))
		{
		ERR_PRINTF2(_L("No %S"), &KNumberOfColors());
		SetBlockResult(EFail);
		}
	else
		{
		// create new palette
		TRAPD(err, iPalette = CPalette::NewL(numberOfColors));

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("CPalette::NewL error: %d"), err);
			SetBlockResult(EFail);
			}
		}
	}


/** Creates a default palette for the display mode specified */
void CT_DataPalette::DoCmdNewDefault(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CPalette::NewDefaultL() creating a default palette for the display mode specified"));

	// cleanup if any
	delete iPalette;
	iPalette = NULL;

	// get display mode from parameters
	TDisplayMode	displayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDisplayMode(), displayMode) )
		{
		ERR_PRINTF1(_L("No display mode"));
		SetBlockResult(EFail);
		}
	else
		{
		// create new palette
		TRAPD(err, iPalette = CPalette::NewDefaultL(displayMode));

		// check error code
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("CPalette::NewDefaultL error: %d"), err);
			SetBlockResult(EFail);
			}
		}
	}
