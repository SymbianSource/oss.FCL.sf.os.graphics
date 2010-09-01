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

#include "T_TWinAnim.h"
#include "T_TestAnimDll.h"
#include "txtfrmat.h" 
#include "bitdev.h" 
#include "T_AnimDef.h"
#include "w32adll.h" 

// Implementation of CTimeoutTimer.
CTimeOutTimer::CTimeOutTimer(const TInt aPriority,
		MTimeoutNotify& aTimeOutNotify) :
	CTimer(aPriority), iNotify(aTimeOutNotify)
	{
	// No implementation required
	}

CTimeOutTimer* CTimeOutTimer::NewL(const TInt aPriority, 
		MTimeoutNotify& aTimeOutNotify)
	{
	CTimeOutTimer *self = new ( ELeave ) CTimeOutTimer( aPriority, aTimeOutNotify );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CTimeOutTimer::~CTimeOutTimer()
	{
	Cancel();
	}

void CTimeOutTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add( this);
	}

void CTimeOutTimer::RunL()
	{
	iNotify.TimedOut();
	}

//Implementation of CTWinAnim
CTWinAnim::CTWinAnim(CTestAnimDll* aDll, TInt aType) :
	CAnimProc(aDll, aType)
	{
	// No implementation required
	}

CTWinAnim::~CTWinAnim()
	{
	WriteLog(_L("~CTWinAnim"));
	if (iServerTimer)
		{
		iServerTimer->Cancel();
		}

	// Then delete the timer object
	delete iServerTimer;
	iServerTimer = NULL;

	if (iIntervalTimeStamp)
		delete [] iIntervalTimeStamp;

	if (iDuplicateBitmap)
		delete iDuplicateBitmap;

	if (iMemArray.Count() > 0)
		FreeMemory();
	}

void CTWinAnim::ConstructL(TAny *aArgs, TBool /*aHasFocus*/)
	{
	WriteLog(_L("CTWinAnim::ConstructL"));
	ConstructWinL(aArgs, iFunctions);

	// Store the screen size
	iScreenSize = iWindowFunctions->WindowSize();

	TBuf<50> buf;
	buf.Format(_L("Screen width=%d height=%d"),iScreenSize.Width(),iScreenSize.Height());
	WriteLog(buf);

	// Set the screen visible
	iWindowFunctions->SetVisible(ETrue);
	//just for start heartbeat
	iWindowFunctions->SetRect(TRect(0, 0, 1, 1));
	iServerTimer = CTimeOutTimer::NewL(EPriorityHigh, *this);
	}

void CTWinAnim::Redraw()
	{
	WriteLog(_L("CTWinAnim::Redraw"));
	iResult=ETrue;

	Draw();
	}

void CTWinAnim::Draw()
	{
	// get the background colour
	TLogicalRgb backgroundColour(TLogicalRgb::ESystemBackgroundColor);

	// Clear the old image.
	iGc->SetPenSize(iPenSize);
	iGc->SetPenColor(backgroundColour);
	iGc->DrawRect(TRect(iLastPoint, iSize) );

	// Draw a shape at the new position
	iGc->SetPenSize(iPenSize);
	iGc->SetPenColor(iColor);
	iGc->DrawRect(TRect(iNewPoint, iSize) );

	if (iDuplicateBitmap)
		{
		TRect rect;
		rect.SetRect(0, 0, 200, 200);
		iGc->DrawBitmap(rect, iDuplicateBitmap);
		}

	// signal that the current position has been drawn
	iDrawn = ETrue;
	}

void CTWinAnim::HandleNotification(const TWsEvent &aEvent)
	{
	if (aEvent.Type() == iExpectEventCode)
		{
		iResult = ETrue;
		}
	}

void CTWinAnim::FocusChanged(TBool /*aState*/)
	{

	}

