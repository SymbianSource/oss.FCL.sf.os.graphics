// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent - Internal Symbian test code 
*/

#ifndef __TPROFILER_H__
#define __TPROFILER_H__

#include <e32base.h>
#include <test/tefexportconst.h>

class CTestStep;


class CTProfiler : public CBase
    {
public:
    IMPORT_C static CTProfiler* NewL(CTestStep& aTestStep);
    IMPORT_C ~CTProfiler();
    IMPORT_C void StartTimer();
    IMPORT_C TInt64 StopTimer();
    IMPORT_C void InitResults();
    IMPORT_C void MarkResultSetL();
    IMPORT_C void MarkResultSetAndSuspendL();
    IMPORT_C TUint32 GetTrimedMean();
    IMPORT_C TUint32 Mean();
    IMPORT_C TInt PercentageChange(TInt aFirstTime, TInt aSecondTime);
    IMPORT_C void ResultsAnalysis(const TDesC & aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters);
    IMPORT_C void ResultsAnalysisPixelRate(const TDesC & aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters, TInt aNumPixels);
    IMPORT_C void ResultsAnalysisCharacterRate(const TDesC & aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters, TInt aNumPixels);
    IMPORT_C void ResultsAnalysisAverageByIterations(const TDesC & aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters);
    IMPORT_C void ResultsAnalysisAverageByNumberOfIterations(const TDesC& aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters);
    IMPORT_C void ResultsAnalysisFrameRate(const TDesC & aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters, TInt aNumPixels);
    IMPORT_C void ResultsAnalysisScreenRotationRate(const TDesC & aTestName, TInt aRotation, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters, TInt aNumPixels);
    IMPORT_C void ResultsAnalysisZorderSwitchingRate(const TDesC & aTestName, TInt aZorderSwitching, TInt aSrcScreenMode, TInt aDstScreenMode, TInt aIters, TInt aNumPixels);
	IMPORT_C void FreeResultsMemory();
	IMPORT_C TUint32 TimeMax();
	IMPORT_C TUint32 TimeMin();
	IMPORT_C void ShowResultArrayInTimingOrder();
	IMPORT_C void SetStoreResultInTimingOrder(TBool aStoreResultInTimingOrder);

private:
    CTProfiler(CTestStep& aTestStep);
    void ConstructL();
    
private:
    TUint32             iStart;
    TUint32             iEnd;
    TUint32             iDiff;
    TInt                iFreq;  
    RArray<TUint32>     iResults;
    RArray<TUint32>     iResultsTimingOrder;
    TBool               iResultsInitalised;
    CTestStep&          iTestStep;
    TBool               iStoreResultInTimingOrder;
    };

#endif


