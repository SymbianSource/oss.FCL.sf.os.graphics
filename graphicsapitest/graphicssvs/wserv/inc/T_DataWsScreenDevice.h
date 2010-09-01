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


#if (!defined __T_GRAPHICS_WSERV_SCREENDEVICE_H__)
#define __T_GRAPHICS_WSERV_SCREENDEVICE_H__

//	User Includes
#include "T_DataBitmapDevice.h"
#include "T_DataWsClientClass.h"


/**
 * Test Active Notification class
 *
 */
class CT_DataWsScreenDevice : public CT_DataBitmapDevice, public CT_DataWsClientClass
	{
public:
	static CT_DataWsScreenDevice*	NewL();

	~CT_DataWsScreenDevice();

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aAny);

	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataWsScreenDevice();
	void  ConstructL();

	virtual MWsClientClass*		GetClientClass() const;
	virtual MGraphicsDeviceMap*	GetGraphicsDeviceMap() const;
	virtual CGraphicsDevice*	GetGraphicsDevice() const;
	virtual CBitmapDevice*		GetBitmapDevice() const;

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdDestructor();
	void	DoCmdConstruct(const TDesC& aSection);
	void	DoCmdCopyScreenToBitmapL(const TDesC& aSection);
	void	DoCmdCreateContextL(const TDesC& aSection);
	void	DoCmdCurrentScreenMode(const TDesC& aSection);
	void	DoCmdGetCurrentScreenModeScale(const TDesC& aSection);
	void	DoCmdGetCurrentScreenModeScaledOrigin(const TDesC& aSection);
	void	DoCmdGetDefaultScreenModeOrigin(const TDesC& aSection);
	void	DoCmdGetDefaultScreenSizeAndRotation(const TDesC& aSection);
	void	DoCmdGetFontByIdL(const TDesC& aSection);
	void	DoCmdGetRotationsListL(const TDesC& aSection);
	void	DoCmdGetScreenModeDisplayMode(const TDesC& aSection);
	void	DoCmdGetScreenModeOrigin(const TDesC& aSection);
	void	DoCmdGetScreenModeScale(const TDesC& aSection);
	void	DoCmdGetScreenModeScaledOrigin(const TDesC& aSection);
	void	DoCmdGetScreenModeSizeAndRotation(const TDesC& aSection);
	void	DoCmdGetScreenNumber(const TDesC& aSection);
	void	DoCmdGetScreenSizeModeListL(const TDesC& aSection);
	void	DoCmdNumScreenModes(const TDesC& aSection);
	void	DoCmdPointerRect(const TDesC& aSection);
	void	DoCmdRectCompare(const TDesC& aSection);
	void	DoCmdScreenModeEnforcement(const TDesC& aSection);
	void	DoCmdSetAppScreenMode(const TDesC& aSection);
	void	DoCmdSetBackLight(const TDesC& aSection);
	void	DoCmdSetCustomPaletteL(const TDesC& aSection);
	void	DoCmdSetScreenSizeAndRotation(const TDesC& aSection);

	void	DoCmdcheckPixels(const TDesC& aSection);
	void	DoCmdcheckLineColor(const TDesC& aSection);
	void	DoCmdcheckRectColor(const TDesC& aSection);

private:
	CWsScreenDevice*	iWsScreenDevice;
	};

#endif /* __T_GRAPHICS_WSERV_SCREENDEVICE_H__ */
