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

This contains CT_DataFbsDevice
*/

#if (!defined __T_DATA_FBS_DEVICE_H__)
#define __T_DATA_FBS_DEVICE_H__

//	User includes
#include "DataWrapperBase.h"
#include "T_DataBitmapDevice.h"

class CT_DataFbsDevice : public CT_DataBitmapDevice
	{
public:
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	CT_DataFbsDevice();

protected:
	void DoCmdCheckPixels(const TDesC& aSection);
	void DoCmdCheckLineColor(const TDesC& aSection);
	void DoCmdCheckRectColor(const TDesC& aSection);
	void DoCmdGetFontByIdL(const TDesC& aSection);
	TBool DoCmdGetNearestFontInPixelsL(const TDesC& aSection);
	TBool DoCmdGetNearestFontInTwipsL(const TDesC& aSection);	
	TBool DoCmdGetNearestFontToDesignHeightInPixelsL(const TDesC& aSection);
	TBool DoCmdGetNearestFontToDesignHeightInTwipsL(const TDesC& aSection);	
	TBool DoCmdGetNearestFontToMaxHeightInPixelsL(const TDesC& aSection);
	TBool DoCmdGetNearestFontToMaxHeightInTwipsL(const TDesC& aSection);
	void DoCmdSizeInPixels(const TDesC& aSection);

	TBool	DoCmdCreateContextL(const TDesC& aSection);
	void	DoCmdRectCompareL(const TDesC& aSection);
	void	DoCmdSetCustomPaletteL(const TDesC& aSection);
	void	DoCmdGraphicsAccelerator();
	void	DoCmdOrientation(const TDesC& aSection);
	void	DoCmdDisplayMode16M(const TDesC& aSection);
	void	DoCmdSetScalingFactor(const TDesC& aSection);
	void	DoCmdGetDrawRect(const TDesC& aSection);
	void	DoCmdRegisterLinkedTypefaceL(const TDesC& aSection);
	void	DoCmdSetDrawDeviceOffset(const TDesC& aSection);
	
	virtual CFbsDevice*	GetFbsDevice() const = 0;
	};

#endif /* __T_DATA_FBS_DEVICE_H__ */
