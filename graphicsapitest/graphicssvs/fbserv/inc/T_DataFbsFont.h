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

This contains CT_DataFbsFont
*/

#if (!defined __T_DATA_FBS_FONT_H__)
#define __T_DATA_FBS_FONT_H__

//	User Includes
#include "T_DataFont.h"

//	EPOC includes
#include <e32std.h>
#include <fbs.h>


class CT_DataFbsFont : public CT_DataFont
	{
public:
	static CT_DataFbsFont*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	virtual void 	SetObjectL(TAny* aObject);
	virtual TAny*	GetObject();
	virtual void	DisownObjectL();

protected:
	CT_DataFbsFont();

	virtual CFont*	GetFont() const;

private:
	void	DoCmdGetFontMetrics(const TDesC& aSection);
	void	DoCmdTextWidthInPixelsWithCharWidth(const TDesC& aSection);
	void	DoCmdHandle();
	void	DoCmdCharacterMetrics(const TDesC& aSection);
	void	DoCmdRawTextWidthInPixels(const TDesC& aSection);
	void	DoCmdGetFaceAttrib(const TDesC& aSection);
	void	DoCmdIsOpenFont(const TDesC& aSection);
	void	DoCmdHasCharacter(const TDesC& aSection);

private:
	/** CFbsFont class instance that is tested, not owned by the data-object */
	CFbsFont*	iFbsFont;
	};

#endif /* __T_DATA_FBS_FONT_H__ */
