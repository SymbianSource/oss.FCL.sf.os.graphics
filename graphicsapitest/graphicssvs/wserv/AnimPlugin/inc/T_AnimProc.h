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

#ifndef ANIMPROC_H
#define ANIMPROC_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "T_TestAnimDll.h"
#include "badesca.h" 
#include "e32cmn.h" 

// CLASS DECLARATION
#define KAnimTimeSize 3



typedef RArray<TInt> RIntArray;

/**
*  CAnimProc
* 
*/
class CAnimProc 
{
public: // Constructors and destructor

	CAnimProc(CTestAnimDll* aDll,TInt aType);
	virtual ~CAnimProc();	

	TInt CmdReply(MAnimGeneralFunctions* pFunc, TInt aOpcode, TAny *aArgs);

	
	TInt ConstructWinL(TAny *aArgs,MAnimGeneralFunctions* pFunc);
	
	TInt WriteLog(const TDesC& aInfo);
	TInt WriteLog(const TDesC8& aInfo);
protected:

	CDesCArray* ProcParamLC(const TDesC8& param);
	void ConvertParam(TDes8& buf,TAny *aArgs);
	TInt GetIntFromStr(const TDesC& str,RIntArray& ary);
	
	void RecordAnimTime();
	
	void RecordLastCmd(TInt cmd);
	void RecordLastCmd(TInt cmd,const TDesC8& str);
	void RecordLastCmd(const TDesC8& str);
	
	TInt64 iAnimTime[KAnimTimeSize];  	// last time of Animate 

	
	TPoint iNewPoint;		// The square's current position
	TPoint iLastPoint;		// The square's last position	
	
	TInt iStepX;			// move step of x-coordinate
	TInt iStepY;			// move step of y-coordinate    
     
    TSize iSize;			// The square's size   
    
    TRect iScreenSize;		// The screen size
    
    TInt iDrawType;			// draw type
    
    TSize iPenSize;			// pen size
    
    TBool iDrawn;			// Has the animation been drawn
    
    TRgb iColor;			// draw color
    
	CTestAnimDll* iDll;		// anim dll
	
	RBuf iLastCommand; 		// store last command & parameter
	
	TInt iAnimType;			// animation type
};

#endif // ANIMPROC_H

