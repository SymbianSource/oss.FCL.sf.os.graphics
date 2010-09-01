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

#include "T_DataAnim.h"
#include "T_GraphicsUtil.h"
#include "T_AnimDef.h"

#define KMaxBufSize 250

/*@{*/
_LIT(KDataClassname,							"RAnim");

///	Fields
_LIT(KFldAnimDll,								"animdll");
_LIT(KFldWin,									"win");
_LIT(KFldSprite,								"sprite");
_LIT(KFldType,									"type");
_LIT(KFldParam,									"param");
_LIT(KFldIpcArgs,								"ipcargs");
_LIT(KFldOpcode,								"opcode");
_LIT(KFldArgs,									"args");
_LIT(KFldIpcArg,								"ipcargs%d");
_LIT(KFldExpectedIpcArg,						"expected_ipcargs%d");

// enum
_LIT(KEAnimWindows,								"EKWindowAnim");
_LIT(KEAnimSprite,								"EKSpriteAnim");
_LIT(KEAnimFreeTimer,							"EKFreeTimerWindowAnim");


_LIT(KEAnimCmdSync,								"ECmdSync");
_LIT(KEAnimCmdTestWithoutPara,					"ECmdTestWithoutPara");
_LIT(KEAnimCmdTestWithPara,						"ECmdTestWithPara");
_LIT(KEAnimCmdGetLast,							"ECmdGetLast");
_LIT(KEAnimCmdGetAnimInterval,					"ECmdGetAnimInterval");


// animate interval
_LIT(KFldCheckAnimateInterval,					"CheckAnimateInterval");
_LIT(KFldAnimIpcArgSlot,						"AnimIpcArgSlot");
_LIT(KFldAnimIntervalMin,						"AnimIntervalMin");
_LIT(KFldAnimIntervalMax,						"AnimIntervalMax");

_LIT(KIpcArgNothing,							"ENothing");

///	Commands
_LIT(KCmdnew,									"new");
_LIT(KCmdDestructorGeneral,						"~");
_LIT(KCmdDestructor,							"~RAnim");
_LIT(KCmdConstruct,								"Construct");
_LIT(KCmdCommandReply,							"CommandReply");
_LIT(KCmdCommand,								"Command");
_LIT(KCmdAsyncCommandReply,						"AsyncCommandReply");
_LIT(KCmdDestroy,								"Destroy");
_LIT(KCmdClose,									"Close");

///	Logging
_LIT(KLogErrNum,				"Error=%d");
_LIT(KLogErrMissingPara,		"Missing parameter '%S'");
_LIT(KLogInfoCmdnewL1,			"execute new RAnim(RAnimDll &)");
_LIT(KLogInfoCmdnewL2,			"execute new RAnim()");
_LIT(KLogErrConstruct,			"Construct return err = %d");
_LIT(KLogInfoCmdConstruct1,		"execute RAnim::Construct(const RWindowBase &, TInt, const TDesC8 &)");
_LIT(KLogInfoCmdConstruct2,		"execute RAnim::Construct(const RWindowBase &, TInt, const TDesC8 &, const TIpcArgs &)");
_LIT(KLogInfoCmdConstruct3,		"execute RAnim::Construct(const RWsSprite &, TInt, const TDesC8 &)");
_LIT(KLogInfoCmdConstruct4,		"execute RAnim::Construct(const RWsSprite &, TInt, const TDesC8 &, const TIpcArgs &)");
_LIT(KLogErrCommandReply,		"CommandReply return err = %d");
_LIT(KLogInfoCmdCommandReply1,	"execute RAnim::CommandReply(TInt)");
_LIT(KLogInfoCmdCommandReply2,	"execute RAnim::CommandReply(TInt, const TPtrC8 &)");
_LIT(KLogInfoCmdCommandReply3,	"execute RAnim::CommandReply(TInt, const TDesC8 &, const TIpcArgs &)");
_LIT(KLogErrCommand,			"Command return err = %d");
_LIT(KLogErrCheckIpcValue,		"Ipc arg compare fail expected=%S actual=%S");
_LIT(KLogErrIpcValue,			"Ipc arg return value err");

