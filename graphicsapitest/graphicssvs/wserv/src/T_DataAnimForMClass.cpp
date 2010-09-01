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

#include "T_DataAnimForMClass.h"
#include "T_GraphicsUtil.h"
#include "T_AnimDef.h"
#include "../AnimPlugin/inc/T_AnimDef.h"
#include <w32adll.h>
#include <e32keys.h>
#include "w32std.h" 
#define KMaxBufSize 250

/*@{*/
_LIT(KDataClassname,							"RAnimForMClass");

///	Fields

_LIT(KFldWin, 									"win");
_LIT(KFldWingrp, 								"wingrp");
_LIT(KFldSprite, 								"sprite");
_LIT(KFldType, 									"type");


_LIT(KFldOpcode, 								"opcode");


_LIT(KFldExpected, 								"expected");
_LIT(KFldRect, 									"rect");
_LIT(KFldState, 								"state");
_LIT(KFldWs, 									"ws");
_LIT(KFldWingrpId, 								"windowgroupid");
_LIT(KFldOrdinalPosition, 						"pos");
_LIT(KFldOrdinalPriority, 						"ordinalpriority");
_LIT(KFldScreenNumb, 							"screennumber");
_LIT(KFldFullOrdinPostn, 						"fullpos");
_LIT(KFldExpectedPoint, 						"expectedpoint");
_LIT(KFldGetEvent, 								"getevent");
_LIT(KFldInterface, 							"interface");
_LIT(KFldPostRawEvent, 							"rawevent");




_LIT(KFldSyncMode, 								"animsync");
_LIT(KFldInterval, 								"interval");
_LIT(KFldNotifications, 						"notification");
_LIT(KFldMessage, 								"message");
_LIT(KFldHandleBitmap, 							"bitmap");
_LIT(KFldHandleFont, 							"font");
_LIT(KFldMemberIndex, 							"member_index");
_LIT(KFldIsFullUpdate, 							"isfullupdate");
_LIT(KFldIsActivate, 							"isactivate");
_LIT(KFldSetReplyBufHandle1, 					"slot2");
_LIT(KFldSetReplyBufHandle2, 					"slot3");
_LIT(KFldPointPosition,							"pos");
_LIT(KFldRAnim,						        	"anim");

_LIT(KESyncNone , 								"ESyncNone");
_LIT(KESyncFlash, 								"ESyncFlash");
_LIT(KESyncSecond, 								"ESyncSecond");
_LIT(KESyncMinute, 								"ESyncMinute");
_LIT(KESyncDay, 								"ESyncDay");

_LIT(KENone, 									"ENone");
_LIT(KEPointerMove, 							"EPointerMove");
_LIT(KEPointerSwitchOn, 						"EPointerSwitchOn");
_LIT(KEKeyDown, 								"EKeyDown");
_LIT(KEKeyUp, 									"EKeyUp");
_LIT(KERedraw, 									"ERedraw");
_LIT(KESwitchOn, 								"ESwitchOn");
_LIT(KEActive, 								    "EActive");
_LIT(KEInactive,                                "EInactive");
_LIT(KEUpdateModifiers,                         "EUpdateModifiers");
_LIT(KEButton1Down,                             "EButton1Down");
_LIT(KEButton1Up,                               "EButton1Up");
_LIT(KEButton2Down,                             "EButton2Down");
_LIT(KEButton2Up,                               "EButton2Up");
_LIT(KEButton3Down,                             "EButton3Down");
_LIT(KEButton3Up,                               "EButton3Up");
_LIT(KESwitchOff,                               "ESwitchOff");
_LIT(KEKeyRepeat,                               "EKeyRepeat");
_LIT(KECaseOpen,                                "ECaseOpen");
_LIT(KECaseClose,                               "ECaseClose");
_LIT(KERestartSystem,                           "ERestartSystem");



///	Commands
_LIT(KCmdnew, 									"new");
_LIT(KCmdDestructorGeneral, 					"~");
_LIT(KCmdDestructor, 							"~RAnimForMClass");
_LIT(KCmdConstruct, 							"Construct");
_LIT(KCmdCommandReply, 							"CommandReply");
_LIT(KCmdDestroy, 								"Destroy");
_LIT(KCmdClose, 								"Close");

///	Logging
_LIT(KLogErrMissingPara, "Missing parameter '%S'");


_LIT(KLogErrConstruct, "Construct return err = %d");
_LIT(KLogInfoCmdConstruct1, "execute RAnimForMClass::Construct(const RWindowBase &, TInt, const TDesC8 &)");

_LIT(KLogInfoCmdConstruct3, "execute RAnimForMClass::Construct(const RWsSprite &, TInt, const TDesC8 &)");

_LIT(KLogErrCommandReply, "CommandReply return err = %d");
_LIT(KLogInfoCmdCommandReply1, "execute RAnimForMClass::CommandReply(TInt),opcode=%d");

_LIT(KLogInfoCmdCommandReply3, "execute RAnimForMClass::CommandReply(TInt, const TDesC8 &, const TIpcArgs &),opcode=%d");

_LIT(KLogInfoCmdDestroy, "execute RAnimForMClass::Destroy()");
_LIT(KLogInfoCmdDestructor1, "execute ~RAnimForMClass()");
_LIT(KLogMissingParameter, "Missing parameter '%S'");
_LIT(KlogDateTime, "DateTime");
/*@}*/

CT_DataAnimForMClass* CT_DataAnimForMClass::NewL()
	{
	CT_DataAnimForMClass*	ret = new (ELeave) CT_DataAnimForMClass();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataAnimForMClass::CT_DataAnimForMClass()
:	iActiveCallback(NULL)
	,iAnim(NULL)
	{
	}

void CT_DataAnimForMClass::ConstructL()
	{
	iSendBuf.CreateL(KMaxBufSize);
	iSendBuf.FillZ(1);
	
	for (TInt i=0;i<KIpcArgNum;i++)	
		iIpcBuf[i].CreateL(KMaxBufSize);
	}

CT_DataAnimForMClass::~CT_DataAnimForMClass()
	{
	INFO_PRINTF1(KLogInfoCmdDestructor1);
	DestroyData();
	iSendBuf.Close();
	
	for (TInt i=0;i<KIpcArgNum;i++)	
		iIpcBuf[i].Close();	
	}

TAny* CT_DataAnimForMClass::GetObject()
	{
    return iAnim;
	}

void CT_DataAnimForMClass::SetObjectL(TAny* aAny)
	{
	DestroyData();
    iAnim = static_cast<T_RAnimChild*> (aAny);
	}

void CT_DataAnimForMClass::DisownObjectL()
	{
    iAnim = NULL;
	}

void CT_DataAnimForMClass::DestroyData()
	{
	DoCmdDestroy();
	}

/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @return ETrue if the command is processed
*/
TBool CT_DataAnimForMClass::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection,const TInt)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructorGeneral || aCommand==KCmdDestructor )
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
	else if ( aCommand==KCmdCommandReply )
		{
		DoCmdCommandReplyL(aSection);
		}
	else if ( aCommand==KCmdDestroy )
		{
		DoCmdDestroy();
		}
	else if ( aCommand==KCmdClose )
		{
		DoCmdClose();
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CT_DataAnimForMClass::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();
	TPtrC  datRAnimName ; 
	if ( GetStringFromConfig(aSection, KFldRAnim, datRAnimName))
		{
		iAnim = static_cast<T_RAnimChild*>(GetDataObjectL(datRAnimName));
		}
	else 
		ERR_PRINTF2(KLogErrMissingPara, &KFldWin);
	}

void CT_DataAnimForMClass::ResetIpcArg()
	{
	for (TInt i=0; i<KIpcArgNum; i++)
		{
		iIpcArgs.Set(i, TIpcArgs::ENothing);
		}
	}

void CT_DataAnimForMClass::DoCmdConstructL(const TDesC& aSection)
	{
	TInt err = KErrNone;
	
	// Get test data for command input parameter(s)
	TBool dataOk = ETrue;

	// Get "const RWindowBase &aDevice" or "const RWsSprite &aDevice"
	TPtrC        datWinName;
	RWindowBase* win = NULL;
	TPtrC        datSpriteName;
	RWsSprite*   sprite = NULL;
	if ( GetStringFromConfig(aSection, KFldWin, datWinName))
		{
		win = static_cast<RWindowBase*>(GetDataObjectL(datWinName));
		}
	else if ( GetStringFromConfig(aSection, KFldSprite, datSpriteName))
		{
		sprite = static_cast<RWsSprite*>(GetDataObjectL(datSpriteName));
		}
	if (!win && !sprite )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldWin);
		ERR_PRINTF2(KLogErrMissingPara, &KFldSprite);
		}
	
	//Get "TInt aType"
	TInt datType;
	if ( !CT_GraphicsUtil::ReadAnimType(*this,aSection, KFldType, datType))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldType);
		}
	
	// Get "const TDesC8 &aArgs"
	TPtrC 	inputStr;

	// get "const TIpcArgs &aIpcArgs"


	if ( !dataOk )
		{
		SetBlockResult(EFail);
		}
	else
		{	
		// Execute command and log parameters
		if (win)
			{
				INFO_PRINTF1(KLogInfoCmdConstruct1);
				if (iAnim != NULL)
				     err = iAnim->Construct(*win, datType, iSendBuf);	
			}

		else
			{
				INFO_PRINTF1(KLogInfoCmdConstruct3);
				TInt len=iSendBuf.Length();
				if (iAnim != NULL)
					err = iAnim->Construct(*sprite, datType, iSendBuf);	
				    
				 
			}
		
		// Check the command return code
		if(err != KErrNone)
			{
			ERR_PRINTF2(KLogErrConstruct, err);
			SetError(err);
			}
		}
	}

