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

#include "T_DataWindowBase.h"
#include "T_GraphicsUtil.h"


/*@{*/
//Commands
_LIT(KCmdActivate,							"Activate");
_LIT(KCmdSetPosition,						"SetPosition");
_LIT(KCmdSetSizeErr,						"SetSizeErr");
_LIT(KCmdSetExtentErr,						"SetExtentErr");
_LIT(KCmdSize,								"Size");
_LIT(KCmdInquireOffset,						"InquireOffset");
_LIT(KCmdPointerFilter,						"PointerFilter");
_LIT(KCmdSetPointerGrab,					"SetPointerGrab");
_LIT(KCmdClaimPointerGrab,					"ClaimPointerGrab");
_LIT(KCmdSetPointerCapture,					"SetPointerCapture");
_LIT(KCmdSetPointerCapturePriority,			"SetPointerCapturePriority");
_LIT(KCmdGetPointerCapturePriority,			"GetPointerCapturePriority");
_LIT(KCmdSetVisible,						"SetVisible");
_LIT(KCmdSetShadowHeight,					"SetShadowHeight");
_LIT(KCmdSetShadowDisabled,					"SetShadowDisabled");
_LIT(KCmdPosition,							"Position");
_LIT(KCmdAbsPosition,						"AbsPosition");
_LIT(KCmdSetCornerType,						"SetCornerType");
_LIT(KCmdSetShape,							"SetShape");
_LIT(KCmdSetRequiredDisplayMode,			"SetRequiredDisplayMode");
_LIT(KCmdDisplayMode,						"DisplayMode");
_LIT(KCmdEnableBackup,						"EnableBackup");
_LIT(KCmdRequestPointerRepeatEvent,			"RequestPointerRepeatEvent");
_LIT(KCmdCancelPointerRepeatEventRequest,	"CancelPointerRepeatEventRequest");
_LIT(KCmdAllocPointerMoveBuffer,			"AllocPointerMoveBuffer");
_LIT(KCmdFreePointerMoveBuffer,				"FreePointerMoveBuffer");
_LIT(KCmdEnablePointerMoveBuffer,			"EnablePointerMoveBuffer");
_LIT(KCmdDisablePointerMoveBuffer,			"DisablePointerMoveBuffer");
_LIT(KCmdRetrievePointerMoveBuffer,			"RetrievePointerMoveBuffer");
_LIT(KCmdDiscardPointerMoveBuffer,			"DiscardPointerMoveBuffer");
_LIT(KCmdAddKeyRect,						"AddKeyRect");
_LIT(KCmdRemoveAllKeyRects,					"RemoveAllKeyRects");
_LIT(KCmdPasswordWindow,					"PasswordWindow");
_LIT(KCmdFadeBehind,						"FadeBehind");
_LIT(KCmdIsFaded,							"IsFaded");
_LIT(KCmdIsNonFading,						"IsNonFading");
_LIT(KCmdMoveToGroup,						"MoveToGroup");

// Fields
_LIT(KFldActivatedByPointerSwitchOn,		"activatedbypointerswitchon");
_LIT(KFldBackupType,						"backuptype");
_LIT(KFldCornerFlags,						"cornerflags");
_LIT(KFldCornerType,						"cornertype");
_LIT(KFldExpected,							"expected");
_LIT(KFldExpectedNumOfPoints,				"expected_numofpoints");
_LIT(KFldFade,								"fade");
_LIT(KFldFlags,								"flags");
_LIT(KFldHeight,							"height");
_LIT(KFldIdentifier,						"identifier");
_LIT(KFldIdentifier_Name,					"identifier_name");
_LIT(KFldMaxPoints,							"maxpoints");
_LIT(KFldMode,								"mode");
_LIT(KFldPasswordMode,						"passwordmode");
_LIT(KFldPoint,								"point");
_LIT(KFldPriority,							"priority");
_LIT(KFldRect,								"rect");
_LIT(KFldRegion,							"region");
_LIT(KFldScanCode,							"scancode");
_LIT(KFldSendUpEvent,						"sendupevent");
_LIT(KFldSize,								"size");
_LIT(KFldState,								"state");
_LIT(KFldTime,								"time");
_LIT(KFldWindow,							"window");
_LIT(KFldFilterMask,	 					"filterMask");
_LIT(KFldFilter,			 				"filter");

