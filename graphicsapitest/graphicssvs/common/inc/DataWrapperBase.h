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


#if (!defined __DATA_WRAPPER_BASE__)
#define __DATA_WRAPPER_BASE__

//	EPOC includes
#include <test/datawrapper.h>
#include <gdi.h>
#include <w32std.h>

class CDataWrapperBase : public CDataWrapper
	{
public:
	TBool	GetBoolFromConfig(const TDesC& aSectName,const TDesC& aKeyName,TBool& aResult);
	TBool	GetFontSpecFromConfig(const TDesC& aSectName,const TDesC& aKeyName,TFontSpec& aResult);
	TBool	GetIntFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt& aResult);
	TBool	GetStringFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TPtrC& aResult);
	TBool	GetHexFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt& aResult);
	TBool	GetPointFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TPoint& aResult);
	TBool	GetRectFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TRect& aResult);
	TBool	GetRegionFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TRegion& aResult);
	TBool	GetRgbFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TRgb& aResult);
	TBool	GetSizeFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TSize& aResult);
	TBool	GetRgbListFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt aSize, TRgb*& aResult);
	TBool	GetPointListFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt aSize, TPoint*& aResult);
	TBool	GetPointListFromConfigL(const TDesC& aSectName, const TDesC& aKeyName, CArrayFix<TPoint>& aResult);
	TBool	GetWsGraphicIdFromConfigL(const TDesC& aSectName, const TDesC& aKeyName, TWsGraphicId& aResult);

	TBool   GetDateTimeFromConfig(const TDesC& aSectName,  TDateTime& aResult);
	void	LogRegion(const TDesC& aMessage, const TRegion& aRegion);
	void	LogRect(const TDesC& aMessage, const TRect& aRect);

	virtual void	InitialiseL();

	inline TInt					GetHandle() const;
	inline TInt					GetIdentifier() const;
	inline const TWsGraphicId&	GetId();
	
protected:
	CDataWrapperBase();
	virtual ~CDataWrapperBase();

	inline RFs&	FileServer();
	inline void	SetHandle(TInt aHandle);
	inline void SetIdentifier(TInt aIdentifier);
	inline void	SetId(const TWsGraphicId&);
protected:
	TBool                   iGraphicsOrientation[4];
private:
	TBool	GetCommandStringParameterL(const TDesC& aSectName, const TDesC& aKeyName, TPtrC& aResult);

private:
	// Included ini files
	RPointerArray<CIniData>	iInclude;
	RPointerArray<HBufC>	iBuffer;
	RFs                     iFs;
	TInt					iHandle;
	TInt					iIdentifier;
	TWsGraphicId			iId;
	};

#include "DataWrapperBase.inl"

#endif /* __DATA_WRAPPER_BASE__ */
