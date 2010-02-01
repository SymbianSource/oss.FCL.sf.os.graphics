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
#include "T_DataWsGraphicBitmapAnimation.h"

/*@{*/
//Commands
_LIT(KCmdNew,							    "NewL");
_LIT(KCmdDestroy,						    "~");
_LIT(KCmdUtil_StartAnimTesterClientL,		"Util_StartAnimTesterClientL");
_LIT(KCmdUtil_CloseAnimTesterClientL,		"Util_CloseAnimTesterClientL");

// Fields
_LIT(KFldFrameCount,						"framescount");
_LIT(KFldTFrame,						    "frame%d");
_LIT(KFldUid,                               "uid");
_LIT(KFldReplace,					        "replace");
///	Logging
_LIT(KLogError,								"Error=%d");
_LIT(KLogMissingParameter,					"Missing parameter '%S'");

_LIT(KLogNew,                               "execute CWsGraphicBitmapAnimation::NewL(const TFrames &aFrames)");
_LIT(KLogNew2,                              "execute CWsGraphicBitmapAnimation::NewL(TUid aUid, const TFrames &aFrames)");
_LIT(KLogNew3,                              "execute CWsGraphicBitmapAnimation::NewL(const TWsGraphicId &aReplace, const TFrames &aFrames)");
_LIT(KLogUtil_StartAnimTesterClientL,		"execute CWsGraphicBitmapAnimation::Util_StartAnimTesterClientL(const TDesC& aSection)");
_LIT(KLogUtil_CloseAnimTesterClientL,		"execute CWsGraphicBitmapAnimation::Util_CloseAnimTesterClientL()");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphicBitmapAnimation::CT_DataWsGraphicBitmapAnimation()
:	CT_DataWsGraphic(),iWsGraphicBitmapAnimation(NULL)
	{
	}

