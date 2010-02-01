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

#ifndef T_DATAFBSBITGCBITMAP_H_
#define T_DATAFBSBITGCBITMAP_H_

#include "T_DataFbsBitmap.h"


class CT_DataFbsBitGcBitmap : public CT_DataFbsBitmap
	{
public:
	static CT_DataFbsBitGcBitmap* NewL();

	~CT_DataFbsBitGcBitmap();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	
protected:
	CT_DataFbsBitGcBitmap();
    void ConstructL();
	virtual CFbsBitmap*	GetFbsBitmap() const;
	
private:
	void DestroyData();
	void DoCmdDestructor();
	void DoCmdNewL();
	void DoCmdAddress(const TDesC& aSection);
	void DoCmdLockHeap();
	void DoCmdUnlockHeap();
private:	
	CFbsBitGcBitmap* iFbsBitGcBitmap;
	
	};

#endif /*T_DATAFBSBITGCBITMAP_H_*/

