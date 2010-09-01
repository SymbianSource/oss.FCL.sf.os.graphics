// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @test
 @internalComponent
*/

#include <e32math.h>
#include <e32debug.h>
#include <cinidata.h>
#include "t_inidata.h"
#include "t_simloadutils.h"
#include "t_wservconsts.h"

EXPORT_C TBool CTSimLoadAppUtils::SpikedLoadL()
	{
	TBool ret = EFalse;
	
	HBufC* simLoadType = NULL;
	simLoadType = CTIniData::ReadStrL(KConfigSimLoadType(), KWServSimLoadConfigFile);
	
	if (simLoadType!=NULL && simLoadType->Des().Compare(_L("KSimLoadSpiked")) == KErrNone)
		{
		ret = ETrue;
		}
	
	return ret;
	}

EXPORT_C TBool CTSimLoadAppUtils::StaticLoadL()
	{
	TBool ret = EFalse;
	
	HBufC* simLoadType = NULL;
	simLoadType = CTIniData::ReadStrL(KConfigSimLoadType(), KWServSimLoadConfigFile);
	
	if (simLoadType!=NULL && simLoadType->Des().Compare(_L("KSimLoadStatic")) == KErrNone)
		{
		ret = ETrue;
		}
	
	return ret;
	}

EXPORT_C TInt CTSimLoadAppUtils::SimLoadL()
	{
	return CTIniData::ReadIntL(KConfigSimLoadValue(), KWServSimLoadConfigFile);
	}
	
EXPORT_C void CTSimLoadAppUtils::SimulateStaticLoadL(const TInt& aLoadPercentage, const TInt& aDelay)
	{
	if (aLoadPercentage>=0)
		{
		// assuming the figure read in was '%' calculate the loop count
		// required to achieve the simulated load required
		TInt mySimLoad = CalculateSimLoadL(aLoadPercentage);
		
		TInt internalDelay = aDelay;
		
		if (aLoadPercentage==0)
			{
			// set delay to large number, so that simload process is still
			// running at end of test, and therefore not cause issue with
			// TEF execution determining the test has failed because
			// the sim load app is not running at end of test.
			internalDelay = 30000000; // 30s
			}
			
		SimulateTestLoad(mySimLoad, internalDelay);
		}	
	}
	
EXPORT_C void CTSimLoadAppUtils::SimulateSpikedLoad()
	{
	SimulateTestLoad(1500000, 100000);
	}

void CTSimLoadAppUtils::SimulateTestLoad(const TInt& aSimLoad, const TInt& aDelay)
	{
	FOREVER
		{
		for (TInt i = 0; i <= aSimLoad; i++)
			{
			#if !defined(__WINS__) && !defined(__X86GCC__)
			asm{nop;};
			#endif
			}

		User::After(aDelay);
		}
	}

TInt CTSimLoadAppUtils::CalculateSimLoadL(const TInt& aPercentage)
	{
	TInt32 calculatedLoad = 0;
	
	if (aPercentage>0)
		{
		// based on measurements performed using Symbian Sample Profiler
		// .../base/e32utils/analyse/profiler.rtf
		// and then utilising curve fit (http://www.ebicom.net/~dhyams/cftp.htm)
		// the following formula was determined to best calculate the load
		// based on a required percentage
		// simload = a + b*aPercentage + c*aPercentage^2 + d*aPercentage^3 + e*aPercentage^4
		const TReal a = 92600;
		const TReal b = -32000;
		const TReal c = 4700;
		const TReal d = -114;
		const TReal e = 0.91;
		TReal tempValue = a + (b*aPercentage) + (c*aPercentage*aPercentage) 
		                    + (d*aPercentage*aPercentage*aPercentage) 
		                    + (e*aPercentage*aPercentage*aPercentage*aPercentage);
		User::LeaveIfError(Math::Int(calculatedLoad, tempValue));
		}
		
	return calculatedLoad;
	}

