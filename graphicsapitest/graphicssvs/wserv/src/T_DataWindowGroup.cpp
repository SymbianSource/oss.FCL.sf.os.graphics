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

#include "T_DataWindowGroup.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname,	"RWindowGroup");

// Fields
_LIT(KFldAdjust					,"adjust");
_LIT(KFldCaptureKey				,"capturekey");
_LIT(KFldClientHandle			,"clienthandle");
_LIT(KFldClipRect				,"cliprect");
_LIT(KFldCursor					,"cursor");
_LIT(KFldDeviceNumber			,"devicenumber");
_LIT(KFldFlags					,"flags");
_LIT(KFldIdOfParentWindowGroup	,"idofparentwindowgroup");
_LIT(KFldInputKeyCode			,"inputkeycode");
_LIT(KFldIsFocusable			,"isfocusable");
_LIT(KFldKeycode				,"keycode");
_LIT(KFldModifier				,"modifier");
_LIT(KFldModifierMask			,"modifiermask");
_LIT(KFldOrdinalPriority		,"ordinalpriority");
_LIT(KFldOutputKeyCode			,"outputkeycode");
_LIT(KFldPos					,"pos");
_LIT(KFldProcessSID				,"processsid");
_LIT(KFldPriority				,"priority");
_LIT(KFldRepeatTime				,"repeattime");
_LIT(KFldScanCode				,"scancode");
_LIT(KFldScreenDevice			,"screendevice");
_LIT(KFldState					,"state");
_LIT(KFldStdScanCode			,"stdscancode");
_LIT(KFldTip					,"tip");
_LIT(KFldType					,"type");
_LIT(KFldWin					,"win");
_LIT(KFldWs						,"ws");
_LIT(KFldID						,"id");
_LIT(KFldIDName					,"id_name");

//Parameters
_LIT(KinputWGN,					"inputWGN");
_LIT(KexpectedReturnWGN,		"expectRtnWGN");
_LIT(KOwningWGIdentifier,		"OwningWGId");
_LIT(KOwningWGIdName,			"OwningWGId_name");

//Commands
_LIT(KCmdnew,									"new");
_LIT(KCmdDestructor,							"~");
_LIT(KCmdConstruct,								"Construct");
_LIT(KCmdConstructChildApp,						"ConstructChildApp");
_LIT(KCmdAllowProcessToCreateChildWindowGroups,	"AllowProcessToCreateChildWindowGroups");
_LIT(KCmdEnableReceiptOfFocus,					"EnableReceiptOfFocus");
_LIT(KCmdAutoForeground,						"AutoForeground");
_LIT(KCmdSetOrdinalPriorityAdjust,				"SetOrdinalPriorityAdjust");
_LIT(KCmdSetOrdinalPositionErr,					"SetOrdinalPositionErr");
_LIT(KCmdCaptureKey,							"CaptureKey");
_LIT(KCmdCancelCaptureKey,						"CancelCaptureKey");
_LIT(KCmdCaptureKeyUpAndDowns,					"CaptureKeyUpAndDowns");
_LIT(KCmdCancelCaptureKeyUpAndDowns,			"CancelCaptureKeyUpAndDowns");
_LIT(KCmdCaptureLongKey,						"CaptureLongKey");
_LIT(KCmdCancelCaptureLongKey,					"CancelCaptureLongKey");
_LIT(KCmdAddPriorityKey,						"AddPriorityKey");
_LIT(KCmdRemovePriorityKey,						"RemovePriorityKey");
_LIT(KCmdSetTextCursor,							"SetTextCursor");
_LIT(KCmdCancelTextCursor,						"CancelTextCursor");
_LIT(KCmdSetOwningWindowGroup,					"SetOwningWindowGroup");
_LIT(KCmdDefaultOwningWindow,					"DefaultOwningWindow");
_LIT(KCmdSetName,								"SetName");
_LIT(KCmdName,									"Name");
_LIT(KCmdIdentifier,							"Identifier");
_LIT(KCmdDisableKeyClick,						"DisableKeyClick");
_LIT(KCmdEnableScreenChangeEvents,				"EnableScreenChangeEvents");
_LIT(KCmdDisableScreenChangeEvents,				"DisableScreenChangeEvents");
_LIT(KCmdSimulatePointerEvent,					"SimulatePointerEvent");
_LIT(KCmdSetChildGroup,							"SetChildGroup");
_LIT(KCmdClearChildGroup,						"ClearChildGroup");

