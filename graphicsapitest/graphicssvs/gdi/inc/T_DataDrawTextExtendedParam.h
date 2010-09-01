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

This contains CT_DataDrawTextExtendedParam
*/

#if (!defined __T_DATA_DRAW_TEXT_EXTENDED_PARAM_H__)
#define __T_DATA_DRAW_TEXT_EXTENDED_PARAM_H__

//	User Includes
#include "T_DataDrawTextParam.h"

class CT_DataDrawTextExtendedParam : public CT_DataDrawTextParam
	{
public:
	CT_DataDrawTextExtendedParam();
	~CT_DataDrawTextExtendedParam();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	virtual CGraphicsContext::TDrawTextParam*	GetDrawTextParam() const;

	void	DoCmd_new();
	void	DoCmd_destructor();
	void	DoCmd_iParRightToLeft(const TDesC& aSection);

private:
	void	DestroyData();

private:
	CGraphicsContext::TDrawTextExtendedParam*	iDrawTextExtendedParam;
	};

#endif /* __T_DATA_DRAW_TEXT_EXTENDED_PARAM_H__ */
