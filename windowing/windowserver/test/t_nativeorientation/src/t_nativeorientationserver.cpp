// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
@file
@test
@internalComponent
*/

//	User Includes
#include "t_nativeorientationserver.h"
#include "t_nativeorientationtest.h"

_LIT(KTestServerName, "NativeOrientationTestServer");

CT_NativeOrientationServer* CT_NativeOrientationServer::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CT_NativeOrientationServer*	server = new (ELeave) CT_NativeOrientationServer;
	CleanupStack::PushL(server);
	server->ConstructL();
	CleanupStack::Pop(server);
	return server;
	}

void CT_NativeOrientationServer::ConstructL()
	{
	RProcess	handle = RProcess();
	TParsePtrC	serverName(handle.FileName());
	CTestServer::ConstructL(serverName.Name());
	}

LOCAL_C void MainL()
	{
	CActiveScheduler*		sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CT_NativeOrientationServer*	server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CT_NativeOrientationServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	else
	    {
        RDebug::Print(_L("Server error=%d"), err);
	    }
	delete server;
	delete sched;
	}

/**
 * @return - Standard Epoc error code on process exit
 * Process entry point. Called by client using RProcess Integ
 */
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	
    TRAPD(err,MainL()); 
    if (err)
        {
        _LIT(KMainErrorStatement, "CNativeOrientationTestServer MainL - Error: %d");
        RDebug::Print(KMainErrorStatement, err);
        User::Panic(KTestServerName, err);
        }       
    
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }

CTestStep* CT_NativeOrientationServer::CreateTestStep(const TDesC& aStepName)
/**
 * @return - A CTestStep derived instance
 * Secure and non-secure variants
 * Implementation of CTestServer pure virtual
 */
	{
	CTestStep*	testStep = NULL;

	if ( aStepName==KT_NativeOrientationTestStep )
		{
		testStep=new CT_NativeOrientationTest();
		}

	return testStep;
	}
