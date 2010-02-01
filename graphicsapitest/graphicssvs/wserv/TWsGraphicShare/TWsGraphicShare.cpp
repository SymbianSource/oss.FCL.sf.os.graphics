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


//  Include Files  

#include "TWsGraphicShare.h"
#include <e32base.h>
#include <e32std.h>
#include <w32stdgraphic.h>

//  Local Functions

LOCAL_C void DrawSharedGraphicL(const TDesC& aSemName, TInt aScrNum, const TWsGraphicId& aId, const TRect& aDestRect, const TDesC8& aData)
	{
	// Construct & Initiate
	RWsSession ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);
	
	CWsScreenDevice* screenDevice = new(ELeave) CWsScreenDevice(ws);
	CleanupStack::PushL(screenDevice);
	User::LeaveIfError(screenDevice->Construct(aScrNum));

	TSize screenSize = screenDevice->SizeInPixels();

	CWindowGc* gc = new(ELeave) CWindowGc(screenDevice);
	CleanupStack::PushL(gc);
	User::LeaveIfError(gc->Construct());
	
	RWindowGroup winGroup(ws);
	CleanupClosePushL(winGroup);
	User::LeaveIfError(winGroup.Construct(1));
		
	RWindow win(ws);
	CleanupClosePushL(win);
	TUint32 handle = 0x0F00;
	User::LeaveIfError(win.Construct(winGroup, handle));
	win.SetBackgroundColor(TRgb(255, 255, 255));
	win.SetPosition(TPoint(aDestRect.iTl.iX, aDestRect.iTl.iY));
	win.SetSize(aDestRect.Size());
	win.Activate();

	// Draw
	gc->Activate(win);
	win.Invalidate();
	win.BeginRedraw();
	ws.Flush();	
	gc->DrawWsGraphic(aId, TRect(0,0,aDestRect.Width(),aDestRect.Height()), aData );
	gc->Deactivate();
	win.EndRedraw();
	ws.Flush();
	
	// Synchronize with master process
	RSemaphore semaphore;
	CleanupClosePushL(semaphore);
	User::LeaveIfError(semaphore.OpenGlobal(aSemName));
	semaphore.Signal();
	semaphore.Wait();
	semaphore.Close();
    
	// Deconstruct
	CleanupStack::PopAndDestroy(6);
	}

LOCAL_C void MainL ( )
	{
	TBuf<128> commandLine;
	TLex lex;
	TBuf<32> semName;
	TInt screenNumber = 0; 
	TInt graphId;	
	TInt left, top, right, bottom;

	// Read arguments from the command line
	// There is a space between two adjacent parameters
	// Commandline's format looks like that:
	// 'screenNumber' 'graphId' 'left' 'top' 'right' 'bottom' 
	User::CommandLine(commandLine);
	commandLine.TrimLeft();
	
	_LIT(KSpace, " ");
	TInt pos;
	// Get process name
	pos = commandLine.Find(KSpace);
	semName = commandLine.Left(pos);
	commandLine = commandLine.Mid(pos+1);
	
	// Get screen number	
	pos = commandLine.Find(KSpace);
	lex = commandLine.Left(pos);
	lex.Val(screenNumber);
	commandLine = commandLine.Mid(pos+1);
	
	// Get gragaphic id	
	pos = commandLine.Find(KSpace);
	lex = commandLine.Left(pos);
	lex.Val(graphId);
	commandLine = commandLine.Mid(pos+1);	
	
	// Get coordinates
	pos = commandLine.Find(KSpace);
	lex = commandLine.Left(pos);
	lex.Val(left);	
	commandLine = commandLine.Mid(pos+1);
	
	pos = commandLine.Find(KSpace);
	lex = commandLine.Left(pos);
	lex.Val(top);
	commandLine = commandLine.Mid(pos+1);
	
	pos = commandLine.Find(KSpace);
	lex = commandLine.Left(pos);
	lex.Val(right);
	commandLine = commandLine.Mid(pos+1);
	
	pos = commandLine.Find(KSpace);
	lex = commandLine.Left(pos);
	lex.Val(bottom);
	commandLine = commandLine.Mid(pos+1);
	
	// Get data
	TBuf8<32> data;
	data.Copy(commandLine);
	
	TWsGraphicId id(TUid::Uid(graphId));
	TRect destRect(left, top, right, bottom);	
	
	// Draw the graphic
	DrawSharedGraphicL(semName, screenNumber, id, destRect, data);
	}

//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(error, MainL());
	_LIT(KWsGraphicShareError, "WsGraphicShareError");
	__ASSERT_ALWAYS(!error,User::Panic(KWsGraphicShareError,error));
	
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
