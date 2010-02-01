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


#if (!defined __T_GRAPHICS_WSERV_EVENT_H__)
#define __T_GRAPHICS_WSERV_EVENT_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC Includes
#include <w32std.h>

class CT_DataWsEvent : public CDataWrapperBase
	{
public:
	static CT_DataWsEvent*	NewL();

	~CT_DataWsEvent();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataWsEvent();
	void	ConstructL();

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnew();
	void	DoCmdErrorMessage(const TDesC& aSection);
	void	DoCmdEventData(const TDesC& aSection);
	void	DoCmdHandle(const TDesC& aSection);
	void	DoCmdInt(const TDesC& aSection);
	void	DoCmdKey(const TDesC& aSection);
	void	DoCmdModifiersChanged(const TDesC& aSection);
	void	DoCmdPointer(const TDesC& aSection);
	void	DoCmdSetHandle(const TDesC& aSection);
	void	DoCmdSetTimeNow();
	void	DoCmdSetType(const TDesC& aSection);
	void	DoCmdTime();
	void	DoCmdType(const TDesC& aSection);
	void	DoCmdVisibilityChanged(const TDesC& aSection);

private:
	TWsEvent*			iWsEvent;
    };

#endif /* __T_GRAPHICS_WSERV_EVENT_H__ */
