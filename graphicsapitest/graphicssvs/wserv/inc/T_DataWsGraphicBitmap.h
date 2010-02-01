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


#if (!defined __T_GRAPHICS_WSERV_WSGRAPHICBITMAP_H__)
#define __T_GRAPHICS_WSERV_WSGRAPHICBITMAP_H__

// User Includes
#include "T_DataWsGraphic.h"


// EPOC includes
#include <w32stdgraphic.h>


/**
 * Test Active Notification class
 *
 */
class CT_DataWsGraphicBitmap : public CT_DataWsGraphic
	{

public:
	/**
	* Two phase constructor
	*/
	static CT_DataWsGraphicBitmap*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataWsGraphicBitmap();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iWsGraphicBitmap; }

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
	CT_DataWsGraphicBitmap();

	/**
	* Second phase construction
	*/
	void  ConstructL();

	//	CT_DataWsGraphic Implementation
	virtual CWsGraphic*	GetWsGraphic() const;
	virtual TInt		Share(TSecureId aClientId);
	virtual TInt		ShareGlobally();
	virtual TInt		UnShare(TSecureId aClientId);
	virtual TInt		UnShareGlobally();

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdNewL(const TDesC& aEntry);
	void	DoCmdDestructor();

private:
	CWsGraphicBitmap*	iWsGraphicBitmap;

	};
#endif /* __T_GRAPHICS_WSERV_WSGRAPHICBITMAP_H__ */
