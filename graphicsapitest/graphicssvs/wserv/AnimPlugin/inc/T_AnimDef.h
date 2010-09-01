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
#ifndef ANIMDEF_H_
#define ANIMDEF_H_


//Anim type
#define EKWindowAnim 			1
#define EKSpriteAnim          	2
#define EKFreeTimerWindowAnim 	3


//command for RAnim::Command
#define ECmdSync   	    		1

#define ECmdTestWithoutPara    	100
#define ECmdTestWithPara    	101
#define ECmdGetLast    	    	102
#define ECmdGetAnimInterval     103

enum TCommandsForAnimMClass
	{
	ECmdNone = 150,
	ECmdRetrieveResult,
	ECmdGeneralAnimate,
	ECmdGeneralSetSync,
	ECmdGeneralSync,
	ECmdGeneralSystemTime,
	ECmdGeneralSetInterval,
	ECmdGeneralSetNextInterval,
	ECmdGeneralClient,
	ECmdGeneralScreenDevice,
	ECmdGeneralEventExtension,
	ECmdGeneralWindowExtension,
	ECmdGeneralExtendedInterface,
	ECmdGeneralNumOfExtInterfaces,
	ECmdGeneralPanic,
	ECmdGeneralReplyBuf,
	ECmdGeneralFlashStateOn,
	ECmdGeneralDuplicateBitmapL,
	ECmdGeneralDuplicateFontL,
	ECmdGeneralCloseFont,
	ECmdGeneralGetRawEvents,
	ECmdGeneralPostKeyEvent,
	ECmdGeneralPostRawEvent,
	ECmdGeneralMessage,
	ECmdGeneralRegisterForNotis,
	ECmdSpriteGetSpriteMember,
	ECmdSpriteUpdateMember,
	ECmdSpriteActivate, 
	ECmdSpriteSpriteCanBeSeen,
	ECmdSpriteSizeChangedL,
	ECmdSpriteSetPosition,
	ECmdGfweFocusScreens,
	ECmdGfweScreens,
	ECmdGfweSetFocusScreen,
	ECmdGfweSetOrdinalPosition,
	ECmdGfweWindowGroupInfo,
	ECmdGfweWindowGroupName,
	ECmdGfweWindowGroups,
	ECmdGfweIsFocusable,
	ECmdGfweWindowConfig,
	ECmdWindowActivateGc,
	ECmdWindowInvalidate,
	ECmdWindowParameters,
	ECmdWindowIsHidden,
	ECmdWindowSetRect,
	ECmdWindowSetVisible,
	ECmdWindowVisibleRegion,
	ECmdWindowWindowSize,
	ECmdFreeTimerWinDeactivateGc,
	ECmdFreeTimerWinUpdate,
	ECmdWindowIsStarted,
	ECmdReceiveResult,
	ECmdGeneralReplyBuf8,
	ECmdGeneralReplyBuf16,
	
	ECmdUtilEatupMemory,
	ECmdUtilFreeEatenMemory
};

#endif /*ANIMDEF_H_*/
