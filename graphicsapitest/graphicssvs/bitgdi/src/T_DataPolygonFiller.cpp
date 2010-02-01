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


#include "T_DataPolygonFiller.h"
#include "T_GraphicsUtil.h"

_LIT(KDataClassname,								"CPolygonFiller");

///	Fields
_LIT(KFldScanLine,									"scanline");
_LIT(KFldPointList,									"pointlist");
_LIT(KFldFillRule,									"fillrule");
_LIT(KFldUsage,										"usage");
_LIT(KFldnumPoints,									"numpoints");
_LIT(KFldExpectedScanLine,							"expectedscanline");
_LIT(KFldExpectedStart,								"expectedstart");
_LIT(KFldExpectedEnd,								"expectedend");
_LIT(KFldExpectedExists,							"expectedexists");
///	Commands
_LIT(KCmdnewL,										"new");
_LIT(KCmdConstructL,								"Construct");
_LIT(KCmdDestructorGeneral,							"~");
_LIT(KCmdDestructor,								"~CPolygonFiller");
_LIT(KCmdReset,										"Reset");
_LIT(KCmdGetNextPixelRun,							"GetNextPixelRun");
_LIT(KCmdGetNextPixelRunOnSpecifiedScanLine,		"GetNextPixelRunOnSpecifiedScanLine");

//Log
_LIT(KLogErrNum,									"Error=%d");
_LIT(KLogInfoCmdnewL, 						    	"execute new CPolygonFiller()");
_LIT(KLogMissingParameter, 							"Missing parameter '%S'");
_LIT(KLogInfoCmdDestructor1, 						"execute ~CPolygonFiller()");
_LIT(KLogInfoCmdConstruct1,							"execute Construct(TPoint*, TInt, TFillRule,TUsage)");
_LIT(KLogInfoCmdConstruct2,							"execute Construct(CArrayFix<TPoint>*, TFillRule,TUsage)");
_LIT(KLogInfoCmdReset,								"execute Reset()");
_LIT(KLogInfoCmdGetNextPixelRun,					"execute GetNextPixelRun()");
_LIT(KLogInfoCmdGetNextPixelRunOnSpecifiedScanLine,	"execute GetNextPixelRunOnSpecifiedScanLine()");
_LIT(KLogInfoNotCheckThePosition,   			    "Won't check the position on the scan line in the test case");
CT_DataPolygonFiller* CT_DataPolygonFiller::NewL()
	{
	CT_DataPolygonFiller*	ret = new (ELeave) CT_DataPolygonFiller();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

CT_DataPolygonFiller::CT_DataPolygonFiller()
:	iPolygonFiller(NULL)
	{
	}

void CT_DataPolygonFiller::ConstructL()
	{
	iPointsArray=NULL;
	iPoints=NULL;
	}

CT_DataPolygonFiller::~CT_DataPolygonFiller()
	{
	DestroyData();
	
	delete iPointsArray;
	iPointsArray=NULL;
	
	delete[] iPoints;
	iPoints=NULL;
	}

TAny* CT_DataPolygonFiller::GetObject()
	{
	return iPolygonFiller;
	}

void CT_DataPolygonFiller::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iPolygonFiller = static_cast<CPolygonFiller*> (aAny);
	}

void CT_DataPolygonFiller::DisownObjectL()
	{
	iPolygonFiller = NULL;
	}

void CT_DataPolygonFiller::DestroyData()
	{
	DoCmdDestructor();
	}


/**
* Process a command read from the ini file
*
* @param aCommand			the command to process
* @param aSection			the entry in the ini file requiring the command to be processed
* @return ETrue if the command is processed
*/
TBool CT_DataPolygonFiller::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdDestructorGeneral || aCommand==KCmdDestructor )
		{
		DoCmdDestructor();
		}
	else if ( aCommand==KCmdnewL || aCommand==KDataClassname )
		{
		DoCmdnewL();
		}
	else if(aCommand==KCmdConstructL)
		{
		DoCmdConstructL(aSection);
		}
	else if ( aCommand==KCmdReset )
		{
		DoCmdReset();
		}
	else if ( aCommand==KCmdGetNextPixelRun )
		{
		DoCmdGetNextPixelRun(aSection);
		}
	else if ( aCommand==KCmdGetNextPixelRunOnSpecifiedScanLine )
		{
		DoCmdGetNextPixelRunOnSpecifiedScanLine(aSection);
		}
	else
		{
		ret=EFalse;
		}
	return ret;
	}

void CT_DataPolygonFiller::DoCmdnewL()
	{ 
	DestroyData();
	TInt err = KErrNone;
	INFO_PRINTF1(KLogInfoCmdnewL);
	TRAP(err, iPolygonFiller = new (ELeave) CPolygonFiller());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogErrNum, err);
		SetError(err);
		}
	}

