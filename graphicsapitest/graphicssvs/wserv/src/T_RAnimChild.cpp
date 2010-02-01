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


#include "T_RAnimChild.h"

T_RAnimChild::T_RAnimChild()
{
}
T_RAnimChild::T_RAnimChild(RAnimDll &aDll):RAnim(aDll)
{
	// No implementation required
}

T_RAnimChild::~T_RAnimChild()
{
}

TInt T_RAnimChild::Construct(const RWindowBase &aDevice, TInt aType, const TDesC8 &aParams)
	{
	return RAnim::Construct(aDevice,  aType,  aParams);
	}
TInt T_RAnimChild::Construct(const RWindowBase &aDevice, TInt aType, const TDesC8 &aParams, const TIpcArgs& aIpcArgs)
	{
	return RAnim::Construct(aDevice, aType, aParams, aIpcArgs);
	}
TInt T_RAnimChild::Construct(const RWsSprite &aDevice, TInt aType, const TDesC8 &aParams)
	{
	return RAnim::Construct(aDevice, aType, aParams);
	}
TInt T_RAnimChild::Construct(const RWsSprite &aDevice, TInt aType, const TDesC8 &aParams, const TIpcArgs& aIpcArgs)
	{
	return RAnim::Construct(aDevice, aType, aParams, aIpcArgs);
	}
TInt T_RAnimChild::CommandReply(TInt aOpcode)
	{
	return RAnim::CommandReply(aOpcode);
	}
TInt T_RAnimChild::CommandReply(TInt aOpcode, const TPtrC8 &aArgs)
	{
	return RAnim::CommandReply(aOpcode,  aArgs);
	}
TInt T_RAnimChild::CommandReply(TInt aOpcode, const TDesC8 &aArgs, const TIpcArgs &aIpcArgs)
	{
	return RAnim::CommandReply( aOpcode,  aArgs,aIpcArgs);
	}
void T_RAnimChild::Command(TInt aOpcode, const TPtrC8 &aArgs)
	{
	RAnim::Command( aOpcode,  aArgs);
	}
void T_RAnimChild::Command(TInt aOpcode)
	{
	RAnim::Command(aOpcode);
	}
void T_RAnimChild::AsyncCommandReply(TRequestStatus& aRequestStatus,TInt aOpcode, const TIpcArgs& aIpcArgs)
	{
	RAnim::AsyncCommandReply( aRequestStatus, aOpcode,   aIpcArgs);
	}
