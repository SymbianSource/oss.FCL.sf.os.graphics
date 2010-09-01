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

This contains CT_DataMeasureTextInput
*/

#include "T_DataMeasureTextInput.h"


/*@{*/
///	Parameters
_LIT(KValue,									"value");
_LIT(KFlag,										"flag%d");

///	CFont::TMeasureTextInput::TFlags
_LIT(KEFVisualOrder,							"EFVisualOrder");
_LIT(KEFVisualOrderRightToLeft,					"EFVisualOrderRightToLeft");
_LIT(KEFIncludePenPositionInBoundsCheck,		"EFIncludePenPositionInBoundsCheck");

///	Commands
_LIT(KCmdNew,									"new");
_LIT(KCmdDestructor,							"~");
_LIT(KCmdSetCharJustExcess,						"iCharJustExcess");
_LIT(KCmdSetCharJustNum,						"iCharJustNum");
_LIT(KCmdSetDirection,							"iDirection");
_LIT(KCmdSetEndInputChar,						"iEndInputChar");
_LIT(KCmdSetFlags,								"iFlags");
_LIT(KCmdSetMaxAdvance,							"iMaxAdvance");
_LIT(KCmdSetMaxBounds,							"iMaxBounds");
_LIT(KCmdSetStartInputChar,						"iStartInputChar");
_LIT(KCmdSetWordJustExcess,						"iWordJustExcess");
_LIT(KCmdSetWordJustNum,						"iWordJustNum");
/*@}*/


/**
* Two phase constructor
*/
CT_DataMeasureTextInput* CT_DataMeasureTextInput::NewL()
	{
	CT_DataMeasureTextInput* ret = new (ELeave) CT_DataMeasureTextInput();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataMeasureTextInput::CT_DataMeasureTextInput()
:	CDataWrapperBase()
,	iMeasureTextInput(NULL)
	{
	}


/**
* Protected second phase construction
*/
void CT_DataMeasureTextInput::ConstructL()
	{
	}


/**
* Destructor.
*/
CT_DataMeasureTextInput::~CT_DataMeasureTextInput()
	{
	DestroyData();
	}


/**
* cleanup implementation.
*/
void CT_DataMeasureTextInput::DestroyData()
	{
	delete iMeasureTextInput;
	iMeasureTextInput = NULL;
	}


/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataMeasureTextInput::GetObject()
	{
	return iMeasureTextInput;
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
TBool CT_DataMeasureTextInput::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNew();
		}
	else if (aCommand == KCmdDestructor)
		{
		DoCmdDestructor();
		}
	else if (aCommand == KCmdSetCharJustExcess)
		{
		DoCmdSetCharJustExcess(aSection);
		}
	else if (aCommand == KCmdSetCharJustNum)
		{
		DoCmdSetCharJustNum(aSection);
		}
	else if (aCommand == KCmdSetDirection)
		{
		DoCmdSetDirection(aSection);
		}
	else if (aCommand == KCmdSetEndInputChar)
		{
		DoCmdSetEndInputChar(aSection);
		}
	else if (aCommand == KCmdSetFlags)
		{
		DoCmdSetFlags(aSection);
		}
	else if (aCommand == KCmdSetMaxAdvance)
		{
		DoCmdSetMaxAdvance(aSection);
		}
	else if (aCommand == KCmdSetMaxBounds)
		{
		DoCmdSetMaxBounds(aSection);
		}
	else if (aCommand == KCmdSetStartInputChar)
		{
		DoCmdSetStartInputChar(aSection);
		}
	else if (aCommand == KCmdSetWordJustExcess)
		{
		DoCmdSetWordJustExcess(aSection);
		}
	else if (aCommand == KCmdSetWordJustNum)
		{
		DoCmdSetWordJustNum(aSection);
		}
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** Creates an instance of TMeasureTextInput structure */
void CT_DataMeasureTextInput::DoCmdNew()
	{
	INFO_PRINTF1(_L("Creates an instance of TMeasureTextInput structure"));

	// cleanup if any
	delete iMeasureTextInput;
	iMeasureTextInput = NULL;

	// call new operator
	TRAPD(err, iMeasureTextInput = new (ELeave) CFont::TMeasureTextInput());

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creating an instance: %d"), err);
		SetError(err);
		}
	}


/** Destructor */
void CT_DataMeasureTextInput::DoCmdDestructor()
	{
	DestroyData();
	}