void CT_DataAnimForMClass::DoCmdCommandReplyL(const TDesC& aSection)
	{

	// Get test data for command input parameter(s)
	TBool dataOk = ETrue;

	// Get "TInt aOpcode"
	TInt datOpcode;
	if ( !CT_GraphicsUtil::ReadAnimCommand(*this, aSection, KFldOpcode,
			datOpcode))//ReadAnimCommand():Convert datOpcode from string type to Int type
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldOpcode);
		}

	if (!dataOk)
		{
		_LIT(KFailed, "Can not read the Operation code from the config file. Failed. ");
		INFO_PRINTF1(KFailed);
		SetBlockResult(EFail);
		}
	switch (datOpcode)
		{
			//Util
		case ECmdRetrieveResult:
			DoCmdRetrieveResult(aSection, datOpcode);
			break;
		case ECmdUtilEatupMemory:
			DoCmdEatupMemory(datOpcode);
			break;
		case ECmdUtilFreeEatenMemory:
			DoCmdFreeEatenMemory(datOpcode);
			break;
			// The MAnimGeneralFunctioins
		case ECmdGeneralAnimate:
			DoCmdGeneralAnimate(aSection, datOpcode);
			break;
		case ECmdGeneralClient:
			DoCmdGeneralClient(aSection, datOpcode);
			break;
		case ECmdGeneralFlashStateOn:
			DoCmdGeneralFlashStateOn(aSection, datOpcode);
			break;
		case ECmdGeneralPanic:
			DoCmdGeneralPanic(aSection, datOpcode) ;
			break;
		case ECmdGeneralScreenDevice:
			DoCmdGeneralScreenDevice(aSection, datOpcode) ;
			break;
		case ECmdGeneralExtendedInterface:
			DoCmdGeneralExtendedInterface(aSection, datOpcode) ;
			break;
		case ECmdGeneralWindowExtension:
			DoCmdGeneralWindowExtension(aSection, datOpcode) ;
			break;
		case ECmdGeneralNumOfExtInterfaces:
			DoCmdGeneralNumOfExtInterfaces(aSection, datOpcode) ;
			break;
		case ECmdGeneralEventExtension:
			DoCmdGeneralEventExtension(aSection, datOpcode) ;
			break;
		case ECmdGeneralGetRawEvents:
			DoCmdGeneralGetRawEvents(aSection, datOpcode);
			break;
		case ECmdGeneralPostRawEvent:
			DoCmdGeneralPostRawEvent(aSection, datOpcode);
			break;
		case ECmdGeneralPostKeyEvent:
			DoCmdGeneralPostKeyEventL(aSection, datOpcode);
			break;
		case ECmdGeneralSetSync:
			DoCmdGeneralSetSync(aSection, datOpcode);
			break;
		case ECmdGeneralSync:
			DoCmdGeneralSync(aSection, datOpcode);
			break;
		case ECmdGeneralSetInterval:
			DoCmdGeneralSetInterval(aSection, datOpcode);
			break;
		case ECmdGeneralSetNextInterval:
			DoCmdGeneralSetNextInterval(aSection, datOpcode);
			break;
		case ECmdGeneralSystemTime:
			DoCmdGeneralSystemTime(aSection, datOpcode);
			break;
		case ECmdGeneralRegisterForNotis:
			DoCmdGeneralRegisterForNotis(aSection, datOpcode);
			break;
		case ECmdGeneralMessage:
			DoCmdGeneralMessage(aSection, datOpcode);
			break;
		case ECmdGeneralDuplicateBitmapL:
			DoCmdGeneralDuplicateBitmapL(aSection, datOpcode);
			break;
		case ECmdGeneralDuplicateFontL:
			DoCmdGeneralDuplicateFontL(aSection, datOpcode);
			break;
		case ECmdGeneralCloseFont:
			DoCmdGeneralCloseFontL(aSection, datOpcode);
			break;
		case ECmdGeneralReplyBuf8:
			DoCmdGeneralReplyBuf8(aSection, datOpcode);
			break;
		case ECmdGeneralReplyBuf16:
			DoCmdGeneralReplyBuf16(aSection, datOpcode);
			break;
			// the MAnimSpriteFunctioins 
		case ECmdSpriteGetSpriteMember:
			DoCmdSpriteGetSpriteMember(aSection, datOpcode);
			break;
		case ECmdSpriteUpdateMember:
			DoCmdSpriteUpdateMember(aSection, datOpcode) ;
			break;
		case ECmdSpriteActivate:
			DoCmdSpriteActivate(aSection, datOpcode) ;
			break;
		case ECmdSpriteSpriteCanBeSeen:
			DoCmdSpriteSpriteCanBeSeen(aSection, datOpcode) ;
			break;
		case ECmdSpriteSizeChangedL:
			DoCmdSpriteSizeChangedL(aSection, datOpcode) ;
			break;
		case ECmdSpriteSetPosition:
			DoCmdSpriteSetPosition(aSection, datOpcode) ;
			break;
			//MAnimWindowFunctions
		case ECmdWindowActivateGc:
			{
			DoWindowActivateGc(datOpcode);
			break;
			}
		case ECmdWindowSetRect:
		case ECmdWindowIsStarted:
			{
			DoWindowSetRect(datOpcode, aSection);
			break;
			}
		case ECmdWindowWindowSize:
			{
			DoWindowWindowSizeL(datOpcode, aSection);
			break;
			}
		case ECmdWindowIsHidden:
			{
			DoWindowIsHidden(datOpcode, aSection);
			break;
			}
		case ECmdWindowSetVisible:
			{
			DoWindowSetVisible(datOpcode, aSection);
			break;
			}
		case ECmdWindowInvalidate:
			{
			DoWindowInvalidate(datOpcode, aSection);
			break;
			}
		case ECmdWindowParameters:
			{
			DoWindowParametersL(datOpcode, aSection);
			break;
			}
		case ECmdWindowVisibleRegion:
			{
			DoWindowVisibleRegion(datOpcode, aSection);
			break;

			}
			//MAnimGeneralFunctionsWindowExtension
		case ECmdGfweScreens:
			{
			DoGeneralExtScreensL(datOpcode, aSection);
			break;
			}
		case ECmdGfweFocusScreens:
			{
			DoGeneralExtFocusScreensL(datOpcode, aSection);
			break;
			}
		case ECmdGfweSetFocusScreen:
			{
			DoGeneralExtSetFocusScreen(datOpcode, aSection);
			break;
			}
		case ECmdGfweWindowGroups:
			{
			DoGeneralExtWindowGroupsL(datOpcode, aSection);
			break;
			}
		case ECmdGfweWindowGroupInfo:
			{
			DoGeneralExtWindowGroupInfoL(datOpcode, aSection);
			break;
			}
		case ECmdGfweWindowGroupName:
			{
			DoGeneralExtWindowGroupNameL(datOpcode, aSection);
			break;
			}
		case ECmdGfweSetOrdinalPosition:
			{
			DoGeneralExtSetOrdinalPositionL(datOpcode, aSection);
			break;
			}
		case ECmdGfweWindowConfig:
			{
			DoGeneralExtWindowConfigL(datOpcode, aSection);
			break;
			}
			//MAnimFreeTimerWindowFunctions
		case ECmdFreeTimerWinDeactivateGc:
			{
			DoFreeTimerWinDeactivateGc(datOpcode);
			break;
			}
		case ECmdFreeTimerWinUpdate:
			{
			DoFreeTimerWinUpdate(datOpcode);
			break;
			}
		default:
			{
			_LIT(KFailed2, "Unsupported operation code. ");
			INFO_PRINTF1(KFailed2);
			break;
			}
		}
	}

void CT_DataAnimForMClass::DoCmdClose()
	{
	iAnim  = NULL ;
	}

void CT_DataAnimForMClass::DoCmdDestroy()
	{
	INFO_PRINTF1(KLogInfoCmdDestroy);
	iAnim  = NULL ;
	}

void CT_DataAnimForMClass::DoCmdDestructor()
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogInfoCmdDestructor1);
	iAnim = NULL;
	}

//*************************MAnimWindowFunctions verifying********************************
void CT_DataAnimForMClass::DoWindowActivateGc(TInt aOpcode)
	{
	INFO_PRINTF2(KLogInfoCmdCommandReply1, aOpcode);
	TInt err=iAnim->CommandReply(aOpcode);
	INFO_PRINTF2(KLogErrCommandReply, err);

	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogErrCommandReply, err);
		SetError(err);
		}
	}

void CT_DataAnimForMClass::DoWindowSetRect(TInt aOpcode, const TDesC& aSection)
	{
	switch (aOpcode)
		{
		case ECmdWindowSetRect:
			{
			TPtrC8 ptr;
			TRect sendRect;
			if (!GetRectFromConfig(aSection, KFldRect, sendRect))
				{
				ERR_PRINTF2(KLogErrMissingPara, &KFldRect());
				SetBlockResult(EFail);
				}
			TPckgBuf<TRect> sendVal(sendRect);
			iIpcArgs.Set(0, TIpcArgs::ENothing);
			iIpcArgs.Set(1, &sendVal);

			INFO_PRINTF2(KLogInfoCmdCommandReply3, aOpcode);
			TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
			INFO_PRINTF2(KLogErrCommandReply, err);

			if (err != KErrNone)
				{
				ERR_PRINTF2(KLogErrCommandReply, err);
				SetError(err);
				}
			ResetIpcArg();
			break;
			}
		case ECmdWindowIsStarted:
			{
			INFO_PRINTF2(KLogInfoCmdCommandReply1, aOpcode);
			TInt err=iAnim->CommandReply(aOpcode);
			INFO_PRINTF2(KLogErrCommandReply, err);
			if (err != KErrNone)
				{
				ERR_PRINTF2(KLogErrCommandReply, err);
				SetError(err);
				}
			break;
			}
		}
	}

