// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef TESTANIMDLL_H
#define TESTANIMDLL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "w32adll.h" 
#include "T_log.h"

// CLASS DECLARATION

/**
*  CTestAnimDll
* 
*/
class CTestAnimDll : public CAnimDll
{
public: // Constructors and destructor

	/**
		* Destructor.
		*/
	~CTestAnimDll();

		/**
		* Two-phased constructor.
		*/
	static CTestAnimDll* NewL();

		/**
		* Two-phased constructor.
		*/
	static CTestAnimDll* NewLC();
	
	virtual CAnim *CreateInstanceL(TInt aType);
	
	TInt WriteLog(const TDesC& aInfo);
	TInt WriteLog(const TDesC8& aInfo);
	
protected:
	CLog* iLog;
	RFs iFs;

private:

	/**
		* Constructor for performing 1st stage construction
		*/
	CTestAnimDll();

	/**
		* EPOC default constructor for performing 2nd stage construction
		*/
	void ConstructL();

};

#endif // TESTANIMDLL_H