///	Logging
_LIT(KLogError,								"Error=%d");
_LIT(KLogMissingParameter,					"Missing parameter '%S'");
_LIT(KLogMissingExpectedValue,				"Missing expected value '%S'");
_LIT(KLogNotAsExpectedValue,				"'%S' is not as expected value");
_LIT(KLogNotExpectedValueInt,				"'%S' is not as expected=%d, actual=%d");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWindowBase::CT_DataWindowBase()
:	CT_DataWindowTreeNode()
	{
	}

RWindowTreeNode* CT_DataWindowBase::GetWindowTreeNode() const
	{
	return GetWindowBase();
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
TBool CT_DataWindowBase::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdActivate )
		{
		DoCmdActivate();
		}
	else if ( aCommand==KCmdSetPosition )
		{
		DoCmdSetPosition(aSection);
		}
	else if ( aCommand==KCmdSetSizeErr )
		{
		DoCmdSetSizeErr(aSection);
		}
	else if ( aCommand==KCmdSetExtentErr )
		{
		DoCmdSetExtentErr(aSection);
		}
	else if ( aCommand==KCmdSize )
		{
		DoCmdSize(aSection);
		}
	else if ( aCommand==KCmdInquireOffset )
		{
		DoCmdInquireOffsetL(aSection);
		}
	else if ( aCommand==KCmdPointerFilter )
		{
		DoCmdPointerFilter(aSection);
		}
	else if ( aCommand==KCmdSetPointerGrab )
		{
		DoCmdSetPointerGrab(aSection);
		}
	else if ( aCommand==KCmdClaimPointerGrab )
		{
		DoCmdClaimPointerGrab(aSection);
		}
	else if ( aCommand==KCmdSetPointerCapture )
		{
		DoCmdSetPointerCapture(aSection);
		}
	else if ( aCommand==KCmdSetPointerCapturePriority )
		{
		DoCmdSetPointerCapturePriority(aSection);
		}
	else if ( aCommand==KCmdGetPointerCapturePriority )
		{
		DoCmdGetPointerCapturePriority(aSection);
		}
	else if ( aCommand==KCmdSetVisible )
		{
		DoCmdSetVisible(aSection);
		}
	else if ( aCommand==KCmdSetShadowHeight )
		{
		DoCmdSetShadowHeight(aSection);
		}
	else if ( aCommand==KCmdSetShadowDisabled )
		{
		DoCmdSetShadowDisabled(aSection);
		}
	else if ( aCommand==KCmdPosition )
		{
		DoCmdPosition(aSection);
		}
	else if ( aCommand==KCmdAbsPosition )
		{
		DoCmdAbsPosition(aSection);
		}
	else if ( aCommand==KCmdSetCornerType )
		{
		DoCmdSetCornerType(aSection);
		}
	else if ( aCommand==KCmdSetShape )
		{
		DoCmdSetShapeL(aSection);
		}
	else if ( aCommand==KCmdSetRequiredDisplayMode )
		{
		DoCmdSetRequiredDisplayMode(aSection);
		}
	else if ( aCommand==KCmdDisplayMode )
		{
		DoCmdDisplayMode(aSection);
		}
	else if ( aCommand==KCmdEnableBackup )
		{
		DoCmdEnableBackup(aSection);
		}
	else if ( aCommand==KCmdRequestPointerRepeatEvent )
		{
		DoCmdRequestPointerRepeatEvent(aSection);
		}
	else if ( aCommand==KCmdCancelPointerRepeatEventRequest )
		{
		DoCmdCancelPointerRepeatEventRequest();
		}
	else if ( aCommand==KCmdAllocPointerMoveBuffer )
		{
		DoCmdAllocPointerMoveBuffer(aSection);
		}
	else if ( aCommand==KCmdFreePointerMoveBuffer )
		{
		DoCmdFreePointerMoveBuffer();
		}
	else if ( aCommand==KCmdEnablePointerMoveBuffer )
		{
		DoCmdEnablePointerMoveBuffer();
		}
	else if ( aCommand==KCmdDisablePointerMoveBuffer )
		{
		DoCmdDisablePointerMoveBuffer();
		}
	else if ( aCommand==KCmdRetrievePointerMoveBuffer )
		{
		DoCmdRetrievePointerMoveBufferL(aSection);
		}
	else if ( aCommand==KCmdDiscardPointerMoveBuffer )
		{
		DoCmdDiscardPointerMoveBuffer();
		}
	else if ( aCommand==KCmdAddKeyRect )
		{
		DoCmdAddKeyRect(aSection);
		}
	else if ( aCommand==KCmdRemoveAllKeyRects )
		{
		DoCmdRemoveAllKeyRects();
		}
	else if ( aCommand==KCmdPasswordWindow )
		{
		DoCmdPasswordWindow(aSection);
		}
	else if ( aCommand==KCmdFadeBehind )
		{
		DoCmdFadeBehind(aSection);
		}
	else if ( aCommand==KCmdIsFaded )
		{
		DoCmdIsFaded(aSection);
		}
	else if ( aCommand==KCmdIsNonFading )
		{
		DoCmdIsNonFading(aSection);
		}
	else if ( aCommand==KCmdMoveToGroup )
		{
		DoCmdMoveToGroupL(aSection);
		}
	else
		{
		ret=CT_DataWindowTreeNode::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataWindowBase::DoCmdActivate()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Activate()"));
	GetWindowBase()->Activate();

	// No command return value and output parameter to display and check
	}


