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

/**
@test
@internalComponent

This contains CT_DataBitmapUtil
*/

#if (!defined __T_DATA_BITMAP_UTIL_H__)
#define __T_DATA_BITMAP_UTIL_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <fbs.h>

class CT_DataBitmapUtil : public CDataWrapperBase
	{
public:
	CT_DataBitmapUtil();
	~CT_DataBitmapUtil();
	
	static	CT_DataBitmapUtil*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	void			DestroyData();

private:
	void	ConstructL();

	static CT_DataBitmapUtil* GetBitmapUtilDataObjectFromParameterL(CDataWrapper& aDataWrapper, const TDesC& aParameterName, const TDesC& aSection);

	void	DoCmdNewL(const TDesC& aSection);
	void	DoCmdBeginL(const TDesC& aSection);
	void	DoCmdEnd();
	void	DoCmdSetPixelL(const TDesC& aSection);
	void	DoCmdGetPixel(const TDesC& aSection);
	void	DoCmdSetPos(const TDesC& aSection);
	void	DoCmdDecXPos();
	void	DoCmdDecYPos();
	void	DoCmdIncXPos();
	void	DoCmdIncYPos();

private:
	/** TBitmapUtil class instance that is tested */
	TBitmapUtil*	iBitmapUtil;
	};

#endif /* __T_DATA_BITMAP_UTIL_H__ */
