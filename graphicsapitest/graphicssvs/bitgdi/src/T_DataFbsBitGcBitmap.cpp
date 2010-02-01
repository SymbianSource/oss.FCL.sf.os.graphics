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


#include "T_DataFbsBitGcBitmap.h"


///	Commands
_LIT(KCmdNewL,										"new");
_LIT(KCmdAddress,									"Address");
_LIT(KCmdLockHeap,									"LockHeap");
_LIT(KCmdUnlockHeap,								"UnlockHeap");
_LIT(KCmdDestructorGeneral,							"~");
_LIT(KCmdDestructor,								"~CFbsBitGcBitmap");

//Log
_LIT(KLogErrNum,									"Error=%d");
_LIT(KLogInfoCmdnewL, 						    	"execute new CFbsBitGcBitmap()");
_LIT(KLogInfoCmdDestructor1, 						"execute ~CFbsBitGcBitmap()");

//Fields
_LIT(KFldExpectedWidth, 							"expectedWidth");
_LIT(KFldExpectedHeight, 							"expectedHeight");


CT_DataFbsBitGcBitmap* CT_DataFbsBitGcBitmap::NewL()
	{
	CT_DataFbsBitGcBitmap*	ret = new (ELeave) CT_DataFbsBitGcBitmap();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataFbsBitGcBitmap::CT_DataFbsBitGcBitmap()
:	iFbsBitGcBitmap(NULL)
	{
	
	}

void CT_DataFbsBitGcBitmap::ConstructL()
	{

	}

CT_DataFbsBitGcBitmap::~CT_DataFbsBitGcBitmap()
	{
	DestroyData();
	}

TAny* CT_DataFbsBitGcBitmap::GetObject()
	{
	return iFbsBitGcBitmap;
	}

void CT_DataFbsBitGcBitmap::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iFbsBitGcBitmap = static_cast<CFbsBitGcBitmap*> (aAny);
	}

void CT_DataFbsBitGcBitmap::DisownObjectL()
	{
	iFbsBitGcBitmap = NULL;
	}

CFbsBitmap* CT_DataFbsBitGcBitmap::GetFbsBitmap() const
	{
	return iFbsBitGcBitmap;
	}

/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @return ETrue if the command is processed
*/
TBool CT_DataFbsBitGcBitmap::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;
    if (aCommand==KCmdNewL)
    	{
    	DoCmdNewL();
    	}
	else if ( aCommand==KCmdDestructorGeneral || aCommand==KCmdDestructor )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdAddress)
		{
		DoCmdAddress(aSection);
		}
	else if(aCommand==KCmdLockHeap)
		{
		DoCmdLockHeap();
		}
	else if(aCommand==KCmdUnlockHeap)
		{
		DoCmdUnlockHeap();
		}
	else
		{
		ret=CT_DataFbsBitmap::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}
    
	return ret;
	}

void CT_DataFbsBitGcBitmap::DestroyData()
	{
	delete iFbsBitGcBitmap;
	iFbsBitGcBitmap = NULL;
	}

void CT_DataFbsBitGcBitmap::DoCmdDestructor()
	{
	INFO_PRINTF1(KLogInfoCmdDestructor1);
	DestroyData();
	}

void CT_DataFbsBitGcBitmap::DoCmdNewL()
	{ 
	DestroyData();
	TInt err = KErrNone;
	INFO_PRINTF1(KLogInfoCmdnewL);
	TRAP(err, iFbsBitGcBitmap = new (ELeave) CFbsBitGcBitmap());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

void CT_DataFbsBitGcBitmap::DoCmdAddress(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("execute CFbsBitGcBitmap::Address()"));
	CBitwiseBitmap* bmp=iFbsBitGcBitmap->Address();
	if(bmp==NULL)
		{
		INFO_PRINTF1(_L("The bitmap address is NULL"));
		return;
		}
	
    TInt displaymode=bmp->DisplayMode();
	TSize size=bmp->SizeInPixels();
	TInt sizeH=size.iHeight;
	TInt sizeW=size.iWidth;
	TInt expectedWidth, expectedHeight;	
	if(GetIntFromConfig(aSection,KFldExpectedWidth, expectedWidth))
		{
        if(expectedWidth!=sizeW)
        	{
    		ERR_PRINTF3(_L("The bitmap width in pixel is: %d not as expected: %d"), sizeW, expectedWidth);
    		SetBlockResult(EFail);
        	}
        else
        	{
        	INFO_PRINTF2(_L("The bitmap width in pixel is: %d as expected"),sizeW);
        	}
		}

	if(GetIntFromConfig(aSection, KFldExpectedHeight, expectedHeight))
		{
		if(expectedHeight!=sizeH)
			{
			ERR_PRINTF3(_L("The bitmap height in pixel is: %d not as expected: %d"), sizeH, expectedHeight);
			SetBlockResult(EFail);
			}
		else
			{
			INFO_PRINTF2(_L("The bitmap height in pixel is: %d as expected"),sizeH);
			}
		}
	}

void CT_DataFbsBitGcBitmap::DoCmdLockHeap()
	{
	INFO_PRINTF1(_L("execute CFbsBitGcBitmap::LockHeap()"));
	iFbsBitGcBitmap->LockHeap();
	}

void CT_DataFbsBitGcBitmap::DoCmdUnlockHeap()
	{
	INFO_PRINTF1(_L("execute CFbsBitGcBitmap::UnlockHeap"));
	iFbsBitGcBitmap->UnlockHeap();
	}
