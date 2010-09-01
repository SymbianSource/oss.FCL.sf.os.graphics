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

#include "T_DataWsSession.h"
#include "T_GraphicsUtil.h"
#include "T_DataWindowGroup.h"

//	EPOC Includes
#include <apgtask.h>
#include <w32std.h>
#include <gdi.h>

/*@{*/
_LIT(KDataClassname,							"RWsSession");

///	Fields
_LIT(KFldCheckWithStore,						"checkwithstore");
_LIT(KFldExpected,								"expected");
_LIT(KFldKeyCode,								"keycode");
_LIT(KFldModifier,								"modifier");
_LIT(KFldModifierMask,							"modifiermask");
_LIT(KFldModifiers,								"modifiers");
_LIT(KFldNullPointer,							"nullpointer");
_LIT(KFldOffset,								"offset");
_LIT(KFldRepeats,								"repeats");
_LIT(KFldStdScanCode,							"stdscancode");
_LIT(KFldType,									"type");
_LIT(KFldStore,									"store");
_LIT(KFldPos,									"pos");
_LIT(KFldDeviceNumber,							"devicenumber");
_LIT(KFldTip,									"tip");
_LIT(KFldIdentifier,							"identifier");
_LIT(KFldSpriteMember,							"spritemember");
_LIT(KFldSpriteFlags,							"spriteflags");
_LIT(KFldAlignment,								"alignment");
_LIT(KFldWindowGroup,							"windowgroup");

///	Parameters

_LIT(KLogMessage,	 							"logMessage");
_LIT(KLogCommand,	 							"logCommand");
_LIT(KTPoint,	 								"point");
_LIT(KInputWGId,			  					"inputWGId");
_LIT(KBGColour,									"bgcolour");
_LIT(KInputCursorNumber,						"inputcursornumber");
_LIT(KComputeMode,								"computemode");
_LIT(KInputScrnNo,	  							"inputscreenno");
_LIT(KDefDisplayMode,	  						"defDisplayMode");
_LIT(KDefModeMaxNumColor,	  					"defModeMaxNumColor");
_LIT(KDefModeMaxNumGray,	  					"defModeMaxNumGray");
_LIT(KWindowGroupIdentifier,					"wgIdentifier");
_LIT(KInputDefaultCursorNumber,					"defaultsyscurnum");
_LIT(KUseRfs,									"userfs");
_LIT(KExpectedHeapCount,    					"heapCount");
_LIT(KExpectPtrCursorMode,    					"expectptrcurmode");
_LIT(KExpectBgColour,							"expectbgcolour");
_LIT(KExpectedDiff,								"expected_diff");
_LIT(KSetBuffer,			  					"setbuffer");
_LIT(KSetMaxBuffer,			  					"setmaxbuffer");
_LIT(KSetFocusScreen,		  					"setfocusscreen");
_LIT(KSetEventOffOn,							"seteventoffon");
_LIT(KWTreeNodeObj,								"treenodeobj");
_LIT(KExpectShadowVector,						"expectshdwvet");
_LIT(KAutoFlushState,		  					"newFlushstate");
_LIT(KExpectFlushState,		  					"expectedFlushstate");
_LIT(KVersionBuild,			  					"build");
_LIT(KVersionMajor,			  					"major");
_LIT(KVersionMinor,			  					"minor");
_LIT(KWGUseArray,			  					"usearray");
_LIT(KWGListPriority,							"priority");
_LIT(KWGGivenPriority,							"givenwgpriority");
_LIT(KWGSpecifieScrnNo,							"specifiescrnno");
_LIT(KWGScreenNumber,							"screennum");
_LIT(KWGOrdinalId,								"identifier");
_LIT(KWGOrdinalPosition,						"position");
_LIT(KWGOrdinalPriority,						"wgid");
_LIT(KWGinquiredId,								"inquiredwgid");
_LIT(KFindWGPreId,								"prewgid");
_LIT(KMatchWGString,							"matchwgname");
_LIT(KThreadId,									"threadid");
_LIT(KRequestEvent,								"requestevent");
_LIT(KRequestRedraw,							"requestRedraw");
_LIT(KPKPriorityKey,							"prioritykey");
_LIT(KSetHotKey,								"hotkey");
_LIT(KClsHotKey,								"clshotkey");
_LIT(KRestoreDefHotKey,							"restoredefhk");
_LIT(KSetKeyCode,								"keycode");
_LIT(KDblClickInterval,							"maxinterval");
_LIT(KDblClickDistance,							"maxdistance");
_LIT(KExpectedDblMaxInitialTime,				"expecteddblmaxinittime");
_LIT(KExpectedDblDistance,						"expecteddblmaxdistance");
_LIT(KSetKBRepRateInterval,						"kbreprateinterval");
_LIT(KSetKBRepRateTime,							"kbrepratetime");
_LIT(KWGGetExpectedKBInitialTime,				"expectedkbinittime");
_LIT(KWGGetExpectedKBRepeatTime,				"expectedkbRepeattime");
_LIT(KSetSysFading,								"sysfadonoff");
_LIT(KSetblackMapFading,						"blackmapfading");
_LIT(KSetwhiteMapFading,						"whitemapfading");
_LIT(KSetDefblackMapFading,						"defblackmapfading");
_LIT(KSetDefwhiteMapFading,						"defwhitemapfading");
_LIT(KSetShadowVec,								"shadowvector");
_LIT(KSetScrnSizeMode,							"screensizemode");
_LIT(KSetPtrCurPosValue,						"ptrcurpos");
_LIT(KPtrCurPosArea,							"ptrcurarea");
_LIT(KSetPtrCurMode,							"pointercurmode");
_LIT(KSetCltCurMode,							"cltcurmode");
_LIT(KInputPermanentModifier,					"inputpermanmodif");
_LIT(KInputPermanentModifierState,				"inputpermanmodifstate");

//Commands
_LIT(KCmdnew,									"new");
_LIT(KCmdConnect,								"Connect");
_LIT(KCmdClose,									"Close");
_LIT(KCmdVersion,								"Version");
_LIT(KCmdSetHotKey,								"SetHotKey");
_LIT(KCmdClearHotKeys,							"ClearHotKeys");
_LIT(KCmdRestoreDefaultHotKey,					"RestoreDefaultHotKey");
_LIT(KCmdEventReady,							"EventReady");
_LIT(KCmdEventReadyCancel,						"EventReadyCancel");
_LIT(KCmdGetEvent,								"GetEvent");
_LIT(KCmdPurgePointerEvents,					"PurgePointerEvents");
_LIT(KCmdRedrawReady,							"RedrawReady");
_LIT(KCmdRedrawReadyCancel,						"RedrawReadyCancel");
_LIT(KCmdGetRedraw,								"GetRedraw");
_LIT(KCmdPriorityKeyReady,						"PriorityKeyReady");
_LIT(KCmdPriorityKeyReadyCancel,				"PriorityKeyReadyCancel");
_LIT(KCmdGetPriorityKey,						"GetPriorityKey");
_LIT(KCmdFlush,									"Flush");
_LIT(KCmdSetAutoFlush,							"SetAutoFlush");
_LIT(KCmdSetKeyboardRepeatRate,					"SetKeyboardRepeatRate");
_LIT(KCmdGetKeyboardRepeatRate,					"GetKeyboardRepeatRate");
_LIT(KCmdGetDoubleClickSettings,				"GetDoubleClickSettings");
_LIT(KCmdSetDoubleClick,						"SetDoubleClick");
_LIT(KCmdNumWindowGroups,						"NumWindowGroups");
_LIT(KCmdWindowGroupList,						"WindowGroupList");
_LIT(KCmdGetFocusWindowGroup,					"GetFocusWindowGroup");
_LIT(KCmdGetDefaultOwningWindow,				"GetDefaultOwningWindow");
_LIT(KCmdSetWindowGroupOrdinalPosition,			"SetWindowGroupOrdinalPosition");
_LIT(KCmdGetWindowGroupClientThreadId,			"GetWindowGroupClientThreadId");
_LIT(KCmdGetWindowGroupHandle,					"GetWindowGroupHandle");
_LIT(KCmdGetWindowGroupOrdinalPriority,			"GetWindowGroupOrdinalPriority");
_LIT(KCmdGetWindowGroupNameFromIdentifier,		"GetWindowGroupNameFromIdentifier");
_LIT(KCmdFindWindowGroupIdentifier,				"FindWindowGroupIdentifier");
_LIT(KCmdFetchMessage,							"FetchMessage");
_LIT(KCmdSetShadowVector,						"SetShadowVector");
_LIT(KCmdShadowVector,							"ShadowVector");
_LIT(KCmdSetBackgroundColor,					"SetBackgroundColor");
_LIT(KCmdGetBackgroundColor,					"GetBackgroundColor");
_LIT(KCmdSetSystemPointerCursor,				"SetSystemPointerCursor");
_LIT(KCmdClearSystemPointerCursor,				"ClearSystemPointerCursor");
_LIT(KCmdClaimSystemPointerCursorList,			"ClaimSystemPointerCursorList");
_LIT(KCmdFreeSystemPointerCursorList,			"FreeSystemPointerCursorList");
_LIT(KCmdSetCustomTextCursor,					"SetCustomTextCursor");
_LIT(KCmdResourceCount,							"ResourceCount");
_LIT(KCmdPasswordEntered,						"PasswordEntered");
_LIT(KCmdComputeMode,							"ComputeMode");
_LIT(KCmdHeapCount,								"HeapCount");
_LIT(KCmdSetModifierState,						"SetModifierState");
_LIT(KCmdGetModifierState,						"GetModifierState");
_LIT(KCmdRequestOffEvents,						"RequestOffEvents");
_LIT(KCmdGetDefModeMaxNumColors,				"GetDefModeMaxNumColors");
_LIT(KCmdGetColorModeList,						"GetColorModeList");
_LIT(KCmdSetPointerCursorArea,					"SetPointerCursorArea");
_LIT(KCmdPointerCursorArea,						"PointerCursorArea");
_LIT(KCmdSetPointerCursorMode,					"SetPointerCursorMode");
_LIT(KCmdSetClientCursorMode,					"SetClientCursorMode");
_LIT(KCmdPointerCursorMode,						"PointerCursorMode");
_LIT(KCmdSetDefaultSystemPointerCursor,			"SetDefaultSystemPointerCursor");
_LIT(KCmdClearDefaultSystemPointerCursor,		"ClearDefaultSystemPointerCursor");
_LIT(KCmdSetPointerCursorPosition,				"SetPointerCursorPosition");
_LIT(KCmdPointerCursorPosition,					"PointerCursorPosition");
_LIT(KCmdSetDefaultFadingParameters,			"SetDefaultFadingParameters");
_LIT(KCmdPrepareForSwitchOff,					"PrepareForSwitchOff");
_LIT(KCmdSetBufferSizeL,						"SetBufferSizeL");
_LIT(KCmdSetMaxBufferSizeL,						"SetMaxBufferSizeL");
_LIT(KCmdSetSystemFaded,						"SetSystemFaded");
_LIT(KCmdSetFocusScreen,						"SetFocusScreen");
_LIT(KCmdGetFocusScreen,						"GetFocusScreen");
_LIT(KCmdClearAllRedrawStores,					"ClearAllRedrawStores");
_LIT(KCmdLogCommand,							"LogCommand");
_LIT(KCmdLogMessage,							"LogMessage");
_LIT(KCmdNumberOfScreens,						"NumberOfScreens");
_LIT(KCmdSimulateRawEvent,						"SimulateRawEvent");
_LIT(KCmdSimulateKeyEvent,						"SimulateKeyEvent");

