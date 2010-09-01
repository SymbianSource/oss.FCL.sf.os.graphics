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

#include "T_DataWindow.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname,	"RWindow");

//Commands
_LIT(KCmdnew,							"new");
_LIT(KCmdDestructor,					"~");
_LIT(KCmdConstruct,						"Construct");
_LIT(KCmdBeginRedraw,					"BeginRedraw");
_LIT(KCmdEndRedraw,						"EndRedraw");
_LIT(KCmdInvalidate,					"Invalidate");
_LIT(KCmdGetInvalidRegion,				"GetInvalidRegion");
_LIT(KCmdSetBackgroundColor,			"SetBackgroundColor");
_LIT(KCmdSetSize,						"SetSize");
_LIT(KCmdSetExtent,						"SetExtent");
_LIT(KCmdHandleTransparencyUpdate,		"HandleTransparencyUpdate");
_LIT(KCmdSetTransparencyFactor,			"SetTransparencyFactor");
_LIT(KCmdSetTransparencyBitmap,			"SetTransparencyBitmap");
_LIT(KCmdSetTransparencyWsBitmap,		"SetTransparencyWsBitmap");
_LIT(KCmdSetNonTransparent,				"SetNonTransparent");
_LIT(KCmdEnableRedrawStore,				"EnableRedrawStore");
_LIT(KCmdSetTransparencyAlphaChannel,	"SetTransparencyAlphaChannel");
_LIT(KCmdSetTransparentRegion,			"SetTransparentRegion");
_LIT(KCmdSetTransparencyPolicy,			"SetTransparencyPolicy");
_LIT(KCmdIsRedrawStoreEnabled,			"IsRedrawStoreEnabled");
_LIT(KCmdEnableOSB,						"EnableOSB");

// Fields
_LIT(KFldColor,							"color");
_LIT(KFldEnabled,						"enabled");
_LIT(KFldExpected,						"expected");
_LIT(KFldFlag,							"flag");
_LIT(KFldHandle,						"handle");
_LIT(KFldParent,						"parent");
_LIT(KFldPoint,							"point");
_LIT(KFldPolicy,						"policy");
_LIT(KFldRect,							"rect");
_LIT(KFldRegion,						"region");
_LIT(KFldSize,							"size");
_LIT(KFldTransparencyBitmap,			"transparencybitmap");
_LIT(KFldTransparencyFactor,			"transparencyfactor");
_LIT(KFldWs,							"ws");

///	Logging
_LIT(KLogError,							"Error=%d");
_LIT(KLogMissingParameter,				"Missing parameter '%S'");
_LIT(KLogMissingExpectedValue,			"Missing expected value '%S'");
_LIT(KLogNotExpectedValueInt,			"'%S' is not as expected=%d, actual=%d");

/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWindow* CT_DataWindow::NewL()
	{
	CT_DataWindow*	ret=new (ELeave) CT_DataWindow();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWindow::CT_DataWindow()
:	CT_DataDrawableWindow()
,	iWindow(NULL)
	{
	}

void CT_DataWindow::ConstructL()
	{
	}

CT_DataWindow::~CT_DataWindow()
	{
	DestroyData();
	}

void CT_DataWindow::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWindow = static_cast<RWindow*> (aAny);
	}

void CT_DataWindow::DisownObjectL()
	{
	iWindow = NULL;
	}

void CT_DataWindow::DestroyData()
	{
	delete iWindow;
	iWindow=NULL;
	}

RDrawableWindow* CT_DataWindow::GetDrawableWindow() const
	{
	return iWindow;
	}

