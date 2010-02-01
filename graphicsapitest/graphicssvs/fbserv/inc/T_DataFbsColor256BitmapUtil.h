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

This contains CT_DataFbsColor256BitmapUtil
*/

#if (!defined __T_DATA_FBS_COLOR256_BITMAP_UTIL_H__)
#define __T_DATA_FBS_COLOR256_BITMAP_UTIL_H__

//	User Includes
#include "DataWrapperBase.h"
//	EPOC includes
#include <e32std.h>
#include <fbs.h>
#include "DataWrapperBase.h"

class CT_DataFbsColor256BitmapUtil : public CDataWrapperBase
	{
public:

	static CT_DataFbsColor256BitmapUtil*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	~CT_DataFbsColor256BitmapUtil();
	virtual TAny*	GetObject();

protected:
	CT_DataFbsColor256BitmapUtil();
	void ConstructL();

private:
	void	DestroyData();
	void	DoCmdNewL(const TDesC& aSection);
	void	DoCmdCopyBitmapL(const TDesC& aSection);

	TBool	ReadDither(CDataWrapper& aDataWrapper, const TDesC& aSection, CFbsColor256BitmapUtil::TDither& aDither);

private:
	/** CFbsColor256BitmapUtil class instance to work with*/
    CFbsColor256BitmapUtil*	iFbsColor256BitmapUtil;
	};

#endif /* __T_DATA_FBS_COLOR256_BITMAP_UTIL_H__ */
