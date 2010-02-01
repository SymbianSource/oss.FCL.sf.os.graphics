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

This contains CT_DataBitmapFont
*/

#if (!defined __T_DATA_BITMAP_FONT_H__)
#define __T_DATA_BITMAP_FONT_H__

//	User Includes
#include "T_DataFont.h"

//	EPOC includes
#include <e32std.h>
#include <fntstore.h>

class CT_DataBitmapFont : public CT_DataFont
	{
public:
	static CT_DataBitmapFont* NewL();
	
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	
protected:
	CT_DataBitmapFont();
	
	virtual CFont*	GetFont() const;
	
private:	
	void	DoCmdBitmapEncoding(const TDesC& aSection);
	void	DoCmdCharacterMetrics(const TDesC& aSection);
	void	DoCmdCharacterNeedsToBeRasterized(const TDesC& aSection);
	void	DoCmdGetCharacterData(const TDesC& aSection);
	void	DoCmdGetFaceAttrib(const TDesC& aSection);
	void	DoCmdGetFontMetrics(const TDesC& aSection);
	void	DoCmdGlyphBitmapType(const TDesC& aSection);
	void	DoCmdHasCharacterL(const TDesC& aSection);
	void	DoCmdIsOpenFont(const TDesC& aSection);
	void	DoCmdOpenFont(const TDesC& aSection);
	void	DoCmdRasterize(const TDesC& aSection);
	void	DoCmdUniqueFontId(const TDesC& aSection);
	
	static TBool GetCharacterMetricsFromConfig(CDataWrapper& aDataWrapper,const TDesC& aSectName,const TDesC& aKeyName,TCharacterMetrics& aResult);
	static TBool GetOpenFontCharMetricsFromConfig(CDataWrapperBase& aDataWrapper,const TDesC& aSectName,const TDesC& aKeyName,TOpenFontCharMetrics& aResult);
	static TBool GetOpenFontFaceAttribFromConfig(CDataWrapper& aDataWrapper,const TDesC& aSectName,const TDesC& aKeyName,TOpenFontFaceAttrib& aResult);
	static TBool GetOpenFontMetricsFromConfig(CDataWrapper& aDataWrapper,const TDesC& aSectName,const TDesC& aKeyName,TOpenFontMetrics& aResult);
	
private:
    CBitmapFont*	iBitmapFont;
	};

#endif /* __T_DATA_BITMAP_FONT_H__ */
