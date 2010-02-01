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

#include "T_DataWindowTreeNode.h"


//Parameters
_LIT(KObjectName,							"objectName");
_LIT(KExpected,								"expected");
_LIT(KInputWindowPosition,					"inputWindowPosition");
_LIT(KInputWindowPriority,					"inputWindowPriority");
_LIT(KInputCursorNumber,					"inputCursorNumber");
_LIT(KInputEventControl,					"inputEventControl");
_LIT(KInputEventModifier,					"inputEventModifier");
_LIT(KInputNonFading,						"inputNonFading");
_LIT(KInputFaded,							"inputFaded");
_LIT(KInputFadeControl,						"inputFadeControl");
_LIT(KInputBlackMap,						"inputBlackMap");
_LIT(KInputWhiteMap,						"inputWhiteMap");

_LIT(KFldPointerCursor,                     "pointerCursor");
//Commands
_LIT(KCmdClose,                         	"Close");
_LIT(KCmdDestroy,                       	"Destroy");
_LIT(KCmdParent,                        	"Parent");
_LIT(KCmdPrevSibling,                   	"PrevSibling");
_LIT(KCmdNextSibling,                   	"NextSibling");
_LIT(KCmdChild,                         	"Child");
_LIT(KCmdOrdinalPriority,               	"OrdinalPriority");
_LIT(KCmdOrdinalPosition,               	"OrdinalPosition");
_LIT(KCmdFullOrdinalPosition,           	"FullOrdinalPosition");
_LIT(KCmdSetOrdinalPosition,            	"SetOrdinalPosition");
_LIT(KCmdWindowGroupId,                 	"WindowGroupId");
_LIT(KCmdSetPointerCursor,              	"SetPointerCursor");
_LIT(KCmdSetCustomPointerCursor,        	"SetCustomPointerCursor");
_LIT(KCmdEnableOnEvents,                	"EnableOnEvents");
_LIT(KCmdDisableOnEvents,               	"DisableOnEvents");
_LIT(KCmdEnableGroupChangeEvents,       	"EnableGroupChangeEvents");
_LIT(KCmdDisableGroupChangeEvents,      	"DisableGroupChangeEvents");
_LIT(KCmdEnableFocusChangeEvents,       	"EnableFocusChangeEvents");
_LIT(KCmdDisableFocusChangeEvents,      	"DisableFocusChangeEvents");
_LIT(KCmdEnableGroupListChangeEvents,   	"EnableGroupListChangeEvents");
_LIT(KCmdDisableGroupListChangeEvents,  	"DisableGroupListChangeEvents");
_LIT(KCmdEnableVisibilityChangeEvents,  	"EnableVisibilityChangeEvents");
_LIT(KCmdDisableVisibilityChangeEvents, 	"DisableVisibilityChangeEvents");
_LIT(KCmdEnableErrorMessages,           	"EnableErrorMessages");
_LIT(KCmdDisableErrorMessages,          	"DisableErrorMessages");
_LIT(KCmdEnableModifierChangedEvents,   	"EnableModifierChangedEvents");
_LIT(KCmdDisableModifierChangedEvents,  	"DisableModifierChangedEvents");
_LIT(KCmdSetNonFading,                  	"SetNonFading");
_LIT(KCmdSetFaded,                      	"SetFaded");
_LIT(KCmdClearPointerCursor,            	"ClearPointerCursor");

/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWindowTreeNode::CT_DataWindowTreeNode()
:    CDataWrapperBase()
	{
	}

