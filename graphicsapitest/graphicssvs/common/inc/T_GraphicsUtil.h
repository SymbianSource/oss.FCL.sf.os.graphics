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


#if (!defined __T_GRAPHICS_UTIL_H__)
#define __T_GRAPHICS_UTIL_H__

//	User includes
#include "DataWrapperBase.h"

//	EPOC includes
#include <gdi.h>
#include <fntstore.h>
#include <bitmap.h>
#include <w32std.h>
#include <icl/imagedata.h>
#include <w32stdgraphic.h>	// for TWsGraphicAnimation

class CT_GraphicsUtil
	{
public:
	static TBool 	ReadBitmapfileCompressionScheme(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TBitmapfileCompressionScheme& aCompressionScheme);
	static TBool 	ReadBrushStyle(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TBrushStyle& aBrushStyle);
	static TBool 	ReadCaptureFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aCaptureFlags);
	static TBool	ReadComputeMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsSession::TComputeMode& aComputeMode);
	static TBool	ReadCornerType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TCornerType& aCornerType);
	static TBool	ReadDisplayMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TDisplayMode& aDisplayMode);
	static TBool 	ReadDrawMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TDrawMode& aDrawMode);
	static TBool 	ReadErrorCategory(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsErrorMessage::TErrorCategory& aErrorCategory);
	static TBool 	ReadEventCode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TEventCode& aEventCode);
	static TBool 	ReadEventControl(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TEventControl& aEventControl);
	static TBool 	ReadEventModifier(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TEventModifier& aEventModifier);
	static TBool 	ReadEventModifier(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aEventModifier);
	static TBool 	ReadFadeControl(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWindowTreeNode::TFadeControl& aFadeControl);
	static TBool 	ReadFillRule(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TFillRule& aFillRule);
	static TBool 	ReadFontStrikethrough(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TFontStrikethrough& aFontStrikethrough);
	static TBool 	ReadFontUnderline(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TFontUnderline& aFontUnderline);
	static TBool 	ReadGlyphBitmapType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TGlyphBitmapType& aGlyphBitmapType);
	static TBool 	ReadGraphicsOrientation(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsBitGc::TGraphicsOrientation& aGraphicsOrientation);
	static TBool 	ReadHotKey(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, THotKey& aHotKey);
	static TBool 	ReadKeyCode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TKeyCode& aKeyCode);
	static TBool	ReadLoggingCommand(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsSession::TLoggingCommand& aLoggingCommand);
	static TBool 	ReadModifierState(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TModifierState& aModifierState);
	static TBool 	ReadPasswordMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPasswordMode& aPasswordMode);
	static TBool 	ReadPenStyle(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TPenStyle& aPenStyle);
	static TBool 	ReadPointerCursorMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPointerCursorMode& aPointerCursorMode);
	static TBool 	ReadPointerFilter(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPointerFilter& aPointerFilter);
	static TBool 	ReadPointerFilter(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aPointerFilter);
	//For MAnim
	static TBool 	ReadAnimCommand(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TInt& aCommand);
	static TBool 	ReadAnimType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TInt& aType);
	static TBool 	ReadTUsage(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CPolygonFiller::TUsage& aUsage);
#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
	static TBool 	ReadPriorities(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TInt& aPriorities);
