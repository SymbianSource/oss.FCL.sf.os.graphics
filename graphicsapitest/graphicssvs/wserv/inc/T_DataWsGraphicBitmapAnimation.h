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


#if (!defined __T_GRAPHICS_WSERV_WSGRAPHICBITMAPANIMATION_H__)
#define __T_GRAPHICS_WSERV_WSGRAPHICBITMAPANIMATION_H__

#include <w32stdgraphic.h>
#include "T_DataWsGraphic.h"
/**
 * Test Active Notification class
 *
 */
class CT_DataWsGraphicBitmapAnimation : public CT_DataWsGraphic
	{
public:
	/**
	* Two phase constructor
	*/
	static CT_DataWsGraphicBitmapAnimation*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataWsGraphicBitmapAnimation();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iWsGraphicBitmapAnimation; }

	/**
	* Set the object that the data wraps
	*
	* @param	aObject object that the wrapper is testing
	*
	*/
	virtual void	SetObjectL(TAny* aAny);

	/**
	* The object will no longer be owned by this
	*
	* @leave	KErrNotSupported if the the function is not supported
	*/
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataWsGraphicBitmapAnimation();

	/**
	* Second phase construction
	*/
	void  ConstructL();

	//from CT_DataWsGraphic
	CWsGraphic*	GetWsGraphic() const {return iWsGraphicBitmapAnimation;};
	TInt		Share(TSecureId aClientId);
	TInt		ShareGlobally();
	TInt		UnShare(TSecureId aClientId);
	TInt		UnShareGlobally();
	
private:
	void	DoCmdDestroy();
    void	DoCmdNewL(const TDesC& aSection);
    void	Util_StartAnimTesterClientL(const TDesC& aSection);
    void	Util_CloseAnimTesterClientL(const TDesC& aSection);
    
private:
    CWsGraphicBitmapAnimation* iWsGraphicBitmapAnimation;
	};

#endif /* __T_GRAPHICS_WSERV_WSGRAPHICBITMAPANIMATION_H__ */
