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


#if (!defined __T_DATA_WS_GRAPHIC_MSG_FIXED_BASE_H__)
#define __T_DATA_WS_GRAPHIC_MSG_FIXED_BASE_H__

// User Includes
#include "DataWrapperBase.h"

/**
 * Test Active Notification class
 *
 */
class CT_DataWsGraphicMsgFixedBase : public CDataWrapperBase
	{
public:
	~CT_DataWsGraphicMsgFixedBase();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	TPtrC8			GetPckg();

protected:
	CT_DataWsGraphicMsgFixedBase();

	virtual TWsGraphicMsgFixedBase*	GetWsGraphicMsgFixedBase() const = 0;

private:
	/**
	* Helper methods
	*/
	void	DoCmdPckg();
	void	DoCmdSize(const TDesC& aSection);
	void	DoCmdTypeId(const TDesC& aSection);

private:
	TPtrC8	iPckg;
	};

#endif /* __T_DATA_WS_GRAPHIC_MSG_FIXED_BASE_H__ */
