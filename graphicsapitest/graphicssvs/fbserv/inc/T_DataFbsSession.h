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

/**
@test
@internalComponent

This contains CT_DataFbsSession
*/

#if (!defined __T_DATA_FBS_SESSION_H__)
#define __T_DATA_FBS_SESSION_H__

//	User Includes
#include "DataWrapperBase.h"
//	EPOC includes
#include <e32std.h>
#include <fbs.h>

class CT_DataFbsSession : public CDataWrapperBase
	{
public:
	static CT_DataFbsSession*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	
	~CT_DataFbsSession();

	virtual TAny*	GetObject();

protected:
	CT_DataFbsSession();
	void	ConstructL();

private:
	inline void	DoCmdConstructor();
	inline void	DoCmdConnect(const TDesC& aSection);
	inline void	DoCmdDisconnect();
	inline void	DoCmdGetSession(const TDesC& aSection);
	inline void	DoCmdCallBack(const TDesC& aSection);
	inline void	DoCmdSetCallBack();
	inline void	DoCmdResetCallBack();
	inline void	DoCmdResourceCount(const TDesC& aSection);
	inline void	DoCmdSendCommandL(const TDesC& aSection);
	inline void	DoCmdVersion(const TDesC& aSection);
	inline void	DoCmdHeapBase();
	inline void	DoCmdSessionHandle();

	static TInt CallBackFunction(TAny* aPtr);
	TInt		CallBackFunction();

	TBool		GetFbsMessage(const TDesC& aSection, TInt& aMessage);

private:
	RFs				iFsUnconnected;

	RFbsSession*	iSession;

	/** vaiable that temporarily stores value of server resource count that is set
	* by iSession->ResourceCount()
	*/
	TInt			iResourceCount;

	/* used to check if the callback function was called*/
	TBool			iCallBackCalled;
	};

#endif /* __T_DATA_FBS_SESSION_H__ */
