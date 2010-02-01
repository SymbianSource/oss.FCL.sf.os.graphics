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

This contains CT_DataBitmapUtil
*/

//	User includes
#include "T_DataBitmapUtil.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Parameters
_LIT(KBitmap,								"bitmap");
_LIT(KPointX,								"point_x");
_LIT(KPointY,								"point_y");
_LIT(KOtherBitmapUtil,						"other_bitmap_util");
_LIT(KColor,								"color");
_LIT(KColorMask,							"colormask");

///	Commands
_LIT(KCmdNew,								"new");
_LIT(KCmdBegin,								"Begin");
_LIT(KCmdEnd,								"End");
_LIT(KCmdSetPixel,							"SetPixel");
_LIT(KCmdGetPixel,							"GetPixel");
_LIT(KCmdSetPos,							"SetPos");
_LIT(KCmdDecXPos,							"DecXPos");
_LIT(KCmdDecYPos,							"DecYPos");
_LIT(KCmdIncXPos,							"IncXPos");
_LIT(KCmdIncYPos,							"IncYPos");
_LIT(KCleanup,								"~");
/*@}*/

/**
* Two phase constructor
*/
CT_DataBitmapUtil* CT_DataBitmapUtil::NewL()
	{
	CT_DataBitmapUtil*	ret = new (ELeave) CT_DataBitmapUtil();
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataBitmapUtil::CT_DataBitmapUtil()
:	CDataWrapperBase()
,	iBitmapUtil(NULL)
	{
	}

/**
* Protected desctructor
*/
CT_DataBitmapUtil::~CT_DataBitmapUtil()
	{
	DestroyData();
	}

/**
* Contains cleanup implementation
*/
void CT_DataBitmapUtil::DestroyData()
	{
	delete iBitmapUtil;
	iBitmapUtil = NULL;
	}

TAny* CT_DataBitmapUtil::GetObject()
	{
	return iBitmapUtil;
	}

void CT_DataBitmapUtil::SetObjectL(TAny* aObject)
	{
	DestroyData();
	iBitmapUtil	= static_cast<TBitmapUtil*> (aObject);
	}
		
void CT_DataBitmapUtil::DisownObjectL() 
	{
	iBitmapUtil = NULL;
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
TBool CT_DataBitmapUtil::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	retVal = ETrue;

	if (aCommand == KCmdNew)
		{
		DoCmdNewL(aSection);
		}
	else if (aCommand == KCmdBegin)
		{
		DoCmdBeginL(aSection);
		}
	else if (aCommand == KCmdEnd)
		{
		DoCmdEnd();
		}
	else if (aCommand == KCmdSetPixel)
		{
		DoCmdSetPixelL(aSection);
		}
	else if (aCommand == KCmdGetPixel)
		{
		DoCmdGetPixel(aSection);
		}
	else if (aCommand == KCmdSetPos)
		{
		DoCmdSetPos(aSection);
		}
	else if (aCommand == KCmdDecXPos)
		{
		DoCmdDecXPos();
		}
	else if (aCommand == KCmdDecYPos)
		{
		DoCmdDecYPos();
		}
	else if (aCommand == KCmdIncXPos)
		{
		DoCmdIncXPos();
		}
	else if (aCommand == KCmdIncYPos)
		{
		DoCmdIncYPos();
		}
	else if (aCommand == KCleanup)
		{
		DestroyData();
		}
	else
		{
		retVal=EFalse;
		}

	return retVal;
	}



/** Creates a TBitmapUtil instance */
void CT_DataBitmapUtil::DoCmdNewL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Creates a TBitmapUtil instance"));

	// get fbsBitmap passed as a parameter
	CFbsBitmap*	fbsBitmap=NULL;
	CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KBitmap(), fbsBitmap);

	// cleanup if already created
	delete iBitmapUtil;
	iBitmapUtil = NULL;

	// call new operator
	TRAPD(err, iBitmapUtil = new (ELeave) TBitmapUtil(fbsBitmap));

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creation instance: %d"), err);
		SetError(err);
		}
	}


/** Calls TBitmapUtil::Begin() */
void CT_DataBitmapUtil::DoCmdBeginL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::Begin()"));
	TBool								dataOk=ETrue;

	// get position x-coordinate from parameters
	TInt	pointX;
	if(!GetIntFromConfig(aSection, KPointX(), pointX))
		{
		ERR_PRINTF2(_L("No %S"), &KPointX());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get position y-coordinate from parameters
	TInt	pointY;
	if(!GetIntFromConfig(aSection, KPointY(), pointY))
		{
		ERR_PRINTF2(_L("No %S"), &KPointY());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		// call Begin()
		// get bitmapUtil passed as a parameter
		CT_DataBitmapUtil*	bitmapUtilDataObject = CT_DataBitmapUtil::GetBitmapUtilDataObjectFromParameterL(*this, KOtherBitmapUtil(), aSection);

		if (bitmapUtilDataObject == NULL)
			{
			iBitmapUtil->Begin(TPoint(pointX, pointY));
			}
		else
			{
			TBitmapUtil*	otherBitmapUtil = static_cast<TBitmapUtil*>(bitmapUtilDataObject->GetObject());

			// call Begin()
			iBitmapUtil->Begin(TPoint(pointX, pointY), *otherBitmapUtil);
			}
		}
	}


