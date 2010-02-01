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

#include "T_DataWsGraphicMsgFixedBase.h"

/*@{*/
///	Commands
_LIT(KCmdPckg,					"Pckg");
_LIT(KCmdSize,					"Size");
_LIT(KCmdTypeId,				"TypeId");

// Fields
_LIT(KFldExpected,				"expected");

///	Logging
_LIT(KLogNotExpectedValue,		"Not expected value");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphicMsgFixedBase::CT_DataWsGraphicMsgFixedBase()
:	CDataWrapperBase()
	{
	}

CT_DataWsGraphicMsgFixedBase::~CT_DataWsGraphicMsgFixedBase()
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
TBool CT_DataWsGraphicMsgFixedBase::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret=ETrue;

	if ( aCommand==KCmdPckg )
		{
		DoCmdPckg();
		}
	else if ( aCommand==KCmdSize )
		{
		DoCmdSize(aSection);
		}
	else if ( aCommand==KCmdTypeId )
		{
		DoCmdTypeId(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CT_DataWsGraphicMsgFixedBase::DoCmdPckg()
	{
	INFO_PRINTF1(_L("execute Pckg()"));
	iPckg.Set(GetWsGraphicMsgFixedBase()->Pckg());
	}

void CT_DataWsGraphicMsgFixedBase::DoCmdSize(const TDesC& aSection)
	{
	TInt	size=GetWsGraphicMsgFixedBase()->Size();
	INFO_PRINTF2(_L("execute Size() = %d"), size);

	TInt	expected;
	if ( GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=size )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphicMsgFixedBase::DoCmdTypeId(const TDesC& aSection)
	{
	TUid	uid=GetWsGraphicMsgFixedBase()->TypeId();
	INFO_PRINTF2(_L("execute TypeId() = 0x%x"), uid.iUid);

	TInt	expected;
	if ( GetHexFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=uid.iUid )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}
