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


#if (!defined __T_GRAPHICS_WSERV_ANIM_FOR_MCLASS_H__)
#define __T_GRAPHICS_WSERV_ANIM_FOR_MCLASS_H__

#include "T_RAnimChild.h"
#include "DataWrapperBase.h"

#define KIpcArgNum	3

class CT_DataAnimForMClass : public CDataWrapperBase
	{
public:
	static CT_DataAnimForMClass*	NewL();

	~CT_DataAnimForMClass();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection,const TInt);
	//void RunL(CActive* aActive, TInt aIndex); 

protected:
	CT_DataAnimForMClass();
	void  ConstructL();

private:
	void	DestroyData();
	
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdConstructL(const TDesC& aSection);
	void	DoCmdCommandReplyL(const TDesC& aSection);
	void	DoCmdCommand(const TDesC& aSection);
	void	DoCmdAsyncCommandReply(const TDesC& aSection, const TInt aAsyncErrorIndex);
	void	DoCmdClose();
	void	DoCmdDestroy();
	void	DoCmdDestructor();
	
	TDesC8& CopyToSendBuf(const TDesC& buf);
	
	TBool GetIpcArg(const TDesC& aSection);

	TBool CheckIpcArg(const TDesC& aSection);
	TBool CheckAnimateInterval(const TDesC& aSection);
	void ResetIpcArg();
	//MAnimGeneralFunctions
	void DoCmdRetrieveResult(const TDesC& aSectName , const TInt& datOpcode);
	
	void DoCmdGeneralAnimate(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralClient(const TDesC& aSectName , const TInt& datOpcode );
	void DoCmdGeneralFlashStateOn(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralPanic(const TDesC& aSectName, const TInt& datOpcode);
	void DoCmdGeneralScreenDevice(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralWindowExtension(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralEventExtension(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralExtendedInterface(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralNumOfExtInterfaces(const TDesC& aSectName , const TInt& datOpcode);
    void DoCmdGeneralGetRawEvents(const TDesC& aSectName , const TInt& datOpcode);
    void DoCmdGeneralPostRawEvent(const TDesC& aSectName ,  const TInt& datOpcode);
    void DoCmdGeneralPostKeyEventL(const TDesC& aSectName ,  const TInt& datOpcode);
	void DoCmdGeneralSetSync(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralSync(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralSetInterval(const TDesC& aSectName, const TInt& datOpcode);
    void DoCmdGeneralSetNextInterval(const TDesC& aSectName, const TInt& datOpcode);
	void DoCmdGeneralSystemTime(const TDesC& aSectName, const TInt& datOpcode);
	void DoCmdGeneralRegisterForNotis(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralMessage(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralDuplicateBitmapL(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralDuplicateFontL(const TDesC& aSectName, const TInt& datOpcode);
	void DoCmdGeneralCloseFontL(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdGeneralReplyBuf8(const TDesC& aSectName, const TInt& datOpcode);
	void DoCmdGeneralReplyBuf16(const TDesC& aSectName, const TInt& datOpcode);
	
	void DoCmdSpriteGetSpriteMember(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdSpriteUpdateMember(const TDesC& aSectName, const TInt& datOpcode);
	void DoCmdSpriteActivate(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdSpriteSpriteCanBeSeen(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdSpriteSizeChangedL(const TDesC& aSectName , const TInt& datOpcode);
	void DoCmdSpriteSetPosition(const TDesC& aSectName , const TInt& datOpcode);

    //MAnimWindowFunctions
    void DoWindowActivateGc(TInt aOpcode);
    void DoWindowSetRect(TInt aOpcode,const TDesC& aSection);
    void DoWindowWindowSizeL(TInt aOpcode,const TDesC& aSection);
    void DoWindowIsHidden(TInt aOpcode,const TDesC& aSection);
    void DoWindowSetVisible(TInt aOpcode,const TDesC& aSection);
    void DoWindowInvalidate(TInt aOpcode,const TDesC& aSection);
    void DoWindowParametersL(TInt aOpcode,const TDesC& aSection);
    void DoWindowVisibleRegion(TInt aOpcode,const TDesC& aSection);
    void GetWindowRetrieveResult(TInt aOpcode,const TDesC& aSection);
    
    //MAnimGeneralFunctionsWindowExtension
    void DoGeneralExtScreensL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtFocusScreensL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtSetFocusScreen(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtWindowGroupsL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtWindowGroupInfoL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtWindowGroupNameL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtSetOrdinalPositionL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtIsFocusableL(TInt aOpcode,const TDesC& aSection);
    void DoGeneralExtWindowConfigL(TInt aOpcode,const TDesC& aSection);
    
    //MAnimFreeTimerWindowFunctions
    void DoFreeTimerWinDeactivateGc(TInt aOpcode);
    void DoFreeTimerWinUpdate(TInt aOpcode);
    
    //Common Util
    void DoCmdEatupMemory(const TInt& datOpcode);
    void DoCmdFreeEatenMemory(const TInt& datOpcode);
private:
	CActiveCallback* iActiveCallback;
	T_RAnimChild*	iAnim;
	
	TIpcArgs iIpcArgs;
	
	RBuf8 iSendBuf;
	RBuf iIpcBuf[KIpcArgNum];		
	
	TBuf<200> iAsyncCmdSection;
	};


#endif /* __T_GRAPHICS_WSERV_TESTANIM_H__ */
