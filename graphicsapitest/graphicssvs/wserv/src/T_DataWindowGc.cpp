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

#include "T_DataWindowGc.h"
#include "T_GraphicsUtil.h"

/*@{*/
_LIT(KDataClassname,	"CWindowGc");

//Commands
_LIT(KCmdnew,						"new");
_LIT(KCmdDestructor,				"~CWindowGc");
_LIT(KCmdDestructorGeneral,			"~");
_LIT(KCmdConstruct,					"Construct");
_LIT(KCmdActivate,					"Activate");
_LIT(KCmdDeactivate,				"Deactivate");
_LIT(KCmdBitBlt,					"BitBlt");
_LIT(KCmdBitBltMasked,				"BitBltMasked");
_LIT(KCmdSetDitherOrigin,			"SetDitherOrigin");
_LIT(KCmdSetOpaque,					"SetOpaque");
_LIT(KCmdDrawWsGraphic,				"DrawWsGraphic");

// Fields
_LIT(KFldData,						"data");
_LIT(KFldWsga,						"wsga");
_LIT(KFldDestRect,					"destrect");
_LIT(KFldDestination,				"destination");
_LIT(KFldDevice,					"device");
_LIT(KFldId,						"id");
_LIT(KFldInvertMask,				"invertmask");
_LIT(KFldMaskBitmap,				"maskbitmap");
_LIT(KFldPoint,						"point");
_LIT(KFldSource,					"source");
_LIT(KFldSourceRect,				"sourcerect");
_LIT(KFldWsBitmap,					"wsbitmap");
_LIT(KFldDrawOpaque,				"drawOpaque");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWindowGc* CT_DataWindowGc::NewL()
	{
	CT_DataWindowGc*	ret=new (ELeave) CT_DataWindowGc();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataWindowGc::CT_DataWindowGc()
:	CT_DataBitmapContext()
,	iWindowGc(NULL)
	{
	}

void CT_DataWindowGc::ConstructL()
	{
	}

CT_DataWindowGc::~CT_DataWindowGc()
	{
	DestroyData();
	}

void CT_DataWindowGc::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iWindowGc = static_cast<CWindowGc*> (aAny);
	}

void CT_DataWindowGc::DisownObjectL()
	{
	iWindowGc = NULL;
	}

void CT_DataWindowGc::DestroyData()
	{
	delete iWindowGc;
	iWindowGc=NULL;
	}

MWsClientClass* CT_DataWindowGc::GetClientClass() const
	{
	return iWindowGc;
	}

CBitmapContext* CT_DataWindowGc::GetBitmapContext() const
	{
	return iWindowGc;
	}

CGraphicsContext* CT_DataWindowGc::GetGraphicsContext() const
	{
	return iWindowGc;
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
TBool CT_DataWindowGc::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructor || aCommand==KCmdDestructorGeneral )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnew || aCommand==KDataClassname )
		{
		DoCmdnewL(aSection);
		}
	else if ( aCommand==KCmdConstruct )
		{
		DoCmdConstruct();
		}
	else if ( aCommand==KCmdActivate )
		{
		DoCmdActivateL(aSection);
		}
	else if ( aCommand==KCmdDeactivate )
		{
		DoCmdDeactivate();
		}
	else if ( aCommand==KCmdBitBlt )
		{
		DoCmdBitBltL(aCommand, aSection, aAsyncErrorIndex);
		}
	else if ( aCommand==KCmdBitBltMasked )
		{
		DoCmdBitBltMaskedL(aCommand, aSection, aAsyncErrorIndex);
		}
	else if ( aCommand==KCmdSetDitherOrigin )
		{
		DoCmdSetDitherOrigin(aSection);
		}
	else if ( aCommand==KCmdSetOpaque )
		{
		DoCmdSetOpaque(aSection);
		}
	else if ( aCommand==KCmdDrawWsGraphic )
		{
		DoCmdDrawWsGraphicL(aSection);
		}
	else if ( CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex) )
		{
		}
	else
		{
		ret=CT_DataWsClientClass::DoCommandL(*this, aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataWindowGc::DoCmdnewL(const TDesC& aSection)
	{
	DestroyData();

	// Get test data for command input parameter(s)
	CWsScreenDevice*	device = NULL;
	if ( !CT_GraphicsUtil::GetWsScreenDeviceL(*this, aSection, KFldDevice, device) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldDevice);
		SetBlockResult(EFail);
		}
	else
		{
		INFO_PRINTF1(_L("execute new CWindowGc(CWsScreenDevice)"));
		TRAPD( err, iWindowGc = new (ELeave) CWindowGc(device) );
		if ( err != KErrNone )
			{
			ERR_PRINTF2(KLogError, err);
			SetError(err);
			}
		}
	}

void CT_DataWindowGc::DoCmdDestructor()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute ~CWindowGc()"));
	delete iWindowGc;
	iWindowGc=NULL;
	}


void CT_DataWindowGc::DoCmdConstruct()
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Construct()"));
	TInt	returnCode = iWindowGc->Construct();

	// Check the command return code, if !=KErrNone then stop this command
	if ( returnCode!=KErrNone )
		{
		ERR_PRINTF2(KLogError, returnCode);
		SetError(returnCode);
		}
	}


void CT_DataWindowGc::DoCmdActivateL(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	RDrawableWindow	*device = NULL;

	if ( !CT_GraphicsUtil::GetDrawableWindowL(*this, aSection, KFldDevice, device) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldDevice);
		SetBlockResult(EFail);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute Activate(RDrawableWindow)"));
		iWindowGc->Activate(*device);

		// No command return value and output parameter to display and check
		}
	}