_LIT(KLogErrAnimInterval,		"Animate interval error expected min=%d max=%d");

_LIT(KLogInfoAnimInterval,		"Animate interval actual min=%d max=%d");

_LIT(KLogInfoCmdCommand1,		"execute RAnim::Command(TInt, const TPtrC8 &)");
_LIT(KLogInfoCmdCommand2,		"execute RAnim::Command(TInt)");
_LIT(KLogInfoCmdAsyncCommandReply1,	"execute RAnim::AsyncCommandReply(TRequestStatus &, TInt, const TIpcArgs &)");

_LIT(KLogInfoCmdClose1,			"execute RAnim::Close()");
_LIT(KLogInfoCmdDestroy1,		"execute RAnim::Destroy()");
_LIT(KLogInfoCmdDestructor1,	"execute ~RAnim()");
/*@}*/

CT_DataAnim* CT_DataAnim::NewL()
	{
	CT_DataAnim*	ret = new (ELeave) CT_DataAnim();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataAnim::CT_DataAnim()
:	iActiveCallback(NULL)
	,iAnim(NULL)
	{
	}

void CT_DataAnim::ConstructL()
	{
	iActiveCallback = CActiveCallback::NewL(*this);
	iSendBuf.CreateL(KMaxBufSize);
	
	for (TInt i=0;i<KIpcArgNum;i++)	
		iIpcBuf[i].CreateL(KMaxBufSize);
	}

CT_DataAnim::~CT_DataAnim()
	{
	DestroyData();
	delete iActiveCallback;
	iActiveCallback = NULL;
	
	iSendBuf.Close();
	
	for (TInt i=0;i<KIpcArgNum;i++)	
		iIpcBuf[i].Close();	
	}

TAny* CT_DataAnim::GetObject()
	{
	return iAnim;
	}

void CT_DataAnim::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iAnim = static_cast<T_RAnimChild*> (aAny);
	}

void CT_DataAnim::DisownObjectL()
	{
	iAnim = NULL;
	}

void CT_DataAnim::DestroyData()
	{
	DoCmdDestructor();
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
TBool CT_DataAnim::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
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
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstructL(aSection);
		}
	else if ( aCommand==KCmdCommandReply )
		{
		DoCmdCommandReply(aSection);
		}
	else if ( aCommand==KCmdCommand )
		{
		DoCmdCommand(aSection);
		}
	else if ( aCommand==KCmdAsyncCommandReply )
		{
		DoCmdAsyncCommandReply(aSection, aAsyncErrorIndex);
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
		ret=EFalse;
		}

	return ret;
	}

void CT_DataAnim::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	TPtrC		datDllName;
	RAnimDll*	animDll = NULL;
	if ( GetStringFromConfig(aSection, KFldAnimDll, datDllName) )
		{
		animDll = static_cast<RAnimDll*>(GetDataObjectL(datDllName));
		}

	TInt err = KErrNone;
	if ( animDll )
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogInfoCmdnewL1);
		iAnim = new (ELeave) T_RAnimChild(*animDll) ;
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(KLogInfoCmdnewL2);
		iAnim = new (ELeave) T_RAnimChild();
		}
	}

TBool CT_DataAnim::GetIpcArg(const TDesC& aSection)
	{
	TBool argexist=EFalse;
	
	TBuf <100> field;
	TPtrC    val;
	for (TInt i=0;i<KIpcArgNum;i++)	
		{
		field.Format(KFldIpcArg,i);
		if ( GetStringFromConfig(aSection, field, val))
			{
			if (val.Compare(KIpcArgNothing)==0)
				iIpcArgs.Set(i,TIpcArgs::ENothing);
			else
				{
				iIpcBuf[i].Copy(val);
				iIpcArgs.Set(i,&iIpcBuf[i]);
				}
			argexist=ETrue;
			}
		else
			break;
		
		}	
	return argexist;	
	}