void CT_DataAnimForMClass::DoWindowWindowSizeL(TInt aOpcode,const TDesC& aSection)
	{
	TPtrC8 ptr;
	TPtrC datWinName;
	RWindowBase* win= NULL;
	if (GetStringFromConfig(aSection, KFldWin, datWinName))
		{
		win = static_cast<RWindowBase*>(GetDataObjectL(datWinName));
		}
	TSize expected=win->Size();
	TSize resultBuf;
	TPckgBuf<TSize> retrndPck(expected);
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1, &retrndPck);
	INFO_PRINTF2(KLogInfoCmdCommandReply3, aOpcode);
	TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, err);

	resultBuf=retrndPck();
	if (expected!=resultBuf)
		{
		ERR_PRINTF5(_L("The returned value is not as expected, expected: (%d,%d), actual: (%d,%d)"), expected.iWidth,expected.iHeight, resultBuf.iWidth,resultBuf.iHeight);
		SetBlockResult(EFail);
		}
	ResetIpcArg();
	} 

void CT_DataAnimForMClass::DoWindowIsHidden(TInt aOpcode,const TDesC& aSection)
	{
	TBool expectedVal;

	if (!GetBoolFromConfig(aSection, KFldExpected, expectedVal))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF2(KLogInfoCmdCommandReply1, aOpcode);
		TInt actual=iAnim->CommandReply(aOpcode);
		INFO_PRINTF2(KLogErrCommandReply, actual);
		if (actual!=expectedVal)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, actual);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataAnimForMClass::DoWindowSetVisible(TInt aOpcode,const TDesC& aSection)
	{
	TBool setVisible;
	TPtrC8 ptr;

	if (!GetBoolFromConfig(aSection, KFldState, setVisible))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldState());
		SetBlockResult(EFail);
		}

	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TBool> setVisiblePck(setVisible);
	iIpcArgs.Set(1, &setVisiblePck);
	INFO_PRINTF2(KLogInfoCmdCommandReply3, aOpcode);
	TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, err);
	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogErrCommandReply, err);
		SetError(err);
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoWindowInvalidate(TInt aOpcode,const TDesC& aSection)
	{

	TPtrC datWinName;
	TPtrC8 ptr;
	TRect rectVal;
	if (!GetRectFromConfig(aSection, KFldRect, rectVal))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldRect());
		SetBlockResult(EFail);
		}
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TRect> rectValPck(rectVal);
	iIpcArgs.Set(1, &rectValPck);
	INFO_PRINTF2(KLogInfoCmdCommandReply3, aOpcode);
	TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, err);
	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogErrCommandReply, err);
		SetError(err);
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoWindowParametersL(TInt aOpcode,const TDesC& aSection)
	{
	RWindowBase* win= NULL;
	TSize expectedSize;
	TInt expectedDisplyMd;
	TPtrC val;
	TPtrC8 ptr;
	TPtrC datWinName;
	TPckgBuf<TRect> retrndWindowPostn;
	TPckgBuf<TInt> retrndDisplyMd;
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1, &retrndWindowPostn);
	iIpcArgs.Set(2, &retrndDisplyMd);

	//Send command to server
	TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, err);
	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogErrCommandReply, err);
		SetError(err);
		}
	else
		{
		//Get the window's Size and Displaymode
		if (GetStringFromConfig(aSection, KFldWin, datWinName))
			{
			win = static_cast<RWindowBase*>(GetDataObjectL(datWinName));
			}
		expectedSize=win->Size();
		expectedDisplyMd=win->DisplayMode();

		TRect retrndWinInf=retrndWindowPostn();
		TSize retrndSize=retrndWinInf.Size();
		TInt retrndDisplyM=retrndDisplyMd();

		if (retrndSize!=expectedSize)
			{
			ERR_PRINTF5(_L("The value is not as expected! expected: (%d,%d), actual: (%d,%d)"), expectedSize.iWidth,expectedSize.iHeight, retrndSize.iWidth,retrndSize.iHeight);
			SetBlockResult(EFail);
			}
		if (retrndDisplyM!=expectedDisplyMd)
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expectedDisplyMd, retrndDisplyM);
			SetBlockResult(EFail);
			}
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoWindowVisibleRegion(TInt aOpcode,const TDesC& aSection)
	{
	TInt expecteValue;
	TInt actual;

	if (!GetIntFromConfig(aSection, KFldExpected, expecteValue))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldExpected());
		SetBlockResult(EFail);
		}
	else
		{
		actual=iAnim->CommandReply(aOpcode);
		INFO_PRINTF2(KLogErrCommandReply, actual);
		if (actual!=expecteValue)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expecteValue, actual);
			SetBlockResult(EFail);
			}
		}
	}

//****************MAnimGeneralFunctionsWindowExtension verifying*******************************

void CT_DataAnimForMClass::DoGeneralExtScreensL(TInt aOpcode,const TDesC& aSection)
	{
	RWsSession* ws;
	TPtrC datWsName;
	if (GetStringFromConfig(aSection, KFldWs, datWsName))
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(datWsName));
		}
	TInt expectedScreenNumb=ws->NumberOfScreens();
	//Send Command, no data to server, check the returned value from Server by comparing with RWsSession::NumberOfScreens
	TInt actualScreenNumb=iAnim->CommandReply(aOpcode);
	INFO_PRINTF2(KLogErrCommandReply, actualScreenNumb);
	if (actualScreenNumb!=expectedScreenNumb)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedScreenNumb, actualScreenNumb);
		SetBlockResult(EFail);
		}
	}

void CT_DataAnimForMClass::DoGeneralExtFocusScreensL(TInt aOpcode,const TDesC& aSection)
	{
	RWsSession* ws;
	TPtrC datWsName;
	TInt expectedVal;

	TInt actual=iAnim->CommandReply(aOpcode);
	INFO_PRINTF2(_L("FocuseScreens is %d"),actual);

	//If there is no expected value set in ini file, get the focus screen; 
	if (!GetIntFromConfig(aSection, KFldExpected, expectedVal))
		{
		if (GetStringFromConfig(aSection, KFldWs, datWsName))
			{
			ws = static_cast<RWsSession*>(GetDataObjectL(datWsName));
			}

		expectedVal=ws->GetFocusScreen();
		}

	if (actual!=expectedVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, actual);
		SetBlockResult(EFail);
		}
	}

