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

This contains CT_DataFbsColor256BitmapUtil
*/

//	User includes
#include "T_DataFbsColor256BitmapUtil.h"
#include "T_GraphicsUtil.h"

/*@{*/
///	Parameters
_LIT(KDither,							"dither");
_LIT(KBitmapFrom,						"bitmap_from");
_LIT(KBitmapTo,							"bitmap_to");
_LIT(KPalette,							"palette");

///	Enumeration parameter values
_LIT(KDitherNoDither,					"ENoDither");
_LIT(KDitherFloydSteinberg,				"EFloydSteinberg");

///	Commands
_LIT(KCmdNewL,							"NewL");
_LIT(KCmdCopyBitmap,					"CopyBitmap");
_LIT(KCleanup,							"~");
/*@}*/


/**
* Two phase constructor
*/
CT_DataFbsColor256BitmapUtil* CT_DataFbsColor256BitmapUtil::NewL()
	{
	CT_DataFbsColor256BitmapUtil* ret = new (ELeave) CT_DataFbsColor256BitmapUtil();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}


/**
* Protected constructor. First phase construction
*/
CT_DataFbsColor256BitmapUtil::CT_DataFbsColor256BitmapUtil()
:	CDataWrapperBase()
,	iFbsColor256BitmapUtil(NULL)
	{
	}


void CT_DataFbsColor256BitmapUtil::ConstructL()
	{
	}


CT_DataFbsColor256BitmapUtil::~CT_DataFbsColor256BitmapUtil()
	{
	DestroyData();
	}


/**
* Contains cleanup implementation
*/
void CT_DataFbsColor256BitmapUtil::DestroyData()
	{
	delete iFbsColor256BitmapUtil;
	iFbsColor256BitmapUtil = NULL;
	}


/**
* Return a pointer to the object that the data wraps
*
* @return pointer to the object that the data wraps
*/
TAny* CT_DataFbsColor256BitmapUtil::GetObject()
	{
	return iFbsColor256BitmapUtil;
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
TBool CT_DataFbsColor256BitmapUtil::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool retVal = ETrue;

	if (aCommand == KCmdNewL)
		{
		DoCmdNewL(aSection);
		}
	else if (aCommand == KCmdCopyBitmap)
		{
		DoCmdCopyBitmapL(aSection);
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


////////////////// COMMANDS IMPLEMENTATION  ////////////////////////

/** CFbsColor256BitmapUtil::NewL() call */
void CT_DataFbsColor256BitmapUtil::DoCmdNewL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("CFbsColor256BitmapUtil::NewL() call"));

	// cleanup if any
	delete iFbsColor256BitmapUtil;
	iFbsColor256BitmapUtil = NULL;

	// get CPalette object passed as a parameter
	CPalette*	palette = NULL;
	CT_GraphicsUtil::GetPaletteL(*this, aSection, KPalette(), palette);

	// call NewL()
	TRAPD(err, iFbsColor256BitmapUtil = CFbsColor256BitmapUtil::NewL(palette));

	// check error code
	if (err != KErrNone)
		{
		ERR_PRINTF2(_L("CFbsColor256BitmapUtil::NewL error: %d"), err);
		SetError(err);
		}
	}


/** CFbsColor256BitmapUtil::CopyBitmap() call */
void CT_DataFbsColor256BitmapUtil::DoCmdCopyBitmapL(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("CFbsColor256BitmapUtil::CopyBitmap() call"));

	// get source CFbsBitmap passed as a parameter
	CFbsBitmap*	fbsBitmapFrom = NULL;
	CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KBitmapFrom(), fbsBitmapFrom);

	// get destination CFbsBitmap passed as a parameter
	CFbsBitmap*	fbsBitmapTo = NULL;
	CT_GraphicsUtil::GetFbsBitmapL(*this, aSection, KBitmapTo(), fbsBitmapTo);

	// get dither mode from parameters
	CFbsColor256BitmapUtil::TDither	dither;
	if (!ReadDither(*this, aSection, dither))
		{
		ERR_PRINTF1(_L("No dither"));
		SetBlockResult(EFail);
		}
	else
		{
		// call CopyBitmap()
		TInt	err = iFbsColor256BitmapUtil->CopyBitmap(fbsBitmapTo, fbsBitmapFrom, dither);

		// check error
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("CopyBitmap failed with error %d"), err);
			SetError(err);
			}
		}
	}

//////////////////// UTIL METHODS ////////////////////////////


TBool CT_DataFbsColor256BitmapUtil::ReadDither(CDataWrapper& aDataWrapper, const TDesC& aSection, CFbsColor256BitmapUtil::TDither& aDither)
	{
	// Read dither from INI file
	TPtrC	ditherStr;
	TBool	found=aDataWrapper.GetStringFromConfig(aSection, KDither(), ditherStr);
	if ( found )
		{
		if (ditherStr == KDitherNoDither)
			{
			aDither = CFbsColor256BitmapUtil::ENoDither;
			}
		else if (ditherStr == KDitherFloydSteinberg)
			{
			aDither = CFbsColor256BitmapUtil::EFloydSteinberg;
			}
		else
			{
        	TInt	dither = 0;
        	found = aDataWrapper.GetIntFromConfig(aSection, KDither(), dither);
			if ( found )
				{
	        	aDither = (CFbsColor256BitmapUtil::TDither) dither;
				}
			}
		}

	return found;
	}
