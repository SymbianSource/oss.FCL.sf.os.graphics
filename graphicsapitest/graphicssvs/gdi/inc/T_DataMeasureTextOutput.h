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

This contains CT_DataMeasureTextOutput
*/

#if (!defined __T_DATA_MEASURE_TEXT_OUTPUT_H__)
#define __T_DATA_MEASURE_TEXT_OUTPUT_H__

//	User Includes
#include "DataWrapperBase.h"
//	EPOC includes
#include <e32std.h>
#include <gdi.h>

class CT_DataMeasureTextOutput : public CDataWrapperBase
	{
public:

	static CT_DataMeasureTextOutput*	NewL();

	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	
	~CT_DataMeasureTextOutput();

	virtual TAny*	GetObject();

protected:
	CT_DataMeasureTextOutput();
	void ConstructL();

private:
	void		DestroyData();
	void		DoCmdNew();
	void		DoCmdSetBounds(const TDesC& aSection);
	void		DoCmdSetChars(const TDesC& aSection);
	void		DoCmdSetGlyphs(const TDesC& aSection);
	void		DoCmdSetGroups(const TDesC& aSection);
	void		DoCmdSetMaxGlyphSize(const TDesC& aSection);
	void		DoCmdSetSpaces(const TDesC& aSection);

	static TBool	CT_DataMeasureTextOutput::GetValueFromConfig(CDataWrapper& iInputStep, const TDesC& aSection, TInt& aValue);

private:
	/** TMeasureTextOutput class instance to work with*/
    CFont::TMeasureTextOutput*	iMeasureTextOutput;
	};

#endif /* __T_DATA_MEASURE_TEXT_OUTPUT_H__ */
