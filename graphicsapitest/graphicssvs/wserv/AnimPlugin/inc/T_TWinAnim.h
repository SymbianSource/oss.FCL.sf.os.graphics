// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
// 
// Description:
//

#ifndef WINANIM_H
#define WINANIM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "w32adll.h" 
#include "w32std.h" 
#include "T_AnimProc.h"


// CLASS DECLARATION

/**
*  MTimeoutNotify 
* 
*/
class MTimeoutNotify 
    {
    public:

		/**
		* TimedOut
		* Handle the event when a time out occurs.
		*/
        virtual void TimedOut() = 0;
    };

/**
* CTimeOutTimer
*/
class CTimeOutTimer : public CTimer
    {
    public: // Constructors and destructor
       
        static CTimeOutTimer* NewL( const TInt aPriority, 
                                    MTimeoutNotify& aTimeOutNotify );
        virtual ~CTimeOutTimer();

    public: // From CTimer
        void RunL();

    protected: // Constructor
        CTimeOutTimer( const TInt aPriority, MTimeoutNotify& aTimeOutNotify );

    protected: // Constructor
        void ConstructL();

    private:
        MTimeoutNotify& iNotify;
    };

/**
*  CWinAnim
* 
*/

const TInt KBufMessage = 200;

class CTestAnimDll;

class CTWinAnim : public CWindowAnim, CAnimProc , public MTimeoutNotify
{
public: // Constructors and destructor

	/**
		* Destructor.
		*/
	~CTWinAnim();

	/**
		* Constructor for performing 1st stage construction
		*/
	CTWinAnim(CTestAnimDll* aDll, TInt aType);
	
	virtual void ConstructL(TAny *aArgs, TBool aHasFocus);
	virtual void Redraw();
	virtual void FocusChanged(TBool aState);
	virtual TInt CommandReplyL(TInt aOpcode, TAny *aArgs);
	virtual void Command(TInt aOpcode, TAny *aArgs);
	virtual void Animate(TDateTime *aDateTime);
	virtual TBool OfferRawEvent(const TRawEvent &aRawEvent);
	virtual void HandleNotification(const TWsEvent &aEvent);
public:
	virtual void TimedOut();
	
private:
	void Draw();
	void Compare();

//for M class testing	
private:
	//for MAnimWindowFunctions
	TInt DoCmdSetRect();
	TInt DoCmdWindowSize();
	TInt DoCmdSetVisible();
	TInt DoCmdInvalidate();
	TInt DoCmdParameters();
	TInt DoCmdVisibleRegion();
	//for MAnimGeneralFunctionsWindowExtension
	TInt DoCmdSetFocusScreen();
	TInt DoCmdWindowGroups();
	TBool DoCmdWindowGroupInfo();
	TBool DoCmdWindowGroupName();
	TInt DoCmdSetOrdinalPosition();
	TBool DoCmdIsFocusable();
	TBool DoCmdWindowConfig();
	//for MAnimGeneralFunctions
	TInt DoCmdDuplicateBitmapL();
	TInt DoCmdDuplicateFontL();
	TInt DoCmdCloseFontL();
	TInt DoCmdSetIntervalL();
	TInt DoCmdSetNextIntervalL();
	TInt DoCmdSystemTimeL();
	TInt DoCmdRegisterForNotisL();
	TInt DoCmdMessageL();
	TInt DoCmdAnimateL();
	TInt DoCmdFlashStateOn();
	TInt DoCmdPanic();
	TInt DoCmdScreenDevice();
	TInt DoCmdWindowExtension();
	TInt DoCmdEventExtension();
	TInt DoCmdExtendedInterfaceL();
	TInt DoCmdNumOfExtInterfaces();
	TInt DoCmdSetSyncL();
	TInt DoCmdSync();
	TInt DoCmdGetRawEventsL();
	TInt DoCmdPostRawEventL();
	TInt DoCmdPostKeyEventL();
	TInt DoCmdClient();
	TInt DoCmdGeneralReplyBuf8L();
	TInt DoCmdGeneralReplyBuf16L();
	
	//Util
	TInt DoCmdEatupMemory();
	TInt DoCmdFreeEatenMemory();
	void FreeMemory();
	
private:
	CTimeOutTimer* iServerTimer;
	MAnimGeneralFunctions::TAnimSync iSyncMode;
	TInt iCurrentCommand;
	CFbsBitmap* iDuplicateBitmap;
	CFbsFont* iDuplicateFont;
	TDateTime iDateTime;
	TInt iResult;
	TDateTime iAnimateTime;
	TInt64* iIntervalTimeStamp;
	TInt iInterval;
	TInt iNextInterval;	
	TEventCode iExpectEventCode;
	TInt iAvaiableSpace;
	TInt iBiggestBlock;
	RPointerArray < TAny > iMemArray;
};

#endif // WINANIM_H