///	Logging
_LIT(KLogError,									"Error=%d");
_LIT(KLogMissingParameter,						"Missing parameter '%S'");
_LIT(KLogMissingExpectedValue,					"Missing expected value '%S'");
_LIT(KLogNotExpectedValueString,				"'%S' is not as expected='%S', actual='%S'");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWindowGroup* CT_DataWindowGroup::NewL()
	{
	CT_DataWindowGroup*	ret=new (ELeave) CT_DataWindowGroup();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWindowGroup::CT_DataWindowGroup()
:	CT_DataWindowTreeNode()
,	iWindowGroup(NULL)
,	iCaptureKeyHandle(0)
	{
	}

void CT_DataWindowGroup::ConstructL()
	{
	}

CT_DataWindowGroup::~CT_DataWindowGroup()
	{
	DestroyData();
	}

void CT_DataWindowGroup::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWindowGroup = static_cast<RWindowGroup*> (aAny);
	}

void CT_DataWindowGroup::DisownObjectL()
	{
	iWindowGroup = NULL;
	}

void CT_DataWindowGroup::DestroyData()
	{
	delete iWindowGroup;
	iWindowGroup=NULL;
	}

RWindowTreeNode* CT_DataWindowGroup::GetWindowTreeNode() const
	{
	return iWindowGroup;
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
TBool CT_DataWindowGroup::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnewL(aSection);
		}
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstructL(aSection);
		}
	else if ( aCommand==KCmdConstructChildApp )
		{
		DoCmdConstructChildAppL(aSection);
		}
	else if ( aCommand==KCmdAllowProcessToCreateChildWindowGroups )
		{
		DoCmdAllowProcessToCreateChildWindowGroups(aSection);
		}
	else if ( aCommand==KCmdEnableReceiptOfFocus )
		{
		DoCmdEnableReceiptOfFocus(aSection);
		}
	else if ( aCommand==KCmdAutoForeground )
		{
		DoCmdAutoForeground(aSection);
		}
	else if ( aCommand==KCmdSetOrdinalPriorityAdjust )
		{
		DoCmdSetOrdinalPriorityAdjust(aSection);
		}
	else if ( aCommand==KCmdSetOrdinalPositionErr )
		{
		DoCmdSetOrdinalPositionErr(aSection);
		}
	else if ( aCommand==KCmdCaptureKey )
		{
		DoCmdCaptureKey(aSection);
		}
	else if ( aCommand==KCmdCancelCaptureKey )
		{
		DoCmdCancelCaptureKey(aSection);
		}
	else if ( aCommand==KCmdCaptureKeyUpAndDowns )
		{
		DoCmdCaptureKeyUpAndDowns(aSection);
		}
	else if ( aCommand==KCmdCancelCaptureKeyUpAndDowns )
		{
		DoCmdCancelCaptureKeyUpAndDowns(aSection);
		}
	else if ( aCommand==KCmdCaptureLongKey )
		{
		DoCmdCaptureLongKey(aSection);
		}
	else if ( aCommand==KCmdCancelCaptureLongKey )
		{
		DoCmdCancelCaptureLongKey(aSection);
		}
	else if ( aCommand==KCmdAddPriorityKey )
		{
		DoCmdAddPriorityKey(aSection);
		}
	else if ( aCommand==KCmdRemovePriorityKey )
		{
		DoCmdRemovePriorityKey(aSection);
		}
	else if ( aCommand==KCmdSetTextCursor )
		{
		DoCmdSetTextCursorL(aSection);
		}
	else if ( aCommand==KCmdCancelTextCursor )
		{
		DoCmdCancelTextCursor();
		}
	else if ( aCommand==KCmdSetOwningWindowGroup )
		{
		DoCmdSetOwningWindowGroupL(aSection);
		}
	else if ( aCommand==KCmdDefaultOwningWindow )
		{
		DoCmdDefaultOwningWindow();
		}
	else if ( aCommand==KCmdSetName )
		{
		DoCmdSetName(aSection);
		}
	else if ( aCommand==KCmdName )
		{
		DoCmdName(aSection);
		}
	else if ( aCommand==KCmdIdentifier )
		{
		DoCmdIdentifier();
		}
	else if ( aCommand==KCmdDisableKeyClick )
		{
		DoCmdDisableKeyClick(aSection);
		}
	else if ( aCommand==KCmdEnableScreenChangeEvents )
		{
		DoCmdEnableScreenChangeEvents();
		}
	else if ( aCommand==KCmdDisableScreenChangeEvents )
		{
		DoCmdDisableScreenChangeEvents();
		}
	else if ( aCommand==KCmdSimulatePointerEvent )
		{
		DoCmdSimulatePointerEvent(aSection);
		}
	else if ( aCommand==KCmdSetChildGroup )
		{
		DoCmdSetChildGroupL(aSection);
		}
	else if ( aCommand==KCmdClearChildGroup )
		{
		DoCmdClearChildGroup();
		}
	else
		{
		ret=CT_DataWindowTreeNode::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataWindowGroup::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	TPtrC	wsName;
	RWsSession* ws=NULL;
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}

	TInt	err=KErrNone;
	if ( ws!=NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RWindowGroup(RWsSession)"));
		TRAP( err, iWindowGroup = new (ELeave) RWindowGroup(*ws));
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RWindowGroup()"));
		TRAP( err, iWindowGroup = new (ELeave) RWindowGroup());
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataWindowGroup::DoCmdDestructor()
	{
	DestroyData();
	}


void CT_DataWindowGroup::DoCmdConstructL(const TDesC& aSection)
	{
	TInt	returnCode=KErrNone;

	// Get test data for command input parameter(s)
	TInt	datClientHandle;
	TBool	hasClientHandle = GetIntFromConfig(aSection, KFldClientHandle, datClientHandle);
	if ( !hasClientHandle )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldClientHandle);
		SetBlockResult(EFail);
		}
	else
		{
		TBool	datIsFocusable;
		TBool	hasIsFocusable = GetBoolFromConfig(aSection, KFldIsFocusable, datIsFocusable);

#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
		CWsScreenDevice*	screenDevice=NULL;
		// Execute command and log parameters
		if ( CT_GraphicsUtil::GetWsScreenDeviceL(*this, aSection, KFldScreenDevice, screenDevice) )
			{
			if ( hasIsFocusable )
				{
				INFO_PRINTF1(_L("execute Construct(TUint32, TBool, CWsScreenDevice)"));
				returnCode = iWindowGroup->Construct(datClientHandle, datIsFocusable, screenDevice);
				}
			else
				{
				INFO_PRINTF1(_L("execute Construct(TUint32, CWsScreenDevice)"));
				returnCode = iWindowGroup->Construct(datClientHandle, screenDevice);
				}
			}
		else
#endif
			{
			if ( hasIsFocusable )
				{
				INFO_PRINTF1(_L("execute Construct(TUint32, TBool)"));
				returnCode = iWindowGroup->Construct(datClientHandle, datIsFocusable);
				}
			else
				{
				INFO_PRINTF1(_L("execute Construct(TUint32)"));
				returnCode = iWindowGroup->Construct(datClientHandle);
				}
			}
		}

	// Check the command return code, if !=KErrNone then stop this command
	if ( returnCode!=KErrNone )
		{
		ERR_PRINTF2(KLogError, returnCode);
		SetError(returnCode);
		}

	// No command output parameter to display and check
	}


