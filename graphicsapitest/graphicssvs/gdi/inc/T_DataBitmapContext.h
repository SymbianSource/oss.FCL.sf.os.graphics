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

This contains CT_DataBitmapContext
*/

#if (!defined __T_DATA_BITMAP_CONTEXT_H__)
#define __T_DATA_BITMAP_CONTEXT_H__

//	User includes
#include "T_DataGraphicsContext.h"

class CT_DataBitmapContext : public CT_DataGraphicsContext
	{
public:
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataBitmapContext();
	virtual CBitmapContext*	GetBitmapContext() const = 0;

	void	DoCmdAlphaBlendBitmapsL(const TDesC& aSection);
	void	DoCmdBitBltL(const TDesC& aSection);
	void	DoCmdBitBltMaskedL(const TDesC& aSection);
	void	DoCmdClear(const TDesC& aSection);
	void	DoCmdCopyRect(const TDesC& aSection);
	void	DoCmdSetFaded(const TDesC& aSection);
	void	DoCmdSetFadingParameters(const TDesC& aSection);
	};

#endif /* __T_DATA_BITMAP_CONTEXT_H__ */
