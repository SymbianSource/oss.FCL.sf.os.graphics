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

#include "UtilityClearPanicDlg.h"

//	EPOC Includes
#include <w32std.h>

/*@{*/
///	Constant Literals used.
_LIT(KCmdUtilityClearPanicDlg,	"utilityClearPanicDlg");
/*@}*/

CUtilityClearPanicDlg::CUtilityClearPanicDlg()
	{
	SetTestStepName(KCmdUtilityClearPanicDlg);
	}

enum TVerdict CUtilityClearPanicDlg::doTestStepL()
	{
	RWsSession	ws;
	User::LeaveIfError(ws.Connect());
	CleanupClosePushL(ws);

	TKeyEvent	event;
	event.iCode=EKeyEnter;
	event.iScanCode=EKeyEnter;
	event.iModifiers=0;
	event.iRepeats=0;
	ws.SimulateKeyEvent(event);

	ws.Flush();
	CleanupStack::PopAndDestroy(&ws);

	return TestStepResult();
	}
