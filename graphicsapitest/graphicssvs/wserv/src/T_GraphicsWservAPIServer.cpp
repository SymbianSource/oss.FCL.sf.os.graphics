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

#include "T_GraphicsWservAPIServer.h"
#include "T_DataWsSession.h"
#include "T_DataWindowGroup.h"
#include "T_DataBlankWindow.h"
#include "T_DataWindow.h"
#include "T_DataBackedUpWindow.h"
#include "T_DataWindowGc.h"
#include "T_DataWsBitmap.h"
#include "T_DataWsGraphicBitmap.h"
#include "T_DataWsGraphicMsgBuf.h"
#include "T_DataWindowInfo.h"
#include "T_DataWsScreenDevice.h"
#include "T_DataFbsSession.h"
#include "T_DataFbsFont.h"
#include "T_DataFbsBitmap.h"
#include "T_DataBitmapUtil.h"
#include "T_DataFbsTypefaceStore.h"
#include "T_DataFbsColor256BitmapUtil.h"
#include "T_DataPalette.h"
#include "T_DataFbsBitmapDevice.h"
#include "T_DataFbsScreenDevice.h"
#include "T_DataAlgStyle.h"
#include "T_DataMeasureTextInput.h"
#include "T_DataMeasureTextOutput.h"
#include "T_DataWsGraphicAnimation.h"
#include "T_DataWsGraphicFrameRate.h"
#include "T_DataWsEvent.h"
#include "T_DataWsRedrawEvent.h"
#include "T_DataWsPriorityKeyEvent.h"
#include "T_DataSoundPlugIn.h"
#include "T_DataFbsFont.h"
#include "T_DataWsSprite.h"
#include "T_DataWsPointerCursor.h"
#include "T_DataDirectScreenAccess.h"
#include "T_DataDsaSession.h"
#include "T_DataFbsBitGc.h"
#include "T_DataAnimDll.h"
#include "T_DataAnim.h"
#include "T_DataAnimForMClass.h"
#include "T_DataWsGraphicBitmapAnimation.h"
#include "T_DataFrame.h"
//	EPOC Includes
#include <rsshared.h>

/**
 * @enum Constant Literals used.
 */
/*@{*/
// Graphics WSERV API
_LIT(KDataWsSession,			"RWsSession");
_LIT(KDataWindowGroup,			"RWindowGroup");
_LIT(KDataBlankWindow,			"RBlankWindow");
_LIT(KDataWindow,				"RWindow");
_LIT(KDataBackedUpWindow,		"RBackedUpWindow");
_LIT(KDataWindowGc,				"CWindowGc");
_LIT(KDataWsBitmap,				"CWsBitmap");
_LIT(KDataWsGraphicBitmap,		"CWsGraphicBitmap");
_LIT(KDataWsGraphicMsgBuf,		"RWsGraphicMsgBuf");
_LIT(KDataWsEvent,				"TWsEvent");
_LIT(KDataWsPriorityKeyEvent, 	"TWsPriorityKeyEvent");
_LIT(KDataWsRedrawEvent,		"TWsRedrawEvent");
_LIT(KDataWindowInfo,			"TWindowInfo");
_LIT(KDataWsScreenDevice,		"CWsScreenDevice");
_LIT(KDataWsGraphicAnimation,	"TWsGraphicAnimation");
_LIT(KDataWsGraphicFrameRate,	"TWsGraphicFrameRate");
_LIT(KDataSoundPlugIn,			"RSoundPlugIn");
_LIT(KDataFbsFont,				"CFbsFont");
_LIT(KDataWsSprite,             "RWsSprite");
_LIT(KDataWsPointerCursor,      "RWsPointerCursor");
_LIT(KDataCDirectScreenAccess,	"CDirectScreenAccess");
_LIT(KDataRDirectScreenAccess,	"RDirectScreenAccess");
_LIT(KDataFbsBitGc,				"CFbsBitGc");
_LIT(KDataAnimDll,				"RAnimDll");
_LIT(KDataAnim,					"RAnim");
_LIT(KDataAnimForMClass,		"RAnimForMClass");
_LIT(KDataWsGraphicBitmapAnimation, "CWsGraphicBitmapAnimation");
_LIT(KDataFrame,                "CFrame");
//from T_FBServAPI
_LIT(KRFbsSessionData, 			"RFbsSession");
_LIT(KCFbsFontData, 			"CFbsFont");
_LIT(KCFbsBitmapData, 			"CFbsBitmap");
_LIT(KTBitmapUtilData, 			"TBitmapUtil");
_LIT(KCFbsTypefaceStoreData,	"CFbsTypefaceStore");
_LIT(KCFbsColor256BitmapUtil,	"CFbsColor256BitmapUtil");
_LIT(KCPalette,					"CPalette");
_LIT(KCFbsBitmapDevice,			"CFbsBitmapDevice");
_LIT(KCFbsScreenDevice,			"CFbsScreenDevice");
_LIT(KTAlgStyle,				"TAlgStyle");
_LIT(KTMeasureTextInput,		"TMeasureTextInput");
_LIT(KTMeasureTextOutput,		"TMeasureTextOutput");
/*@}*/


