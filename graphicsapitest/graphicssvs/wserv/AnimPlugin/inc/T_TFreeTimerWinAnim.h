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


#ifndef CFREETIMERWINANIM_H
#define CFREETIMERWINANIM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32adll.h>
// CLASS DECLARATION

/**
 * CTFreeTimerWinAnim
 *
 */
class CTFreeTimerWinAnim : public CFreeTimerWindowAnim
	{
public:
	CTFreeTimerWinAnim();
	~CTFreeTimerWinAnim();

	//from CAnim
	virtual TInt CommandReplyL(TInt aOpcode, TAny *aArgs);
	virtual void Command(TInt aOpcode, TAny *aArgs);
	virtual void Animate(TDateTime *aDateTime);
	//from MEventHandler
	virtual TBool OfferRawEvent(const TRawEvent &aRawEvent);
	//from CWindowAnim
	virtual void ConstructL(TAny *aArgs, TBool aHasFocus);
	virtual void Redraw();
	virtual void FocusChanged(TBool aState);
	
private:
	TBool iResult;
	TInt iCurrentCommand;
	TRect iScreenSize;		// The screen size
	}; // class CTFreeTimerWinAnim

#endif // CFREETIMERWINANIM_H