void CT_DataWindowGc::DoCmdDeactivate()
	{
	// No command input parameter to process

	// Execute command and log parameters
	INFO_PRINTF1(_L("execute Deactivate()"));
	iWindowGc->Deactivate();

	// No command return value and output parameter to display and check
	}

void CT_DataWindowGc::DoCmdBitBltL(const TDesC& aCommand, const TDesC& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	dataOk=ETrue;

	CWsBitmap*	wsBitmap=NULL;
	if ( !CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldWsBitmap, wsBitmap) )
		{
		CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}
	else
		{
		TPoint		point;
		if ( GetPointFromConfig(aSection, KFldPoint, point) )
			{
			if ( dataOk )
				{
				// Execute command and log parameters
				INFO_PRINTF1(_L("execute BitBlt(TPoint, CWsBitmap*)"));
				iWindowGc->BitBlt(point, wsBitmap);
				}
			}
		else
			{
			TPoint	destination;
			if ( !GetPointFromConfig(aSection, KFldDestination, destination) )
				{
				dataOk=EFalse;
				ERR_PRINTF2(KLogMissingParameter, &KFldDestination());
				SetBlockResult(EFail);
				}
    	
			TRect	source;
			if ( !GetRectFromConfig(aSection, KFldSource, source) )
				{
				dataOk=EFalse;
				ERR_PRINTF2(KLogMissingParameter, &KFldSource());
				SetBlockResult(EFail);
				}
    	
			if ( dataOk )
				{
				// Execute command and log parameters
				INFO_PRINTF1(_L("execute BitBlt(TRect, CWsBitmap*, TRect)"));
				iWindowGc->BitBlt(destination, wsBitmap, source);
				}
			}
		}
	}


void CT_DataWindowGc::DoCmdBitBltMaskedL(const TDesC& aCommand, const TDesC& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	dataOk=ETrue;

	CWsBitmap*	wsBitmap=NULL;
	if ( !CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldWsBitmap, wsBitmap) )
		{
		CT_DataBitmapContext::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}
	else
		{
		// Get test data for command input parameter(s)
		TPoint	point;
		if ( !GetPointFromConfig(aSection, KFldPoint, point) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldPoint());
			SetBlockResult(EFail);
			}
    	
		TRect	sourceRect;
		if ( !GetRectFromConfig(aSection, KFldSourceRect, sourceRect) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldSourceRect());
			SetBlockResult(EFail);
			}
    	
		TBool	invertMask;
		if ( !GetBoolFromConfig(aSection, KFldInvertMask, invertMask) )
			{
			dataOk=EFalse;
			ERR_PRINTF2(KLogMissingParameter, &KFldInvertMask());
			SetBlockResult(EFail);
			}
    	
		if ( dataOk )
			{
			CWsBitmap*	maskBitmap=NULL;
			CT_GraphicsUtil::GetWsBitmapL(*this, aSection, KFldMaskBitmap, maskBitmap);
    	
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute BitBltMasked(TPoint, CWsBitmap*, TRect, CWsBitmap*, TBool)"));
			iWindowGc->BitBltMasked(point, wsBitmap, sourceRect, maskBitmap, invertMask);
			}
		}
	}

void CT_DataWindowGc::DoCmdSetDitherOrigin(const TDesC& aSection)
	{
	TPoint	point;
	if ( GetPointFromConfig(aSection, KFldPoint, point) )
		{
		INFO_PRINTF1(_L("CWindowGc::SetDitherOrigin"));
		iWindowGc->SetDitherOrigin(point);
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldPoint());
		SetBlockResult(EFail);
		}
	}

void CT_DataWindowGc::DoCmdSetOpaque(const TDesC& aSection)
	{
	TBool	drawOpaque=ETrue;
	if(	GetBoolFromConfig(aSection, KFldDrawOpaque(), drawOpaque))
		{
		INFO_PRINTF1(_L("CWindowGc::SetOpaque"));
		iWindowGc->SetOpaque(drawOpaque);
		}
	else
		{
		ERR_PRINTF2(_L("Missing parameter %S"), &KFldDrawOpaque());
		SetBlockResult(EFail);
		}
	}

void CT_DataWindowGc::DoCmdDrawWsGraphicL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;

	// Get test data for command input parameter(s)
	TWsGraphicId	datId(0);
	if ( !GetWsGraphicIdFromConfigL(aSection, KFldId, datId) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldId);
		SetBlockResult(EFail);
		}

	TRect	datDestRect;
	if ( !GetRectFromConfig(aSection, KFldDestRect, datDestRect) )
		{
		dataOk=EFalse;
		ERR_PRINTF2(KLogMissingParameter, &KFldDestRect);
		SetBlockResult(EFail);
		}

	if ( dataOk )
		{
		RWsGraphicMsgBuf*	pData = NULL;
		TWsGraphicAnimation* pWsga = NULL;
		if ( CT_GraphicsUtil::GetWsGraphicMsgBufL(*this, aSection, KFldData, pData) )
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawWsGraphic(TWsGraphicId, TRect, TDesC8)"));
			iWindowGc->DrawWsGraphic(datId, datDestRect, pData->Pckg());
			}
		else if( CT_GraphicsUtil::GetWsGraphicAnimationL(*this, aSection, KFldWsga, pWsga) )
			{
			INFO_PRINTF1(_L("execute DrawWsGraphic(TWsGraphicId, TRect, TDesC8)"));
			iWindowGc->DrawWsGraphic(datId, datDestRect, pWsga->Pckg());
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(_L("execute DrawWsGraphic(TWsGraphicId, TRect)"));
			iWindowGc->DrawWsGraphic(datId, datDestRect);
			}
		// No command return value and output parameter to display and check
		}
	}