/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @param aAsyncErrorIndex	index of command. used for async calls
*
* @return ETrue if the command is processed
*/
TBool CT_DataWindow::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor )
		{
		DestroyData();
		}
	else if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnewL(aSection);
		}
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstructL(aSection);
		}
	else if ( aCommand==KCmdBeginRedraw )
		{
		DoCmdBeginRedraw(aSection);
		}
	else if ( aCommand==KCmdEndRedraw )
		{
		DoCmdEndRedraw();
		}
	else if ( aCommand==KCmdInvalidate )
		{
		DoCmdInvalidate(aSection);
		}
	else if ( aCommand==KCmdGetInvalidRegion )
		{
		DoCmdGetInvalidRegionL(aSection);
		}
	else if ( aCommand==KCmdSetBackgroundColor )
		{
		DoCmdSetBackgroundColor(aSection);
		}
	else if ( aCommand==KCmdSetSize )
		{
		DoCmdSetSize(aSection);
		}
	else if ( aCommand==KCmdSetExtent )
		{
		DoCmdSetExtent(aSection);
		}
	else if ( aCommand==KCmdHandleTransparencyUpdate )
		{
		DoCmdHandleTransparencyUpdate();
		}
	else if ( aCommand==KCmdSetTransparencyFactor )
		{
		DoCmdSetTransparencyFactor(aSection);
		}
	else if ( aCommand==KCmdSetTransparencyBitmap )
		{
		DoCmdSetTransparencyBitmapL(aSection);
		}
	else if ( aCommand==KCmdSetTransparencyWsBitmap )
		{
		DoCmdSetTransparencyWsBitmapL(aSection);
		}
	else if ( aCommand==KCmdSetNonTransparent )
		{
		DoCmdSetNonTransparent();
		}
	else if ( aCommand==KCmdEnableRedrawStore )
		{
		DoCmdEnableRedrawStore(aSection);
		}
	else if ( aCommand==KCmdSetTransparencyAlphaChannel )
		{
		DoCmdSetTransparencyAlphaChannel();
		}
	else if ( aCommand==KCmdSetTransparentRegion )
		{
		DoCmdSetTransparentRegionL(aSection);
		}
	else if ( aCommand==KCmdSetTransparencyPolicy )
		{
		DoCmdSetTransparencyPolicy(aSection);
		}
	else if ( aCommand==KCmdIsRedrawStoreEnabled )
		{
		DoCmdIsRedrawStoreEnabled(aSection);
		}
	else if ( aCommand==KCmdEnableOSB )
		{
		DoCmdEnableOSB(aSection);
		}
	else
		{
		ret=CT_DataDrawableWindow::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataWindow::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	TPtrC	wsName;
	RWsSession* ws=NULL;
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}

	TInt	err=KErrNone;
	if ( ws!=NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RWindow(RWsSession)"));
		TRAP( err, iWindow = new (ELeave) RWindow(*ws));
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RWindow()"));
		TRAP( err, iWindow = new (ELeave) RWindow());
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataWindow::DoCmdConstructL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPtrC	parentName;
	RWindowTreeNode*	parent=NULL;
	if ( GetStringFromConfig(aSection, KFldParent, parentName) )
		{
		parent = static_cast<RWindowTreeNode*>(GetDataObjectL(parentName));
		}
	if ( parent==NULL )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldParent);
		SetBlockResult(EFail);
		}

	TInt	datHandle;
	if ( !GetIntFromConfig(aSection, KFldHandle, datHandle) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldHandle);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Construct(RWindowTreeNode, TUint32)"));
		TInt	returnCode = iWindow->Construct(*parent, datHandle);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdBeginRedraw(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	datRect;

	if ( GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute BeginRedraw(TRect)"));
		iWindow->BeginRedraw(datRect);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute BeginRedraw()"));
		iWindow->BeginRedraw();
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindow::DoCmdEndRedraw()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute EndRedraw()"));
	iWindow->EndRedraw();

	// No command return value and output parameter to display and check
	}


void CT_DataWindow::DoCmdInvalidate(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRect	datRect;

	if ( GetRectFromConfig(aSection, KFldRect, datRect) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Invalidate(TRect)"));
		iWindow->Invalidate(datRect);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Invalidate()"));
		iWindow->Invalidate();
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindow::DoCmdGetInvalidRegionL(const TDesC& aSection)
	{
	RRegion	region(1);
	CleanupClosePushL(region);

	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute GetInvalidRegion(RRegion)"));
	iWindow->GetInvalidRegion(region);

	// Diaplay command return value, check if it matches the expected value
	LogRegion(_L("GetInvalidRegion()"), region);

	RRegion	expectedRegion(1);
	CleanupClosePushL(expectedRegion);
	if ( GetRegionFromConfig(aSection, KFldExpected, expectedRegion) )
		{
		region.SubRegion(expectedRegion);
		if ( !region.IsEmpty() )
			{
			ERR_PRINTF1(_L("Region not as expected"));
			LogRegion(_L("Region mismatch"), region);
			SetBlockResult(EFail);
			}
		}

	CleanupStack::PopAndDestroy(2, &region);
	}


void CT_DataWindow::DoCmdSetBackgroundColor(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRgb	datColor;

	if ( GetRgbFromConfig(aSection, KFldColor, datColor) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetBackgroundColor(TRgb)"));
		iWindow->SetBackgroundColor(datColor);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetBackgroundColor()"));
		iWindow->SetBackgroundColor();
		}

	// No command return value and output parameter to display and check
	}


void CT_DataWindow::DoCmdSetSize(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TSize	datSize;
	if ( !GetSizeFromConfig(aSection, KFldSize, datSize) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldSize);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetSize(TSize)"));
		iWindow->SetSize(datSize);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdSetExtent(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPoint	datPoint;
	if ( !GetPointFromConfig(aSection, KFldPoint, datPoint) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldPoint);
		SetBlockResult(EFail);
		}

	TSize	datSize;
	if ( !GetSizeFromConfig(aSection, KFldSize, datSize) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSize);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetExtent(TPoint, TSize)"));
		iWindow->SetExtent(datPoint, datSize);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdHandleTransparencyUpdate()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute HandleTransparencyUpdate()"));
	iWindow->HandleTransparencyUpdate();

	// No command return value and output parameter to display and check
	}


void CT_DataWindow::DoCmdSetTransparencyFactor(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRgb	datTransparencyFactor;

	if ( !GetRgbFromConfig(aSection, KFldTransparencyFactor, datTransparencyFactor) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldTransparencyFactor);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetTransparencyFactor(TRgb)"));
		TInt	returnCode = iWindow->SetTransparencyFactor(datTransparencyFactor);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdSetTransparencyBitmapL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	CFbsBitmap*	transparencyBitmap = NULL;

	if ( !CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldTransparencyBitmap, transparencyBitmap) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldTransparencyBitmap);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetTransparencyBitmap(CFbsBitmap)"));
		TInt	returnCode = iWindow->SetTransparencyBitmap(*transparencyBitmap);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdSetTransparencyWsBitmapL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	CWsBitmap*	transparencyBitmap = NULL;

	if ( !CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldTransparencyBitmap, transparencyBitmap) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldTransparencyBitmap);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetTransparencyWsBitmap(CWsBitmap)"));
		TInt	returnCode = iWindow->SetTransparencyWsBitmap(*transparencyBitmap);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdSetNonTransparent()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute SetNonTransparent()"));
	iWindow->SetNonTransparent();

	// No command return value and output parameter to display and check
	}


void CT_DataWindow::DoCmdEnableRedrawStore(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datEnabled;

	if ( !GetBoolFromConfig(aSection, KFldEnabled, datEnabled) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldEnabled);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute EnableRedrawStore(TBool)"));
		iWindow->EnableRedrawStore(datEnabled);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdSetTransparencyAlphaChannel()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute SetTransparencyAlphaChannel()"));
	TInt	returnCode = iWindow->SetTransparencyAlphaChannel();

	// Check the command return code, if !=KErrNone then stop this command
	if ( returnCode!=KErrNone )
		{
		ERR_PRINTF2(KLogError, returnCode);
		SetError(returnCode);
		}

	// No command output parameter to display and check
	}


void CT_DataWindow::DoCmdSetTransparentRegionL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	RRegion	region(1);
	CleanupClosePushL(region);

	if ( !GetRegionFromConfig(aSection, KFldRegion, region) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldRegion);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetTransparentRegion(TRegion)"));
		TInt	returnCode = iWindow->SetTransparentRegion(region);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}

	CleanupStack::PopAndDestroy(&region);
	}


