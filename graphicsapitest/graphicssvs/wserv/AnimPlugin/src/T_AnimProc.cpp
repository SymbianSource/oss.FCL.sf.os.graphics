// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
// 
// Description:
//

#include "T_AnimProc.h"
#include "T_AnimDef.h"

#define KMaxBufSize 250

_LIT(KLim1,";");
_LIT(KLim2,",");

_LIT8(KSyncSecond,"ESyncSecond");
_LIT8(KSyncMinute,"ESyncMinute");
_LIT8(KSyncFlash,"ESyncFlash");
_LIT8(KSyncNone,"ESyncNone");


CAnimProc::CAnimProc(CTestAnimDll* aDll,TInt aType)
	{
	for (TInt i=0;i<KAnimTimeSize;i++)
		iAnimTime[i]=0;

	iNewPoint.SetXY (0, 0);
	iLastPoint.SetXY (0, 0);

	iStepX=0;
	iStepY=0;
	iDll=aDll;
	
	iAnimType=aType;

	}


CAnimProc::~CAnimProc()
	{
	iLastCommand.Close();
	}
TInt CAnimProc::WriteLog(const TDesC& aInfo)
	{
	return iDll->WriteLog(aInfo);
	}
TInt CAnimProc::WriteLog(const TDesC8& aInfo)
	{
	return iDll->WriteLog(aInfo);
	}



void CAnimProc::ConvertParam(TDes8& buf, TAny *aArgs)
	{
	if ( aArgs)
		{
		buf.Zero();

		TUint8* p=STATIC_CAST(TUint8*,aArgs);
		TInt len=p[0];

		if ( len>0)
			buf.Copy (p+1, len);
		}
	}

void CAnimProc::RecordLastCmd(TInt cmd)
	{
	iLastCommand.Format(_L("%d"),cmd);
	}
void CAnimProc::RecordLastCmd(TInt cmd,const TDesC8& str)
	{
	TBuf<KMaxBufSize> buf;
	buf.Copy(str);
	iLastCommand.Format(_L("%d %S"),cmd,&buf);
	}
void CAnimProc::RecordLastCmd(const TDesC8& str)
	{
	iLastCommand.Copy(str);
	}
TInt CAnimProc::CmdReply(MAnimGeneralFunctions* pFunc , TInt aOpcode,
		TAny *aArgs)
	{
	TBuf<KMaxBufSize> logbuf;
	TBuf<KMaxBufSize> msgbuf;
	logbuf.Format (_L("CAnimProc::CmdReplyL %d"),aOpcode);
	WriteLog(logbuf);
	
	TBuf8<KMaxBufSize> arg;		
			
	TInt ret,slot;
	TBool msgexist=EFalse,argexist=ETrue,userpanic=EFalse;
	
	// check is msg exist
	const RMessagePtr2 *msg=pFunc->Message();
	if ( msg && !msg->IsNull())
		{
		for (TInt i=1;i<4;i++)
			{
			ret=msg->Read (i, msgbuf);
			if ( ret==KErrNone)
				{
				slot=i;
				msgexist=ETrue;
				break;
				}
			}
		}	

	switch (aOpcode)
		{		
		case ECmdTestWithoutPara:
			{	
			argexist=EFalse;			
			if (msgexist)	
				{
				RecordLastCmd (aOpcode);				
				ret=msg->Write(slot,iLastCommand);	
				// this function should only used for AsyncCommandReply, otherwise emulator will crash.
				msg->Complete(KErrNone); 
				}
			break;
			}
		case ECmdTestWithPara:
			{		
			ConvertParam(arg, aArgs);
			WriteLog(arg);		
			if (msgexist)	
				{
				RecordLastCmd (aOpcode, arg);				
				msg->Write(slot,iLastCommand);	
				}
			break;
			}
		case ECmdGetLast:
			{
			argexist=EFalse;
			if (msgexist)	
				{				
				logbuf.Format (_L("send last command %S"),&iLastCommand);
				WriteLog(logbuf);
				msg->Write(slot,iLastCommand);	
				}
			else
				userpanic=ETrue;				
			break;
			}
		case ECmdSync:
			{
			ConvertParam(arg, aArgs);
			WriteLog(arg);			
			MAnimGeneralFunctions::TAnimSync sync;
			if (arg.Compare(KSyncSecond)==0)
				sync=MAnimGeneralFunctions::ESyncSecond;
			else if (arg.Compare(KSyncNone)==0)
				sync=MAnimGeneralFunctions::ESyncNone;
			else if (arg.Compare(KSyncFlash)==0)
				sync=MAnimGeneralFunctions::ESyncFlash;
			else if (arg.Compare(KSyncMinute)==0)
				sync=MAnimGeneralFunctions::ESyncMinute;
			else
				{
				userpanic=ETrue;
				break;
				}

			pFunc->SetSync(sync);
			break;
			}
		case ECmdGetAnimInterval:
			{
			argexist=EFalse;	
			if (msgexist)
				{
					TInt i1,i2;
					i1=(iAnimTime[1]-iAnimTime[0])/1000;
					i2=(iAnimTime[2]-iAnimTime[1])/1000;
					if (i1>i2)
						{
						TInt tmp;
						tmp=i1;
						i1=i2;
						i2=tmp;
						}
					msgbuf.Format(_L("%d %d"),i1,i2);
					msg->Write(slot,msgbuf);	
				}
			else
				userpanic=ETrue;
			break;
			}		
		default:		
			{
			userpanic=ETrue;
			break;
			}	
		}
	
	if ( userpanic)
		pFunc->Panic ();
	else
		{
		if ( argexist)
			RecordLastCmd (aOpcode, arg);
		else
			RecordLastCmd (aOpcode);
		

		}
	return KErrNone ;
	}

