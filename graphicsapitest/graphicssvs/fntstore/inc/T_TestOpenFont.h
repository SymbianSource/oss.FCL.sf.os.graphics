// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Creates a dummy rasterizer and a dummy shaper for FontStore APIs testing.
// 
//

#ifndef __T_TESTOPENFONT_H__
#define __T_TESTOPENFONT_H__

#include <fntstore.h>
#include <openfont.h>
#include <e32base.h>

_LIT(KTestFont1, "dummy");
_LIT(KTestFont2, "dummy_2");


class CTestRasterizer : public COpenFontRasterizer
	{
public:
	static CTestRasterizer* NewL();
	COpenFontFile* NewFontFileL(TInt aUid, const TDesC& aFileName, RFs& aFileSession);
	};
	

class CTestFontFile: public COpenFontFile
	{
public:
	static CTestFontFile* NewL(TInt aUid,const TDesC& aFileName);

	TBool GetNearestFontHelper(const TOpenFontSpec& aDesiredFontSpec, TInt aPixelWidth, TInt aPixelHeight,
										TInt& aFaceIndex, TOpenFontSpec& aActualFontSpec) const;
	void GetNearestFontInPixelsL(RHeap* aHeap, COpenFontSessionCacheList* aSessionCacheList,
								 const TOpenFontSpec& aDesiredFontSpec, TInt aPixelWidth, TInt aPixelHeight,
								 COpenFont*& aFont,TOpenFontSpec& aActualFontSpec);
	void GetNearestFontToDesignHeightInPixelsL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,
								 const TOpenFontSpec& aDesiredFontSpec,TInt aPixelWidth,TInt aPixelHeight,
								 COpenFont*& aFont,TOpenFontSpec& aActualFontSpec);
	void GetNearestFontToMaxHeightInPixelsL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,
								 const TOpenFontSpec& aDesiredFontSpec,TInt aPixelWidth,TInt aPixelHeight,
								 COpenFont*& aFont,TOpenFontSpec& aActualFontSpec, TInt aMaxHeight);
	virtual TBool HasUnicodeCharacterL(TInt aFaceIndex,TInt aCode) const;

private:
	CTestFontFile(TInt aUid,const TDesC& aFileName);
	void ConstructL();
	};


class CTestFont: public COpenFont
	{
public:
	static CTestFont* NewL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,CTestFontFile* aFontFile,TInt aSizeInPixels);
private:
	CTestFont(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,CTestFontFile* aFontFile,TInt aSizeInPixels);
	virtual void RasterizeL(TInt aCode,TOpenFontGlyphData* aGlyphData);
	};


class CTestShaperFactory: public CShaperFactory
	{
public:
	static CTestShaperFactory* NewL();
	virtual ~CTestShaperFactory();
	virtual CShaper* NewShaperL(CBitmapFont* aBitmapfont, TInt aScript, TInt aLanguage, RHeap* aHeap);

private:
	CTestShaperFactory();
	};
	

class CTestShaper : public CShaper
	{
public:

	static CShaper* NewL(CBitmapFont* aBitmapfont, TInt aScript, TInt aLanguage, RHeap* aHeap);
	~CTestShaper();	
	virtual TInt ShapeText(TShapeHeader*& aOutput, const TInput& aInput, RHeap* aHeapForOutput);
		
private:
	CTestShaper();
	TInt ConstructL(CBitmapFont* aBitmapfont, TInt aScript, TInt aLanguage, RHeap* iHeap);
	TInt TestShapeTextL(TShapeHeader*& aOutput, const TInput& aInput, RHeap* aHeapForOutput);
	
private:	
	RHeap* iClientHeap;
	RHeap* iHeap;
	TAny* iHeapMemory;
 	TInt32* iGlyphs;
    TInt32* iIndices;
    TInt32* iPositions;    
    };

#endif // __T_TESTOPENFONT_H__