void CT_DataWindowBase::DoCmdSetPosition(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	datPoint;

	if ( !GetPointFromConfig(aSection, KFldPoint, datPoint) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPosition(TPoint)"));
		GetWindowBase()->SetPosition(datPoint);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSetSizeErr(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TSize	datSize;

	if ( !GetSizeFromConfig(aSection, KFldSize, datSize) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSize);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetSizeErr(TSize)"));
		TInt	returnCode = GetWindowBase()->SetSizeErr(datSize);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSetExtentErr(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPoint	datPoint;
	if ( !GetPointFromConfig(aSection, KFldPoint, datPoint) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint);
		SetBlockResult(EFail);
		}

	TSize	datSize;
	if ( !GetSizeFromConfig(aSection, KFldSize, datSize) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSize);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetExtentErr(TPoint, TSize)"));
		TInt	returnCode = GetWindowBase()->SetExtentErr(datPoint, datSize);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSize(const TDesC& aSection)
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Size()"));
	TSize	actualResult = GetWindowBase()->Size();

	// Diaplay command return value, check if it matches the expected value
	TSize	expectResult;

	if ( !GetSizeFromConfig(aSection, KFldExpected, expectResult) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	else if ( actualResult!=expectResult )
		{
		ERR_PRINTF2(KLogNotAsExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowBase::DoCmdInquireOffsetL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPtrC	windowName;
	RWindowTreeNode*	window=NULL;
	if ( GetStringFromConfig(aSection, KFldWindow, windowName) )
		{
		window = static_cast<RWindowTreeNode*>(GetDataObjectL(windowName));
		}

	if ( window==NULL )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldWindow);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute InquireOffset(RWindowTreeNode)"));
		TPoint	actualResult = GetWindowBase()->InquireOffset(*window);

		// Diaplay command return value, check if it matches the expected value
		TPoint	expectResult;

		if ( !GetPointFromConfig(aSection, KFldExpected, expectResult)	)
			{
			ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
			SetBlockResult(EFail);
			}
		else if ( actualResult!=expectResult )
			{
			ERR_PRINTF2(KLogNotAsExpectedValue, &KFldExpected);
			SetBlockResult(EFail);
			}
		}
	}


void CT_DataWindowBase::DoCmdPointerFilter(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	TUint32	eventFilterMask=0;
	if ( !CT_GraphicsUtil::ReadPointerFilter(*this, aSection, KFldFilterMask, (TUint&)eventFilterMask) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFilterMask);
		SetBlockResult(EFail);
		}

	TUint32	eventFilter=0;
	if ( !CT_GraphicsUtil::ReadPointerFilter(*this, aSection, KFldFilter, (TUint&)eventFilter) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFilter);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		INFO_PRINTF3(_L("execute PointerFilter(0x%x, 0x%x)"), eventFilterMask, eventFilter);
		GetWindowBase()->PointerFilter(eventFilterMask, eventFilter);
		}
	}


void CT_DataWindowBase::DoCmdSetPointerGrab(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datState = EFalse;

	if ( !GetBoolFromConfig(aSection, KFldState, datState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldState);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPointerGrab(TBool)"));
		GetWindowBase()->SetPointerGrab(datState);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdClaimPointerGrab(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datSendUpEvent = EFalse;

	if ( !GetBoolFromConfig(aSection, KFldSendUpEvent, datSendUpEvent) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSendUpEvent);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute ClaimPointerGrab(TBool)"));
		GetWindowBase()->ClaimPointerGrab(datSendUpEvent);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSetPointerCapture(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	RWindowBase::TCaptureFlags	datFlags;

	if ( !CT_GraphicsUtil::ReadCaptureFlags(*this, aSection, KFldFlags, (TUint&)datFlags) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFlags);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPointerCapture(TInt)"));
		GetWindowBase()->SetPointerCapture(datFlags);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSetPointerCapturePriority(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	datPriority;

	if ( !GetIntFromConfig(aSection, KFldPriority, datPriority) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPriority);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetPointerCapturePriority(TInt)"));
		GetWindowBase()->SetPointerCapturePriority(datPriority);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdGetPointerCapturePriority(const TDesC& aSection)
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute GetPointerCapturePriority()"));
	TInt	actualResult = GetWindowBase()->GetPointerCapturePriority();

	// Diaplay command return value, check if it matches the expected value
	TInt	expectResult;
	if ( !GetIntFromConfig(aSection, KFldExpected, expectResult) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	else if ( actualResult!=expectResult )
		{
		ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expectResult, actualResult);
		SetBlockResult(EFail);
		}

	// No command output parameter to display and check
	}


void CT_DataWindowBase::DoCmdSetVisible(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datState = EFalse;

	if ( !GetBoolFromConfig(aSection, KFldState, datState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldState);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetVisible(TBool)"));
		GetWindowBase()->SetVisible(datState);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSetShadowHeight(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TInt	datHeight;

	if ( !GetIntFromConfig(aSection, KFldHeight, datHeight) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldHeight);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetShadowHeight(TInt)"));
		GetWindowBase()->SetShadowHeight(datHeight);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdSetShadowDisabled(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datState = EFalse;

	if ( !GetBoolFromConfig(aSection, KFldState, datState) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldState);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetShadowDisabled(TBool)"));
		GetWindowBase()->SetShadowDisabled(datState);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdPosition(const TDesC& aSection)
	{
	TPoint	actual=GetWindowBase()->Position();
	INFO_PRINTF3(_L("execute Position()=(%d,%d)"), actual.iX, actual.iY);

	// Diaplay command return value, check if it matches the expected value
	TPoint	expected;
	if ( !GetPointFromConfig(aSection, KFldExpected, expected)	)
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


void CT_DataWindowBase::DoCmdAbsPosition(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute AbsPosition()"));
	TPoint	actual=GetWindowBase()->AbsPosition();

	// Diaplay command return value, check if it matches the expected value
	TPoint	expected;
	if ( !GetPointFromConfig(aSection, KFldExpected, expected)	)
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


void CT_DataWindowBase::DoCmdSetCornerType(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TCornerType datCornerType;
	if ( !CT_GraphicsUtil::ReadCornerType(*this, aSection, KFldCornerType, datCornerType) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldCornerType);
		SetBlockResult(EFail);
		}

	TInt	datCornerFlags;
	if ( !GetHexFromConfig(aSection, KFldCornerFlags, datCornerFlags) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldCornerFlags);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetCornerType(TCornerType, TInt)"));
		GetWindowBase()->SetCornerType(datCornerType, datCornerFlags);
		}
	}


void CT_DataWindowBase::DoCmdSetShapeL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	RRegion	region(1);
	CleanupClosePushL(region);

	if ( !GetRegionFromConfig(aSection, KFldRegion, region) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRegion);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetShape(TRegion)"));
		TInt	err=GetWindowBase()->SetShape(region);

		// Check the command return code, if !=KErrNone then stop this command
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}

		// No command output parameter to display and check
		}
	CleanupStack::PopAndDestroy(&region);
	}


void CT_DataWindowBase::DoCmdSetRequiredDisplayMode(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TDisplayMode	datMode;

	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldMode, datMode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldMode);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		TInt			actual=GetWindowBase()->SetRequiredDisplayMode(datMode);
		INFO_PRINTF2(_L("execute SetRequiredDisplayMode(TDisplayMode)=%d"), actual);

		// Diaplay command return value, check if it matches the expected value
		TDisplayMode	expected;
		if ( CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldExpected, expected) )
			{
			if ( actual!=expected )
				{
				ERR_PRINTF2(KLogNotAsExpectedValue, &KFldExpected);
				SetBlockResult(EFail);
				}
			}
		}
	}

void CT_DataWindowBase::DoCmdDisplayMode(const TDesC& aSection)
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute DisplayMode()"));
	TDisplayMode	actualResult = GetWindowBase()->DisplayMode();

	// Diaplay command return value, check if it matches the expected value
	TDisplayMode	expectResult;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldExpected, expectResult) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	else if ( actualResult!=expectResult )
		{
		ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expectResult, actualResult);
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowBase::DoCmdEnableBackup(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TWindowBackupType	datBackupType;

	if ( !CT_GraphicsUtil::ReadWindowBackupType(*this, aSection, KFldBackupType, (TUint&)datBackupType) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBackupType);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute EnableBackup(TUint)"));
		GetWindowBase()->EnableBackup(datBackupType);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdRequestPointerRepeatEvent(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TInt	datTime=0;
	if ( !GetIntFromConfig(aSection, KFldTime, datTime) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldTime);
		SetBlockResult(EFail);
		}

	TRect	datRect;
	if ( !GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute RequestPointerRepeatEvent(TTimeIntervalMicroSeconds32, TRect)"));
		GetWindowBase()->RequestPointerRepeatEvent((TTimeIntervalMicroSeconds32)datTime, datRect);

		// No command return value and output parameter to display and check
		}
	}

void CT_DataWindowBase::DoCmdCancelPointerRepeatEventRequest()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute CancelPointerRepeatEventRequest()"));
	GetWindowBase()->CancelPointerRepeatEventRequest();

	// No command return value and output parameter to display and check
	}


void CT_DataWindowBase::DoCmdAllocPointerMoveBuffer(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TInt	datMaxPoints;
	if ( !GetIntFromConfig(aSection, KFldMaxPoints, datMaxPoints) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldMaxPoints);
		SetBlockResult(EFail);
		}

	TInt	datFlags;
	if ( !GetIntFromConfig(aSection, KFldFlags, datFlags) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFlags);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute AllocPointerMoveBuffer(TInt, TUint)"));
		TInt	returnCode = GetWindowBase()->AllocPointerMoveBuffer(datMaxPoints, datFlags);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdFreePointerMoveBuffer()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute FreePointerMoveBuffer()"));
	GetWindowBase()->FreePointerMoveBuffer();

	// No command return value and output parameter to display and check
	}


