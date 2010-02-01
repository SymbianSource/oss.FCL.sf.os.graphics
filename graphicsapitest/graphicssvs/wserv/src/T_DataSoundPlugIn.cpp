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

This contains CT_DataSoundPlugIn
*/

#include "T_DataSoundPlugIn.h"

/*@{*/
///	Parameters
_LIT(KExpectedBool,							"expected_bool");
_LIT(KEnabled,								"enabled");
_LIT(KPlugInFileName,						"filename");
_LIT(KOpcode,                               "opcode");
_LIT(KPackagedArgs,                         "args");
_LIT(KUid,                                  "uid");
_LIT(KChangableBool, 						"isChangeable");

/// Commands
_LIT(KCmdNew,								"new");
_LIT(KCmdConstruct,							"Construct");
_LIT(KCmdClose,								"Close");
_LIT(KCmdDestroy,							"Destroy");
_LIT(KCmdIsLoaded,							"IsLoaded");
_LIT(KCmdUnload,							"Unload");
_LIT(KCmdLoad,								"Load");
_LIT(KCmdSetKeyClick,						"SetKeyClick");
_LIT(KCmdKeyClickEnabled,					"KeyClickEnabled");
_LIT(KCmdSetPenClick,						"SetPenClick");
_LIT(KCmdPenClickEnabled,					"PenClickEnabled");
_LIT(KCmdCommandReply,						"CommandReply");

// Fields
_LIT(KFldWs,								"ws");

// Log
_LIT(KLogMissingParameter, 					"Missing parameter '%S'");

/*@}*/

/**
* Two phase constructor
*/
CT_DataSoundPlugIn* CT_DataSoundPlugIn::NewL()
	{
	CT_DataSoundPlugIn*	ret = new (ELeave) CT_DataSoundPlugIn();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}

/**
* Protected constructor. First phase construction
*/
CT_DataSoundPlugIn::CT_DataSoundPlugIn()
:	iSoundPlugIn(NULL)
	{
	}

/**
* Second phase construction
*/
void CT_DataSoundPlugIn::ConstructL()
	{
	}

/**
* Protected destructor.
*/
CT_DataSoundPlugIn::~CT_DataSoundPlugIn()
	{
	Cleanup();
	}

void CT_DataSoundPlugIn::Cleanup()
	{
	delete iSoundPlugIn;
	iSoundPlugIn = NULL;
	}
/**
 * Return a pointer to the object that the data wraps. Not implemented, returns NULL.
 *
 * @return pointer to the object that the data wraps. Not implemented, returns NULL.
 */
 TAny* CT_DataSoundPlugIn::GetObject()
 	{
 	return NULL;
 	}
 
 MWsClientClass* CT_DataSoundPlugIn::GetClientClass() const
 	{
 	return iSoundPlugIn;
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
TBool CT_DataSoundPlugIn::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
	{
	TBool	retVal=ETrue;

	if(aCommand == KCmdNew)
		{
		DoCmdNewL(aSection);
		}
	else if(aCommand == KCmdConstruct)
		{
		DoCmdConstruct(aSection);
		}
	else if(aCommand == KCmdClose)
		{
		DoCmdClose();
		}
	else if(aCommand == KCmdDestroy)
		{
		DoCmdDestroy();
		}
	else if(aCommand == KCmdIsLoaded)
		{
		DoCmdIsLoaded(aSection);
		}
	else if(aCommand == KCmdUnload)
		{
		DoCmdUnload();
		}
	else if(aCommand == KCmdLoad)
		{
		DoCmdLoad(aSection);
		}
	else if(aCommand == KCmdSetKeyClick)
		{
		DoCmdSetKeyClick(aSection);
		}
	else if(aCommand == KCmdKeyClickEnabled)
		{
		DoCmdKeyClickEnabled(aSection);
		}
	else if(aCommand == KCmdSetPenClick)
		{
		DoCmdSetPenClick(aSection);
		}
	else if(aCommand == KCmdPenClickEnabled)
		{
		DoCmdPenClickEnabled(aSection);
		}
	else if(aCommand == KCmdCommandReply)
		{
		DoCmdCommandReply(aSection);
		}
	else
		{
		retVal = EFalse;
		}

	return retVal;
	}

/** RSoundPlugIn::RSoundPlugIn() Call */
void CT_DataSoundPlugIn::DoCmdNewL(const TDesC& aSection)
	{
	// Cleanup if already created
	Cleanup();
	
	TPtrC wsName;
	RWsSession* ws = NULL;
	if(GetStringFromConfig(aSection, KFldWs, wsName))
		{
		ws = static_cast<RWsSession*>(GetDataObjectL(wsName));
		}
	

	TInt err = KErrNone;
	
	if(ws == NULL)
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RSoundPlugIn()"));
		TRAP(err, iSoundPlugIn = new (ELeave) RSoundPlugIn);
		}
	else
		{
		// Execute command and log parameters
		INFO_PRINTF1(_L("execute new RSoundPlugIn(RWsSession)"));
		TRAP(err, iSoundPlugIn = new (ELeave) RSoundPlugIn(*ws));
		}
	
	// Check error code
	if(err != KErrNone)
		{
		ERR_PRINTF2(_L("Error creation instance: %d"), err);
		SetError(err);
		}
	}

