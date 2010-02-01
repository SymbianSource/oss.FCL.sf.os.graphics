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


#ifndef T_DATADSASESSION_H_
#define T_DATADSASESSION_H_

#include <w32std.h>
#include "DataWrapperBase.h"
#include "T_DataWsClientClass.h"

class CT_DataDsaSession : public CDataWrapperBase, public CT_DataWsClientClass
{
public:
	static CT_DataDsaSession* NewL();
	~CT_DataDsaSession();
	virtual TAny* GetObject() {return iDsaSession;}
	virtual void SetObjectL(TAny* aAny);
	virtual void DisownObjectL();
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const
			TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataDsaSession();
	void ConstructL();
	void RunL(CActive* aActive, TInt aIndex);
	void DoCancel(CActive* aActive, TInt aIndex);
	virtual MWsClientClass* GetClientClass() const;
	
private://functions used to test.
	void DoCmdnewL(const TDesC& aSection);
	void DoCmdConstruct();
	void DoCmdClose();
	void DoCmdRequestL(const TDesC& aSection, const TInt aAsyncErrorIndex);
	void DoCmdCompleted();
	void DoCmdCancel();
	
	void DoCmdEatupMemory(const TDesC& aSection);
	void DoCmdFreeEatenMemory();
private:
	//helper functions
	void DestroyData();
	//Helper class
	RWsSession* GetRWSessionL(const TDesC& aSection);
	
	//used in ActiveCallback
	void RunRequestReady( const TInt aIndex);

private:
	RDirectScreenAccess* iDsaSession;
	//ActiveCallback objects
	CActiveCallback* iActiveRequestReady;
};


#endif /*T_DATADSASESSION_H_*/
