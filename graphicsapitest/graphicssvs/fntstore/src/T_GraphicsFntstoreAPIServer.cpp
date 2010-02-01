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

#include "T_GraphicsFntstoreAPIServer.h"
#include "T_DataAlgStyle.h"
#include "T_DataFontStore.h"
#include "T_DataBitmapFont.h"
#include "T_DataOpenFontGlyphData.h"
#include "T_DataOpenFontRasterizerContext.h"

//	EPOC Includes
#include <rsshared.h>

/**
 * @enum Constant Literals used.
 */
/*@{*/
// Graphics Fntstore API

_LIT(KTAlgStyle,						"TAlgStyle");
_LIT(KCFontStore,						"CFontStore");
_LIT(KCBitmapFont,						"CBitmapFont");
_LIT(KTOpenFontGlyphData,				"TOpenFontGlyphData");
_LIT(KCOpenFontRasterizerContext,		"COpenFontRasterizerContext");
/*@}*/


inline CDataWrapper* CT_GraphicsFntstoreAPIServer::CT_GraphicsFntstoreAPIBlock::CreateDataL( const TDesC& aData )
	{
	CDataWrapper*	wrapper = NULL;

	if (aData == KCFontStore)
		{
		wrapper = CT_DataFontStore::NewL();
		}
	else if (aData == KCBitmapFont)
		{
		wrapper = CT_DataBitmapFont::NewL();
		}
	else if (aData == KTAlgStyle)
		{
		wrapper = CT_DataAlgStyle::NewL();
		}
	else if (aData == KTOpenFontGlyphData)
		{
		wrapper = CT_DataOpenFontGlyphData::NewL();
		}
	else if (aData == KCOpenFontRasterizerContext)
		{
		wrapper = CT_DataOpenFontRasterizerContext::NewL();
		}
	
	return wrapper;
	}

CT_GraphicsFntstoreAPIServer* CT_GraphicsFntstoreAPIServer::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CT_GraphicsFntstoreAPIServer*	server = new (ELeave) CT_GraphicsFntstoreAPIServer();
	CleanupStack::PushL(server);
	// CServer base class call
	server->ConstructL();		/*Parsing the server name from the file name*/
	CleanupStack::Pop(server);
	return server;
	}


TInt LoadDrivers()
	{
#ifdef __WINS__
	#define KPDDName _L("ECDRV")
	#define KLDDName _L("ECOMM")
#else
	#define KPDDName _L("EUART1")
	#define KLDDName _L("ECOMM")
#endif
	TInt rerr = KErrNone;

	rerr = StartC32();
	if( rerr!=KErrNone && rerr!=KErrAlreadyExists )
		{
		return rerr;
		}

	rerr = User::LoadPhysicalDevice(KPDDName);
	if(rerr != KErrNone && rerr != KErrAlreadyExists)
		{
		return rerr;
		}
	rerr = User::LoadLogicalDevice(KLDDName);
	if(rerr != KErrNone && rerr != KErrAlreadyExists)
		{
		return rerr;
		}
	return KErrNone;
	}

LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler*	sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CT_GraphicsFntstoreAPIServer*	server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CT_GraphicsFntstoreAPIServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	TInt rerr = LoadDrivers();
	if( rerr != KErrNone )
		{
		return rerr;
		}

	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
#if (defined TRAP_IGNORE)
	TRAP_IGNORE(MainL());
#else
	TRAPD(err,MainL());
#endif
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
