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

#include "T_DataWsEvent.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdnew,							"new");
_LIT(KCmdDestructor,					"~");
_LIT(KCmdErrorMessage,					"ErrorMessage");
_LIT(KCmdEventData,						"EventData");
_LIT(KCmdHandle,						"Handle");
_LIT(KCmdInt,							"Int");
_LIT(KCmdKey,							"Key");
_LIT(KCmdModifiersChanged,				"ModifiersChanged");
_LIT(KCmdPointer,						"Pointer");
_LIT(KCmdSetHandle,						"SetHandle");
_LIT(KCmdSetTimeNow,					"SetTimeNow");
_LIT(KCmdSetType,						"SetType");
_LIT(KCmdTime,							"Time");
_LIT(KCmdType,							"Type");
_LIT(KCmdVisibilityChanged,				"VisibilityChanged");

///	Fields
_LIT(KFldExpected,						"expected");
_LIT(KFldType,							"type");
_LIT(KFldError,							"error");
_LIT(KFldErrorCategory,					"errorcategory");
_LIT(KFldExpectedCategory,				"expectedcategory");
_LIT(KFldExpectedError,					"expectederror");
_LIT(KFldExpected_Code,					"expected_code");
_LIT(KFldExpected_ChangedModifier,		"expected_changedmodifier");
_LIT(KFldExpected_Modifier,				"expected_modifier");
_LIT(KFldExpected_Repeats,				"expected_repeats");
_LIT(KFldExpected_ScanCode,				"expected_scancode");
_LIT(KFldExpected_Type,					"expected_type");
_LIT(KFldExpected_Pos,					"expected_pos");
_LIT(KFldHandle,						"handle");
_LIT(KFldNew_Value,						"new_value");
_LIT(KFldUseConst,						"useconst");

