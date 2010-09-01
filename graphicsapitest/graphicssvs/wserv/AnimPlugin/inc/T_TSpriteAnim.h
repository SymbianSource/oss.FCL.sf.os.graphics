// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
// 
// Description:
//

#ifndef SPRITEANIM_H
#define SPRITEANIM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "w32adll.h" 
#include "w32std.h" 




// CLASS DECLARATION

/**
*  CWinAnim
* 
*/

class CTestAnimDll;
class CAnimProc;

class CTSpriteAnim: public CSpriteAnim, CAnimProc
{
public: // Constructors and destructor

	/**
		* Destructor.
		*/
	~CTSpriteAnim();

	/**
		* Constructor for performing 1st stage construction
		*/
	CTSpriteAnim(CTestAnimDll* aDll, TInt aType);
	
	virtual void ConstructL(TAny *aArgs);
	virtual void Redraw();
	virtual void FocusChanged(TBool aState);
	virtual TInt CommandReplyL(TInt aOpcode, TAny *aArgs);
	virtual void Command(TInt aOpcode, TAny *aArgs);
	virtual void Animate(TDateTime *aDateTime);
	virtual TBool OfferRawEvent(const TRawEvent &aRawEvent);

private:
	void Draw();
	void Compare();
	
	TInt DoCmdGetSpriteMemberL();
	TInt DoCmdActiveL();
	TInt DoCmdSetPositionL();
	TInt DoCmdSpriteCanBeSeen();
	TInt DoCmdUpdateMemberL();
	TInt DoCmdSizeChangeL();
	
protected:	
   

};

#endif // WINANIM_H

