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

/**
@test
@internalComponent

This contains CT_DataFbsSession
*/

#include "T_DataFbsSession.h"
#include "T_GraphicsUtil.h"


/*@{*/
///	Parameters
_LIT(KSession,										"session");
_LIT(KInvalidSession,								"invalid_session");
_LIT(KNullSession,									"null_session");
_LIT(KExpectedCallBack,								"expected");
_LIT(KExpectedDiff,									"expected_diff");
_LIT(KFbsMessage,									"fbs_message");
_LIT(KBitmap,										"bitmap");
_LIT(KFbsParameter0,								"parameter0");
_LIT(KFbsParameter1,								"parameter1");
_LIT(KFbsParameter2,								"parameter2");
_LIT(KFbsParameter3,								"parameter3");
_LIT(KVersionName,									"name");
_LIT(KVersionBuild,									"build");
_LIT(KVersionMajor,									"major");
_LIT(KVersionMinor,									"minor");

///	Commands
_LIT(KCmdConstructor,								"Constructor");
_LIT(KCmdConnect,									"Connect");
_LIT(KCmdDisconnect,								"Disconnect");
_LIT(KCmdGetSession,								"GetSession");
_LIT(KCmdCallBack,									"CallBack");
_LIT(KCmdSetCallBack,								"SetCallBack");
_LIT(KCmdResetCallBack,								"ResetCallBack");
_LIT(KCmdResourceCount,								"ResourceCount");
_LIT(KCmdSendCommand,								"SendCommand");
_LIT(KCmdVersion,									"Version");
_LIT(KCmdHeapBase,									"HeapBase");
_LIT(KCmdSessionHandle,								"SessionHandle");

