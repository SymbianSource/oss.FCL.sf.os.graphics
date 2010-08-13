// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Guest video driver - request handler extension (a.k.a. Command Scheduler extension)
// This allows the sgdevice.ldd to make calls into the Command Scheduler
//

#ifndef ReqHandlerExtension_H
#define ReqHandlerExtension_H


#include <kernel/kernel.h>

#include <sgresource/sgcommon.h>//TSgImageMetaData

#include "virtualvideotracing.h"

class MReqHandlerCallback
    {
public:
	virtual void CreateSgImagePbuffer( const TSgImageMetaData& aInfo, TRequestStatus* aStatus, DThread* aThread ) = 0;
	virtual void CreateSgImageVGImage( const TSgImageMetaData& aInfo, TRequestStatus* aStatus, DThread* aThread ) = 0;
	virtual TInt DestroySgImage( const TUint64 aId ) = 0;    
    };

NONSHARABLE_CLASS(DReqHandlerExtensionImpl): public DBase
	{
public:
	TInt Construct();
	void CreateSgImagePbuffer( const TSgImageMetaData& aInfo, TRequestStatus* aStatus, DThread* aThread );
	void CreateSgImageVGImage( const TSgImageMetaData& aInfo, TRequestStatus* aStatus, DThread* aThread );
	TInt DestroySgImage( const TUint64 aId );
	TInt SetReqHandler( MReqHandlerCallback* aHandler );
private:
	DMutex* iMutex;
	MReqHandlerCallback* iCallBackHandler;//not owned
	};

class ReqHandlerExtension
	{
public:
	//SgImage Creation and Destruction commands:
	IMPORT_C static TInt CreateSgImagePbuffer( const TSgImageMetaData& aInfo );
	IMPORT_C static TInt CreateSgImageVGImage( const TSgImageMetaData& aInfo );
	IMPORT_C static TInt DestroySgImage( const TUint64 aId );
	IMPORT_C static TInt SetReqHandler( MReqHandlerCallback* aHandler );
	//other functions to be added
	};

//#include <sgresource/ReqHandlerExtension.inl>

#endif // ReqHandlerExtension_H