void CTWinAnim::TimedOut()
	{
	switch (iCurrentCommand)
		{
		case ECmdGeneralSetInterval:
			{
			TInt64 interval=iIntervalTimeStamp[1]-iIntervalTimeStamp[0];
			TInt expectInterval=0;
			if(iInterval>0)
				expectInterval = iInterval * 1000000 / 2;

			if (interval == expectInterval)
				iResult=ETrue;
			else
				iResult=EFalse;

			break;
			}

		case ECmdGeneralSetNextInterval:
			{
			TInt64 interval1=iIntervalTimeStamp[1]-iIntervalTimeStamp[0];
			TInt64 interval2=iIntervalTimeStamp[2]-iIntervalTimeStamp[1];
			TInt expectInterval = iInterval * 1000000 / 2;
			TInt expectNextInterval = 0;
			
			//If the value is less than 1, it automatically gets set to 1. 
			if(iNextInterval > 0)
				{
				expectNextInterval = iNextInterval * 1000000 / 2;
				}
			else
				{
				expectNextInterval = 1000000 / 2;
				}
			
			if ((expectNextInterval*5/12)<=interval1 && interval1<=1000000
					&& interval2==expectInterval)
				iResult=ETrue;
			else
				iResult=EFalse;
			break;
			}
		}
	}

TInt CTWinAnim::CommandReplyL(TInt aOpcode, TAny *aArgs)
	{
	if (aOpcode != ECmdRetrieveResult)
		{
		iCurrentCommand=aOpcode;
		}
	TInt nReturn=KErrNone;
	switch (aOpcode)
		{
		case ECmdWindowActivateGc:
			iWindowFunctions->ActivateGc();
			break;
		case ECmdWindowSetRect:
			nReturn=DoCmdSetRect();
			break;
		case ECmdWindowIsStarted:
			iResult=EFalse;
			iSyncMode=iFunctions->Sync();
			iFunctions->SetSync(MAnimGeneralFunctions::ESyncFlash);
			break;
		case ECmdRetrieveResult:
			nReturn=iResult;
			break;
		case ECmdWindowWindowSize:
			nReturn=DoCmdWindowSize();
			break;
		case ECmdWindowIsHidden:
			nReturn=iWindowFunctions->IsHidden();
			break;
		case ECmdWindowSetVisible:
			nReturn=DoCmdSetVisible();
			break;
		case ECmdWindowInvalidate:
			nReturn=DoCmdInvalidate();
			break;
		case ECmdWindowParameters:
			nReturn=DoCmdParameters();
			break;
		case ECmdWindowVisibleRegion:
			nReturn=DoCmdVisibleRegion();
			break;

		case ECmdGfweScreens:
			nReturn=iFunctions->WindowExtension()->Screens();
			break;
		case ECmdGfweFocusScreens:
			nReturn=iFunctions->WindowExtension()->FocusScreens();
			break;
		case ECmdGfweSetFocusScreen:
			nReturn=DoCmdSetFocusScreen();
			break;
		case ECmdGfweWindowGroups:
			nReturn=DoCmdWindowGroups();
			break;
		case ECmdGfweWindowGroupInfo:
			nReturn=DoCmdWindowGroupInfo();
			break;
		case ECmdGfweWindowGroupName:
			nReturn=DoCmdWindowGroupName();
			break;
		case ECmdGfweSetOrdinalPosition:
			nReturn=DoCmdSetOrdinalPosition();
			break;
		case ECmdGfweIsFocusable:
			nReturn=DoCmdIsFocusable();
			break;
		case ECmdGfweWindowConfig:
			nReturn=DoCmdWindowConfig();
			break;
			
		case ECmdGeneralDuplicateBitmapL:
			nReturn = DoCmdDuplicateBitmapL();
			break;
		case ECmdGeneralDuplicateFontL:
			nReturn = DoCmdDuplicateFontL();
			break;
		case ECmdGeneralCloseFont:
			nReturn = DoCmdCloseFontL();
			break;
		case ECmdGeneralSetInterval:
			nReturn = DoCmdSetIntervalL();
			break;
		case ECmdGeneralSetNextInterval:
			nReturn = DoCmdSetNextIntervalL();
			break;
		case ECmdGeneralSystemTime:
			nReturn = DoCmdSystemTimeL();
			break;
		case ECmdGeneralRegisterForNotis:
			nReturn = DoCmdRegisterForNotisL();
			break;
		case ECmdGeneralMessage:
			nReturn = DoCmdMessageL();
			break;
		case ECmdGeneralAnimate:
			nReturn = DoCmdAnimateL();
			break;
		case ECmdGeneralFlashStateOn:
			nReturn = DoCmdFlashStateOn();
			break;
		case ECmdGeneralPanic:
			nReturn = DoCmdPanic();
			break;
		case ECmdGeneralScreenDevice:
			nReturn = DoCmdScreenDevice();
			break;
		case ECmdGeneralWindowExtension:
			nReturn = DoCmdWindowExtension();
			break;
		case ECmdGeneralEventExtension:
			nReturn = DoCmdEventExtension();
			break;
		case ECmdGeneralExtendedInterface:
			nReturn = DoCmdExtendedInterfaceL();
			break;
		case ECmdGeneralNumOfExtInterfaces:
			nReturn = DoCmdNumOfExtInterfaces();
			break;
		case ECmdGeneralSetSync:
			nReturn = DoCmdSetSyncL();
			break;
		case ECmdGeneralSync:
			nReturn = DoCmdSync();
			break;
		case ECmdGeneralGetRawEvents:
			nReturn = DoCmdGetRawEventsL();
			break;
		case ECmdGeneralPostRawEvent:
			nReturn = DoCmdPostRawEventL();
			break;
		case ECmdGeneralPostKeyEvent:
			nReturn = DoCmdPostKeyEventL();
			break;
		case ECmdGeneralClient:
			nReturn = DoCmdClient();
			break;
		case ECmdGeneralReplyBuf8:
			nReturn = DoCmdGeneralReplyBuf8L();
			break;
		case ECmdGeneralReplyBuf16:
			nReturn = DoCmdGeneralReplyBuf16L();
			break;
		case ECmdUtilEatupMemory:
			nReturn = DoCmdEatupMemory();
			break;
		case ECmdUtilFreeEatenMemory:
			nReturn = DoCmdFreeEatenMemory();
			break;
		default:
			nReturn=CmdReply(iFunctions, aOpcode, aArgs);
		}

	return nReturn;
	}