void CT_DataAnimForMClass::DoGeneralExtSetFocusScreen(TInt aOpcode,const TDesC& aSection)
	{
	TInt screenNo;
	TPtrC8 ptr;
	TPtrC datWsName;

	if (!GetIntFromConfig(aSection, KFldScreenNumb, screenNo))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldScreenNumb());
		SetBlockResult(EFail);
		}
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TInt> screenNoPck(screenNo);
	iIpcArgs.Set(1, &screenNoPck);
	TInt actual=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, actual);

	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoGeneralExtWindowGroupsL(TInt aOpcode,const TDesC& aSection)
	{
	TPtrC8 ptr;
	TInt screenNo;
	TPtrC datWsName;
	RWsSession* ws;
	if (!GetIntFromConfig(aSection, KFldScreenNumb, screenNo))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldScreenNumb());
		SetBlockResult(EFail);
		}
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TInt> screenNoPck(screenNo);
	iIpcArgs.Set(1, &screenNoPck);
	TInt actual=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, actual);

	if (!GetStringFromConfig(aSection, KFldWs, datWsName))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldWs());
		SetBlockResult(EFail);
		}
	ws = static_cast<RWsSession*>(GetDataObjectL(datWsName));
	TInt expectedVal=ws->NumWindowGroups();

	if (actual!=expectedVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, actual);
		SetBlockResult(EFail);
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoGeneralExtWindowGroupInfoL(TInt aOpcode,const TDesC& aSection)
	{
	TInt screenNo;
	TPtrC8 ptr;
	TPtrC datWingrpName;
	TInt retrnPck;
	TInt expectedGrpInfo;
	RWindowGroup* wingrp;
	//Get window group object in order to verify the window group's name's existance.
	if (!GetStringFromConfig(aSection, KFldWingrp, datWingrpName))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldWingrp());
		SetBlockResult(EFail);
		}
	wingrp = static_cast<RWindowGroup*>(GetDataObjectL(datWingrpName));

	//Get screen number from ini file and set to IPC slot 1.
	if (!GetIntFromConfig(aSection, KFldScreenNumb, screenNo))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldScreenNumb());
		SetBlockResult(EFail);
		}
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TInt> screenNoPck(screenNo);
	iIpcArgs.Set(1, &screenNoPck);

	//Get fullOrdipostion from ini or by using FullOrdinalPosition method and set to IPC slot 2.
	TInt fullOrdiPostion;
	if (!GetIntFromConfig(aSection, KFldFullOrdinPostn, fullOrdiPostion))
		{
		fullOrdiPostion=wingrp->FullOrdinalPosition();
		}
	TPckgBuf<TInt> fullOrdiPostionPck(fullOrdiPostion);
	iIpcArgs.Set(2, &fullOrdiPostionPck);

	//Set IPC slot 3 to receive ID of the TWindowGroupInfo.
	TPckgBuf<TInt> retrnWinGrpInfo;
	iIpcArgs.Set(3, &retrnWinGrpInfo);

	TInt actual=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, actual);
	TBool expectedVal;
	if (!GetBoolFromConfig(aSection, KFldExpected, expectedVal))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	if (actual!=expectedVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, actual);
		SetBlockResult(EFail);
		}
	if (actual!=EFalse)
		{
		retrnPck=retrnWinGrpInfo();//Get returned window group ID

		expectedGrpInfo=wingrp->WindowGroupId();
		if (retrnPck!=expectedGrpInfo)
			{
			ERR_PRINTF3(
			_L("The returned value is not as expected, expected: %d, actual: %d"),
			expectedGrpInfo, retrnPck);
			SetBlockResult(EFail);
			}
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoGeneralExtWindowGroupNameL(TInt aOpcode,const TDesC& aSection)
	{
	TBuf<32> getWindowName;
	TBool expectedVal;
	TInt screenNo;
	TInt fullOrdiPostion;
	//Get Screen number from ini and set to IPC slot 1.
	if (!GetIntFromConfig(aSection, KFldScreenNumb, screenNo))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldScreenNumb);
		SetBlockResult(EFail);
		}
	TPtrC8 ptr;
	TPckgBuf<TInt> screenNoPck(screenNo);
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1, &screenNoPck);

	TPtrC datWingrpName;
	RWindowGroup* wingrp;
	//Get window group object
	if (!GetStringFromConfig(aSection, KFldWingrp, datWingrpName))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldWingrp());
		SetBlockResult(EFail);
		}
	wingrp = static_cast<RWindowGroup*>(GetDataObjectL(datWingrpName));
	//Get fullOrdinary position and set to IPC slot 2.
	if (!GetIntFromConfig(aSection, KFldFullOrdinPostn, fullOrdiPostion))
		{
		fullOrdiPostion=wingrp->FullOrdinalPosition();
		}

	TPckgBuf<TInt> fullOrdiPostionPck(fullOrdiPostion);
	iIpcArgs.Set(2, &fullOrdiPostionPck);
	//Set IPC slot 3 to receive the window name returned from server side.
	TBuf<32> retrnWindowName;
	iIpcArgs.Set(3, &retrnWindowName);

	TInt actual=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	INFO_PRINTF2(KLogErrCommandReply, actual);
	if (!GetBoolFromConfig(aSection, KFldExpected, expectedVal))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	if (actual!=expectedVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, actual);
		SetBlockResult(EFail);
		}
	//If commandreply returns ETrue then check the window group's name.
	if (actual)
		{
		TInt result=wingrp->Name(getWindowName);
		if (result!=KErrNone)
			{
			ERR_PRINTF1(_L("Failed to get the window group's name!"));
			SetError(result);
			}
		INFO_PRINTF3(_L("Check the group name is expected. expected: %d,actual: %d"),&getWindowName,&retrnWindowName);
		if (getWindowName!=retrnWindowName)
			{
			ERR_PRINTF1(_L("The returned window group name is not expected"));
			SetBlockResult(EFail);
			}
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoGeneralExtSetOrdinalPositionL(TInt aOpcode,const TDesC& aSection)
	{
	TInt expectedVal;
	TPtrC8 ptr;
	TInt wingrpid;
	TInt newOrdinPrio;
	TInt newOrdinaPostn;
	TPtrC datWingrpName;
	RWindowGroup* wingrp;
	if (GetStringFromConfig(aSection, KFldWingrp, datWingrpName))
		{
		wingrp = static_cast<RWindowGroup*>(GetDataObjectL(datWingrpName));
		}
	//Before sending command, get window group ID from wrapper or ini as arg(IPC slot 1)
	if (!GetIntFromConfig(aSection, KFldWingrpId, wingrpid))
		{
		wingrpid=wingrp->WindowGroupId();
		INFO_PRINTF2(_L("RWindowTreeNode::WindowGroupId	= %d"), wingrpid);
		}
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TInt> wingrpidPck(wingrpid);
	iIpcArgs.Set(1, &wingrpidPck);

	//Get Group Id from wrapper or ini and send to server as IPC slot2
	if (!GetIntFromConfig(aSection, KFldOrdinalPosition, newOrdinaPostn))
		{
		newOrdinaPostn = wingrp->OrdinalPosition();
		INFO_PRINTF2(_L("RWindowTreeNode::FullOrdinalPosition	= %d"), newOrdinaPostn);
		}
	TPckgBuf<TInt> newOrdinaPostnPck(newOrdinaPostn);
	iIpcArgs.Set(2, &newOrdinaPostnPck);

	//Get new ordinal priority of the window as arg(IPC slot 3) from ini file;
	if (!GetIntFromConfig(aSection, KFldOrdinalPriority, newOrdinPrio))
		{
		newOrdinPrio = wingrp->OrdinalPriority();
		INFO_PRINTF2(_L("RWindowTreeNode::FullOrdinalPriority	= %d"), newOrdinPrio);
		}
	TPckgBuf<TInt> newOrdinPrioPck(newOrdinPrio);
	iIpcArgs.Set(3, &newOrdinPrioPck);

	TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	if (!GetIntFromConfig(aSection, KFldExpected, expectedVal))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}

	if (err!=expectedVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, err);
		SetBlockResult(EFail);
		}
	//Get the new OrdinalPostion and Priority using window group methods;
	if (err!=KErrNotFound)
		{
		TInt retrnOrdinPostn=wingrp->OrdinalPosition();
		TInt retrnOrdinPrio=wingrp->OrdinalPriority();

		if (retrnOrdinPostn!=newOrdinaPostn)
			{
			ERR_PRINTF3(
			_L("The returned ordinalposition is not as expected, expected: %d, actual: %d"),
			newOrdinaPostn, retrnOrdinPostn);
			SetBlockResult(EFail);
			}

		if (retrnOrdinPrio!=newOrdinPrio)
			{
			ERR_PRINTF3(
			_L("The returned ordinalpriority is not as expected, expected: %d, actual: %d"),
			newOrdinPrio, retrnOrdinPrio);
			SetBlockResult(EFail);
			}
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoGeneralExtIsFocusableL(TInt aOpcode, const TDesC& aSection)
	{
	TInt screenNub;
	TBool expectedVal;
	TInt sendFullOrdinPstn;
	if (!GetIntFromConfig(aSection, KFldScreenNumb, screenNub))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KFldScreenNumb);
		}
	TPtrC8 ptr;
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TInt> screenNubPck(screenNub);
	iIpcArgs.Set(1, &screenNubPck);

	TPtrC datWingrpName;
	RWindowGroup* wingrp;
	if (GetStringFromConfig(aSection, KFldWingrp, datWingrpName))
		{
		wingrp = static_cast<RWindowGroup*>(GetDataObjectL(datWingrpName));
		}

	//Get animation window's FullOrdinalPosition as arg(IPC slot 2)

	sendFullOrdinPstn = wingrp->FullOrdinalPosition();
	INFO_PRINTF2(_L("RWindowTreeNode::FullOrdinalPosition	= %d"), sendFullOrdinPstn);
	TPckgBuf<TInt> sendFullOrdinPstnPck(sendFullOrdinPstn);
	iIpcArgs.Set(2, &sendFullOrdinPstnPck);

	//Check the CommandReply return value
	TInt err=iAnim->CommandReply(aOpcode, ptr, iIpcArgs);
	if (!GetBoolFromConfig(aSection, KFldExpected, expectedVal))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}

	if (err!=expectedVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, err);
		SetBlockResult(EFail);
		}
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoGeneralExtWindowConfigL(TInt aOpcode, const TDesC& aSection)
	{
	TBool expectedVal;
	if (!GetBoolFromConfig(aSection, KFldExpected, expectedVal))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		TPtrC8 ptr;
		TInt ret = iAnim->CommandReply(aOpcode, ptr);
		if (ret != expectedVal)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedVal, ret);
			SetBlockResult(EFail);
			}			
		}	
	}

//*******************MAnimFreeTimerWindowFunction verifying*******************************
void CT_DataAnimForMClass::DoFreeTimerWinDeactivateGc(TInt aOpcode)
	{
	INFO_PRINTF2(_L("Excute %d"), aOpcode);
	TPtrC8 ptr;
	TInt err=iAnim->CommandReply(aOpcode, ptr);
	if (err != KErrNone)
		{
		ERR_PRINTF2(KLogErrCommandReply, err);
		SetError(err);
		}
	}

void CT_DataAnimForMClass::DoFreeTimerWinUpdate(TInt aOpcode)
	{
	INFO_PRINTF1(KLogInfoCmdCommandReply1);
	
	TInt err=iAnim->CommandReply(aOpcode);
	
	if(err!=KErrNone)
		{
		ERR_PRINTF2(_L("CommandReply failed with error %d"), err);
		SetError(err);
		}
	}

//------------------------------------------------------------------------------------
void CT_DataAnimForMClass::DoCmdRetrieveResult(const TDesC& aSectName , const TInt& datOpcode)
	{
	TInt result = iAnim->CommandReply(datOpcode);
	TBool expectdVal;
    //Get  expected value from ini file.
	if(!GetBoolFromConfig(aSectName,KFldExpected,expectdVal))
		{
		if ( result != KErrNone)
		   ERR_PRINTF2( KLogErrCommandReply, result);	 	
		else // success
		   { 
		   INFO_PRINTF1(_L("Retrieve Result event return ETrue"));
		   }
		}
	if(result!=expectdVal)
		{
		ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectdVal, result);
		SetBlockResult (EFail);
		}
	else   
		{
		INFO_PRINTF1(_L("Retrieve Result equals to the expected value"));
		}
	} 

void CT_DataAnimForMClass::DoCmdGeneralAnimate(const TDesC& aSectName,  const TInt& datOpcode )
    {
    //Get the datetime from the ini file
    TDateTime dateTime ;
	if(!GetDateTimeFromConfig(aSectName,  dateTime))
	  	{
	 	ERR_PRINTF2(KLogMissingParameter , &KlogDateTime);
	 	SetBlockResult(EFail);  
	  	}	
	TPckgBuf<TDateTime> sendPckg(dateTime);
	iIpcArgs.Set(0,TIpcArgs::ENothing);
	iIpcArgs.Set(1, &sendPckg);
	 
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	// Send the command to notify the server
	TInt ret = iAnim->CommandReply(datOpcode, iSendBuf, iIpcArgs); 
	 
	// Check the command return code 
	if(ret == KErrNone)   
    	{
	    ERR_PRINTF2( KLogErrCommandReply, ret);	 
        SetBlockResult (EFail);          
		}
	else
		{
		INFO_PRINTF1(_L("General Animate invoked succesffully and return successfully "));
		}
	ResetIpcArg();
    }