/** Calls TBitmapUtil::End() */
void CT_DataBitmapUtil::DoCmdEnd()
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::End()"));

	// call End()
	iBitmapUtil->End();
	}


/** Calls TBitmapUtil::SetPixel() */
void CT_DataBitmapUtil::DoCmdSetPixelL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::SetPixel()"));

	// get bitmapUtil passed as a parameter
	CT_DataBitmapUtil*	bitmapUtilDataObject = CT_DataBitmapUtil::GetBitmapUtilDataObjectFromParameterL(*this, KOtherBitmapUtil(), aSection);

	if (bitmapUtilDataObject == NULL)
		{
		// get color from parameters
		TInt	color;
		if(!GetIntFromConfig(aSection, KColor(), color))
			{
			ERR_PRINTF2(_L("No %S"), &KColor());
			SetBlockResult(EFail);
			}
		else
			{
			// call SetPixel()
			iBitmapUtil->SetPixel(TUint32(color));
			}
		}
	else
		{
		TBitmapUtil* otherBitmapUtil = static_cast<TBitmapUtil*>(bitmapUtilDataObject->GetObject());

		// call SetPixel()
		iBitmapUtil->SetPixel(*otherBitmapUtil);
		}
	}


/** Calls TBitmapUtil::GetPixel() */
void CT_DataBitmapUtil::DoCmdGetPixel(const TDesC& aSection)
	{
	// call GetPixel()
	TUint32	color = iBitmapUtil->GetPixel();
	INFO_PRINTF2(_L("Calls TBitmapUtil::GetPixel() %d"), color);

	TInt	colorMask;
	if ( GetHexFromConfig(aSection, KColorMask(), colorMask) )
		{
		color &= colorMask;
		INFO_PRINTF2(_L("Color after masking %d"), color);
		}

	// get expected color from parameters
	TInt	expectedColor;
	if(GetIntFromConfig(aSection, KColor(), expectedColor))
		{
		// check that the value is as expected
		if (color != (TUint32)expectedColor )
			{
			ERR_PRINTF3(_L("The value is not as expected! expected: %d, actual: %d"), expectedColor, color);
			SetBlockResult(EFail);
			}
		}
	}

/** Calls TBitmapUtil::SetPos() */
void CT_DataBitmapUtil::DoCmdSetPos(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::SetPos()"));
	TBool								dataOk=ETrue;

	// get position x-coordinate from parameters
	TInt	pointX;
	if(!GetIntFromConfig(aSection, KPointX(), pointX))
		{
		ERR_PRINTF2(_L("No %S"), &KPointX());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	// get position y-coordinate from parameters
	TInt	pointY;
	if(!GetIntFromConfig(aSection, KPointY(), pointY))
		{
		ERR_PRINTF2(_L("No %S"), &KPointY());
		SetBlockResult(EFail);
		dataOk=EFalse;
		}

	if ( dataOk )
		{
		// call SetPos()
		iBitmapUtil->SetPos(TPoint(pointX, pointY));
		}
	}


/** Calls TBitmapUtil::DecXPos() */
void CT_DataBitmapUtil::DoCmdDecXPos()
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::DecXPos()"));

	// call DecXPos()
	iBitmapUtil->DecXPos();
	}


/** Calls TBitmapUtil::DecYPos() */
void CT_DataBitmapUtil::DoCmdDecYPos()
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::DecYPos()"));

	// call DecYPos()
	iBitmapUtil->DecYPos();
	}


/** Calls TBitmapUtil::IncXPos() */
void CT_DataBitmapUtil::DoCmdIncXPos()
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::IncXPos()"));

	// call IncXPos()
	iBitmapUtil->IncXPos();
	}


/** Calls TBitmapUtil::IncYPos() */
void CT_DataBitmapUtil::DoCmdIncYPos()
	{
	INFO_PRINTF1(_L("Calls TBitmapUtil::IncYPos()"));

	// call IncYPos()
	iBitmapUtil->IncYPos();
	}



///////////////////////////////// UTIL METHODS //////////////////////////////////////


/**
* Utility method that fetches CT_DataBitmapUtil pointer by command parameter name from INI-file.
*/
CT_DataBitmapUtil* CT_DataBitmapUtil::GetBitmapUtilDataObjectFromParameterL(CDataWrapper& aDataWrapper, const TDesC& aParameterName, const TDesC& aSection)
	{
	// get AlgStyleData object from parameters
	CT_DataBitmapUtil*	ret=NULL;

	TPtrC bitmapUtilDataObjectName;
	// get BitmapUtilData object from parameters
	if ( aDataWrapper.GetStringFromConfig(aSection, aParameterName, bitmapUtilDataObjectName) )
		{
		// check that the data object was found
		ret=static_cast<CT_DataBitmapUtil*>(aDataWrapper.GetDataWrapperL(bitmapUtilDataObjectName));
		}

	return ret;
	}
