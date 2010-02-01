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

#include "T_DataWsGraphicAnimation.h"

/*@{*/
///	Commands
_LIT(KCmdnew,					"new");
_LIT(KCmdIsPaused,				"IsPaused");
_LIT(KCmdIsPlaying,				"IsPlaying");
_LIT(KCmdIsStopped,				"IsStopped");
_LIT(KCmdIsStopping,			"IsStopping");
_LIT(KCmdLoops,					"Loops");
_LIT(KCmdPause,					"Pause");
_LIT(KCmdPlay,					"Play");
_LIT(KCmdStop,					"Stop");

///	Fields
_LIT(KFldExpected,				"expected");
_LIT(KFldLoop,					"loop");
_LIT(KFldImmediately,			"immediately");

///	Logging
_LIT(KLogError,						"Error=%d");
_LIT(KLogMissingParameter,			"Missing parameter '%S'");
_LIT(KLogNotExpectedValue,			"Not expected value");
/*@}*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CT_DataWsGraphicAnimation::CT_DataWsGraphicAnimation()
:	CT_DataWsGraphicMsgFixedBase()
,	iWsGraphicAnimation(NULL)
	{
	}

CT_DataWsGraphicAnimation::~CT_DataWsGraphicAnimation()
	{
	DestroyData();
	}

void CT_DataWsGraphicAnimation::DestroyData()
	{
	delete iWsGraphicAnimation;
	iWsGraphicAnimation=NULL;
	}

/**
 * Return a pointer to the object that the data wraps
 *
 * @return pointer to the object that the data wraps
 */
TAny* CT_DataWsGraphicAnimation::GetObject()
	{
	return iWsGraphicAnimation;
	}

TWsGraphicMsgFixedBase* CT_DataWsGraphicAnimation::GetWsGraphicMsgFixedBase() const
	{
	return iWsGraphicAnimation;
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
TBool CT_DataWsGraphicAnimation::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool	ret=ETrue;

	if ( aCommand==KCmdnew )
		{
		DoCmdnew();
		}
	else if ( aCommand==KCmdIsPaused )
		{
		DoCmdIsPaused(aSection);
		}
	else if ( aCommand==KCmdIsPlaying )
		{
		DoCmdIsPlaying(aSection);
		}
	else if ( aCommand==KCmdIsStopped )
		{
		DoCmdIsStopped(aSection);
		}
	else if ( aCommand==KCmdIsStopping )
		{
		DoCmdIsStopping(aSection);
		}
	else if ( aCommand==KCmdLoops )
		{
		DoCmdLoops(aSection);
		}
	else if ( aCommand==KCmdPause )
		{
		DoCmdPause();
		}
	else if ( aCommand==KCmdPlay )
		{
		DoCmdPlay(aSection);
		}
	else if ( aCommand==KCmdStop )
		{
		DoCmdStop(aSection);
		}
	else
		{
		ret=CT_DataWsGraphicMsgFixedBase::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
		}

	return ret;
	}

void CT_DataWsGraphicAnimation::DoCmdnew()
	{
	INFO_PRINTF1(_L("execute new"));
	DestroyData();
	TRAPD(err, iWsGraphicAnimation=new (ELeave) TWsGraphicAnimation());
	if ( err!=KErrNone )
		{
		ERR_PRINTF2(KLogError, err);
		SetError(err);
		}
	}

void CT_DataWsGraphicAnimation::DoCmdIsPaused(const TDesC& aSection)
	{
	TBool	actual=iWsGraphicAnimation->IsPaused();
	INFO_PRINTF2(_L("execute IsPaused() = %d"), actual);

	TBool	expected;
	if ( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=actual )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphicAnimation::DoCmdIsPlaying(const TDesC& aSection)
	{
	TBool	actual=iWsGraphicAnimation->IsPlaying();
	INFO_PRINTF2(_L("execute IsPlaying() = %d"), actual);

	TBool	expected;
	if ( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=actual )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphicAnimation::DoCmdIsStopped(const TDesC& aSection)
	{
	TBool	actual=iWsGraphicAnimation->IsStopped();
	INFO_PRINTF2(_L("execute IsStopped() = %d"), actual);

	TBool	expected;
	if ( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=actual )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphicAnimation::DoCmdIsStopping(const TDesC& aSection)
	{
	TBool	actual=iWsGraphicAnimation->IsStopping();
	INFO_PRINTF2(_L("execute IsStopping() = %d"), actual);

	TBool	expected;
	if ( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=actual )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphicAnimation::DoCmdLoops(const TDesC& aSection)
	{
	TBool	actual=iWsGraphicAnimation->Loops();
	INFO_PRINTF2(_L("execute Loops() = %d"), actual);

	TBool	expected;
	if ( GetBoolFromConfig(aSection, KFldExpected, expected) )
		{
		if ( expected!=actual )
			{
			ERR_PRINTF1(KLogNotExpectedValue);
			SetBlockResult(EFail);
			}
		}
	}

void CT_DataWsGraphicAnimation::DoCmdPause()
	{
	iWsGraphicAnimation->Pause();
	INFO_PRINTF1(_L("execute Pause()"));
	}

void CT_DataWsGraphicAnimation::DoCmdPlay(const TDesC& aSection)
	{
	TBool	loop;
	if ( !GetBoolFromConfig(aSection, KFldLoop, loop) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldLoop());
		SetBlockResult(EFail);
		}
	else
		{
		iWsGraphicAnimation->Play(loop);
		INFO_PRINTF1(_L("execute Play(TBool)"));
		}
	}

void CT_DataWsGraphicAnimation::DoCmdStop(const TDesC& aSection)
	{
	TBool	immediately;
	if ( !GetBoolFromConfig(aSection, KFldImmediately, immediately) )
		{
		ERR_PRINTF2(KLogMissingParameter, &KFldImmediately());
		SetBlockResult(EFail);
		}
	else
		{
		iWsGraphicAnimation->Stop(immediately);
		INFO_PRINTF1(_L("execute Stop(TBool)"));
		}
	}