/** Sets TMeasureTextInputData::iStartInputChar */
void CT_DataMeasureTextInput::DoCmdSetStartInputChar(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iStartInputChar"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iStartInputChar = value;
		}
	}


/** Sets TMeasureTextInputData::iEndInputChar */
void CT_DataMeasureTextInput::DoCmdSetEndInputChar(const TDesC& aSection)
	{
	INFO_PRINTF2(_L("Sets TMeasureTextInputData::iEndInputChar %S"), &aSection);

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iEndInputChar = value;
		}
	}


/** Sets TMeasureTextInputData::iDirection */
void CT_DataMeasureTextInput::DoCmdSetDirection(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iDirection"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iDirection = value;
		}
	}


/** Sets TMeasureTextInputData::iFlags */
void CT_DataMeasureTextInput::DoCmdSetFlags(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iFlags"));

	// get value from parameters
	if ( !ReadFlags(aSection, iMeasureTextInput->iFlags) )
		{
		ERR_PRINTF2(_L("No %S"), &aSection);
		SetBlockResult(EFail);
		}
	}


/** Sets TMeasureTextInputData::iMaxAdvance */
void CT_DataMeasureTextInput::DoCmdSetMaxAdvance(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iMaxAdvance"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iMaxAdvance = value;
		}
	}


/** Sets TMeasureTextInputData::iMaxBounds */
void CT_DataMeasureTextInput::DoCmdSetMaxBounds(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iMaxBounds"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iMaxBounds = value;
		}
	}


/** Sets TMeasureTextInputData::iCharJustNum */
void CT_DataMeasureTextInput::DoCmdSetCharJustNum(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iCharJustNum"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iCharJustNum = value;
		}
	}


/** Sets TMeasureTextInputData::iCharJustExcess */
void CT_DataMeasureTextInput::DoCmdSetCharJustExcess(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iCharJustExcess"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iCharJustExcess = value;
		}
	}


/** Sets TMeasureTextInputData::iWordJustNum */
void CT_DataMeasureTextInput::DoCmdSetWordJustNum(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iWordJustNum"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iWordJustNum = value;
		}
	}


/** Sets TMeasureTextInputData::iWordJustExcess */
void CT_DataMeasureTextInput::DoCmdSetWordJustExcess(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Sets TMeasureTextInputData::iWordJustExcess"));

	// get value from parameters
	TInt	value;
	if (GetValueFromConfig(aSection, value))
		{
		iMeasureTextInput->iWordJustExcess = value;
		}
	}


///////////////////////////////// UTIL METHODS //////////////////////////////////////


/**
* Utility method that fetches a int value from parameters
*/
TBool CT_DataMeasureTextInput::GetValueFromConfig(const TDesC& aSection, TInt& aValue)
	{
	TBool	ret=GetIntFromConfig(aSection, KValue(), aValue);
	if ( !ret )
		{
		ERR_PRINTF2(_L("No %S"), &KValue());
		SetBlockResult(EFail);
		}

	return ret;
	}

/**
 ReadFlags - read in bitwise measeure text input flags
 @internalComponent
 @param aSection - main entry in .ini file
 @param aFlags - bitwise flags output
 @return ETrue if data is valid in the ini file
 @pre None
 @post 2nd stage construction complete
*/
TBool CT_DataMeasureTextInput::ReadFlags(const TDesC& aSection, TUint16& aFlags)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TInt								index=0;
	TBool								moreData=ETrue;
	TPtrC								flagString;
	TBool								ret=ETrue;

	aFlags=0;
	while ( moreData && ret )
		{
		tempStore.Format(KFlag(), &aSection, ++index);
		if ( GetStringFromConfig(aSection, tempStore, flagString) )
			{
			TInt	flag=0;

			if ( flagString==KEFVisualOrder )
				{
				flag=CFont::TMeasureTextInput::EFVisualOrder;
				}
			else if ( flagString==KEFVisualOrderRightToLeft )
				{
				flag=CFont::TMeasureTextInput::EFVisualOrderRightToLeft;
				}
			else if ( flagString==KEFIncludePenPositionInBoundsCheck )
				{
				flag=CFont::TMeasureTextInput::EFIncludePenPositionInBoundsCheck;
				}
			else
				{
				ret=GetIntFromConfig(aSection, tempStore, flag);
				}

			if ( ret )
				{
				aFlags |= flag;
				}
			}
		else
			{
			moreData=EFalse;
			}
		}

	return ret;
	}
