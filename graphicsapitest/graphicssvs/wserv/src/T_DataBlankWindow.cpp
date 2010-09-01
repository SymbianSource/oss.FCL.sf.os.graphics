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

#include "T_DataBlankWindow.h"

/*@{*/
_LIT(KDataClassname,	"RBlankWindow");

//Commands
_LIT(KCmdnew,						"new");
_LIT(KCmdDestructor,				"~");
_LIT(KCmdConstruct,					"Construct");
_LIT(KCmdSetColor,					"SetColor");
_LIT(KCmdSetSize,					"SetSize");
_LIT(KCmdSetExtent,					"SetExtent");

// Fields
_LIT(KFldColor,						"color");
_LIT(KFldHandle,					"handle");
_LIT(KFldParent,					"parent");
_LIT(KFldPoint,						"point");
_LIT(KFldSize,						"size");
_LIT(KFldWs,						"ws");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataBlankWindow* CT_DataBlankWindow::NewL()
	{
	CT_DataBlankWindow*	ret=new (ELeave) CT_DataBlankWindow();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataBlankWindow::CT_DataBlankWindow()
:	CT_DataWindowBase()
,	iBlankWindow(NULL)
	{
	}

void CT_DataBlankWindow::ConstructL()
	{
	}

CT_DataBlankWindow::~CT_DataBlankWindow()
	{
	DestroyData();
	}

void CT_DataBlankWindow::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iBlankWindow = static_cast<RBlankWindow*> (aAny);
	}

void CT_DataBlankWindow::DisownObjectL()
	{
	iBlankWindow = NULL;
	}

void CT_DataBlankWindow::DestroyData()
	{
	delete iBlankWindow;
	iBlankWindow=NULL;
	}

RWindowBase* CT_DataBlankWindow::GetWindowBase() const
	{
	return iBlankWindow;
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
TBool CT_DataBlankWindow::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
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
	else if ( aCommand==KCmdSetColor )
		{
		DoCmdSetColor(aSection);
		}
	else if ( aCommand==KCmdSetSize )
		{
		DoCmdSetSize(aSection);
		}
	else if ( aCommand==KCmdSetExtent )
		{
		DoCmdSetExtent(aSection);
		}
	else
		{
		ret=CT_DataWindowBase::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataBlankWindow::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	TPtrC	wsName;
	RWsSession* ws=NULL;
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}

	TInt	err;
	if ( ws != NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RBlankWindow(RWsSession)"));
		TRAP( err,	iBlankWindow = new (ELeave) RBlankWindow(*ws) );
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RBlankWindow()"));
		TRAP( err,	iBlankWindow = new (ELeave) RBlankWindow() );
		}

	if ( err != KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}

	// No command output parameter to display and check
	}


void CT_DataBlankWindow::DoCmdConstructL(const TDesC& aSection)
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
		TInt	returnCode = iBlankWindow->Construct(*parent, datHandle);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode != KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}

		// No command output parameter to display and check
		}
	}


void CT_DataBlankWindow::DoCmdSetColor(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TRgb	datColor;
	if ( GetRgbFromConfig(aSection, KFldColor, datColor) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetColor(TRgb)"));
		iBlankWindow->SetColor(datColor);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute SetColor()"));
		iBlankWindow->SetColor();
		}
	}


void CT_DataBlankWindow::DoCmdSetSize(const TDesC& aSection)
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
		iBlankWindow->SetSize(datSize);
		}
	}


void CT_DataBlankWindow::DoCmdSetExtent(const TDesC& aSection)
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
		iBlankWindow->SetExtent(datPoint, datSize);
		}
	}
