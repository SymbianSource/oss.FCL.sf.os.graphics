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

#include "DataWrapperActive.h"

/*@{*/
///	Constant Literals used.
_LIT(KCmdCancel,					"Cancel");
_LIT(KCmdiStatus,					"iStatus");

_LIT(KFldExpected,					"expected");
/*@}*/

CDataWrapperActive::CDataWrapperActive()
:	CDataWrapperBase()
/**
 * Protected constructor
 */
	{
	}

CDataWrapperActive::~CDataWrapperActive()
/**
 * Protected destructor
 */
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
TBool CDataWrapperActive::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool ret=ETrue;

	if ( aCommand==KCmdCancel )
		{
		DoCmdCancel();
		}
	else if ( aCommand==KCmdiStatus )
		{
		DoCmdiStatus(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CDataWrapperActive::DoCmdCancel()
	{
	INFO_PRINTF1(_L("++CActive::Cancel()"));
	GetActive()->Cancel();
	DecOutstanding();
	INFO_PRINTF1(_L("--CActive::Cancel()"));
	}

void CDataWrapperActive::DoCmdiStatus(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("++CActive::iStatus"));
	TRequestStatus	actual=GetActive()->iStatus;
	INFO_PRINTF2(_L("iStatus=%d"), actual.Int());

	TInt	expected;
	if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		if ( expected!=actual.Int() )
			{
			ERR_PRINTF1(_L("Expected value does not match actual"));
			SetBlockResult(EFail);
			}
		}
	INFO_PRINTF1(_L("--CActive::iStatus"));
	}