_LIT(KFbsMessInit,									"EFbsMessInit");
_LIT(KFbsMessShutdown,								"EFbsMessShutdown");
_LIT(KFbsMessClose,									"EFbsMessClose");
_LIT(KFbsMessResourceCount,							"EFbsMessResourceCount");
_LIT(KFbsMessNumTypefaces,							"EFbsMessNumTypefaces");
_LIT(KFbsMessTypefaceSupport,						"EFbsMessTypefaceSupport");
_LIT(KFbsMessFontHeightInTwips,						"EFbsMessFontHeightInTwips");
_LIT(KFbsMessFontHeightInPixels,					"EFbsMessFontHeightInPixels");
_LIT(KFbsMessAddFontStoreFile,						"EFbsMessAddFontStoreFile");
_LIT(KFbsMessInstallFontStoreFile,					"EFbsMessInstallFontStoreFile");
_LIT(KFbsMessRemoveFontStoreFile,					"EFbsMessRemoveFontStoreFile");
_LIT(KFbsMessSetPixelHeight,						"EFbsMessSetPixelHeight");
_LIT(KFbsMessGetFontById,							"EFbsMessGetFontById");
_LIT(KFbsMessFontDuplicate,							"EFbsMessFontDuplicate");
_LIT(KFbsMessBitmapCreate,							"EFbsMessBitmapCreate");
_LIT(KFbsMessBitmapResize,							"EFbsMessBitmapResize");
_LIT(KFbsMessBitmapDuplicate,						"EFbsMessBitmapDuplicate");
_LIT(KFbsMessBitmapLoad,							"EFbsMessBitmapLoad");
_LIT(KFbsMessDefaultAllocFail,						"EFbsMessDefaultAllocFail");
_LIT(KFbsMessDefaultMark,							"EFbsMessDefaultMark");
_LIT(KFbsMessDefaultMarkEnd,						"EFbsMessDefaultMarkEnd");
_LIT(KFbsMessUserAllocFail,							"EFbsMessUserAllocFail");
_LIT(KFbsMessUserMark,								"EFbsMessUserMark");
_LIT(KFbsMessUserMarkEnd,							"EFbsMessUserMarkEnd");
_LIT(KFbsMessHeapCheck,								"EFbsMessHeapCheck");
_LIT(KFbsMessRasterize,								"EFbsMessRasterize");
_LIT(KFbsMessFaceAttrib,							"EFbsMessFaceAttrib");
_LIT(KFbsMessHasCharacter,							"EFbsMessHasCharacter");
_LIT(KFbsMessSetDefaultGlyphBitmapType,				"EFbsMessSetDefaultGlyphBitmapType");
_LIT(KFbsMessGetDefaultGlyphBitmapType,				"EFbsMessGetDefaultGlyphBitmapType");
_LIT(KFbsMessFontNameAlias,							"EFbsMessFontNameAlias");
_LIT(KFbsMessBitmapCompress,						"EFbsMessBitmapCompress");
_LIT(KFbsMessGetHeapSizes,							"EFbsMessGetHeapSizes");
_LIT(KFbsMessGetNearestFontToDesignHeightInTwips,	"EFbsMessGetNearestFontToDesignHeightInTwips");
_LIT(KFbsMessGetNearestFontToMaxHeightInTwips,		"EFbsMessGetNearestFontToMaxHeightInTwips");
_LIT(KFbsMessGetNearestFontToDesignHeightInPixels,	"EFbsMessGetNearestFontToDesignHeightInPixels");
_LIT(KFbsMessGetNearestFontToMaxHeightInPixels,		"EFbsMessGetNearestFontToMaxHeightInPixels");
_LIT(KFbsMessShapeText,								"EFbsMessShapeText");
_LIT(KFbsMessShapeDelete,							"EFbsMessShapeDelete");
_LIT(KFbsMessDefaultLanguageForMetrics,				"EFbsMessDefaultLanguageForMetrics");
_LIT(KFbsMessSetTwipsHeight,						"EFbsMessSetTwipsHeight");
_LIT(KFbsMessGetTwipsHeight,						"EFbsMessGetTwipsHeight");
_LIT(KFbsCompress,									"EFbsCompress");
_LIT(KFbsMessBitmapBgCompress,						"EFbsMessBitmapBgCompress");
_LIT(KFbsSetSystemDefaultTypefaceName,				"EFbsSetSystemDefaultTypefaceName");
_LIT(KFbsGetAllBitmapHandles,						"EFbsGetAllBitmapHandles");
_LIT(KFbsMessCreateLinkedTypeface,					"EFbsMessCreateLinkedTypeface");
_LIT(KFbsMessSetHeapFail,							"EFbsMessSetHeapFail");
_LIT(KFbsMessHeapCount,								"EFbsMessHeapCount");
_LIT(KFbsMessSetHeapReset,							"EFbsMessSetHeapReset");
_LIT(KFbsMessSetHeapCheck,							"EFbsMessSetHeapCheck");
_LIT(KFbsMessHeap,									"EFbsMessHeap");
_LIT(KFbsMessLinkedCache,							"EFbsMessLinkedCache");
_LIT(KFbsMessBitmapClean,							"EFbsMessBitmapClean");
/*@}*/

/**
* Two phase constructor
*/
CT_DataFbsSession* CT_DataFbsSession::NewL()
	{
	CT_DataFbsSession*	ret = new (ELeave) CT_DataFbsSession();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsSession::CT_DataFbsSession()
:	CDataWrapperBase()
,	iSession(NULL)
,	iResourceCount(-1)
,	iCallBackCalled(EFalse)
	{
	}

/**
* Second phase construction
*/
void CT_DataFbsSession::ConstructL()
	{
	}

/**
* Protected destructor.
*/
CT_DataFbsSession::~CT_DataFbsSession()
	{
	}

/**
* Process a command read from the ini file
*
* @param aDataWrapper	test step requiring command to be processed
* @param aCommand	the command to process
* @param aSection		the entry in the ini file requiring the command to be processed
*
* @return ETrue if the command is processed
*/
TBool CT_DataFbsSession::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret=ETrue;

	if (aCommand == KCmdConstructor)
		{
		DoCmdConstructor();
		}
	else if (aCommand == KCmdConnect)
		{
		DoCmdConnect(aSection);
		}
	else if (aCommand == KCmdDisconnect)
		{
		DoCmdDisconnect();
		}
	else if (aCommand == KCmdGetSession)
		{
		DoCmdGetSession(aSection);
		}
	else if (aCommand == KCmdCallBack)
		{
		DoCmdCallBack(aSection);
		}
	else if (aCommand == KCmdResetCallBack)
		{
		DoCmdResetCallBack();
		}
	else if (aCommand == KCmdSetCallBack)
		{
		DoCmdSetCallBack();
		}
	else if (aCommand == KCmdResourceCount)
		{
		DoCmdResourceCount(aSection);
		}
	else if (aCommand == KCmdSendCommand)
		{
		DoCmdSendCommandL(aSection);
		}
	else if (aCommand == KCmdVersion)
		{
		DoCmdVersion(aSection);
		}
	else if (aCommand == KCmdHeapBase)
		{
		DoCmdHeapBase();
		}
	else if (aCommand == KCmdSessionHandle)
		{
		DoCmdSessionHandle();
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}


void CT_DataFbsSession::DoCmdConstructor()
	{
	INFO_PRINTF1(_L("Call RFbsSession's constructor"));

	RFbsSession	tempSession;
	}


/** RFbsSession::Connect() Call */
void CT_DataFbsSession::DoCmdConnect(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("RFbsSession::Connect() call"));

	TBool	session=EFalse;
	GetBoolFromConfig(aSection, KSession(), session);

	TBool	invalidSession=EFalse;
	GetBoolFromConfig(aSection, KInvalidSession(), invalidSession);

	TInt	err=KErrNone;
	if ( session )
		{
		err=RFbsSession::Connect(FileServer());
		}
	else if ( invalidSession )
		{
		err=RFbsSession::Connect(iFsUnconnected);
		}
	else
		{
		err=RFbsSession::Connect();
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(_L("Connect failed with error %d"), err);
		SetError(err);
		}
	}

