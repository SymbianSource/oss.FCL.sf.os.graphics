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


#if (!defined __T_GRAPHICS_WSERV_WSBITMAP_H__)
#define __T_GRAPHICS_WSERV_WSBITMAP_H__

// User Includes
#include "T_DataWsClientClass.h"
#include "T_DataFbsBitmap.h"


/**
 * Test Active Notification class
 *
 */
class CT_DataWsBitmap : public CT_DataFbsBitmap, public CT_DataWsClientClass
	{

public:
	/**
	* Two phase constructor
	*/
	static CT_DataWsBitmap*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataWsBitmap();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iWsBitmap; }

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
	CT_DataWsBitmap();


	/**
	* Second phase construction
	*/
	void  ConstructL();

	virtual MWsClientClass* GetClientClass() const;

private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnewL(const TDesC& aEntry);
	void	DoCmdDestructor();
	void	DoCmdCreate(const TDesC& aEntry);
	void	DoCmdDuplicateL(const TDesC& aEntry);
	void	DoCmdLoad(const TDesC& aEntry);
	void	DoCmdInternalizeL(const TDesC& aEntry);
	void	DoCmdReset();

private:
	CWsBitmap*	iWsBitmap;
	};

#endif /* __T_GRAPHICS_WSERV_WSBITMAP_H__ */
