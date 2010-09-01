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

#include "T_TestAnimDll.h"
#include "T_AnimDef.h"
#include "T_TWinAnim.h"
#include "T_TSpriteAnim.h"
#include "T_TFreeTimerWinAnim.h"


CTestAnimDll::CTestAnimDll()
{
	// No implementation required
}


CTestAnimDll::~CTestAnimDll()
{
	WriteLog(_L("~CTestAnimDll"));
	delete iLog;
	iFs.Close();
}

CTestAnimDll* CTestAnimDll::NewLC()
{
	CTestAnimDll* self = new (ELeave)CTestAnimDll();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CTestAnimDll* CTestAnimDll::NewL()
{
	CTestAnimDll* self=CTestAnimDll::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CTestAnimDll::ConstructL()
{
	User::LeaveIfError(iFs.Connect());
	_LIT(KFileName,"c:\\anim.txt");
	iLog=CLog::NewL(iFs,KFileName);

}

EXPORT_C CAnimDll* CreateCAnimDllL()
{	
	return CTestAnimDll::NewL();
}

TInt CTestAnimDll::WriteLog(const TDesC& aInfo)
	{
	return iLog->WriteLog((aInfo));	
	}
TInt CTestAnimDll::WriteLog(const TDesC8& aInfo)
	{
	return iLog->WriteLog((aInfo));	
	}

CAnim* CTestAnimDll::CreateInstanceL(TInt aType)
    {
	TBuf<50> buf;
	buf.Format(_L("CTestAnimDll::CreateInstanceL %d"),aType);
	WriteLog(buf);
	
	CAnim* anim=NULL;
	switch (aType)
	{
	case EKWindowAnim:
		{
		anim=new (ELeave)CTWinAnim(this,aType);		
		break;
		}
	case EKSpriteAnim:
		{
		anim=new (ELeave)CTSpriteAnim(this,aType);
		break;
		}	
	case EKFreeTimerWindowAnim:
		{
		anim=new (ELeave) CTFreeTimerWinAnim();
		break;
		}
	default:
		{
//		iFunctions->Panic();
		break; 
		}	  

	}

	return anim;
    }
