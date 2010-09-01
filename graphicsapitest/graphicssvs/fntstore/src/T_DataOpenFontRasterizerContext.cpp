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

This contains CT_DataOpenFontRasterizerContext
*/

#include "T_DataOpenFontRasterizerContext.h"
#include "T_GraphicsUtil.h"


/*@{*/
///	Parameters
_LIT(KFldGlyphData,							"glyphdata");
_LIT(KFldBit,								"bit");
_LIT(KFldByte,								"byte");

///	Commands
_LIT(KCmdNew,								"new");
_LIT(KCmdEndGlyph,							"EndGlyph");
_LIT(KCmdStartGlyph,						"StartGlyph");
_LIT(KCmdWriteGlyphBit,						"WriteGlyphBit");
_LIT(KCmdWriteGlyphByte,					"WriteGlyphByte");
_LIT(KCmdDestructor,						"~");

/// Logs
_LIT(KLogMissingParameter,			"Missing parameter '%S'");

/*@}*/


/**
* Two phase constructor
*/
CT_DataOpenFontRasterizerContext* CT_DataOpenFontRasterizerContext::NewL()
	{
	CT_DataOpenFontRasterizerContext* ret = new (ELeave) CT_DataOpenFontRasterizerContext();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataOpenFontRasterizerContext::CT_DataOpenFontRasterizerContext()
:	CDataWrapperBase()
,	iRasterizerContext(NULL)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataOpenFontRasterizerContext::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataOpenFontRasterizerContext::~CT_DataOpenFontRasterizerContext()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataOpenFontRasterizerContext::DestroyData()
	{
	delete iRasterizerContext;
	iRasterizerContext = NULL;
	}

TAny* CT_DataOpenFontRasterizerContext::GetObject()
	{
	return iRasterizerContext;
	}

void CT_DataOpenFontRasterizerContext::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iRasterizerContext	= static_cast<COpenFontRasterizerContext*> (aObject);
	}
		
void CT_DataOpenFontRasterizerContext::DisownObjectL() 
	{
	iRasterizerContext = NULL;
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
TBool CT_DataOpenFontRasterizerContext::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNew();
		}
	else if (aCommand == KCmdEndGlyph)
		{
		DoCmdEndGlyph();
		}
	else if (aCommand == KCmdStartGlyph)
		{
		DoCmdStartGlyphL(aSection);
		}
	else if (aCommand == KCmdWriteGlyphBit)
		{
		DoCmdWriteGlyphBit(aSection);
		}
	else if (aCommand == KCmdWriteGlyphByte)
		{
		DoCmdWriteGlyphByte(aSection);
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

/** Creates an instance of COpenFontRasterizerContext structure */
void CT_DataOpenFontRasterizerContext::DoCmdNew()
	{
	INFO_PRINTF1(_L("Creates an instance of COpenFontRasterizerContext"));

	// cleanup if any
	DestroyData();

	// call new operator
	TRAPD(err, iRasterizerContext = new (ELeave) COpenFontRasterizerContext());

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating an instance: %d"), err);
		SetError(err);
		}
	}

void CT_DataOpenFontRasterizerContext::DoCmdEndGlyph()
	{
	INFO_PRINTF1(_L("execute COpenFontRasterizerContext::EndGlyph()"));
	iRasterizerContext->EndGlyph();
	}

void CT_DataOpenFontRasterizerContext::DoCmdStartGlyphL(const TDesC& aSection)
	{
	TOpenFontGlyphData* glyphData= NULL;
	
	if (!CT_GraphicsUtil::GetOpenFontGlyphDataL(*this, aSection, KFldGlyphData, glyphData))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldGlyphData());
		SetBlockResult(EFail);
		}
	else
		{
        INFO_PRINTF1(_L("execute COpenFontRasterizerContext::StartGlyph(TOpenFontGlyphData*)"));
		iRasterizerContext->StartGlyph(glyphData);
		}
	}

void CT_DataOpenFontRasterizerContext::DoCmdWriteGlyphBit(const TDesC& aSection)
	{
	TInt bit;
	
	if (!GetIntFromConfig(aSection, KFldBit, bit))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldBit());
		SetBlockResult(EFail);
		}
	else
		{
        INFO_PRINTF1(_L("execute COpenFontRasterizerContext::WriteGlyphBit(TInt)"));
		iRasterizerContext->WriteGlyphBit(bit);
		}
	}

void CT_DataOpenFontRasterizerContext::DoCmdWriteGlyphByte(const TDesC& aSection)
	{
	TInt byte;
		
	if (!GetIntFromConfig(aSection, KFldByte, byte))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldByte());
		SetBlockResult(EFail);
		}
	else
		{
        INFO_PRINTF1(_L("execute COpenFontRasterizerContext::WriteGlyphByte(TInt)"));
		iRasterizerContext->WriteGlyphByte(byte);
		}
	}

void CT_DataOpenFontRasterizerContext::DoCmdDestructor()
	{
	DestroyData();
	}
