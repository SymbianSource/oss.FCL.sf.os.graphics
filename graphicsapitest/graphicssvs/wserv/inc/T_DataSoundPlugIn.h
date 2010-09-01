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

#if (!defined __T_WSSOUNDPLUGIN_DATA_H__)
#define __T_WSSOUNDPLUGIN_DATA_H__

//	EPOC includes
#include <w32std.h>
#include <test/datawrapper.h>
//	User Includes
#include "DataWrapperBase.h"
#include "T_DataWsClientClass.h"


class CT_DataSoundPlugIn : public CDataWrapperBase , public CT_DataWsClientClass
	{
public:
	static CT_DataSoundPlugIn*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	
	~CT_DataSoundPlugIn();

	virtual TAny*	GetObject();

protected:
	CT_DataSoundPlugIn();
	void	ConstructL();

	virtual MWsClientClass*		GetClientClass() const;

private:
	void Cleanup();
	
	// Commands
	void DoCmdNewL(const TDesC& aSection);
	void DoCmdConstruct(const TDesC& aSection);
	void DoCmdClose();
	void DoCmdDestroy();
	void DoCmdIsLoaded(const TDesC& aSection);
	void DoCmdUnload();
	void DoCmdLoad(const TDesC& aSection);
	void DoCmdSetKeyClick(const TDesC& aSection);
	void DoCmdSetPenClick(const TDesC& aSection);
	void DoCmdKeyClickEnabled(const TDesC& aSection);
	void DoCmdPenClickEnabled(const TDesC& aSection);
	void DoCmdCommandReply(const TDesC& aSection);
private:
	RSoundPlugIn*	iSoundPlugIn;
	};

#endif /* __T_WSSOUNDPLUGIN_DATA_H__ */
