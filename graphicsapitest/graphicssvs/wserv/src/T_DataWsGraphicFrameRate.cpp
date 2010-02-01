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

#include "T_DataWsGraphicFrameRate.h"

/*@{*/
///	Commands
_LIT(KCmdnew,					"new");

///	Logging
_LIT(KLogError,					"Error=%d");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphicFrameRate::CT_DataWsGraphicFrameRate()
:	CT_DataWsGraphicMsgFixedBase()
,	iWsGraphicFrameRate(NULL)
	{
	}

CT_DataWsGraphicFrameRate::~CT_DataWsGraphicFrameRate()
	{
	DestroyData();
	}

void CT_DataWsGraphicFrameRate::DestroyData()
	{
	delete iWsGraphicFrameRate;
	iWsGraphicFrameRate=NULL;
	}

/**
 * Return a pointer to the object that the data wraps
 *
 * @return pointer to the object that the data wraps
 */
TAny* CT_DataWsGraphicFrameRate::GetObject()
	{
	return iWsGraphicFrameRate;
	}

TWsGraphicMsgFixedBase* CT_DataWsGraphicFrameRate::GetWsGraphicMsgFixedBase() const
	{
	return iWsGraphicFrameRate;
	}

/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @param aAsyncErrorIndex	index of command. used for async calls
*
* @return ETrue if the command is processed
*/
TBool CT_DataWsGraphicFrameRate::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret=ETrue;

	if ( aCommand==KCmdnew )
		{
		DoCmdnew();
		}
	else
		{
		ret=CT_DataWsGraphicMsgFixedBase::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataWsGraphicFrameRate::DoCmdnew()
	{
	INFO_PRINTF1(_L("execute new"));
	DestroyData();
	TRAPD(err, iWsGraphicFrameRate=new (ELeave) TWsGraphicFrameRate());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}