/** RFbsSession::Disconnect() call */
void CT_DataFbsSession::DoCmdDisconnect()
	{
	INFO_PRINTF1(_L("RFbsSession::Disconnect() call"));
	RFbsSession::Disconnect();
	}

/** RFbsSession::GetSession() check */
void CT_DataFbsSession::DoCmdGetSession(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("RFbsSession::GetSession() check"));

	// get not_null_session parameter value

	TBool	nullSessionExpected = EFalse;
	GetBoolFromConfig(aSection, KNullSession(), nullSessionExpected);

	// check result of GetSession() method
	iSession = RFbsSession::GetSession();

	if ( nullSessionExpected )
		{
		if ( iSession != NULL )
			{
			// Session is expected to be null but it is not null
			ERR_PRINTF1(_L("Session is expected to be null but it is not null!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		if ( iSession == NULL )
			{
			ERR_PRINTF1(_L("Session is expected to be not null but it is null!"));
			SetBlockResult(EFail);
			}
		}
	}


/** RFbsSession::CallBack() call */
void CT_DataFbsSession::DoCmdCallBack(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("RFbsSession::CallBack() call"));
	iCallBackCalled=EFalse;
	iSession->CallBack();

	// get expected callback parameter value
	TBool	expected;
	if( GetBoolFromConfig(aSection, KExpectedCallBack(), expected) )
		{
		if ( expected != iCallBackCalled )
			{
			ERR_PRINTF3(_L("Expected callback %d does not equal actual callback %d"), expected, iCallBackCalled);
			SetBlockResult(EFail);
			}
		}
	}


/** Set call back function uding RFbsSession::SetCallBack() */
void CT_DataFbsSession::DoCmdSetCallBack()
	{
	INFO_PRINTF1(_L("Set call back function uding SetCallBack()"));
	TCallBack	callBack(CallBackFunction, this);
	iCallBackCalled=EFalse;
	iSession->SetCallBack(callBack);
	}


/** RFbsSession::ResetCallBack() call */
void CT_DataFbsSession::DoCmdResetCallBack()
	{
	INFO_PRINTF1(_L("RFbsSession::ResetCallBack() call"));
	iCallBackCalled=EFalse;
	iSession->ResetCallBack();
	}


/** RFbsSession::ResourceCount() call, memorize resource count */
void CT_DataFbsSession::DoCmdResourceCount(const TDesC& aSection)
	{
	TInt	resourceCount=iSession->ResourceCount();
	INFO_PRINTF2(_L("RFbsSession::ResourceCount() %d"), resourceCount);


	// get expected difference from parameters
	TInt	expectedDifference;
	if ( GetIntFromConfig(aSection, KExpectedDiff(), expectedDifference) )
		{
		TInt	actualDifference=resourceCount-iResourceCount;
		if ( actualDifference != expectedDifference )
			{
			ERR_PRINTF3(_L("Resource count difference is not as expected! expected: %d, actual: %d"), expectedDifference, actualDifference);
			SetBlockResult(EFail);
			}
		}

	iResourceCount = resourceCount;
	}


/** RFbsSession::SendCommand() call */
void CT_DataFbsSession::DoCmdSendCommandL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("RFbsSession::SendCommand() call"));

	TInt		fbsParameter0=0;
	CFbsBitmap*	fbsBitmap=NULL;
	if ( CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KBitmap(), fbsBitmap) )
		{
		fbsParameter0=fbsBitmap->Handle();
		}
	else
		{
		GetIntFromConfig(aSection, KFbsParameter0(), fbsParameter0);
		}

	TInt	fbsParameter1=0;
	GetIntFromConfig(aSection, KFbsParameter1(), fbsParameter1);

	TInt	fbsParameter2=0;
	GetIntFromConfig(aSection, KFbsParameter2(), fbsParameter2);

	TInt	fbsParameter3=0;
	GetIntFromConfig(aSection, KFbsParameter3(), fbsParameter3);

	TInt	fbsMessage;
	if ( !GetFbsMessage(aSection, fbsMessage) )
		{
		ERR_PRINTF1(_L("No fbs message"));
		SetBlockResult(EFail);
		}
	else
		{
		iSession->SendCommand(fbsMessage, fbsParameter0, fbsParameter1, fbsParameter2, fbsParameter3);
		}
	}


