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


#if (!defined __DATA_WRAPPER_ACTIVE__)
#define __DATA_WRAPPER_ACTIVE__

//	User includes
#include "DataWrapperBase.h"

class CDataWrapperActive : public CDataWrapperBase
	{
protected:
	CDataWrapperActive();
	virtual ~CDataWrapperActive();

	virtual	CActive*	GetActive() = NULL;

	virtual TBool		DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

private:
	void				DoCmdCancel();
	void				DoCmdiStatus(const TDesC& aSection);
	};

#endif /* __DATA_WRAPPER_ACTIVE__ */