void CT_DataAnimForMClass::DoCmdGeneralClient(const TDesC& /*aSectName*/,  const TInt& datOpcode )
    {
    // Send the command to notify the server
    INFO_PRINTF1(KLogInfoCmdCommandReply3);
    TInt ret =	iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	
	// Check the return thread id equals to the client thread or not.
	TInt clientThreadId = RThread().Id().Id();
	
	if ( ret == clientThreadId )
		{
		INFO_PRINTF1(_L("General client invoked succesffully"));
		}
	else 
		{
		ERR_PRINTF2( KLogErrCommandReply, ret);	 
	    SetBlockResult (EFail);     
		}
    }
   

void CT_DataAnimForMClass::DoCmdGeneralFlashStateOn(const TDesC& aSectName,  const TInt& datOpcode )
    {
    // Send the command to notify the server. 
    INFO_PRINTF1(KLogInfoCmdCommandReply3);
    TBool expected(EFalse);
    TInt ret = -1 ;
    
    if ( !GetBoolFromConfig(aSectName,KFldExpected, expected) )
		{
	    ERR_PRINTF2(KLogMissingParameter , &KFldExpected);
		SetBlockResult(EFail); 
		} 
   
    ret = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);	
    //If ret return -1, the result value can not be trusted(Please refer to the code in AnimPlugin::WinAnim).
    //So it need another calling untill the ret return a ETrue or EFalse.
    while(ret == -1)
    	{
    	User::After(100000);
    	ret = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);	
    	}
    
   
    // Check if the return is ETrue (1) or EFalse(0)
    if ( ret == static_cast<TInt>(expected) )
	   { 
       INFO_PRINTF1(_L("GeneralFlashStateOn invoked succesffully. Return code equals to expected vaule "));						 			
	   }
   else 
	   {
	   ERR_PRINTF2( KLogErrCommandReply, ret);	 	
       SetBlockResult (EFail); 
	   }
    }

void CT_DataAnimForMClass::DoCmdGeneralPanic(const TDesC& /*aSectName*/ , const TInt& datOpcode)
    {
	//Send the command to notify the server.
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	TInt err = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
								
	//The client will be paniced by the server
	INFO_PRINTF1(_L("GeneralPanic invoked successfully"));	 	 				 		 			
	}

void CT_DataAnimForMClass::DoCmdGeneralScreenDevice(const TDesC& /*aSectName*/ , const TInt& datOpcode )
    {
	//Send the command to notify the server .
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	TInt ret = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
									 
	//Check if the return code is 0(Success) or not.
	if ( ret == KErrNone ) 
	    { 
	    INFO_PRINTF1(_L("GeneralScreenDevice invoked succesffully."));						 			
	    }
	else 
	    {
	    ERR_PRINTF2( KLogErrCommandReply, ret);	 	
	    SetBlockResult (EFail); 
	    }
	}

void CT_DataAnimForMClass::DoCmdGeneralWindowExtension(const TDesC& /*aSectName*/, const TInt& datOpcode)
	{
	// Send the command to notify the server .
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	TInt ret = iAnim->CommandReply(datOpcode, iSendBuf, iIpcArgs);

	// Check if the return code is 0(Success) or not.
	if (ret == KErrNone)
		{
		INFO_PRINTF1(_L("GeneralWindowExtension invoked succesffully."));
		}
	else
		{
		ERR_PRINTF2( KLogErrCommandReply, ret);
		SetBlockResult(EFail);
		}
	}

