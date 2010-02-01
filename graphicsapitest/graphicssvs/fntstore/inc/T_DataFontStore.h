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

This contains CT_DataFontStore
*/

#if (!defined __T_DATA_FONTSTORE_H__)
#define __T_DATA_FONTSTORE_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <fntstore.h>
#include "T_DataTypefaceStore.h"

class CT_DataFontStore : public CT_DataTypefaceStore
	{
public:
	
	~CT_DataFontStore();
	static	CT_DataFontStore*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	void			DestroyData();
	virtual CTypefaceStore*	GetTypefaceStore() const;

protected:
	CT_DataFontStore();
	void ConstructL();
	
private:
	void	DoCmdNewL(const TDesC& aSection);
	void	DoCmdDestructor();
	
	void 	DoCmdAddFileL(const TDesC& aSection);
	void	DoCmdRemoveFile(const TDesC& aSection);
	void	DoCmdDefaultBitmapType(const TDesC& aSection);
	void	DoCmdDeleteSessionCache(const TDesC& aSection);
	void	DoCmdFontHeightInPixels(const TDesC& aSection);
	void	DoCmdFontHeightInTwips(const TDesC& aSection);
	void	DoCmdGetFontByIdL(const TDesC& aSection);
	void	DoCmdGetNearestFontToDesignHeightInPixelsL(const TDesC& aSection);
	TBool	DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection);
	TBool	DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection);
	void	DoCmdInstallRasterizerL(const TDesC& aSection);
	void	DoCmdInstallShaperFactoryL(const TDesC& aSection);
	void	DoCmdNumTypefaces(const TDesC& aSection);
	void	DoCmdSetDefaultBitmapType(const TDesC& aSection);
	void	DoCmdTypefaceSupport(const TDesC& aSection);

private:
	TBool	FindIdByName(const TDesC&, TInt&);

private:
	/** CLinkedTypefaceSpecification class instance to work with*/
	CFontStore*		iFntStore;
	RArray<TPtrC>	iFile;
	RArray<TInt>	iId;
	};

#endif /* __T_DATA_FONTSTORE_H__ */
