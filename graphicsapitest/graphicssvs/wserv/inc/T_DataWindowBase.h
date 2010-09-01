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


#if (!defined __T_GRAPHICS_WSERV_WINDOWBASE_H__)
#define __T_GRAPHICS_WSERV_WINDOWBASE_H__

// User Includes
#include "T_DataWindowTreeNode.h"

/**
 * Test Active Notification class
 *
 */
class CT_DataWindowBase : public CT_DataWindowTreeNode
	{
public:
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataWindowBase();

	virtual RWindowBase*		GetWindowBase() const=0;
	virtual RWindowTreeNode*	GetWindowTreeNode() const;

private:
	/**
	* Helper methods
	*/
	void	DoCmdActivate();
	void	DoCmdSetPosition(const TDesC& aSection);
	void	DoCmdSetSizeErr(const TDesC& aSection);
	void	DoCmdSetExtentErr(const TDesC& aSection);
	void	DoCmdSize(const TDesC& aSection);
	void	DoCmdInquireOffsetL(const TDesC& aSection);
	void	DoCmdPointerFilter(const TDesC& aSection);
	void	DoCmdSetPointerGrab(const TDesC& aSection);
	void	DoCmdClaimPointerGrab(const TDesC& aSection);
	void	DoCmdSetPointerCapture(const TDesC& aSection);
	void	DoCmdSetPointerCapturePriority(const TDesC& aSection);
	void	DoCmdGetPointerCapturePriority(const TDesC& aSection);
	void	DoCmdSetVisible(const TDesC& aSection);
	void	DoCmdSetShadowHeight(const TDesC& aSection);
	void	DoCmdSetShadowDisabled(const TDesC& aSection);
	void	DoCmdPosition(const TDesC& aSection);
	void	DoCmdAbsPosition(const TDesC& aSection);
	void	DoCmdSetCornerType(const TDesC& aSection);
	void	DoCmdSetShapeL(const TDesC& aSection);
	void	DoCmdSetRequiredDisplayMode(const TDesC& aSection);
	void	DoCmdDisplayMode(const TDesC& aSection);
	void	DoCmdEnableBackup(const TDesC& aSection);
	void	DoCmdRequestPointerRepeatEvent(const TDesC& aSection);
	void	DoCmdCancelPointerRepeatEventRequest();
	void	DoCmdAllocPointerMoveBuffer(const TDesC& aSection);
	void	DoCmdFreePointerMoveBuffer();
	void	DoCmdEnablePointerMoveBuffer();
	void	DoCmdDisablePointerMoveBuffer();
	void	DoCmdRetrievePointerMoveBufferL(const TDesC& aSection);
	void	DoCmdDiscardPointerMoveBuffer();
	void	DoCmdAddKeyRect(const TDesC& aSection);
	void	DoCmdRemoveAllKeyRects();
	void	DoCmdPasswordWindow(const TDesC& aSection);
	void	DoCmdFadeBehind(const TDesC& aSection);
	void	DoCmdIsFaded(const TDesC& aSection);
	void	DoCmdIsNonFading(const TDesC& aSection);
	void	DoCmdMoveToGroupL(const TDesC& aSection);
	};

#endif /* __T_GRAPHICS_WSERV_WINDOWBASE_H__ */
