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

This contains CT_DataAlgStyle
*/

#if (!defined __T_DATA_ALG_STYLE_H__)
#define __T_DATA_ALG_STYLE_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <fntstore.h>

class CT_DataAlgStyle : public CDataWrapperBase
	{
public:
	CT_DataAlgStyle();
	~CT_DataAlgStyle();
	
	static	CT_DataAlgStyle*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	void			DestroyData();

private:
	void	ConstructL();

	void	DoCmdNew();
	void	DoCmdSetWidthFactor(const TDesC& aSection);
	void	DoCmdSetHeightFactor(const TDesC& aSection);

private:
	/** TAlgStyle class instance to work with*/
    TAlgStyle*	iAlgStyle;
	};

#endif /* __T_DATA_ALG_STYLE_H__ */