void CTWinAnim::Command(TInt aOpcode, TAny *aArgs)
	{
	CmdReply(iFunctions, aOpcode, aArgs);
	}

void CTWinAnim::Animate(TDateTime *aDateTime)
	{
	//Set iAnimateTime.
	TTime now;
	now.HomeTime();
	iAnimateTime = now.DateTime();

	switch (iCurrentCommand)
		{
		case ECmdGeneralAnimate:
			iDateTime = *aDateTime;
			break;
		case ECmdGeneralSetInterval:
			{
			if (iIntervalTimeStamp[0]==0)
				{
				iIntervalTimeStamp[0]=now.Int64();
				}
			else
				if (iIntervalTimeStamp[1]==0)
					{
					iIntervalTimeStamp[1]=now.Int64();
					}
			break;
			}

		case ECmdGeneralSetNextInterval:
			{
			if (iIntervalTimeStamp[1]==0)
				{
				iIntervalTimeStamp[1]=now.Int64();
				}
			else
				if (iIntervalTimeStamp[2]==0)
					{
					iIntervalTimeStamp[2]=now.Int64();
					}
			break;
			}
		}

	iResult=ETrue;
	//if MAnimGeneralFunctions::Sync() not equal to CWindowAnim::iSyncMode then call CWindowAnim::SetSync(CWindowAnim::iSyncMode)
	if (iFunctions->Sync()!=iSyncMode && iCurrentCommand==ECmdWindowIsStarted)
		{
		iFunctions->SetSync(iSyncMode);
		}
	// if the last position has been drawn, update the position
	if (iDrawn)
		{
		// this position has not been drawn, so clear the flag
		iDrawn = EFalse;

		if (iLastPoint != iNewPoint)
			Compare();

		// Save the current position, so we can rub it out later
		iLastPoint = iNewPoint;

		TInt nextpoint=iNewPoint.iX + iStepX;
		TInt right=iNewPoint.iX + iSize.iWidth;

		if (right> iScreenSize.iBr.iX ||nextpoint >= iScreenSize.iBr.iX
				||iNewPoint.iX<iScreenSize.iTl.iX)
			iStepX = -iStepX;

		TInt nexty=iNewPoint.iY + iStepY;
		TInt bottom=nexty + iSize.iHeight;

		if (bottom> iScreenSize.iBr.iY ||nexty >= iScreenSize.iBr.iY
				||iNewPoint.iY<iScreenSize.iTl.iY || nexty
				<= iScreenSize.iTl.iY)
			iStepY = -iStepY;

		iNewPoint.iX += iStepX;
		iNewPoint.iY += iStepY;

		// Force a draw to be performed by declaring the screen invalid
		iWindowFunctions->Invalidate(iScreenSize);
		}

	RecordAnimTime();

	TInt interval=(iAnimTime[KAnimTimeSize-1]-iAnimTime[KAnimTimeSize-2])/1000;

	TBuf<100> buf;
	buf.Format(_L("CTWinAnim::Animate interval=%d x=%d y=%d"),
	interval,iNewPoint.iX,iNewPoint.iY);

	WriteLog(buf);
	}

