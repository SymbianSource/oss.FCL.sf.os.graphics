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

This contains CT_DataOpenFontGlyphData
*/

#if (!defined __T_DATA_OPEN_FONT_GLYPH_DATA_H__)
#define __T_DATA_OPEN_FONT_GLYPH_DATA_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <openfont.h>

class CT_DataOpenFontGlyphData : public CDataWrapperBase
	{
public:
	CT_DataOpenFontGlyphData();
	~CT_DataOpenFontGlyphData();
	
	static	CT_DataOpenFontGlyphData*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	void			DestroyData();

private:
	void	ConstructL();

	void	DoCmdNew(const TDesC& aSection);
	void	DoCmdDestructor();
	
private:
	/** TOpenFontGlyphData class instance to work with*/
    TOpenFontGlyphData*	iGlyphData;
	};

#endif /* __T_DATA_OPEN_FONT_GLYPH_DATA_H__ */
