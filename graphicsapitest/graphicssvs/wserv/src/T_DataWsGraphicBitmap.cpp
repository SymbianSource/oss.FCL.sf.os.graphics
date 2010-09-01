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

#include "T_DataWsGraphicBitmap.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Commands
_LIT(KCmdNewL,						"NewL");
_LIT(KCmdDestructor,				"~CWsGraphicBitmap");
_LIT(KCmdDestructorGeneral,			"~");

///	Fields
_LIT(KFldBitmap,					"bitmap");
_LIT(KFldMask,						"mask");
_LIT(KFldReplace,					"replace");
_LIT(KFldUid,						"uid");

///	Logging
_LIT(KLogError,						"Error=%d");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphicBitmap* CT_DataWsGraphicBitmap::NewL()
	{
	CT_DataWsGraphicBitmap*	ret=new (ELeave) CT_DataWsGraphicBitmap();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWsGraphicBitmap::CT_DataWsGraphicBitmap()
:	CT_DataWsGraphic()
,	iWsGraphicBitmap(NULL)
	{
	}

void CT_DataWsGraphicBitmap::ConstructL()
	{
	}

CT_DataWsGraphicBitmap::~CT_DataWsGraphicBitmap()
	{
	DestroyData();
	}

void CT_DataWsGraphicBitmap::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWsGraphicBitmap = static_cast<CWsGraphicBitmap*> (aAny);
	}

void CT_DataWsGraphicBitmap::DisownObjectL()
	{
	iWsGraphicBitmap = NULL;
	}

void CT_DataWsGraphicBitmap::DestroyData()
	{
	delete iWsGraphicBitmap;
	iWsGraphicBitmap=NULL;
	}

//	CT_DataWsGraphic Implementation
CWsGraphic* CT_DataWsGraphicBitmap::GetWsGraphic() const
	{
	return iWsGraphicBitmap;
	}

TInt CT_DataWsGraphicBitmap::Share(TSecureId aClientId)
	{
	return iWsGraphicBitmap->Share(aClientId);
	}

TInt CT_DataWsGraphicBitmap::ShareGlobally()
	{
	return iWsGraphicBitmap->ShareGlobally();
	}

TInt CT_DataWsGraphicBitmap::UnShare(TSecureId aClientId)
	{
	return iWsGraphicBitmap->UnShare(aClientId);
	}

TInt CT_DataWsGraphicBitmap::UnShareGlobally()
	{
	return iWsGraphicBitmap->UnShareGlobally();
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
TBool CT_DataWsGraphicBitmap::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor || aCommand==KCmdDestructorGeneral )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdNewL )
		{
		DoCmdNewL(aSection);
		}
	else
		{
		ret=CT_DataWsGraphic::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataWsGraphicBitmap::DoCmdNewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	CFbsBitmap*	bitmap = NULL;
	CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldBitmap, bitmap);

	CFbsBitmap*	mask = NULL;
	CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KFldMask, mask);

	TInt	err=KErrNone;

	TInt	datUid;
	if ( GetIntFromConfig(aSection, KFldUid, datUid) )
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute NewL(TUid , CFbsBitmap*, CFbsBitmap*)"));
		TRAP(err, iWsGraphicBitmap = CWsGraphicBitmap::NewL(TUid::Uid(datUid), bitmap, mask));
		}
	else
		{
		TWsGraphicId	datReplace(TWsGraphicId::EUninitialized);
		if ( GetWsGraphicIdFromConfigL(aSection, KFldReplace, datReplace) )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute NewL(TWsGraphicId , CFbsBitmap*, CFbsBitmap*)"));
			TRAP(err, iWsGraphicBitmap = CWsGraphicBitmap::NewL(datReplace, bitmap, mask));
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute NewL(CFbsBitmap*, CFbsBitmap*)"));
			TRAP(err, iWsGraphicBitmap = CWsGraphicBitmap::NewL(bitmap, mask));
			}
		}

	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsGraphicBitmap::DoCmdDestructor()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute ~CWsGraphicBitmap()"));
	DestroyData();
	}
