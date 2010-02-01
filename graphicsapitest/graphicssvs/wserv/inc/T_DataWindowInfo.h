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


#if (!defined __T_GRAPHICS_WSERV_WINDOWINFO_H__)
#define __T_GRAPHICS_WSERV_WINDOWINFO_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC Includes
#include <w32std.h>
#include <w32adll.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataWindowInfo : public CDataWrapperBase
	{

public:
	static CT_DataWindowInfo*	NewL();

	~CT_DataWindowInfo();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataWindowInfo();
	void  ConstructL();

private:
	/**
	 * Helper methods
	 */
	void	DestroyData();
	void	DoCmdnew();
	void	DoCmdDestructor();
	void	DoCmdGetRedrawRegionAndRedrawShadowRegion(const TDesC& aEntry);

private:
	TWindowInfo*	iWindowInfo;
	const TRegion*	iRedrawRegion;
	const TRegion*	iRedrawShadowRegion;
	};

#endif /* __T_GRAPHICS_WSERV_WINDOWINFO_H__ */