void CTWinAnim::Compare()
	{
	// compare     	
	const CFbsScreenDevice* scdev=iFunctions->ScreenDevice();

	TBool compare=scdev->RectCompare(TRect(iNewPoint, iSize), *scdev, TRect(
			TPoint(0, 0), iSize));
	TBuf<50> buf;
	buf.Format(_L("CTWinAnim::Compare res %d"),
	compare);
	iDll->WriteLog(buf);
	}
TBool CTWinAnim::OfferRawEvent(const TRawEvent& aRawEvent)
	{
	WriteLog(_L("CTWinAnim::OfferRawEvent has been called"));
	TBuf<10> event;
	event.AppendNum(aRawEvent.Type());
	WriteLog(event);
	iResult = ETrue;
	return ETrue;
	}

//for MAnimWindowFunctions
TInt CTWinAnim::DoCmdSetRect()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TRect> arg;
	TRect rect(0, 0, 0, 0);
	TInt nReturn=KErrNotFound;

	if (msg && !msg->IsNull())
		{
        nReturn=msg->Read(1, arg);
		if (KErrNone==nReturn)
			{
			rect=arg();
			iWindowFunctions->SetRect(rect);
			}
		}
	return nReturn;
	}

TInt CTWinAnim::DoCmdWindowSize()
	{
	TSize size=iWindowFunctions->WindowSize();
	TPckgBuf<TSize> pckg(size);
	TInt nReturn=KErrNone;
	const RMessagePtr2 *msg=iFunctions->Message();

	if (msg)
		{
		nReturn=msg->Write(1, pckg);
		}

	return nReturn;
	}

TInt CTWinAnim::DoCmdSetVisible()
	{
	TInt nReturn=KErrNotFound;
	TPckgBuf<TBool> arg;
	const RMessagePtr2 *msg=iFunctions->Message();

	if (msg)
		{
        nReturn=msg->Read(1, arg);
		if (KErrNone==nReturn)
			{
			iWindowFunctions->SetVisible(arg());
			}
		}

	return nReturn;
	}

TInt CTWinAnim::DoCmdInvalidate()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TRect> arg;
	TRect rect(0, 0, 0, 0);
	TInt nReturn=KErrNotFound;

	if (msg && !msg->IsNull())
		{
        nReturn=msg->Read(1, arg);
		if (KErrNone==nReturn)
			{
			rect=arg();
			iResult=EFalse;
			iWindowFunctions->Invalidate(rect);
			iWindowFunctions->ActivateGc();
			iGc->DrawRect(rect);
			}
		}
	return nReturn;
	}

TInt CTWinAnim::DoCmdParameters()
	{
	TWindowInfo data;
	iWindowFunctions->Parameters(data);

	TPckgBuf<TRect> pckgScreenPos(data.iScreenPos);
	TPckgBuf<TInt> pckgMode((TInt)data.iMode);

	TInt nReturn=KErrNone;
	const RMessagePtr2 *msg=iFunctions->Message();

	if (msg)
		{
		nReturn=msg->Write(1, pckgScreenPos);
		nReturn=msg->Write(2, pckgMode);
		}

	return nReturn;
	}

TInt CTWinAnim::DoCmdVisibleRegion()
	{
	RRegion region;
	iWindowFunctions->VisibleRegion(region);
	return region.Count();
	}