void CT_DataAnimForMClass::DoCmdGeneralEventExtension( const TDesC& /*aSectName*/,const TInt& datOpcode)
	{
	// Send the command to notify the server .
    INFO_PRINTF1(KLogInfoCmdCommandReply3);
	TInt ret = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );

	// Check if the return code is 0(Success) or not.
	if ( ret == KErrNone )
		{
		INFO_PRINTF1(_L("GeneralEventExtension invoked succesffully."));
		}
	else
		{
		ERR_PRINTF2( KLogErrCommandReply, ret);
		SetBlockResult (EFail);
		}
	}

 void CT_DataAnimForMClass::DoCmdGeneralExtendedInterface(const TDesC& aSectName , const TInt& datOpcode )  
    {
    TInt interface ;
    //Get interface from ini file and set it as IpcArgs 1. 
	if ( !GetIntFromConfig(aSectName,KFldInterface, interface) )
	 	 {
	 	 ERR_PRINTF2(KLogMissingParameter , &KFldInterface);
	 	 SetBlockResult (EFail); 
	 	 }
    TPckgBuf<TInt> sendPckg(interface);
	iIpcArgs.Set(0,TIpcArgs::ENothing);
	iIpcArgs.Set(1, &sendPckg);
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
    //Send the command to notify the server .
	TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
										 
	//  Check the command return code 
	if ( interface == 0 )
	   {
	   TInt expected ;

	   if ( !GetIntFromConfig(aSectName,KFldExpected, expected) )
		   {
		   ERR_PRINTF2(KLogMissingParameter , &KFldInterface);
		   SetBlockResult (EFail); 
		   }
	   if ( result != expected ) /*so far, the expected value is 2.*/
           { 
	       ERR_PRINTF2( KLogErrCommandReply, result);	 	
	       SetBlockResult (EFail); 
           }
	   else 
	       {
           INFO_PRINTF1(_L("extended interface return is okay when the input paramter is 0"));
	       }
	   }
	else if( interface == 1 )
	   {
	   if ( result == 0) // server return 0 stands for okay.
		   { 
		   INFO_PRINTF1(_L("extended interface return is okay when the input paramter is 1")); 
		   }
	   else 
		   { 
		   ERR_PRINTF2( KLogErrCommandReply, result);	 	
		   SetBlockResult (EFail); 
	       } 
	   }
    else if( interface == 2 )
	   {
	   if ( result == 0) // server return 0 stands for okay.
		   { 
		   INFO_PRINTF1(_L("extended interface return is okay when the input paramter is 2")); 
		   }
	   else 
		   { 
		   ERR_PRINTF2( KLogErrCommandReply, result);	 	
		   SetBlockResult (EFail); 
		   } 
	   }  
	else 
	   {
	   if ( result == 0) // server return 0 stands for okay.
		   { 
		   INFO_PRINTF1(_L("extended interface return is a NULL pointer when the input paramter is OUT of 0~2")); 
		   }
	   else 
		   { 
		   ERR_PRINTF2( KLogErrCommandReply, result);	 	
		   SetBlockResult (EFail); 
	       } 
	   }
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoCmdGeneralNumOfExtInterfaces(const TDesC& aSectName,const TInt& datOpcode)
    {
    // Send the command to notify the server.
    INFO_PRINTF1(KLogInfoCmdCommandReply3);
	TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs);

	INFO_PRINTF2(KLogErrCommandReply, result);

	// Check the return code 
	TInt expected;
	if ( !GetIntFromConfig(aSectName,KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingParameter , &KFldInterface);
		SetBlockResult (EFail);
		}
	if (result != expected) // so far ,the expected value is 2. 
		{
		ERR_PRINTF2( KLogErrCommandReply, result);
		SetBlockResult (EFail);
		}
	else
		{
		INFO_PRINTF1(_L("NumberofExtendedInterfaces return is okay when the input paramter is 0"));
		}
	}
 

 void CT_DataAnimForMClass::DoCmdGeneralGetRawEvents(const TDesC& aSectName , const TInt& datOpcode)
    {
    TBool bGetEvent;
    //Get the rawevent parameter from ini file and set it as the IpcArgs 1 . 
    if ( !GetBoolFromConfig(aSectName,KFldGetEvent ,bGetEvent))
		{
		ERR_PRINTF2(KLogMissingParameter , &KFldGetEvent);
		SetBlockResult(EFail);
		}
	iIpcArgs.Set(0,TIpcArgs::ENothing);
	TPckgBuf<TBool> sendPckg(bGetEvent);
	iIpcArgs.Set(1, &sendPckg);

	INFO_PRINTF1(KLogInfoCmdCommandReply3);
    // Send the command to notify the server .
    TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );

	//Check the result is 0(Success) or not.
	if (result != KErrNone )
		{
		ERR_PRINTF2( KLogErrCommandReply, result);
		SetBlockResult (EFail);
		}
	else
		{
		INFO_PRINTF1(_L("GetRawEvents return is okay "));
		}
	}
 
 void CT_DataAnimForMClass::DoCmdGeneralPostRawEvent(const TDesC& aSectName , const TInt& datOpcode)
	{
	TRawEvent rawEvent;
	TPtrC str;
    //Get the post rawevent parameter from ini file and set it as the IpcArgs 1 . 
	if (!GetStringFromConfig(aSectName,KFldPostRawEvent,str))
	    {
	    ERR_PRINTF2(KLogMissingParameter , &KFldPostRawEvent);
	    SetBlockResult(EFail);
	    }
    TRawEvent::TType type;
    if ( str == KENone )
        type = TRawEvent::ENone;
    else if ( str == KEPointerMove )
        type = TRawEvent::EPointerMove;
    else if ( str == KEPointerSwitchOn)
        type = TRawEvent::EPointerSwitchOn;
    else if ( str == KEKeyDown )
        type = TRawEvent::EKeyDown;
    else if ( str == KEKeyUp )
        type = TRawEvent::EKeyUp;
    else if ( str == KERedraw)
        type = TRawEvent::ERedraw;
    else if ( str == KESwitchOn )
        type = TRawEvent::ESwitchOn;
    else if ( str == KEActive)
        type = TRawEvent::EActive;
    else if ( str == KEInactive)
        type = TRawEvent::EInactive;
    else if ( str == KEUpdateModifiers)
        type = TRawEvent::EUpdateModifiers;
    else if ( str == KEButton1Down)
        type = TRawEvent::EButton1Down;
    else if ( str == KEButton1Up)
        type = TRawEvent::EButton1Up;
    else if ( str == KEButton2Down)
        type = TRawEvent::EButton2Down;
    else if ( str == KEButton2Up)
        type = TRawEvent::EButton2Up;
    else if ( str == KEButton3Down)
        type = TRawEvent::EButton3Down;
    else if ( str == KEButton3Up)
        type = TRawEvent::EButton3Up;
    else if ( str == KESwitchOff)
        type = TRawEvent::ESwitchOff;
    else if ( str == KEKeyRepeat)
        type = TRawEvent::EKeyRepeat;
    else if ( str == KECaseOpen)
        type = TRawEvent::ECaseOpen;
    else if ( str == KECaseClose)
        type = TRawEvent::ECaseClose;
    else if ( str == KERestartSystem)
        type = TRawEvent::ERestartSystem;

    rawEvent.Set(type);
    TPckgBuf<TRawEvent> sendPckg(rawEvent);
    
    iIpcArgs.Set(0,TIpcArgs::ENothing);
    iIpcArgs.Set(1, &sendPckg);

    INFO_PRINTF1(KLogInfoCmdCommandReply3);
    //Send the command to notify the server .
	TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );

    //Check the result is 0(Success) or not.
    if (result != KErrNone )
	   {
	   ERR_PRINTF2(KLogErrCommandReply, result);
       SetBlockResult (EFail);
	   }
    else
	   {
	   INFO_PRINTF1(_L("PostRawEvent return is okay "));
	   }
    }  
 

 void CT_DataAnimForMClass::DoCmdGeneralPostKeyEventL(const TDesC& aSectName ,  const TInt& datOpcode)
	{
	/*
	* 1.Use WinAnim plugin dll to post a key event. 
	* 2.Use an RWindowGroup object to capture the key event.
	* */

	TKeyEvent keyEvent;
	TKeyCode aKeyCode;
	_LIT( KCode , "Code"); 
    //Get the keycode from the ini file. 
	if (!CT_GraphicsUtil::ReadKeyCode(*this, aSectName, KCode , aKeyCode))
		{
		ERR_PRINTF2(KLogMissingParameter , &KCode);
		SetBlockResult(EFail);
		}
    // The keyEvent will be set as the IpcArg 1 .    
	TInt captureKey = aKeyCode;
	keyEvent.iCode = aKeyCode;
	keyEvent.iScanCode =aKeyCode;
	keyEvent.iModifiers = 0;
	keyEvent.iRepeats = 0;
		   
	_LIT(KWindowGroup, "windowgroup");
	_LIT(KWsSession, "session");
	RWindowGroup* group = NULL;
	RWsSession* session = NULL;
	TPtrC datName;
	if ( GetStringFromConfig(aSectName, KWindowGroup , datName) )
		{
		group = static_cast<RWindowGroup*>(GetDataObjectL(datName));
		}
	
   	if ( GetStringFromConfig(aSectName, KWsSession , datName) )
   		{
	    session = static_cast<RWsSession*>(GetDataObjectL(datName));
   		}
   	group->CaptureKey(captureKey, 0, 0);	
	TPckgBuf<TKeyEvent> sendPckg(keyEvent);
	iIpcArgs.Set(0,TIpcArgs::ENothing);
	iIpcArgs.Set(1, &sendPckg);   
	TInt err = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
	 
	//Check the result is 0(Success) or not.
	if (err != KErrNone )  
		{
		ERR_PRINTF2(KLogErrCommandReply, err);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("PostKeyEvent return is okay "));
		}  	
	TRequestStatus myStatus;
	session->EventReady(&myStatus);
	TBool bStop = ETrue;
	TWsEvent we;
	while(bStop)
		{
    	User::WaitForRequest(myStatus);
    	session->GetEvent(we);
    	if(we.Type() == EEventKey)
    		{
    		bStop = EFalse;
    	    TInt rst = we.Key()->iCode;    		
    	    if(rst != captureKey)
    	    	{
    	        ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), captureKey, rst);
    	        SetBlockResult(EFail);
    	        }
    	     else
    	    	{
    	    	_LIT(KSuccess, "PostKeyEvent successfully");   
    	        INFO_PRINTF1(KSuccess);
    	        }
    		}
    	else
    		{
    		session->EventReady(&myStatus);
    		}
		}//end while	
	}  
 
 void CT_DataAnimForMClass::DoCmdGeneralSetSync(const TDesC& aSectName ,  const TInt& datOpcode )
   {
   MAnimGeneralFunctions::TAnimSync  syncmode ;
   TPtrC	str;
   // Get the syncmode to be set from ini file and set it as IpcArg 1. 
   if ( !GetStringFromConfig(aSectName,KFldSyncMode ,str))
	  {
	  ERR_PRINTF2(KLogMissingParameter , &KFldSyncMode);
	  SetBlockResult (EFail); 		
	  }
   if (str  ==  KESyncNone )
	  syncmode = MAnimGeneralFunctions::ESyncNone ;
   else if ( str ==  KESyncFlash)
	  syncmode = MAnimGeneralFunctions::ESyncFlash ;
   else if ( str == KESyncSecond)
	  syncmode = MAnimGeneralFunctions::ESyncSecond ;
   else if ( str == KESyncMinute)
	  syncmode = MAnimGeneralFunctions::ESyncMinute ;
   else if ( str == KESyncDay)
	  syncmode = MAnimGeneralFunctions::ESyncDay ;
   iIpcArgs.Set(0,TIpcArgs::ENothing);
   TPckgBuf<MAnimGeneralFunctions::TAnimSync> sendPckg(syncmode);
   iIpcArgs.Set(1, &sendPckg);
	   
   INFO_PRINTF1(KLogInfoCmdCommandReply3) ;
   //Send the command to notify the server .
   TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
										 
   //Check the result is 0(Success) or not.
   if (result != KErrNone )  
	  {
	  ERR_PRINTF2(KLogErrCommandReply, result);
	  SetBlockResult (EFail); 
	  }			
   else 
	  {
	  INFO_PRINTF1(_L("SetSync return is okay "));
	  }						 			
   }
 
 void CT_DataAnimForMClass::DoCmdGeneralSync(const TDesC& aSectName , const TInt& datOpcode )
   {
   TInt  expectedsyncmode ;
   TPtrC	str;
   // Get the expected syncmode  from ini file. 
   if ( !GetStringFromConfig(aSectName,KFldExpected ,str))
	  {
	  ERR_PRINTF2(KLogMissingParameter , &KFldExpected);
	  SetBlockResult(EFail);		
	  } 
   if (str  ==  KESyncNone )
	  expectedsyncmode = MAnimGeneralFunctions::ESyncNone ;
   else if ( str ==  KESyncFlash)
	  expectedsyncmode = MAnimGeneralFunctions::ESyncFlash ;
   else if ( str == KESyncSecond)
	  expectedsyncmode = MAnimGeneralFunctions::ESyncSecond ;
   else if ( str == KESyncMinute)
	  expectedsyncmode = MAnimGeneralFunctions::ESyncMinute ;
   else if ( str == KESyncDay)
	  expectedsyncmode = MAnimGeneralFunctions::ESyncDay ;
	  
   iIpcArgs.Set( 0, TIpcArgs::ENothing);
   INFO_PRINTF1(KLogInfoCmdCommandReply3) ;
   //Send the command to notify the server .
   TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
										 
   //Check the command return code  . 
   if ( result != expectedsyncmode )
	   {
	   ERR_PRINTF2( KLogErrCommandReply, result);	
	   SetBlockResult(EFail); 
	   }
   else
	   {
	   INFO_PRINTF1(_L("GeneralSync invoked successfully"));	 
	   }
   }
 
 void CT_DataAnimForMClass::DoCmdGeneralSetInterval(const TDesC& aSectName ,  const TInt& datOpcode)
   {
   // Get the interval from ini file and set it as IpcArg 1. 
   TInt  interval  ;
   if ( !GetIntFromConfig(aSectName,KFldInterval ,interval))
	  {
	  ERR_PRINTF2(KLogMissingParameter , &KFldInterval);
	  SetBlockResult(EFail);		
	  }
   iIpcArgs.Set(0,TIpcArgs::ENothing);
   TPckgBuf<TInt> sendPckg(interval) ;
   iIpcArgs.Set(1, &sendPckg );		  
   INFO_PRINTF1(KLogInfoCmdCommandReply3) ;
   //Send the command to notify the server .
   TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
		  
   //Check the result is 0(Success) or not.
   if ( result != KErrNone )
	  {
	  ERR_PRINTF2( KLogErrCommandReply, result);	
	  SetBlockResult(EFail); 
	  }
   else
	  {
	  INFO_PRINTF1(_L("GeneralSetInterval invoked successfully"));
	  }
   }	
  
 void CT_DataAnimForMClass::DoCmdGeneralSetNextInterval(const TDesC& aSectName , const TInt& datOpcode)
   {
   //Read the interval from ini file
   TInt  interval;
   if ( !GetIntFromConfig(aSectName,KFldInterval ,interval))
   {
   ERR_PRINTF2(KLogMissingParameter , &KFldInterval);
   return;
   } 
   TPckgBuf<TInt> sendPckg1(interval);
   
   TInt  nextinterval  ;
   _LIT( KNextInterval , "nextinterval"); 
   //Read the next interval from ini file		
   if ( !GetIntFromConfig(aSectName,KNextInterval ,nextinterval))
	  {
	  ERR_PRINTF2(KLogMissingParameter , &KNextInterval);
	  SetBlockResult(EFail);
	  } 
   TPckgBuf<TInt> sendPckg2(nextinterval) ;
   iIpcArgs.Set(0,TIpcArgs::ENothing);   
   iIpcArgs.Set(1, &sendPckg1);
   iIpcArgs.Set(2, &sendPckg2);
   INFO_PRINTF1(KLogInfoCmdCommandReply3) ;
   //Send the command to notify the server.
   TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
   
   //Check the result is 0(Success) or not.
   if ( result != KErrNone )
	  {
	  ERR_PRINTF2( KLogErrCommandReply, result);	
	  SetBlockResult(EFail); 
	  }
   else
	  {
	  INFO_PRINTF1(_L("GeneralSetNextInterval invoked successfully"));
	  }
   }	
   
   
 void CT_DataAnimForMClass::DoCmdGeneralSystemTime(const TDesC& aSectName , const TInt& datOpcode)
   {
   MAnimGeneralFunctions::TAnimSync syncmode;
   TPtrC str;
   _LIT(KExpected , "expected");
   _LIT( KSyncMode , "animsync");
   // Get the syncmode from ini file and set it as IpcArg 1.
   if ( !GetStringFromConfig(aSectName,KSyncMode ,str))
	   {
	   ERR_PRINTF2(KLogMissingParameter , &KSyncMode);
	   SetBlockResult(EFail);
	   }
   if (str == KESyncNone )
	   syncmode = MAnimGeneralFunctions::ESyncNone;
   else if ( str == KESyncFlash)
	   syncmode = MAnimGeneralFunctions::ESyncFlash;
   else if ( str == KESyncSecond)
	   syncmode = MAnimGeneralFunctions::ESyncSecond;
   else if ( str == KESyncMinute)
	   syncmode = MAnimGeneralFunctions::ESyncMinute;
   else if ( str == KESyncDay)
	   syncmode = MAnimGeneralFunctions::ESyncDay;
   iIpcArgs.Set(0,TIpcArgs::ENothing);
   TPckgBuf<MAnimGeneralFunctions::TAnimSync> sendPckg(syncmode);
   iIpcArgs.Set(1, &sendPckg);
   INFO_PRINTF1(KLogInfoCmdCommandReply3);
   TInt result = iAnim->CommandReply(datOpcode, iSendBuf , iIpcArgs );
   TBool expected;
   if ( !GetBoolFromConfig(aSectName,KExpected ,expected))
	   {
	   ERR_PRINTF2(KLogMissingParameter , &KExpected);
	   return;
	   }
   if(result != static_cast<TInt>(expected))
	   {
	   ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expected, result);
	   SetBlockResult (EFail);
	   }
   else
	   {
	   INFO_PRINTF1(_L("GeneralSystemTime invoked is okay."));
	   }
   }	 
   
