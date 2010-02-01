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
#include <apgtask.h>
#include "T_GraphicsUtil.h"
#include "T_DataDirectScreenAccess.h"
#include "T_DataFbsScreenDevice.h"
#include "T_DataFbsBitGc.h"

_LIT(KDataClassname,				"CDirectScreenAccess");
_LIT(KCmdDestructor,				"~CDirectScreenAccess");
_LIT(KCmdDestructorGeneral,			"~");

//command
_LIT( KCmdNewL, "NewL" );
_LIT( KCmdStart, "StartL" );
_LIT( KCmdGc, "Gc" );
_LIT( KCmdScreenDevice, "ScreenDevice" );
_LIT( KCmdDrawRegion, "DrawingRegion" );
_LIT( KCmdSetPriority, "SetPriority" );
_LIT( KCmdBringAppFg, "Util_BringAppFg" );

// Fields
_LIT( KRWsSessionObj, "ws");
_LIT( KCWsScreenDeviceObj, "scrdev");
_LIT( KRWindowObj, "win");
_LIT( KAppName, "app");
_LIT( KPriorityObj, "Priority");
_LIT( KFldGc, "gc");
_LIT( KFldScreenDevice, "ScreenDevice");
_LIT( KFldDrawRegion, "region1");
_LIT( KWantedReasonObj, "reason");

//Error
_LIT( KErrNewL, "Create CDirectScreenAccess object failed.");
_LIT( KErrGc, "Execute CDirectScreenAccess::Gc failed. return value is NULL");
_LIT( KErrScreenDevice, "Execute CDirectScreenAccess::ScreenDevices failed. return value is NULL");
_LIT( KErrDrawRegion, "Execute CDirectScreenAccess::DrawRegion failed. return value is NULL");
_LIT( KErrDrawRegionSetting, "Set DrawRegion failed.");
_LIT( KErrAbortNowReason, "the AbortNow's reason do not match the expected value.");
_LIT( KErrRestartReason, "the Restart's reason do not match the expected value.");
_LIT( KLogMissingParameter, "Missing parameter '%S'");
_LIT( KLogMissingWrapper,"Missing Wrapper class '%S'");

//info
_LIT( KInfoNewL, "NewL() has been called.");
_LIT( KInfoStartL, "StartL() has been called");
_LIT( KInfoGc, "Gc() has been called" );
_LIT( KInfoScreenDevice, "ScreenDevice() has been called" );
_LIT( KInfoDrawRegion, "DrawingRegion() has been called" );
_LIT( KInfoSetPriority, "SetPriority() has been called" );
_LIT( KInfoAbortNow, "AbortNow has been called, with the reason=%d");
_LIT( KInfoRestart, "Restart has been called, with the reason=%d");
_LIT( KInfoDestructor, "execute ~CDirectScreenAccess()");

const RDirectScreenAccess::TTerminationReasons KDefaultReason = RDirectScreenAccess::ETerminateRegion; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/**
 * Protected constructor. First phase construction
 */
CT_DataDirectScreenAccess::CT_DataDirectScreenAccess() :
	iDirectScreenAccess(NULL),
	iWantedReason(KDefaultReason)
	{
	}
/**
 * Public destructor
 */
CT_DataDirectScreenAccess::~CT_DataDirectScreenAccess()
	{
	DestroyData();
	}

/**
 * Two phase constructor
 */