void CT_DataWindowGroup::DoCmdConstructChildAppL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	TInt	returnCode=KErrNone;

	// Get test data for command input parameter(s)
	TInt	datIdOfParentWindowGroup;
	if ( !GetIntFromConfig(aSection, KFldIdOfParentWindowGroup, datIdOfParentWindowGroup) )
		{
		CDataWrapperBase*	wrapper=NULL;
		TPtrC	objectName;
		if ( GetStringFromConfig(aSection, KFldIdOfParentWindowGroup(), objectName) )
			{
			wrapper=static_cast<CDataWrapperBase*>(GetDataWrapperL(objectName));

			}
		
		if ( wrapper==NULL )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldIdOfParentWindowGroup);
			SetBlockResult(EFail);
			}
		else
			{
			datIdOfParentWindowGroup = wrapper->GetIdentifier();
			}
		}

	TInt	datClientHandle;
	if ( !GetIntFromConfig(aSection, KFldClientHandle, datClientHandle) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldClientHandle);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TBool	datIsFocusable;
		if ( GetBoolFromConfig(aSection, KFldIsFocusable, datIsFocusable) )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute ConstructChildApp(TInt, TUint32, TBool)"));
			returnCode = iWindowGroup->ConstructChildApp(datIdOfParentWindowGroup, datClientHandle, datIsFocusable);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute ConstructChildApp(TInt, TUint32)"));
			returnCode = iWindowGroup->ConstructChildApp(datIdOfParentWindowGroup, datClientHandle);
			}

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindowGroup::DoCmdAllowProcessToCreateChildWindowGroups(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	datProcess;
	if ( !GetIntFromConfig(aSection, KFldProcessSID, datProcess) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldProcessSID);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute AllowProcessToCreateChildWindowGroups(TUid)"));
		iWindowGroup->AllowProcessToCreateChildWindowGroups(TUid::Uid(datProcess));
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindowGroup::DoCmdEnableReceiptOfFocus(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datState;
	if ( !GetBoolFromConfig(aSection, KFldState, datState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldState);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute EnableReceiptOfFocus(TBool)"));
		iWindowGroup->EnableReceiptOfFocus(datState);
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindowGroup::DoCmdAutoForeground(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datState;
	if ( !GetBoolFromConfig(aSection, KFldState, datState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldState);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute AutoForeground(TBool)"));
		iWindowGroup->AutoForeground(datState);
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindowGroup::DoCmdSetOrdinalPriorityAdjust(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	datAdjust;
	if ( !GetIntFromConfig(aSection, KFldAdjust, datAdjust) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldAdjust);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetOrdinalPriorityAdjust(TInt)"));
		iWindowGroup->SetOrdinalPriorityAdjust(datAdjust);
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindowGroup::DoCmdSetOrdinalPositionErr(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TInt	datPos;
	if ( !GetIntFromConfig(aSection, KFldPos, datPos) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPos);
		SetBlockResult(EFail);
		}

	TInt	datOrdinalPriority;
	if ( !GetIntFromConfig(aSection, KFldOrdinalPriority, datOrdinalPriority) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldOrdinalPriority);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetOrdinalPositionErr(TInt, TInt)"));
		TInt	returnCode = iWindowGroup->SetOrdinalPositionErr(datPos, datOrdinalPriority);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}

void CT_DataWindowGroup::DoCmdCaptureKey(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TKeyCode	keyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldKeycode, keyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldKeycode);
		SetBlockResult(EFail);
		}

	TUint	modifierMask;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifierMask, modifierMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifierMask);
		SetBlockResult(EFail);
		}

	TUint	modifier;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifier, modifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifier);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TInt	ret=KErrNone;
		TInt	priority;
		if ( GetIntFromConfig(aSection, KFldPriority, priority) )
			{
			INFO_PRINTF1(_L("execute CaptureKey(keyCode, modifierMask, modifier, priority)"));
			ret=iWindowGroup->CaptureKey(keyCode, modifierMask, modifier, priority);
			}
		else
			{
			INFO_PRINTF1(_L("execute CaptureKey(keyCode, modifierMask, modifier)"));
			ret=iWindowGroup->CaptureKey(keyCode, modifierMask, modifier);
			}
		if ( ret<KErrNone )
			{
			ERR_PRINTF2(KLogError, ret);
			SetError(ret);
			}
		iCaptureKeyHandle=ret;
		}
	}


