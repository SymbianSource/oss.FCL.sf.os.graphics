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

#include "T_DataWsGraphicMsgBuf.h"

/*@{*/
//Parameters
_LIT(KExpected,					"expected");
_LIT(KInputTypeId,				"inputTypeId");
_LIT(KInputText,				"inputText");
_LIT(KInputUseText8,			"inputUseText8");
_LIT(KInputMsgLen,				"inputMsgLen");
_LIT(KInputMsgPosition,			"inputMsgPosition");
_LIT(KInputFormat,				"inputFormat");

//Commands
_LIT(KCmdnew,					"new");
_LIT(KCmdClose,					"Close");
_LIT(KCmdCleanupClosePushL,		"CleanupClosePushL");
_LIT(KCmdAppend,				"Append");
_LIT(KCmdRemove,				"Remove");
_LIT(KCmdCount,					"Count");
_LIT(KCmdTypeId,				"TypeId");
_LIT(KCmdData,					"Data");
_LIT(KCmdGetFixedMsg,			"GetFixedMsg");
_LIT(KCmdPckg,					"Pckg");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphicMsgBuf* CT_DataWsGraphicMsgBuf::NewL()
    {
    CT_DataWsGraphicMsgBuf*    ret=new (ELeave) CT_DataWsGraphicMsgBuf();
    CleanupStack::PushL(ret);
    ret->ConstructL();
    CleanupStack::Pop(ret);
    return ret;
    }

CT_DataWsGraphicMsgBuf::CT_DataWsGraphicMsgBuf()
:    CDataWrapperBase()
,    iWsGraphicMsgBuf(NULL)
    {
    }

void CT_DataWsGraphicMsgBuf::ConstructL()
    {
    }

CT_DataWsGraphicMsgBuf::~CT_DataWsGraphicMsgBuf()
    {
    DestroyData();
    }

void CT_DataWsGraphicMsgBuf::SetObjectL(TAny* aAny)
    {
    DestroyData();
    iWsGraphicMsgBuf = static_cast<RWsGraphicMsgBuf*> (aAny);
    }

void CT_DataWsGraphicMsgBuf::DisownObjectL()
	{
	iWsGraphicMsgBuf = NULL;
	}

