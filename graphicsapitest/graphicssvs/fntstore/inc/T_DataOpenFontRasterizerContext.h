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

This contains CT_DataOpenFontRasterizerContext
*/

#if (!defined __T_DATA_OPEN_FONT_RASTERIZER_CONTEXT_H__)
#define __T_DATA_OPEN_FONT_RASTERIZER_CONTEXT_H__

//	User Includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <e32std.h>
#include <openfont.h>

class CT_DataOpenFontRasterizerContext : public CDataWrapperBase
	{
public:
	CT_DataOpenFontRasterizerContext();
	~CT_DataOpenFontRasterizerContext();
	
	static	CT_DataOpenFontRasterizerContext*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	void			DestroyData();

private:
	void	ConstructL();

	void	DoCmdNew();
	void	DoCmdEndGlyph();
	void	DoCmdStartGlyphL(const TDesC& aSection);
	void	DoCmdWriteGlyphBit(const TDesC& aSection);
	void	DoCmdWriteGlyphByte(const TDesC& aSection);
	void	DoCmdDestructor();

private:
	/** COpenFontRasterizerContext class instance to work with*/
    COpenFontRasterizerContext*	iRasterizerContext;
	};

#endif /* __T_DATA_OPEN_FONT_RASTERIZER_CONTEXT_H__ */