void CT_DataWindowGroup::DoCmdCancelCaptureKey(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	captureKey;
	if ( !GetIntFromConfig(aSection, KFldCaptureKey, captureKey) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCaptureKey);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute CancelCaptureKey(captureKey)"));
		if ( captureKey == -1 )
			{
			iWindowGroup->CancelCaptureKey(iCaptureKeyHandle);
			}
		else
			{
			iWindowGroup->CancelCaptureKey(captureKey);
			}
		}
	}


void CT_DataWindowGroup::DoCmdCaptureKeyUpAndDowns(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TUint	scanCode;
	if ( !CT_GraphicsUtil::ReadStdScanCode(*this, aSection, KFldScanCode, (TStdScanCode&)scanCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldScanCode);
		SetBlockResult(EFail);
		}

	TUint	modifierMask;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifierMask, modifierMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifierMask);
		SetBlockResult(EFail);
		}

	TUint	modifier;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifier, modifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifier);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TInt	ret=KErrNone;
		TInt	priority;
		if ( !GetIntFromConfig(aSection, KFldPriority, priority) )
			{
			INFO_PRINTF1(_L("execute CaptureKeyUpAndDowns(TUint, TUint, TUint)"));
			ret=iWindowGroup->CaptureKeyUpAndDowns(scanCode, modifierMask, modifier);
			}
		else
			{
			INFO_PRINTF1(_L("execute CaptureKeyUpAndDowns(TUint, TUint, TUint, TInt)"));
			ret=iWindowGroup->CaptureKeyUpAndDowns(scanCode, modifierMask, modifier, priority);
			}

		if ( ret < KErrNone )
			{
			ERR_PRINTF2(KLogError, ret);
			SetError(ret);
			} 
		else
			{
			iCaptureKeyHandle=ret;
			}
		}
	}


