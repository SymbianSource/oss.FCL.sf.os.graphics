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


#if (!defined __T_GRAPHICS_WSERV_CLIENTCLASS_H__)
#define __T_GRAPHICS_WSERV_CLIENTCLASS_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC Includes
#include <w32std.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataWsClientClass
	{
public:
	virtual TBool	DoCommandL(CDataWrapperBase& aDataWrapper, const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	virtual MWsClientClass*	GetClientClass() const=0;

private:
	/**
	* Helper methods
	*/
	void	DoCmdWsHandle(CDataWrapperBase& aDataWrapper, const TDesC& aSection);
	};

#endif /* __T_GRAPHICS_WSERV_CLIENTCLASS_H__ */
