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


#if (!defined __T_GRAPHICS_WSERV_SESSION_H__)
#define __T_GRAPHICS_WSERV_SESSION_H__

//	User Includes
#include "DataWrapperBase.h"
#include "T_DataWsClientClass.h"

/**
 * Test Active Notification class
 *
 */
class CT_DataWsSession : public CDataWrapperBase, public CT_DataWsClientClass
	{
public:
	static CT_DataWsSession*    NewL();

	~CT_DataWsSession();

	virtual TAny*	GetObject()	{ return iWsSession; }

    virtual void    SetObjectL(TAny* aAny);

	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataWsSession();

	void	ConstructL();

	void	RunL(CActive* aActive, TInt aIndex);
	void	DoCancel(CActive* aActive, TInt aIndex);

	virtual MWsClientClass* GetClientClass() const;

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnew();
	void	DoCmdConnectL(const TDesC& aSection);
	void	DoCmdClose();
	void	DoCmdVersion(const TDesC& aSection);
	void	DoCmdSetHotKey(const TDesC& aSection);
	void	DoCmdClearHotKeys(const TDesC& aSection);
	void	DoCmdRestoreDefaultHotKey(const TDesC& aSection);
	void	DoCmdEventReady(const TDesC& aSection, TInt aAsyncErrorIndex);
	void	DoCmdEventReadyCancel();
	void	DoCmdGetEventL(const TDesC& aSection);
	void	DoCmdPurgePointerEvents();
	void	DoCmdRedrawReady(const TDesC& aSection, TInt aAsyncErrorIndex);
	void	DoCmdRedrawReadyCancel();
	void	DoCmdGetRedrawL(const TDesC& aSection);
	void	DoCmdPriorityKeyReady(const TDesC& aSection, TInt aAsyncErrorIndex);
	void	DoCmdPriorityKeyReadyCancel();
	void	DoCmdGetPriorityKeyL(const TDesC& aSection);
	void	DoCmdFlush();
	void	DoCmdSetAutoFlush(const TDesC& aSection);
	void	DoCmdSetKeyboardRepeatRate(const TDesC& aSection);
	void	DoCmdGetKeyboardRepeatRate(const TDesC& aSection);
	void	DoCmdGetDoubleClickSettings(const TDesC& aSection);
	void	DoCmdSetDoubleClick(const TDesC& aSection);
	void	DoCmdNumWindowGroups(const TDesC& aSection);
	void	DoCmdWindowGroupListL(const TDesC& aSection);
	void	DoCmdGetFocusWindowGroup(const TDesC& aSection);
	void	DoCmdGetDefaultOwningWindow(const TDesC& aSection);
	void	DoCmdSetWindowGroupOrdinalPosition(const TDesC& aSection);
	void	DoCmdGetWindowGroupClientThreadId(const TDesC& aSection);
	void	DoCmdGetWindowGroupHandle(const TDesC& aSection);
	void	DoCmdGetWindowGroupOrdinalPriority(const TDesC& aSection);
	void	DoCmdGetWindowGroupNameFromIdentifier(const TDesC& aSection);
	void	DoCmdFindWindowGroupIdentifier(const TDesC& aSection);
	void	DoCmdFetchMessage(const TDesC& aSection);
	void	DoCmdSetShadowVector(const TDesC& aSection);
	void	DoCmdShadowVector(const TDesC& aSection);
	void	DoCmdSetBackgroundColor(const TDesC& aSection);
	void	DoCmdGetBackgroundColor(const TDesC& aSection);
	void	DoCmdSetSystemPointerCursor(const TDesC& aSection);
	void	DoCmdClearSystemPointerCursor(const TDesC& aSection);
	void	DoCmdClaimSystemPointerCursorList();
	void	DoCmdFreeSystemPointerCursorList();
	void	DoCmdSetCustomTextCursorL(const TDesC& aSection);
	void	DoCmdResourceCount(const TDesC& aSection);
	void	DoCmdPasswordEntered();
	void	DoCmdComputeMode(const TDesC& aSection);
	void	DoCmdHeapCount(const TDesC& aSection);
	void	DoCmdSetModifierState(const TDesC& aSection);
	void	DoCmdGetModifierState(const TDesC& aSection);
	void	DoCmdRequestOffEventsL(const TDesC& aSection);
	void	DoCmdGetDefModeMaxNumColors(const TDesC& aSection);
	void	DoCmdGetColorModeListL(const TDesC& aSection);
	void	DoCmdSetPointerCursorArea(const TDesC& aSection);
	void	DoCmdPointerCursorArea(const TDesC& aSection);
	void	DoCmdSetPointerCursorMode(const TDesC& aSection);
	void	DoCmdSetClientCursorMode(const TDesC& aSection);
	void	DoCmdPointerCursorMode(const TDesC& aSection);
	void	DoCmdSetDefaultSystemPointerCursor(const TDesC& aSection);
	void	DoCmdClearDefaultSystemPointerCursor();
	void	DoCmdSetPointerCursorPosition(const TDesC& aSection);
	void	DoCmdPointerCursorPosition(const TDesC& aSection);
	void	DoCmdSetDefaultFadingParameters(const TDesC& aSection);
	void	DoCmdPrepareForSwitchOff();
	void	DoCmdSetBufferSizeL(const TDesC& aSection);
	void	DoCmdSetMaxBufferSizeL(const TDesC& aSection);
	void	DoCmdSetSystemFaded(const TDesC& aSection);
	void	DoCmdSetFocusScreen(const TDesC& aSection);
	void	DoCmdGetFocusScreen(const TDesC& aSection);
	void	DoCmdClearAllRedrawStores();
	void	DoCmdLogCommand(const TDesC& aSection);
	void	DoCmdLogMessage(const TDesC& aSection);
	void	DoCmdNumberOfScreens(const TDesC& aSection);
	void	DoCmdSimulateRawEvent(const TDesC& aSection);
	void	DoCmdSimulateKeyEvent(const TDesC& aSection);

	//	CActiveCallback support
	void	RunEventReady(const TInt aIndex);
	void	DoCancelEventReady(const TInt aIndex);

	void	RunPriorityKeyReady(const TInt aIndex);
	void	DoCancelPriorityKeyReady(const TInt aIndex);

	void	RunRedrawReady(const TInt aIndex);
	void	DoCancelRedrawReady(const TInt aIndex);

private:
	RWsSession*					iWsSession;
	/** vaiable that temporarily stores value of server resource count that is set
	* by iSession->ResourceCount()
	*/
	TInt						iResourceCount;
	TInt						iNumWinGroup;
	TInt						iColorModeCount;
	TInt						iWindowGroupIdentifier;

	/** Active object for EventReady async operation */
	CActiveCallback*			iActiveEventReady;

	/** Active object for PriorityKeyReady async operation */
	CActiveCallback*			iActivePriorityKeyReady;

	/** Active object for RedrawReady async operation */
	CActiveCallback*			iActiveRedrawReady;

	/** Keyboard repeat rate data */
	TTimeIntervalMicroSeconds32	iKeyboardRepeateRateInitialTime;
	TTimeIntervalMicroSeconds32	iKeyboardRepeateRateTime;
	};

#endif /* __T_GRAPHICS_WSERV_SESSION_H__ */
