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

#include "T_DataWsRedrawEvent.h"

/*@{*/
//Commands
_LIT(KCmdnew,						"new");
_LIT(KCmdDestructor,				"~");
_LIT(KCmdHandle,					"Handle");
_LIT(KCmdRect,						"Rect");

///	Fields
_LIT(KFldExpected,					"expected");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingExpectedValue,		"Missing expected value '%S'");
_LIT(KLogNotAsExpectedValue,		"'%S' is not as expected value");
_LIT(KLogNotExpectedValueInt,		"'%S' is not as expected=%d, actual=%d");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsRedrawEvent* CT_DataWsRedrawEvent::NewL()
	{
	CT_DataWsRedrawEvent*    ret=new (ELeave) CT_DataWsRedrawEvent();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsRedrawEvent::CT_DataWsRedrawEvent()
:	CDataWrapperBase()
,	iWsRedrawEvent(NULL)
	{
	}

void CT_DataWsRedrawEvent::ConstructL()
	{
	}

CT_DataWsRedrawEvent::~CT_DataWsRedrawEvent()
	{
	DestroyData();
	}

void CT_DataWsRedrawEvent::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsRedrawEvent = static_cast<TWsRedrawEvent*> (aAny);
	}

void CT_DataWsRedrawEvent::DisownObjectL()
	{
	iWsRedrawEvent = NULL;
	}

void CT_DataWsRedrawEvent::DestroyData()
	{
	delete iWsRedrawEvent;
	iWsRedrawEvent=NULL;
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
TBool CT_DataWsRedrawEvent::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret = ETrue;
	if ( aCommand==KCmdnew )
		{
		DoCmdnew();
		}
	else if ( aCommand==KCmdDestructor )
		{
		DestroyData();
		}
	else if ( aCommand==KCmdHandle )
		{
		DoCmdHandle(aSection);
		}
	else if ( aCommand==KCmdRect )
		{
		DoCmdRect(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}


void CT_DataWsRedrawEvent::DoCmdnew()
	{
	DestroyData();
	INFO_PRINTF1(_L("execute new TWsRedrawEvent()"));
	TRAPD(err, iWsRedrawEvent = new (ELeave) TWsRedrawEvent());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataWsRedrawEvent::DoCmdHandle(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute Handle()"));
	TUint	actual=iWsRedrawEvent->Handle();
	SetHandle(actual);

	TInt	expected;
	if ( !GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=(TUint)expected )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expected, actual);
			SetBlockResult(EFail);
			}
		}
	}


void CT_DataWsRedrawEvent::DoCmdRect(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute Rect()"));
	TRect	actual=iWsRedrawEvent->Rect();
	LogRect(_L("Rect()"), actual);

	TRect	expected;
	if ( !GetRectFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF2(KLogNotAsExpectedValue, &KFldExpected);
			SetBlockResult(EFail);
			}
		}
	}

