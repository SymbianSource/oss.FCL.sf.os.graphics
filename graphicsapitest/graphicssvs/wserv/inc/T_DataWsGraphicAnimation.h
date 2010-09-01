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


#if (!defined __T_DATA_WS_GRAPHIC_ANNIMATION_H__)
#define __T_DATA_WS_GRAPHIC_ANNIMATION_H__

//	User Includes
#include "T_DataWsGraphicMsgFixedBase.h"

//	EPOC Includes
#include <w32stdgraphic.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataWsGraphicAnimation : public CT_DataWsGraphicMsgFixedBase
	{
public:
	CT_DataWsGraphicAnimation();
	~CT_DataWsGraphicAnimation();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	virtual TAny*	GetObject();

protected:
	//	CT_DataWsGraphicMsgFixedBase implementation
	virtual TWsGraphicMsgFixedBase*	GetWsGraphicMsgFixedBase() const;

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnew();
	void	DoCmdIsPaused(const TDesC& aSection);
	void	DoCmdIsPlaying(const TDesC& aSection);
	void	DoCmdIsStopped(const TDesC& aSection);
	void	DoCmdIsStopping(const TDesC& aSection);
	void	DoCmdLoops(const TDesC& aSection);
	void	DoCmdPause();
	void	DoCmdPlay(const TDesC& aSection);
	void	DoCmdStop(const TDesC& aSection);

private:
	TWsGraphicAnimation*	iWsGraphicAnimation;
	};

#endif /* __T_DATA_WS_GRAPHIC_ANNIMATION_H__ */
