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

#include "T_GraphicsUtil.h"
#include "T_DataDsaSession.h"

_LIT(KDataClassname,	"RDirectScreenAccess");
//command
_LIT( KCmdnewL, "new" );
_LIT( KCmdConstruct, "Construct" );
_LIT( KCmdClose, "Close" );
_LIT( KCmdRequest, "Request" );
_LIT( KCmdCompleted, "Completed" );
_LIT( KCmdCancel, "Cancel" );
_LIT( KCmdEatupMemory, "Util_EatupMemory" );
_LIT( KCmdFreeEatenMemory, "Util_FreeEatenMemory" );
// Fields
_LIT( KRWsSessionObj, "ws");
_LIT( KRWindowObj, "win");
_LIT( KFldRegion, "region1");
_LIT( KFldMemLeft, "memleft");

//Error
_LIT( KErrNewL, "Create RDirectScreenAccess() failed.");
_LIT( KErrRequestRegionMatch, "Execute RDirectScreenAccess::Request() failed, Error: region size not match.");
_LIT( KLogMissingParameter, "Missing parameter '%S'");
_LIT( KLogError, "Error=%d");
_LIT( KStraySignal, "Stray signal");

//Info
_LIT( KRdsaNew1, "RDSA::new() has been called");
_LIT( KRdsaNew2, "RDSA::new(session) has been called");
_LIT( KInfoConstruct, "Construct() has been called" );
_LIT( KInfoClose, "Close() has been called" );
_LIT( KInfoRequest, "Request() has been called" );
_LIT( KInfoCompleted, "Completed() has been called" );
_LIT( KInfoCancel, "Cancel() has been called" );
_LIT( KRunRequestReady, "RunRequestReady %d");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CT_DataDsaSession::CT_DataDsaSession() : 
	iDsaSession(NULL),
	iActiveRequestReady(NULL)
	{
	}

void CT_DataDsaSession::ConstructL()
	{
	iActiveRequestReady = CActiveCallback::NewL(*this);
	}

CT_DataDsaSession::~CT_DataDsaSession()
	{
	this->DestroyData();
	}