/** RSoundPlugIn::Construct() Call */
void CT_DataSoundPlugIn::DoCmdConstruct(const TDesC& aSection)
	{
	TInt err = KErrNone;
	
	if(0 == aSection.Length())
		{
		INFO_PRINTF1(_L("Calls RSoundPlugIn::Construct() with TUid::Null()"));
		
		// Call Construct()
		err = iSoundPlugIn->Construct();
		}
	else
		{
		INFO_PRINTF1(_L("Calls RSoundPlugIn::Construct() with a plug-in DLL's third UID"));
		
		// Get UID from parameters
		TInt uid;
		if(!GetIntFromConfig(aSection, KUid(), uid))
			{
			ERR_PRINTF2(KLogMissingParameter, &KUid());
			SetBlockResult(EFail);
			}
		else
			{
			//Call Construct()
			err = iSoundPlugIn->Construct(TUid::Uid(uid));
			}
		}
	
	// Check error code
	if(err != KErrNone)
		{
		ERR_PRINTF2(_L("Calls Construct() error %d"), err);
		SetError(err);
		}
	}


/** RSoundPlugIn::Close() Call */
void CT_DataSoundPlugIn::DoCmdClose()
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::Close()"));
	
	// Call Close()
	iSoundPlugIn->Close();
	}

/** RSoundPlugIn::Destroy() Call */
void CT_DataSoundPlugIn::DoCmdDestroy()
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::Destroy()"));
	
	// Call Destroy()
	iSoundPlugIn->Destroy();
	iSoundPlugIn = NULL;
	}

/** RSoundPlugIn::IsLoaded() Call */
void CT_DataSoundPlugIn::DoCmdIsLoaded(const TDesC& aSection)
	{
	TBool actualValue,actualChangeable;
	TBool setChangable = EFalse;
	TBool expectedValue = EFalse;

	if ( !GetBoolFromConfig(aSection, KExpectedBool(), expectedValue))
		{
		ERR_PRINTF2(KLogMissingParameter, &KExpectedBool());
		SetBlockResult (EFail);
		}		
	else
		{
		actualValue = iSoundPlugIn->IsLoaded (actualChangeable);
		INFO_PRINTF2(_L("Calls RSoundPlugIn::IsLoaded() %d"), actualValue);

		if ( expectedValue != actualValue)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, actualValue);
			SetBlockResult (EFail);
			}
		else if (actualValue)
			{
			if ( !GetBoolFromConfig(aSection, KChangableBool(), setChangable))
				{
				ERR_PRINTF2(KLogMissingParameter, &KChangableBool());
				SetBlockResult (EFail);
				}
			else
			{
			if (actualChangeable)
				actualChangeable=ETrue;
			if (actualChangeable != setChangable)
				{
				ERR_PRINTF3(_L("The IsChangeable is not as expected, expected: %d, actual: %d"), setChangable, actualChangeable);
				SetBlockResult (EFail);
				}
			}
			}		 
		}
	}

/** RSoundPlugIn::Unload() Call */
void CT_DataSoundPlugIn::DoCmdUnload()
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::Unload()"));

	// Call Unload to get actual return value
	TInt err = iSoundPlugIn->Unload();
	if(err != KErrNone)
		{
		ERR_PRINTF2(_L("Call Unload() error %d"), err);
		SetError(err);
		}
	}

