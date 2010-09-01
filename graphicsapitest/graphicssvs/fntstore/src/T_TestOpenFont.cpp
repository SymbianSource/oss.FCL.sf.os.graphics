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

#include <e32std.h>
#include <f32file.h>
#include "T_TestOpenFont.h"

CTestRasterizer* CTestRasterizer::NewL()
	{
	return new(ELeave) CTestRasterizer;
	}

COpenFontFile* CTestRasterizer::NewFontFileL(TInt aUid,const TDesC& aFileName,RFs& /*aFileSession*/)
	{
	TParse parse;
	parse.Set(aFileName, NULL, NULL);

	if (parse.Name().CompareF(KTestFont1) == 0 || parse.Name().CompareF(KTestFont2) == 0)
		{
		// create a dummy font.
		return CTestFontFile::NewL(aUid,aFileName);
		}
	else
		{
		return NULL;	
		}	
	}

CTestFontFile::CTestFontFile(TInt aUid, const TDesC& aFileName)
: COpenFontFile(aUid, aFileName)
	{
	}

CTestFontFile* CTestFontFile::NewL(TInt aUid, const TDesC& aFileName)
	{
	CTestFontFile* self = new(ELeave)CTestFontFile(aUid,aFileName);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

TBool CTestFontFile::GetNearestFontHelper(const TOpenFontSpec& aDesiredFontSpec, TInt aPixelWidth, TInt aPixelHeight,
											  TInt& aFaceIndex, TOpenFontSpec& aActualFontSpec) const
	{
	return COpenFontFile::GetNearestFontHelper(aDesiredFontSpec, aPixelWidth, aPixelHeight, aFaceIndex, aActualFontSpec);
	}

void CTestFontFile::ConstructL()
	{
	const TDesC& filename = FileName();
	TOpenFontFaceAttrib attrib;

	if (filename.CompareF(KTestFont1) == 0)
		attrib.SetFullName(_L("Test Font1"));
	else if (filename.CompareF(KTestFont2) == 0)
		{
		attrib.SetFullName(_L("Test Font2"));
		attrib.SetBold(TRUE);
		attrib.SetItalic(TRUE);
		}
	attrib.SetFamilyName(_L("Test"));
	attrib.SetLocalFullName(attrib.FullName());
	attrib.SetCoverage(TOpenFontFaceAttrib::ELatinSet);
	attrib.SetMinSizeInPixels(8);
	AddFaceL(attrib);
	}

void CTestFontFile::GetNearestFontInPixelsL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,
											 const TOpenFontSpec& aDesiredFontSpec,TInt aPixelWidth,TInt aPixelHeight,
											 COpenFont*& aFont,TOpenFontSpec& aActualFontSpec)
	{
	aFont = NULL;
	TInt face_index = 0;
	if (GetNearestFontHelper(aDesiredFontSpec,aPixelWidth,aPixelHeight,face_index,aActualFontSpec))
		{
		aFont = CTestFont::NewL(aHeap,aSessionCacheList,this,aActualFontSpec.Height());
		}
	}

void CTestFontFile::GetNearestFontToDesignHeightInPixelsL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,
											 const TOpenFontSpec& aDesiredFontSpec,TInt aPixelWidth,TInt aPixelHeight,
											 COpenFont*& aFont,TOpenFontSpec& aActualFontSpec)
	{
	aFont = NULL;
	TInt face_index = 0;
	if (GetNearestFontHelper(aDesiredFontSpec,aPixelWidth,aPixelHeight,face_index,aActualFontSpec))
		{
		aFont = CTestFont::NewL(aHeap,aSessionCacheList,this,aActualFontSpec.Height());
		}
	}

void CTestFontFile::GetNearestFontToMaxHeightInPixelsL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,
											 const TOpenFontSpec& aDesiredFontSpec,TInt aPixelWidth,TInt aPixelHeight,
											 COpenFont*& aFont,TOpenFontSpec& aActualFontSpec,TInt /*aMaxHeight*/)
	{
	aFont = NULL;
	TInt face_index = 0;
	if (GetNearestFontHelper(aDesiredFontSpec,aPixelWidth,aPixelHeight,face_index,aActualFontSpec))
		{
		aFont = CTestFont::NewL(aHeap,aSessionCacheList,this,aActualFontSpec.Height());
		}
	}

TBool CTestFontFile::HasUnicodeCharacterL(TInt /*aFaceIndex*/,TInt /*aCode*/) const
	{
	// has no characters
	return EFalse;
	}

CTestFont* CTestFont::NewL(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,
							 CTestFontFile* aFontFile,TInt aSizeInPixels)
	{
	CTestFont* f = (CTestFont*)aHeap->AllocL(sizeof(CTestFont));
	new(f) CTestFont(aHeap,aSessionCacheList,aFontFile,aSizeInPixels);
	return f;
	}

CTestFont::CTestFont(RHeap* aHeap,COpenFontSessionCacheList* aSessionCacheList,CTestFontFile* aFontFile,
					   TInt aSizeInPixels):
	COpenFont(aHeap,aSessionCacheList,aFontFile)
	{
	iMetrics.SetSize(aSizeInPixels);
	iMetrics.SetAscent(aSizeInPixels * 3 / 4);
	iMetrics.SetDescent(aSizeInPixels - iMetrics.Ascent());
	iMetrics.SetMaxHeight(iMetrics.Ascent());
	iMetrics.SetMaxDepth(iMetrics.Descent());
	iMetrics.SetMaxWidth(aSizeInPixels * 2);
	}

void CTestFont::RasterizeL(TInt /*aCode*/,TOpenFontGlyphData* /*aGlyphData*/)
	{
	// has no glyphs
	User::Leave(KErrNotSupported); 
	}


CTestShaperFactory* CTestShaperFactory::NewL()
	{
	return new(ELeave) CTestShaperFactory;	
	}

CTestShaperFactory::CTestShaperFactory()
	{
	}

CTestShaperFactory::~CTestShaperFactory()
	{	
	}


CShaper* CTestShaperFactory::NewShaperL(CBitmapFont* aBitmapfont, TInt aScript, TInt aLanguage, RHeap* aHeap)
	{
	return CTestShaper::NewL(aBitmapfont, aScript, aLanguage, aHeap);
	} 

void* CShaperFactory::ExtendedInterface(TUid /*aInterfaceId*/)
	{
	return 0;
	}


CShaper * CTestShaper::NewL(CBitmapFont* aBitmapfont, TInt aScript, TInt aLanguage, RHeap* aHeap)
	{
	CTestShaper* self = new(ELeave) CTestShaper();
	CleanupStack::PushL(self);
	TInt error = self->ConstructL(aBitmapfont, aScript, aLanguage, aHeap);
	if (KErrNone == error)
		{
		CleanupStack::Pop();
		return self;
		}
	else
		{
		CleanupStack::PopAndDestroy(); 
		return NULL;		
		}		
	} 

 TInt CTestShaper::ConstructL(CBitmapFont* /*aBitmapfont*/, TInt /*aScript*/, TInt /*aLanguage*/, RHeap* /*aHeap*/ )
	{
	return KErrNone;
	}

CTestShaper::CTestShaper()
	{
	}	
		
 CTestShaper::~CTestShaper()
	{
	}	

TInt CTestShaper::ShapeText(TShapeHeader*& /*aOutput*/, const TInput& /*aInput*/, RHeap* /*aHeapForOutput*/)
	{
 	return KErrNotSupported;
	}

