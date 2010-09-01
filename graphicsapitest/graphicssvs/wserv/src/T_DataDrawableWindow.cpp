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

#include "T_DataDrawableWindow.h"

/*@{*/
//Commands
_LIT(KCmdScroll,					"Scroll");
_LIT(KCmdGetDrawRect,				"GetDrawRect");

// Fields
_LIT(KFldClipRect,					"cliprect");
_LIT(KFldOffset,					"offset");
_LIT(KFldRect,						"rect");
_LIT(KFldExpected,					"expected");

///	Logging
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogMissingExpectedValue,		"Missing expected value '%S'");
_LIT(KLogNotAsExpectedValue,		"'%S' is not as expected value");

/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataDrawableWindow::CT_DataDrawableWindow()
:	CT_DataWindowBase()
	{
	}

RWindowBase* CT_DataDrawableWindow::GetWindowBase() const
	{
	return GetDrawableWindow();
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
TBool CT_DataDrawableWindow::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret = ETrue;

	if ( aCommand==KCmdScroll )
		{
		DoCmdScroll(aSection);
		}
	else if ( aCommand==KCmdGetDrawRect )
		{
		DoCmdGetDrawRect(aSection);
		}
	else
		{
		ret=CT_DataWindowBase::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}


void CT_DataDrawableWindow::DoCmdScroll(const TDesC& aSection)
	{
	// Get test data for command input parameter(s)
	TPoint	datOffset;

	if ( !GetPointFromConfig(aSection, KFldOffset, datOffset) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldOffset);
		SetBlockResult(EFail);
		}
	else
		{
		TRect	datRect;
		TBool	hasRect = GetRectFromConfig(aSection, KFldRect, datRect);

		TRect	datClipRect;

		// Execute command and log parameters
		if ( GetRectFromConfig(aSection, KFldClipRect,	datClipRect) )
			{
			if ( hasRect )
				{
				INFO_PRINTF1(_L("execute Scroll(TRect, TPoint, TRect)"));
				GetDrawableWindow()->Scroll(datClipRect, datOffset, datRect);
				}
			else
				{
				INFO_PRINTF1(_L("execute Scroll(TRect, TPoint)"));
				GetDrawableWindow()->Scroll(datClipRect, datOffset);
				}
			}
		else
			{
			if ( hasRect )
				{
				INFO_PRINTF1(_L("execute Scroll(TPoint, TRect)"));
				GetDrawableWindow()->Scroll(datOffset, datRect);
				}
			else
				{
				INFO_PRINTF1(_L("execute Scroll(TPoint)"));
				GetDrawableWindow()->Scroll(datOffset);
				}
			}

		// No command return value and output parameter to display and check
		}
	}


void CT_DataDrawableWindow::DoCmdGetDrawRect(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(_L("execute GetDrawRect()"));
	TRect	actual=GetDrawableWindow()->GetDrawRect();
	LogRect(_L("GetDrawableWindow()"), actual);

	// Diaplay command return value, check if it matches the expected value
	TRect	expected;
	if ( !GetRectFromConfig(aSection, KFldExpected, expected) )
		{
		ERR_PRINTF2(KLogMissingExpectedValue, &KFldExpected);
		SetBlockResult(EFail);
		}
	else
		{
		if ( actual!=expected )
			{
			ERR_PRINTF2(KLogNotAsExpectedValue, &KFldExpected);
			SetBlockResult(EFail);
			}
		}
	}
