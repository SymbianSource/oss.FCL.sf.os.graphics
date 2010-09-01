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


#if (!defined __T_GRAPHICS_WSERV_DRAWABLEWINDOW_H__)
#define __T_GRAPHICS_WSERV_DRAWABLEWINDOW_H__

// User Includes
#include "T_DataWindowBase.h"


/**
 * Test Active Notification class
 *
 */
class CT_DataDrawableWindow : public CT_DataWindowBase
	{
public:
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataDrawableWindow();

	virtual RDrawableWindow*	GetDrawableWindow() const=0;
	virtual RWindowBase*		GetWindowBase() const;

private:
	/**
	* Helper methods
	*/
	void	DoCmdScroll(const TDesC& aEntry);
	void	DoCmdGetDrawRect(const TDesC& aEntry);
	};

	#endif /* __T_GRAPHICS_WSERV_DRAWABLEWINDOW_H__ */