// check is the time interval of CAnim::Animate() is in specified scope
TBool CT_DataAnim::CheckAnimateInterval(const TDesC& aSection)
	{
	TBool result=EFalse;	
			
	TInt slot,min,max,find;
	TInt amin,amax;

	_LIT(KSpace," ");
	
	// get IPC args slot index
	if ( GetIntFromConfig(aSection, KFldAnimIpcArgSlot,slot))
		{
		// get expected min/max interval of Animate()
		if ( GetIntFromConfig(aSection, KFldAnimIntervalMin,min) && 
				GetIntFromConfig(aSection, KFldAnimIntervalMax,max) )
			{
			find=iIpcBuf[slot].Find(KSpace); // get actual min/max interval of Animate()
			if (find>0 )
				{
				// get actual min interval 
				TPtrC val1=iIpcBuf[slot].Left(find);
				TLex lex1(val1);
				lex1.Val(amin);
				
				// get actual max interval 
				TPtrC val2=iIpcBuf[slot].Mid(find+1);
				TLex lex2(val2);
				lex2.Val(amax);
				
				INFO_PRINTF3(KLogInfoAnimInterval,amin,amax);
				
				// check is the interval in expected scope 
				if (amin<min || amax>max)
					{
					ERR_PRINTF3(KLogErrAnimInterval, min,max);
					}
				else
					result=ETrue;
				}			
			}
		}	

	return result;	
	}
TBool CT_DataAnim::CheckIpcArg(const TDesC& aSection)
	{
	TBool result=ETrue;	

	// check animate interval
	TInt checkanim=0;
	if ( GetIntFromConfig(aSection, KFldCheckAnimateInterval, checkanim) 
			&& checkanim)
		{
		return CheckAnimateInterval(aSection);
		}
			
	TBuf <100> field;
	TPtrC    val;
	
	// check ipc expected value
	for (TInt i=0;i<KIpcArgNum;i++)	
		{
		field.Format(KFldExpectedIpcArg,i);
		if ( GetStringFromConfig(aSection, field, val) && 
				val.Compare(iIpcBuf[i])!=0)
			{
			ERR_PRINTF3(KLogErrCheckIpcValue, &val, &iIpcBuf[i]);
			result=EFalse;
			break;
			}		
		}	
	return result;	
	}

void CT_DataAnim::DoCmdConstructL(const TDesC& aSection)
	{
	TInt err = KErrNone;
	
	// Get test data for command input parameter(s)
	TBool dataOk = ETrue;

	// get "const RWindowBase &aDevice" or "const RWsSprite &aDevice"
	TPtrC        datWinName;
	RWindowBase* win = NULL;
	TPtrC        datSpriteName;
	RWsSprite*   sprite = NULL;
	if ( GetStringFromConfig(aSection, KFldWin, datWinName))
		{
		win = static_cast<RWindowBase*>(GetDataObjectL(datWinName));
		}
	else if ( GetStringFromConfig(aSection, KFldSprite, datSpriteName))
		{
		sprite = static_cast<RWsSprite*>(GetDataObjectL(datSpriteName));
		}
	if (!win && !sprite )
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldWin);
		ERR_PRINTF2(KLogErrMissingPara, &KFldSprite);
		}
	
	//get "TInt aType"
	TInt datType;
	if ( !ReadAnimType(aSection, KFldType, datType))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldType);
		}
	
	// get "const TDesC8 &aArgs"
	TPtrC 	inputStr;
	TBool argexist=EFalse;
	if( GetStringFromConfig(aSection, KFldParam, inputStr))
		{
		CopyToSendBuf(inputStr);
		argexist=ETrue;
		}

	if (!argexist)
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldParam);
		}
	
	// get "const TIpcArgs &aIpcArgs"
	TBool ipcexist= EFalse;
	if ( GetIpcArg(aSection))
		ipcexist = ETrue;
	
	if ( !dataOk )
		{
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		if (win )
			{
			if (!ipcexist)
				{
				INFO_PRINTF1(KLogInfoCmdConstruct1);
				err = iAnim->Construct(*win, datType, iSendBuf);				
				}
			else
				{
				INFO_PRINTF1(KLogInfoCmdConstruct2);
				err = iAnim->Construct(*win, datType, iSendBuf, iIpcArgs);								
				}
			}
		else
			{
			if (!ipcexist )
				{
				INFO_PRINTF1(KLogInfoCmdConstruct3);
				err = iAnim->Construct(*sprite, datType, iSendBuf);				
				}
			else
				{
				INFO_PRINTF1(KLogInfoCmdConstruct4);
				err = iAnim->Construct(*sprite, datType, iSendBuf, iIpcArgs);
				}
			}
		
		// Check the command return code
		if(err != KErrNone)
			{
			ERR_PRINTF2(KLogErrConstruct, err);
			SetError(err);
			}
		else if (ipcexist && ! CheckIpcArg(aSection))
			{
			ERR_PRINTF1(KLogErrIpcValue);
			SetBlockResult(EFail);
			}			
		}
	}

