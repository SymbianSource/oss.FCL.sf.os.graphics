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

This contains CT_DataLinkedTypefaceSpecification
*/

#include "T_DataLinkedTypefaceSpecification.h"


/*@{*/
///	Parameters
_LIT(KFldTypefaceName,							"typefacename");

///	Commands
_LIT(KCmdNewL,									"NewL");
_LIT(KCmdDestructor,							"~");

/// Logs
_LIT(KLogMissingParameter,						"Missing parameter '%S'");
_LIT(KLogError,									"Error=%d");
/*@}*/


/**
* Two phase constructor
*/
CT_DataLinkedTypefaceSpecification* CT_DataLinkedTypefaceSpecification::NewL()
	{
	CT_DataLinkedTypefaceSpecification* ret = new (ELeave) CT_DataLinkedTypefaceSpecification();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
* Protected second phase construction
*/
void CT_DataLinkedTypefaceSpecification::ConstructL()
	{
	}

/**
* Protected constructor. First phase construction
*/
CT_DataLinkedTypefaceSpecification::CT_DataLinkedTypefaceSpecification()
:	CDataWrapperBase()
,	iSpec(NULL)
	{
	}

/**
* Destructor.
*/
CT_DataLinkedTypefaceSpecification::~CT_DataLinkedTypefaceSpecification()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataLinkedTypefaceSpecification::DestroyData()
	{
	delete iSpec;
	iSpec = NULL;
	}

TAny* CT_DataLinkedTypefaceSpecification::GetObject()
	{
	return iSpec;
	}

void CT_DataLinkedTypefaceSpecification::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iSpec	= static_cast<CLinkedTypefaceSpecification*> (aObject);
	}
		
void CT_DataLinkedTypefaceSpecification::DisownObjectL() 
	{
	iSpec = NULL;
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
TBool CT_DataLinkedTypefaceSpecification::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
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
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Creates an instance of CT_DataLinkedTypefaceSpecification class */
void CT_DataLinkedTypefaceSpecification::DoCmdNewL(const TDesC& aSection)
	{
	// cleanup if any
	DestroyData();

	// call new operator
	TPtrC name;
	if (!GetStringFromConfig(aSection, KFldTypefaceName, name))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldTypefaceName);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute CLinkedTypefaceSpecification::NewLC"));
		TRAPD(err, iSpec = CLinkedTypefaceSpecification::NewLC(name); CleanupStack::Pop(iSpec));
		if (err != KErrNone)
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataLinkedTypefaceSpecification::DoCmdDestructor()
	{
	// cleanup if any
	DestroyData();
	}
