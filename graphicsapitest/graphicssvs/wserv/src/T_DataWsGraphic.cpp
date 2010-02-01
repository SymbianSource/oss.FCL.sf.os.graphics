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

#include "T_DataWsGraphic.h"

/*@{*/
//Commands
_LIT(KCmdDestructor,				"~CWsGraphic");
_LIT(KCmdDestroy,					"Destroy");
_LIT(KCmdId,						"Id");
_LIT(KCmdIsActive,					"IsActive");
_LIT(KCmdShare,						"Share");
_LIT(KCmdShareGlobally,				"ShareGlobally");
_LIT(KCmdUnShare,					"UnShare");
_LIT(KCmdUnShareGlobally,			"UnShareGlobally");

///	Fields
_LIT(KFldClientId,					"clientid");
_LIT(KFldExpected,					"expected");
_LIT(KFldExpectNot,					"expectnot");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,			"Not expected value");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphic::CT_DataWsGraphic()
:	CDataWrapperBase()
	{
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
TBool CT_DataWsGraphic::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor )
		{
		DoCmdDestructorL();
		}
	else if ( aCommand==KCmdDestroy )
		{
		DoCmdDestroy();
		}
	else if ( aCommand==KCmdId )
		{
		DoCmdIdL(aSection);
		}
	else if ( aCommand==KCmdIsActive )
		{
		DoCmdIsActive(aSection);
		}
	else if ( aCommand==KCmdShare )
		{
		DoCmdShare(aSection);
		}
	else if ( aCommand==KCmdShareGlobally )
		{
		DoCmdShareGlobally();
		}
	else if ( aCommand==KCmdUnShare )
		{
		DoCmdUnShare(aSection);
		}
	else if ( aCommand==KCmdUnShareGlobally )
		{
		DoCmdUnShareGlobally();
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CT_DataWsGraphic::DoCmdDestructorL()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute ~CWsGraphic()"));
	CWsGraphic*	graphic=GetWsGraphic();
	CleanupStack::PushL(graphic);
	DisownObjectL();
	CleanupStack::PopAndDestroy(graphic);
	}

void CT_DataWsGraphic::DoCmdDestroy()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Destroy()"));
	GetWsGraphic()->Destroy();
	}

void CT_DataWsGraphic::DoCmdIdL(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Id()"));
	const TWsGraphicId&	actual=GetWsGraphic()->Id();
	SetId(actual);
	INFO_PRINTF3(_L("GetWsGraphic() Uid=0x%x Id=0x%x"), actual.Uid().iUid, actual.Id());

	// Diaplay command return value, check if it matches the expected value
	TWsGraphicId	expected(0);
	if ( GetWsGraphicIdFromConfigL(aSection, KFldExpectNot, expected) )
		{
		if ( actual.Id()==expected.Uid().iUid )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	else
		{
		if ( !GetWsGraphicIdFromConfigL(aSection, KFldExpected, expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpectNot);
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual.Uid()!=expected.Uid() )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataWsGraphic::DoCmdIsActive(const TDesC& aSection)
	{
	// Execute command and log parameters
	TBool	actual=GetWsGraphic()->IsActive();
	INFO_PRINTF2(_L("execute IsActive()=%d"), actual);

	// Diaplay command return value, check if it matches the expected value
	TBool	expected;
	if ( !GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphic::DoCmdShare(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	datId;

	if ( !GetIntFromConfig(aSection, KFldClientId, datId) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldClientId());
		SetBlockResult(EFail);
		}
	else
		{
		_LIT_SECURE_ID(datClientId, datId);

		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Share(TSecureId)"));
		TInt	err=Share(datClientId);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsGraphic::DoCmdShareGlobally()
	{
	INFO_PRINTF1(_L("execute ShareGlobally()"));
	TInt	err=ShareGlobally();
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsGraphic::DoCmdUnShare(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	datId;

	if ( !GetIntFromConfig(aSection, KFldClientId, datId) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldClientId());
		SetBlockResult(EFail);
		}
	else
		{
		_LIT_SECURE_ID(datClientId, datId);

		// Execute command and log parameters
		INFO_PRINTF1(_L("execute UnShare(TSecureId)"));
		TInt	err=UnShare(datClientId);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsGraphic::DoCmdUnShareGlobally()
	{
	INFO_PRINTF1(_L("execute UnShareGlobally()"));
	TInt	err=UnShareGlobally();
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}
