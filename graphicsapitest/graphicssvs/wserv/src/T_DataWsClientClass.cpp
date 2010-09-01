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

#include "T_DataWsClientClass.h"

/*@{*/
//Commands
_LIT(KCmdWsHandle,					"WsHandle");

// Fields
_LIT(KFldExpected,					"expected");
_LIT(KFldExpectNot,					"expectnot");

///	Logging
_LIT(KLogNotExpectedValueInt,		"'%S' is not as expected=%d, actual=%d");
_LIT(KLogAsExpectedValueInt,		"actual value as '%S'=%d");
/*@}*/


/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @param aAsyncErrorIndex	index of command. used for async calls
*
* @return ETrue if the command is processed
*/
TBool CT_DataWsClientClass::DoCommandL(CDataWrapperBase& aDataWrapper, const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdWsHandle )
		{
		DoCmdWsHandle(aDataWrapper, aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}


void CT_DataWsClientClass::DoCmdWsHandle(CDataWrapperBase& aDataWrapper, const TDesC& aSection)
	{
	// Execute command and log parameters
	aDataWrapper.INFO_PRINTF1(_L("execute WsHandle()"));
	TInt	actualResult = GetClientClass()->WsHandle();

	// Diaplay command return value, check if it matches the expected value
	TInt	expnotResult;
	if ( aDataWrapper.GetIntFromConfig(aSection, KFldExpectNot, expnotResult) )
		{
		if ( actualResult==expnotResult )
			{
			aDataWrapper.ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpectNot, expnotResult, actualResult);
			aDataWrapper.SetBlockResult(EFail);
			}
		else
			{
			aDataWrapper.INFO_PRINTF3(KLogAsExpectedValueInt, &KFldExpectNot, actualResult);
			}
		}

	TInt	expectResult;
	if ( aDataWrapper.GetIntFromConfig(aSection, KFldExpected, expectResult) )
		{
		if ( actualResult!=expectResult )
			{
			aDataWrapper.ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expectResult, actualResult);
			aDataWrapper.SetBlockResult(EFail);
			}
		else
			{
			aDataWrapper.INFO_PRINTF3(KLogAsExpectedValueInt, &KFldExpected, actualResult);
			}
		}
	}
