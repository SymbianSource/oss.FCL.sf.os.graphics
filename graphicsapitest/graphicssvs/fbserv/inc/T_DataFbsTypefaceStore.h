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

This contains CT_DataFbsTypefaceStore
*/

#if (!defined __T_DATA_FBS_TYPEFACE_STORE_H__)
#define __T_DATA_FBS_TYPEFACE_STORE_H__

//	User Includes
#include "T_DataTypefaceStore.h"

//	EPOC includes
#include <e32std.h>
#include <fbs.h>

class CT_DataFbsTypefaceStore : public CT_DataTypefaceStore
	{
public:
	static CT_DataFbsTypefaceStore*	NewL();

	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	~CT_DataFbsTypefaceStore();

	virtual TAny*	GetObject();

protected:
	CT_DataFbsTypefaceStore();
	void	ConstructL();

	//	CT_DataTypefaceStore implementation
	virtual CTypefaceStore*	GetTypefaceStore() const;

private:
	void	DestroyData();
	void	DoCmdNewL(const TDesC& aSection);
	void	DoCmdGetNearestFontInPixelsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToDesignHeightInPixelsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection);
	void	DoCmdAddFile(const TDesC& aSection);
	void	DoCmdInstallFile(const TDesC& aSection);
	void	DoCmdRemoveFile(const TDesC& aSection);
	void	DoCmdGetFontByIdL(const TDesC& aSection);
	void	DoCmdFontHeightInPixels(const TDesC& aSection);
	void	DoCmdDefaultBitmapType(const TDesC& aSection);
	void	DoCmdSetDefaultBitmapType(const TDesC& aSection);
	void	DoCmdSetFontNameAlias(const TDesC& aSection);
	void	DoCmdSetDefaultLanguageForMetrics(const TDesC& aSection);
	void	DoCmdRemoveFontFileLocks(const TDesC& aSection);
	void	DoCmdSetSystemDefaultTypefaceName(const TDesC& aSection);

private:
	/** CFbsTypefaceStore class instance to work with*/
    CFbsTypefaceStore*	iFbsTypefaceStore;

    /** temporarily stored value of typeface file id. Set by AddFile() and InstallFile() for later use in RemoveFile() */
    TInt				iLastTypefaceFileId;
	};

#endif /* __T_DATA_FBS_TYPEFACE_STORE_H__ */