///	Logging
_LIT(KLogError,									"Error=%d");
_LIT(KLogMissingParameter,						"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,						"Not expected value");

//constants 
const TInt  KGroupNameMaxSize =100;

/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsSession* CT_DataWsSession::NewL()
	{
	CT_DataWsSession*    ret=new (ELeave) CT_DataWsSession();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsSession::CT_DataWsSession()
:	CDataWrapperBase()
,	iWsSession(NULL)
,	iResourceCount(0)
,	iNumWinGroup(0)
,	iColorModeCount(0)
,	iWindowGroupIdentifier(0)
,	iActiveEventReady(NULL)
,	iActivePriorityKeyReady(NULL)
,	iActiveRedrawReady(NULL)
	{
	}

void CT_DataWsSession::ConstructL()
	{
	iActiveEventReady=CActiveCallback::NewL(*this);
	iActivePriorityKeyReady=CActiveCallback::NewL(*this);
	iActiveRedrawReady=CActiveCallback::NewL(*this);
	}

CT_DataWsSession::~CT_DataWsSession()
	{
	DestroyData();
	delete iActiveRedrawReady;
	iActiveRedrawReady=NULL;
	delete iActivePriorityKeyReady;
	iActivePriorityKeyReady=NULL;
	delete iActiveEventReady;
	iActiveEventReady=NULL;
	}

void CT_DataWsSession::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsSession = static_cast<RWsSession*> (aAny);
	}

void CT_DataWsSession::DisownObjectL()
	{
	iWsSession = NULL;
	}

void CT_DataWsSession::DestroyData()
	{
	delete iWsSession;
	iWsSession=NULL;
	}

MWsClientClass* CT_DataWsSession::GetClientClass() const
	{
	return iWsSession;
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
TBool CT_DataWsSession::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnew();
		}
	else if ( aCommand==KCmdConnect )
		{
		DoCmdConnectL(aSection);
		}
	else if ( aCommand==KCmdClose )
		{
		DoCmdClose();
		}
	else if ( aCommand==KCmdVersion )
		{
		DoCmdVersion(aSection);
		}
	else if ( aCommand==KCmdSetHotKey )
		{
		DoCmdSetHotKey(aSection);
		}
	else if ( aCommand==KCmdClearHotKeys )
		{
		DoCmdClearHotKeys(aSection);
		}
	else if ( aCommand==KCmdRestoreDefaultHotKey )
		{
		DoCmdRestoreDefaultHotKey(aSection);
		}
	else if ( aCommand==KCmdEventReady )
		{
		DoCmdEventReady(aSection, aAsyncErrorIndex);
		}
	else if ( aCommand==KCmdEventReadyCancel )
		{
		DoCmdEventReadyCancel();
		}
	else if ( aCommand==KCmdGetEvent )
		{
		DoCmdGetEventL(aSection);
		}
	else if ( aCommand==KCmdPurgePointerEvents )
		{
		DoCmdPurgePointerEvents();
		}
	else if ( aCommand==KCmdRedrawReady )
		{
		DoCmdRedrawReady(aSection, aAsyncErrorIndex);
		}
	else if ( aCommand==KCmdRedrawReadyCancel )
		{
		DoCmdRedrawReadyCancel();
		}
	else if ( aCommand==KCmdGetRedraw )
		{
		DoCmdGetRedrawL(aSection);
		}
	else if ( aCommand==KCmdPriorityKeyReady )
		{
		DoCmdPriorityKeyReady(aSection, aAsyncErrorIndex);
		}
	else if ( aCommand==KCmdPriorityKeyReadyCancel )
		{
		DoCmdPriorityKeyReadyCancel();
		}
	else if ( aCommand==KCmdGetPriorityKey )
		{
		DoCmdGetPriorityKeyL(aSection);
		}
	else if ( aCommand==KCmdFlush )
		{
		DoCmdFlush();
		}
	else if ( aCommand==KCmdSetAutoFlush )
		{
		DoCmdSetAutoFlush(aSection);
		}
	else if ( aCommand==KCmdSetKeyboardRepeatRate )
		{
		DoCmdSetKeyboardRepeatRate(aSection);
		}
	else if ( aCommand==KCmdGetKeyboardRepeatRate )
		{
		DoCmdGetKeyboardRepeatRate(aSection);
		}
	else if ( aCommand==KCmdGetDoubleClickSettings )
		{
		DoCmdGetDoubleClickSettings(aSection);
		}
	else if ( aCommand==KCmdSetDoubleClick )
		{
		DoCmdSetDoubleClick(aSection);
		}
	else if ( aCommand==KCmdNumWindowGroups )
		{
		DoCmdNumWindowGroups(aSection);
		}
	else if ( aCommand==KCmdWindowGroupList )
		{
		DoCmdWindowGroupListL(aSection);
		}
	else if ( aCommand==KCmdGetFocusWindowGroup )
		{
		DoCmdGetFocusWindowGroup(aSection);
		}
	else if ( aCommand==KCmdGetDefaultOwningWindow )
		{
		DoCmdGetDefaultOwningWindow(aSection);
		}
	else if ( aCommand==KCmdSetWindowGroupOrdinalPosition )
		{
		DoCmdSetWindowGroupOrdinalPosition(aSection);
		}
	else if ( aCommand==KCmdGetWindowGroupClientThreadId )
		{
		DoCmdGetWindowGroupClientThreadId(aSection);
		}
	else if ( aCommand==KCmdGetWindowGroupHandle )
		{
		DoCmdGetWindowGroupHandle(aSection);
		}
	else if ( aCommand==KCmdGetWindowGroupOrdinalPriority )
		{
		DoCmdGetWindowGroupOrdinalPriority(aSection);
		}
	else if ( aCommand==KCmdGetWindowGroupNameFromIdentifier )
		{
		DoCmdGetWindowGroupNameFromIdentifier(aSection);
		}
	else if ( aCommand==KCmdFindWindowGroupIdentifier )
		{
		DoCmdFindWindowGroupIdentifier(aSection);
		}
	else if ( aCommand==KCmdFetchMessage )
		{
		DoCmdFetchMessage(aSection);
		}
	else if ( aCommand==KCmdSetShadowVector )
		{
		DoCmdSetShadowVector(aSection);
		}
	else if ( aCommand==KCmdShadowVector )
		{
		DoCmdShadowVector(aSection);
		}
	else if ( aCommand==KCmdSetBackgroundColor )
		{
		DoCmdSetBackgroundColor(aSection);
		}
	else if ( aCommand==KCmdGetBackgroundColor )
		{
		DoCmdGetBackgroundColor(aSection);
		}
	else if ( aCommand==KCmdSetSystemPointerCursor )
		{
		DoCmdSetSystemPointerCursor(aSection);
		}
	else if ( aCommand==KCmdClearSystemPointerCursor )
		{
		DoCmdClearSystemPointerCursor(aSection);
		}
	else if ( aCommand==KCmdClaimSystemPointerCursorList )
		{
		DoCmdClaimSystemPointerCursorList();
		}
	else if ( aCommand==KCmdFreeSystemPointerCursorList )
		{
		DoCmdFreeSystemPointerCursorList();
		}
	else if ( aCommand==KCmdSetCustomTextCursor )
		{
		DoCmdSetCustomTextCursorL(aSection);
		}
	else if ( aCommand==KCmdResourceCount )
		{
		DoCmdResourceCount(aSection);
		}
	else if ( aCommand==KCmdPasswordEntered )
		{
		DoCmdPasswordEntered();
		}
	else if ( aCommand==KCmdComputeMode )
		{
		DoCmdComputeMode(aSection);
		}
	else if ( aCommand==KCmdHeapCount )
		{
		DoCmdHeapCount(aSection);
		}
	else if ( aCommand==KCmdSetModifierState )
		{
		DoCmdSetModifierState(aSection);
		}
	else if ( aCommand==KCmdGetModifierState )
		{
		DoCmdGetModifierState(aSection);
		}
	else if ( aCommand==KCmdRequestOffEvents )
		{
		DoCmdRequestOffEventsL(aSection);
		}
	else if ( aCommand==KCmdGetDefModeMaxNumColors )
		{
		DoCmdGetDefModeMaxNumColors(aSection);
		}
	else if ( aCommand==KCmdGetColorModeList )
		{
		DoCmdGetColorModeListL(aSection);
		}
	else if ( aCommand==KCmdSetPointerCursorArea )
		{
		DoCmdSetPointerCursorArea(aSection);
		}
	else if ( aCommand==KCmdPointerCursorArea )
		{
		DoCmdPointerCursorArea(aSection);
		}
	else if ( aCommand==KCmdSetPointerCursorMode )
		{
		DoCmdSetPointerCursorMode(aSection);
		}
	else if ( aCommand==KCmdSetClientCursorMode )
		{
		DoCmdSetClientCursorMode(aSection);
		}
	else if ( aCommand==KCmdPointerCursorMode )
		{
		DoCmdPointerCursorMode(aSection);
		}
	else if ( aCommand==KCmdSetDefaultSystemPointerCursor )
		{
		DoCmdSetDefaultSystemPointerCursor(aSection);
		}
	else if ( aCommand==KCmdClearDefaultSystemPointerCursor )
		{
		DoCmdClearDefaultSystemPointerCursor();
		}
	else if ( aCommand==KCmdSetPointerCursorPosition )
		{
		DoCmdSetPointerCursorPosition(aSection);
		}
	else if ( aCommand==KCmdPointerCursorPosition )
		{
		DoCmdPointerCursorPosition(aSection);
		}
	else if ( aCommand==KCmdSetDefaultFadingParameters )
		{
		DoCmdSetDefaultFadingParameters(aSection);
		}
	else if ( aCommand==KCmdPrepareForSwitchOff )
		{
		DoCmdPrepareForSwitchOff();
		}
	else if ( aCommand==KCmdSetBufferSizeL )
		{
		DoCmdSetBufferSizeL(aSection);
		}
	else if ( aCommand==KCmdSetMaxBufferSizeL )
		{
		DoCmdSetMaxBufferSizeL(aSection);
		}
	else if ( aCommand==KCmdSetSystemFaded )
		{
		DoCmdSetSystemFaded(aSection);
		}
	else if ( aCommand==KCmdSetFocusScreen )
		{
		DoCmdSetFocusScreen(aSection);
		}
	else if ( aCommand==KCmdGetFocusScreen )
		{
		DoCmdGetFocusScreen(aSection);
		}
	else if ( aCommand==KCmdClearAllRedrawStores )
		{
		DoCmdClearAllRedrawStores();
		}
	else if ( aCommand==KCmdLogCommand )
		{
		DoCmdLogCommand(aSection);
		}
	else if ( aCommand==KCmdLogMessage )
		{
		DoCmdLogMessage(aSection);
		}
	else if ( aCommand==KCmdNumberOfScreens )
		{
		DoCmdNumberOfScreens(aSection);
		}
	else if ( aCommand==KCmdSimulateRawEvent )
		{
		DoCmdSimulateRawEvent(aSection);
		}
	else if ( aCommand==KCmdSimulateKeyEvent )
		{
		DoCmdSimulateKeyEvent(aSection);
		}
	else
		{
		ret=CT_DataWsClientClass::DoCommandL(*this, aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataWsSession::DoCmdnew()
	{
	INFO_PRINTF1(_L("Session is going to create"));
	TRAPD(err,	iWsSession = new (ELeave) RWsSession());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsSession::DoCmdConnectL(const TDesC& aSection)
	{
	TBool	useRfs=EFalse;
	GetBoolFromConfig(aSection, KUseRfs(), useRfs);

	TInt	err=KErrNone;
	if ( useRfs )
		{
		INFO_PRINTF1(_L("RWsSession::Connect(RFs&) call"));
		err=iWsSession->Connect(FileServer());
		}
	else
		{
		INFO_PRINTF1(_L("RWsSession::Connect() call"));
		err=iWsSession->Connect();
		}
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsSession::DoCmdClose()
	{
	INFO_PRINTF1(_L("RWsSession::Close() call"));
	iWsSession->Close();
	}

void CT_DataWsSession::DoCmdVersion(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("WsSession::Version() call"));

	TVersion		version		= iWsSession->Version();

	INFO_PRINTF2(_L("Version build : %d"), (TInt)version.iBuild);
	INFO_PRINTF2(_L("Version major : %d"), (TInt)version.iMajor);
	INFO_PRINTF2(_L("Version minor : %d"), (TInt)version.iMinor);
	TInt	intTemp;
	if ( GetIntFromConfig(aSection, KVersionBuild(), intTemp) )
		{
		if ( intTemp != version.iBuild )
			{
			ERR_PRINTF1(_L("Build does not match expected build"));
			SetBlockResult(EFail);
			}
		}

	if ( GetIntFromConfig(aSection, KVersionMajor(), intTemp) )
		{
		if ( intTemp != version.iMajor )
			{
			ERR_PRINTF1(_L("Major does not match expected major"));
			SetBlockResult(EFail);
			}
		}

	if ( GetIntFromConfig(aSection, KVersionMinor(), intTemp) )
		{
		if ( intTemp != version.iMinor )
			{
			ERR_PRINTF1(_L("Minor does not match expected minor"));
			SetBlockResult(EFail);
			}
		}

	if (version.Name() == _L("") && version.iBuild == 0 && version.iMajor == 0 && version.iMinor == 0)
		{
		ERR_PRINTF1(_L("Some version fields are not set!"));
		SetBlockResult(EFail);
		}
	}

void CT_DataWsSession::DoCmdSetHotKey(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	THotKey	hotKey;
	TInt	err=KErrNone;
	
	if ( !CT_GraphicsUtil::ReadHotKey(*this, aSection, KSetHotKey(), hotKey) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KSetHotKey());
		SetBlockResult(EFail);
		}

	TKeyCode	keyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KSetKeyCode(), keyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KSetKeyCode());
		SetBlockResult(EFail);
		}

	TUint	modifierMask=EModifierAlt | EModifierCtrl | EModifierShift | EModifierFunc | EModifierCapsLock;
	CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifierMask, modifierMask);
	TUint	modifier=EModifierAlt | EModifierCtrl | EModifierShift | EModifierFunc | EModifierCapsLock;
	CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifier, modifier);
	if ( dataOk )
		{
		err = iWsSession->SetHotKey(hotKey, keyCode, modifierMask, modifier);
		}
	
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsSession::DoCmdClearHotKeys(const TDesC& aSection)
	{
	THotKey	hotKey;
	
	TInt	err=KErrNone;
	if ( !CT_GraphicsUtil::ReadHotKey(*this, aSection, KClsHotKey(), hotKey) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KClsHotKey());
		SetBlockResult(EFail);
		}
	else
		{
		err = iWsSession->ClearHotKeys(hotKey);
		}
	
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsSession::DoCmdRestoreDefaultHotKey(const TDesC& aSection)
	{
	THotKey	hotKey;
	
	TInt	err=KErrNone;
	if ( !CT_GraphicsUtil::ReadHotKey(*this, aSection, KRestoreDefHotKey(), hotKey) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KRestoreDefHotKey());
		SetBlockResult(EFail);
		}
	else
		{
		err=iWsSession->RestoreDefaultHotKey(hotKey);
		}
	
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
		
	}