void CT_DataWindowBase::DoCmdEnablePointerMoveBuffer()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute EnablePointerMoveBuffer()"));
	GetWindowBase()->EnablePointerMoveBuffer();

	// No command return value and output parameter to display and check
	}


void CT_DataWindowBase::DoCmdDisablePointerMoveBuffer()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute DisablePointerMoveBuffer()"));
	GetWindowBase()->DisablePointerMoveBuffer();

	// No command return value and output parameter to display and check
	}

void CT_DataWindowBase::DoCmdRetrievePointerMoveBufferL(const TDesC& aSection)
	{
	const TInt KPointerMoveBufferSize = 50;
	TInt	maxPoints=KPointerMoveBufferSize;
	GetIntFromConfig(aSection, KFldMaxPoints, maxPoints);

	TPoint*	pnts = new (ELeave) TPoint[maxPoints];
	CleanupArrayDeletePushL(pnts);
	TPtr8	ptr((TUint8 *)pnts,sizeof(TPoint)*maxPoints);

	INFO_PRINTF1(_L("execute RetrievePointerMoveBuffer(TDes8&)"));
	TInt err=GetWindowBase()->RetrievePointerMoveBuffer(ptr);

	if ( err<KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	else
		{
		TInt	numberOfPointsInBuffer = ptr.Length() / sizeof(TPoint);
		TInt	expectedNumOfPoints=0;

		for (TInt i=0; ; i++)
			{
			TBuf<KMaxTestExecuteCommandLength>	tempStore;
			tempStore.Format(_L("%S%d"), &KFldExpected, i+1);

			TPoint	expectedPoint;
			if ( GetPointFromConfig(aSection, tempStore, expectedPoint) )
				{
				expectedNumOfPoints++;
				if ( i<numberOfPointsInBuffer )
					{
					if ( pnts[i]!=expectedPoint )
						{
						ERR_PRINTF2(KLogNotAsExpectedValue, &tempStore);
						ERR_PRINTF3(_L("%d%d"), pnts[i].iX, pnts[i].iY);
						SetBlockResult(EFail);
						}
					}
				}
			else
				{
				break;
				}
			}

		if ( numberOfPointsInBuffer!=expectedNumOfPoints )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpectedNumOfPoints, expectedNumOfPoints, numberOfPointsInBuffer);
			SetBlockResult(EFail);
			}
		}

	CleanupStack::PopAndDestroy(pnts);
	}

