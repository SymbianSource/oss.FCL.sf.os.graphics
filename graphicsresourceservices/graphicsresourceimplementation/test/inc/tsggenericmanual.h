// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Graphics Resource generic non-automated tests

#ifndef TSGGENERICMANUAL_H
#define TSGGENERICMANUAL_H

#include "tgraphicsresourceteststepbase.h"

/**
Class containing all the tests for SgResource that must be run manually
and not part of the automated testing system.
 */
class CTSgGenericManual : public CTSgTestStepBase
{
public:
	CTSgGenericManual(TBool aConformanceTests);
	~CTSgGenericManual();
	
	// tests
	void TestStressResourceLeakL();

private:
	// utility methods
	TInt CreateImages(const TSgImageInfo& aInfo, RArray<RSgImage>& aTestImages);
    void DestroyImages(RArray<RSgImage>& aTestImages);
    
	// From CTSgTestStepBase
	virtual TVerdict doTestStepL();
};

_LIT(KTSgGenericManual,"TSgGenericManual");

#endif /*TSGGENERICMANUAL_H*/
