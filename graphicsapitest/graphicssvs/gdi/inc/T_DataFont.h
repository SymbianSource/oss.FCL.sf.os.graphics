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

This contains CT_DataFont
*/

#if (!defined __T_DATA_FONT_H__)
#define __T_DATA_FONT_H__

//	User Includes
#include "DataWrapperBase.h"

//	epoc32 includes
#include <gdi.h>
#include <openfont.h>

class CT_DataFont : public CDataWrapperBase
	{
public:
	CT_DataFont();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	virtual CFont*	GetFont() const = 0;

private:
	void			DoCmdTextCount(const TDesC& aSection);
	void			DoCmdTextWidthInPixels(const TDesC& aSection);
	void			DoCmdCharWidthInPixels(const TDesC& aSection);
	void			DoCmdFontCapitalAscent(const TDesC& aSection);
	void			DoCmdFontMaxAscent(const TDesC& aSection);
	void			DoCmdFontStandardDescent(const TDesC& aSection);
	void			DoCmdFontMaxDescent(const TDesC& aSection);
	void			DoCmdFontLineGap(const TDesC& aSection);
	void			DoCmdFontMaxHeight(const TDesC& aSection);
	void			DoCmdTypeUid(const TDesC& aSection);
	void			DoCmdHeightInPixels(const TDesC& aSection);
	void			DoCmdAscentInPixels(const TDesC& aSection);
	void			DoCmdDescentInPixels(const TDesC& aSection);
	void			DoCmdMaxCharWidthInPixels(const TDesC& aSection);
	void			DoCmdMaxNormalCharWidthInPixels(const TDesC& aSection);
	void			DoCmdFontSpecInTwips(const TDesC& aSection);
	void			DoCmdWidthZeroInPixels(const TDesC& aSection);
	void			DoCmdBaselineOffsetInPixels(const TDesC& aSection);
	void			DoCmdGetCharacterData(const TDesC& aSection);
	void			DoCmdExtendedFunction(const TDesC& aSection);
	void			DoCmdMeasureTextL(const TDesC& aSection);

	//	Helpers
	static TBool	GetMeasureTextInputL(CDataWrapper& aDataWrapper, const TDesC& aParameterName, const TDesC& aSection, CFont::TMeasureTextInput*& aMeasureTextInput);
	static TBool	GetMeasureTextOutputL(CDataWrapper& aDataWrapper, const TDesC& aParameterName, const TDesC& aSection, CFont::TMeasureTextOutput*& aMeasureTextOutput);
	};

#endif /* __T_DATA_FONT_H__ */