///	Logging
_LIT(KLogMissingParameter,				"Missing parameter %S");
_LIT(KLogMissingExpectedValue,			"Missing expected value '%S'");
_LIT(KLogNotExpectedValue,				"Not expected value");
_LIT(KLogNotExpectedValueInt,			"'%S' is not as expected=%d, actual=%d");
_LIT(KLogNotExpectedValueHex,			"'%S' is not as expected=0x%x, actual=0x%x");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsEvent* CT_DataWsEvent::NewL()
/**
 * Two phase constructor
 */
	{
	CT_DataWsEvent*    ret=new (ELeave) CT_DataWsEvent();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsEvent::CT_DataWsEvent()
/**
 * Protected constructor. First phase construction
 */
:	CDataWrapperBase()
,	iWsEvent(NULL)
	{
	}

void CT_DataWsEvent::ConstructL()
/**
 * Second phase construction
 */
	{
	}

CT_DataWsEvent::~CT_DataWsEvent()
/**
 * Public destructor
 */
	{
	DestroyData();
	}

TAny* CT_DataWsEvent::GetObject()
/**
 * Return a pointer to the object that the data wraps
 *
 * @return pointer to the object that the data wraps
 */
	{
	return iWsEvent;
	}

void CT_DataWsEvent::SetObjectL(TAny* aAny)
/**
 * Set the object that the data wraps
 *
 * @param    aObject object that the wrapper is testing
 *
 */
	{
	DestroyData();
	iWsEvent = static_cast<TWsEvent*> (aAny);
	}

void CT_DataWsEvent::DisownObjectL()
/**
 * The object will no longer be owned by this
 *
 * @leave	KErrNotSupported if the the function is not supported
 */
	{
	iWsEvent = NULL;
	}

void CT_DataWsEvent::DestroyData()
	{
	delete iWsEvent;
	iWsEvent=NULL;
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
TBool CT_DataWsEvent::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool    ret = ETrue;
	if ( aCommand==KCmdnew )
		{
		DoCmdnew();
		}
	else if ( aCommand==KCmdDestructor )
		{
		DestroyData();
		}
	else if ( aCommand==KCmdErrorMessage )
		{
		DoCmdErrorMessage(aSection);
		}
	else if ( aCommand==KCmdEventData )
		{
		DoCmdEventData(aSection);
		}
	else if ( aCommand==KCmdHandle )
		{
		DoCmdHandle(aSection);
		}
	else if ( aCommand==KCmdInt )
		{
		DoCmdInt(aSection);
		}
	else if ( aCommand==KCmdKey )
		{
		DoCmdKey(aSection);
		}
	else if ( aCommand==KCmdModifiersChanged )
		{
		DoCmdModifiersChanged(aSection);
		}
	else if ( aCommand==KCmdPointer )
		{
		DoCmdPointer(aSection);
		}
	else if ( aCommand==KCmdSetHandle )
		{
		DoCmdSetHandle(aSection);
		}
	else if ( aCommand==KCmdSetTimeNow )
		{
		DoCmdSetTimeNow();
		}
	else if ( aCommand==KCmdSetType )
		{
		DoCmdSetType(aSection);
		}
	else if ( aCommand==KCmdTime )
		{
		DoCmdTime();
		}
	else if ( aCommand==KCmdType )
		{
		DoCmdType(aSection);
		}
	else if ( aCommand==KCmdVisibilityChanged )
		{
		DoCmdVisibilityChanged(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CT_DataWsEvent::DoCmdnew()
	{
	INFO_PRINTF1(_L("new TWsEvent()"));
	DestroyData();
	TRAPD(err,	iWsEvent = new (ELeave) TWsEvent());
	if(err	!= KErrNone)
		{
		ERR_PRINTF2(_L("**** TWsEvent Constructor failed with error %d"), err);
		SetError(err);
		}
	}

void CT_DataWsEvent::DoCmdErrorMessage(const TDesC& aSection)
	{
	const	TWsEvent*	wsEvent=iWsEvent;
	TWsErrorMessage*	actual=wsEvent->ErrorMessage();
	INFO_PRINTF3(_L("ErrorMessage: Category %d. Error %d"), actual->iErrorCategory, actual->iError);

	TWsErrorMessage		expected;
	if ( CT_GraphicsUtil::ReadErrorCategory(*this, aSection, KFldExpectedCategory(), expected.iErrorCategory) )
		{
		if ( actual->iErrorCategory!=expected.iErrorCategory )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}

	TWsErrorMessage::TErrorCategory	expectedError;
	if ( CT_GraphicsUtil::ReadErrorCategory(*this, aSection, KFldExpectedError(), expectedError) )
		{
		if ( actual->iErrorCategory!=expectedError )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}

	CT_GraphicsUtil::ReadErrorCategory(*this, aSection, KFldErrorCategory(), actual->iErrorCategory);

	TInt	error;
	if ( GetIntFromConfig(aSection, KFldError(), error) )
		{
		actual->iError=(TUint)error;
		}
	}

void CT_DataWsEvent::DoCmdEventData(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TUint8*	actual=iWsEvent->EventData();
	INFO_PRINTF2(_L("EventData: FirstByte %d"), *actual);

	TInt	index=0;
	for ( index=0; index<TWsEvent::EWsEventDataSize; ++index )
		{
		TInt	expected;
		tempStore.Format(_L("%S%d"), &KFldExpected, index+1);
		if ( GetIntFromConfig(aSection, tempStore, expected) )
			{
			if ( *(actual+index)!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}

	for ( index=0; index<TWsEvent::EWsEventDataSize; ++index )
		{
		TInt	actualInt;
		tempStore.Format(_L("%S%d"), &KFldNew_Value, index+1);
		if ( GetIntFromConfig(aSection, tempStore, actualInt) )
			{
			*(actual+index)=(TUint8)actualInt;
			}
		}
	}

void CT_DataWsEvent::DoCmdSetHandle(const TDesC& aSection)
	{
	TInt	handle=0;
	if ( !GetIntFromConfig(aSection, KFldHandle(), handle) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldHandle());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("TWsEvent::SetHandle"));
		iWsEvent->SetHandle((TUint)handle);
		}
	}

void CT_DataWsEvent::DoCmdHandle(const TDesC& aSection)
	{
	TUint	actual=iWsEvent->Handle();
	INFO_PRINTF2(_L("Handle() actual=%d"), actual);
	SetHandle(actual);

	TInt	expected;
	if ( !GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		if ( expected!=(TInt)actual )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsEvent::DoCmdSetType(const TDesC& aSection)
	{
	TEventCode	eventCode;
	if ( !CT_GraphicsUtil::ReadEventCode(*this, aSection, KFldType(), eventCode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldType());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("SetType()"));
		iWsEvent->SetType(eventCode);
		}
	}

void CT_DataWsEvent::DoCmdType(const TDesC& aSection)
	{
	TInt	actual=iWsEvent->Type();
	INFO_PRINTF2(_L("Type() actual Type:%d"), actual);

	TEventCode	expected;
	if ( !CT_GraphicsUtil::ReadEventCode(*this, aSection, KFldExpected(), expected) )
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

void CT_DataWsEvent::DoCmdVisibilityChanged(const TDesC& aSection)
	{
	const TWsVisibilityChangedEvent*	actual=NULL;

	TBool	useConst=EFalse;
	GetBoolFromConfig(aSection, KFldUseConst(), useConst);
	if ( useConst )
		{
		INFO_PRINTF1(_L("const VisibilityChanged"));
		const	TWsEvent*	wsEvent=iWsEvent;
		actual=wsEvent->VisibilityChanged();
		}
	else
		{
		INFO_PRINTF1(_L("VisibilityChanged"));
		TWsVisibilityChangedEvent*	actualNonConst=iWsEvent->VisibilityChanged();
		actual=actualNonConst;
		}
	INFO_PRINTF2(_L("VisibilityChanged:%d"), actual->iFlags);

	TWsVisibilityChangedEvent	expected;
	if ( CT_GraphicsUtil::ReadWsVisibilityChangedEvent(*this, aSection, KFldExpected(), expected) )
		{
		if ( actual->iFlags!=expected.iFlags )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsEvent::DoCmdSetTimeNow()
	{
	INFO_PRINTF1(_L("SetTimeNow()"));
	iWsEvent->SetTimeNow();
	}

void CT_DataWsEvent::DoCmdTime()
	{
	INFO_PRINTF1(_L("Time()"));
	TTime	eventTime=iWsEvent->Time();

	TDateTime	dateTime = eventTime.DateTime();
	INFO_PRINTF4(_L("Time() returned %d/%d/%d"), dateTime.Year(), dateTime.Month(), dateTime.Day());
	INFO_PRINTF5(_L("Time() returned %d:%d:%d.%d"), dateTime.Hour(), dateTime.Minute(), dateTime.Second(), dateTime.MicroSecond());
	}

void CT_DataWsEvent::DoCmdInt(const TDesC& aSection)
	{
	TInt*	actual=iWsEvent->Int();
	INFO_PRINTF2(_L("Int:0x%x"), *actual);

	TInt	expected;
	if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		if ( *actual!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}

	GetIntFromConfig(aSection, KFldNew_Value(), *actual);
	}

void CT_DataWsEvent::DoCmdKey(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Key()"));
	TKeyEvent*	eventKey=iWsEvent->Key();

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

void CT_DataWsEvent::DoCmdModifiersChanged(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("ModifiersChanged()"));
	TModifiersChangedEvent*	event=iWsEvent->ModifiersChanged();
	INFO_PRINTF3(_L("TModifiersChangedEvent: iChangedModifiers=0x%x iModifiers=0x%x"), event->iChangedModifiers, event->iModifiers);

	TUint	changedModifiers;
	if ( CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldExpected_ChangedModifier, changedModifiers) )
		{
		if ( changedModifiers!=event->iChangedModifiers )
			{
			ERR_PRINTF4(KLogNotExpectedValueHex, &KFldExpected_Modifier, changedModifiers, event->iChangedModifiers);
			SetBlockResult(EFail);
			}
		}

	TUint	modifiers;
	if ( CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldExpected_Modifier, modifiers) )
		{
		if ( modifiers!=(event->iModifiers&event->iChangedModifiers) )
			{
			ERR_PRINTF4(KLogNotExpectedValueHex, &KFldExpected_Modifier, modifiers, event->iModifiers);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsEvent::DoCmdPointer(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Pointer()"));
	TPointerEvent*	actual=iWsEvent->Pointer();
	INFO_PRINTF2(_L("iType            =%d"), actual->iType);
	INFO_PRINTF2(_L("iModifiers       =0x%x"), actual->iModifiers);
	INFO_PRINTF3(_L("iPosition        =(%d,%d)"), actual->iPosition.iX, actual->iPosition.iY);
	INFO_PRINTF3(_L("iParentPosition  =(%d,%d)"), actual->iParentPosition.iX, actual->iParentPosition.iY);

	TPointerEvent::TType	expectedType;
	if ( CT_GraphicsUtil::ReadType(*this, aSection, KFldExpected_Type, expectedType) )
		{
		if ( actual->iType!=expectedType )
			{
			ERR_PRINTF4(KLogNotExpectedValueHex, &KFldExpected_Type, expectedType, actual->iType);
			SetBlockResult(EFail);
			}
		}

	TUint	modifiers;
	if ( CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldExpected_Modifier, modifiers) )
		{
		if ( modifiers!=(actual->iModifiers&actual->iType) )
			{
			ERR_PRINTF4(KLogNotExpectedValueHex, &KFldExpected_Modifier, modifiers, actual->iModifiers);
			SetBlockResult(EFail);
			}
		}

	TPoint	position;
	if ( GetPointFromConfig(aSection, KFldExpected_Pos, position) )
		{
		if ( position!=actual->iPosition )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}
