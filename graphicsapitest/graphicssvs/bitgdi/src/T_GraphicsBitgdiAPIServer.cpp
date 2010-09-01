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

#include "T_GraphicsBitgdiAPIServer.h"
#include "T_DataBitmapUtil.h"
#include "T_DataFbsBitmap.h"
#include "T_DataWsBitmap.h"
#include "T_DataFbsBitmapDevice.h"
#include "T_DataFbsScreenDevice.h"
#include "T_DataAlgStyle.h"
#include "T_DataFbsBitGc.h"
#include "T_DataFbsSession.h"
#include "T_DataWsSession.h"
#include "T_DataPalette.h"
#include "T_DataPolygonFiller.h"
#include "T_DataFbsFont.h"
#include "T_DataFbsTypefaceStore.h"
#include "T_DataWsScreenDevice.h"
#include "T_DataFbsBitGcBitmap.h"
#include "T_DataFbsBitGcFont.h"
#include "T_DataLinkedTypefaceSpecification.h"

//	EPOC Includes
#include <rsshared.h>

/**
 * @enum Constant Literals used.
 */
/*@{*/
// Graphics BITGDI API
_LIT(KDataFbsBitGc,				"CFbsBitGc");
_LIT(KDataPolygonFiller,       	"CPolygonFiller");

//from T_FBServAPI
_LIT(KRFbsSessionData, 			"RFbsSession");
_LIT(KCFbsFontData, 			"CFbsFont");
_LIT(KCFbsBitmapData, 			"CFbsBitmap");
_LIT(KCWsBitmapData, 			"CWsBitmap");
_LIT(KTBitmapUtilData, 			"TBitmapUtil");
_LIT(KCFbsTypefaceStoreData,	"CFbsTypefaceStore");
_LIT(KCFbsColor256BitmapUtil,	"CFbsColor256BitmapUtil");
_LIT(KCPalette,					"CPalette");
_LIT(KCFbsBitmapDevice,			"CFbsBitmapDevice");
_LIT(KCFbsScreenDevice,			"CFbsScreenDevice");
_LIT(KTAlgStyle,				"TAlgStyle");
_LIT(KTMeasureTextInput,		"TMeasureTextInput");
_LIT(KTMeasureTextOutput,		"TMeasureTextOutput");
_LIT(KRWsSession,				"RWsSession");
_LIT(KCWsScreenDevice,			"CWsScreenDevice");
_LIT(KCFbsBitGcBitmap,			"CFbsBitGcBitmap");
_LIT(KCFbsBitGcFont,			"CFbsBitGcFont");

//from T_GraphicsFntstoreAPI
_LIT(KCLinkedTypefaceSpecification,	"CLinkedTypefaceSpecification");

/*@}*/


inline CDataWrapper* CT_GraphicsBitgdiAPIServer::CT_GraphicsBitgdiAPIBlock::CreateDataL( const TDesC& aData )
	{
	CDataWrapper*	wrapper = NULL;

	if ( aData==KRFbsSessionData )
		{
		wrapper=CT_DataFbsSession::NewL();
		}
	else if ( aData==KCFbsScreenDevice )
		{
		wrapper=CT_DataFbsScreenDevice::NewL();
		}
	else if ( aData==KDataFbsBitGc )
		{
		wrapper=CT_DataFbsBitGc::NewL();
		}
	else if ( aData==KCPalette )
		{
		wrapper=CT_DataPalette::NewL();
		}
	else if (aData==KDataPolygonFiller)
		{
		wrapper=CT_DataPolygonFiller::NewL();
		}
	else if (aData == KCFbsFontData)
        {
		wrapper=CT_DataFbsFont::NewL();
        }
	else if (aData==KCFbsBitmapData)
		{
		wrapper=CT_DataFbsBitmap::NewL();
		}
	else if (aData==KCWsBitmapData)
		{
		wrapper=CT_DataWsBitmap::NewL();
		}
	else if (aData==KRWsSession)
		{
		wrapper=CT_DataWsSession::NewL();
		}
	else if (aData==KCFbsTypefaceStoreData)
		{
		wrapper=CT_DataFbsTypefaceStore::NewL();
		}
	else if (aData==KCWsScreenDevice)
		{
		wrapper=CT_DataWsScreenDevice::NewL();
		}
	else if (aData==KCFbsBitmapDevice)
		{
		wrapper=CT_DataFbsBitmapDevice::NewL();
		}
	else if (aData==KCFbsBitGcBitmap)
		{
		wrapper=CT_DataFbsBitGcBitmap::NewL();
		}
	else if (aData==KCFbsBitGcFont)
		{
		wrapper=CT_DataFbsBitGcFont::NewL();
		}
	// from fontstore
	else if (aData==KTAlgStyle)
		{
		wrapper=CT_DataAlgStyle::NewL();
		}	
	else if (aData==KCLinkedTypefaceSpecification)
		{
		wrapper=CT_DataLinkedTypefaceSpecification::NewL();
		}
	return wrapper;
	}

CT_GraphicsBitgdiAPIServer* CT_GraphicsBitgdiAPIServer::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CT_GraphicsBitgdiAPIServer*	server = new (ELeave) CT_GraphicsBitgdiAPIServer();
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
	CT_GraphicsBitgdiAPIServer*	server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CT_GraphicsBitgdiAPIServer::NewL());
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
