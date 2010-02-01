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

This contains CT_DataFbsBitGcFont
*/

#if (!defined __T_DATA_FBS_BITGC_FONT_H__)
#define __T_DATA_FBS_BITGC_FONT_H__

//	User includes
#include "T_DataFbsFont.h"

class CT_DataFbsBitGcFont : public CT_DataFbsFont
	{
public:
	static CT_DataFbsBitGcFont*	NewL();
	
	~CT_DataFbsBitGcFont();

	//	CTPData implementation
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();

protected:
	CT_DataFbsBitGcFont();
	void	ConstructL();

	// CT_DataFont implementation
	virtual CFont*	GetFont() const;
	
	// CT_DataFbsFont implementation
	virtual CFbsFont* GetFbsFont() const;

private:
	void	DoCmdNewL();
	void	DoCmdDestructor();
	
	void 	DoCmdAddress();
	void	DoCmdDuplicate(const TDesC& aSection);
	void	DoCmdReset();


protected:
	/** test step reference*/
	CFbsBitGcFont*	iFbsBitGcFont;
	};

#endif /* __T_DATA_FBS_BITGC_FONT_H__ */