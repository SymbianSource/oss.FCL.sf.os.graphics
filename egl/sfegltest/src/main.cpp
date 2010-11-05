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

#include <e32std.h>
#include <w32std.h>
#include <e32math.h>
#include <e32keys.h>
#include <bacline.h>
#include "gles1cube.h"
#include "vgline.h"

#define KDefaultScreenNo 0

class CWsCanvas: public CBase
{
public:
	static CWsCanvas* NewL(TInt, const TPoint&);
	~CWsCanvas();

	RWsSession& Session() { return iWs; }
	RWindow& Window() { return iWin; }
    const TSize& ScreenSize() const { return iSz; }

private:
	CWsCanvas(TInt, const TPoint&);
	void ConstructL();

private:
    const TInt iScrId;
	const TPoint iPos;
	TSize iSz;
	RWsSession iWs;
	RWindowGroup iGrp;
	RWindow iWin;
	CWsScreenDevice* iScr;
	CWindowGc* iGc;
};

/**
 * Create a canvas to draw to.
 *
 * @param aScrId	Screen number to use
 * @param aPos		Position on screen to use
 */
CWsCanvas* CWsCanvas::NewL(TInt aScrId, const TPoint& aPos)
	{
	CWsCanvas* self = new (ELeave) CWsCanvas(aScrId, aPos);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CWsCanvas::CWsCanvas(TInt aScrId, const TPoint& aPos)
    :   iScrId(aScrId)
    ,   iPos(aPos)
	{
	}

CWsCanvas::~CWsCanvas()
	{
	delete iGc;
	delete iScr;
	iGrp.Close();
	iWin.Close();
	iWs.Close();
	}

/**
 * Construct the application canvas.
 *
 * Here we setup the collaboration with the Window Server.  We want to get a window
 * on the appropriate screen, and setup a redraw handler so we can re-paint our window
 * when the Window Server wants us to.
 */
void CWsCanvas::ConstructL()
	{
	TInt err = iWs.Connect();
	User::LeaveIfError(err);

	iScr = new(ELeave) CWsScreenDevice(iWs);
	err = iScr->Construct(iScrId);
	User::LeaveIfError(err);

	err = iScr->CreateContext(iGc);
	User::LeaveIfError(err);

	iGrp = RWindowGroup(iWs);
	err = iGrp.Construct(0xbadf00d, ETrue, iScr);
	User::LeaveIfError(err);

	iWin = RWindow(iWs);
	err = iWin.Construct(iGrp, (TUint32)this);
	User::LeaveIfError(err);

	iSz = iScr->SizeInPixels();
	iWin.SetExtent(iPos, iSz);
	iWin.SetBackgroundColor();
	iWin.Activate();

	iWs.Flush();

	iWs.SetFocusScreen(iScrId);
	}

class MWsEventObserver
    {
public:
    virtual void PointerEvent() = 0;
    };

class CWsEventHandler : public CActive
    {
public:
    CWsEventHandler(RWsSession& aSession, RWindow& aWindow, MWsEventObserver& aObserver);
    ~CWsEventHandler();
private:
    void ConstructL();
    void RequestEvent();
    void RunL();
    void DoCancel();
private:
    RWsSession& iSession;
    RWindow& iWindow;
    MWsEventObserver& iObserver;
    };

CWsEventHandler::CWsEventHandler(RWsSession& aSession, RWindow& aWindow, MWsEventObserver& aObserver)
    :   CActive(CActive::EPriorityStandard)
    ,   iSession(aSession)
    ,   iWindow(aWindow)
    ,   iObserver(aObserver)
    {
    RDebug::Printf("[EBT] CWsEventHandler::CWsEventHandler");
    CActiveScheduler::Add(this);
    RequestEvent();
    }

CWsEventHandler::~CWsEventHandler()
    {
    RDebug::Printf("[EBT] CWsEventHandler::~CWsEventHandler");
    Cancel();
    }

void CWsEventHandler::ConstructL()
    {
    CActiveScheduler::Add(this);
    RequestEvent();
    }

void CWsEventHandler::RequestEvent()
    {
    iStatus = KRequestPending;
    iSession.EventReady(&iStatus);
    SetActive();
    }

void CWsEventHandler::RunL()
    {
    if (KErrNone == iStatus.Int())
        {
        TWsEvent event;
        iSession.GetEvent(event);
        RequestEvent();
        switch (event.Type())
            {
            case EEventPointer:
                iObserver.PointerEvent();
                break;
            }
        }
    }

void CWsEventHandler::DoCancel()
    {
    iSession.EventReadyCancel();
    }

class CWsApp : public CBase
             , public MWsEventObserver
    {
public:
    static CWsApp* NewL();
    ~CWsApp();
    void Start();
    void Stop();

    // MWsEventObserver
    void PointerEvent();

private:
    CWsApp();
    void ConstructL();

private:
    CWsCanvas* iAppView;
    CWsEventHandler* iEventHandler;
    CEGLRendering* iDemo;
    TPoint iPos;
    TSize iSz;
    TInt iScrId;
    };

CWsApp::CWsApp()
	{
	}

CWsApp* CWsApp::NewL()
	{
	RDebug::Printf("[EBT] CWsApp::NewL");
	CWsApp* self = new (ELeave) CWsApp;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

_LIT(KOptionScreen, "-screen");

typedef CEGLRendering* (*RendererFactoryFunctionL)(RWindow&);

/**
 * Constructor for CWsApp
 *
 * @note This constructor looks at the command line argument, if any, supplied when
 *		 launching the application.  If specified, its used as the screen number to
 *		 target the output of the program.  By default, screen 0 is used.
 */
void CWsApp::ConstructL()
    {
	RDebug::Printf("[EBT] CWsApp::ConstructL");

	iScrId = KDefaultScreenNo;
	HBufC* rendererName = 0;
	CCommandLineArguments* args = CCommandLineArguments::NewLC();
	for (TInt i=1; i<args->Count(); ++i)
		{
		const TPtrC arg = args->Arg(i);
		if (!arg.Compare(KOptionScreen))
			{
			if (++i < args->Count())
				{
				TLex lex(args->Arg(i));
				User::LeaveIfError(lex.Val(iScrId));
				RDebug::Printf("[EBT] CWsApp::ConstructL screenId %d", iScrId);
				}
			}
		else if (!rendererName)
			{
			RDebug::Print(_L("[EBT] CWsApp::ConstructL rendererName %S"), &arg);
			rendererName = HBufC::NewL(arg.Length());
			rendererName->Des() = arg;
			}
		else
			{
			RDebug::Print(_L("[EBT] CWsApp::ConstructL ignoring argument %S"), &arg);
			}
		}

	CleanupStack::PopAndDestroy(args);
	if (rendererName)
		{
		CleanupStack::PushL(rendererName);
		}

	iAppView = CWsCanvas::NewL(iScrId, iPos);
    iSz = iAppView->ScreenSize();
	iEventHandler = new (ELeave) CWsEventHandler(iAppView->Session(), iAppView->Window(), *this);

	RendererFactoryFunctionL factoryFunctionL = CVGLine::NewL;
	if (rendererName)
		{
		if (!rendererName->Des().Compare(CGLES1Cube::Name()))
			{
			factoryFunctionL = CGLES1Cube::NewL;
			}
		if (!rendererName->Des().Compare(CVGLine::Name()))
			{
			factoryFunctionL = CVGLine::NewL;
			}
		CleanupStack::PopAndDestroy(rendererName);
		}
	iDemo = (*factoryFunctionL)(iAppView->Window());
	}

void CWsApp::Start()
	{
	RDebug::Printf("[EBT] CWsApp::Start");
	CActiveScheduler::Start();
	}

void CWsApp::Stop()
	{
    RDebug::Printf("[EBT] CWsApp::Stop");
    CActiveScheduler::Stop();
	}

void CWsApp::PointerEvent()
    {
    RDebug::Printf("[EBT] CWsApp::PointerEvent");
    Stop();
    }

CWsApp::~CWsApp()
	{
	delete iDemo;
	delete iEventHandler;
	delete iAppView;
	}

/**
 * Application second level entry point.
 *
 * Launches the application specific class CWsApp and calls Start() on it.
 *
 * @pre Active scheduler established.
 */
void MainL()
	{
	RDebug::Printf("[EBT] ::MainL");
	CWsApp* app = CWsApp::NewL();
	CleanupStack::PushL(app);
    app->Start();
	CleanupStack::PopAndDestroy(1, app);
	}

/**
 * Application entry point.
 *
 * This sets up the application environment active scheduler and runs MainL under a trap
 * harness.
 */
GLDEF_C TInt E32Main()
    {
	RDebug::Printf("[EBT] ::E32Main");

	CTrapCleanup* tc = CTrapCleanup::New();
	if (!tc)
		{
		return KErrNoMemory;
		}

	CActiveScheduler* as = new CActiveScheduler;
	if (!as)
		{
		delete tc;
		return KErrNoMemory;
		}

	CActiveScheduler::Install(as);
	TRAPD(err, MainL());

	delete as;
	delete tc;
	return err;
    }

