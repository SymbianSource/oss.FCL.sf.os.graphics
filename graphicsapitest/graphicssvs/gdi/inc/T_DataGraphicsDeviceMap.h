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

This contains CT_DataGraphicsDeviceMap
*/

#if (!defined __T_DATA_GRAPHICS_DEVICE_MAP_H__)
#define __T_DATA_GRAPHICS_DEVICE_MAP_H__

//	epoc32 includes
#include <bitdev.h>

//	User Includes
#include "DataWrapperBase.h"

class CT_DataGraphicsDeviceMap : public CDataWrapperBase
	{
public:
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataGraphicsDeviceMap();

	virtual MGraphicsDeviceMap*	GetGraphicsDeviceMap() const = 0;

	void	DoCmdGetNearestFontInTwipsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection);
	void	DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection);
	void	DoCmdHorizontalPixelsToTwips(const TDesC& aSection);
	void	DoCmdHorizontalTwipsToPixels(const TDesC& aSection);
	void	DoCmdPixelsToTwips(const TDesC& aSection);
	void	DoCmdReleaseFontL(const TDesC& aSection);
	void	DoCmdTwipsToPixels(const TDesC& aSection);
	void	DoCmdVerticalPixelsToTwips(const TDesC& aSection);
	void	DoCmdVerticalTwipsToPixels(const TDesC& aSection);

private:
	TPoint	iTwipsToPixelsPoint;
	TRect	iTwipsToPixelsRect;
	TPoint	iPixelsToTwipsPoint;
	TRect	iPixelsToTwipsRect;
	};

#endif /* __T_DATA_GRAPHICS_DEVICE_MAP_H__ */