void CT_DataWindowBase::DoCmdDiscardPointerMoveBuffer()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute DiscardPointerMoveBuffer()"));
	GetWindowBase()->DiscardPointerMoveBuffer();

	// No command return value and output parameter to display and check
	}


void CT_DataWindowBase::DoCmdAddKeyRect(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TRect	datRect;
	if ( !GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldRect);
		SetBlockResult(EFail);
		}

	TInt	datScanCode;
	if ( !CT_GraphicsUtil::ReadStdScanCode(*this, aSection, KFldScanCode, (TStdScanCode&)datScanCode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldScanCode);
		SetBlockResult(EFail);
		}

	TBool	datActivatedByPointerSwitchOn;
	if ( !GetBoolFromConfig(aSection, KFldActivatedByPointerSwitchOn, datActivatedByPointerSwitchOn) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldActivatedByPointerSwitchOn);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute AddKeyRect(TRect, TInt, TBool)"));
		TInt	returnCode = GetWindowBase()->AddKeyRect(datRect, datScanCode, datActivatedByPointerSwitchOn);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdRemoveAllKeyRects()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute RemoveAllKeyRects()"));
	GetWindowBase()->RemoveAllKeyRects();

	// No command return value and output parameter to display and check
	}


void CT_DataWindowBase::DoCmdPasswordWindow(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPasswordMode	datPasswordMode;

	if ( !CT_GraphicsUtil::ReadPasswordMode(*this, aSection, KFldPasswordMode, datPasswordMode) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPasswordMode);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute PasswordWindow(TPasswordMode)"));
		TInt	returnCode = GetWindowBase()->PasswordWindow(datPasswordMode);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdFadeBehind(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datFade;

	if ( !GetBoolFromConfig(aSection, KFldFade, datFade) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFade);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute FadeBehind(TBool)"));
		GetWindowBase()->FadeBehind(datFade);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowBase::DoCmdIsFaded(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute IsFaded()"));
	TBool	actualResult = GetWindowBase()->IsFaded();

	// Diaplay command return value, check if it matches the expected value
	TBool	expectResult;
	if ( !GetBoolFromConfig(aSection, KFldExpected, expectResult) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		if ( (actualResult!=0) != expectResult )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expectResult, actualResult);
			SetBlockResult(EFail);
			}
		}
	}


