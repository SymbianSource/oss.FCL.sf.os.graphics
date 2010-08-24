// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent - Internal Symbian test code
 */ 

#ifndef TPTROFFSET_H_
#define TPTROFFSET_H_

#include <e32std.h>
#include <e32cmn.h>
#include "w32std.h"
#include "../tlib/testbase.h"
#include "AUTO.H"
#include "AUTODLL.H"
#include "TGraphicsHarness.h"


class CTPointerOffsetTest;
class CTPointerOffsetClient;

/*
 * CTEvent derived class which acts as eventhandler and eventbuffer 
 * Stores the events in buffer, which is used for comparing events received from wserv.
 */ 
class CTPointerOffsetBuffer : public CTEvent
	{
	enum {EEventBufferSize=40};
public:
	CTPointerOffsetBuffer(RWsSession *aWs, CTPointerOffsetTest *aTest, CTPointerOffsetClient* aClient);
	~CTPointerOffsetBuffer();
	void ConstructL();
	void AddExpectedEvent(TWsEvent &aEvent);
	TInt EventsRemaining();
	void SetEventCount(TInt aCount) {iEventCount = aCount;}

protected:
	// Pure virtual from CTEventBase
	void doRunL();
private:
	void TestL(TInt aTest);

private:
	CCirBuf<TWsEvent> iEventBuffer;
	CTPointerOffsetTest* iTest;
	TInt iEventCount;
	CTPointerOffsetClient* iClient;
	};

/* 
 * CTClient derived class which gives Wserv client environment 
 * i,e iWs, iScreen, iGc, iGroup, iEventHandler etc...
 * Owns CTPointerOffsetBuffer and stores in iEventhandler of its base class
 * Accepts the event from test class and passes them on to buffer class
 */ 
class CTPointerOffsetClient : public CTClient
	{
public:
	CTPointerOffsetClient(CTPointerOffsetTest *aTest);
	~CTPointerOffsetClient();
	void ConstructL();
	
	inline CTBlankWindow* Win() { return iWin; }
	inline CTPointerOffsetBuffer* EventBuffer() {return static_cast<CTPointerOffsetBuffer*>(iEventHandler);}
	
	void AddExpectedPointerEvent(TPointerEvent::TType aType, TPoint aPos, TUint8 aPointerNumber, TUint aHandle = 0);
	void AddExpectedWsEvent(TEventCode aType, TInt aPointerNumber = 0, TUint aHandle = 0);

	// Virtual from CTClient
	void ConstructEventHandlerL();
	
private:
	CTPointerOffsetTest* iTest;
	TSize iWinSize;
	TPoint iWinPos;
	CTBlankWindow* iWin;
	};


/*
 * CTWsGraphicsBase derived class which implements RunTestCaseL pure virtual.
 * Runs all the testcases for pointer events which have offset.
 * For each test case it creates CTPointerOffsetClient object and creates a nested activescheduler
 * Calls NextSetOfEventsL() which in turn calls respective tests depending upon the testcase number
 * For each test, simulate the events and adds the same to CTPointerOffsetBuffer
 * When all the tests for a particular testcase completes, stops activescheduler.
 * Repeats the same for all testcases
 */ 
class CTPointerOffsetTest : public CTWsGraphicsBase
	{
public:
	CTPointerOffsetTest(CTestStep* aStep);
	~CTPointerOffsetTest();
	void ConstructL();
	void NextSetOfEventsL();
	void Failed();
	
	// Function for simulating events, which use UserSvr::AddEvent
	void SimulatePointerEvent(TRawEvent::TType aType, TInt aX, TInt aY, TUint8 aPointerNumber);
	
	// Function for adding event, calls the client's AddExpectedPointer
	void AddExpectedPointerEvent(TPointerEvent::TType aType, TInt aX, TInt aY, TUint8 aPointerNumber, TUint aHandle = 0);
	void AddExpectedWsEvent(TEventCode aType, TInt aPointerNumber = 0, TUint aHandle = 0);
	
	// Supplementary function for tests
	TInt GetShiftedYValue(TInt aY);
	
	// All tests function can come here
	void TestYOffsetValues();
	
protected:
	void RunTestCaseL(TInt aCurTestCase);
private:
	void RunTestsL();
private:
	TInt iYOffsetTop;
	TInt iYOffsetBottom;
	TInt iYOffsetMax;	
	
	TInt iEventSet;			// Set of events for one particular test case
	TBool iFailed;
	TSize iPhysicalScreenSize;
	TInt iPointerNumber;
	TInt iMaxPointerNumbers;
	
	CTPointerOffsetClient *iPointerOffsetClient;
	};

class CTPointerOffsetTestStep : public CTGraphicsStep
	{
public:
	CTPointerOffsetTestStep();
protected:
	//from CTGraphicsStep
	virtual CTGraphicsBase* CreateTestL();
	};

_LIT(KTPointerOffsetTestStep, "TPointerOffsetTest");

#endif /*TPTROFFSET_H_*/
