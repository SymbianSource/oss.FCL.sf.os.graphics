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

#include "T_AnimDef.h"

#include "T_TFreeTimerWinAnim.h"

CTFreeTimerWinAnim::CTFreeTimerWinAnim()
	{
	iResult=EFalse;
	iCurrentCommand=ECmdNone;
	}

CTFreeTimerWinAnim::~CTFreeTimerWinAnim()
	{
	}

//from CWindowAnim
void CTFreeTimerWinAnim::ConstructL(TAny */*aArgs*/, TBool /*aHasFocus*/)
	{
	 // Store the screen size
	 iScreenSize = iWindowFunctions->WindowSize();
	    
	 // Set the screen visible
	 iWindowFunctions->SetVisible( ETrue );	
	 //just for start heartbeat
	 iWindowFunctions->SetRect(TRect(0,0,1,1));
	}

void CTFreeTimerWinAnim::Redraw()
	{	
	if(iCurrentCommand==ECmdFreeTimerWinUpdate)
		{
		iResult=ETrue;
		}
	}

void CTFreeTimerWinAnim::FocusChanged(TBool /*aState*/)
	{
	
	}

//from CAnim
TInt CTFreeTimerWinAnim::CommandReplyL(TInt aOpcode, TAny */*aArgs*/)
	{
	iCurrentCommand=aOpcode;
	TInt nReturn=KErrNone;
	switch(aOpcode)
		{
		case ECmdFreeTimerWinDeactivateGc:
			{
			TRect rect(10,10,20,20);
			TInt err;
			iWindowFunctions->ActivateGc();
			TRAP(err,iGc->DrawRect(rect));
			if(KErrNone == err)
				{
				WindowFunctions()->DeactivateGc();
				TRAP(err,iGc->DrawRect(rect));	
				if(KErrGeneral != err)
					{
					nReturn = err;
					}
				}
			else
				{
				nReturn = err;
				}
			}
			break;
		case ECmdFreeTimerWinUpdate:
			{
			iResult=EFalse;
			WindowFunctions()->Update();
			}
			break;
		case ECmdRetrieveResult:
			nReturn=iResult;
			break;
		}
	return nReturn;
	}

void CTFreeTimerWinAnim::Command(TInt /*aOpcode*/, TAny */*aArgs*/)
	{
	
	}
void CTFreeTimerWinAnim::Animate(TDateTime */*aDateTime*/)
	{
	
	}

//from MEventHandler
TBool CTFreeTimerWinAnim::OfferRawEvent(const TRawEvent &/*aRawEvent*/)
	{
	return EFalse;
	}