MWsClientClass* CT_DataWindowTreeNode::GetClientClass() const
	{
	return GetWindowTreeNode();
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
TBool CT_DataWindowTreeNode::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool    ret = ETrue;

	if ( aCommand==KCmdClose )
		{
		DoCmdClose();
		}
	else if ( aCommand==KCmdDestroy )
		{
		DoCmdDestroyL();
		}
	else if ( aCommand==KCmdParent )
		{
		DoCmdParent(aSection);
		}
	else if ( aCommand==KCmdPrevSibling )
		{
		DoCmdPrevSibling(aSection);
		}
	else if ( aCommand==KCmdNextSibling )
		{
		DoCmdNextSibling(aSection);
		}
	else if ( aCommand==KCmdChild )
		{
		DoCmdChild(aSection);
		}
	else if ( aCommand==KCmdOrdinalPriority )
		{
		DoCmdOrdinalPriority(aSection);
		}
	else if ( aCommand==KCmdOrdinalPosition )
		{
		DoCmdOrdinalPosition(aSection);
		}
	else if ( aCommand==KCmdFullOrdinalPosition )
		{
		DoCmdFullOrdinalPosition(aSection);
		}
	else if ( aCommand==KCmdSetOrdinalPosition )
		{
		DoCmdSetOrdinalPosition(aSection);
		}
	else if ( aCommand==KCmdWindowGroupId )
		{
		DoCmdWindowGroupId();
		}
	else if ( aCommand==KCmdSetPointerCursor )
		{
		DoCmdSetPointerCursor(aSection);
		}
	else if (	aCommand==KCmdSetCustomPointerCursor )
		{
		DoCmdSetCustomPointerCursorL(aSection);
		}
	else if ( aCommand==KCmdEnableOnEvents )
		{
		DoCmdEnableOnEvents(aSection);
		}
	else if ( aCommand==KCmdDisableOnEvents )
		{
		DoCmdDisableOnEvents();
		}
	else if ( aCommand==KCmdEnableGroupChangeEvents )
		{
		DoCmdEnableGroupChangeEvents();
		}
	else if ( aCommand==KCmdDisableGroupChangeEvents )
		{
		DoCmdDisableGroupChangeEvents();
		}
	else if ( aCommand==KCmdEnableFocusChangeEvents )
		{
		DoCmdEnableFocusChangeEvents();
		}
	else if ( aCommand==KCmdDisableFocusChangeEvents )
		{
		DoCmdDisableFocusChangeEvents();
		}
	else if ( aCommand==KCmdEnableGroupListChangeEvents )
		{
		DoCmdEnableGroupListChangeEvents();
		}
	else if ( aCommand==KCmdDisableGroupListChangeEvents )
		{
		DoCmdDisableGroupListChangeEvents();
		}
	else if ( aCommand==KCmdEnableVisibilityChangeEvents )
		{
		DoCmdEnableVisibilityChangeEvents();
		}
	else if ( aCommand==KCmdDisableVisibilityChangeEvents )
		{
		DoCmdDisableVisibilityChangeEvents();
		}
	else if	( aCommand==KCmdEnableErrorMessages	)
		{
		DoCmdEnableErrorMessages(aSection);
		}
	else if ( aCommand==KCmdDisableErrorMessages )
		{
		DoCmdDisableErrorMessages();
		}
	else if ( aCommand==KCmdEnableModifierChangedEvents )
		{
		DoCmdEnableModifierChangedEvents(aSection);
		}
	else if ( aCommand==KCmdDisableModifierChangedEvents )
		{
		DoCmdDisableModifierChangedEvents();
		}
	else if ( aCommand==KCmdSetNonFading )
		{
		DoCmdSetNonFading(aSection);
		}
	else if ( aCommand==KCmdSetFaded )
		{
		DoCmdSetFaded(aSection);
		}
	else if ( aCommand==KCmdClearPointerCursor )
		{
		DoCmdClearPointerCursor();
		}
	else
		{
		ret=CT_DataWsClientClass::DoCommandL(*this, aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataWindowTreeNode::DoCmdClose()
	{
	GetWindowTreeNode()->Close();
	INFO_PRINTF1(_L("RWindowTreeNode::Close"));
	}

void CT_DataWindowTreeNode::DoCmdDestroyL()
	{
	RWindowTreeNode*	node=GetWindowTreeNode();

	//	We disown the object as Destroy() also deletes it
	DisownObjectL();
	INFO_PRINTF1(_L("RWindowTreeNode::Destroy"));
	node->Destroy();
	}

void CT_DataWindowTreeNode::DoCmdParent(const TDesC& aSection)
	{
	TUint32	actual = GetWindowTreeNode()->Parent();
	INFO_PRINTF2(_L("RWindowTreeNode::Parent = %d"), actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if ( actual!=(TUint32)expected )
			{
			ERR_PRINTF1(_L("Parent is	not	as expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdPrevSibling(const TDesC& aSection)
	{
	TUint32	actual = GetWindowTreeNode()->PrevSibling();
	INFO_PRINTF2(_L("RWindowTreeNode::PrevSibling	= %d"),	actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if ( actual!=(TUint32)expected )
			{
			ERR_PRINTF1(_L("PrevSibling is not as	expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdNextSibling(const TDesC& aSection)
	{
	TUint32	actual = GetWindowTreeNode()->NextSibling();
	INFO_PRINTF2(_L("RWindowTreeNode::NextSibling	= %d"),	actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if ( actual!=(TUint32)expected )
			{
			ERR_PRINTF1(_L("NextSibling is not as	expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdChild(const TDesC& aSection)
	{
	TUint32	actual = GetWindowTreeNode()->Child();
	INFO_PRINTF2(_L("RWindowTreeNode::Child =	%d"), actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if ( actual!=(TUint32)expected )
			{
			ERR_PRINTF1(_L("Child	is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdOrdinalPriority(const TDesC& aSection)
	{
	TInt		actual = GetWindowTreeNode()->OrdinalPriority();
	INFO_PRINTF2(_L("RWindowTreeNode::OrdinalPriority	= %d"),	actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if(	actual != expected )
			{
			ERR_PRINTF1(_L("OrdinalPriority is not as	expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdOrdinalPosition(const TDesC& aSection)
	{
	TInt		actual = GetWindowTreeNode()->OrdinalPosition();
	INFO_PRINTF2(_L("RWindowTreeNode::OrdinalPosition	= %d"),	actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if(	actual != expected )
			{
			ERR_PRINTF1(_L("OrdinalPosition is not as	expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdFullOrdinalPosition(const TDesC& aSection)
	{
	TInt		actual = GetWindowTreeNode()->FullOrdinalPosition();
	INFO_PRINTF2(_L("RWindowTreeNode::FullOrdinalPosition	= %d"),	actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if(	actual != expected )
			{
			ERR_PRINTF1(_L("FullOrdinalPosition is not as	expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KExpected());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdSetOrdinalPosition(const TDesC& aSection)
	{
	TInt	inputWindowPosition=0;
	if(	GetIntFromConfig(aSection, KInputWindowPosition(), inputWindowPosition))
		{
		TInt	inputWindowPriority=0;
		if(	GetIntFromConfig(aSection, KInputWindowPriority(), inputWindowPriority))
			{
			GetWindowTreeNode()->SetOrdinalPosition(inputWindowPosition, inputWindowPriority);
			INFO_PRINTF1(_L("RWindowTreeNode::SetOrdinalPosition (Priority)"));
			}
		else
			{
			GetWindowTreeNode()->SetOrdinalPosition(inputWindowPosition);
			INFO_PRINTF1(_L("RWindowTreeNode::SetOrdinalPosition"));
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KInputWindowPosition());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdWindowGroupId()
	{
	TInt	actual = GetWindowTreeNode()->WindowGroupId();
	INFO_PRINTF2(_L("RWindowTreeNode::WindowGroupId =	%d"), actual);
	SetIdentifier(actual);
	}


void CT_DataWindowTreeNode::DoCmdSetPointerCursor(const TDesC& aSection)
	{
	TPointerCursorMode	inputCursorNumber=EPointerCursorNormal;
 	if ( CT_GraphicsUtil::ReadPointerCursorMode(*this, aSection, KInputCursorNumber(), inputCursorNumber) )
		{
		TInt	err=GetWindowTreeNode()->SetPointerCursor(inputCursorNumber);
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(_L("**** SetPointerCursor failed with error	%d"),	err);
			SetError(err);
			}

		INFO_PRINTF1(_L("RWindowTreeNode::SetPointerCursor"));
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KInputCursorNumber());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdSetCustomPointerCursorL(const TDesC& aSection)
	{
	TPtrC	objectName;
	if( GetStringFromConfig(aSection, KObjectName(), objectName) )
		{
		RWsSession*			wsSession = static_cast<RWsSession*>(GetDataObjectL(objectName));
		RWsPointerCursor 	pointerCursor(*wsSession);
		User::LeaveIfError(pointerCursor.Construct(0));
		CleanupClosePushL(pointerCursor);

		INFO_PRINTF1(_L("RWindowTreeNode::SetCustomPointerCursor"));
		GetWindowTreeNode()->SetCustomPointerCursor(pointerCursor);

		CleanupStack::PopAndDestroy(&pointerCursor);
		}
	else
		{
		TPtrC pointerCursorName;
	
		if( GetStringFromConfig(aSection, KFldPointerCursor(), pointerCursorName) )
			{
			RWsPointerCursor*			pointerCursor = static_cast<RWsPointerCursor*>(GetDataObjectL(pointerCursorName));

			INFO_PRINTF1(_L("RWindowTreeNode::SetCustomPointerCursor"));
			GetWindowTreeNode()->SetCustomPointerCursor(*pointerCursor);
			}
		
		else
			{
			ERR_PRINTF3(_L("Missing parameter %S or %S"), &KFldPointerCursor(),&KObjectName());
			SetBlockResult(EFail);
			}
		}
	
	}


void CT_DataWindowTreeNode::DoCmdEnableOnEvents(const TDesC& aSection)
	{
	TEventControl	eventControl=EEventControlOnlyWithKeyboardFocus;
	CT_GraphicsUtil::ReadEventControl(*this, aSection, KInputEventControl(), eventControl);

	INFO_PRINTF1(_L("RWindowTreeNode::EnableOnEvents"));
	TInt	err=GetWindowTreeNode()->EnableOnEvents(eventControl);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableOnEvents failed with error %d"), err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableOnEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableOnEvents"));
	GetWindowTreeNode()->DisableOnEvents();
	}


void CT_DataWindowTreeNode::DoCmdEnableGroupChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::EnableGroupChangeEvents"));
	TInt	err = GetWindowTreeNode()->EnableGroupChangeEvents();
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableGroupChangeEvents failed with error %d"),	err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableGroupChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableGroupChangeEvents"));
	GetWindowTreeNode()->DisableGroupChangeEvents();
	}


void CT_DataWindowTreeNode::DoCmdEnableFocusChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::EnableFocusChangeEvents"));
	TInt	err = GetWindowTreeNode()->EnableFocusChangeEvents();
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableFocusChangeEvents failed with error %d"),	err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableFocusChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableFocusChangeEvents"));
	GetWindowTreeNode()->DisableFocusChangeEvents();
	}


void CT_DataWindowTreeNode::DoCmdEnableGroupListChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::EnableGroupListChangeEvents"));
	TInt	err = GetWindowTreeNode()->EnableGroupListChangeEvents();
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableGroupListChangeEvents failed with error %d"),	err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableGroupListChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableGroupListChangeEvents"));
	GetWindowTreeNode()->DisableGroupListChangeEvents();
	}


void CT_DataWindowTreeNode::DoCmdEnableVisibilityChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::EnableVisibilityChangeEvents"));
	TInt	err = GetWindowTreeNode()->EnableVisibilityChangeEvents();
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableVisibilityChangeEvents failed with error %d"),	err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableVisibilityChangeEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableVisibilityChangeEvents"));
	GetWindowTreeNode()->DisableVisibilityChangeEvents();
	}


void CT_DataWindowTreeNode::DoCmdEnableErrorMessages(const TDesC& aSection)
	{
	TEventControl	eventControl=EEventControlOnlyWithKeyboardFocus;
	CT_GraphicsUtil::ReadEventControl(*this, aSection, KInputEventControl(), eventControl);

	INFO_PRINTF1(_L("RWindowTreeNode::EnableErrorMessages"));
	TInt	err=GetWindowTreeNode()->EnableErrorMessages(eventControl);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableErrorMessages failed with error	%d"),	err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableErrorMessages()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableErrorMessages"));
	GetWindowTreeNode()->DisableErrorMessages();
	}


void CT_DataWindowTreeNode::DoCmdEnableModifierChangedEvents(const TDesC& aSection)
	{
	TEventModifier	eventModifier = EModifierAutorepeatable;
	CT_GraphicsUtil::ReadEventModifier(*this, aSection, KInputEventModifier(), eventModifier);

	TEventControl	eventControl = EEventControlOnlyWithKeyboardFocus;
	CT_GraphicsUtil::ReadEventControl(*this, aSection, KInputEventControl(), eventControl);

	INFO_PRINTF1(_L("RWindowTreeNode::EnableModifierChangedEvents"));
	TInt	err=GetWindowTreeNode()->EnableModifierChangedEvents(eventModifier, eventControl);
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("**** EnableModifierChangedEvents failed with error	%d"),	err);
		SetError(err);
		}
	}


void CT_DataWindowTreeNode::DoCmdDisableModifierChangedEvents()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::DisableModifierChangedEvents"));
	GetWindowTreeNode()->DisableModifierChangedEvents();
	}


void CT_DataWindowTreeNode::DoCmdSetNonFading(const TDesC& aSection)
	{
	TBool	inputNonFading=ETrue;
	if(	GetBoolFromConfig(aSection, KInputNonFading(), inputNonFading))
		{
		INFO_PRINTF1(_L("RWindowTreeNode::SetNonFading"));
		GetWindowTreeNode()->SetNonFading(inputNonFading);
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KInputNonFading());
		SetBlockResult(EFail);
		}
	}


void CT_DataWindowTreeNode::DoCmdSetFaded(const TDesC& aSection)
	{
	TBool	inputFaded=ETrue;
	if( !GetBoolFromConfig(aSection, KInputFaded(), inputFaded))
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KInputFaded());
		SetBlockResult(EFail);
		}
	else
		{
		RWindowTreeNode::TFadeControl	fadeControl=RWindowTreeNode::EFadeIncludeChildren;
		CT_GraphicsUtil::ReadFadeControl(*this, aSection, KInputFadeControl(), fadeControl);

		TInt	inputBlackMap=0;
		TInt	inputWhiteMap=255;
		if( GetIntFromConfig(aSection, KInputBlackMap(), inputBlackMap) &&
			GetIntFromConfig(aSection, KInputWhiteMap(), inputWhiteMap) )
			{
			INFO_PRINTF1(_L("RWindowTreeNode::SetFaded (Map Fading)"));
			GetWindowTreeNode()->SetFaded(inputFaded, fadeControl, (TUint8) inputBlackMap, (TUint8) inputWhiteMap);
			}
		else
			{
			INFO_PRINTF1(_L("RWindowTreeNode::SetFaded"));
			GetWindowTreeNode()->SetFaded(inputFaded, fadeControl);
			}
		}
	}


void CT_DataWindowTreeNode::DoCmdClearPointerCursor()
	{
	INFO_PRINTF1(_L("RWindowTreeNode::ClearPointerCursor"));
	GetWindowTreeNode()->ClearPointerCursor();
	}