CT_DataWsGraphicBitmapAnimation*	CT_DataWsGraphicBitmapAnimation::NewL()
	{
	CT_DataWsGraphicBitmapAnimation*    ret=new (ELeave) CT_DataWsGraphicBitmapAnimation();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

void CT_DataWsGraphicBitmapAnimation::ConstructL()
	{
	}

CT_DataWsGraphicBitmapAnimation::~CT_DataWsGraphicBitmapAnimation()
	{
	DoCmdDestroy();
	}

/**
 * Called by TEF framework to set object.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsGraphicBitmapAnimation::SetObjectL(TAny* aAny)
	{
	DoCmdDestroy();
	iWsGraphicBitmapAnimation = static_cast<CWsGraphicBitmapAnimation*> (aAny);
	}

/**
 * Called by TEF framework to disown the object.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsGraphicBitmapAnimation::DisownObjectL()
	{
	iWsGraphicBitmapAnimation = NULL;
	}

/**
 * Destroy the object which has been constructed.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsGraphicBitmapAnimation::DoCmdDestroy()
	{
		delete iWsGraphicBitmapAnimation;
		iWsGraphicBitmapAnimation = NULL;
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
TBool CT_DataWsGraphicBitmapAnimation::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret = ETrue;

	if ( aCommand==KCmdNew )
		{
		DoCmdNewL(aSection);
		}
	else if ( aCommand==KCmdDestroy )
		{
		DoCmdDestroy();
		}
	else if ( aCommand==KCmdUtil_StartAnimTesterClientL )
		{
		Util_StartAnimTesterClientL(aSection);
		}
	else if ( aCommand==KCmdUtil_CloseAnimTesterClientL )
		{
		Util_CloseAnimTesterClientL(aSection);
		}
	else
		{
		ret=CT_DataWsGraphic::DoCommandL(aCommand,aSection,aAsyncErrorIndex);
		}

	return ret;
	}

/**
 * Process new command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsGraphicBitmapAnimation::DoCmdNewL(const TDesC& aSection)
	{
	TBool dataOk=ETrue;
	RPointerArray<CWsGraphicBitmapAnimation::CFrame> frames;

	//CWsGraphicBitmapAnimation::TFrames frames;
    TInt nFrameCount=0;
    
    if(!GetIntFromConfig(aSection,KFldFrameCount,nFrameCount))
        {
        ERR_PRINTF2(KLogMissingParameter,KFldFrameCount);
        SetBlockResult(EFail);
        dataOk=EFalse;
        }
    else
        {
        RBuf info;
    	info.Create(256);
        TPtrC	framename;
        for(TInt i=0;i<nFrameCount;i++)
            {
            info.Format(KFldTFrame,i+1);
            
            if(!GetStringFromConfig(aSection,info, framename))
                {
                ERR_PRINTF2(KLogMissingParameter,info.Ptr());
                SetBlockResult(EFail);
                dataOk=EFalse;
                }
            else
                {
            	frames.Append(static_cast<CWsGraphicBitmapAnimation::CFrame*>(GetDataObjectL(framename)));
                }
            }
        info.Close();
        }
		
	TInt err=KErrNone;
	
	if ( dataOk )
		{
        TInt ndata;
        TWsGraphicId	datReplace(TWsGraphicId::EUninitialized);
        if(GetIntFromConfig(aSection,KFldUid,ndata))
            {
            // Execute command and log parameters
    		INFO_PRINTF1(KLogNew2);
     		TRAP( err, iWsGraphicBitmapAnimation=CWsGraphicBitmapAnimation::NewL(TUid::Uid(ndata),frames.Array()));
            }
        else if(GetWsGraphicIdFromConfigL(aSection, KFldReplace, datReplace))
            {
            // Execute command and log parameters
    		INFO_PRINTF1(KLogNew3);
    		TRAP( err, iWsGraphicBitmapAnimation=CWsGraphicBitmapAnimation::NewL(datReplace,frames.Array()));
            }
        else
            {
            // Execute command and log parameters
    		INFO_PRINTF1(KLogNew);
    		TRAP( err, iWsGraphicBitmapAnimation=CWsGraphicBitmapAnimation::NewL(frames.Array()));
            }
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	// No command return value and output parameter to display and check
	}

/**
 * Process Util_StartAnimTesterClientL command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
 */
void CT_DataWsGraphicBitmapAnimation::Util_StartAnimTesterClientL(const TDesC& aSection)
	{
	TInt dataOK;
	TRAPD( err, dataOK = CT_GraphicsUtil::StartAnimTesterClientL(*this, aSection) );
	if( dataOK )
		{
	    if ( KErrNone == err )
	        {
	        INFO_PRINTF1(KLogUtil_StartAnimTesterClientL);
	        }
	    else
	        {
	        ERR_PRINTF2(KLogError, err);
	        SetBlockResult(EFail);
	        }
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, KLogUtil_StartAnimTesterClientL);
		}
	}

/**
 * Process Util_CloseAnimTesterClientL command
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataWsGraphicBitmapAnimation::Util_CloseAnimTesterClientL(const TDesC& aSection)
	{
	TInt dataOK;
	TRAPD( err, dataOK = CT_GraphicsUtil::CloseAnimTesterClientL(*this, aSection) );
	if( dataOK )
		{
	    if ( KErrNone == err )
	        {
	        INFO_PRINTF1(KLogUtil_CloseAnimTesterClientL);
	        }
	    else
	        {
	        ERR_PRINTF2(KLogError, err);
	        SetBlockResult(EFail);
	        }	
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter, KLogUtil_CloseAnimTesterClientL);
		}
	}


TInt CT_DataWsGraphicBitmapAnimation::ShareGlobally()
	{
	return iWsGraphicBitmapAnimation->ShareGlobally();
	}

TInt CT_DataWsGraphicBitmapAnimation::UnShareGlobally()
	{
    return iWsGraphicBitmapAnimation->UnShareGlobally();
  	}


 TInt CT_DataWsGraphicBitmapAnimation::Share(TSecureId aClientId)
	{
    return iWsGraphicBitmapAnimation->Share(aClientId);
	}
    

 TInt CT_DataWsGraphicBitmapAnimation::UnShare(TSecureId aClientId)
	{
    return iWsGraphicBitmapAnimation->UnShare(aClientId);    
	}