TDesC8& CT_DataAnim::CopyToSendBuf(const TDesC& buf) 
	{
	TUint8 len=buf.Length();

	iSendBuf.Zero();
	iSendBuf.Copy(&len,sizeof(len));
	if (len>0)
		iSendBuf.Append(buf);
	return iSendBuf;
	}

void CT_DataAnim::DoCmdCommandReply(const TDesC& aSection)
	{
	TInt err = KErrNone;
	
	// Get test data for command input parameter(s)
	TBool dataOk = ETrue;

	// get "TInt aOpcode"
	TInt datOpcode;
	if ( !ReadAnimCommand(aSection, KFldOpcode, datOpcode))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldOpcode);
		}

	// get "const TPtrC8 &aArgs" or "const TDesC8 &aArgs"
	TPtrC 	inputStr;
	TBool argexist=EFalse;
	if( GetStringFromConfig(aSection, KFldArgs, inputStr))
		{
		CopyToSendBuf(inputStr);
		argexist=ETrue;
		}
	
	// get "const TIpcArgs &aIpcArgs"
	TBool ipcexist= EFalse;
	if ( GetIpcArg(aSection))
		ipcexist = ETrue;
	

	if ( dataOk )
		{
		if (!argexist)
			{
			INFO_PRINTF1(KLogInfoCmdCommandReply1);
			err = iAnim->CommandReply(datOpcode);
			}
		else if (ipcexist)
			{
			INFO_PRINTF1(KLogInfoCmdCommandReply3);
			err = iAnim->CommandReply(datOpcode, iSendBuf, iIpcArgs);
			}			
		else
			{			
			INFO_PRINTF1(KLogInfoCmdCommandReply2);
			err = iAnim->CommandReply(datOpcode, iSendBuf);
			}	
		
		// Check the command return code
		if(err != KErrNone)
			{
			ERR_PRINTF2(KLogErrCommandReply, err);
			SetError(err);
			}
		else if (ipcexist && ! CheckIpcArg(aSection))
			{
			ERR_PRINTF1(KLogErrIpcValue);
			SetBlockResult(EFail);
			}			
		}
	else
		SetBlockResult(EFail);

	}

void CT_DataAnim::DoCmdCommand(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TBool dataOk = ETrue;

	// get "TInt aOpcode"
	TInt datOpcode;
	if ( !ReadAnimCommand(aSection, KFldOpcode, datOpcode))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldOpcode);
		}

	// get "const TPtrC8 &aArgs" or "const TDesC8 &aArgs"
	TPtrC 	inputStr;
	TBool argexist=EFalse;
	if( GetStringFromConfig(aSection, KFldArgs, inputStr))
		{
		CopyToSendBuf(inputStr);
		argexist=ETrue;
		}
	
	if ( dataOk )
		{
		if (!argexist)
			{
			INFO_PRINTF1(KLogInfoCmdCommand2);
			iAnim->Command(datOpcode);
			}
		else
			{			
			INFO_PRINTF1(KLogInfoCmdCommand1);
			iAnim->Command(datOpcode, iSendBuf);
			}	
		}
	else
		SetBlockResult(EFail);

	}