/** RFbsSession::Version() call */
void CT_DataFbsSession::DoCmdVersion(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("RFbsSession::Version() call"));

	TVersion		version		= iSession->Version();
	TVersionName	versionName	= version.Name();
	INFO_PRINTF2(_L("Version name  : %s"), &versionName);
	INFO_PRINTF2(_L("Version build : %d"), (TInt)version.iBuild);
	INFO_PRINTF2(_L("Version major : %d"), (TInt)version.iMajor);
	INFO_PRINTF2(_L("Version minor : %d"), (TInt)version.iMinor);

	TPtrC	name;
	if ( GetStringFromConfig(aSection, KVersionName(), name) )
		{
		if ( name != version.Name() )
			{
			ERR_PRINTF1(_L("Name does not match expected name"));
			SetBlockResult(EFail);
			}
		}

	TInt	intTemp;
	if ( GetIntFromConfig(aSection, KVersionBuild(), intTemp) )
		{
		if ( intTemp != version.iBuild )
			{
			ERR_PRINTF1(_L("Build does not match expected build"));
			SetBlockResult(EFail);
			}
		}

	if ( GetIntFromConfig(aSection, KVersionMajor(), intTemp) )
		{
		if ( intTemp != version.iMajor )
			{
			ERR_PRINTF1(_L("Major does not match expected major"));
			SetBlockResult(EFail);
			}
		}

	if ( GetIntFromConfig(aSection, KVersionMinor(), intTemp) )
		{
		if ( intTemp != version.iMinor )
			{
			ERR_PRINTF1(_L("Minor does not match expected minor"));
			SetBlockResult(EFail);
			}
		}

	if (version.Name() == _L("") && version.iBuild == 0 && version.iMajor == 0 && version.iMinor == 0)
		{
		ERR_PRINTF1(_L("Some version fields are not set!"));
		SetBlockResult(EFail);
		}
	}


/** RFbsSession::HeapBase() call */
void CT_DataFbsSession::DoCmdHeapBase()
	{
	TUint8*	heapBase = iSession->HeapBase();
	INFO_PRINTF2(_L("RFbsSession::HeapBase() %d"), heapBase);

	if (heapBase == NULL)
		{
		ERR_PRINTF1(_L("Heap base is null!"));
		SetBlockResult(EFail);
		}
	}


/** RFbsSession::SessionHandle() call */
void CT_DataFbsSession::DoCmdSessionHandle()
	{
	TInt	sessionHandle = iSession->SessionHandle();
	INFO_PRINTF2(_L("RFbsSession::SessionHandle() %d"), sessionHandle);

	if (sessionHandle == 0)
		{
		ERR_PRINTF1(_L("Session handle is 0!"));
		SetBlockResult(EFail);
		}
	}