void CT_DataWsGraphicMsgBuf::DestroyData()
    {
    delete iWsGraphicMsgBuf;
    iWsGraphicMsgBuf=NULL;
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
TBool CT_DataWsGraphicMsgBuf::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
    {
    TBool    ret = ETrue;

    if ( aCommand==KCmdnew )
        {
        DoCmdnew();
        }
    else if ( aCommand==KCmdClose )
        {
        DoCmdClose();
        }
    else if ( aCommand==KCmdCleanupClosePushL )
        {
        DoCmdCleanupClosePushL();
        }
    else if ( aCommand==KCmdAppend )
        {
        DoCmdAppendL(aSection);
        }
    else if ( aCommand==KCmdRemove )
        {
        DoCmdRemove(aSection);
        }
    else if ( aCommand==KCmdCount )
        {
        DoCmdCount(aSection);
        }
    else if ( aCommand==KCmdTypeId )
        {
        DoCmdTypeId(aSection);
        }
    else if ( aCommand==KCmdData )
        {
        DoCmdDataL(aSection);
        }
    else if( aCommand==KCmdGetFixedMsg)
		{
		DoCmdGetFixedMsg(aSection);
		}
    else if ( aCommand==KCmdPckg )
        {
        DoCmdPckg();
        }
    else
        {
        ret=EFalse;
        }

    return ret;
    }


void CT_DataWsGraphicMsgBuf::DoCmdnew()
    {
	DestroyData();
	
	TRAPD(err,	iWsGraphicMsgBuf =	new	(ELeave) RWsGraphicMsgBuf());
	if(err	!= KErrNone)
		{
		ERR_PRINTF2(_L("**** RWsGraphicMsgBuf	Constructor	failed	with error %d"), err);
		SetError(err);
		}

	INFO_PRINTF1(_L("RWsGraphicMsgBuf::RWsGraphicMsgBuf"));
	}

void CT_DataWsGraphicMsgBuf::DoCmdClose()
	{
	iWsGraphicMsgBuf->Close();
	INFO_PRINTF1(_L("RWsGraphicMsgBuf::Close"));
    }

void CT_DataWsGraphicMsgBuf::DoCmdCleanupClosePushL()
	{
	iWsGraphicMsgBuf->CleanupClosePushL();
	CleanupStack::PopAndDestroy(iWsGraphicMsgBuf);
	INFO_PRINTF1(_L("RWsGraphicMsgBuf::CleanupClosePushL"));
	}

void CT_DataWsGraphicMsgBuf::DoCmdAppendL(const TDesC& aSection)
    {		
	TInt	err=KErrNone;
	TInt	inputTypeId=0;
	if(	GetIntFromConfig(aSection, KInputTypeId(), inputTypeId))
		{
		TUid	uid = TUid::Uid(inputTypeId);
		TPtrC 	inputStr;
		if( !GetStringFromConfig(aSection, KInputText(), inputStr) )
			{
			ERR_PRINTF2(_L("Missing parameter %S"), &KInputText());
			SetBlockResult(EFail);
			}
		else
			{
			TBool useBuf8=EFalse;
			GetBoolFromConfig(aSection, KInputUseText8(), useBuf8);
			if ( useBuf8 )
				{
				TInt		inputMsgLen=0;
				HBufC8*		tempStr = HBufC8::NewLC(inputStr.Length());
				tempStr->Des().Copy(inputStr);
				if(	GetIntFromConfig(aSection, KInputMsgLen(), inputMsgLen))
					{
					// Test iWsGraphicMsgBuf->Append(TUid aTypeId, TInt aLen, TPtr8 &aPtr);
					TPtr8		tempInputStr(tempStr->Des());
					err = iWsGraphicMsgBuf->Append(uid, inputMsgLen, tempInputStr);
					INFO_PRINTF1(_L("RWsGraphicMsgBuf::Append  (TPtr8)"));
					}
				else
					{
					// Test iWsGraphicMsgBuf->Append(TUid aTypeId, const TDesC8 &aData);
					err	= iWsGraphicMsgBuf->Append(uid,	*tempStr);
					INFO_PRINTF1(_L("RWsGraphicMsgBuf::Append  (TDesC8)"));
					}
				CleanupStack::PopAndDestroy(tempStr);
				}
			else
				{
				//  Test iWsGraphicMsgBuf->Append(TUid aTypeId, const TDesC16 &aData);
				err	= iWsGraphicMsgBuf->Append(uid,	inputStr);
				INFO_PRINTF1(_L("RWsGraphicMsgBuf::Append (TDesC16)"));
				}
			}
		}
	else
		{
		TWsGraphicFrameRate		frameRate;
		err	= iWsGraphicMsgBuf->Append(frameRate);
		INFO_PRINTF1(_L("RWsGraphicMsgBuf::Append	(TWsGraphicMsgFixedBase)"));
		}
		
	if(err != KErrNone)
		{
		ERR_PRINTF2(_L("**** Append failed with error %d"),	err);
		SetError(err);
		}
	}

void CT_DataWsGraphicMsgBuf::DoCmdRemove(const TDesC& aSection)
	{
	TInt	inputMsgPosition=0;
	if(	GetIntFromConfig(aSection, KInputMsgPosition(), inputMsgPosition))
		{
		iWsGraphicMsgBuf->Remove(inputMsgPosition);
		INFO_PRINTF1(_L("RWsGraphicMsgBuf::Remove"));
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KInputMsgPosition());
		SetBlockResult(EFail);
    	}
	}

void CT_DataWsGraphicMsgBuf::DoCmdCount(const TDesC& aSection)
	{
	TInt	actual = iWsGraphicMsgBuf->Count();
	INFO_PRINTF2(_L("RWsGraphicMsgBuf::Count = %d"), actual);

	TInt	expected = 0;
	if(	GetIntFromConfig(aSection, KExpected(), expected))
		{
		if( actual != expected )
			{
			ERR_PRINTF1(_L("Count is not as expected!"));
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KExpected());
		SetBlockResult(EFail);
		}
	}

