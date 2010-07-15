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
// YShifting, Wserv gradual reduction of YOffset test code
// 
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/

#include "PARSEINIDATA.H"
#include "tptroffset.h"
#include <e32cmn.h>
#include <hal.h>

//#define FAILLOG 1			// Uncomment this code to get detailed log

//CTPointerOffsetBuffer
CTPointerOffsetBuffer::CTPointerOffsetBuffer(RWsSession *aWs, CTPointerOffsetTest *aTest, CTPointerOffsetClient* aClient) 
	: CTEvent(aWs), iTest(aTest), iClient(aClient)
	{
	}

CTPointerOffsetBuffer::~CTPointerOffsetBuffer()
	{
	}

void CTPointerOffsetBuffer::ConstructL()
	{
	CTEventBase::Construct();
	iEventBuffer.SetLengthL(EEventBufferSize);
	}

void CTPointerOffsetBuffer::AddExpectedEvent(TWsEvent &aEvent)
	{
	iEventBuffer.Add(&aEvent);
	}

TInt CTPointerOffsetBuffer::EventsRemaining()
	{
	return iEventBuffer.Count();
	}


// Main function which gets the event, checks with the event in buffer
// Then calls function NextSetOfEventsL for running other tests of a particualar test case
void CTPointerOffsetBuffer::doRunL()
	{
	// Get the event from wserv
	TWsEvent wsEvent;
	iWs->GetEvent(wsEvent);

	TWsEvent expectedEvent;
	TInt wsType=wsEvent.Type();
	
	// if this is called accidentally
	TInt count=iEventBuffer.Count();
	if (count==0 && wsType==EEventFocusGained)
		{
		goto End;
		}

	iEventBuffer.Remove(&expectedEvent);
	iEventCount++;
	
#if defined(FAILLOG)	
	TLogMessageText logText;
	_LIT(KEventCountCheck, "Checking event number = %d");
	logText.Format(KEventCountCheck, iEventCount);
	iTest->LOG_MESSAGE(logText);
	_LIT(KEventType, "Actual Event type from Wserv = %d Expected Event Type = %d ");
	logText.Format(KEventType, wsEvent.Type(), expectedEvent.Type());
	iTest->LOG_MESSAGE(logText);
#endif
	
	TestL(wsEvent.Type() == expectedEvent.Type());
	
#if defined(FAILLOG)
	_LIT(KEventHandle, "Actual Window Handle from Wserv = %d Expected Window Handle = %d ");
	logText.Format(KEventHandle, wsEvent.Handle(), expectedEvent.Handle());
	iTest->LOG_MESSAGE(logText);
#endif    
	TestL(wsEvent.Handle() == expectedEvent.Handle());
	
	TAdvancedPointerEvent *expectedPointerEvent = expectedEvent.Pointer();
    TAdvancedPointerEvent *actualPointerEvent = wsEvent.Pointer();
    
    if (wsType == EEventPointer)
    	{
#if defined(FAILLOG)
		_LIT(KPointerType, "Actual PointerType from Wserv = %d Expected PointerType = %d ");
		logText.Format(KPointerType, actualPointerEvent->iType, expectedPointerEvent->iType);
		iTest->LOG_MESSAGE(logText);
#endif
		TestL(actualPointerEvent->iType == expectedPointerEvent->iType);
		
#if defined(FAILLOG)
		_LIT(KPointerPosition, "Actual PointerPosition from Wserv = (%d, %d) Expected PointerPosition = (%d, %d) ");
		logText.Format(KPointerPosition, actualPointerEvent->iPosition.iX, actualPointerEvent->iPosition.iY, expectedPointerEvent->iPosition.iX, expectedPointerEvent->iPosition.iY);
		iTest->LOG_MESSAGE(logText);
#endif
		TestL(actualPointerEvent->iPosition == expectedPointerEvent->iPosition);
		
#if defined(FAILLOG)
		_LIT(KPointerNumber, "Actual PointerNumber from Wserv = %d Expected PointerNumber = %d ");
		logText.Format(KPointerNumber, actualPointerEvent->PointerNumber(), expectedPointerEvent->PointerNumber());
		iTest->LOG_MESSAGE(logText);
#endif		
		TestL(actualPointerEvent->PointerNumber() == expectedPointerEvent->PointerNumber());
    	}
	
End:
	// Call NextSetOfEvents() when all the events have been checked
	if (iEventBuffer.Count()==0)	
		{
		iTest->NextSetOfEventsL();		// Cannot Leave
		}
	}

void CTPointerOffsetBuffer::TestL(TInt aTest)
	{
	if (!aTest)
		{
		iTest->Failed();
		User::Leave(ETestFailed);
		}
	}