inline CDataWrapper* CT_GraphicsWservAPIServer::CT_GraphicsWservAPIBlock::CreateDataL( const TDesC& aData )
	{
	CDataWrapper*	wrapper = NULL;

	if ( aData==KDataWsSession )
		{
		wrapper=CT_DataWsSession::NewL();
		}
	else if ( aData==KDataWindowGroup )
		{
		wrapper=CT_DataWindowGroup::NewL();
		}
	else if ( aData==KDataBlankWindow )
		{
		wrapper=CT_DataBlankWindow::NewL();
		}
	if ( aData==KDataWindow )
		{
		wrapper=CT_DataWindow::NewL();
		}
	else if ( aData==KDataBackedUpWindow )
		{
		wrapper=CT_DataBackedUpWindow::NewL();
		}
	else if ( aData==KDataWindowGc )
		{
		wrapper=CT_DataWindowGc::NewL();
		}
	else if ( aData==KDataWsBitmap )
		{
		wrapper=CT_DataWsBitmap::NewL();
		}
	else if ( aData==KDataWsGraphicBitmap )
		{
		wrapper=CT_DataWsGraphicBitmap::NewL();
		}
	else if ( aData==KDataWsGraphicMsgBuf )
		{
		wrapper=CT_DataWsGraphicMsgBuf::NewL();
		}
	else if ( aData==KDataWsEvent )
		{
		wrapper=CT_DataWsEvent::NewL();
		}
	else if ( aData==KDataWsPriorityKeyEvent )
		{
		wrapper=CT_DataWsPriorityKeyEvent::NewL();
		}
	else if ( aData==KDataWsRedrawEvent )
		{
		wrapper=CT_DataWsRedrawEvent::NewL();
		}
	else if ( aData==KDataWindowInfo )
		{
		wrapper=CT_DataWindowInfo::NewL();
		}
	else if ( aData==KDataWsScreenDevice )
		{
		wrapper=CT_DataWsScreenDevice::NewL();
		}
	else if( aData == KDataSoundPlugIn)
		{
		wrapper = CT_DataSoundPlugIn::NewL();
		}
	else if (aData==KDataCDirectScreenAccess)
		{
		wrapper=CT_DataDirectScreenAccess::NewL();
		}
	else if (aData==KDataRDirectScreenAccess)
		{
		wrapper=CT_DataDsaSession::NewL();
		}
// from T_FBServAPI
	else if (aData == KRFbsSessionData)
		{
		wrapper=CT_DataFbsSession::NewL();
		}
	else if (aData == KCFbsFontData)
		{
		wrapper=CT_DataFbsFont::NewL();
		}
	else if (aData == KCFbsBitmapData)
		{
		wrapper=CT_DataFbsBitmap::NewL();
		}
	else if (aData == KTBitmapUtilData)
		{
		wrapper=CT_DataBitmapUtil::NewL();
		}
	else if (aData == KCFbsTypefaceStoreData)
		{
		wrapper=CT_DataFbsTypefaceStore::NewL();
		}
	else if (aData == KCFbsColor256BitmapUtil)
		{
		wrapper=CT_DataFbsColor256BitmapUtil::NewL();
		}
	else if (aData == KCPalette)
		{
		wrapper=CT_DataPalette::NewL();
		}
	else if ( aData==KCFbsBitmapDevice )
		{
		wrapper=CT_DataFbsBitmapDevice::NewL();
		}
	else if ( aData==KCFbsScreenDevice )
		{
		wrapper=CT_DataFbsScreenDevice::NewL();
		}
	else if ( aData==KTAlgStyle )
		{
		wrapper=CT_DataAlgStyle::NewL();
		}
	else if ( aData==KTMeasureTextInput )
		{
		wrapper=CT_DataMeasureTextInput::NewL();
		}
	else if ( aData==KTMeasureTextOutput )
		{
		wrapper=CT_DataMeasureTextOutput::NewL();
		}
	else if ( aData==KDataWsGraphicAnimation )
		{
		wrapper=new (ELeave) CT_DataWsGraphicAnimation();
		}
	else if ( aData==KDataWsGraphicFrameRate )
		{
		wrapper=new (ELeave) CT_DataWsGraphicFrameRate();
		}
	else if (aData==KDataFbsFont)
		{
		wrapper=CT_DataFbsFont::NewL();
		}
	//for RWsSprite
	else if (aData==KDataWsSprite)
		{
		wrapper= CT_DataWsSprite::NewL();
		}
	//for RWsPointerCursor
	else if (aData==KDataWsPointerCursor)
		{
		wrapper=CT_DataWsPointerCursor::NewL();
		}
	else if (aData == KDataFbsBitGc)
		{
		wrapper=CT_DataFbsBitGc::NewL();
		}	
	else if( aData == KDataAnimDll)
		{
		wrapper = CT_DataAnimDll::NewL();
		}
	else if( aData == KDataAnim)
		{
		wrapper = CT_DataAnim::NewL();
		}
	else if(aData==KDataAnimForMClass)
		{
		wrapper=CT_DataAnimForMClass::NewL();
		}
	else if( aData == KDataWsGraphicBitmapAnimation)
		{
		wrapper = CT_DataWsGraphicBitmapAnimation::NewL();
		}
	else if( aData == KDataFrame)
		{
		wrapper = CT_DataFrame::NewL();
		}
	return wrapper;
	}

CT_GraphicsWservAPIServer* CT_GraphicsWservAPIServer::NewL()
/**
 * @return - Instance of the test server
 * Same code for Secure and non-secure variants
 * Called inside the MainL() function to create and start the
 * CTestServer derived server.
 */
	{
	CT_GraphicsWservAPIServer*	server = new (ELeave) CT_GraphicsWservAPIServer();
	CleanupStack::PushL(server);
	// CServer base class call
	//RProcess	handle = RProcess();
	//TParsePtrC	serverName(handle.FileName());
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
	CT_GraphicsWservAPIServer*	server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CT_GraphicsWservAPIServer::NewL());
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
