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

This contains CT_DataGraphicsDevice
*/

#if (!defined __T_DATA_GRAPHICS_DEVICE_H__)
#define __T_DATA_GRAPHICS_DEVICE_H__

//	User Includes
#include "T_DataGraphicsDeviceMap.h"

class CT_DataGraphicsDevice : public CT_DataGraphicsDeviceMap
	{
public:
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataGraphicsDevice();

	virtual CGraphicsDevice*	GetGraphicsDevice() const = 0;

	void	DoCmdCreateContextL(const TDesC& aSection);
	void	DoCmdDisplayMode(const TDesC& aSection);
	void	DoCmdFontHeightInTwips(const TDesC& aSection);
	void	DoCmdGetPaletteL(const TDesC& aSection);
	void	DoCmdNumTypefaces(const TDesC& aSection);
	void	DoCmdPaletteAttributes(const TDesC& aSection);
	void	DoCmdSetPaletteL(const TDesC& aSection);
	void	DoCmdSizeInPixels(const TDesC& aSection);
	void	DoCmdSizeInTwips(const TDesC& aSection);
	void	DoCmdTypefaceSupport(const TDesC& aSection);

	//	helpers
	inline TDisplayMode	GetDisplayMode() const;

private:
	TDisplayMode		iDisplayMode;
	TSize				iSizeInPixels;
	TSize				iSizeInTwips;
	TTypefaceSupport	iTypefaceSupport;
	};

#include "T_DataGraphicsDevice.inl"

#endif /* __T_DATA_GRAPHICS_DEVICE_H__ */