void CT_DataWsSession::DoCmdEventReady(const TDesC& aSection, TInt aAsyncErrorIndex)
	{
	CActive::TPriority	priority=CActive::EPriorityHigh;
	CT_GraphicsUtil::ReadPriority(*this, aSection, KRequestEvent(), priority);

	iActiveEventReady->SetPriority(priority);
	INFO_PRINTF1(_L("RWsSession::EventReady() call"));
	iWsSession->EventReady(&iActiveEventReady->iStatus);
	iActiveEventReady->Activate(aAsyncErrorIndex);
	IncOutstanding();
	}

void CT_DataWsSession::DoCmdEventReadyCancel()
	{
	iWsSession->EventReadyCancel();
	}

void CT_DataWsSession::DoCmdGetEventL(const TDesC& aSection)
	{
	TWsEvent	event;

	iWsSession->GetEvent(event);
	INFO_PRINTF1(_L("RWsSession::GetEvent() call"));

	TWsEvent*	store=NULL;
	if ( CT_GraphicsUtil::GetWsEventL(*this, aSection, KFldStore(), store) )
		{
		*store=event;
		}
	}

void CT_DataWsSession::DoCmdPurgePointerEvents()
	{
	// Execute command and log parameters
	INFO_PRINTF3(_L("execute %S::%S()"), &KDataClassname, &KCmdPurgePointerEvents);
	iWsSession->PurgePointerEvents();
	}