void CT_DataAnim::DoCmdAsyncCommandReply(const TDesC& aSection, const TInt aAsyncErrorIndex)
	{
	// Get test data for command input parameter(s)
	TBool dataOk = ETrue;

	// get "TInt aOpcode"
	TInt datOpcode;
	if ( !ReadAnimCommand(aSection, KFldOpcode, datOpcode))
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldOpcode);
		}

	// get "const TIpcArgs &aIpcArgs"
	TBool ipcexist= EFalse;
	if ( GetIpcArg(aSection))
		ipcexist = ETrue;
	
	if (!ipcexist)
		{
		dataOk = EFalse;
		ERR_PRINTF2(KLogErrMissingPara, &KFldIpcArgs);		
		}
	
	if ( !dataOk )
		{
		SetBlockResult(EFail);
		}
	else
		{
		iAsyncCmdSection.Copy(aSection);
		// Execute command and log parameters
		INFO_PRINTF1(KLogInfoCmdAsyncCommandReply1);
		iAnim->AsyncCommandReply(iActiveCallback->iStatus, datOpcode, iIpcArgs);
	    iActiveCallback->Activate(aAsyncErrorIndex);
	    IncOutstanding();
		}

	}

void CT_DataAnim::RunL(CActive* aActive, TInt aIndex)
    {
    TInt err = aActive->iStatus.Int();  
    
    if (! CheckIpcArg(iAsyncCmdSection))
		{
		ERR_PRINTF1(KLogErrIpcValue);
		SetBlockResult(EFail);
		}   
    
    SetAsyncError(aIndex, err);
    DecOutstanding();
	}

void CT_DataAnim::DoCmdClose()
	{
	// Execute command and log parameters
	if (iAnim != NULL)
		{
		INFO_PRINTF1(KLogInfoCmdClose1);
		iAnim->Close();
		}
	}

void CT_DataAnim::DoCmdDestroy()
	{
	// Execute command and log parameters
	if (iAnim != NULL)
		{
		INFO_PRINTF1(KLogInfoCmdDestroy1);
		iAnim->Destroy();
		iAnim = NULL;
		}
	}

void CT_DataAnim::DoCmdDestructor()
	{
	// Execute command and log parameters
	INFO_PRINTF1(KLogInfoCmdDestructor1);
	delete iAnim;
	iAnim = NULL;
	}


TBool CT_DataAnim::ReadAnimCommand(const TDesC& aSectName, const TDesC& aKeyName, TInt& aCommand)
	{
	TPtrC	str;
	TBool	ret=GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KEAnimCmdSync )
			{
			aCommand=ECmdSync;
			}
		else if ( str==KEAnimCmdTestWithoutPara )
			{
			aCommand=ECmdTestWithoutPara;
			}
		else if ( str==KEAnimCmdTestWithPara )
			{
			aCommand=ECmdTestWithPara;
			}
		else if ( str==KEAnimCmdGetLast )
			{
			aCommand=ECmdGetLast;
			}
		else if ( str==KEAnimCmdGetAnimInterval )
			{
			aCommand=ECmdGetAnimInterval;
			}
		else
			ret=EFalse;
		}

	return ret;
	}
TBool CT_DataAnim::ReadAnimType(const TDesC& aSectName, const TDesC& aKeyName, TInt& aType)
	{
	TPtrC	str;
	TBool	ret=GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KEAnimWindows)
			{
			aType=EKWindowAnim;
			}
		else if ( str==KEAnimSprite )
			{
			aType=EKSpriteAnim;
			}
		else if ( str==KEAnimFreeTimer )
			{
			aType=EKFreeTimerWindowAnim;
			}
		else
			ret=EFalse;
		}

	return ret;	
	}