void CT_DataWindowBase::DoCmdIsNonFading(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute IsNonFading()"));
	TBool	actualResult = GetWindowBase()->IsNonFading();

	// Diaplay command return value, check if it matches the expected value
	TBool	expectResult;
	if ( !GetBoolFromConfig(aSection, KFldExpected, expectResult) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		if ( (actualResult!=0) != expectResult )
			{
			ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expectResult, actualResult);
			SetBlockResult(EFail);
			}
		}
	}


void CT_DataWindowBase::DoCmdMoveToGroupL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TInt	datIdentifier;
	if ( !GetIntFromConfig(aSection, KFldIdentifier, datIdentifier) )
		{
		TPtrC	identifierName;
		RWindowGroup*	identifier = NULL;
		if ( GetStringFromConfig(aSection, KFldIdentifier_Name, identifierName) )
			{
			identifier = static_cast<RWindowGroup*>(GetDataObjectL(identifierName));
			}

		if ( identifier==NULL )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldIdentifier);
			SetBlockResult(EFail);
			}
		else
			{
			datIdentifier = identifier->Identifier();
			INFO_PRINTF3(_L("get '%S_name' identifier = %d to MoveToGroup"), &KFldIdentifier, datIdentifier);
			}
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute MoveToGroup(TInt)"));
		TInt	returnCode = GetWindowBase()->MoveToGroup(datIdentifier);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}