void CT_DataPolygonFiller::DoCmdConstructL(const TDesC& aSection)
	{
	TBool	dataOk=ETrue;
    TInt datNumPoints=0;
    TInt err=0;
    CGraphicsContext::TFillRule	datFillRule=CGraphicsContext::EAlternate;
    CPolygonFiller::TUsage datUsage=CPolygonFiller::EGetAllPixelRunsSequentially;

	CT_GraphicsUtil::ReadFillRule(*this, aSection, KFldFillRule, datFillRule);
    CT_GraphicsUtil::ReadTUsage(*this,aSection,KFldUsage,datUsage);
	
//Use TPoint parameter to construct if the number of points in the list is assigned.
	if (GetIntFromConfig(aSection, KFldnumPoints, datNumPoints))
		{
		iPoints=new (ELeave) TPoint[datNumPoints];
		if ( !GetPointListFromConfig(aSection, KFldPointList, datNumPoints, iPoints))
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(KLogInfoCmdConstruct1);
			iPolygonFiller->Construct(iPoints, datNumPoints, datFillRule, datUsage);
			}
		}
	else
		{
		iPointsArray=new (ELeave) CArrayFixFlat<TPoint>(1);
		TBool	hasPointList=GetPointListFromConfigL(aSection, KFldPointList, *iPointsArray);
				
		if ( !hasPointList )
			{
			ERR_PRINTF2(KLogMissingParameter, &KFldPointList());
			SetBlockResult(EFail);
			}
		else
			{
			// Execute command and log parameters
			INFO_PRINTF1(KLogInfoCmdConstruct2);
			iPolygonFiller->Construct(iPointsArray,datFillRule,datUsage);
			}
		}
	}

void CT_DataPolygonFiller::DoCmdDestructor()
	{
	INFO_PRINTF1(KLogInfoCmdDestructor1);
	delete iPolygonFiller;
	iPolygonFiller = NULL;
	}

void CT_DataPolygonFiller::DoCmdReset()
	{
	INFO_PRINTF1(KLogInfoCmdReset);
	iPolygonFiller->Reset();
	}

void CT_DataPolygonFiller::DoCmdGetNextPixelRun(const TDesC& aSection)
	{
	TBool relExists,expectedExists;
	TInt relScanLine,relStart,relEnd,expectedScanLine,expectedStart,expectedEnd;
	INFO_PRINTF1(KLogInfoCmdGetNextPixelRun);
	iPolygonFiller->GetNextPixelRun(relExists,relScanLine,relStart,relEnd);
	//Verify the scanline is expected
	if(!GetBoolFromConfig(aSection,KFldExpectedExists,expectedExists))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpectedExists);
		SetBlockResult(EFail);
		}
	else if(relExists!=expectedExists)
    	{
    	ERR_PRINTF3(_L("The returned Exists value is not as expected, expected: %d, result: %d"), expectedExists , relExists);        
    	SetBlockResult(EFail);
    	}
	if(!GetIntFromConfig(aSection,KFldExpectedScanLine,expectedScanLine))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpectedScanLine);
		SetBlockResult(EFail);
		}
	else if(relScanLine!=expectedScanLine)
    	{
    	ERR_PRINTF3(_L("The returned Scanline value is not as expected, expected: %d, result: %d"), expectedScanLine , relScanLine);        
    	SetBlockResult(EFail);
    	}
    //Verify the start position on the scan line  
    if(!GetIntFromConfig(aSection,KFldExpectedStart,expectedStart))
    	{
    	INFO_PRINTF1(KLogInfoNotCheckThePosition);

    	}
    else if(relStart!=expectedStart)
    	{
    	ERR_PRINTF3(_L("The returned Start Position value is not as expected, expected: %d, result: %d"), expectedStart , relStart);        
    	SetBlockResult(EFail);
    	}
    //Verify the end position on the scan line
    if(!GetIntFromConfig(aSection,KFldExpectedEnd,expectedEnd))
    	{
    	INFO_PRINTF1(KLogInfoNotCheckThePosition);

		}
    else if(relEnd!=expectedEnd)
    	{
    	ERR_PRINTF3(_L("The returned End Position value is not as expected, expected: %d, result: %d"), expectedEnd , relEnd);        
    	SetBlockResult(EFail);
    	} 
	}

void CT_DataPolygonFiller::DoCmdGetNextPixelRunOnSpecifiedScanLine(const TDesC& aSection)
	{
	TInt datScanLine=0;
	TBool relExists,expectedExists;
	TInt relStart,relEnd,expectedStart,expectedEnd;
	
	if (!GetIntFromConfig(aSection, KFldScanLine, datScanLine))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldScanLine);
		SetBlockResult(EFail);
		}
	INFO_PRINTF1(KLogInfoCmdGetNextPixelRunOnSpecifiedScanLine);
    iPolygonFiller->GetNextPixelRunOnSpecifiedScanLine(relExists,datScanLine,relStart,relEnd);
	if(!GetBoolFromConfig(aSection,KFldExpectedExists,expectedExists))
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldExpectedExists);
		SetBlockResult(EFail);
		}
	else if(relExists!=expectedExists)
    	{
    	ERR_PRINTF3(_L("The returned Exists value is not as expected, expected: %d, result: %d"), expectedExists , relExists);        
    	SetBlockResult(EFail);
    	}
    //Verify the start position on the scan line  
    if(!GetIntFromConfig(aSection,KFldExpectedStart,expectedStart))
    	{
    	INFO_PRINTF1(KLogInfoNotCheckThePosition);

    	}
    else if(relStart!=expectedStart)
		{
    	ERR_PRINTF3(_L("The returned Start Position value is not as expected, expected: %d, result: %d"), expectedStart , relStart);        
    	SetBlockResult(EFail);
		}
    //Verify the end position on the scan line
    if(!GetIntFromConfig(aSection,KFldExpectedEnd,expectedEnd))
    	{
    	INFO_PRINTF1(KLogInfoNotCheckThePosition);

    	}
    else if(relEnd!=expectedEnd)
    	{
    	ERR_PRINTF3(_L("The returned End Position value is not as expected, expected: %d, result: %d"), expectedEnd , relEnd);        
    	SetBlockResult(EFail);
    	} 
	}