//for MAnimGeneralFunctionsWindowExtension
TInt CTWinAnim::DoCmdSetFocusScreen()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TInt> arg;
    TInt nReturn=KErrNotFound;
    
	if (msg && !msg->IsNull())
		{
        nReturn=msg->Read(1, arg);
		if (KErrNone==nReturn)
			{
			iFunctions->WindowExtension()->SetFocusScreen(arg());
			}
		}
    return nReturn;
	}

TInt CTWinAnim::DoCmdWindowGroups()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TInt> arg;
	TInt nReturn=KErrNotFound;

	if (msg && !msg->IsNull())
		{
        nReturn=msg->Read(1, arg);
		if (KErrNone==nReturn)
			{
			nReturn=iFunctions->WindowExtension()->WindowGroups(arg());
			}
		}
	return nReturn;
	}

TBool CTWinAnim::DoCmdWindowGroupInfo()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TInt> argScrNumber;
	TPckgBuf<TInt> argPosition;

	TBool nReturn=EFalse;
	MAnimGeneralFunctionsWindowExtension::TWindowGroupInfo aInfo;

	if (msg && !msg->IsNull())
		{
		if (KErrNone==msg->Read(1, argScrNumber) && KErrNone==msg->Read(2, argPosition))
			{
			nReturn=iFunctions->WindowExtension()->WindowGroupInfo(aInfo, argScrNumber(), argPosition());

			if (nReturn)
				{
				TPckgBuf<TInt> pckgInfo(aInfo.iId);
				msg->Write(3, pckgInfo);
				}
			}
		}
	return nReturn;
	}

TBool CTWinAnim::DoCmdWindowGroupName()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TInt> argScrNumber;
	TPckgBuf<TInt> argPosition;

	TBool nReturn=EFalse;

	if (msg && !msg->IsNull())
		{
		if (KErrNone==msg->Read(1, argScrNumber) && KErrNone==msg->Read(2, argPosition))
			{
			TPtrC name;
			nReturn=iFunctions->WindowExtension()->WindowGroupName(name, argScrNumber(), argPosition());
			if (nReturn)
				{
				msg->Write(3, name);
				}
			}
		}

	return nReturn;
	}

TInt CTWinAnim::DoCmdSetOrdinalPosition()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TInt> argGroupId;
	TPckgBuf<TInt> argPosition;
	TPckgBuf<TInt> argPriority;

	TInt nReturn=KErrNotFound;

	if (msg && !msg->IsNull())
		{
		if (KErrNone==msg->Read(1, argGroupId) && KErrNone==msg->Read(2, argPosition)
				&& KErrNone==msg->Read(3, argPriority))
			{
			nReturn=iFunctions->WindowExtension()->SetOrdinalPosition(argGroupId(), argPosition(),
					argPriority());
			}
		}

	return nReturn;
	}

TBool CTWinAnim::DoCmdIsFocusable()
	{
	const RMessagePtr2 *msg=iFunctions->Message();
	TPckgBuf<TInt> argScrNumber;
	TPckgBuf<TInt> argPosition;

	TBool nReturn=EFalse;
	MAnimGeneralFunctionsWindowExtension::TWindowGroupInfo aInfo;

	if (msg && !msg->IsNull())
		{
		if (KErrNone==msg->Read(1, argScrNumber) && KErrNone==msg->Read(2, argPosition))
			{
			if (iFunctions->WindowExtension()->WindowGroupInfo(aInfo, argScrNumber(), argPosition()))
				{
				nReturn=aInfo.IsFocusable();
				}
			}
		}
	return nReturn;
	}

TBool CTWinAnim::DoCmdWindowConfig()
	{
	TWindowConfig config;
	iFunctions->WindowExtension()->WindowConfig(config);
	return config.iFlags & TWindowConfig::ETransparencyEnabled; 
	}

TInt CTWinAnim::DoCmdDuplicateBitmapL()
	{
	TInt result = 0;
	TInt handle;
	TPtr8 ptr1((TUint8*)&handle, sizeof(TInt));
	const RMessagePtr2 *msg=iFunctions->Message();
		
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iDuplicateBitmap = iFunctions->DuplicateBitmapL(handle);
		if (iDuplicateBitmap)
			{
			TRect rect;
			iWindowFunctions->ActivateGc();
			rect.SetRect(0, 0, 200, 200);
			iGc->DrawBitmap(rect, iDuplicateBitmap);
			}
		}
	else
		{
		result = -1;
		}
	return result;
	}