CT_DataDsaSession* CT_DataDsaSession::NewL()
	{
	CT_DataDsaSession* self = new (ELeave) CT_DataDsaSession();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CT_DataDsaSession::SetObjectL(TAny* aAny)
	{
	this->DestroyData();
	iDsaSession = static_cast<RDirectScreenAccess*>(aAny);
	}

void CT_DataDsaSession::DisownObjectL()
	{
	iDsaSession = NULL;
	}

void CT_DataDsaSession::DestroyData()
	{
	delete iDsaSession;
	iDsaSession = NULL;
	delete iActiveRequestReady;
	iActiveRequestReady = NULL;
	}

MWsClientClass* CT_DataDsaSession::GetClientClass() const
	{
	return iDsaSession;
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
TBool CT_DataDsaSession::DoCommandL(const TTEFFunction& aCommand, const
			TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool result( ETrue );
	INFO_PRINTF1(aCommand);
	
	if ( !aCommand.Compare( KCmdnewL ) || !aCommand.Compare( KDataClassname ) )
        {
    	DoCmdnewL(aSection);
        }
	else if ( !aCommand.Compare( KCmdConstruct ) )
        {
        DoCmdConstruct();
        }
	else if ( !aCommand.Compare( KCmdClose ) )
        {
        DoCmdClose();
        }
	else if ( !aCommand.Compare( KCmdRequest) )
        {
        DoCmdRequestL(aSection , aAsyncErrorIndex);
        }
	else if ( !aCommand.Compare( KCmdCompleted ) )
        {
        DoCmdCompleted();
        }
	else if ( !aCommand.Compare( KCmdCancel ) )
        {
        DoCmdCancel();
        }    
    else if ( !aCommand.Compare(KCmdEatupMemory))
    	{
    	DoCmdEatupMemory(aSection);
    	}
    else if (!aCommand.Compare(KCmdFreeEatenMemory))
    	{
    	DoCmdFreeEatenMemory();
    	}
    else
        {
        result = EFalse;
        }
	
    return result;
	}

/**
 * Helper method to get RWsSession
 */
RWsSession* CT_DataDsaSession::GetRWSessionL(const TDesC& aSection)
	{
	RWsSession* iClient = NULL; //Window Session Client
	TPtrC objectName;
	
	if ( GetStringFromConfig(aSection, KRWsSessionObj(), objectName) )
		{
		iClient = static_cast<RWsSession*>(GetDataObjectL(objectName));
		}
	
	return iClient;
	}

/**
 * Test NewL();
 */
void CT_DataDsaSession::DoCmdnewL(const TDesC& aSection)
	{
	RWsSession* session = GetRWSessionL(aSection);
	
	if(!session)
		{
		//no parameter.
		iDsaSession = new (ELeave) RDirectScreenAccess();
		INFO_PRINTF1(KRdsaNew1);
		}
	else
		{
		//with parameter
		iDsaSession = new (ELeave) RDirectScreenAccess(*session);
		INFO_PRINTF1(KRdsaNew2);
		}

	if(!iDsaSession)
		{
		ERR_PRINTF1(KErrNewL);
		SetBlockResult(EFail);
		}
	
	}


/**
 * Test ConstructL();
 */
void CT_DataDsaSession::DoCmdConstruct()
	{
	INFO_PRINTF1(KInfoConstruct);
	TInt result = iDsaSession->Construct();
	
	if(result)
		{
		ERR_PRINTF2(KLogError, result);
		SetBlockResult(EFail);
		}
	
	}


/**
 * Test Close();
 */
void CT_DataDsaSession::DoCmdClose()
	{
	INFO_PRINTF1(KInfoClose);
	iDsaSession->Close();
	}


/**
 * Test RequestL();
 */
void CT_DataDsaSession::DoCmdRequestL(const TDesC& aSection, const TInt aAsyncErrorIndex)
	{
	INFO_PRINTF1(KInfoRequest);
	RWindowBase* iWindow = NULL;
	TPtrC objectName;
	
	if ( GetStringFromConfig(aSection, KRWindowObj(), objectName))
		{
		iWindow = static_cast<RWindowBase*> (GetDataObjectL(objectName));
		}
	
	if(!iWindow)
		{
		ERR_PRINTF2(KLogMissingParameter , &KRWindowObj);
		SetBlockResult(EFail);
		}
	
	RRegion* aRegion = NULL;
	
	TInt result = iDsaSession->Request(aRegion, iActiveRequestReady->iStatus, *iWindow);
	if(result)
		{
 		ERR_PRINTF2(KLogError, result);
		SetError(result);
		}
    else
        {    	
    	TRect temp;
    	if( GetRectFromConfig(aSection ,KFldRegion, temp ) && aRegion->Count())
    		{
    		//compare the region
    		TRect temp2 = (*aRegion)[0];
    		if(temp != temp2)
    			{
    			ERR_PRINTF1(KErrRequestRegionMatch);
    			SetBlockResult(EFail);
    			}
    		
    		}
    	
        iActiveRequestReady->Activate(aAsyncErrorIndex);
        IncOutstanding();		
    	}

	}

/**
 * Test Completed();
 */
void CT_DataDsaSession::DoCmdCompleted()
	{
	INFO_PRINTF1(KInfoCompleted);
	iDsaSession->Completed();
	}

/**
 * Test Cancel();
 */
void CT_DataDsaSession::DoCmdCancel()
	{
	INFO_PRINTF1(KInfoCancel);
	iDsaSession->Cancel();
	}

void CT_DataDsaSession::DoCmdEatupMemory(const TDesC& aSection)
	{
	CT_GraphicsUtil::EatupMemory(*this,aSection,KFldMemLeft);
	}
void CT_DataDsaSession::DoCmdFreeEatenMemory()
	{
	CT_GraphicsUtil::FreeEatenMemory(*this);
	}

void CT_DataDsaSession::RunL(CActive* aActive, TInt aIndex)
	{
	if ( iActiveRequestReady == aActive )
		{
		RunRequestReady(aIndex);
		}
	else
		{
		ERR_PRINTF1(KStraySignal);
		SetBlockResult(EFail);
		}
	
	}

void CT_DataDsaSession::RunRequestReady( const TInt aIndex)
	{
	DecOutstanding();
	TInt err = iActiveRequestReady->iStatus.Int();
	INFO_PRINTF2(KRunRequestReady, err);
	
	if ( KErrNone != err )
		{
		ERR_PRINTF2(KLogError, err);
		SetAsyncError(aIndex, err);
		}
	
	}

void CT_DataDsaSession::DoCancel(CActive* /*aActive*/, TInt /*aIndex*/)
	{
	//nothing.
	}


