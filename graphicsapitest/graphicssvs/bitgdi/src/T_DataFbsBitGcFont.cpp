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

This contains CT_DataFbsBitGcFont
*/

#include "T_DataFbsBitGcFont.h"

/*@{*/
///	Parameters
_LIT(KFldHandle,						"handle");
_LIT(KFontDataObjectName,				"font_data_object_name");

///	Commands
_LIT(KCmdNewL,							"NewL");
_LIT(KCmdDestructor,					"~");
_LIT(KCmdAddress,						"Address");
_LIT(KCmdDuplicate,						"Duplicate");
_LIT(KCmdReset, 						"Reset");


/// Logs
_LIT(KLogMissingParameter,				"Missing parameter '%S'");
_LIT(KLogErrNum,						"Execute failed with error = %d!");	
/*@}*/


/**
* Constructor
*/
CT_DataFbsBitGcFont* CT_DataFbsBitGcFont::NewL()
	{
	CT_DataFbsBitGcFont* ret = new (ELeave) CT_DataFbsBitGcFont();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataFbsBitGcFont::CT_DataFbsBitGcFont()
:	CT_DataFbsFont()
,	iFbsBitGcFont(NULL)
	{
	}

/**
* Protected second phase construction
*/
void CT_DataFbsBitGcFont::ConstructL()
	{
	}

/**
* Destructor.
*/
CT_DataFbsBitGcFont::~CT_DataFbsBitGcFont()
	{
	DisownObjectL();
	}


CFont*	CT_DataFbsBitGcFont::GetFont() const
	{
	return iFbsBitGcFont;
	}

CFbsFont* CT_DataFbsBitGcFont::GetFbsFont() const
	{
	return iFbsBitGcFont;
	}

/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsBitGcFont::GetObject()
	{
	return iFbsBitGcFont;
	}

void CT_DataFbsBitGcFont::SetObjectL(TAny* aObject)
	{
	iFbsBitGcFont = static_cast<CFbsBitGcFont*> (aObject);
	}

void CT_DataFbsBitGcFont::DisownObjectL() 
	{
	iFbsBitGcFont = NULL;
	}


/**
* Process a command read from the ini file
*
* @param aDataWrapper	test step requiring command to be processed
* @param aCommand		the command to process
* @param aSection		the entry in the ini file requiring the command to be processed
*
* @return ETrue if the command is processed
*/
TBool CT_DataFbsBitGcFont::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	/* Another work package will complete the implement of this */
	TBool	retVal = ETrue;

	if ( aCommand == KCmdNewL )
		{
		DoCmdNewL();
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else if (aCommand == KCmdAddress)
		{
		DoCmdAddress();
		}
	else if (aCommand == KCmdDuplicate)
		{
		DoCmdDuplicate(aSection);
		}
	else if (aCommand == KCmdReset)
		{
		DoCmdReset();
		}
	else
		{
		retVal = CT_DataFbsFont::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}
	return retVal;
	}


void CT_DataFbsBitGcFont::DoCmdNewL()
	{
	TInt err = KErrNone;
	INFO_PRINTF1(_L("Create CFbsBitGcFont Object by CFbsBitGcFont() constructor function."));
	TRAP(err, iFbsBitGcFont = new (ELeave) CFbsBitGcFont());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

void CT_DataFbsBitGcFont::DoCmdDestructor()
	{
	DisownObjectL();
	}

void CT_DataFbsBitGcFont::DoCmdAddress()
	{
	CBitmapFont* BitmapFontPtr = NULL;
	INFO_PRINTF1(_L("Execute CFbsBitGcFont::Address() function."));
	
	BitmapFontPtr = iFbsBitGcFont->Address();
	if (NULL == BitmapFontPtr)
		{
		INFO_PRINTF1(_L("The CBitmapFont* returned is NULL."));
		}
	}

void CT_DataFbsBitGcFont::DoCmdDuplicate(const TDesC& aSection)
	{
	TInt handle = 0;
	INFO_PRINTF1(_L("Execute CFbsBitGcFont::Duplicate(TInt aHandle) function."));
	
	if(GetIntFromConfig(aSection,KFldHandle,handle))
		{
		INFO_PRINTF2(_L("Execute Duplicate(TInt aHandle) using the specified handle = %d"), handle);
		}
	else
		{
		TPtrC fontDataObjectName;
		if (!GetStringFromConfig(aSection, KFontDataObjectName(), fontDataObjectName))
			{
			ERR_PRINTF2(KLogMissingParameter, &KFontDataObjectName);
			SetBlockResult(EFail);
			return;
			}
		else
			{
			CT_DataFbsBitGcFont* fontDataObject = NULL;
			fontDataObject = static_cast<CT_DataFbsBitGcFont*>(GetDataWrapperL(fontDataObjectName));
			if (NULL == fontDataObject)
				{
				ERR_PRINTF1(_L("Bad Object!"));
				SetBlockResult(EFail);
				return;
				}
			else
				{
				handle = ((CFbsBitGcFont*)fontDataObject->GetObject())->Handle();
				INFO_PRINTF2(_L("Execute Duplicate(TInt aHandle) using CFbsBitGcFont Object's handle = %d"), handle);
				}
			}
		}
	
	TInt err = iFbsBitGcFont->Duplicate(handle);
	if (KErrNone != err)
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

void CT_DataFbsBitGcFont::DoCmdReset()
	{
	INFO_PRINTF1(_L("Execute CFbsBitGcFont::Reset() function."));
	iFbsBitGcFont->Reset();
	}
