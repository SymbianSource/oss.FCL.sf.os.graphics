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


#if (!defined __T_GRAPHICS_WSERV_PRIORITYKEYEVENT_H__)
#define __T_GRAPHICS_WSERV_PRIORITYKEYEVENT_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC Includes
#include <w32std.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataWsPriorityKeyEvent : public CDataWrapperBase
	{
public:
	/**
	* Two phase constructor
	*/
	static CT_DataWsPriorityKeyEvent*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataWsPriorityKeyEvent();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iWsPriorityKeyEvent; }

	/**
	* Set the object that the data wraps
	*
	* @param    aObject object that the wrapper is testing
	*
	*/
	virtual void	SetObjectL(TAny* aAny);

	/**
	* The object will no longer be owned by this
	*
	* @leave	KErrNotSupported if the the function is not supported
	*/
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataWsPriorityKeyEvent();

	/**
	* Second phase construction
	*/
	void	ConstructL();

private:
	/**
	* Helper methods
	*/
	void	DestroyData();

	void	DoCmdnew();
	void	DoCmdHandle(const TDesC& aEntry);
	void	DoCmdKey(const TDesC& aEntry);
	void	DoCmdSetHandle(const TDesC& aEntry);

private:
    TWsPriorityKeyEvent*	iWsPriorityKeyEvent;
    };

#endif /* __T_GRAPHICS_WSERV_PRIORITYKEYEVENT_H__ */