void CT_DataWindowGroup::DoCmdCancelCaptureKeyUpAndDowns(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	captureKey;
	if ( !GetIntFromConfig(aSection, KFldCaptureKey, captureKey) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCaptureKey);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute CancelCaptureKeyUpAndDowns(captureKey)"));
		if ( captureKey == -1 )
			{
			iWindowGroup->CancelCaptureKeyUpAndDowns(iCaptureKeyHandle);
			}
		else
			{
			iWindowGroup->CancelCaptureKeyUpAndDowns(captureKey);
			}
		}
	}


void CT_DataWindowGroup::DoCmdCaptureLongKey(const TDesC& aSection)
	{
	TBool			dataOk=ETrue;

	TKeyCode	inputKeyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldInputKeyCode, inputKeyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldInputKeyCode);
		SetBlockResult(EFail);
		}

	TKeyCode	outputKeyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldOutputKeyCode, outputKeyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldOutputKeyCode);
		SetBlockResult(EFail);
		}

	TUint	modifierMask;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifierMask, modifierMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifierMask);
		SetBlockResult(EFail);
		}

	TUint	modifier;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifier, modifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifier);
		SetBlockResult(EFail);
		}

	TInt	priority;
	if ( !GetIntFromConfig(aSection, KFldPriority, priority) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPriority);
		SetBlockResult(EFail);
		}

	TLongCaptureFlags	flags;
	if ( !CT_GraphicsUtil::ReadLongCaptureFlags(*this, aSection, KFldFlags, flags) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFlags);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TInt	ret=KErrNone;
		TInt	repeatTime;
		if ( GetIntFromConfig(aSection, KFldRepeatTime, repeatTime) )
			{
			TTimeIntervalMicroSeconds32	theRepeatTime(repeatTime);

			// Execute command and log parameters
			INFO_PRINTF1(_L("execute CaptureLongKey(repeatTime, inputKeyCode, iutputKeyCode, modifierMask, modifier, priority, flags)"));
			ret=iWindowGroup->CaptureLongKey(theRepeatTime, inputKeyCode, outputKeyCode, modifierMask, modifier, priority, flags);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute CaptureLongKey(inputKeyCode, outputKeyCode, modifierMask, modifier, priority, flags)"));
			ret=iWindowGroup->CaptureLongKey(inputKeyCode, outputKeyCode, modifierMask, modifier, priority, flags);
			}

		// Check the command return code, if !=KErrNone then stop this command
		if ( ret<KErrNone )
			{
			ERR_PRINTF2(KLogError, ret);
			SetError(ret);
			}
		iCaptureKeyHandle=ret;
		}
	}


void CT_DataWindowGroup::DoCmdCancelCaptureLongKey(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	captureKey;
	if ( !GetIntFromConfig(aSection, KFldCaptureKey, captureKey) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCaptureKey);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute CancelCaptureLongKey(captureKey)"));
		if ( captureKey == -1 )
			{
			iWindowGroup->CancelCaptureLongKey(iCaptureKeyHandle);
			}
		else
			{
			iWindowGroup->CancelCaptureLongKey(captureKey);
			}
		}
	}


void CT_DataWindowGroup::DoCmdAddPriorityKey(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TKeyCode	keyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldKeycode, keyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldKeycode);
		SetBlockResult(EFail);
		}

	TUint	modifierMask;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifierMask, modifierMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifierMask);
		SetBlockResult(EFail);
		}

	TUint	modifier;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifier, modifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifier);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute AddPriorityKey(TUint, TUint, TUint)"));
		TInt	ret=iWindowGroup->AddPriorityKey(keyCode, modifierMask, modifier);

		// Check the command return code, if <KErrNone then stop this command
		if ( ret!=KErrNone )
			{
			ERR_PRINTF2(KLogError, ret);
			SetError(ret);
			}
		}
	}


