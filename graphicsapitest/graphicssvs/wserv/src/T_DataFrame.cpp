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
#include "T_DataFrame.h"
#include <fbs.h>

/*@{*/
//Commands
_LIT(KCmdNew,							    "NewL");
_LIT(KCmdDestroy,						    "~");
_LIT(KCmdFrameInfo,						    "FrameInfo");
_LIT(KCmdSetFrameInfo,					    "SetFrameInfo");
_LIT(KCmdBitmap,						    "Bitmap");
_LIT(KCmdSetBitmap,						    "SetBitmap");
_LIT(KCmdMask,						        "Mask");
_LIT(KCmdSetMask,						    "SetMask");
// Fields
_LIT(KFldFrameInfo,						    "frameinfostate");
_LIT(KFldTFrame,						    "frame%d");
_LIT(KFldUid,                               "uid");
_LIT(KFldTWsGraphicId,					    "graphicid");
_LIT(KFldSecureId,                          "secureid");
_LIT(KFldSetMask,                           "setmask");
_LIT(KFldSetFrameInfo,                      "setframeinfo");
_LIT(KFldSetBitmap,                         "setbitmap");
_LIT(KFldMask,                              "expected_mask");
_LIT(KFldBitmap,                            "expected_bitmap");
_LIT(KFldGotMask,                           "got_mask");
_LIT(KFldGotBitmap,                         "got_bitmap");
///	Logging
_LIT(KLogError,								"Error=%d");
_LIT(KLogMissingParameter,					"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,					"Not expected value");

_LIT(KLogNew,                               "execute CT_DataFrame::NewL()");
_LIT(KLogFrameInfo,                         "execute CT_DataFrame::FrameInfo()");
_LIT(KLogSetFrameInfo,                      "execute CT_DataFrame::SetFrameInfo(const TFrameInfo &aFrameInfo)");
_LIT(KLogBitmap,                            "execute CT_DataFrame::Bitmap()");
_LIT(KLogSetBitmap,                         "execute CT_DataFrame::SetBitmap(CFbsBitmap *aBitmap)");
_LIT(KLogMask,                              "execute CT_DataFrame::Mask()");
_LIT(KLogSetMask,                           "execute CT_DataFrame::SetMask(CFbsBitmap *aMask)");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataFrame::CT_DataFrame()
:	CDataWrapperBase(),iFrame(NULL)
	{
	}

CT_DataFrame* CT_DataFrame::NewL()
	{
	CT_DataFrame* ret=new (ELeave) CT_DataFrame();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

void CT_DataFrame::ConstructL()
	{
	}

CT_DataFrame::~CT_DataFrame()
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
void CT_DataFrame::SetObjectL(TAny* aAny)
	{
	DoCmdDestroy();
	iFrame = static_cast<CWsGraphicBitmapAnimation::CFrame*> (aAny);
	}

/**
 * Called by TEF framework to disown the object.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataFrame::DisownObjectL()
	{
	iFrame = NULL;
	}

/**
 * Destroy the object which has been constructed.
 *
 * @param: None
 *
 * @return: None
 */
void CT_DataFrame::DoCmdDestroy()
	{
	delete iFrame;
	iFrame = NULL;
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
TBool CT_DataFrame::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool ret = ETrue;

	if ( aCommand==KCmdNew )
		{
		DoCmdNewL();
		}
	else if ( aCommand==KCmdDestroy )
		{
		DoCmdDestroy();
		}
	else if ( aCommand==KCmdFrameInfo )
		{
		DoCmdFrameInfo(aSection);
		}
    else if ( aCommand==KCmdSetFrameInfo )
		{
		DoCmdSetFrameInfo(aSection);
		}
    else if ( aCommand==KCmdBitmap )
		{
		DoCmdBitmapL(aSection);
		}
    else if ( aCommand==KCmdSetBitmap )
		{
		DoCmdSetBitmapL(aSection);
		}
    else if ( aCommand==KCmdMask )
		{
		DoCmdMaskL(aSection);
		}
    else if ( aCommand==KCmdSetMask )
		{
		DoCmdSetMaskL(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

/**
 * Process new command
 *
 * @return: None
 */
void CT_DataFrame::DoCmdNewL()
	{
    // Execute command and log parameters
    INFO_PRINTF1(KLogNew);
    TInt err=KErrNone;
    TRAP( err, iFrame=CWsGraphicBitmapAnimation::CFrame::NewL());

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	// No command return value and output parameter to display and check
	}

/**
 * Process FrameInfo command
 * 
 * @param aSection			the entry in the ini file requiring the command to be processed
 * 
 * @return: None
 */
void CT_DataFrame::DoCmdFrameInfo(const TDesC& aSection)
	{
	INFO_PRINTF1(KLogFrameInfo);
    TFrameInfo frameinfo = iFrame->FrameInfo();
    
    // Get test data for command input parameter(s)
	TFrameInfo expected;
	if ( CT_GraphicsUtil::GetFrameInfo(*this, aSection, expected) )
		{
		if( frameinfo.iFlags!= expected.iFlags)		
			{
			ERR_PRINTF1(KLogNotExpectedValue);
   			SetBlockResult(EFail);
			}
		}
	}

/**
 * Process SetFrameInfo command
 * 
 * @param aSection			the entry in the ini file requiring the command to be processed 
 * 
 * @return: None
 */
void CT_DataFrame::DoCmdSetFrameInfo(const TDesC& aSection)
	{
    TFrameInfo frameinfo;
    
    if ( CT_GraphicsUtil::GetFrameInfo(*this,aSection, frameinfo) )
        {
        INFO_PRINTF1(KLogSetFrameInfo);
        iFrame->SetFrameInfo(frameinfo);
        }
    else
        {
        ERR_PRINTF2(KLogMissingParameter,KFldSetFrameInfo);
        SetBlockResult(EFail);
        }        
	}

/**
 * Process Bitmap command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
*/
 void CT_DataFrame::DoCmdBitmapL(const TDesC& aSection)
	{
	INFO_PRINTF1(KLogBitmap);
    CFbsBitmap* bitmap=const_cast<CFbsBitmap*>(iFrame->Bitmap());
        
    TPtrC gotBitmap;
    if(GetStringFromConfig(aSection,KFldGotBitmap,gotBitmap))
    	{
    	SetDataObjectL(gotBitmap,bitmap);
    	}
    
    // Get test data for command input parameter(s)
	CFbsBitmap*	expected=NULL;
	if ( CT_GraphicsUtil::GetFbsBitmapL(*this,aSection, KFldBitmap, expected) )
        {
        if ( bitmap!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}  
        }
	}
    
/**
 * Process SetBitmap command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
*/
 void CT_DataFrame::DoCmdSetBitmapL(const TDesC& aSection)
	{
    CFbsBitmap*  bitmap=NULL;
    
    if ( CT_GraphicsUtil::GetFbsBitmapL(*this,aSection, KFldSetBitmap, bitmap) )
        {
        INFO_PRINTF1(KLogSetBitmap);
        iFrame->SetBitmap(bitmap);
        }
    else
        {
        ERR_PRINTF2(KLogMissingParameter,KFldSetBitmap);
        SetBlockResult(EFail);
        }
	}

/**
 * Process Mask command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
*/
 void CT_DataFrame::DoCmdMaskL(const TDesC& aSection)
	{
	INFO_PRINTF1(KLogMask);
    CFbsBitmap* mask=const_cast<CFbsBitmap*>(iFrame->Mask());
    
    TPtrC gotMask;
    if(GetStringFromConfig(aSection,KFldGotMask,gotMask))
    	{
    	SetDataObjectL(gotMask,mask);
    	}
    
    // Get test data for command input parameter(s)
	CFbsBitmap*	expected=NULL;
	if ( CT_GraphicsUtil::GetFbsBitmapL(*this,aSection, KFldMask, expected) )
        {
        if ( mask!=expected )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}  
        }
	}
    
/**
 * Process SetMask command
 *
 * @param aSection			the entry in the ini file requiring the command to be processed
 *
 * @return: None
*/
 void CT_DataFrame::DoCmdSetMaskL(const TDesC& aSection)
	{
    CFbsBitmap*  bitmap=NULL;
    
    if ( CT_GraphicsUtil::GetFbsBitmapL(*this,aSection, KFldSetMask, bitmap) )
        {
        INFO_PRINTF1(KLogSetMask);
        iFrame->SetMask(bitmap);
        }
    else
        {
        ERR_PRINTF2(KLogMissingParameter,KFldSetMask);
        SetBlockResult(EFail);
        }
	}
