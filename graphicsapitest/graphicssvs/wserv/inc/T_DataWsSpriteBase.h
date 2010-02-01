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


#if (!defined __T_GRAPHICS_WSERV_WSSPRITEBASE_H__)
#define __T_GRAPHICS_WSERV_WSSPRITEBASE_H__

// User Includes
#include "DataWrapperBase.h"
#include "T_DataWsClientClass.h"
/**
 * Test Active Notification class
 *
 */
class CT_DataWsSpriteBase : public CDataWrapperBase, public CT_DataWsClientClass
	{
public:
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataWsSpriteBase();

	virtual MWsClientClass*		GetClientClass() const;
	virtual RWsSpriteBase*		GetWsSpriteBase() const=0;
private:
	/**
	* Helper methods
	*/
	void	DoCmdActivate();
	void	DoCmdAppendMemberL(const TDesC& aSection);
	void    DoCmdUpdateMemberL(const TDesC& aSection);
	void    DoCmdClose();

    
	};

#endif /* __T_GRAPHICS_WSERV_WSSPRITEBASE_H__ */
