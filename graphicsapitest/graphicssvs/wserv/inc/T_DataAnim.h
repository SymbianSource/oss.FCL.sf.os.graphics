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


#if (!defined __T_GRAPHICS_WSERV_TESTANIM_H__)
#define __T_GRAPHICS_WSERV_TESTANIM_H__

#include "T_RAnimChild.h"
#include "DataWrapperBase.h"

#define KIpcArgNum	3

class CT_DataAnim : public CDataWrapperBase
	{
public:
	static CT_DataAnim*	NewL();

	~CT_DataAnim();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	void RunL(CActive* aActive, TInt aIndex); 

protected:
	CT_DataAnim();
	void  ConstructL();

private:
	void	DestroyData();
	
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdConstructL(const TDesC& aSection);
	void	DoCmdCommandReply(const TDesC& aSection);
	void	DoCmdCommand(const TDesC& aSection);
	void	DoCmdAsyncCommandReply(const TDesC& aSection, const TInt aAsyncErrorIndex);
	void	DoCmdClose();
	void	DoCmdDestroy();
	void	DoCmdDestructor();
	
	TDesC8& CopyToSendBuf(const TDesC& buf);
	
	TBool GetIpcArg(const TDesC& aSection);
	
	TBool CheckIpcArg(const TDesC& aSection);
	TBool CheckAnimateInterval(const TDesC& aSection);
	
    TBool 	ReadAnimCommand(const TDesC& aSectName, const TDesC& aKeyName, TInt& aCommand);
    TBool 	ReadAnimType(const TDesC& aSectName, const TDesC& aKeyName, TInt& aType);
private:
	CActiveCallback* iActiveCallback;
	T_RAnimChild*	iAnim;
	
	TIpcArgs iIpcArgs;
	
	RBuf8 iSendBuf;
	RBuf iIpcBuf[KIpcArgNum];		
	
	TBuf<200> iAsyncCmdSection;
	};


#endif /* __T_GRAPHICS_WSERV_TESTANIM_H__ */