/** RSoundPlugIn::Load() Call */
void CT_DataSoundPlugIn::DoCmdLoad(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::Load()"));
	
	// Get file name from parameters
	TPtrC fileName;
	if(!GetStringFromConfig(aSection, KPlugInFileName(), fileName))
		{
		ERR_PRINTF2(KLogMissingParameter, &KPlugInFileName());
		SetBlockResult(EFail);
		}
	else
		{	
		// Call Load()
		TInt err = iSoundPlugIn->Load(fileName);
		
		// Check error code
		if(err != KErrNone)
			{
			ERR_PRINTF3(_L("Load file error %d. File name: %S"), err, &fileName);
			SetError(err);
			}
		}
	}

/** RSoundPlugIn::SetKeyClick() Call */
void CT_DataSoundPlugIn::DoCmdSetKeyClick(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::SetKeyClick()"));
	
	// Get bool value from parameters
	TBool keyClickEnabled = ETrue;
	if(!GetBoolFromConfig(aSection, KEnabled(), keyClickEnabled))
		{
		ERR_PRINTF2(KLogMissingParameter, &KEnabled());
		SetBlockResult(EFail);
		}
	else
		{	
		// Call SetKeyClick()
		iSoundPlugIn->SetKeyClick(keyClickEnabled);
		}
	}

/** RSoundPlugIn::KeyClickEnabled() Call */
void CT_DataSoundPlugIn::DoCmdKeyClickEnabled(const TDesC& aSection)
	{
	// Call KeyClickEnabled() to get actual value
	TBool actualValue = iSoundPlugIn->KeyClickEnabled();
	INFO_PRINTF2(_L("Calls RSoundPlugIn::KeyClickEnabled() %d"), actualValue);
	
	// Get expected value from parameters
	TBool expectedValue = EFalse;
	if(GetBoolFromConfig(aSection, KExpectedBool(), expectedValue))
		{
		if(expectedValue != actualValue)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, actualValue);
			SetBlockResult(EFail);
			}
		}
	}

/** RSoundPlugIn::SetPenClick() Call */
void CT_DataSoundPlugIn::DoCmdSetPenClick(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::SetPenClick()"));
	
	// Get bool value from parameters
	TBool penClickEnabled = ETrue;
	if(!GetBoolFromConfig(aSection, KEnabled(), penClickEnabled))
		{
		ERR_PRINTF2(KLogMissingParameter, &KEnabled());
		SetBlockResult(EFail);
		}
	else
		{
		// Call SetPenClick()
		iSoundPlugIn->SetPenClick(penClickEnabled);
		}
	}

/** RSoundPlugIn::PenClickEnabled() Call */
void CT_DataSoundPlugIn::DoCmdPenClickEnabled(const TDesC& aSection)
	{
	// Call PenClickEnabled() to get actual value
	TBool actualValue = iSoundPlugIn->PenClickEnabled();
	INFO_PRINTF2(_L("Calls RSoundPlugIn::PenClickEnabled() %d"), actualValue);
	
	// Get expected value from parameters
	TBool expectedValue = EFalse;
	if(GetBoolFromConfig(aSection, KExpectedBool(), expectedValue))
		{
		if(expectedValue != actualValue)
			{
			ERR_PRINTF3(_L("The returned value is not as expected, expected: %d, actual: %d"), expectedValue, actualValue);
			SetBlockResult(EFail);
			}
		}
	}

/** RSoundPlugIn::DoCmdCommandReply() Call */
void CT_DataSoundPlugIn::DoCmdCommandReply(const TDesC& aSection)
	{
	INFO_PRINTF1(_L("Calls RSoundPlugIn::DoCmdCommandReply()"));
	
	TBool dataOK = ETrue;
	
	// Get Opcode from parameters
	TInt opcode = 0;
	if(!GetIntFromConfig(aSection, KOpcode(), opcode))
		{
		ERR_PRINTF2(KLogMissingParameter, &KOpcode());
		SetBlockResult(EFail);
		dataOK = EFalse;
		}
	
	// Get packeaged args from parameters
	TPtrC theString;
	if(!GetStringFromConfig(aSection, KPackagedArgs(), theString))
		{
		ERR_PRINTF2(KLogMissingParameter, &KPackagedArgs());
		SetBlockResult(EFail);
		dataOK = EFalse;
		}
	
	if(dataOK)
		{
		// Call CommandReply()
		TInt err = iSoundPlugIn->CommandReply(opcode, TPckgC<TDesC>(theString));
		
		// Check error code
		if ( err!=KErrNone )
			{
			ERR_PRINTF2(_L("CommandReply failed with error %d"), err);
			SetError(err);
			}
		}
	}
