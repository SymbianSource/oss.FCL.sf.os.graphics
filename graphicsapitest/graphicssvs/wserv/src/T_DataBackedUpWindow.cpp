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

#include "T_DataBackedUpWindow.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname,	"RBackedUpWindow");

//Commands
_LIT(KCmdnew,						"new");
_LIT(KCmdDestructor,				"~");
_LIT(KCmdConstruct,					"Construct");
_LIT(KCmdBitmapHandle,				"BitmapHandle");
_LIT(KCmdUpdateScreen,				"UpdateScreen");
_LIT(KCmdUpdateBackupBitmap,		"UpdateBackupBitmap");
_LIT(KCmdMaintainBackup,			"MaintainBackup");

// Fields
_LIT(KFldDisplayMode,				"displaymode");
_LIT(KFldHandle,					"handle");
_LIT(KFldParent,					"parent");
_LIT(KFldRegion,					"region");
_LIT(KFldWs,						"ws");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataBackedUpWindow* CT_DataBackedUpWindow::NewL()
	{
	CT_DataBackedUpWindow*	ret=new (ELeave) CT_DataBackedUpWindow();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataBackedUpWindow::CT_DataBackedUpWindow()
:	CT_DataDrawableWindow()
,	iBackedUpWindow(NULL)
	{
	}

void CT_DataBackedUpWindow::ConstructL()
	{
	}

CT_DataBackedUpWindow::~CT_DataBackedUpWindow()
	{
	DestroyData();
	}

void CT_DataBackedUpWindow::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iBackedUpWindow = static_cast<RBackedUpWindow*> (aAny);
	}

void CT_DataBackedUpWindow::DestroyData()
	{
	delete iBackedUpWindow;
	iBackedUpWindow=NULL;
	}

void CT_DataBackedUpWindow::DisownObjectL()
	{
	iBackedUpWindow = NULL;
	}

RDrawableWindow* CT_DataBackedUpWindow::GetDrawableWindow() const
	{
	return iBackedUpWindow;
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
TBool CT_DataBackedUpWindow::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
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
	else if ( aCommand==KCmdBitmapHandle )
		{
		DoCmdBitmapHandle();
		}
	else if ( aCommand==KCmdUpdateScreen )
		{
		DoCmdUpdateScreenL(aSection);
		}
	else if ( aCommand==KCmdUpdateBackupBitmap )
		{
		DoCmdUpdateBackupBitmap();
		}
	else if ( aCommand==KCmdMaintainBackup )
		{
		DoCmdMaintainBackup();
		}
	else
		{
		ret=CT_DataDrawableWindow::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataBackedUpWindow::DoCmdnewL(const TDesC& aSection)
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
		INFO_PRINTF1(_L("execute new RBackedUpWindow(RWsSession)"));
		TRAP( err, iBackedUpWindow = new (ELeave) RBackedUpWindow(*ws));
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RBackedUpWindow()"));
		TRAP( err, iBackedUpWindow = new (ELeave) RBackedUpWindow());
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataBackedUpWindow::DoCmdConstructL(const TDesC& aSection)
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

	TDisplayMode	datDisplayMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldDisplayMode, datDisplayMode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDisplayMode);
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
		INFO_PRINTF1(_L("execute Construct(RWindowTreeNode, TDisplayMode, TUint32)"));
		TInt	returnCode = iBackedUpWindow->Construct(*parent, datDisplayMode, datHandle);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataBackedUpWindow::DoCmdBitmapHandle()
	{
	// Execute command and log parameters
	TInt	handle=iBackedUpWindow->BitmapHandle();
	INFO_PRINTF2(_L("BitmapHandle() %d"), handle);
	SetHandle(handle);
	}


void CT_DataBackedUpWindow::DoCmdUpdateScreenL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	RRegion	region(1);
	CleanupClosePushL(region);

	if ( GetRegionFromConfig(aSection, KFldRegion, region) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute UpdateScreen(TRegion)"));
		iBackedUpWindow->UpdateScreen(region);

		// No command return value and output parameter to display and check
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute UpdateScreen()"));
		iBackedUpWindow->UpdateScreen();

		// No command return value and output parameter to display and check
		}

	CleanupStack::PopAndDestroy(&region);
	}


void CT_DataBackedUpWindow::DoCmdUpdateBackupBitmap()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute UpdateBackupBitmap()"));
	iBackedUpWindow->UpdateBackupBitmap();

	// No command return value and output parameter to display and check
	}


void CT_DataBackedUpWindow::DoCmdMaintainBackup()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute MaintainBackup()"));
	iBackedUpWindow->MaintainBackup();

	// No command return value and output parameter to display and check
	}
