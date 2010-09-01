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

#include "T_DataAnimDll.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname,							"RAnimDll");

///	Fields
_LIT(KFldWs,									"ws");
_LIT(KPlugInFileName,							"filename");

///	Commands
_LIT(KCmdnew,									"new");
_LIT(KCmdDestructorGeneral,						"~");
_LIT(KCmdDestructor,							"~RAnimDll");
_LIT(KCmdLoad,									"Load");
_LIT(KCmdDestroy,								"Destroy");
_LIT(KCmdClose,									"Close");

///	Logging
_LIT(KLogErrNum,			"Error=%d");
_LIT(KLogErrMissingPara,	"Missing parameter '%S'");
_LIT(KLogErrLoad,			"Load file error %d. File name: %S");
_LIT(KLogInfoCmdnewL1,		"execute new RAnimDll(RWsSession &)");
_LIT(KLogInfoCmdnewL2,		"execute new RAnimDll()");
_LIT(KLogInfoCmdLoad1,		"execute RAnimDll::Load(const TDesC &)");
_LIT(KLogInfoCmdClose1,		"execute RAnimDll::Close()");
_LIT(KLogInfoCmdDestroy1,	"execute RAnimDll::Destroy()");
_LIT(KLogInfoCmdDestructor1,"execute ~RAnimDll()");
/*@}*/

CT_DataAnimDll* CT_DataAnimDll::NewL()
	{
	CT_DataAnimDll*	ret = new (ELeave) CT_DataAnimDll();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataAnimDll::CT_DataAnimDll()
:	iAnimDll(NULL)
	{
	}

void CT_DataAnimDll::ConstructL()
	{
	}

CT_DataAnimDll::~CT_DataAnimDll()
	{
	DestroyData();
	}

TAny* CT_DataAnimDll::GetObject()
	{
	return iAnimDll;
	}

void CT_DataAnimDll::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iAnimDll = static_cast<RAnimDll*> (aAny);
	}

void CT_DataAnimDll::DisownObjectL()
	{
	iAnimDll = NULL;
	}

void CT_DataAnimDll::DestroyData()
	{
	DoCmdDestructor();
	}

MWsClientClass* CT_DataAnimDll::GetClientClass() const
	{
	return iAnimDll;
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
TBool CT_DataAnimDll::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructorGeneral || aCommand==KCmdDestructor )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnewL(aSection);
		}
	else if ( aCommand==KCmdLoad )
		{
		DoCmdLoad(aSection);
		}
	else if ( aCommand==KCmdDestroy )
		{
		DoCmdDestroy();
		}
	else if ( aCommand==KCmdClose )
		{
		DoCmdClose();
		}
	else
		{
		ret=CT_DataWsClientClass::DoCommandL(*this, aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataAnimDll::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	TPtrC		wsName;
	RWsSession*	ws = NULL;
	if ( GetStringFromConfig(aSection, KFldWs, wsName) )
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}

	TInt err = KErrNone;
	if ( ws!= NULL )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogInfoCmdnewL1);
		TRAP( err, iAnimDll = new (ELeave) RAnimDll(*ws) );
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogInfoCmdnewL2);
		TRAP(err, iAnimDll = new (ELeave) RAnimDll());
		}
	
	// Check the command return code
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

void CT_DataAnimDll::DoCmdLoad(const TDesC& aSection)
	{
	TInt err = KErrNone;
	
	// Get test data for command input parameter(s)
	TPtrC fileName;
	if(!GetStringFromConfig(aSection, KPlugInFileName(), fileName))
		{
		ERR_PRINTF2(KLogErrMissingPara, &KPlugInFileName());
		SetBlockResult(EFail);
		}
	else
		{	
		// Execute command and log parameters
		if (iAnimDll != NULL)
			{
			INFO_PRINTF1(KLogInfoCmdLoad1);
			err = iAnimDll->Load(fileName);
			}
		
		// Check the command return code
		if(err != KErrNone)
			{
			ERR_PRINTF3(KLogErrLoad, err, &fileName);
			SetError(err);
			}
		}
	}

void CT_DataAnimDll::DoCmdClose()
	{
	// Execute command and log parameters
	if (iAnimDll != NULL)
		{
		INFO_PRINTF1(KLogInfoCmdClose1);
		iAnimDll->Close();
		}
	}

void CT_DataAnimDll::DoCmdDestroy()
	{
	// Execute command and log parameters
	if (iAnimDll != NULL)
		{
		INFO_PRINTF1(KLogInfoCmdDestroy1);
		iAnimDll->Destroy();
		iAnimDll = NULL;
		}
	}

void CT_DataAnimDll::DoCmdDestructor()
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogInfoCmdDestructor1);
	delete iAnimDll;
	iAnimDll = NULL;
	}

