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

This contains CT_DataAlgStyle
*/

#include "T_DataAlgStyle.h"


/*@{*/
///	Parameters
_LIT(KFactor,									"factor");

///	Commands
_LIT(KCmdNew,									"new");
_LIT(KCmdSetWidthFactor,						"SetWidthFactor");
_LIT(KCmdSetHeightFactor,						"SetHeightFactor");
_LIT(KCleanup,									"~");

/*@}*/


/**
* Two phase constructor
*/
CT_DataAlgStyle* CT_DataAlgStyle::NewL()
	{
	CT_DataAlgStyle* ret = new (ELeave) CT_DataAlgStyle();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataAlgStyle::CT_DataAlgStyle()
:	CDataWrapperBase()
,	iAlgStyle(NULL)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataAlgStyle::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataAlgStyle::~CT_DataAlgStyle()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataAlgStyle::DestroyData()
	{
	delete iAlgStyle;
	iAlgStyle = NULL;
	}

TAny* CT_DataAlgStyle::GetObject()
	{
	return iAlgStyle;
	}

void CT_DataAlgStyle::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iAlgStyle	= static_cast<TAlgStyle*> (aObject);
	}
		
void CT_DataAlgStyle::DisownObjectL() 
	{
	iAlgStyle = NULL;
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
TBool CT_DataAlgStyle::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNew();
		}
	else if (aCommand == KCmdSetWidthFactor)
		{
		DoCmdSetWidthFactor(aSection);
		}
	else if (aCommand == KCmdSetHeightFactor)
		{
		DoCmdSetHeightFactor(aSection);
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

/** Creates an instance of TAlgSAtyle structure */
void CT_DataAlgStyle::DoCmdNew()
	{
	INFO_PRINTF1(_L("Creates an instance of TAlgSAtyle structure"));

	// cleanup if any
	DestroyData();

	// call new operator
	TRAPD(err, iAlgStyle = new (ELeave) TAlgStyle());

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating an instance: %d"), err);
		SetError(err);
		}
	}


/** Calls TAlgSAtyle::SetWidthFactor() */
void CT_DataAlgStyle::DoCmdSetWidthFactor(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls TAlgSAtyle::SetWidthFactor()"));
	// get factor from parameters
	TInt	factor;
	if (!GetIntFromConfig(aSection, KFactor(), factor) )
		{
		ERR_PRINTF2(_L("No %S"), &KFactor());
		SetBlockResult(EFail);
		}

	// call SetWidthFactor()		
	iAlgStyle->SetWidthFactor(factor);
	}


/** Calls TAlgSAtyle::SetWidthFactor() */
void CT_DataAlgStyle::DoCmdSetHeightFactor(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls TAlgSAtyle::SetHeightFactor()"));

	// get factor from parameters
	TInt	factor;
	if (!GetIntFromConfig(aSection, KFactor(), factor) )
		{
		ERR_PRINTF2(_L("No %S"), &KFactor());
		SetBlockResult(EFail);
		}

	// call SetHeightFactor()		
	iAlgStyle->SetHeightFactor(factor);
	}


