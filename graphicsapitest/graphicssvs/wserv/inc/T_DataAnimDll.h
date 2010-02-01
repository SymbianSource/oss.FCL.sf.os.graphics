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


#if (!defined __T_GRAPHICS_WSERV_ANIMDLL_H__)
#define __T_GRAPHICS_WSERV_ANIMDLL_H__

//	User Includes
#include "T_DataWsClientClass.h"

class CT_DataAnimDll : public CDataWrapperBase, public CT_DataWsClientClass
	{
public:
	static CT_DataAnimDll*	NewL();

	~CT_DataAnimDll();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);

	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataAnimDll();
	void  ConstructL();
	
	virtual MWsClientClass*		GetClientClass() const;

private:
	void	DestroyData();
	
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdLoad(const TDesC& aSection);
	void	DoCmdClose();
	void	DoCmdDestroy();
	void	DoCmdDestructor();

private:
	RAnimDll*	iAnimDll;
	};

#endif /* __T_GRAPHICS_WSERV_ANIMDLL_H__ */
