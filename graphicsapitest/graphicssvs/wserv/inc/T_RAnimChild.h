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


#ifndef TESTANIM_H
#define TESTANIM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "w32std.h" 

// CLASS DECLARATION

/**
*  CTestAnim
* 
*/
class T_RAnimChild : public RAnim
{
public: // Constructors and destructor
	T_RAnimChild();
	T_RAnimChild(RAnimDll &aDll);	
	virtual ~T_RAnimChild();

	TInt Construct(const RWindowBase &aDevice, TInt aType, const TDesC8 &aParams);
	TInt Construct(const RWindowBase &aDevice, TInt aType, const TDesC8 &aParams, const TIpcArgs& aIpcArgs);
	TInt Construct(const RWsSprite &aDevice, TInt aType, const TDesC8 &aParams);
	TInt Construct(const RWsSprite &aDevice, TInt aType, const TDesC8 &aParams, const TIpcArgs& aIpcArgs);
	TInt CommandReply(TInt aOpcode);
	TInt CommandReply(TInt aOpcode, const TPtrC8 &aArgs);
	TInt CommandReply(TInt aOpcode, const TDesC8& aArgs, const TIpcArgs& aIpcArgs);
	void Command(TInt aOpcode, const TPtrC8 &aArgs);
	void Command(TInt aOpcode);
	void AsyncCommandReply(TRequestStatus& aRequestStatus,TInt aOpcode, const TIpcArgs& aIpcArgs);
};

#endif // TESTANIM_H

