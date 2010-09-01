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

This contains CT_DataBitmapDevice
*/

#if (!defined __T_DATA_BITMAP_DEVICE_H__)
#define __T_DATA_BITMAP_DEVICE_H__

//	User includes
#include "T_DataGraphicsDevice.h"

class CT_DataBitmapDevice : public CT_DataGraphicsDevice
	{
public:
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataBitmapDevice();
	~CT_DataBitmapDevice();

	virtual CBitmapDevice*	GetBitmapDevice() const = 0;

	void	DoCmdAddFileL(const TDesC& aSection);
	void	DoCmdCreateBitmapContextL(const TDesC& aSection);
	void	DoCmdFontHeightInPixels(const TDesC& aSection);
	void	DoCmdGetNearestFontInPixelsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToDesignHeightInPixelsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection);
	void	DoCmdGetPixel(const TDesC& aSection);
	void	DoCmdGetScanLineL(const TDesC& aSection);
	void	DoCmdRemoveFile(const TDesC& aSection);
	
	void	CheckScanLine(const TDesC& aSection,TDes8 &aBuf);
	
private:
	RArray<TPtrC>	iFile;
	RArray<TInt>	iId;
	};

#endif /* __T_DATA_BITMAP_DEVICE_H__ */