TInt CTWinAnim::DoCmdDuplicateFontL()
	{
	TInt result = 0;
	TInt handle;
	TPtr8 ptr1((TUint8*)&handle, sizeof(TInt));
	const RMessagePtr2 *msg=iFunctions->Message();
			
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iDuplicateFont = iFunctions->DuplicateFontL(handle);
		TInt id =0;
		if (iDuplicateFont)
			{
			id = iDuplicateFont->TypeUid().iUid;
			TPtr8 ptr2((TUint8*)&id, sizeof(TInt), sizeof(TInt));
			msg->Write(2, ptr2);
			}
		}
	else
		{
		return -1;
		}
		
	return result;
	}

TInt CTWinAnim::DoCmdCloseFontL()
	{
	DoCmdDuplicateFontL();
	iFunctions->CloseFont(iDuplicateFont);
	return 0;
	}

TInt CTWinAnim::DoCmdSetIntervalL()
	{
	TInt result = 0;
	TPtr8 ptr1((TUint8*)&iInterval, sizeof(TInt));
	const RMessagePtr2 *msg=iFunctions->Message();
			
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		if (iIntervalTimeStamp)
		delete [] iIntervalTimeStamp;
		iIntervalTimeStamp=new (ELeave) TInt64[3];
		iIntervalTimeStamp[0]=0;
		iIntervalTimeStamp[1]=0;
		iIntervalTimeStamp[2]=0;
		iFunctions->SetInterval(iInterval);
		iServerTimer->After( 5000000); // delay 5 second
		}
	else
		{
		result = -1;
		}	

	return result;
	}

TInt CTWinAnim::DoCmdSetNextIntervalL()
	{
	TInt result = 0;
	TPtr8 ptr1((TUint8*)&iInterval, sizeof(TInt));
	TPtr8 ptr2((TUint8*)&iNextInterval, sizeof(TInt));
	const RMessagePtr2 *msg=iFunctions->Message();
			
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		msg->ReadL(2,ptr2);
		if (iIntervalTimeStamp)
			delete [] iIntervalTimeStamp;
		
		iIntervalTimeStamp=new (ELeave) TInt64[3];
		iIntervalTimeStamp[0]=0;
		iIntervalTimeStamp[1]=0;
		iIntervalTimeStamp[2]=0;
		iFunctions->SetInterval(iInterval);
		iFunctions->SetNextInterval(iNextInterval);//2 flash = 1 second
		if (iIntervalTimeStamp[0]==0)
			{
			TTime now;
			now.HomeTime();
			iIntervalTimeStamp[0]=now.Int64();
			}

		iServerTimer->After( 5000000); //delay 5 second
		}
	else
		{
		result = -1;
		}
		
	return result;
	}

TInt CTWinAnim::DoCmdSystemTimeL()
	{
	TBool result = DoCmdSetSyncL();
	if (result)
		{
		return result;
		}
	iFunctions->Animate(NULL);

	TDateTime time = iFunctions->SystemTime();
	MAnimGeneralFunctions::TAnimSync syncType = iFunctions->Sync();
	switch (syncType)
		{
		case MAnimGeneralFunctions::ESyncDay:
			if (time.Day() == iAnimateTime.Day())
				{
				result = ETrue;
				}
			else
				{
				result = EFalse;
				}
			break;
		case MAnimGeneralFunctions::ESyncMinute:
			if (time.Minute() == iAnimateTime.Minute() && time.Day()
					== iAnimateTime.Day())
				{
				result = ETrue;
				}
			else
				{
				result = EFalse;
				}
			break;
		case MAnimGeneralFunctions::ESyncFlash:
		case MAnimGeneralFunctions::ESyncNone:
		case MAnimGeneralFunctions::ESyncSecond:
			if (time.Day() == iAnimateTime.Day() && time.Minute()
					== iAnimateTime.Minute() && time.Second()
					== iAnimateTime.Second())
				{
				result = ETrue;
				}
			else
				{
				result = EFalse;
				}
			break;
		}
	return result;
	}

