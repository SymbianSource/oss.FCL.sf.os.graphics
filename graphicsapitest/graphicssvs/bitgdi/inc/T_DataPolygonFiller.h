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
#ifndef T_DATAPOLYGONFILLER_H_
#define T_DATAPOLYGONFILLER_H_


#include "DataWrapperBase.h"


class CT_DataPolygonFiller : public CDataWrapperBase
	{
public:
	static CT_DataPolygonFiller* NewL();

	~CT_DataPolygonFiller();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt);
	//void RunL(CActive* aActive, TInt aIndex); 
	
protected:
	CT_DataPolygonFiller();
    void ConstructL();
	
private:
	void DestroyData();
	void DoCmdnewL();
	void DoCmdConstructL(const TDesC& aSection);
	void DoCmdReset();
	void DoCmdGetNextPixelRun(const TDesC& aSection);
	void DoCmdGetNextPixelRunOnSpecifiedScanLine(const TDesC& aSection);
	void DoCmdDestructor();
private:
	CPolygonFiller* iPolygonFiller;
	CArrayFix<TPoint>*	iPointsArray;
	TPoint* iPoints;
	};

#endif /*T_DATAPOLYGONFILLER_H_*/
