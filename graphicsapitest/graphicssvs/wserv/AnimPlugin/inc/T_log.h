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

#ifndef LOG_H_
#define LOG_H_

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>


class CLog : public CBase
{
public: // Constructors and destructor
	~CLog();
	static CLog* NewL(RFs &aFs, const TDesC &aFileName);
	static CLog* NewLC(RFs &aFs, const TDesC &aFileName);

	//write log 
	TInt WriteLog(const TDesC& aInfo);
	TInt WriteLog(const TDesC8& aInfo);
private:
	//Constructor for performing 1st stage construction
	CLog();	
	
	//EPOC default constructor for performing 2nd stage construction
	void ConstructL(RFs &aFs, const TDesC &aFileName);

	RFile iFile;	//log file object
	TFileText iText;
	RBuf8 iBuf;	
};


#endif /*LOG_H_*/
