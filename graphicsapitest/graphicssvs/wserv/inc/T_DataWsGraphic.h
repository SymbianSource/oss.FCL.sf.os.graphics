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


#if (!defined __T_GRAPHICS_WSERV_WSGRAPHIC_H__)
#define __T_GRAPHICS_WSERV_WSGRAPHIC_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC Includes
#include <w32std.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataWsGraphic : public CDataWrapperBase
	{
public:
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataWsGraphic();

	virtual CWsGraphic*	GetWsGraphic() const = 0;
	virtual TInt		Share(TSecureId aClientId) = 0;
	virtual TInt		ShareGlobally() = 0;
	virtual TInt		UnShare(TSecureId aClientId) = 0;
	virtual TInt		UnShareGlobally() = 0;

private:
	//	Helper methods
	void	DoCmdDestructorL();
	void	DoCmdDestroy();
	void	DoCmdIdL(const TDesC& aEntry);
	void	DoCmdIsActive(const TDesC& aEntry);
	void	DoCmdShareGlobally();
	void	DoCmdShare(const TDesC& aEntry);
	void	DoCmdUnShare(const TDesC& aEntry);
	void	DoCmdUnShareGlobally();
	};

#endif /* __T_GRAPHICS_WSERV_WSGRAPHIC_H__ */