void CT_DataWindowGroup::DoCmdRemovePriorityKey(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TKeyCode	keyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldKeycode, keyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldKeycode);
		SetBlockResult(EFail);
		}

	TUint	modifierMask;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifierMask, modifierMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifierMask);
		SetBlockResult(EFail);
		}

	TUint	modifier;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifier, modifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldModifier);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute RemovePriorityKey(TUint, TUint, TUint)"));
		iWindowGroup->RemovePriorityKey(keyCode, modifierMask, modifier);
		}
	}


void CT_DataWindowGroup::DoCmdSetTextCursorL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TPtrC	datWinName;
	RWindowBase*	win = NULL;
	if ( !GetStringFromConfig(aSection, KFldWin, datWinName) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldWin);
		SetBlockResult(EFail);
		dataOk=EFalse;
		}
	else
		{
		win = static_cast<RWindowBase*>(GetDataObjectL(datWinName));
		}

	TPoint	datPos;
	if ( !GetPointFromConfig(aSection, KFldPos, datPos) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPos);
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	TTextCursor	datCursor;
	if ( !CT_GraphicsUtil::GetTextCursor(*this, aSection, KFldCursor, datCursor) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldCursor);
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		// get rectangle top left x-coordinate from parameters
		TRect	datClipRect;
		if ( GetRectFromConfig(aSection, KFldClipRect, datClipRect) )
			{
			INFO_PRINTF1(_L("execute SetTextCursor(RWindowBase, const TPoint, const TTextCursor, const TRect)"));
			iWindowGroup->SetTextCursor(*win, datPos, datCursor, datClipRect);
			}
		else
			{
			INFO_PRINTF1(_L("execute SetTextCursor(RWindowBase, const TPoint, const TTextCursor)"));
			iWindowGroup->SetTextCursor(*win, datPos, datCursor);
			}
		}
	}


void CT_DataWindowGroup::DoCmdCancelTextCursor()
	{
	INFO_PRINTF1(_L("RWindowGroup::CancelTextCursor() call"));
	iWindowGroup->CancelTextCursor();
	}


void CT_DataWindowGroup::DoCmdSetOwningWindowGroupL(const TDesC& aSection)
	{
	TInt	winGroupId=0;
	TBool	hasWGIdentifier=ETrue;

	if ( !GetIntFromConfig(aSection, KOwningWGIdentifier(), winGroupId) )
		{

		CDataWrapperBase*	wrapper=NULL;
		TPtrC	objectName;
		if ( GetStringFromConfig(aSection, KOwningWGIdName(), objectName) )
			{
			wrapper=static_cast<CDataWrapperBase*>(GetDataWrapperL(objectName));
			}
			
		if ( wrapper!=NULL )
			{
			winGroupId = wrapper->GetIdentifier();
			
			INFO_PRINTF2(_L("get id:%d"), winGroupId);
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KOwningWGIdentifier);
			SetBlockResult(EFail);
			hasWGIdentifier = EFalse;
			}
		}

	if ( !hasWGIdentifier )
		{
		ERR_PRINTF2(KLogMissingParameter, &KOwningWGIdentifier);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetOwningWindowGroup(TInt)"));
		iWindowGroup->SetOwningWindowGroup(winGroupId);
		}
	}


void CT_DataWindowGroup::DoCmdDefaultOwningWindow()
	{
	INFO_PRINTF1(_L("execute DefaultOwningWindow()"));
	iWindowGroup->DefaultOwningWindow();
	}