TInt CTWinAnim::DoCmdRegisterForNotisL()
	{
	iResult = EFalse;
	TUint32 notice;
	TInt result = 0;
	TPtr8 ptr1((TUint8*)&notice, sizeof(TUint32));
	TPtr8 ptr2((TUint8*)&iExpectEventCode, sizeof(TEventCode));
	const RMessagePtr2 *msg=iFunctions->Message();
			
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1, ptr1);
		msg->ReadL(2, ptr2);
		iFunctions->RegisterForNotifications(notice);		
		}
	else
		{
		return -1;
		}
	
	return result;
	}

TInt CTWinAnim::DoCmdMessageL()
	{
	TInt result;
	TBuf<KBufMessage> message;
	const RMessagePtr2 *msg=iFunctions->Message();
			
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,message);
		msg->Write(2, message);
		}
	else
		{
		result = -1;
		}
	
	return result;
	}

TInt CTWinAnim::DoCmdAnimateL()
	{
	TInt result = 0;
	TDateTime time;
	TPtr8 ptr1((TUint8*)&time, sizeof(TDateTime));
	const RMessagePtr2 *msg=iFunctions->Message();
			
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iResult = EFalse;
		iFunctions->Animate(&time);
		if (iDateTime.MicroSecond() == time.MicroSecond() && iDateTime.Minute()
				== time.Minute() && iDateTime.Second() == time.Second())
			{
			iResult = ETrue;
			}
		else
			{
			iResult = EFalse;
			}
	
		}
	
	return iResult;
	}


TInt CTWinAnim::DoCmdFlashStateOn()
	{
	TBool status = EFalse;
	TTime time;
	time.HomeTime();
	TBool result = iFunctions->FlashStateOn();
	TInt ms = time.DateTime().MicroSecond();
	TInt H1Limit = 16000; //0.016 * 100000 = 1/64 * 1000000;
	TInt H2Limit = 567000; // (7/12 - 1/64) * 1000000;
	TInt H3Limit = 599000; // (7/12 + 1/64) * 1000000;
	TInt H4Limit = 984000; // (1 - 0.016) * 1000000;
	
	if(ms < H1Limit || 
			(ms > H2Limit && ms < H3Limit) ||
			ms > H4Limit)
		{
		return -1;
		}
	
	if (time.DateTime().MicroSecond() < 1000000 * 7/12)
		{
		status = ETrue;
		}
	else
		{
		status = EFalse;
		}
	
	if (result == status)
		{
		status = ETrue;
		}
	else
		{
		status = EFalse;
		}

	return status;
	}

TInt CTWinAnim::DoCmdPanic()
	{
	iFunctions->Panic();
	return 0;
	}

TInt CTWinAnim::DoCmdScreenDevice()
	{
	TDisplayMode mode = iFunctions->ScreenDevice()->DisplayMode();
	return 0;
	}

TInt CTWinAnim::DoCmdWindowExtension()
	{
	MAnimGeneralFunctionsWindowExtension* ext = iFunctions->WindowExtension();
	TInt screen = ext->FocusScreens();
	return 0;
	}

TInt CTWinAnim::DoCmdEventExtension()
	{
	MAnimGeneralFunctionsEventExtension* ext = iFunctions->EventExtension();
	TKeyEvent event;
	event.iCode = EEventKeyDown;
	ext->PostKeyEvent(event, 1);
	return 0;
	}

TInt CTWinAnim::DoCmdExtendedInterfaceL()
	{
	TInt result = 0;
	TInt interface;
	TPtr8 ptr1((TUint8*)&interface, sizeof(TInt));
	const RMessagePtr2 *msg=iFunctions->Message();
				
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		TAny* any = iFunctions->ExtendedInterface(interface);
		if (interface == 0)
			{
			//Zhang Yue modified.for the return result is a Int value, not a pointer address.
			result = (TInt)any;
			}
		else
			{
			if (interface == 1)
				{
				((MAnimGeneralFunctionsWindowExtension*) any)->FocusScreens();
				}
			else
				{
				if (interface == 2)
					{
					TKeyEvent event;
					event.iCode = EEventKeyDown;
					((MAnimGeneralFunctionsEventExtension*) any)->PostKeyEvent(
							event, 1);
					}
				else
					{
					if (any)
						{
						result = -1;
						}
					else
						{
						result = 0;
						}//end if (any)
					}//end if(interface 2)
				}//end if(interface)
			}//end if(interface)
	
		} //end if(msg)		
	else
		{
		result = -1;
		}
	
	return result;
	}