void CT_DataAnimForMClass::DoCmdGeneralRegisterForNotis(const TDesC& aSectName , const TInt& datOpcode)
   {
   TInt notifications;
   TEventCode expectedCode;
   // Get the notifications value from ini file and set it as IpcArg 1. 
   if ( !GetIntFromConfig(aSectName,KFldNotifications ,notifications))
	  {
	  ERR_PRINTF2(KLogMissingParameter, &KFldNotifications);
	  SetBlockResult(EFail);
	  }
	
   if(!CT_GraphicsUtil::ReadEventCode(*this, aSectName, KFldExpected, expectedCode))
	  {
	  ERR_PRINTF2(KLogMissingParameter , &KFldExpected);
	  return;
	  }
	
   TPckgBuf<TInt> sendPckg(notifications);
   TPckgBuf<TEventCode> sendPckg2(expectedCode);
   iIpcArgs.Set(0, TIpcArgs::ENothing );
   iIpcArgs.Set(1, &sendPckg );
   iIpcArgs.Set(2, &sendPckg2);
	
   INFO_PRINTF1(KLogInfoCmdCommandReply3);
   //Send the command to notify the server .
   TInt result = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	
   //Check the result is 0(Success) or not.
   if ( result != KErrNone )
	  {
	  ERR_PRINTF2( KLogErrCommandReply, result);
	  SetBlockResult(EFail);
	  }
   else
	  {
	  INFO_PRINTF1(_L("RegisterForNotis invoked successfully"));
	  }
   }	 
   
   
void CT_DataAnimForMClass::DoCmdGeneralMessage(const TDesC& aSectName ,const TInt& datOpcode)
   {
   TPtrC	str , expectedStr;
   TBuf<100>  returnStr;
   // Get the  sending message  value from ini file and set it as IpcArg 1. 
   if ( !GetStringFromConfig(aSectName,KFldMessage ,str))
 	   {
 	   ERR_PRINTF2(KLogMissingParameter , &KFldMessage);
 	   SetBlockResult(EFail);	
 	   } 
   // Get the  expected message  value from ini file .     
   if ( !GetStringFromConfig(aSectName,KFldExpected ,expectedStr))
 	   {
 	   ERR_PRINTF2(KLogMissingParameter , &KFldExpected);
 	   SetBlockResult(EFail);		
 	   } 
   iIpcArgs.Set(0, TIpcArgs::ENothing );
   iIpcArgs.Set(1, &str);
   iIpcArgs.Set(2, &returnStr);
	    
   INFO_PRINTF1(KLogInfoCmdCommandReply3);
   //Send the command to notify the server .
   TInt result = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	  
   if(expectedStr.Compare(returnStr)) 
	   {
	   ERR_PRINTF3(_L("The returned value is not as expected, expected: %S, actual: %S"), &expectedStr, &returnStr);
	   SetBlockResult (EFail);
	   }
   else 
	   {
	   INFO_PRINTF1(_L("GeneralMessage invoked successfully"));
	   }
   ResetIpcArg();
   }	
 	 
  
void CT_DataAnimForMClass::DoCmdGeneralDuplicateBitmapL(const TDesC& aSectName , const TInt& datOpcode)
   {
   TPtrC str ;
   TInt bmHandle;
   CFbsBitmap* bmp;
   // Get the object name of CFbsBitmap from ini file.
   if ( !GetStringFromConfig(aSectName,KFldHandleBitmap,str))
		{
	 	//giving a non-existed value to take the nagative case.
	 	bmHandle = 9999; 
	 	} 
   else
		{
		bmp =  static_cast<CFbsBitmap*>(GetDataObjectL(str));
		bmHandle = bmp->Handle();	
		}
	 		    
   iIpcArgs.Set(0, TIpcArgs::ENothing );
   TPckgBuf<TInt> sendPckg(bmHandle);
   iIpcArgs.Set(1, &sendPckg ); 
   //Send the command to notify the server 	    
   INFO_PRINTF1(KLogInfoCmdCommandReply3);
   iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	 
   //Print info
   _LIT(KSuccess,"GeneralDuplicateBitmapL invoked successfully");   
   INFO_PRINTF1(KSuccess);	
   ResetIpcArg();
   }	
 	 
 	 
void CT_DataAnimForMClass::DoCmdGeneralDuplicateFontL(const TDesC& aSectName ,  const TInt& datOpcode)
   {
   TPtrC str;
   TInt fntHandle;
   CFbsFont* fnt;
   TInt typeId;
   // Get the object name of CFbsFont from ini file.
   if ( !GetStringFromConfig(aSectName,KFldHandleFont,str))
	   {
	   //giving a non-existed value to take the nagative case.
	   fntHandle = 9999;
	   }
   else
	   {
	   fnt = static_cast<CFbsFont*>(GetDataObjectL(str));
	   typeId = fnt->TypeUid().iUid; //this is the expected value.
	   fntHandle = fnt->Handle();
	   }
   TInt result = 0;
   TPckgBuf<TInt> sendPckg(fntHandle);
   TPckgBuf<TInt> getPckg(result);

   iIpcArgs.Set(0, TIpcArgs::ENothing );
   iIpcArgs.Set(1, &sendPckg);
   iIpcArgs.Set(2, &getPckg);
   INFO_PRINTF1(KLogInfoCmdCommandReply3);
   //Send the command to notify the server .
   iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
   result = getPckg();
	
   //Check the result 
   if(result == typeId)
	   {
	   INFO_PRINTF1(_L("GeneralDuplicateFontL invoked successfully"));
	   }
   else
	   {
	   ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), typeId, result);
	   SetError(KErrNoMemory);
	   }
   ResetIpcArg();
   }		 
  
  
void CT_DataAnimForMClass::DoCmdGeneralCloseFontL(const TDesC& aSectName , const TInt& datOpcode)
   {
   TPtrC str ;
   // Get the object name of CFbsFont from ini file.
   if ( !GetStringFromConfig(aSectName,KFldHandleFont,str))
	   {
	   ERR_PRINTF2(KLogMissingParameter , &KFldHandleFont);
	   SetBlockResult(EFail);		
	   } 
   CFbsFont* fnt =  static_cast<CFbsFont*>(GetDataObjectL(str));
   TInt typeId = fnt->TypeUid().iUid; //this is the expected value .
   TInt fntHandle = fnt->Handle();
   TInt result = 0;
   TPckgBuf<TInt> sendPckg(fntHandle); 
   TPckgBuf<TInt> getPckg(result);
		  
   iIpcArgs.Set(0, TIpcArgs::ENothing );
   iIpcArgs.Set(1, &sendPckg ); 
   iIpcArgs.Set(2, &getPckg);
   INFO_PRINTF1(KLogInfoCmdCommandReply3);
   iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
   result = getPckg();
   if(result == typeId)
	   {
	   _LIT(KSuccess,"GeneralDuplicateFontL invoked successfully");   
	   INFO_PRINTF1(KSuccess);
	   }
   else
	   {
	   ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), typeId, result);
	   SetBlockResult (EFail);
	   }
   ResetIpcArg();
   } 

