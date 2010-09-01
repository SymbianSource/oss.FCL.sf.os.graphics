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

#include "T_DataWsPriorityKeyEvent.h"
#include "T_GraphicsUtil.h"

/*@{*/
//Commands
_LIT(KCmdnew,						"new");
_LIT(KCmdDestructor,				"~");
_LIT(KCmdHandle,					"Handle");
_LIT(KCmdKey,						"Key");
_LIT(KCmdSetHandle,					"SetHandle");

// Fields
_LIT(KFldExpected,					"expected");
_LIT(KFldExpected_Code,				"expected_code");
_LIT(KFldExpected_Modifier,			"expected_modifier");
_LIT(KFldExpected_Repeats,			"expected_repeats");
_LIT(KFldExpected_ScanCode,			"expected_scancode");
_LIT(KFldHandle,					"handle");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogMissingExpectedValue,		"Missing expected value '%S'");
_LIT(KLogNotExpectedValueInt,		"'%S' is not as expected=%d, actual=%d");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsPriorityKeyEvent* CT_DataWsPriorityKeyEvent::NewL()
	{
	CT_DataWsPriorityKeyEvent*	ret=new (ELeave) CT_DataWsPriorityKeyEvent();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsPriorityKeyEvent::CT_DataWsPriorityKeyEvent()
:	CDataWrapperBase()
,	iWsPriorityKeyEvent(NULL)
	{
	}

void CT_DataWsPriorityKeyEvent::ConstructL()
	{
	}

CT_DataWsPriorityKeyEvent::~CT_DataWsPriorityKeyEvent()
	{
	DestroyData();
	}

void CT_DataWsPriorityKeyEvent::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsPriorityKeyEvent = static_cast<TWsPriorityKeyEvent*> (aAny);
	}

void CT_DataWsPriorityKeyEvent::DisownObjectL()
	{
	iWsPriorityKeyEvent = NULL;
	}

void CT_DataWsPriorityKeyEvent::DestroyData()
	{
	delete iWsPriorityKeyEvent;
	iWsPriorityKeyEvent=NULL;
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
TBool CT_DataWsPriorityKeyEvent::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
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
	else if ( aCommand==KCmdKey )
		{
		DoCmdKey(aSection);
		}
	else if ( aCommand==KCmdSetHandle )
		{
		DoCmdSetHandle(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}



void CT_DataWsPriorityKeyEvent::DoCmdnew()
	{
	DestroyData();
	INFO_PRINTF1(_L("execute new TWsPriorityKeyEvent()"));
	TRAPD(err, iWsPriorityKeyEvent = new (ELeave) TWsPriorityKeyEvent());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataWsPriorityKeyEvent::DoCmdSetHandle(const TDesC& aSection)
	{
	TInt	handle;

	if ( !GetIntFromConfig(aSection, KFldHandle(), handle) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldHandle);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetHandle(TUint)"));
		iWsPriorityKeyEvent->SetHandle((TUint)handle);
		}
	}


void CT_DataWsPriorityKeyEvent::DoCmdHandle(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute Handle()"));
	TUint	actual=iWsPriorityKeyEvent->Handle();
	SetHandle(actual);

	TInt	expected;
	if ( GetIntFromConfig(aSection, KFldExpected, expected) )
		{
		if ( actual!= (TUint)expected )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expected, actual);
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	}


void CT_DataWsPriorityKeyEvent::DoCmdKey(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute Key()"));
	TKeyEvent*	eventKey=iWsPriorityKeyEvent->Key();

	TKeyCode	keyCode;
	if ( CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldExpected_Code, keyCode) )
		{
		if ( (TUint)keyCode != eventKey->iCode )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected_Code, keyCode, eventKey->iCode);
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected_Code);
		SetBlockResult(EFail);
		}

	TStdScanCode	scanCode;
	if ( CT_GraphicsUtil::ReadStdScanCode(*this, aSection, KFldExpected_ScanCode, scanCode) )
		{
		if ( scanCode != eventKey->iScanCode )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected_ScanCode, scanCode, eventKey->iScanCode);
			SetBlockResult(EFail);
			}
		}

	TUint	modifiers;
	if ( CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldExpected_Modifier, modifiers) )
		{
		if ( modifiers != eventKey->iModifiers )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected_Modifier, modifiers, eventKey->iModifiers);
			SetBlockResult(EFail);
			}
		}

	TInt	repeats;
	if ( GetIntFromConfig(aSection, KFldExpected_Repeats, repeats) )
		{
		if ( repeats != eventKey->iRepeats )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected_Repeats, repeats, eventKey->iRepeats);
			SetBlockResult(EFail);
			}
		}
	}

