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

This contains CT_DataDrawTextParam
*/

#if (!defined __T_DATA_DRAW_TEXT_PARAM_H__)
#define __T_DATA_DRAW_TEXT_PARAM_H__

//	epoc32 includes
#include <gdi.h>

//	User Includes
#include "DataWrapperBase.h"

class CT_DataDrawTextParam : public CDataWrapperBase
	{
public:
	CT_DataDrawTextParam();
	~CT_DataDrawTextParam();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	virtual CGraphicsContext::TDrawTextParam*	GetDrawTextParam() const;

	void	DoCmd_new();
	void	DoCmd_destructor();
	void	DoCmd_iDirection(const TDesC& aSection);
	void	DoCmd_iCharJustNum(const TDesC& aSection);
	void	DoCmd_iCharJustExcess(const TDesC& aSection);
	void	DoCmd_iWordJustNum(const TDesC& aSection);
	void	DoCmd_iWordJustExcess(const TDesC& aSection);

private:
	void	DestroyData();

private:
	CGraphicsContext::TDrawTextParam*	iDrawTextParam;
	};

#endif /* __T_DATA_DRAW_TEXT_PARAM_H__ */