void CT_DataWindow::DoCmdSetTransparencyPolicy(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TWsTransparencyPolicy	datPolicy;

	if ( !CT_GraphicsUtil::ReadWsTransparencyPolicy(*this, aSection, KFldPolicy, datPolicy) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldPolicy);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetTransparencyPolicy(TWsTransparencyPolicy)"));
		TInt	returnCode = iWindow->SetTransparencyPolicy(datPolicy);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataWindow::DoCmdIsRedrawStoreEnabled(const TDesC& aSection)
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute IsRedrawStoreEnabled()"));
	TBool	actualResult = iWindow->IsRedrawStoreEnabled();

	// Diaplay command return value, check if it matches the expected value
	TBool	expectResult;
	if ( !GetBoolFromConfig(aSection, KFldExpected, expectResult) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected());
		SetBlockResult(EFail);
		}
	else if ( actualResult != expectResult )
		{
		ERR_PRINTF4(KLogNotExpectedValueInt, &KFldExpected, expectResult, actualResult);
		SetBlockResult(EFail);
		}

	// No command output parameter to display and check
	}


void CT_DataWindow::DoCmdEnableOSB(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool		datFlag;

	if ( !GetBoolFromConfig(aSection, KFldFlag, datFlag) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldFlag);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute EnableOSB(TBool)"));
		iWindow->EnableOSB(datFlag);

		// No command return value and output parameter to display and check
		}
	}