CT_DataDirectScreenAccess* CT_DataDirectScreenAccess::NewL()
	{
	CT_DataDirectScreenAccess* self = new (ELeave) CT_DataDirectScreenAccess();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Second phase construction
 */
void CT_DataDirectScreenAccess::ConstructL()
	{
	//nothing to do here.
	}

/**
 * Set the object that the data wraps
 *
 * @param    aObject object that the wrapper is testing
 *
 */
void CT_DataDirectScreenAccess::SetObjectL(TAny* aAny)
	{
	DestroyData();
	iDirectScreenAccess = static_cast<CDirectScreenAccess*> (aAny);
	}

/**
 * The object will no longer be owned by this
 *
 * @leave	KErrNotSupported if the the function is not supported
 */
void CT_DataDirectScreenAccess::DisownObjectL()
	{
	iDirectScreenAccess = NULL;
	}

void CT_DataDirectScreenAccess::DestroyData()
	{
	delete iDirectScreenAccess;
	iDirectScreenAccess = NULL;
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
TBool CT_DataDirectScreenAccess::DoCommandL(const TTEFFunction& aCommand,
		const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool result = ETrue;
	
	if ( !aCommand.Compare( KCmdNewL ) || !aCommand.Compare( KDataClassname ) )
		{
        DoCmdnewL(aSection);
        }
    else if ( !aCommand.Compare(KCmdDestructor) || !aCommand.Compare(KCmdDestructorGeneral ) )
		{
		DoCmdDestructor();
		}
    else if ( !aCommand.Compare( KCmdStart ) )
        {
        DoCmdStartL(aSection);
        }
    else if ( !aCommand.Compare( KCmdGc ) )
        {
        DoCmdGcL(aSection);
        }
    else if ( !aCommand.Compare( KCmdScreenDevice ) )
        {
        DoCmdScreenDeviceL(aSection);
        }
    else if ( !aCommand.Compare( KCmdDrawRegion ) )
        {
        DoCmdDrawRegion(aSection);
        }
    else if ( !aCommand.Compare( KCmdSetPriority ) )
        {
        DoCmdSetPriority(aSection);
        }
    else if ( !aCommand.Compare( KCmdBringAppFg ) )
        {
        DoCmdBringAppFgL(aSection);
        }
    else
        {
        CDataWrapperActive::DoCommandL(aCommand, aSection, aAsyncErrorIndex);
        }
    
    return result;
	}

/**
 * test AbortNow()
 */
void CT_DataDirectScreenAccess::AbortNow(RDirectScreenAccess::TTerminationReasons aReason)
	{
	INFO_PRINTF2(KInfoAbortNow , aReason);
	DecOutstanding();
	if( iWantedReason != aReason )
		{
		ERR_PRINTF1(KErrAbortNowReason);
		SetBlockResult(EFail);	
		}
	
	}

/**
 * Test Restart()
 */
void CT_DataDirectScreenAccess::Restart(RDirectScreenAccess::TTerminationReasons aReason)
	{
	INFO_PRINTF2(KInfoRestart , aReason);
	DecOutstanding();
	if( iWantedReason != aReason )
		{
		ERR_PRINTF1(KErrRestartReason);
		SetBlockResult(EFail);	
		}
	
	}

/**
 * Helper method to get RWsSession
 */
RWsSession* CT_DataDirectScreenAccess::GetRWSessionL(const TDesC& aSection)
	{
	RWsSession* iClient( NULL ); //Window Session Client
	TPtrC objectName;
	
	if ( GetStringFromConfig(aSection, KRWsSessionObj(), objectName) )
		{
		iClient = static_cast<RWsSession*>(GetDataObjectL(objectName));
		}
	
	return iClient;
	}

/**
 * Helper method to get CWsScreenDevice
 */
CWsScreenDevice* CT_DataDirectScreenAccess::GetDevicesL(const TDesC& aSection)
	{	
	CWsScreenDevice*	device = NULL;
	
	if ( !CT_GraphicsUtil::GetWsScreenDeviceL(*this, aSection, KCWsScreenDeviceObj, device) )
		{
		ERR_PRINTF2(KLogMissingParameter , &KCWsScreenDeviceObj);
		SetBlockResult(EFail);
		}
	
	return device;
	}


/**
 * Helper method to get RWindow
 */
RWindow* CT_DataDirectScreenAccess::GetWindowL(const TDesC& aSection)
	{
	RWindow* window( NULL );
	TPtrC objectName;
	
	if( GetStringFromConfig(aSection, KRWindowObj, objectName))
		{
		window = static_cast<RWindow*>(GetDataObjectL(objectName));		
		}

	return window;

	}


/**
* Test NewL()
*/
void CT_DataDirectScreenAccess::DoCmdnewL(const TDesC& aSection)
	{
	// Execute command and log parameters
	DestroyData();
	INFO_PRINTF1(KInfoNewL);
	iDirectScreenAccess = CDirectScreenAccess::NewL(*GetRWSessionL(aSection),
			*GetDevicesL(aSection), *GetWindowL(aSection), *this );
	
	if(!iDirectScreenAccess)
		{
		ERR_PRINTF1(KErrNewL);
		SetBlockResult(EFail);
		}
	
	}

/**
* Test Destructor()
*/
void CT_DataDirectScreenAccess::DoCmdDestructor()
	{
	INFO_PRINTF1(KInfoDestructor);
	delete iDirectScreenAccess;
	iDirectScreenAccess = NULL;
	}

/**
* Test StartL()
*/
void CT_DataDirectScreenAccess::DoCmdStartL(const TDesC& aSection)
	{
	INFO_PRINTF1(KInfoStartL);
	if ( !CT_GraphicsUtil::ReadTerminateReason(*this , aSection , KWantedReasonObj, iWantedReason) )
		{
		ERR_PRINTF2(KLogMissingParameter , &KWantedReasonObj);
		SetBlockResult(EFail);
		}
	
	iDirectScreenAccess->StartL();
	IncOutstanding();
	}

/**
* Test Gc()
*/
void CT_DataDirectScreenAccess::DoCmdGcL(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(KInfoGc);
	
	TPtrC aGcName;
	if(GetStringFromConfig(aSection, KFldGc, aGcName) )
		{
		CFbsBitGc* iGc = iDirectScreenAccess->Gc();
		if(!iGc)
			{
			INFO_PRINTF1(KErrGc);
			SetError(KErrNotFound);
			}
		else
			{
			SetDataObjectL(aGcName , iGc);
			}
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter , &KFldGc);
		}
	
	}

/**
* Test ScreenDevice()
*/
void CT_DataDirectScreenAccess::DoCmdScreenDeviceL(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(KInfoScreenDevice);
	TPtrC aScreenDeviceName;
	if(GetStringFromConfig(aSection, KFldScreenDevice, aScreenDeviceName))
		{
		CFbsScreenDevice* aScreenDevice = iDirectScreenAccess->ScreenDevice();
		if(!aScreenDevice)
			{
			ERR_PRINTF1(KErrScreenDevice);
			SetError(KErrNotFound);
			}
		else
			{
			SetDataObjectL(aScreenDeviceName , aScreenDevice);
			}
		
		}
	else
		{
		ERR_PRINTF2(KLogMissingParameter , &KFldScreenDevice);
		}
	
	}

/**
* Test DrawRegion()
*/
void CT_DataDirectScreenAccess::DoCmdDrawRegion(const TDesC& aSection)
	{
	// Execute command and log parameters
	INFO_PRINTF1(KInfoDrawRegion);
	TRegion* region;
	region = iDirectScreenAccess->DrawingRegion();
	
	if(!region)
		{
		ERR_PRINTF1(KErrDrawRegion);
		SetError(KErrNotFound);
		}
	// Compare the region.
	// read the exepected region size 
	TRect temp;
	if( GetRectFromConfig(aSection ,KFldDrawRegion, temp ))
		{
		//compare the region
		TRect temp2 = (*region)[0];
		
		if(temp != temp2)
			{
			ERR_PRINTF1(KErrDrawRegionSetting);
			SetBlockResult(EFail);
			}
		}
	
	}

/**
* Test AbortNow() and Restart() by SetPriority()
*/
void CT_DataDirectScreenAccess::DoCmdSetPriority(const TDesC& aSection)
	{
	INFO_PRINTF1(KInfoSetPriority);
	CActive::TPriority priority;
	IncOutstanding();
	if ( !CT_GraphicsUtil::ReadPriority(*this , aSection , KPriorityObj, priority) )
		{
		ERR_PRINTF2(KLogMissingParameter , &KPriorityObj);
		SetBlockResult(EFail);
		}
	iDirectScreenAccess->SetPriority(priority);
	}

/**
* Test AbortNow() and Restart() by bring a app to foreground
*/
void CT_DataDirectScreenAccess::DoCmdBringAppFgL(const TDesC& aSection)
	{
	// Execute command and log parameters
	TPtrC sessionKeyName(KRWsSessionObj);
	TPtrC appKeyName(KAppName);
	IncOutstanding();
	if ( !CT_GraphicsUtil::BringAppForegroundL(*this , aSection , sessionKeyName, appKeyName) )
		{
		ERR_PRINTF2(KLogMissingParameter , &sessionKeyName);
		SetBlockResult(EFail);
		}
	}

