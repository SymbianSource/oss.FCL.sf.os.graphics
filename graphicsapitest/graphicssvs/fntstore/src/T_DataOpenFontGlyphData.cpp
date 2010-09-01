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

This contains CT_DataOpenFontGlyphData
*/

#include "T_DataOpenFontGlyphData.h"


/*@{*/
///	Parameters
_LIT(KFldBufferSize,							"buffer_size");

///	Commands
_LIT(KCmdNew,									"New");
_LIT(KCmdDestructor,							"~");

/// Logs
_LIT(KLogMissingParameter,						"Missing parameter '%S'");

/*@}*/

/**
* Two phase constructor
*/
CT_DataOpenFontGlyphData* CT_DataOpenFontGlyphData::NewL()
	{
	CT_DataOpenFontGlyphData* ret = new (ELeave) CT_DataOpenFontGlyphData();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataOpenFontGlyphData::CT_DataOpenFontGlyphData()
:	CDataWrapperBase()
,	iGlyphData(NULL)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataOpenFontGlyphData::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataOpenFontGlyphData::~CT_DataOpenFontGlyphData()
	{
	if (iGlyphData != NULL)
		DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataOpenFontGlyphData::DestroyData()
	{
	User::Heap().Free(iGlyphData);
	iGlyphData = NULL;
	}

TAny* CT_DataOpenFontGlyphData::GetObject()
	{
	return iGlyphData;
	}

void CT_DataOpenFontGlyphData::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iGlyphData	= static_cast<TOpenFontGlyphData*> (aObject);
	}
		
void CT_DataOpenFontGlyphData::DisownObjectL() 
	{
	iGlyphData = NULL;
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
TBool CT_DataOpenFontGlyphData::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNew(aSection);
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Creates an instance of TOpenFontGlyphData structure */
void CT_DataOpenFontGlyphData::DoCmdNew(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Creates an object of TOpenFontGlyphData"));

	// cleanup if any
	DestroyData();

	TInt	bufsize;
	if (!GetIntFromConfig(aSection, KFldBufferSize, bufsize))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBufferSize());
		SetBlockResult(EFail);
		}
	
	// call TOpenFontGlyphData::New()
	INFO_PRINTF1(_L("execute TOpenFontGlyphData::New(RHeap*, TInt)"));
	iGlyphData = TOpenFontGlyphData::New(&(User::Heap()), bufsize);

	if (iGlyphData == NULL)
		{
		ERR_PRINTF1(_L("No enough memory"));
		SetError(KErrNoMemory);
		}
	}

void CT_DataOpenFontGlyphData::DoCmdDestructor()
	{
	DestroyData();
	}