void CT_DataWsSession::DoCmdRedrawReady(const TDesC& aSection, TInt aAsyncErrorIndex)
	{
	CActive::TPriority	priority=CActive::EPriorityHigh;
	CT_GraphicsUtil::ReadPriority(*this, aSection, KRequestRedraw(), priority);

	iActiveRedrawReady->SetPriority(priority);
	INFO_PRINTF1(_L("RWsSession::RedrawReady() call"));
	iWsSession->RedrawReady(&iActiveRedrawReady->iStatus);
	iActiveRedrawReady->Activate(aAsyncErrorIndex);
	IncOutstanding();
	}

void CT_DataWsSession::DoCmdRedrawReadyCancel()
	{
	INFO_PRINTF1(_L("RWsSession::RedrawReadyCancel() call"));
	iWsSession->RedrawReadyCancel();
	}

void CT_DataWsSession::DoCmdGetRedrawL(const TDesC& aSection)
	{
	TWsRedrawEvent 	redrawEvent;

	iWsSession->GetRedraw(redrawEvent);
	INFO_PRINTF1(_L("RWsSession::GetRedraw() call"));

	TWsRedrawEvent*	store=NULL;
	if ( CT_GraphicsUtil::GetWsRedrawEventL(*this, aSection, KFldStore(), store) )
		{
		*store=redrawEvent;
		}
	}

void CT_DataWsSession::DoCmdPriorityKeyReady(const TDesC& aSection, TInt aAsyncErrorIndex)
	{
	CActive::TPriority	priority=CActive::EPriorityHigh;
	CT_GraphicsUtil::ReadPriority(*this, aSection, KPKPriorityKey(), priority);

	iActivePriorityKeyReady->SetPriority(priority);
	INFO_PRINTF1(_L("RWsSession::PriorityKeyReady() call"));
	iWsSession->PriorityKeyReady(&iActivePriorityKeyReady->iStatus);
	iActivePriorityKeyReady->Activate(aAsyncErrorIndex);
	IncOutstanding();
	}


void CT_DataWsSession::DoCmdPriorityKeyReadyCancel()
	{
	INFO_PRINTF1(_L("RWsSession::PriorityKeyReadyCancel() call"));
	iWsSession->PriorityKeyReadyCancel();
	}

void CT_DataWsSession::DoCmdGetPriorityKeyL(const TDesC& aSection)
	{
	TWsPriorityKeyEvent  	event;

	iWsSession->GetPriorityKey(event);
	INFO_PRINTF1(_L("RWsSession::GetPriorityKey() call"));

	TWsPriorityKeyEvent *	store=NULL;
	if ( CT_GraphicsUtil::GetWsPriorityKeyEventL(*this, aSection, KFldStore(), store) )
		{
		*store=event;
		}
	}

void CT_DataWsSession::DoCmdFlush()
	{
	INFO_PRINTF1(_L("RWsSession::Flush() call"));
	iWsSession->Flush();
	}

