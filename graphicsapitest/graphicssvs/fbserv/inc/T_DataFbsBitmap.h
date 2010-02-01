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

This contains CT_DataFbsBitmap
*/

#if (!defined __T_DATA_FBS_BITMAP_H__)
#define __T_DATA_FBS_BITMAP_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <fbs.h>
#include <s32file.h>
#include <bitdev.h>

class CT_DataFbsBitmap : public CDataWrapperBase
	{
public:
	static CT_DataFbsBitmap*	NewL();
	~CT_DataFbsBitmap();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	virtual TAny*	GetObject();
    virtual void    SetObjectL(TAny* aAny);
	virtual void	DisownObjectL();

protected:
	CT_DataFbsBitmap();
	void	ConstructL();

	virtual CFbsBitmap*		GetFbsBitmap() const;

	void	RunL(CActive* aActive, TInt aIndex);
	void	DoCancel(CActive* aActive, TInt aIndex);

private:
	void	DestroyData();
	void	DoCmdNewL();
	void	DoCmdDestructor();
	void	DoCmdCompress(const TDesC& aSection);
	void	DoCmdCompressInBackground(const TDesC& aSection, const TInt aAsyncErrorIndex);
	void	DoCmdCreate(const TDesC& aSection);
	void	DoCmdCreateHardwareBitmap(const TDesC& aSection);
	void	DoCmdDataAddress(const TDesC& aSection);
	void	DoCmdDataStride(const TDesC& aSection);
	void	DoCmdDisplayMode(const TDesC& aSection);
	void	DoCmdDuplicateL(const TDesC& aSection);
	void	DoCmdExternalizeL(const TDesC& aSection);
	void	DoCmdExternalizeRectangleL(const TDesC& aSection);
	void	DoCmdGetPaletteL(const TDesC& aSection);
	void	DoCmdGetPixel(const TDesC& aSection);
	void	DoCmdGetScanLineL(const TDesC& aSection);
	void	DoCmdGetVerticalScanLineL(const TDesC& aSection);
	void	DoCmdHandle(const TDesC& aSection);
	void	DoCmdHardwareBitmapHandle(const TDesC& aSection);
	void	DoCmdHeader();
	void	DoCmdHorizontalPixelsToTwips(const TDesC& aSection);
	void	DoCmdHorizontalTwipsToPixels(const TDesC& aSection);
	void	DoCmdInitialDisplayMode(const TDesC& aSection);
	void	DoCmdInternalizeL(const TDesC& aSection);
	void	DoCmdIsCompressedInRAM(const TDesC& aSection);
	void	DoCmdIsFileInRom(const TDesC& aSection);
	void	DoCmdIsLargeBitmap(const TDesC& aSection);
	void	DoCmdIsMonochrome(const TDesC& aSection);
	void	DoCmdIsRomBitmap(const TDesC& aSection);
	void	DoCmdLoadL(const TDesC& aSection);
	void	DoCmdLoadAndCompressL(const TDesC& aSection);
	void	DoCmdLockHeap(const TDesC& aSection);
	void	DoCmdLockHeapLC(const TDesC& aSection);
	void	DoCmdPaletteAttributes(const TDesC& aSection);
	void	DoCmdReset();
	void	DoCmdResize(const TDesC& aSection);
	void	DoCmdSave(const TDesC& aSection);
	void	DoCmdScanLineLength(const TDesC& aSection);
	void	DoCmdSetDisplayMode(const TDesC& aSection);
	void	DoCmdSetPaletteL(const TDesC& aSection);
	void	DoCmdSetRomBitmapL(const TDesC& aSection);
	void	DoCmdSetScanLineL(const TDesC& aSection);
	void	DoCmdSetSizeInTwipsL(const TDesC& aSection);
	void	DoCmdSizeInPixels(const TDesC& aSection);
	void	DoCmdSizeInTwips(const TDesC& aSection);
	void	DoCmdStoreL(const TDesC& aSection);
	void	DoCmdSwapWidthAndHeight();
	void	DoCmdUnlockHeapL(const TDesC& aSection);
	void	DoCmdVerticalPixelsToTwips(const TDesC& aSection);
	void	DoCmdVerticalTwipsToPixels(const TDesC& aSection);

	//	MTPActiveCallback implementation
	void	RunCompressInBackground(const TInt aIndex);
	void	DoCancelCompressInBackground(const TInt aIndex);

	//	Helpers
	void				ExternalizeImplL(const TDesC& aSection, const TRect& aRect, TBool aUseRect);
	void				LoadOrLoadAndCompressL(const TDesC& aSection, TBool aCompress);
	inline TDisplayMode	GetDisplayMode() const;

private:
	/** CFbsBitmap class instance that is tested */
	CFbsBitmap*			iFbsBitmap;
	TDisplayMode		iDisplayMode;

	/** Pop when unlocking */
	TBool				iPopWhenUnlocking;
	
	TBool				iCompressFailed;

	/** Active object for CompressInBackground async operation */
	CActiveCallback*	iActiveCompressInBackground;
	};

#include "T_DataFbsBitmap.inl"

#endif /* __T_DATA_FBS_BITMAP_H__ */
