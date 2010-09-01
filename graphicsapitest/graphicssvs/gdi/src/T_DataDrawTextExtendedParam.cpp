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

//	User includes
#include "T_DataDrawTextExtendedParam.h"

/*@{*/
///	Commands
_LIT(KCmd_new,										"new");
_LIT(KCmd_destructor,								"~");
_LIT(KCmd_iParRightToLeft,							"iParRightToLeft");

///	Parameters
_LIT(KFldValue,										"value");

///	Logging
_LIT(KLogError,										"Error=%d");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataDrawTextExtendedParam::CT_DataDrawTextExtendedParam()
:	CT_DataDrawTextParam()
,	iDrawTextExtendedParam(NULL)
	{
	}

/**
* Constructor. First phase construction
*/
CT_DataDrawTextExtendedParam::~CT_DataDrawTextExtendedParam()
	{
	DestroyData();
	}

void CT_DataDrawTextExtendedParam::DestroyData()
	{
	delete iDrawTextExtendedParam;
	iDrawTextExtendedParam=NULL;
	}

TAny* CT_DataDrawTextExtendedParam::GetObject()
	{
	return iDrawTextExtendedParam;
	}

void CT_DataDrawTextExtendedParam::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iDrawTextExtendedParam=static_cast<CGraphicsContext::TDrawTextExtendedParam*>(aAny);
	}

void CT_DataDrawTextExtendedParam::DisownObjectL()
	{
	iDrawTextExtendedParam=NULL;
	}

CGraphicsContext::TDrawTextParam* CT_DataDrawTextExtendedParam::GetDrawTextParam() const
	{
	return iDrawTextExtendedParam;
	}

/**
* Process a command read from the ini file
*
* @param aDataWrapper	test step requiring command to be processed
* @param aCommand	the command to process
* @param aSection		the entry in the ini file requiring the command to be processed
*
* @return ETrue if the command is processed
*/
TBool CT_DataDrawTextExtendedParam::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret=ETrue;
	if ( aCommand==KCmd_new )
		{
		DoCmd_new();
		}
	else if ( aCommand==KCmd_destructor )
		{
		DoCmd_destructor();
		}
	else if ( aCommand==KCmd_iParRightToLeft )
		{
		DoCmd_iParRightToLeft(aSection);
		}
	else
		{
		ret=CT_DataDrawTextParam::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataDrawTextExtendedParam::DoCmd_new()
	{
	DestroyData();
	TRAPD(err, iDrawTextExtendedParam = new (ELeave) CGraphicsContext::TDrawTextExtendedParam());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataDrawTextExtendedParam::DoCmd_destructor()
	{
	DestroyData();
	CT_DataDrawTextParam::DoCmd_destructor();
	}

void CT_DataDrawTextExtendedParam::DoCmd_iParRightToLeft(const TDesC& aSection)
	{
	if ( !GetBoolFromConfig(aSection, KFldValue(), iDrawTextExtendedParam->iParRightToLeft) )
		{
		INFO_PRINTF2(_L("iParRightToLeft=%d"), iDrawTextExtendedParam->iParRightToLeft);
		}
	}
