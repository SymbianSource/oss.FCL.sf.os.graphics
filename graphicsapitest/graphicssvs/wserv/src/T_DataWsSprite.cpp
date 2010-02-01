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
#include "T_DataWsSprite.h"

/*@{*/
//Commands
_LIT(KCmdNew,							    "new");
_LIT(KCmdConstruct,						    "Construct");
_LIT(KCmdSetPosition,						"SetPosition");

// Fields
_LIT(KFldWs,							"ws");
_LIT(KFldFlag,                          "flag");
_LIT(KFldPoint,							"pos");
_LIT(KFldWindow,                        "win");
///	Logging
_LIT(KLogError,								"Error=%d");
_LIT(KLogMissingParameter,					"Missing parameter '%S'");

_LIT(KLogNew,                               "execute new RWsSprite()");
_LIT(KLogNew2,                              "execute new RWsSprite(RWsSession)");
_LIT(KLogConstruct,                         "execute Construct(RWindowTreeNode, const TPoint, TInt)");
_LIT(KLogSetPosition,                       "execute SetPosition(TPoint)");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsSprite::CT_DataWsSprite()
:	CT_DataWsSpriteBase(),iWsSprite(NULL)
	{
	}

CT_DataWsSprite*	CT_DataWsSprite::NewL()
	{
	CT_DataWsSprite*    ret=new (ELeave) CT_DataWsSprite();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

void CT_DataWsSprite::ConstructL()
	{
	}

CT_DataWsSprite::~CT_DataWsSprite()
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
void CT_DataWsSprite::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsSprite = static_cast<RWsSprite*> (aAny);
	}

/**
 * Called by TEF framework to disown the object.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsSprite::DisownObjectL()
	{
	iWsSprite = NULL;
	}

/**
 * Destroy the object which has been constructed.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsSprite::DestroyData()
	{
	delete iWsSprite;
	iWsSprite = NULL;
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
TBool CT_DataWsSprite::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdNew )
		{
		DoCmdNewL(aSection);
		}
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstructL(aSection);
		}
	else if ( aCommand==KCmdSetPosition )
		{
		DoCmdSetPosition(aSection);
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
void CT_DataWsSprite::DoCmdNewL(const TDesC& aSection)
	{
	TPtrC	wsName;
	RWsSession* ws=NULL;
	
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}
		
	TInt err=KErrNone;
	
	if ( ws!=NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogNew2);
		TRAP( err, iWsSprite=new (ELeave) RWsSprite(*ws));
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogNew);
		TRAP( err, iWsSprite=new (ELeave) RWsSprite());
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
void CT_DataWsSprite::DoCmdConstructL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPtrC	winName;
	RWindow* win=NULL;
	
	if ( GetStringFromConfig(aSection, KFldWindow, winName) )
		{
		win=static_cast<RWindow*>(GetDataObjectL(winName));
		}
	
	if ( win==NULL )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldWindow);
		SetBlockResult(EFail);
		}

	TUint aSpriteFlags=0;
	
	if(!CT_GraphicsUtil::ReadSpriteFlags(*this,aSection,KFldFlag,aSpriteFlags))
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFlag);
		SetBlockResult(EFail);
		}

	TPoint	dataPoint;
	
	if ( !GetPointFromConfig(aSection, KFldPoint, dataPoint) )
		{
		dataPoint=TPoint(0,0);
		}
	
	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogConstruct);
		TInt returnCode=iWsSprite->Construct(*win, dataPoint,aSpriteFlags);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}
		}
	}

/**
 * Process SetPosition command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsSprite::DoCmdSetPosition(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	TPoint	dataPoint;
	
	if ( !GetPointFromConfig(aSection, KFldPoint, dataPoint) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint);
		SetBlockResult(EFail);
		}
	
	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogSetPosition);
		iWsSprite->SetPosition(dataPoint);
		// No command return value and output parameter to display and check
		}
	// No command return value and output parameter to display and check
	}
