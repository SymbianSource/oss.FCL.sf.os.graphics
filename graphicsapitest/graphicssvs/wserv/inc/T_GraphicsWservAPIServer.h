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


#if (!defined __T_GRAPHICS_WSERV_API_SERVER_H__)
#define __T_GRAPHICS_WSERV_API_SERVER_H__

//	User Include
#include "TestServerBase.h"

class CT_GraphicsWservAPIServer : public CTestServerBase
	{
private:
	class CT_GraphicsWservAPIBlock : public CTestBlockController
		{
	public:
		inline CT_GraphicsWservAPIBlock();
		inline ~CT_GraphicsWservAPIBlock();

		CDataWrapper*	CreateDataL(const TDesC& aData);
		};

public:
	inline CT_GraphicsWservAPIServer();
	inline ~CT_GraphicsWservAPIServer();

	static CT_GraphicsWservAPIServer* NewL();

	inline CTestBlockController*	CreateTestBlock();
	};

#include "T_GraphicsWservAPIServer.inl"

#endif /* __T_GRAPHICS_WSERV_API_SERVER_H__ */
