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

#include "T_DataWindowInfo.h"

/*@{*/
///	Commands
_LIT(KCmdnew,									"new");
_LIT(KCmdDestructor,							"~");
_LIT(KCmdGetRedrawRegionAndRedrawShadowRegion,	"GetRedrawRegionAndRedrawShadowRegion");

///	Fields
_LIT(KFldNullExpected,							"nullexpected");

///	Logging
_LIT(KLogError,									"Error=%d");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWindowInfo* CT_DataWindowInfo::NewL()
/**
 * Two phase constructor
 */
	{
	CT_DataWindowInfo*	ret=new (ELeave) CT_DataWindowInfo();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWindowInfo::CT_DataWindowInfo()
/**
 * Protected constructor. First phase construction
 */
:	CDataWrapperBase()
,	iWindowInfo(NULL)
,	iRedrawRegion(NULL)
,	iRedrawShadowRegion(NULL)
	{
	}

void CT_DataWindowInfo::ConstructL()
/**
 * Second phase construction
 */
	{
	}

CT_DataWindowInfo::~CT_DataWindowInfo()
/**
 * Public destructor
 */
	{
	DestroyData();
	}

TAny* CT_DataWindowInfo::GetObject()
/**
 * Return a pointer to the object that the data wraps
 *
 * @return pointer to the object that the data wraps
 */
	{
	return iWindowInfo;
	}

void CT_DataWindowInfo::SetObjectL(TAny* aAny)
/**
 * Set the object that the data wraps
 *
 * @param	aObject object that the wrapper is testing
 *
 */
	{
	DestroyData();
	iWindowInfo = static_cast<TWindowInfo*> (aAny);
	}

void CT_DataWindowInfo::DisownObjectL()
/**
 * The object will no longer be owned by this
 *
 * @leave	KErrNotSupported if the the function is not supported
 */
	{
	iWindowInfo = NULL;
	}

void CT_DataWindowInfo::DestroyData()
	{
	delete iWindowInfo;
	iWindowInfo=NULL;
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
TBool CT_DataWindowInfo::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnew )
		{
		DoCmdnew();
		}
	else if ( aCommand==KCmdGetRedrawRegionAndRedrawShadowRegion )
		{
		DoCmdGetRedrawRegionAndRedrawShadowRegion(aSection);
		}
	else
		{
		ret=EFalse;
		}

	return ret;
	}


void CT_DataWindowInfo::DoCmdnew()
	{
	DestroyData();

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute new()"));
	TRAPD(err, iWindowInfo=new (ELeave) TWindowInfo());
	if ( err != KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWindowInfo::DoCmdDestructor()
	{
	INFO_PRINTF1(_L("execute ~"));
	DestroyData();
	}

void CT_DataWindowInfo::DoCmdGetRedrawRegionAndRedrawShadowRegion(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("GetRedrawRegionAndRedrawShadowRegion"));
	iWindowInfo->GetRedrawRegionAndRedrawShadowRegion(iRedrawRegion, iRedrawShadowRegion);

	TBool	nullExpected;
	if ( GetBoolFromConfig(aSection, KFldNullExpected(), nullExpected) )
		{
		if ( nullExpected )
			{
			if ( (iRedrawRegion!=NULL) || (iRedrawShadowRegion!=NULL) )
				{
				ERR_PRINTF3(_L("NULL pointers expected. RedrawRegion=0x%x, RedrawShadowRegion=0x%x"), iRedrawRegion, iRedrawShadowRegion);
				SetBlockResult(EFail);
				}
			}
		else
			{
			if ( (iRedrawRegion==NULL) || (iRedrawShadowRegion==NULL) )
				{
				ERR_PRINTF3(_L("Non NULL pointers expected. RedrawRegion=0x%x, RedrawShadowRegion=0x%x"), iRedrawRegion, iRedrawShadowRegion);
				SetBlockResult(EFail);
				}
			}
		}
	}
