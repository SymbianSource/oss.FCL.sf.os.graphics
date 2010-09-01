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


#include "T_log.h"

#define DES_AS_8_BIT(str) (TPtrC8((TText8*)((str).Ptr()), (str).Size()))
_LIT8(KLimit,"\r\n");
_LIT8(KTimeFormat, "%2d:%2d:%3d  ");

CLog::CLog()
	{
	
	}

CLog::~CLog()
	{
	iFile.Close();
	
	iBuf.Close();
	}

CLog* CLog::NewLC(RFs &aFs, const TDesC &aFileName)
	{
	CLog* self = new (ELeave)CLog();
	CleanupStack::PushL(self);
	self->ConstructL(aFs,aFileName);
	return self;
	}

CLog* CLog::NewL(RFs &aFs, const TDesC &aFileName)
	{
	CLog* self=CLog::NewLC(aFs, aFileName);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLog::ConstructL(RFs &aFs, const TDesC &aFileName)
	{
	User::LeaveIfError(iFile.Replace(aFs,aFileName,EFileWrite|EFileShareAny));
	
	iText.Set(iFile);
	iBuf.Create(1024);
	
	}
//write log
//input: aInfo, text msg;
//return: KErrNone, ok; other, fail;
TInt CLog::WriteLog(const TDesC& aInfo)
	{
	TTime currentTime;
	currentTime.HomeTime();	
	TDateTime time=currentTime.DateTime();	
	
	iBuf.Format(KTimeFormat,time.Minute(),time.Second(),time.MicroSecond());
	iBuf.Append(aInfo);
	
	iFile.Write(iBuf);
	iFile.Write(KLimit);
		
	return 0;
	}

//write log
//input: aInfo, text msg;
//return: KErrNone, ok; other, fail;
TInt CLog::WriteLog(const TDesC8& aInfo)
	{
	TTime currentTime;
	currentTime.HomeTime();	
	TDateTime time=currentTime.DateTime();	
	
	iBuf.Format(KTimeFormat,time.Minute(),time.Second(),time.MicroSecond());
	iBuf.Append(aInfo);
	
	iFile.Write(iBuf);
	iFile.Write(KLimit);
		
	return 0;
	}