void CT_DataWindowGroup::DoCmdSetName(const TDesC& aSection)
	{
	TPtrC	theNameString;

	if ( !GetStringFromConfig(aSection, KinputWGN(), theNameString) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KinputWGN);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute SetName(const TDesC)"));
		TInt	err = iWindowGroup->SetName(theNameString);

		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWindowGroup::DoCmdName(const TDesC& aSection)
	{
	TBuf<KMaxTestExecuteCommandLength>	storeBuffer;

	INFO_PRINTF1(_L("execute Name(TDes&)"));
	TInt	err = iWindowGroup->Name(storeBuffer);

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	else
		{
		INFO_PRINTF2(_L("the length of  name:%d"), storeBuffer.Length());
		INFO_PRINTF1(_L("the name of window group:"));
		INFO_PRINTF1(storeBuffer);

		TPtrC	returnWGName;
		if ( !GetStringFromConfig(aSection, KexpectedReturnWGN(), returnWGName) )
			{
			ERR_PRINTF2(KLogMissingExpectedValue, &KexpectedReturnWGN());
			SetBlockResult(EFail);
			}
		else
			{
			if (storeBuffer!=returnWGName)
				{
				ERR_PRINTF4(KLogNotExpectedValueString, &KexpectedReturnWGN, &returnWGName, &storeBuffer);
				SetBlockResult(EFail);
				}
			}
		}
	}


void CT_DataWindowGroup::DoCmdIdentifier()
	{
	INFO_PRINTF1(_L("execute Identifier()"));
	TInt	actualWGId=iWindowGroup->Identifier();
	INFO_PRINTF2(_L("Identifier:%d"),actualWGId);
	SetIdentifier(actualWGId);
	}


void CT_DataWindowGroup::DoCmdDisableKeyClick(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datState;
	if ( !GetBoolFromConfig(aSection, KFldState, datState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldState);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute DisableKeyClick(TBool)"));
		iWindowGroup->DisableKeyClick(datState);
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindowGroup::DoCmdEnableScreenChangeEvents()
	{
	INFO_PRINTF1(_L("execute EnableScreenChangeEvents()"));
	TInt err = iWindowGroup->EnableScreenChangeEvents();

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataWindowGroup::DoCmdDisableScreenChangeEvents()
	{
	INFO_PRINTF1(_L("execute DisableScreenChangeEvents()"));
	iWindowGroup->DisableScreenChangeEvents();
	}


void CT_DataWindowGroup::DoCmdSimulatePointerEvent(const TDesC& aSection)
	{
	TBool		dataOk=ETrue;
	TRawEvent	event;

	TRawEvent::TType	type=TRawEvent::ENone;
	if ( !CT_GraphicsUtil::ReadType(*this, aSection, KFldType(), type) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldType());
		SetBlockResult(EFail);
		}
	else
		{
		event.Set(type);
		}

	TStdScanCode	stdScanCode;
	if ( CT_GraphicsUtil::ReadStdScanCode(*this, aSection, KFldStdScanCode(), stdScanCode) )
		{
		event.Set(type, stdScanCode);
		}

	TInt	deviceNumber;
	if ( GetIntFromConfig(aSection, KFldDeviceNumber, deviceNumber) )
		{
		event.SetDeviceNumber(deviceNumber);
		}

	TPoint	pointerPos(0,0);
	if ( GetPointFromConfig(aSection, KFldPos, pointerPos) )
		{
		event.Set(type, pointerPos.iX, pointerPos.iY);
		}

	TBool	tip;
	if ( GetBoolFromConfig(aSection, KFldTip, tip) )
		{
		event.SetTip(tip);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute SimulatePointerEvent(TRawEvent)"));
		iWindowGroup->SimulatePointerEvent(event);
		}
	}


void CT_DataWindowGroup::DoCmdSetChildGroupL(const TDesC& aSection)
	{
	TBool	dataOk = ETrue;

	TInt	id;
	if ( !GetIntFromConfig(aSection, KFldID, id) )
		{
		CDataWrapperBase*	wrapper=NULL;
		TPtrC	objectName;
		if ( GetStringFromConfig(aSection, KFldIDName(), objectName) )
			{
			wrapper=static_cast<CDataWrapperBase*>(GetDataWrapperL(objectName));
			//handle=wrapper->GetHandle();
			}
			
		if ( wrapper!=NULL )
			{
			id = wrapper->GetIdentifier();
			
			INFO_PRINTF3(_L("get '%S_name' identifier = %d' to set child group"), &KFldID, id);
			}
		else
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldID);
			SetBlockResult(EFail);
			dataOk = EFalse;
			}
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("execute SetChildGroup(TInt)"));
		TInt	returnCode = iWindowGroup->SetChildGroup(id);

		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}
		}
	}

void CT_DataWindowGroup::DoCmdClearChildGroup()
	{
	INFO_PRINTF1(_L("execute ClearChildGroup()"));
	TInt	returnCode = iWindowGroup->ClearChildGroup();

	if ( returnCode!=KErrNone )
		{
		ERR_PRINTF2(KLogError, returnCode);
		SetError(returnCode);
		}
	}
