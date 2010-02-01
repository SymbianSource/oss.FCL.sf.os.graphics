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

This contains CT_DataPalette
*/

#if (!defined __T_DATA_PALETTE_H__)
#define __T_DATA_PALETTE_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <gdi.h>

class CT_DataPalette : public CDataWrapperBase
	{
public:

	static CT_DataPalette*	NewL();

	~CT_DataPalette();

	virtual TAny* GetObject();

	void DestroyData();

	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataPalette();
	void ConstructL();

private:
	void	DoCmdNew(const TDesC& aSection);
	void	DoCmdNewDefault(const TDesC& aSection);

private:
	/** CPalette class instance to work with*/
    CPalette* iPalette;
	};

#endif /* __T_DATA_PALETTE_H__ */
