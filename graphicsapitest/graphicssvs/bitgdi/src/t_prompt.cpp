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

#include <e32base.h>
#include <e32cons.h>
#include <e32std.h>
#include <e32property.h>

/*@{*/
const TInt KDefaultInterval	= 20000000;
const TUid KMyPropertyCat = {0x10012345};

_LIT(KPromptConsole, "Manual Check");
/*@}*/

LOCAL_C void MainL()
	{
	TInt argLen = User::CommandLineLength();
	HBufC* hBuf = HBufC::NewLC(argLen);
	TPtr tPtr = hBuf->Des();
	User::CommandLine(tPtr);
	CConsoleBase* console = Console::NewL(KPromptConsole, TSize(KConsFullScreen, KConsFullScreen));
	CleanupStack::PushL(console);
	console->Printf(KPromptConsole);
	console->Printf(_L("\n\n\n\n\n"));
	console->Printf(tPtr);
	
	RTimer timer;
	CleanupClosePushL(timer);
	timer.CreateLocal();
	TRequestStatus timerStatus;
	timer.After(timerStatus, TTimeIntervalMicroSeconds32(KDefaultInterval));
	TRequestStatus readStatus;
	console->Read(readStatus);
	User::WaitForRequest(timerStatus, readStatus);
	
	TKeyCode keycode(EKeyNull);
	if(timerStatus != KErrNone)
		{
		if(readStatus == KErrNone)
			{
			keycode = console->KeyCode();
			}
		timer.Cancel();
		}

	if(readStatus == KRequestPending)
		{
		console->ReadCancel();
		}
	
	RProperty property;
	CleanupClosePushL(property);
	User::LeaveIfError( property.Attach(KMyPropertyCat, 1) );
	User::LeaveIfError( property.Set((TInt)keycode) );
	//property.Close();
	CleanupStack::PopAndDestroy(4, hBuf); // &property, &timer, console, hBuf
	}

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		RProcess::Rendezvous(KErrNoMemory);
		return KErrNoMemory;
		}
#if (defined TRAP_IGNORE)
	TRAP_IGNORE(MainL());
	RProcess::Rendezvous(KErrNone);
#else
	TRAPD(err,MainL());
	RProcess::Rendezvous(err);
#endif
	delete cleanup;
	return KErrNone;
    }
