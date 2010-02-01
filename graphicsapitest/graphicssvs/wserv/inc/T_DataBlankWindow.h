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


#if (!defined __T_GRAPHICS_WSERV_BLANKWINDOW_H__)
#define __T_GRAPHICS_WSERV_BLANKWINDOW_H__

// User Includes
#include "T_DataWindowBase.h"

/**
 * Test Active Notification class
 *
 */
class CT_DataBlankWindow : public CT_DataWindowBase
	{
public:
	/**
	* Two phase constructor
	*/
	static CT_DataBlankWindow*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataBlankWindow();

	/**
	* Return a pointer to the object that the data wraps
	*
	* @return	pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iBlankWindow; }

	/**
	* Set the object that the data wraps
	*
	* @param	aObject object that the wrapper is testing
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
	CT_DataBlankWindow();

	/**
	* Second phase construction
	*/
	void	ConstructL();

	virtual RWindowBase*	GetWindowBase() const;

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdConstructL(const TDesC& aSection);
	void	DoCmdSetColor(const TDesC& aSection);
	void	DoCmdSetSize(const TDesC& aSection);
	void	DoCmdSetExtent(const TDesC& aSection);

private:
	RBlankWindow*	iBlankWindow;
	};

#endif /* __T_GRAPHICS_WSERV_BLANKWINDOW_H__ */
