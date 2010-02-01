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


#ifndef T_DATADIRECTSCREENACCESS_H_
#define T_DATADIRECTSCREENACCESS_H_

#include <w32std.h>
#include "DataWrapperActive.h"


class CT_DataDirectScreenAccess : public CDataWrapperActive 
	, public MDirectScreenAccess
{
public:
	static CT_DataDirectScreenAccess* NewL();
	~CT_DataDirectScreenAccess();
	virtual TAny* GetObject() {return iDirectScreenAccess; }
	virtual void SetObjectL(TAny* aAny);
	virtual void DisownObjectL();
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const
			TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	//implement the CDataWrapperActive
	virtual	CActive*	GetActive() {return iDirectScreenAccess; }

protected:
	CT_DataDirectScreenAccess();
	void ConstructL();

private: //functions to be tested
	void DoCmdnewL(const TDesC& aSection);
	void DoCmdStartL(const TDesC& aSection);
	void DoCmdGcL(const TDesC& aSection);
	void DoCmdScreenDeviceL(const TDesC& aSection);
	void DoCmdDrawRegion(const TDesC& aSection);
	void DoCmdSetPriority(const TDesC& aSection);
	void DoCmdBringAppFgL(const TDesC& aSection);
	void DoCmdDestructor();
	
private:	
	//implement the MDirectScreenAccess
	void Restart(RDirectScreenAccess::TTerminationReasons aReason);
	void AbortNow(RDirectScreenAccess::TTerminationReasons aReason);
	
	//Helper function
	void DestroyData();
	RWsSession* GetRWSessionL(const TDesC& aSection);
	CWsScreenDevice* GetDevicesL(const TDesC& aSection);
	RWindow* GetWindowL(const TDesC& aSection);
	
private:
	//Wrapped Object
	CDirectScreenAccess* iDirectScreenAccess;
	RDirectScreenAccess::TTerminationReasons iWantedReason;
	

	
};



#endif /*T_DATADIRECTSCREENACCESS_H_*/
