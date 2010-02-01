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

This contains CT_FBServAPIServer
*/

#if (!defined __T_FBSERV_API_SERVER_H__)
#define __T_FBSERV_API_SERVER_H__

//	User Include
#include "T_DataFbsSession.h"
#include "T_DataFbsFont.h"
#include "T_DataFbsBitmap.h"
#include "T_DataBitmapUtil.h"
#include "T_DataFbsTypefaceStore.h"
#include "T_DataFbsColor256BitmapUtil.h"

#include "T_DataPalette.h"
#include "T_DataFbsBitmapDevice.h"
#include "T_DataFbsScreenDevice.h"
#include "T_DataAlgStyle.h"
#include "T_DataMeasureTextInput.h"
#include "T_DataMeasureTextOutput.h"

#include "TestServerBase.h"

class CT_FBServAPIServer : public CTestServerBase
	{
private:
	class CT_FBServAPIBlock : public CTestBlockController
		{
	public:
		inline CT_FBServAPIBlock();
		inline ~CT_FBServAPIBlock();

		inline CDataWrapper* CreateDataL( const TDesC& aData );
		};
public:
	inline CT_FBServAPIServer();
	inline ~CT_FBServAPIServer();

	static CT_FBServAPIServer* NewL();

	inline CTestBlockController*	CreateTestBlock();
	};

#include "T_FBServAPIServer.inl"

#endif /* __T_FBSERV_API_SERVER_H__ */
