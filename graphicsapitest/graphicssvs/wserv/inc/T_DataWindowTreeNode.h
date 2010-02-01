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


#if (!defined __T_GRAPHICS_WSERV_WINDOWTREENODE_H__)
#define __T_GRAPHICS_WSERV_WINDOWTREENODE_H__

// User Includes
#include "T_DataWsClientClass.h"
#include "DataWrapperBase.h"
#include "T_GraphicsUtil.h"

// EPOC includes
#include <w32std.h>
#include <e32keys.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataWindowTreeNode : public CDataWrapperBase, public CT_DataWsClientClass
	{
public:
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataWindowTreeNode();

	virtual MWsClientClass*		GetClientClass() const;
	virtual RWindowTreeNode*	GetWindowTreeNode() const=0;

private:
	/**
	* Helper methods
	*/
	void	DoCmdClose();
	void	DoCmdDestroyL();
	void	DoCmdParent(const TDesC& aSection);
	void	DoCmdPrevSibling(const TDesC& aSection);
	void	DoCmdNextSibling(const TDesC& aSection);
	void	DoCmdChild(const TDesC& aSection);
	void	DoCmdOrdinalPriority(const TDesC& aSection);
	void	DoCmdOrdinalPosition(const TDesC& aSection);
	void	DoCmdFullOrdinalPosition(const TDesC& aSection);
	void	DoCmdSetOrdinalPosition(const TDesC& aSection);
	void	DoCmdWindowGroupId();
	void	DoCmdSetPointerCursor(const TDesC& aSection);
	void	DoCmdSetCustomPointerCursorL(const TDesC& aSection);
	void	DoCmdEnableOnEvents(const TDesC& aSection);
	void	DoCmdDisableOnEvents();
	void	DoCmdEnableGroupChangeEvents();
	void	DoCmdDisableGroupChangeEvents();
	void	DoCmdEnableFocusChangeEvents();
	void	DoCmdDisableFocusChangeEvents();
	void	DoCmdEnableGroupListChangeEvents();
	void	DoCmdDisableGroupListChangeEvents();
	void	DoCmdEnableVisibilityChangeEvents();
	void	DoCmdDisableVisibilityChangeEvents();
	void	DoCmdEnableErrorMessages(const TDesC& aSection);
	void	DoCmdDisableErrorMessages();
	void	DoCmdEnableModifierChangedEvents(const TDesC& aSection);
	void	DoCmdDisableModifierChangedEvents();
	void	DoCmdSetNonFading(const TDesC& aSection);
	void	DoCmdSetFaded(const TDesC& aSection);
	void	DoCmdClearPointerCursor();
	};

#endif /* __T_GRAPHICS_WSERV_WINDOWTREENODE_H__ */
