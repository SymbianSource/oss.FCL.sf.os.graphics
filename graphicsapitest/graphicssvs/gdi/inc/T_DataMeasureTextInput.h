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

This contains CT_DataMeasureTextInput
*/

#if (!defined __T_DATA_MEASURE_TEXT_INPUT_H__)
#define __T_DATA_MEASURE_TEXT_INPUT_H__

//	User Includes
#include "DataWrapperBase.h"
//	EPOC includes
#include <e32std.h>
#include <gdi.h>

class CT_DataMeasureTextInput : public CDataWrapperBase
	{
public:
	static CT_DataMeasureTextInput*	NewL();

	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	

	~CT_DataMeasureTextInput();

	virtual TAny* GetObject();

protected:
	CT_DataMeasureTextInput();
	void ConstructL();

private:
	void		DoCmdNew();
	void		DoCmdDestructor();
	void		DoCmdSetCharJustExcess(const TDesC& aSection);
	void		DoCmdSetCharJustNum(const TDesC& aSection);
	void		DoCmdSetDirection(const TDesC& aSection);
	void		DoCmdSetEndInputChar(const TDesC& aSection);
	void		DoCmdSetFlags(const TDesC& aSection);
	void		DoCmdSetMaxAdvance(const TDesC& aSection);
	void		DoCmdSetMaxBounds(const TDesC& aSection);
	void		DoCmdSetStartInputChar(const TDesC& aSection);
	void		DoCmdSetWordJustExcess(const TDesC& aSection);
	void		DoCmdSetWordJustNum(const TDesC& aSection);

	///	Helpers
	TBool		GetValueFromConfig(const TDesC& aSection, TInt& aValue);
	TBool		ReadFlags(const TDesC& aSection, TUint16& aFlags);
	void		DestroyData();

private:

	/** TMeasureTextInput class instance to work with*/
    CFont::TMeasureTextInput*	iMeasureTextInput;
	};

#endif /* __T_DATA_MEASURE_TEXT_INPUT_H__ */