void CT_DataAnimForMClass::DoCmdGeneralReplyBuf8(const TDesC& aSectName, const TInt& datOpcode)
   {
   TPtrC str2;

	 if ( !GetStringFromConfig(aSectName,KFldSetReplyBufHandle1,str2))
	 	 {
	 	 ERR_PRINTF2(KLogMissingParameter , &KFldSetReplyBufHandle1);
	 	 SetBlockResult(EFail);		
	 	 } 
	 TBuf8<100> slot2;
	 slot2.Append(str2);
	 
	 TPtrC str3;

	 if ( !GetStringFromConfig(aSectName,KFldSetReplyBufHandle2,str3))
	 	 {
	 	 ERR_PRINTF2(KLogMissingParameter , &KFldSetReplyBufHandle2);
	     return ;				
	 	 } 
	 TBuf8<100> slot3;
	 slot3.Append(str3);
	 
	 TBuf8<100> result;
	 	  
	 iIpcArgs.Set(0, TIpcArgs::ENothing );
	 iIpcArgs.Set(1, &result); 
	 iIpcArgs.Set(2, &slot2);
	 iIpcArgs.Set(3, &slot3);
	 INFO_PRINTF1(KLogInfoCmdCommandReply3);
	 iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	 
	 if(!result.Compare(slot3) && result.Compare(slot2))
		 {
		 _LIT(KSuccess,"GeneralDuplicateFontL invoked successfully");   
		 INFO_PRINTF1(KSuccess);
		 }
	 else
		 {
		 ERR_PRINTF2(_L("The returned value is not as expected, result: %S"), &result);
		 SetBlockResult (EFail);
		 }
	 ResetIpcArg();
	}

void CT_DataAnimForMClass::DoCmdGeneralReplyBuf16(const TDesC& aSectName, const TInt& datOpcode)
	{
	TPtrC str2;

	if ( !GetStringFromConfig(aSectName,KFldSetReplyBufHandle1,str2))
		 {
	 	 ERR_PRINTF2(KLogMissingParameter , &KFldSetReplyBufHandle1);
	 	 SetBlockResult(EFail);			
	 	 } 
	TBuf16<100> slot2(str2);
	 
	TPtrC str3;

	if ( !GetStringFromConfig(aSectName,KFldSetReplyBufHandle2,str3))
		 {
	 	 ERR_PRINTF2(KLogMissingParameter , &KFldSetReplyBufHandle2);
	     return ;				
	 	 } 
	TBuf16<100> slot3(str3);
	 
	TBuf16<100> result;
	 	  
	iIpcArgs.Set(0, TIpcArgs::ENothing );
	iIpcArgs.Set(1, &result); 
	iIpcArgs.Set(2, &slot2);
	iIpcArgs.Set(3, &slot3);
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	 
	if(!result.Compare(slot3) && result.Compare(slot2))
		{
		_LIT(KSuccess,"GeneralDuplicateFontL invoked successfully");   
		INFO_PRINTF1(KSuccess);
		}
	else
		{
		ERR_PRINTF2(_L("The returned value is not as expected, result: %S"), &result);
		SetBlockResult (EFail);
		}
	ResetIpcArg();
	}
//-------------------------------------------------The MAnimSpriteFunctions ----------------------------------------
void CT_DataAnimForMClass::DoCmdSpriteGetSpriteMember(const TDesC& aSectName , const TInt& datOpcode)
	{
	TInt memberindex ;
    // Get the sprite member index from ini file and set it as IpcArg 1 .
	if ( !GetIntFromConfig(aSectName,KFldMemberIndex ,memberindex))
		{
		ERR_PRINTF2(KLogMissingParameter , &KFldMemberIndex);
		SetBlockResult(EFail);	
		} 
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	TPckgBuf<TInt> pckg(memberindex);
	TPckgBuf<TPoint> returnPckg;
	iIpcArgs.Set(1,&pckg);
	iIpcArgs.Set(2,&returnPckg);
	
	// Send the command to notify the server .
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
		  
    TPoint result=returnPckg();
	// Verify the result and the expected .
    TPtrC		spritename;

    TPoint  expectedPoint;
    // Get the expected point value from ini file.
    if ( !GetPointFromConfig(aSectName, KFldExpectedPoint, expectedPoint))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpectedPoint);
		return;
		}
    
	if ( result !=  expectedPoint)
	    { 	   
        ERR_PRINTF3(_L("The returned value is not as expected, expected.x: %d, expect.y: %d"), expectedPoint.iX , expectedPoint.iY);        
	    ERR_PRINTF3(_L("The returned value is not as expected, result.x: %d, result.y: %d"), result.iX , result.iY); 
        SetBlockResult (EFail);
	    }  
	else 
	    { 
	    INFO_PRINTF1(_L("Get Sprite Member successfully"));
	    } 
	ResetIpcArg();
    } 

void CT_DataAnimForMClass::DoCmdSpriteUpdateMember(const TDesC& aSectName , const TInt& datOpcode)
	{ 
    TInt memberindex ;
	TRect rect ;
	TBool isfullupdate ;
    //Get the sprite member index from ini file and set it as IpcArg 1.  
	if ( ! GetIntFromConfig(aSectName,KFldMemberIndex ,memberindex))
		 {
		 ERR_PRINTF2(KLogMissingParameter , &KFldMemberIndex);
		 SetBlockResult(EFail);		
		 }
    //Get the  rect value from ini file and set it as IpcArg 2.  
	if ( ! GetRectFromConfig(aSectName,KFldRect ,rect))
		 {
		 ERR_PRINTF2(KLogMissingParameter , &KFldRect);
		 SetBlockResult(EFail);			
		 }
    //Get the  isfullupdate flag from ini file and set it as IpcArg 3.  
	if ( ! GetBoolFromConfig(aSectName,KFldIsFullUpdate ,isfullupdate))
		 {
		 ERR_PRINTF2(KLogMissingParameter , &KFldIsFullUpdate);
		 SetBlockResult(EFail);		
		 }
	
	TPckgBuf<TInt> memberPckg(memberindex);
	TPckgBuf<TRect> rectPckg(rect);
	TPckgBuf<TBool> updatePckg(isfullupdate);
	
	
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1, &memberPckg);
	iIpcArgs.Set(2, &rectPckg);
	iIpcArgs.Set(3, &updatePckg);
		    
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
    //Send the command to notify the server .
	TInt result = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
		  
	//Check the result is 0(Success) or not.
	if (result != KErrNone )  
		{
		ERR_PRINTF2(KLogErrCommandReply, result);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("UpdateSpriteMember return is okay "));
		}
	}
   

void CT_DataAnimForMClass::DoCmdSpriteActivate(const TDesC& aSectName , const TInt& datOpcode)
	{
	TBool isactive;
	// Get the isactive flag from ini file and set as IpcArg 1. 
	if ( !GetBoolFromConfig(aSectName,KFldIsActivate ,isactive))
		{
		ERR_PRINTF2(KLogMissingParameter , &KFldIsActivate);
		SetBlockResult(EFail);		
		}
	TPckgBuf<TBool> sendPkg(isactive) ;	  
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1,&sendPkg);
		    
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
    //Send the command to notify the server .
	TInt result = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
		  
	//Check the result is 0(Success) or not.
	if (result != KErrNone )  
	    {
		ERR_PRINTF2(KLogErrCommandReply, result);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("SpriteActive return is okay "));
		}
	}
   
void CT_DataAnimForMClass::DoCmdSpriteSpriteCanBeSeen(const TDesC& aSectName , const TInt& datOpcode)
	{
	TBool expected;
	// Get the expected boolean value from the ini file.
	if ( !GetBoolFromConfig(aSectName, KFldExpected, expected))
		 {
		 ERR_PRINTF2(KLogMissingParameter , &KFldExpected);
		 SetBlockResult(EFail);			
		 } 
	TPckgBuf<TBool> resultPkg(EFalse) ;
		  
		  
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1,&resultPkg);
		    
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	//Send the command to notify the server .
	iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
		  
	//Verify the result and the expected 
	TBool result = resultPkg() ; 
		  
	if ( result != expected )
	  {
	  ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expected, (TInt)result);
      SetBlockResult (EFail);
	  }
	else 
	  {
	  INFO_PRINTF1(_L("SpriteCanBeSeen return is okay "));
	  }
	ResetIpcArg();
	}

void CT_DataAnimForMClass::DoCmdSpriteSizeChangedL(const TDesC& /*aSectName*/ ,  const TInt& datOpcode)
	{
	//Send the command to notify the server .
	iIpcArgs.Set(0, TIpcArgs::ENothing);
		  
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
	TInt result = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
	
	//Check the result is 0(Success) or not.
	if (result != KErrNone )  
	    {
		ERR_PRINTF2(KLogErrCommandReply, result);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("SpriteSizeChangedL return is okay"));
		}	
	}
   
void CT_DataAnimForMClass::DoCmdSpriteSetPosition(const TDesC& aSectName ,  const TInt& datOpcode)
	{
    // Get the point value from ini file and set as IpcArg 1. 
	TPoint point ;
	if ( !GetPointFromConfig(aSectName, KFldPointPosition, point))
		 {
		 ERR_PRINTF2(KLogMissingParameter , &KFldPointPosition);
		 SetBlockResult(EFail);
		 }
	TPckgBuf<TPoint> pkg(point) ;
	iIpcArgs.Set(0, TIpcArgs::ENothing);
	iIpcArgs.Set(1, &pkg) ;  
	INFO_PRINTF1(KLogInfoCmdCommandReply3);
    //Send the command to notify the server .
	TInt result = iAnim->CommandReply(datOpcode,iSendBuf,iIpcArgs);
		  
	//Check the result is 0(Success) or not.
	if (result != KErrNone )  
	    {
		ERR_PRINTF2(KLogErrCommandReply, result);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("SpriteSetPosition return is okay"));
		}
	}      

void CT_DataAnimForMClass::DoCmdEatupMemory(const TInt& datOpcode)
	{
	TInt result = iAnim->CommandReply(datOpcode);
	if (result != KErrNone )  
		{
		ERR_PRINTF2(KLogErrCommandReply, result);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("EatupMemory return is okay"));
		}
	}

void CT_DataAnimForMClass::DoCmdFreeEatenMemory(const TInt& datOpcode)
	{
	TInt result = iAnim->CommandReply(datOpcode);
	if (result != KErrNone )  
		{
		ERR_PRINTF2(KLogErrCommandReply, result);
		SetBlockResult (EFail); 
		}			
	else 
		{
		INFO_PRINTF1(_L("FreeEatenMemory return is okay"));
		}
	}

