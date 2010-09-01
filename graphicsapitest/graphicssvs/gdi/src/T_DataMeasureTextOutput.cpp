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

This contains CT_DataMeasureTextOutput
*/

#include "T_DataMeasureTextOutput.h"


/*@{*/
///	Parameters
_LIT(KValue,									"value");
_LIT(KRectAx,									"rect_ax");
_LIT(KRectAy,									"rect_ay");
_LIT(KRectBx,									"rect_bx");
_LIT(KRectBy,									"rect_by");
_LIT(KWidth,									"width");
_LIT(KHeight,									"height");

///	Commands
_LIT(KCmdNew,									"new");
_LIT(KCmdSetBounds,								"iBounds");
_LIT(KCmdSetChars,								"iChars");
_LIT(KCmdSetGlyphs,								"iGlyphs");
_LIT(KCmdSetGroups,								"iGroups");
_LIT(KCmdSetMaxGlyphSize,						"iMaxGlyphSize");
_LIT(KCmdSetSpaces,								"iSpaces");
_LIT(KCmdDestructor,							"~");



/*@}*/


/**
* Two phase constructor
*/
CT_DataMeasureTextOutput* CT_DataMeasureTextOutput::NewL()
	{
	CT_DataMeasureTextOutput* ret = new (ELeave) CT_DataMeasureTextOutput();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataMeasureTextOutput::CT_DataMeasureTextOutput()
:	CDataWrapperBase()
,	iMeasureTextOutput(NULL)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataMeasureTextOutput::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataMeasureTextOutput::~CT_DataMeasureTextOutput()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataMeasureTextOutput::DestroyData()
	{
	delete iMeasureTextOutput;
	iMeasureTextOutput = NULL;
	}


/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataMeasureTextOutput::GetObject()
	{
	return iMeasureTextOutput;
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
TBool CT_DataMeasureTextOutput::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNew();
		}
	else if (aCommand == KCmdSetBounds)
		{
		DoCmdSetBounds(aSection);
		}
	else if (aCommand == KCmdSetChars)
		{
		DoCmdSetChars(aSection);
		}
	else if (aCommand == KCmdSetGlyphs)
		{
		DoCmdSetGlyphs(aSection);
		}
	else if (aCommand == KCmdSetGroups)
		{
		DoCmdSetGroups(aSection);
		}
	else if (aCommand == KCmdSetMaxGlyphSize)
		{
		DoCmdSetMaxGlyphSize(aSection);
		}
	else if (aCommand == KCmdSetSpaces)
		{
		DoCmdSetSpaces(aSection);
		}
	else if (aCommand == KCmdDestructor)
		{
		DestroyData();
		}
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Creates an instance of TMeasureTextOutput structure */
void CT_DataMeasureTextOutput::DoCmdNew()
	{
	INFO_PRINTF1(_L("Creates an instance of TMeasureTextOutput structure"));

	// cleanup if any
	DestroyData();

	// call new operator
	TRAPD(err, iMeasureTextOutput = new (ELeave) CFont::TMeasureTextOutput());

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating an instance: %d"), err);
		SetError(err);
		}
	}


/** Sets TMeasureTextOutputData::iChars */
void CT_DataMeasureTextOutput::DoCmdSetChars(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextOutputData::iChars"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(*this,aSection, value))
		{
		iMeasureTextOutput->iChars = value;
		}
	}


/** Sets TMeasureTextOutputData::iGlyphs */
void CT_DataMeasureTextOutput::DoCmdSetGlyphs(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextOutputData::iGlyphs"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(*this, aSection, value))
		{
		iMeasureTextOutput->iGlyphs = value;
		}
	}


/** Sets TMeasureTextOutputData::iGroups */
void CT_DataMeasureTextOutput::DoCmdSetGroups(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextOutputData::iGroups"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(*this, aSection, value))
		{
		iMeasureTextOutput->iGroups = value;
		}
	}


/** Sets TMeasureTextOutputData::iSpaces */
void CT_DataMeasureTextOutput::DoCmdSetSpaces(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextOutputData::iSpaces"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(*this,  aSection, value))
		{
		iMeasureTextOutput->iSpaces = value;
		}
	}


/** Sets TMeasureTextOutputData::iBounds */
void CT_DataMeasureTextOutput::DoCmdSetBounds(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextOutputData::iBounds"));

	TBool	dataOk = ETrue;

	// get rect Ax from parameters
	TInt	rectAx;
	if(!GetIntFromConfig(aSection, KRectAx(), rectAx))
		{
		ERR_PRINTF2(_L("No %S"), &KRectAx());
		SetBlockResult(EFail);
		dataOk = EFalse;
		}

	// get rect Ay from parameters
	TInt	rectAy;
	if(!GetIntFromConfig(aSection, KRectAy(), rectAy))
		{
		ERR_PRINTF2(_L("No %S"), &KRectAy());
		SetBlockResult(EFail);
		dataOk = EFalse;
		}

	// get rect Bx from parameters
	TInt	rectBx;
	if(!GetIntFromConfig(aSection, KRectBx(), rectBx))
		{
		ERR_PRINTF2(_L("No %S"), &KRectBx());
		SetBlockResult(EFail);
		dataOk = EFalse;
		}
	
	// get rect By from parameters
	TInt	rectBy;
	if(!GetIntFromConfig(aSection, KRectBy(), rectBy))
		{
		ERR_PRINTF2(_L("No %S"), &KRectBy());
		SetBlockResult(EFail);
		dataOk = EFalse;
		}

	// set the field
	if (dataOk)
		{
		iMeasureTextOutput->iBounds.SetRect(rectAx, rectAy, rectBx, rectBy);
		}
	}


/** Sets TMeasureTextOutputData::iMaxGlyphSize */
void CT_DataMeasureTextOutput::DoCmdSetMaxGlyphSize(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextOutputData::iMaxGlyphSize"));

	TBool	dataOk = ETrue;

	// get width from parameters
	TInt	width;
	if(!GetIntFromConfig(aSection, KWidth(), width))
		{
		ERR_PRINTF2(_L("No %S"), &KWidth());
		SetBlockResult(EFail);
		dataOk = EFalse;
		}
	
	// get height from parameters
	TInt	height; 
	if(!GetIntFromConfig(aSection, KHeight(), height))
		{
		ERR_PRINTF2(_L("No %S"), &KHeight());
		SetBlockResult(EFail);
		dataOk = EFalse;
		}

	// set the field
	if (dataOk)
		{
		iMeasureTextOutput->iMaxGlyphSize.iWidth = width;
		iMeasureTextOutput->iMaxGlyphSize.iHeight = height;
		}
	}



///////////////////////////////// UTIL METHODS //////////////////////////////////////


/**
* Utility method that fetches a int value from parameters
*/
TBool CT_DataMeasureTextOutput::GetValueFromConfig(CDataWrapper& iInputStep, const TDesC& aSection, TInt& aValue)
	{
	TBool	ret=iInputStep.GetIntFromConfig(aSection, KValue(), aValue);
	if (!ret)
		{
		iInputStep.ERR_PRINTF2(_L("No %S"), &KValue());
		iInputStep.SetBlockResult(EFail);
		}

	return ret;
	}
