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

This contains CT_FBServAPIServer inline functions
*/


_LIT(KRFbsSessionData, 			"RFbsSession");
_LIT(KCFbsFontData, 			"CFbsFont");
_LIT(KCFbsBitmapData, 			"CFbsBitmap");
_LIT(KTBitmapUtilData, 			"TBitmapUtil");
_LIT(KCFbsTypefaceStoreData,	"CFbsTypefaceStore");
_LIT(KCFbsColor256BitmapUtil,	"CFbsColor256BitmapUtil");
_LIT(KCPalette,					"CPalette");
_LIT(KCFbsBitmapDevice,			"CFbsBitmapDevice");
_LIT(KCFbsScreenDevice,			"CFbsScreenDevice");
_LIT(KTAlgStyle,				"TAlgStyle");
_LIT(KTMeasureTextInput,		"TMeasureTextInput");
_LIT(KTMeasureTextOutput,		"TMeasureTextOutput");

inline CT_FBServAPIServer::CT_FBServAPIBlock::CT_FBServAPIBlock()
    {
    }

inline CT_FBServAPIServer::CT_FBServAPIBlock::~CT_FBServAPIBlock()
    {
    }

inline CDataWrapper* CT_FBServAPIServer::CT_FBServAPIBlock::CreateDataL( const TDesC& aData )
	{
	CDataWrapper* wrapper = NULL;
   	if (aData == KRFbsSessionData())
   		{
   		wrapper = CT_DataFbsSession::NewL();
   		}
   	else if (aData == KCFbsFontData())
   		{
   		wrapper = CT_DataFbsFont::NewL();
   		}
   	else if (aData == KCFbsBitmapData())
   		{
   		wrapper = CT_DataFbsBitmap::NewL();
   		}
   	else if (aData == KTBitmapUtilData())
   		{
   		wrapper = CT_DataBitmapUtil::NewL();
   		}
   	else if (aData == KCFbsTypefaceStoreData())
   		{
   		wrapper = CT_DataFbsTypefaceStore::NewL();
   		}
   	else if (aData == KCFbsColor256BitmapUtil())
   		{
   		wrapper = CT_DataFbsColor256BitmapUtil::NewL();
   		}
   	else if (aData == KCPalette())
   		{
   		wrapper = CT_DataPalette::NewL();
   		}
   	else if ( aData==KCFbsBitmapDevice() )
   		{
   		wrapper = CT_DataFbsBitmapDevice::NewL();
   		}
   	else if ( aData==KCFbsScreenDevice() )
   		{
   		wrapper = CT_DataFbsScreenDevice::NewL();
   		}
   	else if ( aData==KTAlgStyle() )
   		{
   		wrapper = CT_DataAlgStyle::NewL();
   		}
   	else if ( aData==KTMeasureTextInput() )
   		{
   		wrapper = CT_DataMeasureTextInput::NewL();
   		}
   	else if ( aData==KTMeasureTextOutput() )
   		{
   		wrapper = CT_DataMeasureTextOutput::NewL();
   		}
	return wrapper;
	}

inline CT_FBServAPIServer::CT_FBServAPIServer()
    {
    }

inline CT_FBServAPIServer::~CT_FBServAPIServer()
    {
    }

inline CTestBlockController*	CT_FBServAPIServer::CreateTestBlock()
	{
	return new CT_FBServAPIBlock();
	}