TInt CTWinAnim::DoCmdNumOfExtInterfaces()
	{
	return iFunctions->NumberOfExtendedInterfaces();
	}

TInt CTWinAnim::DoCmdSetSyncL()
	{
	TInt result = 0;
	MAnimGeneralFunctions::TAnimSync mode;
	TPtr8 ptr1((TUint8*)&mode, sizeof(MAnimGeneralFunctions::TAnimSync));
	const RMessagePtr2 *msg=iFunctions->Message();
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iFunctions->SetSync(mode);		
		}
	else
		{
		return -1;
		}
	return result;
	}

TInt CTWinAnim::DoCmdSync()
	{
	MAnimGeneralFunctions::TAnimSync mode = iFunctions->Sync();
	return mode;
	}

TInt CTWinAnim::DoCmdGetRawEventsL()
	{
	TInt result = 0;
	TBool getevent;
	TPtr8 ptr1((TUint8*)&getevent, sizeof(TBool));
	const RMessagePtr2 *msg=iFunctions->Message();
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iFunctions->GetRawEvents(getevent);
			
		}
	else
		{
		return -1;
		}

	return result;
	}

TInt CTWinAnim::DoCmdPostRawEventL()
	{
	iResult = EFalse;
	TInt result = 0;
	TRawEvent event;
	TPtr8 ptr1((TUint8*)&event, sizeof(TRawEvent));
	const RMessagePtr2 *msg=iFunctions->Message();
	
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iFunctions->PostRawEvent(event);		
		}
	else
		{
		return -1;
		}

	return result;
	}

TInt CTWinAnim::DoCmdPostKeyEventL()
	{
	iResult = EFalse;
	TInt result = 0;
	TKeyEvent event;
	TPtr8 ptr1((TUint8*)&event, sizeof(TKeyEvent));
	const RMessagePtr2 *msg=iFunctions->Message();
	
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		iFunctions->PostKeyEvent(event);
		}
	else
		{
		result = -1;
		}
	return result;
	}

TInt CTWinAnim::DoCmdClient()
	{
	return iFunctions->Client().Id();
	}

TInt CTWinAnim::DoCmdGeneralReplyBuf8L()
	{
	TInt result;
	TBuf8<KBufMessage> message;
	const RMessagePtr2 *msg=iFunctions->Message();
	if(msg && !msg->IsNull())
		{
		msg->ReadL(3,message);
		iFunctions->ReplyBuf(message);		
		}
	else
		{
		result = -1;
		}
	
	return result;
	}

TInt CTWinAnim::DoCmdGeneralReplyBuf16L()
	{
	TInt result;
	TBuf<KBufMessage> message;
	const RMessagePtr2 *msg=iFunctions->Message();
	if(msg && !msg->IsNull())
		{
		msg->ReadL(3,message);
		iFunctions->ReplyBuf(message);
		}
	else
		{
		result = -1;
		}
	return result;
	}

TInt CTWinAnim::DoCmdEatupMemory()
	{
	TInt leftsize=20;

	// eat memory until fail to avaiable memory is less than is left size  
	TInt nTotalSize=User::Heap().Size();
	TAny* mem=User::AllocZ(nTotalSize);

	if (mem)
		iMemArray.Append(mem);

	do
		{
		mem=User::AllocZ(leftsize);
		if (mem)
			{
			iMemArray.Append(mem);
			}
		}
	while (mem);

	iAvaiableSpace=User::Available(iBiggestBlock);
	return 0;
	}

TInt CTWinAnim::DoCmdFreeEatenMemory()
	{
	FreeMemory();
	return 0;
	}

void CTWinAnim::FreeMemory()
	{
	for (TInt i=0; i<iMemArray.Count(); i++)
		{
		TAny* mem=iMemArray[i];
		User::Free(mem);
		}

	iMemArray.Reset();
	}
