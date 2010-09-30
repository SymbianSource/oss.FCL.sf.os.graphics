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
#include "eglrendering.h"

#define KDefaultScreenNo 0

class CWsRedrawHandler;

class CWsCanvas: public CBase
{
public:
	static CWsCanvas* NewL(TInt, const TPoint&);
	~CWsCanvas();

	void DrawNow();
	void Redraw();
	void Draw(const TRect&);

	RWsSession& Session() {return iWs;}
	RWindow& Window() {return iWin;}
	RWindowGroup& Group() {return iGrp;}
	CWindowGc* Gc() {return iGc;}
	CWsScreenDevice* Screen() {return iScr;}
    inline TSize ScreenSize() const;
	
private:
	CWsCanvas(TInt, const TPoint&);
	void ConstructL();
	
private:
    TInt iScrId;
	TPoint iPos;
	TSize iSz;
	RWsSession iWs;
	RWindowGroup iGrp;
	RWindow iWin;
	CWsScreenDevice* iScr;
	CWindowGc* iGc;
	CWsRedrawHandler* iRedrawHandler;
};

class CWsRedrawHandler: public CActive
{
public:
	CWsRedrawHandler(CWsCanvas&);
	~CWsRedrawHandler();
	
	void RunL();
	void DoCancel();
	
private:
	CWsCanvas& iCanvas;	
};

class CWsApp: public CBase
    {
public:
	static CWsApp* NewL();
	~CWsApp();
	void Start();
	void Stop();

private:
	CWsApp();
    void ConstructL();
    
    CWsCanvas* iAppView;    
	CEGLRendering* iDemo;	
	TBool iCallWindow;	
	
    TPoint iQvgaPos;
    TPoint iQhdPos;
	TBool iQhd;
	TPoint iPos;
	TSize iSz;
	TInt iScrId;
    };

/**
 * Create a canvas to draw to.
 * 
 * @param aScrId	Screen number to use
 * @param aPos		Position on screen to use
 */
CWsCanvas* CWsCanvas::NewL(TInt aScrId, const TPoint& aPos)
	{
	CWsCanvas* c = new(ELeave) CWsCanvas(aScrId, aPos);
	CleanupStack::PushL(c);
	c->ConstructL();
	CleanupStack::Pop(c);

	return c;
	}

CWsCanvas::CWsCanvas(TInt aScrId, const TPoint& aPos):
	iScrId(aScrId), iPos(aPos)
	{
	}

CWsCanvas::~CWsCanvas()
	{
	delete iGc;
	delete iScr;
	iGrp.Close();
	iWin.Close();
	delete iRedrawHandler;
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

	iRedrawHandler = new(ELeave) CWsRedrawHandler(*this);
	iWs.SetFocusScreen(iScrId);
	}

void CWsCanvas::DrawNow()
	{
	iWin.Invalidate();
	Redraw();
	}

void CWsCanvas::Redraw()
	{
	iWin.BeginRedraw();
	iGc->Activate(iWin);
	Draw(TRect(TPoint(), iSz));
	iGc->Deactivate();
	iWin.EndRedraw();

	iWs.Flush();
	}

void CWsCanvas::Draw(const TRect& /*aRect*/)
	{
	}

inline TSize CWsCanvas::ScreenSize() const
    {
    return iSz;
    }

CWsRedrawHandler::CWsRedrawHandler(CWsCanvas& aCanvas):
        CActive(CActive::EPriorityStandard),
        iCanvas(aCanvas)
	{
	CActiveScheduler::Add(this);
	
	iStatus = KRequestPending;	
	iCanvas.Session().RedrawReady(&iStatus);
	SetActive();
	}

CWsRedrawHandler::~CWsRedrawHandler()
	{
	Cancel();
	}

void CWsRedrawHandler::RunL()
	{
	TWsRedrawEvent e;
	iCanvas.Session().GetRedraw(e);
	if (e.Handle() == (TInt) &iCanvas)
		{
		iCanvas.Redraw();		
		}

	iStatus = KRequestPending;
	iCanvas.Session().RedrawReady(&iStatus);
	SetActive();
	}

void CWsRedrawHandler::DoCancel()
	{
	iCanvas.Session().RedrawReadyCancel();
	}


CWsApp::CWsApp():
	iQvgaPos(160,60),
	iQhdPos(0,0),
	iQhd(ETrue)
	{
	}

CWsApp* CWsApp::NewL()
	{
	RDebug::Printf("CWsApp::NewL()");
	CWsApp* app = new(ELeave) CWsApp;
	CleanupStack::PushL(app);
	app->ConstructL();
	CleanupStack::Pop(app);
	
	return app;
	}

/**
 * Constructor for CWsApp
 * 
 * @note This constructor looks at the command line argument, if any, supplied when
 * 		 launching the application.  If specified, its used as the screen number to
 * 		 target the output of the program.  By default, screen 0 is used.
 */
void CWsApp::ConstructL()
    {
	RDebug::Printf("CWsApp::ConstructL()");
    iPos = iQhd? iQhdPos : iQvgaPos;

	iScrId = KDefaultScreenNo;
	if (User::CommandLineLength() > 0)
		{
		TBuf<1> arg;
		User::CommandLine(arg);
		iScrId = (TInt)(arg[0]-'0');
		}
	RDebug::Printf("CWsApp::ConstructL() 1");	    
	iAppView = CWsCanvas::NewL(iScrId, iPos);
	RDebug::Printf("CWsApp::ConstructL() 2");	    
	iDemo = CEGLRendering::NewL(iAppView->Window(), iQhd);
	RDebug::Printf("CWsApp::ConstructL() 3");	    
	iDemo->Start();
	RDebug::Printf("CWsApp::ConstructL() 4");	    

	iSz = iAppView->ScreenSize();
	RDebug::Printf("CWsApp::ConstructL() 5");	    

	//Connstruct dialog Box
	// Get a 212x76 pixel box in the centre of the window.
	TRect rcDialog(TRect(iPos, iSz));

#ifdef PORTRAIT_MODE
	rcDialog.Shrink((rcDialog.Width() - 76) / 2, (rcDialog.Height() - 212) / 2);
#else	
	rcDialog.Shrink((rcDialog.Width() - 212) / 2, (rcDialog.Height() - 76) / 2);
#endif

	iCallWindow = EFalse;
	}

void CWsApp::Start()
	{
	RDebug::Printf("CWsApp::Start");
	CActiveScheduler::Start();
	}

void CWsApp::Stop()
	{
	CActiveScheduler::Stop();
	}

CWsApp::~CWsApp()
	{	
	delete iDemo;
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
	RDebug::Printf("ebt ::MainL");
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
	RDebug::Printf("ebt ::E32Main");

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