//CTPointerOffsetClient
CTPointerOffsetClient::CTPointerOffsetClient(CTPointerOffsetTest *aTest) 
	: iTest(aTest)
	{
	}

CTPointerOffsetClient::~CTPointerOffsetClient()
	{
	delete iWin;
	}

void CTPointerOffsetClient::ConstructL()
	{
	CTClient::ConstructL();
	
	// Create a group window and assign it iGroup
	iGroup = new(ELeave) CTWindowGroup(this);
	iGroup->ConstructL();
	
	iWinSize = iScreen->SizeInPixels();
	iWinPos = TPoint();
	
	// Create window for this client so that events can come to this client
	iWin=new(ELeave) CTBlankWindow();
	iWin->ConstructL(*iGroup);
	iWin->SetExt(iWinPos, iWinSize);
	iWin->SetColor(KRgbRed);
	iWin->BaseWin()->EnableAdvancedPointers();
	iWin->Activate();
	iWs.Flush();
	}

void CTPointerOffsetClient::AddExpectedPointerEvent(TPointerEvent::TType aType, TPoint aPos, TUint8 aPointerNumber, TUint aHandle)
	{
	TWsEvent ptrEvent;
	ptrEvent.SetType(EEventPointer);
	ptrEvent.Pointer()->iParentPosition = aPos;
	ptrEvent.Pointer()->iType = aType;
	ptrEvent.Pointer()->iPosition = aPos;
	ptrEvent.SetPointerNumber(aPointerNumber);
	if (!aHandle)
		ptrEvent.SetHandle((TUint)iWin);
	else
		ptrEvent.SetHandle(aHandle);

	static_cast<CTPointerOffsetBuffer*>(iEventHandler)->AddExpectedEvent(ptrEvent);
	}

void CTPointerOffsetClient::AddExpectedWsEvent(TEventCode aType, TInt /*aPointerNumber*/, TUint aHandle)
	{
	TWsEvent ptrEvent;
	ptrEvent.SetType(aType);
	if (!aHandle)
		ptrEvent.SetHandle((TUint)iGroup);
	else
		ptrEvent.SetHandle(aHandle);
	static_cast<CTPointerOffsetBuffer*>(iEventHandler)->AddExpectedEvent(ptrEvent);
	}

// Create and store CTPointerOffsetBuffer in iEventHandler
void CTPointerOffsetClient::ConstructEventHandlerL()
	{
	CTPointerOffsetBuffer* eventBufferAndHandler = new(ELeave) CTPointerOffsetBuffer(&iWs, iTest, this);
	eventBufferAndHandler->ConstructL();
	iEventHandler = eventBufferAndHandler;
	}

//CTPointerOffsetTest
CTPointerOffsetTest::CTPointerOffsetTest(CTestStep* aStep)
	: CTWsGraphicsBase(aStep)
	{
	}

CTPointerOffsetTest::~CTPointerOffsetTest()
	{
	delete iPointerOffsetClient;
	}

void CTPointerOffsetTest::ConstructL()
	{
	// If YOffsetValues are not defined in wsini file then just return without doing anything
    _LIT(KWsIniFile, "z:\\system\\data\\wsini.ini");
    CIniData* iniData = CIniData::NewL(KWsIniFile, ' ');
    _LIT( KWsiniYShiftingTop, "YSHIFTINGTOP");
    if (iniData->FindVar(KWsiniYShiftingTop, iYOffsetTop) == EFalse)
    	goto FAIL;
    _LIT( KWsiniYShiftingBottom, "YSHIFTINGBOTTOM");
    if (iniData->FindVar(KWsiniYShiftingBottom, iYOffsetBottom) == EFalse)
    	goto FAIL;
    _LIT( KWsiniYShiftingMax, "YSHIFTINGMAX");
    if (iniData->FindVar(KWsiniYShiftingMax, iYOffsetMax) == EFalse)
    	goto FAIL;
    delete iniData;
    
    if (!iYOffsetTop || !iYOffsetBottom || !iYOffsetMax)
    	{
FAIL:	
		delete iniData;
		TLogMessageText logText1;
		_LIT(KWrongHALConfig, "Y Offset values are incorrect. \n");
		logText1.Append(KWrongHALConfig);
		LOG_MESSAGE(logText1);
		TEST(EFalse);
		User::Leave(KErrArgument);
    	}
    iPhysicalScreenSize = TheClient->iScreen->SizeInPixels();
    
    // If we fail to get the maxpointernumber then just test with single pointer
	TInt ret = HAL::Get(HALData::EPointerMaxPointers, iMaxPointerNumbers);
	if (ret != KErrNone || iMaxPointerNumbers < 2 || iMaxPointerNumbers > 8)
		{
		iMaxPointerNumbers = 1;
		}
	}