#endif
	static TBool 	ReadPriority(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CActive::TPriority& aPriority);
	static TBool 	ReadScreenModeEnforcement(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TScreenModeEnforcement& aScreenModeEnforcement);
	static TBool 	ReadSpriteInCompare(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aSpriteInCompare);
	static TBool 	ReadStdScanCode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TStdScanCode& aStdScanCode);
	static TBool 	ReadTextAlign(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TTextAlign& aTextAlign);
	static TBool 	ReadTextDirection(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFont::TTextDirection& aTextDirection);
	static TBool 	ReadType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPointerEvent::TType& aType);
	static TBool 	ReadType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TRawEvent::TType& aType);
	static TBool 	ReadWindowBackupType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aWindowBackupType);
	static TBool 	ReadWsTransparencyPolicy(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsTransparencyPolicy& aWsTransparencyPolicy);
	static TBool 	ReadWsVisibilityChangedEvent(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsVisibilityChangedEvent& aWsVisibilityChangedEvent);
	static TBool	ReadLongCaptureFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TLongCaptureFlags& aLongCaptureFlags);
	static TBool	ReadSpriteFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TSpriteFlags& aSpriteFlags);
    static TBool	ReadSpriteFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aSpriteFlags);
	static TBool	ReadCustomTextCursorAlignment(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsSession::TCustomTextCursorAlignment& aAlignment);
	static TBool	ReadTypefaceSupport(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TTypefaceSupport& aTypefaceSupport);
	static TBool	GetAlgStyleL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TAlgStyle*& aData);
	static TBool	GetDrawableWindowL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RDrawableWindow*& aData);
	static TBool	GetDrawTextExtendedParamL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TDrawTextExtendedParam*& aData);
	static TBool	GetFbsBitmapL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsBitmap*& aData);
	static TBool	GetFbsDeviceL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsDevice*& aData);
	static TBool	GetFontL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFont*& aData);
	static TBool	GetPaletteL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CPalette*& aData);
	static TBool	GetWsBitmapL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CWsBitmap*& aData);
	static TBool	GetWsEventL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsEvent*& aData);
	static TBool	GetWsGraphicMsgBufL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsGraphicMsgBuf*& aData);
	static TBool	GetWsScreenDeviceL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CWsScreenDevice*& aData);
	static TBool	GetWsGraphicMsgFixedBaseL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsGraphicMsgFixedBase*& aData);
	static TBool	GetWsPriorityKeyEventL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsPriorityKeyEvent*& aData);
	static TBool	GetWsRedrawEventL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsRedrawEvent*& aData);
	static TBool	GetTextCursor(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TTextCursor& aTextCursor);
	static TBool	GetSpriteMemberL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TSpriteMember& aSpriteMember);
	static TBool	GetSpriteMemberListL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CArrayFix<TSpriteMember>& aResult);
	static TBool    BringAppForegroundL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aSessionKeyName , const TDesC& aAppKeyName);
	static TBool    ReadTerminateReason(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RDirectScreenAccess::TTerminationReasons& aReason);
    static void     EatupMemory(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName);
    static void     FreeEatenMemory(CDataWrapperBase& aDataWrapper);
    static TBool    GetFrameInfo(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, TFrameInfo& frameinfo);
    static TBool    ReadFrameInfoFlags(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName,TUint32& aFlags);
    static TBool    GetFrameInfoState(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName,TFrameInfo::TFrameInfoState& aState);
    static TBool    GetWsGraphicAnimationL(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName, TWsGraphicAnimation*& aData);
    static TBool    StartAnimTesterClientL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName);
    static TBool    CloseAnimTesterClientL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName);
    static TBool    GetFbsBitGcL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsBitGc*& aData);
    static TBool    GetTextParametersL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TTextParameters* aParam);
    static TBool    GetGraphicsOrientation(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName,CFbsBitGc::TGraphicsOrientation& aParam);
    static TBool	GetLinkedTypefaceSpecificationL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CLinkedTypefaceSpecification*& aData);
    static TBool	GetRasterizerL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, COpenFontRasterizer*& aData);
    static TBool	GetShaperFactoryL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CShaperFactory*& aData);
    static TBool	GetOpenFontGlyphDataL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TOpenFontGlyphData*& aData);
private:
	static TBool	ConvertToCaptureFlags(const TDesC& aStr, RWindowBase::TCaptureFlags& aCaptureFlags);
	static TBool	ConvertToCaptureFlags(const TDesC& aStr, TUint& aCaptureFlags);

	static TBool	ConvertToEventModifier(const TDesC& aStr, TEventModifier& aEventModifier);
	static TBool	ConvertToEventModifier(const TDesC& aStr, TUint& aEventModifier);

	static TBool	ConvertToPointerFilter(const TDesC& aStr, TPointerFilter& aPointerFilter);
	static TBool	ConvertToPointerFilter(const TDesC& aStr, TUint& aPointerFilter);

	static TBool	ConvertToSpriteInCompare(const TDesC& aStr, TUint& aSpriteInCompare);

	static TBool	ConvertToWindowBackupType(const TDesC& aStr, TWindowBackupType& aWindowBackupType);
	static TBool	ConvertToWindowBackupType(const TDesC& aStr, TUint& aWindowBackupType);

	static TBool	ConvertToWsVisibilityChangedEvent(const TDesC& aStr, TUint& aWsVisibilityChangedEvent);

	static TBool	ConvertToSpriteFlags(const TDesC& aStr, TSpriteFlags& aSpriteFlags);
	static TBool	ConvertToSpriteFlags(const TDesC& aStr, TUint& aSpriteFlags);
	static TBool    ConvertToFrameInfoFlags(const TDesC& aStr,TUint32& aFrameInfoFlags);
private:
 
	static TInt iAvaiableSpace;
	static TInt iBiggestBlock;
	static RPointerArray < TAny > iMemArray;
	};

#endif /* __T_GRAPHICS_UTIL_H__ */
