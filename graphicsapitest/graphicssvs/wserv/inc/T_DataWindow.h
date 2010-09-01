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


#if (!defined __T_GRAPHICS_WSERV_WINDOW_H__)
#define __T_GRAPHICS_WSERV_WINDOW_H__

// User Includes
#include "T_DataDrawableWindow.h"


/**
 * Test Active Notification class
 *
 */
class CT_DataWindow : public CT_DataDrawableWindow
	{
public:
	/**
	* Two phase constructor
	*/
	static CT_DataWindow*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataWindow();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iWindow; }

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
	CT_DataWindow();


	/**
	* Second phase construction
	*/
	void  ConstructL();

	virtual RDrawableWindow*	GetDrawableWindow() const;

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdConstructL(const TDesC& aSection);
	void	DoCmdBeginRedraw(const TDesC& aSection);
	void	DoCmdEndRedraw();
	void	DoCmdInvalidate(const TDesC& aSection);
	void	DoCmdGetInvalidRegionL(const TDesC& aSection);
	void	DoCmdSetBackgroundColor(const TDesC& aSection);
	void	DoCmdSetSize(const TDesC& aSection);
	void	DoCmdSetExtent(const TDesC& aSection);
	void	DoCmdHandleTransparencyUpdate();
	void	DoCmdSetTransparencyFactor(const TDesC& aSection);
	void	DoCmdSetTransparencyBitmapL(const TDesC& aSection);
	void	DoCmdSetTransparencyWsBitmapL(const TDesC& aSection);
	void	DoCmdSetNonTransparent();
	void	DoCmdEnableRedrawStore(const TDesC& aSection);
	void	DoCmdSetTransparencyAlphaChannel();
	void	DoCmdSetTransparentRegionL(const TDesC& aSection);
	void	DoCmdSetTransparencyPolicy(const TDesC& aSection);
	void	DoCmdIsRedrawStoreEnabled(const TDesC& aSection);
	void	DoCmdEnableOSB(const TDesC& aSection);

private:
	RWindow*	iWindow;
	};
#endif /* __T_GRAPHICS_WSERV_WINDOW_H__ */