/**
* Return a pointer to the object that the data wraps. Not implemented, returns NULL.
*
* @return pointer to the object that the data wraps. Not implemented, returns NULL.
*/
TAny* CT_DataFbsSession::GetObject()
	{
	return iSession;
	}


/** Function used as a call back, that sets GlobalCallBackCalled to true*/
TInt CT_DataFbsSession::CallBackFunction(TAny* aThis)
	{
	return static_cast<CT_DataFbsSession*>(aThis)->CallBackFunction();
	}


/** Function used as a call back, that sets GlobalCallBackCalled to true*/
TInt CT_DataFbsSession::CallBackFunction()
	{
	return iCallBackCalled = ETrue;
	}

TBool CT_DataFbsSession::GetFbsMessage(const TDesC& aSection, TInt& aMessage)
	{
	TPtrC	fbsMessage;
	TBool	ret=GetStringFromConfig(aSection, KFbsMessage(), fbsMessage);
	if ( !ret )
		{
		ERR_PRINTF2(_L("No %S"), &KFbsMessage());
		SetBlockResult(EFail);
		}
	else
		{
		if ( fbsMessage==KFbsMessInit )
			{
			aMessage = EFbsMessInit;
			}
		else if ( fbsMessage==KFbsMessShutdown )
			{
			aMessage = EFbsMessShutdown;
			}
		else if ( fbsMessage==KFbsMessClose )
			{
			aMessage = EFbsMessClose;
			}
		else if ( fbsMessage==KFbsMessResourceCount )
			{
			aMessage = EFbsMessResourceCount;
			}
		else if ( fbsMessage==KFbsMessNumTypefaces )
			{
			aMessage = EFbsMessNumTypefaces;
			}
		else if ( fbsMessage==KFbsMessTypefaceSupport )
			{
			aMessage = EFbsMessTypefaceSupport;
			}
		else if ( fbsMessage==KFbsMessFontHeightInTwips )
			{
			aMessage = EFbsMessFontHeightInTwips;
			}
		else if ( fbsMessage==KFbsMessFontHeightInPixels )
			{
			aMessage = EFbsMessFontHeightInPixels;
			}
		else if ( fbsMessage==KFbsMessAddFontStoreFile )
			{
			aMessage = EFbsMessAddFontStoreFile;
			}
		else if ( fbsMessage==KFbsMessInstallFontStoreFile )
			{
			aMessage = EFbsMessInstallFontStoreFile;
			}
		else if ( fbsMessage==KFbsMessRemoveFontStoreFile )
			{
			aMessage = EFbsMessRemoveFontStoreFile;
			}
		else if ( fbsMessage==KFbsMessSetPixelHeight )
			{
			aMessage = EFbsMessSetPixelHeight;
			}
		else if ( fbsMessage==KFbsMessGetFontById )
			{
			aMessage = EFbsMessGetFontById;
			}
		else if ( fbsMessage==KFbsMessFontDuplicate )
			{
			aMessage = EFbsMessFontDuplicate;
			}
		else if ( fbsMessage==KFbsMessBitmapCreate )
			{
			aMessage = EFbsMessBitmapCreate;
			}
		else if ( fbsMessage==KFbsMessBitmapResize )
			{
			aMessage = EFbsMessBitmapResize;
			}
		else if ( fbsMessage==KFbsMessBitmapDuplicate )
			{
			aMessage = EFbsMessBitmapDuplicate;
			}
		else if ( fbsMessage==KFbsMessBitmapLoad )
			{
			aMessage = EFbsMessBitmapLoad;
			}
		else if ( fbsMessage==KFbsMessDefaultAllocFail )
			{
			aMessage = EFbsMessDefaultAllocFail;
			}
		else if ( fbsMessage==KFbsMessDefaultMark )
			{
			aMessage = EFbsMessDefaultMark;
			}
		else if ( fbsMessage==KFbsMessDefaultMarkEnd )
			{
			aMessage = EFbsMessDefaultMarkEnd;
			}
		else if ( fbsMessage==KFbsMessUserAllocFail )
			{
			aMessage = EFbsMessUserAllocFail;
			}
		else if ( fbsMessage==KFbsMessUserMark )
			{
			aMessage = EFbsMessUserMark;
			}
		else if ( fbsMessage==KFbsMessUserMarkEnd )
			{
			aMessage = EFbsMessUserMarkEnd;
			}
		else if ( fbsMessage==KFbsMessHeapCheck )
			{
			aMessage = EFbsMessHeapCheck;
			}
		else if ( fbsMessage==KFbsMessRasterize )
			{
			aMessage = EFbsMessRasterize;
			}
		else if ( fbsMessage==KFbsMessFaceAttrib )
			{
			aMessage = EFbsMessFaceAttrib;
			}
		else if ( fbsMessage==KFbsMessHasCharacter )
			{
			aMessage = EFbsMessHasCharacter;
			}
		else if ( fbsMessage==KFbsMessSetDefaultGlyphBitmapType )
			{
			aMessage = EFbsMessSetDefaultGlyphBitmapType;
			}
		else if ( fbsMessage==KFbsMessGetDefaultGlyphBitmapType )
			{
			aMessage = EFbsMessGetDefaultGlyphBitmapType;
			}
		else if ( fbsMessage==KFbsMessFontNameAlias )
			{
			aMessage = EFbsMessFontNameAlias;
			}
		else if ( fbsMessage==KFbsMessBitmapCompress )
			{
			aMessage = EFbsMessBitmapCompress;
			}
		else if ( fbsMessage==KFbsMessGetHeapSizes )
			{
			aMessage = EFbsMessGetHeapSizes;
			}
		else if ( fbsMessage==KFbsMessGetNearestFontToDesignHeightInTwips )
			{
			aMessage = EFbsMessGetNearestFontToDesignHeightInTwips;
			}
		else if ( fbsMessage==KFbsMessGetNearestFontToMaxHeightInTwips )
			{
			aMessage = EFbsMessGetNearestFontToMaxHeightInTwips;
			}
		else if ( fbsMessage==KFbsMessGetNearestFontToDesignHeightInPixels )
			{
			aMessage = EFbsMessGetNearestFontToDesignHeightInPixels;
			}
		else if ( fbsMessage==KFbsMessGetNearestFontToMaxHeightInPixels )
			{
			aMessage = EFbsMessGetNearestFontToMaxHeightInPixels;
			}
		else if ( fbsMessage==KFbsMessShapeText )
			{
			aMessage = EFbsMessShapeText;
			}
		else if ( fbsMessage==KFbsMessShapeDelete )
			{
			aMessage = EFbsMessShapeDelete;
			}
		else if ( fbsMessage==KFbsMessDefaultLanguageForMetrics )
			{
			aMessage = EFbsMessDefaultLanguageForMetrics;
			}
		else if ( fbsMessage==KFbsMessSetTwipsHeight )
			{
			aMessage = EFbsMessSetTwipsHeight;
			}
		else if ( fbsMessage==KFbsMessGetTwipsHeight )
			{
			aMessage = EFbsMessGetTwipsHeight;
			}
		else if ( fbsMessage==KFbsCompress )
			{
			aMessage = EFbsCompress;
			}
		else if ( fbsMessage==KFbsMessBitmapBgCompress )
			{
			aMessage = EFbsMessBitmapBgCompress;
			}

		else if ( fbsMessage==KFbsSetSystemDefaultTypefaceName )
			{
			aMessage = EFbsSetSystemDefaultTypefaceName;
			}
		else if ( fbsMessage==KFbsGetAllBitmapHandles )
			{
			aMessage = EFbsGetAllBitmapHandles;
			}
		else if ( fbsMessage==KFbsMessSetHeapFail )
			{
			aMessage = EFbsMessSetHeapFail;
			}
		else if ( fbsMessage==KFbsMessHeapCount )
			{
			aMessage = EFbsMessHeapCount;
			}
		else if ( fbsMessage==KFbsMessSetHeapReset )
			{
			aMessage = EFbsMessSetHeapReset;
			}
		else if ( fbsMessage==KFbsMessSetHeapCheck )
			{
			aMessage = EFbsMessSetHeapCheck;
			}
		else if ( fbsMessage==KFbsMessHeap )
			{
			aMessage = EFbsMessHeap;
			}
		else if ( fbsMessage==KFbsMessBitmapClean )
			{
			aMessage = EFbsMessBitmapClean;
			}
		else
			{
			ret=GetIntFromConfig(aSection, KFbsMessage(), aMessage);
			}
		}

	return ret;
	}