void CT_DataWsGraphicMsgBuf::DoCmdTypeId(const TDesC& aSection)
	{
	TInt	inputMsgPosition=0;
	if(	GetIntFromConfig(aSection, KInputMsgPosition(), inputMsgPosition))
		{
		TUid	actual = iWsGraphicMsgBuf->TypeId(inputMsgPosition);
		INFO_PRINTF2(_L("RWsGraphicMsgBuf::TypeId = %d"), actual.iUid);

		TInt	expected = 0;
		if(	GetIntFromConfig(aSection, KExpected(), expected))
			{
			TUid	expectedUid = TUid::Uid(expected);

			if( actual != expectedUid )
				{
				ERR_PRINTF1(_L("TypeId is not as expected!"));
				SetBlockResult(EFail);
				}
			}
		else
			{
			ERR_PRINTF2(_L("Missing parameter %S"), &KExpected());
			SetBlockResult(EFail);
			}
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KInputMsgPosition());
		SetBlockResult(EFail);
		}
	}

void CT_DataWsGraphicMsgBuf::DoCmdDataL(const TDesC& aSection)
	{
	TInt     inputMsgPosition=0;
	if( !GetIntFromConfig(aSection, KInputMsgPosition(), inputMsgPosition) )
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KInputMsgPosition());
		SetBlockResult(EFail);
		}
	else
		{
		TPtrC     format;
		if( GetStringFromConfig(aSection, KInputFormat(), format) )
			{
			//     Test modifiable Data
			TPtr8     actual=iWsGraphicMsgBuf->Data(inputMsgPosition);
			HBufC*    buffer=HBufC::NewLC(actual.Length());
			TPtr      bufferPtr=buffer->Des();

			bufferPtr.Copy(actual);
			INFO_PRINTF1(_L("RWsGraphicMsgBuf::Data ="));
			INFO_PRINTF1(bufferPtr);
			bufferPtr.Format(format, &bufferPtr);
			actual.Copy(bufferPtr);
			CleanupStack::PopAndDestroy(buffer);
			}
		else
			{
			//     Test non-modifiable Data
			const RWsGraphicMsgBuf*	wsGraphicMsgBuf=iWsGraphicMsgBuf;
			TPtrC8					actual=wsGraphicMsgBuf->Data(inputMsgPosition);
			HBufC*					buffer=HBufC::NewLC(actual.Length());
			TPtr					bufferPtr=buffer->Des();

			bufferPtr.Copy(actual);
			INFO_PRINTF1(_L("RWsGraphicMsgBuf::Data ="));
			INFO_PRINTF1(bufferPtr);
			TPtrC     expected;
			if( !GetStringFromConfig(aSection, KExpected(), expected) )
				{
				ERR_PRINTF2(_L("Missing parameter %S"), &KInputMsgPosition());
				SetBlockResult(EFail);
				}
			 else
				{
				if( bufferPtr!=expected )
					{
					ERR_PRINTF1(_L("Data is not as expected!"));
					SetBlockResult(EFail);
					}
				}
				CleanupStack::PopAndDestroy(buffer);
			}
		}
	}

void CT_DataWsGraphicMsgBuf::DoCmdGetFixedMsg(const	TDesC& aSection)
	{
	TInt	inputMsgPosition=0;
	if(	GetIntFromConfig(aSection, KInputMsgPosition(), inputMsgPosition))
		{
		TWsGraphicFrameRate		frameRate;
		iWsGraphicMsgBuf->GetFixedMsg(frameRate, inputMsgPosition);
		INFO_PRINTF1(_L("RWsGraphicMsgBuf::GetFixedMsg"));
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter	%S"), &KInputMsgPosition());
		SetBlockResult(EFail);
		}
	}

void CT_DataWsGraphicMsgBuf::DoCmdPckg()
	{
	TBuf8<KMaxTestExecuteCommandLength>		msg;
	msg = iWsGraphicMsgBuf->Pckg();
	INFO_PRINTF1(_L("RWsGraphicMsgBuf::Pckg"));
	}