// Common function for all tests
// Important point to remember is that this function should not leave when running tests
void CTPointerOffsetTest::NextSetOfEventsL()
	{
	// For each test case calls its respective tests
	switch (iTest->iState)
		{
	case 0:
		TestYOffsetValues();
		break;
	case 1:
		TestYOffsetValues();	
		break;
	default:
		AutoPanic(EAutoPanicWrongTest);
		}
	}

void CTPointerOffsetTest::SimulatePointerEvent(TRawEvent::TType aType, TInt aX, TInt aY, TUint8 aPointerNumber)
	{
	TRawEvent rawEvent;
	rawEvent.Set(aType, aX, aY, 0, aPointerNumber);
	UserSvr::AddEvent(rawEvent);
	}

void CTPointerOffsetTest::AddExpectedPointerEvent(TPointerEvent::TType aType, TInt aX, TInt aY, TUint8 aPointerNumber, TUint aHandle)
	{
	iPointerOffsetClient->AddExpectedPointerEvent(aType, TPoint(aX, aY), aPointerNumber, aHandle);
	}

void CTPointerOffsetTest::AddExpectedWsEvent(TEventCode aType, TInt aPointerNumber, TUint aHandle)
	{
	iPointerOffsetClient->AddExpectedWsEvent(aType, aPointerNumber, aHandle);
	}

// If any of the Tests fails, bring the wserv in a consistent state for other test to run, 
// stop ActiveScheduler and display some logs where the fail occured.
void CTPointerOffsetTest::Failed()
	{
	// Display the test case number, subtest case number and number of events left in the buffer which can be used to
	// find out which event actually caused the failure when any test fails
	TLogMessageText logText1;
	_LIT(KEventsAdded, "Test case number %d Sub test case number %d Events remaining in the buffer %d \n");
	logText1.AppendFormat(KEventsAdded, iTest->iState, iEventSet, iPointerOffsetClient->EventBuffer()->EventsRemaining());
	LOG_MESSAGE(logText1);
	RDebug::Printf("Test case number %d Sub test case number %d Events remaining in the buffer %d \n", iTest->iState, iEventSet, iPointerOffsetClient->EventBuffer()->EventsRemaining());

	CActiveScheduler::Stop();
	iFailed=ETrue;
	}

// Get YValue after applying YOffset using iYOffsetTop, iYOffsetBottom and iYOffsetMax variables.
// Depending upon the Y poistion of input, yOffset is gradually reduced from top of the screen to 
// bottom of the screen.
TInt CTPointerOffsetTest::GetShiftedYValue(TInt aY)
	{
	if (!iYOffsetTop || !iYOffsetMax || !iYOffsetBottom)
		return aY;
	
	TInt offset = 0;
	TInt returnValue = aY;
	TInt displayHeight = TheClient->iScreen->SizeInPixels().iHeight;
	if (aY >= iYOffsetMax)
		{
		// Gradual reduction of Yoffset depending upon the aY value
		offset = iYOffsetTop + iYOffsetBottom - (iYOffsetTop * aY / displayHeight );
		if ( offset > iYOffsetMax )
			{
			offset = iYOffsetMax;
			}
		returnValue -= offset; 
		if (returnValue > (displayHeight-1))
			{
			returnValue = displayHeight-1; 
			}
		}
	else
		{
		returnValue = 0;
		}
	
	return returnValue;
	}

// Common function for all test cases
void CTPointerOffsetTest::RunTestsL()
	{
	// Create CTPointerOffsetClient for each test case and call NextSetOfEventsL()
	iPointerOffsetClient = new(ELeave) CTPointerOffsetClient(this);
	iPointerOffsetClient->SetScreenNumber(iTest->iScreenNumber);
	iPointerOffsetClient->ConstructL();
	
	iEventSet=0;

	TRAPD(err, NextSetOfEventsL());
	if (err != KErrNone)
		{
		iFailed = ETrue;
		}

	// Start nested activescheduler if it is not failed
	if (!iFailed)
		{
		CActiveScheduler::Start();
		}

	TInt eventsRem = iPointerOffsetClient->EventBuffer()->EventsRemaining();
	if (eventsRem > 0)
		{
		iFailed=ETrue;
		}
	
	delete iPointerOffsetClient;
	iPointerOffsetClient = NULL;
	}

