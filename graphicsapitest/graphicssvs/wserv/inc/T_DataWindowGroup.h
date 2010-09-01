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


#if (!defined __T_GRAPHICS_WSERV_WINDOWGROUP_H__)
#define __T_GRAPHICS_WSERV_WINDOWGROUP_H__

// User Includes
#include "T_DataWindowTreeNode.h"


/**
 * Test Active Notification class
 *
 */
class CT_DataWindowGroup : public CT_DataWindowTreeNode
	{
public:
	/**
	* Two phase constructor
	*/
	static CT_DataWindowGroup*    NewL();

	/**
	* Public destructor
	*/
	~CT_DataWindowGroup();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*    GetObject()    { return iWindowGroup; }

	/**
	* Set the object that the data wraps
	*
	* @param    aObject object that the wrapper is testing
	*
	*/
	virtual void    SetObjectL(TAny* aAny);

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
	CT_DataWindowGroup();


	/**
	* Second phase construction
	*/
	void  ConstructL();

	virtual RWindowTreeNode*	GetWindowTreeNode() const;


private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdDestructor();
	void	DoCmdConstructL(const TDesC& aSection);
	void	DoCmdConstructChildAppL(const TDesC& aSection);
	void	DoCmdAllowProcessToCreateChildWindowGroups(const TDesC& aSection);
	void	DoCmdEnableReceiptOfFocus(const TDesC& aSection);
	void	DoCmdAutoForeground(const TDesC& aSection);
	void	DoCmdSetOrdinalPriorityAdjust(const TDesC& aSection);
	void	DoCmdSetOrdinalPositionErr(const TDesC& aSection);
	void	DoCmdCaptureKey(const TDesC& aSection);
	void	DoCmdCancelCaptureKey(const TDesC& aSection);
	void	DoCmdCaptureKeyUpAndDowns(const TDesC& aSection);
	void	DoCmdCancelCaptureKeyUpAndDowns(const TDesC& aSection);
	void	DoCmdCaptureLongKey(const TDesC& aSection);
	void	DoCmdCancelCaptureLongKey(const TDesC& aSection);
	void	DoCmdAddPriorityKey(const TDesC& aSection);
	void	DoCmdRemovePriorityKey(const TDesC& aSection);
	void	DoCmdSetTextCursorL(const TDesC& aSection);
	void	DoCmdCancelTextCursor();
	void	DoCmdSetOwningWindowGroupL(const TDesC& aSection);
	void	DoCmdDefaultOwningWindow();
	void	DoCmdSetName(const TDesC& aSection);
	void	DoCmdName(const TDesC& aSection);
	void	DoCmdIdentifier();
	void	DoCmdDisableKeyClick(const TDesC& aSection);
	void	DoCmdEnableScreenChangeEvents();
	void	DoCmdDisableScreenChangeEvents();
	void	DoCmdSimulatePointerEvent(const TDesC& aSection);
	void	DoCmdSetChildGroupL(const TDesC& aSection);
	void	DoCmdClearChildGroup();

private:
	RWindowGroup*	iWindowGroup;
	TInt32			iCaptureKeyHandle;
	};

#endif /* __T_GRAPHICS_WSERV_WINDOWGROUP_H__ */
