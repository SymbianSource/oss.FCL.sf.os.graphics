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

//	User includes
#include "T_DataDrawTextParam.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmd_new,										"new");
_LIT(KCmd_destructor,								"~");
_LIT(KCmd_iDirection,								"iDirection");
_LIT(KCmd_iCharJustNum,								"iCharJustNum");
_LIT(KCmd_iCharJustExcess,							"iCharJustExcess");
_LIT(KCmd_iWordJustNum,								"iWordJustNum");
_LIT(KCmd_iWordJustExcess,							"iWordJustExcess");

///	Parameters
_LIT(KFldValue,										"value");

///	Logging
_LIT(KLogError,										"Error=%d");
/*@}*/

/**
* Constructor. First phase construction
*/
CT_DataDrawTextParam::CT_DataDrawTextParam()
:	CDataWrapperBase()
,	iDrawTextParam(NULL)
	{
	}

/**
* Constructor. First phase construction
*/
CT_DataDrawTextParam::~CT_DataDrawTextParam()
	{
	DestroyData();
	}

void CT_DataDrawTextParam::DestroyData()
	{
	delete iDrawTextParam;
	iDrawTextParam=NULL;
	}

TAny* CT_DataDrawTextParam::GetObject()
	{
	return iDrawTextParam;
	}

void CT_DataDrawTextParam::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iDrawTextParam=static_cast<CGraphicsContext::TDrawTextParam*>(aAny);
	}

void CT_DataDrawTextParam::DisownObjectL()
	{
	iDrawTextParam=NULL;
	}

CGraphicsContext::TDrawTextParam* CT_DataDrawTextParam::GetDrawTextParam() const
	{
	return iDrawTextParam;
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
TBool CT_DataDrawTextParam::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
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
	else if ( aCommand==KCmd_iDirection )
		{
		DoCmd_iDirection(aSection);
		}
	else if ( aCommand==KCmd_iCharJustNum )
		{
		DoCmd_iCharJustNum(aSection);
		}
	else if ( aCommand==KCmd_iCharJustExcess )
		{
		DoCmd_iCharJustExcess(aSection);
		}
	else if ( aCommand==KCmd_iWordJustNum )
		{
		DoCmd_iWordJustNum(aSection);
		}
	else if ( aCommand==KCmd_iWordJustExcess )
		{
		DoCmd_iWordJustExcess(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}

void CT_DataDrawTextParam::DoCmd_new()
	{
	DestroyData();
	TRAPD(err, iDrawTextParam = new (ELeave) CGraphicsContext::TDrawTextParam());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataDrawTextParam::DoCmd_destructor()
	{
	DestroyData();
	}

void CT_DataDrawTextParam::DoCmd_iDirection(const TDesC& aSection)
	{
	CGraphicsContext::TDrawTextParam*	textParam=GetDrawTextParam();
	TPtrC	deviceName;
	if ( !CT_GraphicsUtil::ReadTextDirection(*this, aSection, KFldValue(), textParam->iDirection) )
		{
		INFO_PRINTF2(_L("iDirection=%d"), textParam->iDirection);
		}
	}

void CT_DataDrawTextParam::DoCmd_iCharJustNum(const TDesC& aSection)
	{
	CGraphicsContext::TDrawTextParam*	textParam=GetDrawTextParam();
	TPtrC	deviceName;
	if ( !GetIntFromConfig(aSection, KFldValue(), textParam->iCharJustNum) )
		{
		INFO_PRINTF2(_L("iCharJustNum=%d"), textParam->iCharJustNum);
		}
	}

void CT_DataDrawTextParam::DoCmd_iCharJustExcess(const TDesC& aSection)
	{
	CGraphicsContext::TDrawTextParam*	textParam=GetDrawTextParam();
	TPtrC	deviceName;
	if ( !GetIntFromConfig(aSection, KFldValue(), textParam->iCharJustExcess) )
		{
		INFO_PRINTF2(_L("iCharJustExcess=%d"), textParam->iCharJustExcess);
		}
	}

void CT_DataDrawTextParam::DoCmd_iWordJustNum(const TDesC& aSection)
	{
	CGraphicsContext::TDrawTextParam*	textParam=GetDrawTextParam();
	TPtrC	deviceName;
	if ( !GetIntFromConfig(aSection, KFldValue(), textParam->iWordJustNum) )
		{
		INFO_PRINTF2(_L("iWordJustNum=%d"), textParam->iWordJustNum);
		}
	}

void CT_DataDrawTextParam::DoCmd_iWordJustExcess(const TDesC& aSection)
	{
	CGraphicsContext::TDrawTextParam*	textParam=GetDrawTextParam();
	TPtrC	deviceName;
	if ( !GetIntFromConfig(aSection, KFldValue(), textParam->iWordJustExcess) )
		{
		INFO_PRINTF2(_L("iWordJustExcess=%d"), textParam->iWordJustExcess);
		}
	}