// This function is called by TestExecute Framework, through which all the testcases are run
void CTPointerOffsetTest::RunTestCaseL(TInt /*aCurTestCase*/)
	{
	_LIT(KTest0, "PointerOffset tests");
	_LIT(KTest1, "PointerOffset tests for other pointer");
	
	switch(iTest->iState)
		{
	case 0:
		iTest->LogSubTest(KTest0);
		iPointerNumber = 0;
		RunTestsL();
		break;
	case 1:
		if (iMaxPointerNumbers == 1)
			{
			TestComplete();
			}
		else
			{
			iTest->LogSubTest(KTest1);
			iPointerNumber = iMaxPointerNumbers-1;
			RunTestsL();
			}
		break;
	case 2:
		// Keep this code in last case statement for main test completion 
		TestComplete();
		break;
	default:
		AutoPanic(EAutoPanicWrongTest);
		}

	// 
	if (iFailed)
		{
		TEST(EFalse);
		iFailed=EFalse;
		}
	++iTest->iState;
	}


void CTPointerOffsetTest::TestYOffsetValues()
	{
	TLogMessageText logText;
	_LIT(KSet, "Offset tests number : %d of 5");
	logText.AppendFormat(KSet, iEventSet);
	LOG_MESSAGE(logText);
	
	TPoint simulatedPos(iPhysicalScreenSize.iWidth/2, iPhysicalScreenSize.iHeight/2);
	TPoint expectedPos(simulatedPos);
	switch(iEventSet++)
		{
		case 0:
			// Test any point which has Y value less than YOffsetMax, where Y position would be 0
			simulatedPos.iY = iYOffsetMax - 1;
			SimulatePointerEvent(TRawEvent::EButton1Down, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			SimulatePointerEvent(TRawEvent::EButton1Up, simulatedPos.iX, simulatedPos.iY, iPointerNumber);

			expectedPos.iY = 0;
			AddExpectedWsEvent(EEventFocusGained);
			AddExpectedPointerEvent(TPointerEvent::EButton1Down, expectedPos.iX, expectedPos.iY, iPointerNumber);
			AddExpectedPointerEvent(TPointerEvent::EButton1Up, expectedPos.iX, expectedPos.iY, iPointerNumber);
			break;
		case 1:
			// Test any point which has Y value just over YOffsetMax
			simulatedPos.iY = iYOffsetMax + 1;
			SimulatePointerEvent(TRawEvent::EButton1Down, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			SimulatePointerEvent(TRawEvent::EButton1Up, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			
			expectedPos.iY = GetShiftedYValue(simulatedPos.iY); 
			AddExpectedPointerEvent(TPointerEvent::EButton1Down, expectedPos.iX, expectedPos.iY, iPointerNumber);
			AddExpectedPointerEvent(TPointerEvent::EButton1Up, expectedPos.iX, expectedPos.iY, iPointerNumber);
			break;
		case 2:
			// Test any point which has Y value middle of the screen
			SimulatePointerEvent(TRawEvent::EButton1Down, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			SimulatePointerEvent(TRawEvent::EButton1Up, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			
			expectedPos.iY = GetShiftedYValue(simulatedPos.iY); 
			AddExpectedPointerEvent(TPointerEvent::EButton1Down, expectedPos.iX, expectedPos.iY, iPointerNumber);
			AddExpectedPointerEvent(TPointerEvent::EButton1Up, expectedPos.iX, expectedPos.iY, iPointerNumber);
			break;
		case 3:
			// Test any point which has Y value less than bottommost of the screen
			simulatedPos.iY = iPhysicalScreenSize.iHeight - 10;
			SimulatePointerEvent(TRawEvent::EButton1Down, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			SimulatePointerEvent(TRawEvent::EButton1Up, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			
			expectedPos.iY = GetShiftedYValue(simulatedPos.iY); 
			AddExpectedPointerEvent(TPointerEvent::EButton1Down, expectedPos.iX, expectedPos.iY, iPointerNumber);
			AddExpectedPointerEvent(TPointerEvent::EButton1Up, expectedPos.iX, expectedPos.iY, iPointerNumber);
			break;
		case 4:
			// Test any point which has Y value at bottom of the screen
			// As pixels are zero counted and so it cannot be >= iPhysicalScreenSize.iHeight 
			simulatedPos.iY = iPhysicalScreenSize.iHeight-1;
			SimulatePointerEvent(TRawEvent::EButton1Down, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			SimulatePointerEvent(TRawEvent::EButton1Up, simulatedPos.iX, simulatedPos.iY, iPointerNumber);
			
			expectedPos.iY = GetShiftedYValue(simulatedPos.iY); 
			AddExpectedPointerEvent(TPointerEvent::EButton1Down, expectedPos.iX, expectedPos.iY, iPointerNumber);
			AddExpectedPointerEvent(TPointerEvent::EButton1Up, expectedPos.iX, expectedPos.iY, iPointerNumber);
			break;
		default:
			CActiveScheduler::Stop();
		}
	}
// Macro which is used for construction of CTPointerOffsetTestStep object
// and also used to call CreateTestL() which creates CTPointerOffsetTest and CTestBase objects
__WS_CONSTRUCT_STEP__(PointerOffsetTest)