CDesCArray* CAnimProc::ProcParamLC(const TDesC8& param)
	{		
	CDesCArrayFlat * ary= new (ELeave)  CDesCArrayFlat(2);   
	CleanupStack::PushL(ary);
	if ( param.Length ()>0)
		{
		TBuf<KMaxBufSize> buf;
		TBuf<KMaxBufSize> tmp;		
		buf.Copy(param);
		
		TInt idx;

		do
			{
			idx=buf.Find(KLim1);
			if (idx!=KErrNotFound )
				{

				tmp.Copy(buf.Ptr(),idx);
				ary->AppendL(tmp);					
				buf.Delete(0,idx+1);
				}
			}

		while (idx!=KErrNotFound);
		
		if (buf.Length()>0)
			ary->AppendL(buf);	
		}
	return ary;
	}
TInt CAnimProc::GetIntFromStr(const TDesC& str,RIntArray& ary)
	{	
	ary.Reset();
	TInt val;
	
	if ( str.Length ()>0)
		{
		TBuf<KMaxBufSize> buf;
		TBuf<KMaxBufSize> tmp;	
		buf.Copy(str);

		TInt idx;
		do
			{
			idx=buf.Find(KLim2);
			if (idx!=KErrNotFound )
				{
				tmp.Copy(buf.Ptr(),idx);
				TLex lex(tmp);
				lex.Val(val);
				ary.Append(val);
				buf.Delete(0,idx+1);
				}
			}

		while (idx!=KErrNotFound);
		
		if (buf.Length()>0)
			{
			TLex lex(buf);
			lex.Val(val);
			ary.Append(val);	
			}
		}	

	return KErrNone;
	}
void CAnimProc::RecordAnimTime()
	{	
	TTime currentTime;
	currentTime.HomeTime();	
	
	for (TInt i=0;i<KAnimTimeSize-1;i++)
		iAnimTime[i]=iAnimTime[i+1];
	
	iAnimTime[KAnimTimeSize-1]=currentTime.Int64();	
	}
TInt CAnimProc::ConstructWinL(TAny *aArgs,MAnimGeneralFunctions* pFunc)
	{		
	iLastCommand.CreateL(KMaxBufSize);
	
	TInt ret,val;	
	
	TBuf8<KMaxBufSize> param;
	ConvertParam(param, aArgs);
	WriteLog(param);
	
	RecordLastCmd(iAnimType,param);
	
	TBuf<KMaxBufSize> buf;
	
	CDesCArray* ary=ProcParamLC(param);
	TInt len=ary->Count();
	
	RIntArray intary;
	CleanupClosePushL(intary);
	
	
	for (TInt i=0;i<len;i++)
		{
		const TDesC& str=(*ary)[i];
//		WriteLog (str);
		switch (i)
		{
		case 0: //start point
			{
			GetIntFromStr(str,intary);
			if (intary.Count()!=2)
				ret=KErrArgument;
			else
				{
				iLastPoint.SetXY( intary[0], intary[1] );
			    iNewPoint.SetXY( intary[0], intary[1] );
			    buf.Format(_L("start poinx x=%d y=%d"),iNewPoint.iX,iNewPoint.iY);
			    WriteLog (buf);
				}
			break;
			}
		case 1: //step
			{
			GetIntFromStr(str,intary);
			if (intary.Count()!=2)
				ret=KErrArgument;
			else
				{
				iStepX = intary[0];
			    iStepY =intary[1];			  
			    buf.Format(_L("step x=%d y=%d"),iStepX,iStepY);
			    WriteLog (buf);
				}
			break;
			}
		case 2: //draw type
			{
			TLex lex(str);
			lex.Val(val);
			iDrawType=val;
			buf.Format(_L("draw type=%d"),iDrawType);
			WriteLog (buf);
			break;
			}			
		case 3: //draw rect
			{
			GetIntFromStr(str,intary);
			if (intary.Count()!=2)
				ret=KErrArgument;
			else
				{
				iSize.SetSize( intary[0], intary[1]);
			    buf.Format(_L("draw rect width=%d height=%d"),iSize.iWidth,iSize.iHeight);
			    WriteLog (buf);
				}
			break;
			}
		case 4: //pen color
			{
			GetIntFromStr(str,intary);
			if (intary.Count()!=3)
				ret=KErrArgument;
			else
				{
				iColor.SetRed( intary[0]);
				iColor.SetGreen( intary[1]);
				iColor.SetBlue( intary[2]);
			    buf.Format(_L("pen color r=%d g=%d b=%d"),iColor.Red(),iColor.Green(),iColor.Blue());
			    WriteLog (buf);
				}
			break;
			}
		case 5: //pen size
			{
			GetIntFromStr(str,intary);
			if (intary.Count()!=2)
				ret=KErrArgument;
			else
				{
				iPenSize.SetSize( intary[0], intary[1]);
			    buf.Format(_L("pen size width=%d height=%d"),iPenSize.iWidth,iPenSize.iHeight);
			    WriteLog (buf);
				}
			break;
			}						
		}
		}
	

    // initiates an initial position update
    iDrawn = ETrue;
 
    CleanupStack::PopAndDestroy(&intary);	
	CleanupStack::PopAndDestroy(ary);	
	
	const RMessagePtr2 *msg=pFunc->Message();
	if ( msg && !msg->IsNull())
		{
		TBuf<KMaxBufSize> msgbuf;
		ret=msg->Read (1, msgbuf);
		if ( ret==KErrNone)
			{
			msg->Write(1,iLastCommand);	
			}
		}	
	
	return ret;
	}

