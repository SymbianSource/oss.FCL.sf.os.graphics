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


#if (!defined __T_GRAPHICS_WSERV_WINDOWGC_H__)
#define __T_GRAPHICS_WSERV_WINDOWGC_H__

//	User Includes
#include "DataWrapperBase.h"
#include "T_DataBitmapContext.h"
#include "T_DataWsClientClass.h"

//	EPOC Includes
#include <gdi.h>


/**
 * Test Active Notification class
 *
 */
class CT_DataWindowGc : public CT_DataBitmapContext, public CT_DataWsClientClass
	{

public:
	/**
	* Two phase constructor
	*/
	static CT_DataWindowGc*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataWindowGc();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iWindowGc; }

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
	CT_DataWindowGc();

	/**
	* Second phase construction
	*/
	void  ConstructL();

	virtual MWsClientClass*		GetClientClass() const;
	virtual CGraphicsContext*	GetGraphicsContext() const;
	virtual CBitmapContext*		GetBitmapContext() const;


private:
	/**
	* Helper methods
	*/
	void	DestroyData();
	void	DoCmdnewL(const TDesC& aSection);
	void	DoCmdDestructor();
	void	DoCmdConstruct();
	void	DoCmdActivateL(const TDesC& aSection);
	void	DoCmdDeactivate();
	void	DoCmdBitBltL(const TDesC& aCommand, const TDesC& aSection, const TInt aAsyncErrorIndex);
	void	DoCmdBitBltMaskedL(const TDesC& aCommand, const TDesC& aSection, const TInt aAsyncErrorIndex);
	void	DoCmdSetDitherOrigin(const TDesC& aSection);
	void	DoCmdSetOpaque(const TDesC& aSection);
	void	DoCmdDrawWsGraphicL(const TDesC& aSection);

private:
	CWindowGc*	iWindowGc;
	};

#endif /* __T_GRAPHICS_WSERV_WINDOWGC_H__ */
