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

#include "T_DataWsBitmap.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname, "CWsBitmap");

//Commands
_LIT(KCmdnew,						"new");
_LIT(KCmdDestructor,				"~CWsBitmap");
_LIT(KCmdDestructorGeneral,			"~");
_LIT(KCmdCreate,					"Create");
_LIT(KCmdDuplicate,					"Duplicate");
_LIT(KCmdLoad,						"Load");
_LIT(KCmdInternalizeL,				"InternalizeL");
_LIT(KCmdReset,						"Reset");

// Fields
_LIT(KFldDispMode,					"dispmode");
_LIT(KFldFileName,					"filename");
_LIT(KFldHandle,					"handle");
_LIT(KFldHandle_Name,				"handle_name");
_LIT(KFldId,						"id");
_LIT(KFldShareIfLoaded,				"shareifloaded");
_LIT(KFldSizeInPixels,				"sizeinpixels");
_LIT(KFldUseClosedStream,			"use_closed_stream");
_LIT(KFldWs,						"ws");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsBitmap* CT_DataWsBitmap::NewL()
	{
	CT_DataWsBitmap*	ret=new (ELeave) CT_DataWsBitmap();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsBitmap::CT_DataWsBitmap()
:	CT_DataFbsBitmap()
,	iWsBitmap(NULL)
	{
	}

void CT_DataWsBitmap::ConstructL()
	{
	CT_DataFbsBitmap::ConstructL();
	}

CT_DataWsBitmap::~CT_DataWsBitmap()
	{
	DestroyData();
	}

void CT_DataWsBitmap::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsBitmap = static_cast<CWsBitmap*> (aAny);
	}

void CT_DataWsBitmap::DestroyData()
	{
	delete iWsBitmap;
	iWsBitmap=NULL;
	}

void CT_DataWsBitmap::DisownObjectL()
	{
	iWsBitmap = NULL;
	}

MWsClientClass* CT_DataWsBitmap::GetClientClass() const
	{
	return iWsBitmap;
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
TBool CT_DataWsBitmap::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor || aCommand==KCmdDestructorGeneral )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnewL(aSection);
		}
	else if ( aCommand==KCmdCreate )
		{
		DoCmdCreate(aSection);
		}
	else if ( aCommand==KCmdDuplicate )
		{
		DoCmdDuplicateL(aSection);
		}
	else if ( aCommand==KCmdLoad )
		{
		DoCmdLoad(aSection);
		}
	else if ( aCommand==KCmdInternalizeL )
		{
		DoCmdInternalizeL(aSection);
		}
	else if ( aCommand==KCmdReset )
		{
		DoCmdReset();
		}
	else if ( !CT_DataWsClientClass::DoCommandL(*this, aCommand, aSection, aAsyncErrorIndex) )
		{
		CT_DataFbsBitmap::SetObjectL(iWsBitmap);
		ret = CT_DataFbsBitmap::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		CT_DataFbsBitmap::DisownObjectL();
		}

	return ret;
	}


void CT_DataWsBitmap::DoCmdnewL(const TDesC& aSection)
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
		INFO_PRINTF1(_L("execute new CWsBitmap(RWsSession)"));
		TRAP( err, iWsBitmap = new (ELeave) CWsBitmap(*ws));
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new CWsBitmap()"));
		TRAP( err, iWsBitmap = new (ELeave) CWsBitmap());
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}


void CT_DataWsBitmap::DoCmdDestructor()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute ~CWsBitmap()"));
	delete iWsBitmap;
	iWsBitmap=NULL;
	}


void CT_DataWsBitmap::DoCmdCreate(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TSize	datSizeInPixels;
	if ( !GetSizeFromConfig(aSection, KFldSizeInPixels, datSizeInPixels) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldSizeInPixels);
		SetBlockResult(EFail);
		}

	TDisplayMode	datDispMode;
	if ( !CT_GraphicsUtil::ReadDisplayMode(*this, aSection, KFldDispMode, datDispMode) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDispMode);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Create(TSize, TDisplayMode)"));
		TInt	returnCode = iWsBitmap->Create	(datSizeInPixels, datDispMode);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}
		}
	}


void CT_DataWsBitmap::DoCmdDuplicateL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls CFbsBitmap::Duplicate()"));
	TInt	handle=GetHandle();
	TPtrC	objectName;
	if ( GetStringFromConfig(aSection, KFldHandle_Name(), objectName) )
		{
		CDataWrapperBase*	wrapper=static_cast<CDataWrapperBase*>(GetDataWrapperL(objectName));
		handle=wrapper->GetHandle();
		}
	else
		{
		GetIntFromConfig(aSection, KFldHandle(), handle);
		}

	// call Duplicate()
	TInt	err = iWsBitmap->Duplicate(handle);
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Duplicate error: %d"), err);
		SetError(err);
		}
	}


void CT_DataWsBitmap::DoCmdLoad(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TPtrC	datFileName;
	if ( !GetStringFromConfig(aSection, KFldFileName, datFileName) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldFileName);
		SetBlockResult(EFail);
		}

	TInt	datId;
	if ( !GetIntFromConfig(aSection, KFldId, datId) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldId);
		SetBlockResult(EFail);
		}

	TBool	datShareIfLoaded;
	if ( !GetBoolFromConfig(aSection, KFldShareIfLoaded, datShareIfLoaded) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldShareIfLoaded);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Load(TDesC, TInt32, TBool)"));
		TInt	returnCode = iWsBitmap->Load(datFileName, datId, datShareIfLoaded);

		// Check the command return code, if !=KErrNone then stop this command
		if ( returnCode!=KErrNone )
			{
			ERR_PRINTF2(KLogError, returnCode);
			SetError(returnCode);
			}
		}
	}


void CT_DataWsBitmap::DoCmdInternalizeL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool	datUseClosedStream=EFalse;
	GetBoolFromConfig(aSection, KFldUseClosedStream, datUseClosedStream);

	TPtrC	datFileName;
	if ( !GetStringFromConfig(aSection, KFldFileName, datFileName) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldUseClosedStream);
		SetBlockResult(EFail);
		}
	else
		{
		CDirectFileStore*	readstore = CDirectFileStore::OpenL(FileServer(), datFileName, EFileStream | EFileRead);
		CleanupStack::PushL(readstore);
		TInt				popCount=1;

		TStreamId			headerid = readstore->Root();
		RStoreReadStream	readstrm;
		if ( !datUseClosedStream )
			{
			readstrm.OpenL(*readstore, headerid);
			CleanupClosePushL(readstrm);
			popCount=2;
			}

		// Execute command and log parameters
		INFO_PRINTF1(_L("execute InternalizeL(RReadStream)"));
		TRAPD(err, iWsBitmap->InternalizeL(readstrm));
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}

		// Cleanup
		CleanupStack::PopAndDestroy(popCount, readstore);
		}
	}


void CT_DataWsBitmap::DoCmdReset()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Reset()"));
	iWsBitmap->Reset();
	}