void CT_DataWsSession::DoCmdSetAutoFlush(const TDesC& aSection)
	{
	TBool	actualFlushstate;
	TBool	setFlushState=EFalse;
	TBool	expectedFlush=EFalse;

	if ( !GetBoolFromConfig(aSection, KAutoFlushState(), setFlushState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KAutoFlushState());
		SetBlockResult(EFail);
		}
	else
		{
		actualFlushstate = iWsSession->SetAutoFlush(setFlushState);
		INFO_PRINTF2(_L("Flush State: %d"),actualFlushstate);
		
		if ( !GetBoolFromConfig(aSection, KExpectFlushState(), expectedFlush) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KExpectFlushState());
			SetBlockResult(EFail);
			}
		else
			{
			if ( actualFlushstate!=expectedFlush)
				{
				ERR_PRINTF1(_L("SetAutoFlush is not as expected!"));
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataWsSession::DoCmdSetKeyboardRepeatRate(const TDesC& aSection)
	{
    TInt	setInputInterval=iKeyboardRepeateRateInitialTime.Int();
    GetIntFromConfig(aSection, KSetKBRepRateInterval(), setInputInterval);

	TInt	setInputTime=iKeyboardRepeateRateTime.Int();
	GetIntFromConfig(aSection, KSetKBRepRateTime(), setInputTime);

	TTimeIntervalMicroSeconds32 changeInitialTime(setInputInterval);
	TTimeIntervalMicroSeconds32 changeTime(setInputTime);
	iWsSession->SetKeyboardRepeatRate(changeInitialTime, changeTime);
	}

void CT_DataWsSession::DoCmdGetKeyboardRepeatRate(const TDesC& aSection)
	{
	TTimeIntervalMicroSeconds32	actualInitialTime;
	TTimeIntervalMicroSeconds32	actualTime;
	iWsSession->GetKeyboardRepeatRate(actualInitialTime, actualTime);
	INFO_PRINTF2(_L("actual Interval Time: %d"), actualInitialTime.Int());
	INFO_PRINTF2(_L("actual Time: %d"), actualTime.Int());

	TInt	expectInitialTime;
	if(	GetIntFromConfig(aSection, KWGGetExpectedKBInitialTime(), expectInitialTime))
		{
		if ( actualInitialTime.Int()!=expectInitialTime )
			{
			ERR_PRINTF1(_L("GetKeyboardRepeatRate is not as expected!"));
			SetBlockResult(EFail);
			}
		}

	TInt	expectTime;
	if ( GetIntFromConfig(aSection, KWGGetExpectedKBRepeatTime(), expectTime) )
		{
		if ( actualTime.Int()!=expectTime)
			{
			ERR_PRINTF1(_L("GetKeyboardRepeatRate is not as expected!"));
			SetBlockResult(EFail);
			}
		}

	TBool	checkWithStore=FALSE;
	GetBoolFromConfig(aSection, KFldCheckWithStore(), checkWithStore);
	if ( checkWithStore )
		{
		if ( iKeyboardRepeateRateInitialTime!=actualInitialTime )
			{
			ERR_PRINTF1(_L("GetKeyboardRepeatRate is not as expected!"));
			SetBlockResult(EFail);
			}
		if ( iKeyboardRepeateRateTime!=actualTime )
			{
			ERR_PRINTF1(_L("GetKeyboardRepeatRate is not as expected!"));
			SetBlockResult(EFail);
			}
		}

	TBool	store=FALSE;
	GetBoolFromConfig(aSection, KFldStore(), store);
	if ( store )
		{
		iKeyboardRepeateRateInitialTime=actualInitialTime;
		iKeyboardRepeateRateTime=actualTime;
		}
	}

void CT_DataWsSession::DoCmdSetDoubleClick(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
    TInt	setMaxInputInterval;
    if ( !GetIntFromConfig(aSection, KDblClickInterval(), setMaxInputInterval) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KDblClickInterval());
		SetBlockResult(EFail);
		}

	TInt	setMaxDistance;
	if ( !GetIntFromConfig(aSection, KDblClickDistance(), setMaxDistance) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KDblClickDistance());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TTimeIntervalMicroSeconds32	changeMaxIntervalTime(setMaxInputInterval);
		iWsSession->SetDoubleClick(changeMaxIntervalTime, setMaxDistance);
		}
	}

void CT_DataWsSession::DoCmdGetDoubleClickSettings(const TDesC& aSection)
	{
	TTimeIntervalMicroSeconds32	actualMaxIntervalValue;
	TInt						actualMaxDistanceValue;
	iWsSession->GetDoubleClickSettings(actualMaxIntervalValue, actualMaxDistanceValue);
	INFO_PRINTF2(_L("Maximum interval: %d"), actualMaxIntervalValue.Int());
	INFO_PRINTF2(_L("Maximum distance : %d"), actualMaxDistanceValue);

	TInt	expectMaxIntervalValue;
	if ( !GetIntFromConfig(aSection, KExpectedDblMaxInitialTime(), expectMaxIntervalValue))
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectedDblMaxInitialTime());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actualMaxIntervalValue.Int()!=expectMaxIntervalValue )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}

	TInt	expectMaxDistanceValue;
	if ( !GetIntFromConfig(aSection, KExpectedDblDistance(), expectMaxDistanceValue))
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectedDblDistance());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actualMaxDistanceValue!=expectMaxDistanceValue)
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdNumWindowGroups(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
	TInt	previousCount=iNumWinGroup;

#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
	TInt	priority;
	if ( GetIntFromConfig(aSection, KWGGivenPriority(), priority) )
		{
		TInt	screenNumber=0;
		if ( !GetIntFromConfig(aSection, KWGSpecifieScrnNo(), screenNumber) )
			{
			iNumWinGroup=iWsSession->NumWindowGroups(priority);
			INFO_PRINTF2(_L("NumWindowGroups(TInt aPriority):%d"), iNumWinGroup);
			}
		else
			{
			iNumWinGroup=iWsSession->NumWindowGroups(screenNumber, priority);
			INFO_PRINTF2(_L("NumWindowGroups(TInt aScreenNumber, TInt aPriority):%d"), iNumWinGroup);
			}
		}
	else
#endif
		{
		iNumWinGroup=iWsSession->NumWindowGroups();
		INFO_PRINTF2(_L("NumWindowGroups():%d"), iNumWinGroup);
		}

	if ( dataOk )
		{
		TInt	difference=0;
		if ( GetIntFromConfig(aSection, KExpectedDiff(), difference) )
			{
			if ( (iNumWinGroup-previousCount)!=difference )
				{
				ERR_PRINTF2(_L("Previous count:%d"), previousCount);
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataWsSession::DoCmdWindowGroupListL(const TDesC& aSection)
	{
	INFO_PRINTF2(_L("iNumWinGroup:%d"), iNumWinGroup);
	RArray<RWsSession::TWindowGroupChainInfo>	numWGListRArray(1);
	CArrayFixFlat<TInt>*						numWGListCArray=new (ELeave) CArrayFixFlat<TInt>(iNumWinGroup);
	CleanupStack::PushL(numWGListCArray);
	CleanupClosePushL(numWGListRArray);

	//True:CArrayFixFlat  Flase:RArray
	TInt	priority;
#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
	TBool	hasPriority=CT_GraphicsUtil::ReadPriorities(*this, aSection, KWGListPriority(), priority);
#else
	TBool	hasPriority=GetIntFromConfig(aSection, KWGListPriority(), priority);
#endif

	TBool	useClassArray;
	if ( !GetBoolFromConfig(aSection, KWGUseArray(), useClassArray) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KWGUseArray());
		SetBlockResult(EFail);
		}
	else
		{
		TInt	err=KErrNone;
		INFO_PRINTF2(_L("Use Class array:%d"), useClassArray);
		
		if ( useClassArray )
			{
			if(	hasPriority )
				{
#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
				TInt	theScrnNum;
				if(	GetIntFromConfig(aSection, KWGScreenNumber(), theScrnNum))
					{
					INFO_PRINTF1(_L("WindowGroupList(CArrayFixFlat< TInt >*, TInt, TInt)"));
					err=iWsSession->WindowGroupList(numWGListCArray, theScrnNum, priority);
					}
				else
#endif
					{
					INFO_PRINTF1(_L("WindowGroupList(TInt, CArrayFixFlat<TInt>*)"));
					err=iWsSession->WindowGroupList(priority, numWGListCArray);
					}
				}
			else
				{
				INFO_PRINTF1(_L("WindowGroupList(CArrayFixFlat<TInt>*)"));
				err=iWsSession->WindowGroupList(numWGListCArray);
				}
			}
		else
			{
			if ( hasPriority )
				{
				INFO_PRINTF1(_L("WindowGroupList(TInt, RArray<RWsSession::TWindowGroupChainInfo>*)"));
				err=iWsSession->WindowGroupList(priority, &numWGListRArray);
				}
			else
				{
				INFO_PRINTF1(_L("WindowGroupList(RArray<RWsSession::TWindowGroupChainInfo>)"));
				err=iWsSession->WindowGroupList(&numWGListRArray);
				}
			}

		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		else
			{
			if ( useClassArray )
				{
				TInt	actual=0;
				INFO_PRINTF2(_L("numWGListCArray:%d"), numWGListCArray->Count());
				for ( TInt index=0; index<numWGListCArray->Count(); ++index )
					{
					// Gets a list of identifiers of all window groups in all window server sessions
					actual=(*numWGListCArray)[index];
					INFO_PRINTF3(_L("CArrayFixFlat[%d]= :%d"), index, actual);
					}
				}
			else
				{
				RWsSession::TWindowGroupChainInfo	actual;
				INFO_PRINTF2(_L("numWGListRArray:%d"), numWGListRArray.Count());
				for ( TInt index=0; index<numWGListRArray.Count(); ++index )
					{
					// Gets a list of identifiers of all window groups in all window server sessions
					actual=numWGListRArray[index];
					INFO_PRINTF4(_L("RArray{%d] iId:%d iParentId:%d"), index, actual.iId, actual.iParentId);
					}
				}
			}
		}

	CleanupStack::PopAndDestroy(2, numWGListCArray);
	}

void CT_DataWsSession::DoCmdGetFocusWindowGroup(const TDesC& aSection)
	{
	TInt	actual=0;

#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
	TInt	theScrnNum=0;
	if ( GetIntFromConfig(aSection, KWGScreenNumber(), theScrnNum) )
		{
		INFO_PRINTF1(_L("RWsSession::GetFocusWindowGroup(TInt aScreenNumber)  call"));
		actual=iWsSession->GetFocusWindowGroup(theScrnNum);
		}
	else
#endif
		{
		INFO_PRINTF1(_L("RWsSession::GetFocusWindowGroup() call"));
		actual=iWsSession->GetFocusWindowGroup();
		}
	INFO_PRINTF2(_L("Window Group Identifier:%d"), actual);

	TInt 	expected=0;
	if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(_L("Window group identifier is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdGetDefaultOwningWindow(const TDesC& aSection)
	{
	TInt	actual=0;
	TBool	dataOk=ETrue;

	TInt	theScrnNum=0;
	if(	GetIntFromConfig(aSection, KWGScreenNumber(), theScrnNum))
		{
#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
		INFO_PRINTF1(_L("RWsSession::GetDefaultOwningWindow(TInt aScreenNumber)  call"));
		actual=iWsSession->GetDefaultOwningWindow(theScrnNum);
#else
		dataOk=EFalse;
		ERR_PRINTF1(_L("SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS Not Supported"));
		SetBlockResult(EFail);
#endif
		}
	else
		{
		INFO_PRINTF1(_L("RWsSession::GetDefaultOwningWindow()  call"));
		actual=iWsSession->GetDefaultOwningWindow();
		}

	if ( dataOk )
		{
		INFO_PRINTF2(_L("default owning window:%d"), actual);

		TInt	expected=0;
		if ( !GetIntFromConfig(aSection, KFldExpected(), expected) )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldExpected());
			SetBlockResult(EFail);
			}
		else
			{
			if ( actual!=expected)
				{
				ERR_PRINTF1(_L("Default Owning Window is not as expected!"));
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataWsSession::DoCmdSetWindowGroupOrdinalPosition(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TInt		 inputWGId;
	if ( !GetIntFromConfig(aSection, KWGOrdinalId(), inputWGId) )
		{
		TPtrC		 objectName;
		if ( GetStringFromConfig(aSection, KFldWindowGroup(), objectName) )
			 {
			 INFO_PRINTF1(_L("Get CT_DataWindowGroup's wrapper. "));
			 CDataWrapperBase* wrapper = NULL;
			 wrapper = static_cast<CDataWrapperBase*>(GetDataWrapperL(objectName));
			 if(wrapper)
				 {
				 inputWGId=wrapper->GetIdentifier();
				 INFO_PRINTF2(_L("Window group's identifier is: %d"), inputWGId);
				 }
			 else
				 {
				 dataOk=EFalse;
				 ERR_PRINTF1(_L("Window group's wrapper pointer is NULL"));
				 SetBlockResult(EFail);
				 }
			 }
		else
			 {
			 dataOk=EFalse;
			 ERR_PRINTF2(KLogMissingParameter, &KWGOrdinalId());
			 SetBlockResult(EFail);
			 }
		}
   	
	TInt	inputWGOrdinalPos;
	if ( !GetIntFromConfig(aSection, KWGOrdinalPosition(), inputWGOrdinalPos) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KWGOrdinalPosition());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		TInt	err=iWsSession->SetWindowGroupOrdinalPosition(inputWGId, inputWGOrdinalPos);

		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsSession::DoCmdGetWindowGroupClientThreadId(const TDesC& aSection)
	{
	TInt	inputWindowGroupIdentifier=0;
	if ( !GetIntFromConfig(aSection, KWindowGroupIdentifier(), inputWindowGroupIdentifier) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KWindowGroupIdentifier());
		SetBlockResult(EFail);
		}
	else
		{
		TThreadId	threadId;
		TInt		err=iWsSession->GetWindowGroupClientThreadId(inputWindowGroupIdentifier, threadId);
		TUint64		actual=threadId.Id();
		INFO_PRINTF1(_L("RWsSession::GetWindowGroupClientThreadId"));
		INFO_PRINTF2(_L("actual ThreadId:%d"), actual);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsSession::DoCmdGetWindowGroupHandle(const TDesC& aSection)
    {
	TInt	inputWGId=0;
	if ( !GetIntFromConfig(aSection, KInputWGId(), inputWGId) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KInputWGId());
		SetBlockResult(EFail);
		}
	else
		{
		TInt	actual=iWsSession->GetWindowGroupHandle(inputWGId);
		INFO_PRINTF2(_L("Window group handle :%d"), actual);
		}
    }

void CT_DataWsSession::DoCmdGetWindowGroupOrdinalPriority(const TDesC& aSection)
    {
	TInt	inputWGId=0;
	if ( !GetIntFromConfig(aSection, KWGOrdinalPriority(), inputWGId))
		{
		ERR_PRINTF2(KLogMissingParameter, &KWGOrdinalPriority());
		SetBlockResult(EFail);
		}
	else
		{
		TInt	actual=iWsSession->GetWindowGroupOrdinalPriority(inputWGId);
		INFO_PRINTF2(_L("Window Group Ordinal Priority:%d"), actual);
		
		TInt	expected;
		if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
			{
			if ( actual!=expected )
				{
				ERR_PRINTF1(KLogNotExpectedValue);
				SetBlockResult(EFail);
				}
			}
		}
    }

void CT_DataWsSession::DoCmdGetWindowGroupNameFromIdentifier(const TDesC& aSection)
	{
	TInt 	inquiredWGId=0;
	TInt	rtnStatus;

	if ( !GetIntFromConfig(aSection, KWGinquiredId(), inquiredWGId) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KWGinquiredId());
		SetBlockResult(EFail);
		}
	else
		{
		TBuf<KGroupNameMaxSize> storeAWindowName;
		INFO_PRINTF2(_L("Window Group inquired Id:%d"),inquiredWGId);
		rtnStatus= iWsSession->GetWindowGroupNameFromIdentifier(inquiredWGId, storeAWindowName);
		INFO_PRINTF2(_L("return status:%d"),rtnStatus);

		if (rtnStatus==KErrNone)
			{
			INFO_PRINTF2(_L("the length of  buffer:%d"),storeAWindowName.Length());
			INFO_PRINTF1(_L("the name of window group:"));
			INFO_PRINTF1(storeAWindowName);
			}
		else
			{
			ERR_PRINTF1(_L("GetWindowGroupNameFromIdentifier function is fail!"));
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdFindWindowGroupIdentifier(const TDesC& aSection)
	{
	TInt	previousId=iWindowGroupIdentifier;
	GetIntFromConfig(aSection, KFindWGPreId(), previousId);
	INFO_PRINTF2(_L("previous window group id:%d"), previousId);

	TInt	nextId;
	TPtrC	matchString;
	if ( GetStringFromConfig(aSection, KMatchWGString(), matchString) )
		{
		TInt	offset=0;
		GetIntFromConfig(aSection, KFldOffset(), offset);
		nextId=iWsSession->FindWindowGroupIdentifier(previousId, matchString, offset);
		}
	else
		{
		RThread		proc;
		TThreadId	threadId=proc.Id();

		TInt	id;
		if ( GetIntFromConfig(aSection, KThreadId(), id) )
			{
			threadId=id;
			}
		nextId=iWsSession->FindWindowGroupIdentifier(previousId, threadId);
		}

	if ( nextId<0 )
		{
		ERR_PRINTF1(_L("failed to find Window group name function !"));
		SetError(nextId);
		}
	else
		{
		iWindowGroupIdentifier=nextId;
		}
	}

void CT_DataWsSession::DoCmdFetchMessage(const TDesC& aSection)
	{
	TUid		uid;
	TPtr8		params(NULL,0);
	TWsEvent	event;
	TInt		err=iWsSession->FetchMessage(uid, params, event);
    TInt expected;
	if ( GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		if ( err!=expected )
			{
			ERR_PRINTF2(KLogError, err);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdSetShadowVector(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	setPtShdwVet;
	if ( !GetPointFromConfig(aSection, KSetShadowVec(), setPtShdwVet) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetShadowVec());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetShadowVector(const TPoint &aVector)"));
		iWsSession->SetShadowVector(setPtShdwVet);
		}
    }

void CT_DataWsSession::DoCmdShadowVector(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	actual=iWsSession->ShadowVector();
	INFO_PRINTF3(_L("execute ShadowVector() x=%d y=%d"), actual.iX, actual.iY);

	TPoint	expected;
	if ( !GetPointFromConfig(aSection, KExpectShadowVector(), expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectShadowVector());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(_L("The current value of the shadow vector is not as expected!"));
			SetBlockResult(EFail);
			}
		}
    }

void CT_DataWsSession::DoCmdSetBackgroundColor(const TDesC& aSection)
	{
	TRgb	datColour;
	if ( !GetRgbFromConfig(aSection, KBGColour, datColour) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KBGColour());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetColor(TRgb)"));
		iWsSession->SetBackgroundColor(datColour);
		}
	}

void CT_DataWsSession::DoCmdGetBackgroundColor(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRgb	actualBgColour=iWsSession->GetBackgroundColor();

	TRgb	expectBgColour;
	if ( !GetRgbFromConfig(aSection, KExpectBgColour(), expectBgColour) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectBgColour());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actualBgColour!=expectBgColour )
			{
			ERR_PRINTF1(_L("The current value of the shadow vector is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdSetSystemPointerCursor(const TDesC& aSection)
    {
    RWsPointerCursor	sysPointerCursor=RWsPointerCursor(*iWsSession);

	INFO_PRINTF1(_L("RWsSession::SetSystemPointerCursor() call"));
	TInt	inputCursorNumber=0;
	if ( !GetIntFromConfig(aSection, KInputCursorNumber(), inputCursorNumber) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectBgColour());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("CursorNumber:%d"),inputCursorNumber);
		TInt	err=iWsSession->SetSystemPointerCursor(sysPointerCursor,inputCursorNumber);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsSession::DoCmdClearSystemPointerCursor(const TDesC& aSection)
	{
	TInt	inputCursorNumber=0;

	if ( !GetIntFromConfig(aSection, KInputCursorNumber(), inputCursorNumber) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KInputCursorNumber());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("CursorNumber:%d"),inputCursorNumber);
		INFO_PRINTF1(_L("RWsSession::ClearSystemPointerCursor"));
		iWsSession->ClearSystemPointerCursor(inputCursorNumber);
		}
	}

void CT_DataWsSession::DoCmdClaimSystemPointerCursorList()
	{
	INFO_PRINTF1(_L("RWsSession::ClaimSystemPointerCursorList() call"));
	TInt	err=iWsSession->ClaimSystemPointerCursorList();

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsSession::DoCmdFreeSystemPointerCursorList()
	{
	INFO_PRINTF1(_L("RWsSession::FreeSystemPointerCursorList() call"));
	iWsSession->FreeSystemPointerCursorList();
	}

void CT_DataWsSession::DoCmdSetCustomTextCursorL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TInt	identifier;
	if ( !GetIntFromConfig(aSection, KFldIdentifier(), identifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldIdentifier);
		SetBlockResult(EFail);
		}

	CArrayFixFlat<TSpriteMember>*	spriteMember = new (ELeave) CArrayFixFlat<TSpriteMember>(1);
	if ( !CT_GraphicsUtil::GetSpriteMemberListL(*this, aSection, KFldSpriteMember, *spriteMember) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSpriteMember);
		SetBlockResult(EFail);
		}
	TArray<TSpriteMember>	spriteArray = spriteMember->Array();

	TSpriteFlags	spriteFlags;
	if ( !CT_GraphicsUtil::ReadSpriteFlags(*this, aSection, KFldSpriteFlags, spriteFlags) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSpriteFlags);
		SetBlockResult(EFail);
		}

	RWsSession::TCustomTextCursorAlignment	alignment;
	if ( !CT_GraphicsUtil::ReadCustomTextCursorAlignment(*this, aSection, KFldAlignment, alignment) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldAlignment);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("SetCustomTextCursor(TInt, TArray< TSpriteMember >, TUint, TCustomTextCursorAlignment)"));
		TInt	err=iWsSession->SetCustomTextCursor(identifier, spriteArray, (TUint)spriteFlags, alignment);

		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsSession::DoCmdResourceCount(const TDesC& aSection)
	{
	TInt	resourceCount=iWsSession->ResourceCount();
	INFO_PRINTF2(_L("RWsSession::ResourceCount() %d"), resourceCount);

	// get expected difference from parameters
	TInt	expectedDifference;
	if ( GetIntFromConfig(aSection, KExpectedDiff(), expectedDifference) )
		{
		TInt	actualDifference=resourceCount-iResourceCount;
		if ( actualDifference != expectedDifference )
			{
			ERR_PRINTF3(_L("Resource count difference is not as expected! expected: %d, actual: %d"), expectedDifference, actualDifference);
			SetBlockResult(EFail);
			}
		}
	iResourceCount = resourceCount;
	}

void CT_DataWsSession::DoCmdPasswordEntered()
	{
	INFO_PRINTF1(_L("RWsSession::PasswordEntered() call"));
	iWsSession->PasswordEntered();
	}

void CT_DataWsSession::DoCmdComputeMode(const TDesC& aSection)
	{
	RWsSession::TComputeMode	mode;

	INFO_PRINTF1(_L("RWsSession::ComputeMode() call"));
	if ( !CT_GraphicsUtil::ReadComputeMode(*this, aSection, KComputeMode(), mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KComputeMode());
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->ComputeMode(mode);
		}
	}


void CT_DataWsSession::DoCmdHeapCount(const TDesC& aSection)
	{
	TInt	expectedCount;
	TInt	heapCount;

	if ( !GetIntFromConfig(aSection, KExpectedHeapCount(), expectedCount) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectedHeapCount());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("expected count: %d"), expectedCount);
		INFO_PRINTF1(_L("RWsSession::HeapCount() call"));
		heapCount=iWsSession->HeapCount();
		INFO_PRINTF2(_L("RWsSession::HeapCount(): %d"), heapCount);
		if ( expectedCount > heapCount )
			{
			ERR_PRINTF3(_L("Heap count is lower than expected! expected: %d, actual: %d"), expectedCount, heapCount);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdSetModifierState(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TEventModifier	eventModifier;
	if ( !CT_GraphicsUtil::ReadEventModifier(*this, aSection, KInputPermanentModifier(), eventModifier) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KInputPermanentModifier());
		SetBlockResult(EFail);
		}

	TModifierState	modifierState;
	if(	!CT_GraphicsUtil::ReadModifierState(*this, aSection, KInputPermanentModifierState(), modifierState) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KInputPermanentModifierState());
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF1(_L("RWsSession::SetModifierState"));
		TInt	err = iWsSession->SetModifierState(eventModifier, modifierState);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsSession::DoCmdGetModifierState(const TDesC& aSection)
    {
	TInt	actual=iWsSession->GetModifierState();
	INFO_PRINTF2(_L("GetModifierState:%d"), actual);

	TUint 	expected=0;
	if ( CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldExpected(), expected) )
		{
		if ( actual!=(TInt)expected )
			{
			ERR_PRINTF1(_L("ModifierState is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdRequestOffEventsL(const TDesC& aSection)
	{
	TBool 	eventOffOn;
	if ( !GetBoolFromConfig(aSection, KSetEventOffOn(), eventOffOn) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetEventOffOn());
		SetBlockResult(EFail);
		}
	else
		{
		RWindowTreeNode*	treeNodeObj=NULL;
		TPtrC 	treeNodeObjectName;
		if ( GetStringFromConfig(aSection, KWTreeNodeObj(), treeNodeObjectName) )
			{
			treeNodeObj = static_cast<RWindowTreeNode*>(GetDataObjectL(treeNodeObjectName));
			}

		INFO_PRINTF1(_L("RWsSession::RequestOffEvents(TBool, RWindowTreeNode*) call"));
		TInt	err=iWsSession->RequestOffEvents(eventOffOn, treeNodeObj);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWsSession::DoCmdGetDefModeMaxNumColors(const TDesC& aSection)
	{
	TInt	colors;
	TInt	grays;

	INFO_PRINTF1(_L("RWsSession::GetDefModeMaxNumColors(TInt &aColor, TInt &aGray) call"));
		
	TDisplayMode	displayMode;
#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
	TInt			theScrnNum=0;
	if ( GetIntFromConfig(aSection, KWGScreenNumber(), theScrnNum) )
		{
		displayMode=iWsSession->GetDefModeMaxNumColors(theScrnNum, colors, grays);
		}
	else
#endif
		{
		displayMode=iWsSession->GetDefModeMaxNumColors(colors, grays);
		}
	INFO_PRINTF4(_L("RWsSession::GetDefModeMaxNumColors() DisplayMode:%d Colors:%d Grays:%d"), displayMode, colors, grays);

	TDisplayMode	expectedDisplayMode;
	if ( CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KDefDisplayMode(), expectedDisplayMode) )
		{
		if ( expectedDisplayMode!=displayMode )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	TInt	expected=0;
	if ( GetIntFromConfig(aSection, KDefModeMaxNumColor(), expected) )
		{
		if ( expected!=colors )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	if ( GetIntFromConfig(aSection, KDefModeMaxNumGray(), expected) )
		{
		if ( expected!=grays )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdGetColorModeListL(const TDesC& aSection)
	{
	TBool	nullPointer=EFalse;
	GetBoolFromConfig(aSection, KFldNullPointer(), nullPointer);

	CArrayFixFlat<TInt>*	modeList=NULL;
	if ( !nullPointer )
		{
		modeList=new (ELeave) CArrayFixFlat<TInt>(iColorModeCount);
		}
	CleanupStack::PushL(modeList);

	TBool	dataOk=ETrue;
	TInt	err=KErrNone;
#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
	TInt	inputScreenNo=0;
	if ( GetIntFromConfig(aSection, KInputScrnNo(), inputScreenNo))
		{
		INFO_PRINTF1(_L("RWsSession::GetColorModeList(TInt aScreenNumber, CArrayFixFlat< TInt > *aModeList) call"));
		err=iWsSession->GetColorModeList(inputScreenNo, modeList);
		}
	else
#endif
		{
		INFO_PRINTF1(_L("RWsSession::GetColorModeList(CArrayFixFlat< TInt > *aModeList) call"));
		err=iWsSession->GetColorModeList(modeList);
		}

	if ( err<KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	else
		{
		if ( dataOk )
			{
			if ( nullPointer )
				{
				iColorModeCount=err;
				}
			else
				{
				INFO_PRINTF2(_L("Color Mode List array count:%d"), modeList->Count());
				for ( TInt index=0; index<modeList->Count(); ++index )
					{
					INFO_PRINTF2(_L("Content of Color Mode List:%d"), (*modeList)[index]);
					}
				}
			}
		}

	CleanupStack::PopAndDestroy(modeList);
	}

void CT_DataWsSession::DoCmdSetPointerCursorArea(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	setRectArea;
	if ( !GetRectFromConfig(aSection, KPtrCurPosArea, setRectArea) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KPtrCurPosArea());
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPointerCursorPosition(TPoint)"));
		
		TInt	setScrnSizeMode;
		if ( !GetIntFromConfig(aSection, KSetScrnSizeMode, setScrnSizeMode) )
			{
			iWsSession->SetPointerCursorArea(setRectArea);
			}
		else
			{
			iWsSession->SetPointerCursorArea(setScrnSizeMode,setRectArea);
			}
		}
	}

void CT_DataWsSession::DoCmdPointerCursorArea(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	actual;
	TInt	theScreenSizeMode=0;
	if ( !GetIntFromConfig(aSection, KSetScrnSizeMode, theScreenSizeMode) )
		{
		actual=iWsSession->PointerCursorArea();
		LogRect(_L("PointerCursorArea()"), actual);
		}
	else
		{
		actual=iWsSession->PointerCursorArea(theScreenSizeMode);
		LogRect(_L("PointerCursorArea(TInt)"), actual);
		}

	TRect	expected;
	if ( !GetRectFromConfig(aSection, KPtrCurPosArea, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KPtrCurPosArea);
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


void CT_DataWsSession::DoCmdSetPointerCursorMode(const TDesC& aSection)
	{
	TPointerCursorMode  mode;
	if ( !CT_GraphicsUtil::ReadPointerCursorMode(*this, aSection, KSetPtrCurMode(), mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetPtrCurMode());
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->SetPointerCursorMode(mode);
		}
	}

void CT_DataWsSession::DoCmdSetClientCursorMode(const TDesC& aSection)
	{
	TPointerCursorMode  mode;
	if ( !CT_GraphicsUtil::ReadPointerCursorMode(*this, aSection, KSetCltCurMode(), mode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetCltCurMode());
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->SetClientCursorMode(mode);
		}
	}

void CT_DataWsSession::DoCmdPointerCursorMode(const TDesC& aSection)
    {
	INFO_PRINTF1(_L("RWsSession::PointerCursorMode() call"));
	TPointerCursorMode	actual=iWsSession->PointerCursorMode();
	INFO_PRINTF2(_L("actual Pointer Cursor Mode:%d"), actual);

	TPointerCursorMode  expected;
	if ( !CT_GraphicsUtil::ReadPointerCursorMode(*this, aSection, KFldExpected(), expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectPtrCursorMode());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(_L("Pointer cursor mode is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdSetDefaultSystemPointerCursor(const TDesC& aSection)
	{
	TInt	inputDefCursorNumber=0;
	INFO_PRINTF1(_L("RWsSession::SetDefaultSystemPointerCursor() call"));

	if ( !GetIntFromConfig(aSection, KInputDefaultCursorNumber(), inputDefCursorNumber))
		{
		ERR_PRINTF2(KLogMissingParameter, &KInputDefaultCursorNumber());
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->SetDefaultSystemPointerCursor(inputDefCursorNumber);
		}
	}

void CT_DataWsSession::DoCmdClearDefaultSystemPointerCursor()
	{
	INFO_PRINTF1(_L("RWsSession::ClearDefaultSystemPointerCursor() call"));
	iWsSession->ClearDefaultSystemPointerCursor();
	}

void CT_DataWsSession::DoCmdSetPointerCursorPosition(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	ptrCurPost;
	if ( !GetPointFromConfig(aSection, KSetPtrCurPosValue, ptrCurPost) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetPtrCurPosValue);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPointerCursorPosition(TPoint)"));
		iWsSession->SetPointerCursorPosition(ptrCurPost);
		}
	}

void CT_DataWsSession::DoCmdPointerCursorPosition(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	actualPtrCurPost=iWsSession->PointerCursorPosition();

	TPoint	expectPtrCurPost;
	if ( !GetPointFromConfig(aSection, KTPoint, expectPtrCurPost) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KTPoint);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPointerCursorPosition(TPoint)"));
		if ((expectPtrCurPost.iX!=expectPtrCurPost.iX)||(expectPtrCurPost.iY!=expectPtrCurPost.iY))
				{
				ERR_PRINTF1(_L("The cursor position is not as expected!"));
				SetBlockResult(EFail);
				}
		}
	}

void CT_DataWsSession::DoCmdSetDefaultFadingParameters(const TDesC& aSection)
	{
	TBool dataOk=ETrue;
	TInt	setBlackMap;
	TInt	setWhiteMap;
	
	if ( !GetIntFromConfig(aSection, KSetDefblackMapFading(), setBlackMap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KSetDefblackMapFading);
		SetBlockResult(EFail);
		}

	if ( !GetIntFromConfig(aSection, KSetDefwhiteMapFading(), setWhiteMap) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KSetDefwhiteMapFading);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		iWsSession->SetDefaultFadingParameters(setBlackMap, setWhiteMap);
		}
	}

void CT_DataWsSession::DoCmdPrepareForSwitchOff()
	{
	INFO_PRINTF1(_L("RWsSession::PrepareForSwitchOff() call"));
	iWsSession->PrepareForSwitchOff();
	}


void CT_DataWsSession::DoCmdSetBufferSizeL(const TDesC& aSection)
	{
	TInt	theBuffersize;

	if ( !GetIntFromConfig(aSection, KSetBuffer(), theBuffersize) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetBuffer);
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->SetBufferSizeL(theBuffersize);
		INFO_PRINTF1(_L("RWsSession::SetBufferSizeL"));
		}
	}

void CT_DataWsSession::DoCmdSetMaxBufferSizeL(const TDesC& aSection)
	{
	TInt	theMaxBuffersize;

	if ( !GetIntFromConfig(aSection, KSetMaxBuffer(), theMaxBuffersize) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetBuffer);
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->SetMaxBufferSizeL(theMaxBuffersize);
		INFO_PRINTF1(_L("RWsSession::SetMaxBufferSizeL"));
		}
	}

void CT_DataWsSession::DoCmdSetSystemFaded(const TDesC& aSection)
	{
	TBool 	systemFadOffOn;
	TInt	blackMap;
	TInt	whiteMap;

	if ( !GetBoolFromConfig(aSection, KSetSysFading(), systemFadOffOn) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetSysFading());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("the status of fad window :%d"),systemFadOffOn);
		if(	GetIntFromConfig(aSection, KSetblackMapFading(), blackMap))
			{
			if ( !GetIntFromConfig(aSection, KSetwhiteMapFading(), whiteMap) )
				{
				ERR_PRINTF2(KLogMissingParameter, &KSetwhiteMapFading);
				SetBlockResult(EFail);
				}
			else
				{
				iWsSession->SetSystemFaded(systemFadOffOn,blackMap,whiteMap);
				INFO_PRINTF1(_L("RWsSession::SetSystemFaded"));
				}
			}
		else
			{
			iWsSession->SetSystemFaded(systemFadOffOn);
			INFO_PRINTF1(_L("RWsSession::SetSystemFaded"));
			}
		}
	}

void CT_DataWsSession::DoCmdSetFocusScreen(const TDesC& aSection)
    {
	TInt	theScreenNo;
	if ( !GetIntFromConfig(aSection, KSetFocusScreen(), theScreenNo) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KSetFocusScreen());
		SetBlockResult(EFail);
		}
	else
		{
		iWsSession->SetFocusScreen(theScreenNo);
		INFO_PRINTF1(_L("RWsSession::SetFocusScreen"));
		}
	}

void CT_DataWsSession::DoCmdGetFocusScreen(const TDesC& aSection)
	{
	TInt 	actual=iWsSession->GetFocusScreen();
	INFO_PRINTF2(_L("RWsSession::GetFocusScreen()=%d"), actual);

	TInt	expected;
	if ( !GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(_L("GetFocusScreen is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsSession::DoCmdClearAllRedrawStores()
	{
	INFO_PRINTF1(_L("RWsSession::ClearAllRedrawStores() call"));
	iWsSession->ClearAllRedrawStores();
	}


void CT_DataWsSession::DoCmdLogCommand(const TDesC& aSection)
	{
	RWsSession::TLoggingCommand	inputCommand;
	if ( !CT_GraphicsUtil::ReadLoggingCommand(*this, aSection, KLogCommand(), inputCommand) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KLogCommand());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("RWsSession::LogCommand() call"));
		iWsSession->LogCommand(inputCommand);
		}
	}


void CT_DataWsSession::DoCmdLogMessage(const TDesC& aSection)
    {
	TPtrC	theMessage;

	if ( !GetStringFromConfig(aSection, KLogMessage(), theMessage) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KLogMessage());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("RWsSession::LogMessage() call"));
		iWsSession->LogMessage(theMessage);
		}
	}

#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
void CT_DataWsSession::DoCmdNumberOfScreens(const TDesC& aSection)
	{
	TInt    actual=iWsSession->NumberOfScreens();
	INFO_PRINTF2(_L("NumberOfScreens:%d"), actual);

	TInt	expected=0;
	if ( !GetIntFromConfig(aSection, KFldExpected(), expected) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF1(_L("NumberOfScreens is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	}
#else
void CT_DataWsSession::DoCmdNumberOfScreens(const TDesC& /*aSection*/)
	{
	ERR_PRINTF1(_L("SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS Not Supported"));
	SetBlockResult(EFail);
	}
#endif

void CT_DataWsSession::DoCmdSimulateRawEvent(const TDesC& aSection)
	{
	TBool		dataOk=ETrue;
	TRawEvent	event;

	TRawEvent::TType	type=TRawEvent::ENone;
	if ( !CT_GraphicsUtil::ReadType(*this, aSection, KFldType(), type) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldType());
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
		INFO_PRINTF1(_L("RWsSession::SimulateRawEvent(TRawEvent) call"));
		iWsSession->SimulateRawEvent(event);
		}
	}

void CT_DataWsSession::DoCmdSimulateKeyEvent(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TKeyCode	keyCode;
	if ( !CT_GraphicsUtil::ReadKeyCode(*this, aSection, KFldKeyCode(), keyCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldKeyCode());
		SetBlockResult(EFail);
		}

	TUint	modifiers=0;
	CT_GraphicsUtil::ReadEventModifier(*this, aSection, KFldModifiers(), modifiers);

	TInt	repeats=0;
	GetIntFromConfig(aSection, KFldRepeats(), repeats);

	if ( dataOk )
		{
		TKeyEvent	event;

		event.iCode=keyCode;
		event.iScanCode=keyCode;
		event.iModifiers=modifiers;
		event.iRepeats=repeats;
		INFO_PRINTF1(_L("RWsSession::SimulateKeyEvent(TKeyEvent) call"));
		iWsSession->SimulateKeyEvent(event);
		}
	}

/**
 Virtual RunL - Called on completion of an asynchronous command
 @see MTPActiveCallback
 @param aActive Active Object that RunL has been called on
 @pre N/A
 @post N/A
 @leave system wide error code
*/
void CT_DataWsSession::RunL(CActive* aActive, const TInt aIndex)
	{
	if ( aActive==iActiveEventReady )
		{
		RunEventReady(aIndex);
		}
	else if ( aActive==iActivePriorityKeyReady )
		{
		RunPriorityKeyReady(aIndex);
		}
	else if ( aActive==iActiveRedrawReady )
		{
		RunRedrawReady(aIndex);
		}
	else
		{
		ERR_PRINTF1(_L("Stray signal"));
		SetBlockResult(EFail);
		}
	DecOutstanding();
	}

/**
 Virtual DoCancel - Request to cancel the asynchronous command
 @see - MTPActiveCallback
 @param aActive Active Object that DoCancel has been called on
 @pre - N/A
 @post - N/A
 @leave system wide error code
*/
void CT_DataWsSession::DoCancel(CActive* aActive, const TInt aIndex)
	{
	if ( aActive==iActiveEventReady )
		{
		DoCancelEventReady(aIndex);
		}
	else if ( aActive==iActivePriorityKeyReady )
		{
		DoCancelPriorityKeyReady(aIndex);
		}
	else if ( aActive==iActiveRedrawReady )
		{
		DoCancelRedrawReady(aIndex);
		}
	else
		{
		ERR_PRINTF1(_L("Stray signal"));
		SetBlockResult(EFail);
		}
	DecOutstanding();
	}

void CT_DataWsSession::RunEventReady(const TInt aIndex)
	{
	TInt	err=iActiveEventReady->iStatus.Int();
	INFO_PRINTF2(_L("RunEventReady %d"), err);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("RunL Error %d"), err);
		SetAsyncError(aIndex, err);
		}
	}

void CT_DataWsSession::DoCancelEventReady(const TInt /*aIndex*/)
	{
	}

void CT_DataWsSession::RunPriorityKeyReady(const TInt aIndex)
	{
	TInt	err=iActivePriorityKeyReady->iStatus.Int();
	INFO_PRINTF2(_L("RunPriorityKeyReady %d"), err);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("RunL Error %d"), err);
		SetAsyncError(aIndex, err);
		}
	}

void CT_DataWsSession::DoCancelPriorityKeyReady(const TInt /*aIndex*/)
	{
	}

void CT_DataWsSession::RunRedrawReady(const TInt aIndex)
	{
	TInt	err=iActiveRedrawReady->iStatus.Int();
	INFO_PRINTF2(_L("RunRedrawReady %d"), err);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("RunL Error %d"), err);
		SetAsyncError(aIndex, err);
		}
	}

void CT_DataWsSession::DoCancelRedrawReady(const TInt /*aIndex*/)
	{
	}
