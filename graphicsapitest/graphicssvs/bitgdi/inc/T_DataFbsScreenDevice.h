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

This contains CT_DataFbsScreenDevice
*/

#if (!defined __T_DATA_FBS_SCREEN_DEVICE_H__)
#define __T_DATA_FBS_SCREEN_DEVICE_H__

//	User includes
#include "T_DataFbsDevice.h"

class CT_DataFbsScreenDevice : public CT_DataFbsDevice
	{
public:
	static CT_DataFbsScreenDevice*	NewL();

	//	CTPData implementation
	virtual TAny*	GetObject();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual void SetObjectL(TAny* aAny);
	~CT_DataFbsScreenDevice();
	void	DestroyData();
	virtual void DisownObjectL();

protected:
	CT_DataFbsScreenDevice();
	void ConstructL();

	//	CT_DataGraphicsDeviceMap implementation
	virtual MGraphicsDeviceMap*	GetGraphicsDeviceMap() const;

	//	CT_DataGraphicsDevice implementation
	virtual CGraphicsDevice*	GetGraphicsDevice() const;

	//	CT_DataBitmapDevice implementation
	virtual CBitmapDevice*		GetBitmapDevice() const;

	//	CT_DataFbsDevice implementation
	virtual CFbsDevice*			GetFbsDevice() const;

private:
	void	DoCmdNew(const TDesC& aSection);
	void	DoCmdDestructor();
//	void    DoCmdSizeInTwips();
	
	void    DoCmdChangeScreenDeviceL(const TDesC& aSection);
	void	DoCmdDrawSpriteBegin();
	void	DoCmdDrawSpriteEnd();
	void	DoCmdHardwareBitmap();
	void	DoCmdScreenNo(const TDesC& aSection);
	void	DoCmdSetAutoUpdate(const TDesC& aSection);
	void    DoCmdUpdateL(const TDesC& aSection);
	void	DoCmdCancelSprite();
	void 	DoCmdHideSpriteL(const TDesC& aSection);
	void 	DoCmdShowSpriteL(const TDesC& aSection);

	
protected:
	/** test step reference*/
	CFbsScreenDevice*	iFbsScreenDevice;
	};

#endif /* __T_DATA_FBS_SCREEN_DEVICE_H__ */
