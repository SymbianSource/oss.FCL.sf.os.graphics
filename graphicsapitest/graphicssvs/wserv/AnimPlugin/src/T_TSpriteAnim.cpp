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

#include "txtfrmat.h" 
#include "bitdev.h" 
#include "T_AnimProc.h"
#include "T_TSpriteAnim.h"
#include "T_TestAnimDll.h"
#include "w32std.h"
#include "T_AnimDef.h"

CTSpriteAnim::CTSpriteAnim(CTestAnimDll* aDll, TInt aType) :
	CAnimProc(aDll, aType)
	{
	// No implementation required

	}

CTSpriteAnim::~CTSpriteAnim()
	{
	WriteLog(_L("~CTSpriteAnim"));
	}

void CTSpriteAnim::ConstructL(TAny *aArgs)
	{
	WriteLog(_L("CTSpriteAnim::ConstructL"));
	ConstructWinL(aArgs, iFunctions);

	// screen size
	iFunctions->ScreenDevice()->GetDrawRect(iScreenSize);
	}
void CTSpriteAnim::Redraw()
	{
	WriteLog(_L("CTSpriteAnim::Redraw"));
	Draw();
	}

void CTSpriteAnim::Draw()
	{
	// signal that the current position has been drawn
	iDrawn = ETrue;
	}
void CTSpriteAnim::FocusChanged(TBool /*aState*/)
	{

	}
TInt CTSpriteAnim::CommandReplyL(TInt aOpcode, TAny *aArgs)
	{
	switch (aOpcode)
		{
		case ECmdSpriteGetSpriteMember:
			return DoCmdGetSpriteMemberL();
		case ECmdSpriteUpdateMember:
			return DoCmdUpdateMemberL();
		case ECmdSpriteActivate:
			return DoCmdActiveL();
		case ECmdSpriteSpriteCanBeSeen:
			return DoCmdSpriteCanBeSeen();
		case ECmdSpriteSizeChangedL:
			return DoCmdSizeChangeL();
		case ECmdSpriteSetPosition:
			return DoCmdSetPositionL();
		default:
			break;
		}
	return CmdReply(iFunctions, aOpcode, aArgs);
	}
void CTSpriteAnim::Command(TInt aOpcode, TAny *aArgs)
	{
	CmdReply(iFunctions, aOpcode, aArgs);
	}
void CTSpriteAnim::Animate(TDateTime* /*aDateTime*/)
	{
	iLastPoint = iNewPoint;

	TInt nextpoint=iNewPoint.iX + iStepX;
	TInt right=iNewPoint.iX + iSize.iWidth;

	if (right> iScreenSize.iBr.iX ||nextpoint >= iScreenSize.iBr.iX
			||iNewPoint.iX<iScreenSize.iTl.iX)
		iStepX = -iStepX;

	TInt nexty=iNewPoint.iY + iStepY;
	TInt bottom=nexty + iSize.iHeight;

	if (bottom> iScreenSize.iBr.iY ||nexty >= iScreenSize.iBr.iY ||iNewPoint.iY
			<iScreenSize.iTl.iY || nexty <= iScreenSize.iTl.iY)
		iStepY = -iStepY;

	iNewPoint.iX += iStepX;
	iNewPoint.iY += iStepY;

	iSpriteFunctions->SetPosition(iNewPoint);

	RecordAnimTime();

	TInt interval=(iAnimTime[KAnimTimeSize-1]-iAnimTime[KAnimTimeSize-2])/1000;

	TBuf<100> buf;
	buf.Format(_L("CTSpriteAnim::Animate interval=%d x=%d y=%d"),
	interval,iNewPoint.iX,iNewPoint.iY);

	WriteLog(buf);
	}

void CTSpriteAnim::Compare()
	{
	// compare     	
	const CFbsScreenDevice* scdev=iFunctions->ScreenDevice();

	TBool compare=scdev->RectCompare(TRect(iNewPoint, iSize), *scdev, TRect(
			TPoint(0, 0), iSize));
	TBuf<50> buf;
	buf.Format(_L("CTSpriteAnim::Compare res %d"),
	compare);
	WriteLog(buf);
	}
TBool CTSpriteAnim::OfferRawEvent(const TRawEvent& /*aRawEvent*/)
	{
	return ETrue;
	}

TInt CTSpriteAnim::DoCmdGetSpriteMemberL()
	{
	TInt result = 0;
	TInt aMember;
	TPtr8 ptr1((TUint8*)&aMember, sizeof(TInt));
	const RMessagePtr2 *msg=iFunctions->Message();
	
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		TSpriteMember* member = iSpriteFunctions->GetSpriteMember(aMember);
		TPoint point = member->iOffset;
		TPtr8 ptr2((TUint8*)&point, sizeof(TPoint), sizeof(TPoint));
		msg->Write(2, ptr2);
		
		}
	else
		{
		result = -1;
		}
	return result;
	
	}

TInt CTSpriteAnim::DoCmdActiveL()
	{
	TInt result = 0;
	TBool aState;
	TPtr8 ptr3((TUint8*)&aState, sizeof(TBool));
	const RMessagePtr2 *msg=iFunctions->Message();
		
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr3);
		iSpriteFunctions->Activate(aState);
		}
	else
		{
		result = -1;
		}
	return result;
	}

TInt CTSpriteAnim::DoCmdSetPositionL()
	{
	TInt result = 0;
	TPoint aPoint;
	TPtr8 ptr6((TUint8*)&aPoint, sizeof(TPoint));
	const RMessagePtr2 *msg=iFunctions->Message();
		
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr6);
		iSpriteFunctions->SetPosition(aPoint);
		}	
	else
		{
		result = -1;		
		}
	return result;
	}

TInt CTSpriteAnim::DoCmdSpriteCanBeSeen()
	{
	TBool visible = iSpriteFunctions->SpriteCanBeSeen();
	TPtr8 ptr((TUint8*)&visible, sizeof(TBool), sizeof(TBool));
	iFunctions->Message()->Write(1, ptr);
	return 0;
	}

TInt CTSpriteAnim::DoCmdUpdateMemberL()
	{
	//TInt aMember, const TRect &aRect, TBool aFullUpdate
	TInt result = 0;
	TInt aMember;
	TPtr8 ptr1((TUint8*)&aMember, sizeof(TInt));

	TRect aRect;
	TPtr8 ptr2((TUint8*)&aRect, sizeof(TRect));
	
	TBool aFullUpdate;
	TPtr8 ptr3((TUint8*)&aFullUpdate, sizeof(TInt));

	const RMessagePtr2 *msg=iFunctions->Message();
		
	if(msg && !msg->IsNull())
		{
		msg->ReadL(1,ptr1);
		msg->ReadL(2,ptr2);
		msg->ReadL(3,ptr3);
		iSpriteFunctions->UpdateMember(aMember, aRect, aFullUpdate);
		}	
	else
		{
		result = -1;
		}
	return result;
	}

TInt CTSpriteAnim::DoCmdSizeChangeL()
	{
	iSpriteFunctions->SizeChangedL();
	return 0;
	}

