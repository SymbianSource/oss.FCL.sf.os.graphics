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

#include "T_GraphicsUtil.h"
#include "T_DataWsPointerCursor.h"

/*@{*/
//Commands
_LIT(KCmdNew,							    "new");
_LIT(KCmdConstruct,						    "Construct");

// Fields
_LIT(KFldWs,							"ws");
_LIT(KFldFlag,                          "flag");

///	Logging
_LIT(KLogError,								"Error=%d");
_LIT(KLogMissingParameter,					"Missing parameter '%S'");

_LIT(KLogConstruct,                         "execute Construct(TInt aFlags)");
_LIT(KLogNew,                               "execute new RWsPointerCursor()");
_LIT(KLogNew2,                              "execute new RWsPointerCursor(RWsSession)");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsPointerCursor::CT_DataWsPointerCursor()
:	CT_DataWsSpriteBase(),iWsPointerCursor(NULL)
	{
	}

CT_DataWsPointerCursor*	CT_DataWsPointerCursor::NewL()
	{
	CT_DataWsPointerCursor* ret=new (ELeave) CT_DataWsPointerCursor();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

void CT_DataWsPointerCursor::ConstructL()
	{
	}

CT_DataWsPointerCursor::~CT_DataWsPointerCursor()
	{
	DestroyData();
	}

/**
 * Called by TEF framework to set object.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsPointerCursor::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsPointerCursor=static_cast<RWsPointerCursor*> (aAny);
	}

/**
 * Called by TEF framework to disown the object.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsPointerCursor::DisownObjectL()
	{
	iWsPointerCursor=NULL;
	}

/**
 * Destroy the object which has been constructed.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsPointerCursor::DestroyData()
	{
	delete iWsPointerCursor;
	iWsPointerCursor=NULL;
	}


/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @param aAsyncErrorIndex	index of command. used for async calls
*
* @return ETrue if the command is processed
*/
TBool CT_DataWsPointerCursor::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret=ETrue;

	if ( aCommand==KCmdNew )
		{
		DoCmdNewL(aSection);
		}
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstructL(aSection);
		}
	else
		{
		ret=CT_DataWsSpriteBase::DoCommandL(aCommand,aSection,aAsyncErrorIndex);
		}

	return ret;
	}

/**
 * Process new command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsPointerCursor::DoCmdNewL(const TDesC& aSection)
	{
	TPtrC	wsName;
	RWsSession* ws=NULL;
	
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws=static_cast<RWsSession*>(GetDataObjectL(wsName));
		}
		
	TInt err=KErrNone;
	
	if ( ws!=NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogNew2);
		TRAP( err, iWsPointerCursor = new (ELeave) RWsPointerCursor(*ws));
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogNew);
		TRAP( err, iWsPointerCursor = new (ELeave) RWsPointerCursor());
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	// No command return value and output parameter to display and check
	}


/**
 * Process Construct command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsPointerCursor::DoCmdConstructL(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogConstruct);
	TBool dataOk=ETrue;

	// Get test data for command input parameter(s)
	TUint aSpriteFlags=0;
	
	if(!CT_GraphicsUtil::ReadSpriteFlags(*this,aSection,KFldFlag,aSpriteFlags))
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFlag);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogConstruct);
		TInt returnCode=iWsPointerCursor->Construct(aSpriteFlags);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}
