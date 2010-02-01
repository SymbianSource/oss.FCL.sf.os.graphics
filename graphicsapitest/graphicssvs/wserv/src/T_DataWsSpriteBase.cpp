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
#include "T_DataWsSpriteBase.h"

/*@{*/
//Commands
_LIT(KCmdActivate,							"Activate");
_LIT(KCmdAppendMember,						"AppendMember");
_LIT(KCmdUpdateMember,						"UpdateMember");
_LIT(KCmdClose,  					        "Close");

// Fields
_LIT(KFldIndex, 			 				"index");
_LIT(KFldOneParameter,			 			"oneparameter");
_LIT(KFldSpriteMember,						"spritemember");
///	Logging
_LIT(KLogError,								"Error=%d");
_LIT(KLogMissingParameter,					"Missing parameter '%S'");


_LIT(KLogActivate,                          "execute Activate()");
_LIT(KLogAppendMember,                      "execute AppendMember(const TSpriteMember &aMemberData)");
_LIT(KLogUpdateMember,                      "execute UpdateMember(TInt aIndex)");
_LIT(KLogUpdateMember2,                     "execute UpdateMember(TInt aIndex, const TSpriteMember &aMemberData)");
_LIT(KLogClose,                             "execute Close()");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsSpriteBase::CT_DataWsSpriteBase():CDataWrapperBase()
	{
	}

MWsClientClass* CT_DataWsSpriteBase::GetClientClass() const
	{
	return GetWsSpriteBase();
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
TBool CT_DataWsSpriteBase::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdActivate )
		{
		DoCmdActivate();
		}
	else if ( aCommand==KCmdAppendMember )
		{
		DoCmdAppendMemberL(aSection);
		}
	else if ( aCommand==KCmdUpdateMember )
		{
		DoCmdUpdateMemberL(aSection);
		}
	else if ( aCommand==KCmdClose )
		{
		DoCmdClose();
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

/**
 * Process Activate command
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsSpriteBase::DoCmdActivate()
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogActivate);
	TInt nErr=GetWsSpriteBase()->Activate();

	if(nErr!=KErrNone)
	{
	ERR_PRINTF2(KLogError, nErr);
	SetError(nErr);	
	}

	// No command return value and output parameter to display and check
	}

/**
 * Process AppendMember command
 *
 * @param: aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsSpriteBase::DoCmdAppendMemberL(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogAppendMember);
	
	//Init TSpriteMember 
	TSpriteMember spriteMember;	
	
	if ( !CT_GraphicsUtil::GetSpriteMemberL(*this, aSection, KFldSpriteMember, spriteMember) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSpriteMember);
		SetBlockResult(EFail);
		}
	
	TInt returnCode=GetWsSpriteBase()->AppendMember(spriteMember);
	
	if(KErrNone!=returnCode)
		{
		ERR_PRINTF2(KLogError, returnCode);
		SetError(returnCode);
		}

	// No command return value and output parameter to display and check
	}

/**
 * Process UpdateMember command
 *
 * @param: aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsSpriteBase::DoCmdUpdateMemberL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	//get update index
	TInt datIndex;
	
	if ( !GetIntFromConfig(aSection, KFldIndex, datIndex) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldIndex);
		}
	//get if one parameter
	TBool bOneParameter=TRUE;
	
	if ( !GetBoolFromConfig(aSection, KFldOneParameter, bOneParameter) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldOneParameter);
		}
		
	//Init  new TSpriteMember 
	TSpriteMember spriteMember;	
	
	if ( !CT_GraphicsUtil::GetSpriteMemberL(*this, aSection, KFldSpriteMember, spriteMember) )
		{
		if(!bOneParameter)
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldSpriteMember);
			SetBlockResult(EFail);
			}
		}
	
	if(dataOk)
		{
		TInt returnCode=KErrNone;
		
		if(bOneParameter)
			{
			INFO_PRINTF1(KLogUpdateMember);
			GetWsSpriteBase()->UpdateMember(datIndex);
			}
		else
		    {
			INFO_PRINTF1(KLogUpdateMember2);
			returnCode=GetWsSpriteBase()->UpdateMember(datIndex,spriteMember);
			}
		
		if(returnCode!=KErrNone)
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}
		}
	// No command return value and output parameter to display and check
	}

/**
 * Process Close command
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsSpriteBase::DoCmdClose()
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogClose);
	GetWsSpriteBase()->Close();
	}
