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

This contains CT_DataTypefaceStore
*/

#if (!defined __T_DATA_TYPEFACE_STORE_H__)
#define __T_DATA_TYPEFACE_STORE_H__

//	User Includes
#include "DataWrapperBase.h"
#include "T_DataFont.h"

//	epoc32 includes
#include <gdi.h>

class CT_DataTypefaceStore : public CDataWrapperBase
	{
public:
	virtual TBool			DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	CT_DataTypefaceStore();

protected:

	virtual CTypefaceStore*	GetTypefaceStore() const = 0;
	CT_DataFont*			GetFontDataObjectFromParameterL(const TDesC& aParameterName, const TDesC& aSection);
	void					SetFontDataObjectIfPassedAsParameterL(CFont* font, const TDesC& aSection);

private:
	void	DoCmdGetNearestFontInTwipsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection);
	void	DoCmdNumTypefaces(const TDesC& aSection);
	void	DoCmdTypefaceSupport(const TDesC& aSection);
	void	DoCmdFontHeightInTwips(const TDesC& aSection);
	void	DoCmdReleaseFontL(const TDesC& aSection);
	};

#endif /* __T_DATA_TYPEFACE_STORE_H__ */
