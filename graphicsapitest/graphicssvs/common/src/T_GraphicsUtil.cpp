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

#include "DataWrapperBase.h"
#include "T_GraphicsUtil.h"
#include "../../wserv/AnimPlugin/inc/T_AnimDef.h"

//	EPOC includes
#include <apgtask.h>		// used for apatask

#define INFO_PRINTF1_UTIL(p1)					aDataWrapper.Logger().LogExtra(((TText8*)__FILE__), __LINE__, ESevrInfo, (p1))
#define INFO_PRINTF2_UTIL(p1, p2)					aDataWrapper.Logger().LogExtra(((TText8*)__FILE__), __LINE__, ESevrInfo, (p1), (p2))
/*@{*/
_LIT(KFormatEntryField,							"%S_%S");
_LIT(KFormatFieldNumber,						"%S%d");

_LIT(KTagTextCursorType,						"type");
_LIT(KTagTextCursorHeight,						"height");
_LIT(KTagTextCursorAscent,						"ascent");
_LIT(KTagTextCursorWidth,						"width");
_LIT(KTagTextCursorFlags,						"flags");
_LIT(KTagTextCursorColor,						"color");

_LIT(KTagSpriteMemberBitmap,					"bitmap");
_LIT(KTagSpriteMemberMaskBitmap,				"maskbitmap");
_LIT(KTagSpriteMemberInvertMask,				"invertmask");
_LIT(KTagSpriteMemberDrawMode,					"drawmode");
_LIT(KTagSpriteMemberOffset,					"offset");
_LIT(KTagSpriteMemberInterval,					"interval");
_LIT(KTagParamStart,							"start");
_LIT(KTagParamEnd,							    "end");
_LIT(KTagParamFlags,							"flags");
_LIT(KTagParamEnabled,							"enabled");

///	Enumerations type parameter names and their possible values
_LIT(KRLECompression,							"ERLECompression");
_LIT(KPaletteCompression,						"EPaletteCompression");
_LIT(KPaletteCompressionWithRLEFallback,		"EPaletteCompressionWithRLEFallback");

_LIT(KBrushStyleNull,							"ENullBrush");
_LIT(KBrushStyleSolid,							"ESolidBrush");
_LIT(KBrushStylePatterned,						"EPatternedBrush");
_LIT(KBrushStyleVerticalHatch,					"EVerticalHatchBrush");
_LIT(KBrushStyleForwardDiagonalHatch,			"EForwardDiagonalHatchBrush");
_LIT(KBrushStyleHorizontalHatch,				"EHorizontalHatchBrush");
_LIT(KBrushStyleRearwardDiagonalHatch,			"ERearwardDiagonalHatchBrush");
_LIT(KBrushStyleSquareCrossHatch,				"ESquareCrossHatchBrush");
_LIT(KBrushStyleDiamondCrossHatch,				"EDiamondCrossHatchBrush");

_LIT(KCaptureFlagEnabled,						"TCaptureFlagEnabled");
_LIT(KCaptureFlagDragDrop,						"TCaptureFlagDragDrop");
_LIT(KCaptureFlagAllGroups,						"TCaptureFlagAllGroups");
_LIT(KCaptureDisabled,							"TCaptureDisabled");
_LIT(KCaptureEnabled,							"TCaptureEnabled");
_LIT(KCaptureDragDrop,							"TCaptureDragDrop");

_LIT(KComputeModePriorityControlDisabled,		"EPriorityControlDisabled");
_LIT(KComputeModePriorityControlComputeOn,		"EPriorityControlComputeOn");
_LIT(KComputeModePriorityControlComputeOff,		"EPriorityControlComputeOff");

_LIT(KCornerTypeWindowSquare,					"EWindowCornerSquare");
_LIT(KCornerTypeWindow1,						"EWindowCorner1");
_LIT(KCornerTypeWindow2,						"EWindowCorner2");
_LIT(KCornerTypeWindow3,						"EWindowCorner3");
_LIT(KCornerTypeWindow5,						"EWindowCorner5");
_LIT(KCornerTypeWindowRegion,					"EWindowCornerRegion");
_LIT(KCornerTypeMask,							"ECornerTypeMask");

_LIT(KDisplayModeNone,							"ENone");
_LIT(KDisplayModeGray2,							"EGray2");
_LIT(KDisplayModeGray4,							"EGray4");
_LIT(KDisplayModeGray16,						"EGray16");
_LIT(KDisplayModeGray256,						"EGray256");
_LIT(KDisplayModeColor16,						"EColor16");
_LIT(KDisplayModeColor256,						"EColor256");
_LIT(KDisplayModeColor64K,						"EColor64K");
_LIT(KDisplayModeColor16M,						"EColor16M");
_LIT(KDisplayModeRgb,							"ERgb");
_LIT(KDisplayModeColor4K,						"EColor4K");
_LIT(KDisplayModeColor16MU,						"EColor16MU");
_LIT(KDisplayModeColor16MA,						"EColor16MA");
_LIT(KDisplayModeColor16MAP,					"EColor16MAP");
_LIT(KDisplayModeColorLast,						"EColorLast");

_LIT(KDrawModeAND,								"EDrawModeAND");
_LIT(KDrawModeNOTAND,							"EDrawModeNOTAND");
_LIT(KDrawModePEN,								"EDrawModePEN");
_LIT(KDrawModeANDNOT,							"EDrawModeANDNOT");
_LIT(KDrawModeXOR,								"EDrawModeXOR");
_LIT(KDrawModeOR,								"EDrawModeOR");
_LIT(KDrawModeNOTANDNOT,						"EDrawModeNOTANDNOT");
_LIT(KDrawModeNOTXOR,							"EDrawModeNOTXOR");
_LIT(KDrawModeNOTSCREEN,						"EDrawModeNOTSCREEN");
_LIT(KDrawModeNOTOR,							"EDrawModeNOTOR");
_LIT(KDrawModeNOTPEN,							"EDrawModeNOTPEN");
_LIT(KDrawModeORNOT,							"EDrawModeORNOT");
_LIT(KDrawModeNOTORNOT,							"EDrawModeNOTORNOT");
_LIT(KDrawModeWriteAlpha,						"EDrawModeWriteAlpha");

_LIT(KErrorCategoryDrawingRegion,				"EDrawingRegion");
_LIT(KErrorCategoryBackLight,					"EBackLight");
_LIT(KErrorCategoryLogging,						"ELogging");
_LIT(KErrorCategoryContrast,					"EContrast");

_LIT(KEventCodeNull,							"EEventNull");
_LIT(KEventCodeKey,								"EEventKey");
_LIT(KEventCodeKeyUp,							"EEventKeyUp");
_LIT(KEventCodeKeyDown,							"EEventKeyDown");
_LIT(KEventCodeModifiersChanged,				"EEventModifiersChanged");
_LIT(KEventCodePointer,							"EEventPointer");
_LIT(KEventCodePointerEnter,					"EEventPointerEnter");
_LIT(KEventCodePointerExit,						"EEventPointerExit");
_LIT(KEventCodePointerBufferReady,				"EEventPointerBufferReady");
_LIT(KEventCodeDragDrop,						"EEventDragDrop");
_LIT(KEventCodeFocusLost,						"EEventFocusLost");
_LIT(KEventCodeFocusGained,						"EEventFocusGained");
_LIT(KEventCodeSwitchOn,						"EEventSwitchOn");
_LIT(KEventCodePassword,						"EEventPassword");
_LIT(KEventCodeWindowGroupsChanged,				"EEventWindowGroupsChanged");
_LIT(KEventCodeErrorMessage,					"EEventErrorMessage");
_LIT(KEventCodeMessageReady,					"EEventMessageReady");
_LIT(KEventCodeMarkInvalid,						"EEventMarkInvalid");
_LIT(KEventCodeSwitchOff,						"EEventSwitchOff");
_LIT(KEventCodeKeySwitchOff,					"EEventKeySwitchOff");
_LIT(KEventCodeScreenDeviceChanged,				"EEventScreenDeviceChanged");
_LIT(KEventCodeFocusGroupChanged,				"EEventFocusGroupChanged");
_LIT(KEventCodeCaseOpened,						"EEventCaseOpened");
_LIT(KEventCodeCaseClosed,						"EEventCaseClosed");
_LIT(KEventCodeWindowGroupListChanged,			"EEventWindowGroupListChanged");
_LIT(KEventCodeWindowVisibilityChanged,			"EEventWindowVisibilityChanged");
#if (defined SYMBIAN_PROCESS_MONITORING_AND_STARTUP)
_LIT(KEventCodeRestartSystem,					"EEventRestartSystem");
#endif
_LIT(KEventCodeKeyRepeat,						"EEventKeyRepeat");
_LIT(KEventCodeDirectScreenAccessBegin,			"EEventDirectScreenAccessBegin");
_LIT(KEventCodeDirectScreenAccessEnd,			"EEventDirectScreenAccessEnd");
_LIT(KEventCodeHeartbeatTimerStateChange,		"EEventHeartbeatTimerStateChange");
_LIT(KEventCodePowerMgmt,						"EEventPowerMgmt");
_LIT(KEventCodeReserved,						"EEventReserved");
_LIT(KEventCodeUser,							"EEventUser");

_LIT(KEventControlAlways,						"EEventControlAlways");
_LIT(KEventControlOnlyWithKeyboardFocus,		"EEventControlOnlyWithKeyboardFocus");
_LIT(KEventControlOnlyWhenVisible,				"EEventControlOnlyWhenVisible");

_LIT(KEventModifierAutorepeatable,				"EModifierAutorepeatable");
_LIT(KEventModifierKeypad,						"EModifierKeypad");
_LIT(KEventModifierLeftAlt,						"EModifierLeftAlt");
_LIT(KEventModifierRightAlt,					"EModifierRightAlt");
_LIT(KEventModifierAlt,							"EModifierAlt");
_LIT(KEventModifierLeftCtrl,					"EModifierLeftCtrl");
_LIT(KEventModifierRightCtrl,					"EModifierRightCtrl");
_LIT(KEventModifierCtrl,						"EModifierCtrl");
_LIT(KEventModifierLeftShift,					"EModifierLeftShift");
_LIT(KEventModifierRightShift,					"EModifierRightShift");
_LIT(KEventModifierShift,						"EModifierShift");
_LIT(KEventModifierLeftFunc,					"EModifierLeftFunc");
_LIT(KEventModifierRightFunc,					"EModifierRightFunc");
_LIT(KEventModifierFunc,						"EModifierFunc");
_LIT(KEventModifierCapsLock,					"EModifierCapsLock");
_LIT(KEventModifierNumLock,						"EModifierNumLock");
_LIT(KEventModifierScrollLock,					"EModifierScrollLock");
_LIT(KEventModifierKeyUp,						"EModifierKeyUp");
_LIT(KEventModifierSpecial,						"EModifierSpecial");
_LIT(KEventModifierDoubleClick,					"EModifierDoubleClick");
_LIT(KEventModifierPureKeycode,					"EModifierPureKeycode");
_LIT(KEventModifierKeyboardExtend,				"EModifierKeyboardExtend");
_LIT(KEventModifierCancelRotation,				"EModifierCancelRotation");
_LIT(KEventModifierRotateBy90,					"EModifierRotateBy90");
_LIT(KEventModifierRotateBy180,					"EModifierRotateBy180");
_LIT(KEventModifierRotateBy270,					"EModifierRotateBy270");
_LIT(KEventModifierPointer3DButton1,			"EModifierPointer3DButton1");
_LIT(KEventModifierPointer3DButton2,			"EModifierPointer3DButton2");
_LIT(KEventModifierPointer3DButton3,			"EModifierPointer3DButton3");
_LIT(KEventModifierAll,							"EAllModifiers");

_LIT(KFadeControlIncludeChildren,				"EFadeIncludeChildren");
_LIT(KFadeControlWindowOnly,					"EFadeWindowOnly");

_LIT(KFillRuleAlternate,						"EAlternate");
_LIT(KFillRuleEWinding,							"EWinding");

_LIT(KFontStrikethroughOff,						"EStrikethroughOff");
_LIT(KFontStrikethroughOn,						"EStrikethroughOn");

_LIT(KFontUnderlineOff,							"EUnderlineOff");
_LIT(KFontUnderlineOn,							"EUnderlineOn");

_LIT(KGlyphBitmapTypeDefault,					"EDefaultGlyphBitmap");
_LIT(KGlyphBitmapTypeMonochrome,				"EMonochromeGlyphBitmap");
_LIT(KGlyphBitmapTypeAntiAliased,				"EAntiAliasedGlyphBitmap");
_LIT(KGlyphBitmapTypeSubPixel,					"ESubPixelGlyphBitmap");
_LIT(KGlyphBitmapTypeFourColourBlend,			"EFourColourBlendGlyphBitmap");

_LIT(KGraphicsOrientationNormal,				"EGraphicsOrientationNormal");
_LIT(KGraphicsOrientationRotated90,				"EGraphicsOrientationRotated90");
_LIT(KGraphicsOrientationRotated180,			"EGraphicsOrientationRotated180");
_LIT(KGraphicsOrientationRotated270,			"EGraphicsOrientationRotated270");

_LIT(KHotKeyEnableLogging,						"EHotKeyEnableLogging");
_LIT(KHotKeyDisableLogging,						"EHotKeyDisableLogging");
_LIT(KHotKeyStateDump,							"EHotKeyStateDump");
_LIT(KHotKeyOfDeath,							"EHotKeyOfDeath");
_LIT(KHotKeyShutDown,							"EHotKeyShutDown");
_LIT(KHotKeyHeapDump,							"EHotKeyHeapDump");
_LIT(KHotKeyIncContrast,						"EHotKeyIncContrast");
_LIT(KHotKeyDecContrast,						"EHotKeyDecContrast");
_LIT(KHotKeyOff,								"EHotKeyOff");
_LIT(KHotKeyBacklightOn,						"EHotKeyBacklightOn");
_LIT(KHotKeyBacklightOff,						"EHotKeyBacklightOff");
_LIT(KHotKeyBacklightToggle,					"EHotKeyBacklightToggle");
_LIT(KHotKeyScreenDimension0,					"EHotKeyScreenDimension0");
_LIT(KHotKeyScreenDimension1,					"EHotKeyScreenDimension1");
_LIT(KHotKeyScreenDimension2,					"EHotKeyScreenDimension2");
_LIT(KHotKeyScreenDimension3,					"EHotKeyScreenDimension3");
_LIT(KHotKeyCycleDisplaySize,					"EHotKeyCycleDisplaySize");
_LIT(KHotKeyCycleOrientation,					"EHotKeyCycleOrientation");
_LIT(KHotKeyIncBrightness,						"EHotKeyIncBrightness");
_LIT(KHotKeyDecBrightness,						"EHotKeyDecBrightness");
_LIT(KHotKeyCycleFocusScreen,					"EHotKeyCycleFocusScreen");
_LIT(KHotKeyFirstKeyType,						"EHotKeyFirstKeyType");
_LIT(KHotKeyLastKeyType,						"EHotKeyLastKeyType");

_LIT(KKeyNull,									"EKeyNull");
_LIT(KKeyBell,									"EKeyBell");
_LIT(KKeyBackspace,								"EKeyBackspace");
_LIT(KKeyTab,									"EKeyTab");
_LIT(KKeyLineFeed,								"EKeyLineFeed");
_LIT(KKeyVerticalTab,							"EKeyVerticalTab");
_LIT(KKeyFormFeed,								"EKeyFormFeed");
_LIT(KKeyEnter,									"EKeyEnter");
_LIT(KKeyEscape,								"EKeyEscape");
_LIT(KKeySpace,									"EKeySpace");
_LIT(KKeyDelete,								"EKeyDelete");
_LIT(KKeyPrintScreen,							"EKeyPrintScreen");
_LIT(KKeyPause,									"EKeyPause");
_LIT(KKeyHome,									"EKeyHome");
_LIT(KKeyEnd,									"EKeyEnd");
_LIT(KKeyPageUp,								"EKeyPageUp");
_LIT(KKeyPageDown,								"EKeyPageDown");
_LIT(KKeyInsert,								"EKeyInsert");
_LIT(KKeyLeftArrow,								"EKeyLeftArrow");
_LIT(KKeyRightArrow,							"EKeyRightArrow");
_LIT(KKeyUpArrow,								"EKeyUpArrow");
_LIT(KKeyDownArrow,								"EKeyDownArrow");
_LIT(KKeyLeftShift,								"EKeyLeftShift");
_LIT(KKeyRightShift,							"EKeyRightShift");
_LIT(KKeyLeftAlt,								"EKeyLeftAlt");
_LIT(KKeyRightAlt,								"EKeyRightAlt");
_LIT(KKeyLeftCtrl,								"EKeyLeftCtrl");
_LIT(KKeyRightCtrl,								"EKeyRightCtrl");
_LIT(KKeyLeftFunc,								"EKeyLeftFunc");
_LIT(KKeyRightFunc,								"EKeyRightFunc");
_LIT(KKeyCapsLock,								"EKeyCapsLock");
_LIT(KKeyNumLock,								"EKeyNumLock");
_LIT(KKeyScrollLock,							"EKeyScrollLock");
_LIT(KKeyF1,									"EKeyF1");
_LIT(KKeyF2,									"EKeyF2");
_LIT(KKeyF3,									"EKeyF3");
_LIT(KKeyF4,									"EKeyF4");
_LIT(KKeyF5,									"EKeyF5");
_LIT(KKeyF6,									"EKeyF6");
_LIT(KKeyF7,									"EKeyF7");
_LIT(KKeyF8,									"EKeyF8");
_LIT(KKeyF9,									"EKeyF9");
_LIT(KKeyF10,									"EKeyF10");
_LIT(KKeyF11,									"EKeyF11");
_LIT(KKeyF12,									"EKeyF12");
_LIT(KKeyF13,									"EKeyF13");
_LIT(KKeyF14,									"EKeyF14");
_LIT(KKeyF15,									"EKeyF15");
_LIT(KKeyF16,									"EKeyF16");
_LIT(KKeyF17,									"EKeyF17");
_LIT(KKeyF18,									"EKeyF18");
_LIT(KKeyF19,									"EKeyF19");
_LIT(KKeyF20,									"EKeyF20");
_LIT(KKeyF21,									"EKeyF21");
_LIT(KKeyF22,									"EKeyF22");
_LIT(KKeyF23,									"EKeyF23");
_LIT(KKeyF24,									"EKeyF24");
_LIT(KKeyOff,									"EKeyOff");
_LIT(KKeyIncContrast,							"EKeyIncContrast");
_LIT(KKeyDecContrast,							"EKeyDecContrast");
_LIT(KKeyBacklightOn,							"EKeyBacklightOn");
_LIT(KKeyBacklightOff,							"EKeyBacklightOff");
_LIT(KKeyBacklightToggle,						"EKeyBacklightToggle");
_LIT(KKeySliderDown,							"EKeySliderDown");
_LIT(KKeySliderUp,								"EKeySliderUp");
_LIT(KKeyMenu,									"EKeyMenu");
_LIT(KKeyDictaphonePlay,						"EKeyDictaphonePlay");
_LIT(KKeyDictaphoneStop,						"EKeyDictaphoneStop");
_LIT(KKeyDictaphoneRecord,						"EKeyDictaphoneRecord");
_LIT(KKeyHelp,									"EKeyHelp");
_LIT(KKeyDial,									"EKeyDial");
_LIT(KKeyScreenDimension0,						"EKeyScreenDimension0");
_LIT(KKeyScreenDimension1,						"EKeyScreenDimension1");
_LIT(KKeyScreenDimension2,						"EKeyScreenDimension2");
_LIT(KKeyScreenDimension3,						"EKeyScreenDimension3");
_LIT(KKeyIncVolume,								"EKeyIncVolume");
_LIT(KKeyDecVolume,								"EKeyDecVolume");
_LIT(KKeyDevice0,								"EKeyDevice0");
_LIT(KKeyDevice1,								"EKeyDevice1");
_LIT(KKeyDevice2,								"EKeyDevice2");
_LIT(KKeyDevice3,								"EKeyDevice3");
_LIT(KKeyDevice4,								"EKeyDevice4");
_LIT(KKeyDevice5,								"EKeyDevice5");
_LIT(KKeyDevice6,								"EKeyDevice6");
_LIT(KKeyDevice7,								"EKeyDevice7");
_LIT(KKeyDevice8,								"EKeyDevice8");
_LIT(KKeyDevice9,								"EKeyDevice9");
_LIT(KKeyDeviceA,								"EKeyDeviceA");
_LIT(KKeyDeviceB,								"EKeyDeviceB");
_LIT(KKeyDeviceC,								"EKeyDeviceC");
_LIT(KKeyDeviceD,								"EKeyDeviceD");
_LIT(KKeyDeviceE,								"EKeyDeviceE");
_LIT(KKeyDeviceF,								"EKeyDeviceF");
_LIT(KKeyApplication0,							"EKeyApplication0");
_LIT(KKeyApplication1,							"EKeyApplication1");
_LIT(KKeyApplication2,							"EKeyApplication2");
_LIT(KKeyApplication3,							"EKeyApplication3");
_LIT(KKeyApplication4,							"EKeyApplication4");
_LIT(KKeyApplication5,							"EKeyApplication5");
_LIT(KKeyApplication6,							"EKeyApplication6");
_LIT(KKeyApplication7,							"EKeyApplication7");
_LIT(KKeyApplication8,							"EKeyApplication8");
_LIT(KKeyApplication9,							"EKeyApplication9");
_LIT(KKeyApplicationA,							"EKeyApplicationA");
_LIT(KKeyApplicationB,							"EKeyApplicationB");
_LIT(KKeyApplicationC,							"EKeyApplicationC");
_LIT(KKeyApplicationD,							"EKeyApplicationD");
_LIT(KKeyApplicationE,							"EKeyApplicationE");
_LIT(KKeyApplicationF,							"EKeyApplicationF");
_LIT(KKeyYes,									"EKeyYes");
_LIT(KKeyNo,									"EKeyNo");
_LIT(KKeyIncBrightness,							"EKeyIncBrightness");
_LIT(KKeyDecBrightness,							"EKeyDecBrightness");
_LIT(KKeyKeyboardExtend,						"EKeyKeyboardExtend");
_LIT(KKeyDevice10,								"EKeyDevice10");
_LIT(KKeyDevice11,								"EKeyDevice11");
_LIT(KKeyDevice12,								"EKeyDevice12");
_LIT(KKeyDevice13,								"EKeyDevice13");
_LIT(KKeyDevice14,								"EKeyDevice14");
_LIT(KKeyDevice15,								"EKeyDevice15");
_LIT(KKeyDevice16,								"EKeyDevice16");
_LIT(KKeyDevice17,								"EKeyDevice17");
_LIT(KKeyDevice18,								"EKeyDevice18");
_LIT(KKeyDevice19,								"EKeyDevice19");
_LIT(KKeyDevice1A,								"EKeyDevice1A");
_LIT(KKeyDevice1B,								"EKeyDevice1B");
_LIT(KKeyDevice1C,								"EKeyDevice1C");
_LIT(KKeyDevice1D,								"EKeyDevice1D");
_LIT(KKeyDevice1E,								"EKeyDevice1E");
_LIT(KKeyDevice1F,								"EKeyDevice1F");
_LIT(KKeyApplication10,							"EKeyApplication10");
_LIT(KKeyApplication11,							"EKeyApplication11");
_LIT(KKeyApplication12,							"EKeyApplication12");
_LIT(KKeyApplication13,							"EKeyApplication13");
_LIT(KKeyApplication14,							"EKeyApplication14");
_LIT(KKeyApplication15,							"EKeyApplication15");
_LIT(KKeyApplication16,							"EKeyApplication16");
_LIT(KKeyApplication17,							"EKeyApplication17");
_LIT(KKeyApplication18,							"EKeyApplication18");
_LIT(KKeyApplication19,							"EKeyApplication19");
_LIT(KKeyApplication1A,							"EKeyApplication1A");
_LIT(KKeyApplication1B,							"EKeyApplication1B");
_LIT(KKeyApplication1C,							"EKeyApplication1C");
_LIT(KKeyApplication1D,							"EKeyApplication1D");
_LIT(KKeyApplication1E,							"EKeyApplication1E");
_LIT(KKeyApplication1F,							"EKeyApplication1F");

_LIT(KLoggingEnable,							"ELoggingEnable");
_LIT(KLoggingDisable,							"ELoggingDisable");
_LIT(KLoggingStatusDump,						"ELoggingStatusDump");
_LIT(KLoggingHeapDump,							"ELoggingHeapDump");

_LIT(KModifierStateTurnOnModifier,				"ETurnOnModifier");
_LIT(KModifierStateTurnOffModifier,				"ETurnOffModifier");
_LIT(KModifierStateToggleModifier,				"EToggleModifier");

_LIT(KPasswordModeCancel,						"EPasswordCancel");
_LIT(KPasswordModeNone,							"EPasswordNone");
_LIT(KPasswordModeOnceADay,						"EPasswordOnceADay");
_LIT(KPasswordModeAlways,						"EPasswordAlways");
_LIT(KPasswordModeAlwaysTriggerNow,				"EPasswordAlwaysTriggerNow");
_LIT(KPasswordModeOnceADayTriggerNow,			"EPasswordOnceADayTriggerNow");

_LIT(KPenStyleNull,								"ENullPen");
_LIT(KPenStyleSolid,							"ESolidPen");
_LIT(KPenStyleDotted,							"EDottedPen");
_LIT(KPenStyleDashed,							"EDashedPen");
_LIT(KPenStyleDotDash,							"EDotDashPen");
_LIT(KPenStyleDotDotDash,						"EDotDotDashPen");

_LIT(KPointerCursorNone,						"EPointerCursorNone");
_LIT(KPointerCursorFixed,						"EPointerCursorFixed");
_LIT(KPointerCursorNormal,						"EPointerCursorNormal");
_LIT(KPointerCursorWindow,						"EPointerCursorWindow");
_LIT(KPointerCursorFirstMode,					"EPointerCursorFirstMode");
_LIT(KPointerCursorLastMode,					"EPointerCursorLastMode");

_LIT(KPointerFilterEnterExit,					"EPointerFilterEnterExit");
_LIT(KPointerFilterMove,						"EPointerFilterMove");
_LIT(KPointerFilterDrag,						"EPointerFilterDrag");
_LIT(KPointerFilterGenerateSimulatedMove,		"EPointerGenerateSimulatedMove");
_LIT(KPointerFilterMoveEvents,					"EPointerMoveEvents");

#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
_LIT(KPrioritiesAll,							"EAllPriorities");
#endif

_LIT(KPriorityIdle,								"EPriorityIdle");
_LIT(KPriorityLow,								"EPriorityLow");
_LIT(KPriorityStandard,							"EPriorityStandard");
_LIT(KPriorityUserInput,						"EPriorityUserInput");
_LIT(KPriorityHigh,								"EPriorityHigh");

_LIT(KSizeEnforcementNone,						"ESizeEnforcementNone");
_LIT(KSizeEnforcementPixelsAndRotation,			"ESizeEnforcementPixelsAndRotation");
_LIT(KSizeEnforcementPixelsTwipsAndRotation,	"ESizeEnforcementPixelsTwipsAndRotation");

_LIT(KSpriteInCompareRemoveSprite,				"ERemoveSprite");
_LIT(KSpriteInCompareIncludeSprite,				"EIncludeSprite");
_LIT(KSpriteInCompareIncludeTextCursor,			"EIncludeTextCursor");

_LIT(KStdKeyNull,								"EStdKeyNull");
_LIT(KStdKeyBackspace,							"EStdKeyBackspace");
_LIT(KStdKeyTab,								"EStdKeyTab");
_LIT(KStdKeyEnter,								"EStdKeyEnter");
_LIT(KStdKeyEscape,								"EStdKeyEscape");
_LIT(KStdKeySpace,								"EStdKeySpace");
_LIT(KStdKeyPrintScreen,						"EStdKeyPrintScreen");
_LIT(KStdKeyPause,								"EStdKeyPause");
_LIT(KStdKeyHome,								"EStdKeyHome");
_LIT(KStdKeyEnd,								"EStdKeyEnd");
_LIT(KStdKeyPageUp,								"EStdKeyPageUp");
_LIT(KStdKeyPageDown,							"EStdKeyPageDown");
_LIT(KStdKeyInsert,								"EStdKeyInsert");
_LIT(KStdKeyDelete,								"EStdKeyDelete");
_LIT(KStdKeyLeftArrow,							"EStdKeyLeftArrow");
_LIT(KStdKeyRightArrow,							"EStdKeyRightArrow");
_LIT(KStdKeyUpArrow,							"EStdKeyUpArrow");
_LIT(KStdKeyDownArrow,							"EStdKeyDownArrow");
_LIT(KStdKeyLeftShift,							"EStdKeyLeftShift");
_LIT(KStdKeyRightShift,							"EStdKeyRightShift");
_LIT(KStdKeyLeftAlt,							"EStdKeyLeftAlt");
_LIT(KStdKeyRightAlt,							"EStdKeyRightAlt");
_LIT(KStdKeyLeftCtrl,							"EStdKeyLeftCtrl");
_LIT(KStdKeyRightCtrl,							"EStdKeyRightCtrl");
_LIT(KStdKeyLeftFunc,							"EStdKeyLeftFunc");
_LIT(KStdKeyRightFunc,							"EStdKeyRightFunc");
_LIT(KStdKeyCapsLock,							"EStdKeyCapsLock");
_LIT(KStdKeyNumLock,							"EStdKeyNumLock");
_LIT(KStdKeyScrollLock,							"EStdKeyScrollLock");
_LIT(KStdKeyF1,									"EStdKeyF1");
_LIT(KStdKeyF2,									"EStdKeyF2");
_LIT(KStdKeyF3,									"EStdKeyF3");
_LIT(KStdKeyF4,									"EStdKeyF4");
_LIT(KStdKeyF5,									"EStdKeyF5");
_LIT(KStdKeyF6,									"EStdKeyF6");
_LIT(KStdKeyF7,									"EStdKeyF7");
_LIT(KStdKeyF8,									"EStdKeyF8");
_LIT(KStdKeyF9,									"EStdKeyF9");
_LIT(KStdKeyF10,								"EStdKeyF10");
_LIT(KStdKeyF11,								"EStdKeyF11");
_LIT(KStdKeyF12,								"EStdKeyF12");
_LIT(KStdKeyF13,								"EStdKeyF13");
_LIT(KStdKeyF14,								"EStdKeyF14");
_LIT(KStdKeyF15,								"EStdKeyF15");
_LIT(KStdKeyF16,								"EStdKeyF16");
_LIT(KStdKeyF17,								"EStdKeyF17");
_LIT(KStdKeyF18,								"EStdKeyF18");
_LIT(KStdKeyF19,								"EStdKeyF19");
_LIT(KStdKeyF20,								"EStdKeyF20");
_LIT(KStdKeyF21,								"EStdKeyF21");
_LIT(KStdKeyF22,								"EStdKeyF22");
_LIT(KStdKeyF23,								"EStdKeyF23");
_LIT(KStdKeyF24,								"EStdKeyF24");
_LIT(KStdKeyXXX,								"EStdKeyXXX");
_LIT(KStdKeyComma,								"EStdKeyComma");
_LIT(KStdKeyFullStop,							"EStdKeyFullStop");
_LIT(KStdKeyForwardSlash,						"EStdKeyForwardSlash");
_LIT(KStdKeyBackSlash,							"EStdKeyBackSlash");
_LIT(KStdKeySemiColon,							"EStdKeySemiColon");
_LIT(KStdKeySingleQuote,						"EStdKeySingleQuote");
_LIT(KStdKeyHash,								"EStdKeyHash");
_LIT(KStdKeySquareBracketLeft,					"EStdKeySquareBracketLeft");
_LIT(KStdKeySquareBracketRight,					"EStdKeySquareBracketRight");
_LIT(KStdKeyMinus,								"EStdKeyMinus");
_LIT(KStdKeyEquals,								"EStdKeyEquals");
_LIT(KStdKeyNkpForwardSlash,					"EStdKeyNkpForwardSlash");
_LIT(KStdKeyNkpAsterisk,						"EStdKeyNkpAsterisk");
_LIT(KStdKeyNkpMinus,							"EStdKeyNkpMinus");
_LIT(KStdKeyNkpPlus,							"EStdKeyNkpPlus");
_LIT(KStdKeyNkpEnter,							"EStdKeyNkpEnter");
_LIT(KStdKeyNkp1,								"EStdKeyNkp1");
_LIT(KStdKeyNkp2,								"EStdKeyNkp2");
_LIT(KStdKeyNkp3,								"EStdKeyNkp3");
_LIT(KStdKeyNkp4,								"EStdKeyNkp4");
_LIT(KStdKeyNkp5,								"EStdKeyNkp5");
_LIT(KStdKeyNkp6,								"EStdKeyNkp6");
_LIT(KStdKeyNkp7,								"EStdKeyNkp7");
_LIT(KStdKeyNkp8,								"EStdKeyNkp8");
_LIT(KStdKeyNkp9,								"EStdKeyNkp9");
_LIT(KStdKeyNkp0,								"EStdKeyNkp0");
_LIT(KStdKeyNkpFullStop,						"EStdKeyNkpFullStop");
_LIT(KStdKeyMenu,								"EStdKeyMenu");
_LIT(KStdKeyBacklightOn,						"EStdKeyBacklightOn");
_LIT(KStdKeyBacklightOff,						"EStdKeyBacklightOff");
_LIT(KStdKeyBacklightToggle,					"EStdKeyBacklightToggle");
_LIT(KStdKeyIncContrast,						"EStdKeyIncContrast");
_LIT(KStdKeyDecContrast,						"EStdKeyDecContrast");
_LIT(KStdKeySliderDown,							"EStdKeySliderDown");
_LIT(KStdKeySliderUp,							"EStdKeySliderUp");
_LIT(KStdKeyDictaphonePlay,						"EStdKeyDictaphonePlay");
_LIT(KStdKeyDictaphoneStop,						"EStdKeyDictaphoneStop");
_LIT(KStdKeyDictaphoneRecord,					"EStdKeyDictaphoneRecord");
_LIT(KStdKeyHelp,								"EStdKeyHelp");
_LIT(KStdKeyOff,								"EStdKeyOff");
_LIT(KStdKeyDial,								"EStdKeyDial");
_LIT(KStdKeyIncVolume,							"EStdKeyIncVolume");
_LIT(KStdKeyDecVolume,							"EStdKeyDecVolume");
_LIT(KStdKeyDevice0,							"EStdKeyDevice0");
_LIT(KStdKeyDevice1,							"EStdKeyDevice1");
_LIT(KStdKeyDevice2,							"EStdKeyDevice2");
_LIT(KStdKeyDevice3,							"EStdKeyDevice3");
_LIT(KStdKeyDevice4,							"EStdKeyDevice4");
_LIT(KStdKeyDevice5,							"EStdKeyDevice5");
_LIT(KStdKeyDevice6,							"EStdKeyDevice6");
_LIT(KStdKeyDevice7,							"EStdKeyDevice7");
_LIT(KStdKeyDevice8,							"EStdKeyDevice8");
_LIT(KStdKeyDevice9,							"EStdKeyDevice9");
_LIT(KStdKeyDeviceA,							"EStdKeyDeviceA");
_LIT(KStdKeyDeviceB,							"EStdKeyDeviceB");
_LIT(KStdKeyDeviceC,							"EStdKeyDeviceC");
_LIT(KStdKeyDeviceD,							"EStdKeyDeviceD");
_LIT(KStdKeyDeviceE,							"EStdKeyDeviceE");
_LIT(KStdKeyDeviceF,							"EStdKeyDeviceF");
_LIT(KStdKeyApplication0,						"EStdKeyApplication0");
_LIT(KStdKeyApplication1,						"EStdKeyApplication1");
_LIT(KStdKeyApplication2,						"EStdKeyApplication2");
_LIT(KStdKeyApplication3,						"EStdKeyApplication3");
_LIT(KStdKeyApplication4,						"EStdKeyApplication4");
_LIT(KStdKeyApplication5,						"EStdKeyApplication5");
_LIT(KStdKeyApplication6,						"EStdKeyApplication6");
_LIT(KStdKeyApplication7,						"EStdKeyApplication7");
_LIT(KStdKeyApplication8,						"EStdKeyApplication8");
_LIT(KStdKeyApplication9,						"EStdKeyApplication9");
_LIT(KStdKeyApplicationA,						"EStdKeyApplicationA");
_LIT(KStdKeyApplicationB,						"EStdKeyApplicationB");
_LIT(KStdKeyApplicationC,						"EStdKeyApplicationC");
_LIT(KStdKeyApplicationD,						"EStdKeyApplicationD");
_LIT(KStdKeyApplicationE,						"EStdKeyApplicationE");
_LIT(KStdKeyApplicationF,						"EStdKeyApplicationF");
_LIT(KStdKeyYes,								"EStdKeyYes");
_LIT(KStdKeyNo,									"EStdKeyNo");
_LIT(KStdKeyIncBrightness,						"EStdKeyIncBrightness");
_LIT(KStdKeyDecBrightness,						"EStdKeyDecBrightness");
_LIT(KStdKeyKeyboardExtend,						"EStdKeyKeyboardExtend");
_LIT(KStdKeyDevice10,							"EStdKeyDevice10");
_LIT(KStdKeyDevice11,							"EStdKeyDevice11");
_LIT(KStdKeyDevice12,							"EStdKeyDevice12");
_LIT(KStdKeyDevice13,							"EStdKeyDevice13");
_LIT(KStdKeyDevice14,							"EStdKeyDevice14");
_LIT(KStdKeyDevice15,							"EStdKeyDevice15");
_LIT(KStdKeyDevice16,							"EStdKeyDevice16");
_LIT(KStdKeyDevice17,							"EStdKeyDevice17");
_LIT(KStdKeyDevice18,							"EStdKeyDevice18");
_LIT(KStdKeyDevice19,							"EStdKeyDevice19");
_LIT(KStdKeyDevice1A,							"EStdKeyDevice1A");
_LIT(KStdKeyDevice1B,							"EStdKeyDevice1B");
_LIT(KStdKeyDevice1C,							"EStdKeyDevice1C");
_LIT(KStdKeyDevice1D,							"EStdKeyDevice1D");
_LIT(KStdKeyDevice1E,							"EStdKeyDevice1E");
_LIT(KStdKeyDevice1F,							"EStdKeyDevice1F");
_LIT(KStdKeyApplication10,						"EStdKeyApplication10");
_LIT(KStdKeyApplication11,						"EStdKeyApplication11");
_LIT(KStdKeyApplication12,						"EStdKeyApplication12");
_LIT(KStdKeyApplication13,						"EStdKeyApplication13");
_LIT(KStdKeyApplication14,						"EStdKeyApplication14");
_LIT(KStdKeyApplication15,						"EStdKeyApplication15");
_LIT(KStdKeyApplication16,						"EStdKeyApplication16");
_LIT(KStdKeyApplication17,						"EStdKeyApplication17");
_LIT(KStdKeyApplication18,						"EStdKeyApplication18");
_LIT(KStdKeyApplication19,						"EStdKeyApplication19");
_LIT(KStdKeyApplication1A,						"EStdKeyApplication1A");
_LIT(KStdKeyApplication1B,						"EStdKeyApplication1B");
_LIT(KStdKeyApplication1C,						"EStdKeyApplication1C");
_LIT(KStdKeyApplication1D,						"EStdKeyApplication1D");
_LIT(KStdKeyApplication1E,						"EStdKeyApplication1E");
_LIT(KStdKeyApplication1F,						"EStdKeyApplication1F");

_LIT(KTextAlignLeft,							"ELeft");
_LIT(KTextAlignCenter,							"ECenter");
_LIT(KTextAlignRight,							"ERight");

_LIT(KTextDirectionHorizontal,					"EHorizontal");
_LIT(KTextDirectionVertical,					"EVertical");

_LIT(KTypeNone,									"ENone");
_LIT(KTypePointerMove,							"EPointerMove");
_LIT(KTypePointerSwitchOn,						"EPointerSwitchOn");
_LIT(KTypeKeyDown,								"EKeyDown");
_LIT(KTypeKeyUp,								"EKeyUp");
_LIT(KTypeRedraw,								"ERedraw");
_LIT(KTypeSwitchOn,								"ESwitchOn");
_LIT(KTypeActive,								"EActive");
_LIT(KTypeInactive,								"EInactive");
_LIT(KTypeUpdateModifiers,						"EUpdateModifiers");
_LIT(KTypeButton1Down,							"EButton1Down");
_LIT(KTypeButton1Up,							"EButton1Up");
_LIT(KTypeButton2Down,							"EButton2Down");
_LIT(KTypeButton2Up,							"EButton2Up");
_LIT(KTypeButton3Down,							"EButton3Down");
_LIT(KTypeButton3Up,							"EButton3Up");
_LIT(KTypeSwitchOff,							"ESwitchOff");
_LIT(KTypeKeyRepeat,							"EKeyRepeat");
_LIT(KTypeCaseOpen,								"ECaseOpen");
_LIT(KTypeCaseClose,							"ECaseClose");
_LIT(KTypePointer3DInRange,						"EPointer3DInRange");
_LIT(KTypePointer3DOutOfRange,					"EPointer3DOutOfRange");
_LIT(KTypePointer3DTilt,						"EPointer3DTilt");
_LIT(KTypePointer3DRotation,					"EPointer3DRotation");
_LIT(KTypePointer3DTiltAndMove,					"EPointer3DTiltAndMove");
_LIT(KTypeButton4Down,							"EButton4Down");
_LIT(KTypeButton4Up,							"EButton4Up");
_LIT(KTypeButton5Down,							"EButton5Down");
_LIT(KTypeButton5Up,							"EButton5Up");
_LIT(KTypeButton6Down,							"EButton6Down");
_LIT(KTypeButton6Up,							"EButton6Up");
_LIT(KTypeRestartSystem,						"ERestartSystem");

_LIT(KTypeDrag,									"EDrag");
_LIT(KTypeMove,									"EMove");
_LIT(KTypeButtonRepeat,							"EButtonRepeat");

_LIT(KWindowBackupTypeAreaBehind,				"EWindowBackupAreaBehind");
_LIT(KWindowBackupTypeFullScreen,				"EWindowBackupFullScreen");

_LIT(KWsTransparencyPolicyDefault,				"ETransparencyDefault");
_LIT(KWsTransparencyPolicyFreezeUnder,			"ETransparencyFreezeUnder");

_LIT(KWsVisibilityChangedCanBeSeen,				"ECanBeSeen");
_LIT(KWsVisibilityChangedCantBeSeen,			"ECantBeSeen");
_LIT(KWsVisibilityChangedPartiallyVisible,		"EPartiallyVisible");
_LIT(KWsVisibilityChangedNotVisible,			"ENotVisible");
_LIT(KWsVisibilityChangedFullyVisible,			"EFullyVisible");

_LIT(KELongCaptureShortEventImmediately,		"ELongCaptureShortEventImmediately");
_LIT(KELongCaptureRepeatEvents,					"ELongCaptureRepeatEvents");
_LIT(KELongCaptureNormal,						"ELongCaptureNormal");
_LIT(KELongCaptureWaitShort,					"ELongCaptureWaitShort");

_LIT(KESpriteFlash,								"ESpriteFlash");
_LIT(KESpriteNoChildClip,						"ESpriteNoChildClip");
_LIT(KESpriteNoShadows,							"ESpriteNoShadows");

_LIT(KECustomTextCursorAlignTop,				"ECustomTextCursorAlignTop");
_LIT(KECustomTextCursorAlignBaseline,			"ECustomTextCursorAlignBaseline");
_LIT(KECustomTextCursorAlignBottom,				"ECustomTextCursorAlignBottom");

_LIT(KETypeRectangle,							"ETypeRectangle");
_LIT(KETypeHollowRectangle,						"ETypeHollowRectangle");
_LIT(KETypeFirst,								"ETypeFirst");
_LIT(KETypeLast,								"ETypeLast");
_LIT(KETypeLastBasic,							"ETypeHollowRectangle");

_LIT(KEFlagNoFlash,								"EFlagNoFlash");
_LIT(KEFlagClipHorizontal,						"EFlagClipHorizontal");
_LIT(KEFlagClipVertical,						"EFlagClipVertical");

_LIT(KTypefaceFontName,							"typeface_name");
_LIT(KIsScalable,								"is_scalable");
_LIT(KNumHeights,								"num_heights");
_LIT(KMinHeightInTwips,							"min_height_in_twips");
_LIT(KMaxHeightInTwips,							"max_height_in_twips");

_LIT(KProportional,								"typeface_attribute_proportional");
_LIT(KSerif,									"typeface_attribute_serif");
_LIT(KSymbol,									"typeface_attribute_symbol");

_LIT(KETerminateCancel,							"ETerminateCancel");
_LIT(KETerminateRegion,							"ETerminateRegion");
_LIT(KETerminateRotation,						"ETerminateRotation");
_LIT(KETerminateScreenMode,						"EETerminateScreenMode");

_LIT(KEColor,                                   "EColor");
_LIT(KETransparencyPossible,                    "ETransparencyPossible");
_LIT(KEFullyScaleable,                          "EFullyScaleable");
_LIT(KEConstantAspectRatio,                     "EConstantAspectRatio");
_LIT(KECanDither,                               "ECanDither");
_LIT(KEAlphaChannel,                            "EAlphaChannel");
_LIT(KELeaveInPlace,                            "ELeaveInPlace");
_LIT(KERestoreToBackground,                     "ERestoreToBackground");
_LIT(KERestoreToPrevious,                       "ERestoreToPrevious");
_LIT(KEPartialDecodeInvalid,                    "EPartialDecodeInvalid");
_LIT(KEMngMoreFramesToDecode,                   "EMngMoreFramesToDecode");
_LIT(KEUsesFrameSizeInPixels,                   "EUsesFrameSizeInPixels");

_LIT(KEFrameInfoUninitialised,                  "EFrameInfoUninitialised");
_LIT(KEFrameInfoProcessingFrameHeader,          "EFrameInfoProcessingFrameHeader");
_LIT(KEFrameInfoProcessingFrame,                "EFrameInfoProcessingFrame");
_LIT(KEFrameInfoProcessingComplete,             "EFrameInfoProcessingComplete"); 

_LIT(KFldFrameInfoState,                        "frameinfostate");
_LIT(KFldCurrentDataOffset,                     "currentdataoffset");
_LIT(KFldFrameDataOffset,                       "framedataoffset");
_LIT(KFldFrameCoordsInPixels,                   "framecoordsinpixels");
_LIT(KFldFrameSizeInTwips,                      "framesizeintwips");
_LIT(KFldBitsPerPixel,                          "bitsPerPixel");
_LIT(KFldDelay,                                 "delay");
_LIT(KFldFlags,                                 "flags");
_LIT(KFldOverallSizeInPixels,                   "overallsizeinpixels");
_LIT(KFldFrameDisplayMode,                      "framedisplaymode");
_LIT(KFldBackgroundColor,                       "backgroundcolor");
_LIT(KFldFrameSizeInPixels,                     "framesizeinpixels");

_LIT(KFldProcessName,							"processname");
_LIT(KFldId,									"id");
_LIT(KFldRect,									"rect");
_LIT(KFldWsga,									"wsga");

//For MAnim
//Opcodes for Util
_LIT(KEMAnimECmdRetrieveResult, 				"ECmdRetrieveResult"); 
_LIT(KEMAnimECmdUtilEatupMemory, 				"ECmdUtilEatupMemory"); 
_LIT(KEMAnimECmdUtilFreeEatenMemory,     		"ECmdUtilFreeEatenMemory"); 
//Opcodes for MAnimGeneralFunctions
_LIT(KEMAnimECmdGeneralAnimate, 				"ECmdGeneralAnimate"); 
_LIT(KEMAnimECmdGeneralClient, 				 	"ECmdGeneralClient");  
_LIT(KEMAnimECmdGeneralFlashStateOn, 			"ECmdGeneralFlashStateOn");   
_LIT(KEMAnimECmdGeneralPanic, 					"ECmdGeneralPanic");      
_LIT(KEMAnimECmdGeneralScreenDevice, 			"ECmdGeneralScreenDevice");       
_LIT(KEMAnimECmdGeneralExtendedInterface, 		"ECmdGeneralExtendedInterface");
_LIT(KEMAnimECmdGeneralWindowExtension, 		"ECmdGeneralWindowExtension");  
_LIT(KEMAnimECmdGeneralNumOfExtInterfaces,		"ECmdGeneralNumOfExtInterfaces");  
_LIT(KEMAnimECmdGeneralEventExtension , 		"ECmdGeneralEventExtension");
_LIT(KEMAnimECmdGeneralGetRawEvents , 			"ECmdGeneralGetRawEvents"); 
_LIT(KEMAnimECmdGeneralPostRawEvent , 			"ECmdGeneralPostRawEvent"); 
_LIT(KEMAnimECmdGeneralPostKeyEvent , 			"ECmdGeneralPostKeyEvent"); 
_LIT(KEMAnimECmdGeneralSetSync , 				"ECmdGeneralSetSync"); 
_LIT(KEMAnimECmdGeneralSync , 					"ECmdGeneralSync"); 
_LIT(KEMAnimECmdGeneralSetInterval , 			"ECmdGeneralSetInterval"); 
_LIT(KEMAnimECmdGeneralSetNextInterval , 		"ECmdGeneralSetNextInterval"); 
_LIT(KEMAnimECmdGeneralSystemTime, 				"ECmdGeneralSystemTime"); 
_LIT(KEMAnimECmdGeneralRegisterForNotis, 		"ECmdGeneralRegisterForNotis"); 
_LIT(KEMAnimECmdGeneralMessage, 				"ECmdGeneralMessage");  
_LIT(KEMAnimECmdGeneralDuplicateBitmapL, 		"ECmdGeneralDuplicateBitmapL");   
_LIT(KEMAnimECmdGeneralDuplicateFontL, 			"ECmdGeneralDuplicateFontL");      
_LIT(KEMAnimECmdGeneralCloseFont, 				"ECmdGeneralCloseFont");
_LIT(KEMAnimECmdGeneralReplyBuf8, 				"ECmdGeneralReplyBuf8");
_LIT(KEMAnimECmdGeneralReplyBuf16, 				"ECmdGeneralReplyBuf16");

//Opcodes for MAnimSpriteFunctions          
_LIT(KEMAnimECmdSpriteGetSpriteMember, 			"ECmdSpriteGetSpriteMember");         
_LIT(KEMAnimECmdSpriteUpdateMember, 			"ECmdSpriteUpdateMember");        
_LIT(KEMAnimECmdSpriteActivate, 				"ECmdSpriteActivate");   
_LIT(KEMAnimECmdSpriteSpriteCanBeSeen, 			"ECmdSpriteSpriteCanBeSeen"); 
_LIT(KEMAnimECmdSpriteSizeChangedL, 			"ECmdSpriteSizeChangedL"); 
_LIT(KEMAnimECmdSpriteSetPosition, 				"ECmdSpriteSetPosition");    
   
	  

//Opcodes for MAnimWindowFunctions  
_LIT(KEMAnimECmdWindowActivateGc, 				"ECmdWindowActivateGc");//Verify the return value is expected
_LIT(KEMAnimECmdWindowSetRect, 					"ECmdWindowSetRect");
_LIT(KEMAnimECmdWindowWindowSize, 				"ECmdWindowWindowSize");
_LIT(KEMAnimECmdWindowIsHidden, 				"ECmdWindowIsHidden");
_LIT(KEMAnimECmdWindowSetVisible, 				"ECmdWindowSetVisible");
_LIT(KEMAnimECmdWindowInvalidate, 				"ECmdWindowInvalidate");
_LIT(KEMAnimECmdWindowParameters, 				"ECmdWindowParameters");
_LIT(KEMAnimECmdWindowVisibleRegion, 			"ECmdWindowVisibleRegion");
_LIT(KEMAnimECmdWindowIsStarted,                "ECmdWindowIsStarted");

//Opcodes for MAnimFreeTimerWindowFunctions
_LIT(KEMAnimECmdFreeTimerWinDeactivateGc,       "ECmdFreeTimerWinDeactivateGc");
_LIT(KEMAnimECmdFreeTimerWinUpdate,			    "ECmdFreeTimerWinUpdate");

//Opcodes for MAnimGeneralFunctionsWindowExtension
_LIT(KEMAnimECmdGfweScreens,				    "ECmdGfweScreens");
_LIT(KEMAnimECmdGfweFocusScreens,				"ECmdGfweFocusScreens");
_LIT(KEMAnimECmdGfweSetFocusScreen,				"ECmdGfweSetFocusScreen");
_LIT(KEMAnimECmdGfweWindowGroups,				"ECmdGfweWindowGroups");
_LIT(KEMAnimECmdGfweWindowGroupInfo,			"ECmdGfweWindowGroupInfo");
_LIT(KEMAnimECmdGfweWindowGroupName,			"ECmdGfweWindowGroupName");//use handle to verify it in server side
_LIT(KEMAnimECmdGfweSetOrdinalPosition,			"ECmdGfweSetOrdinalPosition");
_LIT(KEMAnimECmdGfweIsFocusable,			    "ECmdGfweIsFocusable");
_LIT(KEMAnimECmdGfweWindowConfig,			    "ECmdGfweWindowConfig");


_LIT(KEAnimWindows,								"EKWindowAnim");
_LIT(KEAnimSprite,								"EKSpriteAnim");
_LIT(KEAnimFreeTimer,							"EKFreeTimerWindowAnim");


_LIT(KEAnimCmdSync,								"ECmdSync");
_LIT(KEAnimCmdTestWithoutPara,					"ECmdTestWithoutPara");
_LIT(KEAnimCmdTestWithPara,						"ECmdTestWithPara");
_LIT(KEAnimCmdGetLast,							"ECmdGetLast");
_LIT(KEAnimCmdGetAnimInterval,					"ECmdGetAnimInterval");

_LIT(KEGraphicsOrientationNormal,               "EGraphicsOrientationNormal");
_LIT(KEGraphicsOrientationRotated90,            "EGraphicsOrientationRotated90");
_LIT(KEGraphicsOrientationRotated180,           "EGraphicsOrientationRotated180");
_LIT(KEGraphicsOrientationRotated270,           "EGraphicsOrientationRotated270");
//For CPolygonFiller
_LIT(KUsageEGetAllPixelRunsSequentially,		"EGetAllPixelRunsSequentially");
_LIT(KUsageEGetPixelRunsSequentiallyForSpecifiedScanLines,"EGetPixelRunsSequentiallyForSpecifiedScanLines");

/*@}*/

RPointerArray < TAny > CT_GraphicsUtil::iMemArray;
TInt CT_GraphicsUtil::iAvaiableSpace=0;
TInt CT_GraphicsUtil::iBiggestBlock=0;

TBool CT_GraphicsUtil::ReadBitmapfileCompressionScheme(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TBitmapfileCompressionScheme& aCompressionScheme)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KRLECompression )
			{
			aCompressionScheme=ERLECompression;
			}
		else if ( str==KPaletteCompression )
			{
			aCompressionScheme=EPaletteCompression;
			}
		else if ( str==KPaletteCompressionWithRLEFallback )
			{
			aCompressionScheme=EPaletteCompressionWithRLEFallback;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aCompressionScheme=(TBitmapfileCompressionScheme)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadBrushStyle(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TBrushStyle& aBrushStyle)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KBrushStyleNull )
			{
			aBrushStyle=CGraphicsContext::ENullBrush;
			}
		else if ( str==KBrushStyleSolid )
			{
			aBrushStyle=CGraphicsContext::ESolidBrush;
			}
		else if ( str==KBrushStylePatterned )
			{
			aBrushStyle=CGraphicsContext::EPatternedBrush;
			}
		else if ( str==KBrushStyleVerticalHatch )
			{
			aBrushStyle=CGraphicsContext::EVerticalHatchBrush;
			}
		else if ( str==KBrushStyleForwardDiagonalHatch )
			{
			aBrushStyle=CGraphicsContext::EForwardDiagonalHatchBrush;
			}
		else if ( str==KBrushStyleHorizontalHatch )
			{
			aBrushStyle=CGraphicsContext::EHorizontalHatchBrush;
			}
		else if ( str==KBrushStyleRearwardDiagonalHatch )
			{
			aBrushStyle=CGraphicsContext::ERearwardDiagonalHatchBrush;
			}
		else if ( str==KBrushStyleSquareCrossHatch )
			{
			aBrushStyle=CGraphicsContext::ESquareCrossHatchBrush;
			}
		else if ( str==KBrushStyleDiamondCrossHatch )
			{
			aBrushStyle=CGraphicsContext::EDiamondCrossHatchBrush;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aBrushStyle=(CGraphicsContext::TBrushStyle)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadCaptureFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aCaptureFlags)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		TUint	temp=0;
		ret=ConvertToCaptureFlags(str, temp);
		if ( ret )
			{
			aCaptureFlags=temp;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToCaptureFlags(const TDesC& aStr, RWindowBase::TCaptureFlags& aCaptureFlags)
	{
	TBool	ret=ETrue;
	if ( aStr==KCaptureFlagEnabled )
		{
		aCaptureFlags=RWindowBase::TCaptureFlagEnabled;
		}
	else if ( aStr==KCaptureFlagDragDrop )
		{
		aCaptureFlags=RWindowBase::TCaptureFlagDragDrop;
		}
	else if ( aStr==KCaptureFlagAllGroups )
		{
		aCaptureFlags=RWindowBase::TCaptureFlagAllGroups;
		}
	else if ( aStr==KCaptureDisabled )
		{
		aCaptureFlags=RWindowBase::TCaptureDisabled;
		}
	else if ( aStr==KCaptureEnabled )
		{
		aCaptureFlags=RWindowBase::TCaptureEnabled;
		}
	else if ( aStr==KCaptureDragDrop )
		{
		aCaptureFlags=RWindowBase::TCaptureDragDrop;
		}
	else
		{
		TUint	captureFlags;
		TLex	lex(aStr);
		ret=(lex.Val(captureFlags, EHex)==KErrNone);
		if ( ret )
			{
			aCaptureFlags=(RWindowBase::TCaptureFlags)captureFlags;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToCaptureFlags(const TDesC& aStr, TUint& aCaptureFlags)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToCaptureFlags(tempStr, aCaptureFlags);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToCaptureFlags(tempStr, temp) )
			{
			aCaptureFlags|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		RWindowBase::TCaptureFlags	captureFlags;
		ret=ConvertToCaptureFlags(aStr, captureFlags);
		if ( ret )
			{
			aCaptureFlags=(TUint)captureFlags;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadComputeMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsSession::TComputeMode& aComputeMode)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KComputeModePriorityControlDisabled )
			{
			aComputeMode=RWsSession::EPriorityControlDisabled;
			}
		else if ( str==KComputeModePriorityControlComputeOn )
			{
			aComputeMode=RWsSession::EPriorityControlComputeOn;
			}
		else if ( str==KComputeModePriorityControlComputeOff )
			{
			aComputeMode=RWsSession::EPriorityControlComputeOff;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aComputeMode=(RWsSession::TComputeMode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadCornerType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TCornerType& aCornerType)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KCornerTypeWindowSquare )
			{
			aCornerType=EWindowCornerSquare;
			}
		else if ( str==KCornerTypeWindow1 )
			{
			aCornerType=EWindowCorner1;
			}
		else if ( str==KCornerTypeWindow2 )
			{
			aCornerType=EWindowCorner2;
			}
		else if ( str==KCornerTypeWindow3 )
			{
			aCornerType=EWindowCorner3;
			}
		else if ( str==KCornerTypeWindow5 )
			{
			aCornerType=EWindowCorner5;
			}
		else if ( str==KCornerTypeWindowRegion )
			{
			aCornerType=EWindowCornerRegion;
			}
		else if ( str==KCornerTypeMask )
			{
			aCornerType=ECornerTypeMask;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aCornerType=(TCornerType)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadDisplayMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TDisplayMode& aDisplayMode)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KDisplayModeNone )
			{
			aDisplayMode=ENone;
			}
		else if ( str==KDisplayModeGray2 )
			{
			aDisplayMode=EGray2;
			}
		else if ( str==KDisplayModeGray4 )
			{
			aDisplayMode=EGray4;
			}
		else if ( str==KDisplayModeGray16 )
			{
			aDisplayMode=EGray16;
			}
		else if ( str==KDisplayModeGray256 )
			{
			aDisplayMode=EGray256;
			}
		else if ( str==KDisplayModeColor16 )
			{
			aDisplayMode=EColor16;
			}
		else if ( str==KDisplayModeColor256 )
			{
			aDisplayMode=EColor256;
			}
		else if ( str==KDisplayModeColor64K )
			{
			aDisplayMode=EColor64K;
			}
		else if ( str==KDisplayModeColor16M )
			{
			aDisplayMode=EColor16M;
			}
		else if ( str==KDisplayModeRgb )
			{
			aDisplayMode=ERgb;
			}
		else if ( str==KDisplayModeColor4K )
			{
			aDisplayMode=EColor4K;
			}
		else if ( str==KDisplayModeColor16MU )
			{
			aDisplayMode=EColor16MU;
			}
		else if ( str==KDisplayModeColor16MA )
			{
			aDisplayMode=EColor16MA;
			}
		else if ( str==KDisplayModeColor16MAP )
			{
			aDisplayMode=EColor16MAP;
			}
		else if ( str==KDisplayModeColorLast )
			{
			aDisplayMode=EColorLast;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aDisplayMode=(TDisplayMode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadDrawMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TDrawMode& aDrawMode)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KDrawModeAND )
			{
			aDrawMode=CGraphicsContext::EDrawModeAND;
			}
		else if ( str==KDrawModeNOTAND )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTAND;
			}
		else if ( str==KDrawModePEN )
			{
			aDrawMode=CGraphicsContext::EDrawModePEN;
			}
		else if ( str==KDrawModeANDNOT )
			{
			aDrawMode=CGraphicsContext::EDrawModeANDNOT;
			}
		else if ( str==KDrawModeXOR )
			{
			aDrawMode=CGraphicsContext::EDrawModeXOR;
			}
		else if ( str==KDrawModeOR )
			{
			aDrawMode=CGraphicsContext::EDrawModeOR;
			}
		else if ( str==KDrawModeNOTANDNOT )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTANDNOT;
			}
		else if ( str==KDrawModeNOTXOR )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTXOR;
			}
		else if ( str==KDrawModeNOTSCREEN )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTSCREEN;
			}
		else if ( str==KDrawModeNOTOR )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTOR;
			}
		else if ( str==KDrawModeNOTPEN )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTPEN;
			}
		else if ( str==KDrawModeORNOT )
			{
			aDrawMode=CGraphicsContext::EDrawModeORNOT;
			}
		else if ( str==KDrawModeNOTORNOT )
			{
			aDrawMode=CGraphicsContext::EDrawModeNOTORNOT;
			}
		else if ( str==KDrawModeWriteAlpha )
			{
			aDrawMode=CGraphicsContext::EDrawModeWriteAlpha;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aDrawMode=(CGraphicsContext::TDrawMode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadErrorCategory(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsErrorMessage::TErrorCategory& aErrorCategory)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KErrorCategoryDrawingRegion )
			{
			aErrorCategory=TWsErrorMessage::EDrawingRegion;
			}
		else if ( str==KErrorCategoryBackLight )
			{
			aErrorCategory=TWsErrorMessage::EBackLight;
			}
		else if ( str==KErrorCategoryLogging )
			{
			aErrorCategory=TWsErrorMessage::ELogging;
			}
		else if ( str==KErrorCategoryContrast )
			{
			aErrorCategory=TWsErrorMessage::EContrast;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aErrorCategory=(TWsErrorMessage::TErrorCategory)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadEventCode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TEventCode& aEventCode)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KEventCodeNull )
			{
			aEventCode=EEventNull;
			}
		else if ( str==KEventCodeKey )
			{
			aEventCode=EEventKey;
			}
		else if ( str==KEventCodeKeyUp )
			{
			aEventCode=EEventKeyUp;
			}
		else if ( str==KEventCodeKeyDown )
			{
			aEventCode=EEventKeyDown;
			}
		else if ( str==KEventCodeModifiersChanged )
			{
			aEventCode=EEventModifiersChanged;
			}
		else if ( str==KEventCodePointer )
			{
			aEventCode=EEventPointer;
			}
		else if ( str==KEventCodePointerEnter )
			{
			aEventCode=EEventPointerEnter;
			}
		else if ( str==KEventCodePointerExit )
			{
			aEventCode=EEventPointerExit;
			}
		else if ( str==KEventCodePointerBufferReady )
			{
			aEventCode=EEventPointerBufferReady;
			}
		else if ( str==KEventCodeDragDrop )
			{
			aEventCode=EEventDragDrop;
			}
		else if ( str==KEventCodeFocusLost )
			{
			aEventCode=EEventFocusLost;
			}
		else if ( str==KEventCodeFocusGained )
			{
			aEventCode=EEventFocusGained;
			}
		else if ( str==KEventCodeSwitchOn )
			{
			aEventCode=EEventSwitchOn;
			}
		else if ( str==KEventCodePassword )
			{
			aEventCode=EEventPassword;
			}
		else if ( str==KEventCodeWindowGroupsChanged )
			{
			aEventCode=EEventWindowGroupsChanged;
			}
		else if ( str==KEventCodeErrorMessage )
			{
			aEventCode=EEventErrorMessage;
			}
		else if ( str==KEventCodeMessageReady )
			{
			aEventCode=EEventMessageReady;
			}
		else if ( str==KEventCodeMarkInvalid )
			{
			aEventCode=EEventMarkInvalid;
			}
		else if ( str==KEventCodeSwitchOff )
			{
			aEventCode=EEventSwitchOff;
			}
		else if ( str==KEventCodeKeySwitchOff )
			{
			aEventCode=EEventKeySwitchOff;
			}
		else if ( str==KEventCodeScreenDeviceChanged )
			{
			aEventCode=EEventScreenDeviceChanged;
			}
		else if ( str==KEventCodeFocusGroupChanged )
			{
			aEventCode=EEventFocusGroupChanged;
			}
		else if ( str==KEventCodeCaseOpened )
			{
			aEventCode=EEventCaseOpened;
			}
		else if ( str==KEventCodeCaseClosed )
			{
			aEventCode=EEventCaseClosed;
			}
		else if ( str==KEventCodeWindowGroupListChanged )
			{
			aEventCode=EEventWindowGroupListChanged;
			}
		else if ( str==KEventCodeWindowVisibilityChanged )
			{
			aEventCode=EEventWindowVisibilityChanged;
			}
#if (defined SYMBIAN_PROCESS_MONITORING_AND_STARTUP)
		else if ( str==KEventCodeRestartSystem )
			{
			aEventCode=EEventRestartSystem;
			}
#endif
		else if ( str==KEventCodeKeyRepeat )
			{
			aEventCode=EEventKeyRepeat;
			}
		else if ( str==KEventCodeDirectScreenAccessBegin )
			{
			aEventCode=EEventDirectScreenAccessBegin;
			}
		else if ( str==KEventCodeDirectScreenAccessEnd )
			{
			aEventCode=EEventDirectScreenAccessEnd;
			}
		else if ( str==KEventCodeHeartbeatTimerStateChange )
			{
			aEventCode=EEventHeartbeatTimerStateChange;
			}
		else if ( str==KEventCodePowerMgmt )
			{
			aEventCode=EEventPowerMgmt;
			}
		else if ( str==KEventCodeReserved )
			{
			aEventCode=EEventReserved;
			}
		else if ( str==KEventCodeUser )
			{
			aEventCode=EEventUser;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aEventCode=(TEventCode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadEventControl(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TEventControl& aEventControl)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KEventControlAlways )
			{
			aEventControl=EEventControlAlways;
			}
		else if ( str==KEventControlOnlyWithKeyboardFocus )
			{
			aEventControl=EEventControlOnlyWithKeyboardFocus;
			}
		else if ( str==KEventControlOnlyWhenVisible )
			{
			aEventControl=EEventControlOnlyWhenVisible;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aEventControl=(TEventControl)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadEventModifier(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TEventModifier& aEventModifier)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		ret=ConvertToEventModifier(str, aEventModifier);
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadEventModifier(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aEventModifier)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		TUint	temp=0;
		ret=ConvertToEventModifier(str, temp);
		if ( ret )
			{
			aEventModifier=temp;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToEventModifier(const TDesC& aStr, TEventModifier& aEventModifier)
	{
	TBool	ret=ETrue;
	if ( aStr==KEventModifierAutorepeatable )
		{
		aEventModifier=EModifierAutorepeatable;
		}
	else if ( aStr==KEventModifierKeypad )
		{
		aEventModifier=EModifierKeypad;
		}
	else if ( aStr==KEventModifierLeftAlt )
		{
		aEventModifier=EModifierLeftAlt;
		}
	else if ( aStr==KEventModifierRightAlt )
		{
		aEventModifier=EModifierRightAlt;
		}
	else if ( aStr==KEventModifierAlt )
		{
		aEventModifier=EModifierAlt;
		}
	else if ( aStr==KEventModifierLeftCtrl )
		{
		aEventModifier=EModifierLeftCtrl;
		}
	else if ( aStr==KEventModifierRightCtrl )
		{
		aEventModifier=EModifierRightCtrl;
		}
	else if ( aStr==KEventModifierCtrl )
		{
		aEventModifier=EModifierCtrl;
		}
	else if ( aStr==KEventModifierLeftShift )
		{
		aEventModifier=EModifierLeftShift;
		}
	else if ( aStr==KEventModifierRightShift )
		{
		aEventModifier=EModifierRightShift;
		}
	else if ( aStr==KEventModifierShift )
		{
		aEventModifier=EModifierShift;
		}
	else if ( aStr==KEventModifierLeftFunc )
		{
		aEventModifier=EModifierLeftFunc;
		}
	else if ( aStr==KEventModifierRightFunc )
		{
		aEventModifier=EModifierRightFunc;
		}
	else if ( aStr==KEventModifierFunc )
		{
		aEventModifier=EModifierFunc;
		}
	else if ( aStr==KEventModifierCapsLock )
		{
		aEventModifier=EModifierCapsLock;
		}
	else if ( aStr==KEventModifierNumLock )
		{
		aEventModifier=EModifierNumLock;
		}
	else if ( aStr==KEventModifierScrollLock )
		{
		aEventModifier=EModifierScrollLock;
		}
	else if ( aStr==KEventModifierKeyUp )
		{
		aEventModifier=EModifierKeyUp;
		}
	else if ( aStr==KEventModifierSpecial )
		{
		aEventModifier=EModifierSpecial;
		}
	else if ( aStr==KEventModifierDoubleClick )
		{
		aEventModifier=EModifierDoubleClick;
		}
	else if ( aStr==KEventModifierPureKeycode )
		{
		aEventModifier=EModifierPureKeycode;
		}
	else if ( aStr==KEventModifierKeyboardExtend )
		{
		aEventModifier=EModifierKeyboardExtend;
		}
	else if ( aStr==KEventModifierCancelRotation )
		{
		aEventModifier=EModifierCancelRotation;
		}
	else if ( aStr==KEventModifierRotateBy90 )
		{
		aEventModifier=EModifierRotateBy90;
		}
	else if ( aStr==KEventModifierRotateBy180 )
		{
		aEventModifier=EModifierRotateBy180;
		}
	else if ( aStr==KEventModifierRotateBy270 )
		{
		aEventModifier=EModifierRotateBy270;
		}
	else if ( aStr==KEventModifierPointer3DButton1 )
		{
		aEventModifier=EModifierPointer3DButton1;
		}
	else if ( aStr==KEventModifierPointer3DButton2 )
		{
		aEventModifier=EModifierPointer3DButton2;
		}
	else if ( aStr==KEventModifierPointer3DButton3 )
		{
		aEventModifier=EModifierPointer3DButton3;
		}
	else if ( aStr==KEventModifierAll )
		{
		aEventModifier=EAllModifiers;
		}
	else
		{
		TUint	eventModifier;
		TLex	lex(aStr);
		ret=(lex.Val(eventModifier, EHex)==KErrNone);
		if ( ret )
			{
			aEventModifier=(TEventModifier)eventModifier;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToEventModifier(const TDesC& aStr, TUint& aEventModifier)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToEventModifier(tempStr, aEventModifier);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToEventModifier(tempStr, temp) )
			{
			aEventModifier|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		TEventModifier	eventModifier;
		ret=ConvertToEventModifier(aStr, eventModifier);
		if ( ret )
			{
			aEventModifier=(TUint)eventModifier;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadFadeControl(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWindowTreeNode::TFadeControl& aFadeControl)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KFadeControlIncludeChildren )
			{
			aFadeControl=RWindowTreeNode::EFadeIncludeChildren;
			}
		else if ( str==KFadeControlWindowOnly )
			{
			aFadeControl=RWindowTreeNode::EFadeWindowOnly;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aFadeControl=(RWindowTreeNode::TFadeControl)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadFillRule(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TFillRule& aFillRule)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KFillRuleAlternate )
			{
			aFillRule=CGraphicsContext::EAlternate;
			}
		else if ( str==KFillRuleEWinding )
			{
			aFillRule=CGraphicsContext::EWinding;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aFillRule=(CGraphicsContext::TFillRule)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadTUsage(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CPolygonFiller::TUsage& aUsage)
	{
	TPtrC str;
	TBool ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if (ret)
		{
		if (str==KUsageEGetAllPixelRunsSequentially)
			{
			aUsage=CPolygonFiller::EGetAllPixelRunsSequentially;
			}
		else if (str==KUsageEGetPixelRunsSequentiallyForSpecifiedScanLines)
			{
			aUsage=CPolygonFiller::EGetPixelRunsSequentiallyForSpecifiedScanLines;
			}
		}
	return ret;
	}

TBool CT_GraphicsUtil::ReadFontStrikethrough(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TFontStrikethrough& aFontStrikethrough)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KFontStrikethroughOff )
			{
			aFontStrikethrough=EStrikethroughOff;
			}
		else if ( str==KFontStrikethroughOn )
			{
			aFontStrikethrough=EStrikethroughOn;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aFontStrikethrough=(TFontStrikethrough)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadFontUnderline(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TFontUnderline& aFontUnderline)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KFontUnderlineOff )
			{
			aFontUnderline=EUnderlineOff;
			}
		else if ( str==KFontUnderlineOn )
			{
			aFontUnderline=EUnderlineOn;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aFontUnderline=(TFontUnderline)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadHotKey(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, THotKey& aHotKey)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KHotKeyEnableLogging )
			{
			aHotKey=EHotKeyEnableLogging;
			}
		else if ( str==KHotKeyDisableLogging )
			{
			aHotKey=EHotKeyDisableLogging;
			}
		else if ( str==KHotKeyStateDump )
			{
			aHotKey=EHotKeyStateDump;
			}
		else if ( str==KHotKeyOfDeath )
			{
			aHotKey=EHotKeyOfDeath;
			}
		else if ( str==KHotKeyShutDown )
			{
			aHotKey=EHotKeyShutDown;
			}
		else if ( str==KHotKeyHeapDump )
			{
			aHotKey=EHotKeyHeapDump;
			}
		else if ( str==KHotKeyIncContrast )
			{
			aHotKey=EHotKeyIncContrast;
			}
		else if ( str==KHotKeyDecContrast )
			{
			aHotKey=EHotKeyDecContrast;
			}
		else if ( str==KHotKeyOff )
			{
			aHotKey=EHotKeyOff;
			}
		else if ( str==KHotKeyBacklightOn )
			{
			aHotKey=EHotKeyBacklightOn;
			}
		else if ( str==KHotKeyBacklightOff )
			{
			aHotKey=EHotKeyBacklightOff;
			}
		else if ( str==KHotKeyBacklightToggle )
			{
			aHotKey=EHotKeyBacklightToggle;
			}
		else if ( str==KHotKeyScreenDimension0 )
			{
			aHotKey=EHotKeyScreenDimension0;
			}
		else if ( str==KHotKeyScreenDimension1 )
			{
			aHotKey=EHotKeyScreenDimension1;
			}
		else if ( str==KHotKeyScreenDimension2 )
			{
			aHotKey=EHotKeyScreenDimension2;
			}
		else if ( str==KHotKeyScreenDimension3 )
			{
			aHotKey=EHotKeyScreenDimension3;
			}
		else if ( str==KHotKeyCycleDisplaySize )
			{
			aHotKey=EHotKeyCycleDisplaySize;
			}
		else if ( str==KHotKeyCycleOrientation )
			{
			aHotKey=EHotKeyCycleOrientation;
			}
		else if ( str==KHotKeyIncBrightness )
			{
			aHotKey=EHotKeyIncBrightness;
			}
		else if ( str==KHotKeyDecBrightness )
			{
			aHotKey=EHotKeyDecBrightness;
			}
		else if ( str==KHotKeyCycleFocusScreen )
			{
			aHotKey=EHotKeyCycleFocusScreen;
			}
		else if ( str==KHotKeyFirstKeyType )
			{
			aHotKey=EHotKeyFirstKeyType;
			}
		else if ( str==KHotKeyLastKeyType )
			{
			aHotKey=EHotKeyLastKeyType;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aHotKey=(THotKey)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadKeyCode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TKeyCode& aKeyCode)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KKeyNull )
			{
			aKeyCode=EKeyNull;
			}
		else if ( str==KKeyBell )
			{
			aKeyCode=EKeyBell;
			}
		else if ( str==KKeyBackspace )
			{
			aKeyCode=EKeyBackspace;
			}
		else if ( str==KKeyTab )
			{
			aKeyCode=EKeyTab;
			}
		else if ( str==KKeyLineFeed )
			{
			aKeyCode=EKeyLineFeed;
			}
		else if ( str==KKeyVerticalTab )
			{
			aKeyCode=EKeyVerticalTab;
			}
		else if ( str==KKeyFormFeed )
			{
			aKeyCode=EKeyFormFeed;
			}
		else if ( str==KKeyEnter )
			{
			aKeyCode=EKeyEnter;
			}
		else if ( str==KKeyEscape )
			{
			aKeyCode=EKeyEscape;
			}
		else if ( str==KKeySpace )
			{
			aKeyCode=EKeySpace;
			}
		else if ( str==KKeyDelete )
			{
			aKeyCode=EKeyDelete;
			}
		else if ( str==KKeyPrintScreen )
			{
			aKeyCode=EKeyPrintScreen;
			}
		else if ( str==KKeyPause )
			{
			aKeyCode=EKeyPause;
			}
		else if ( str==KKeyHome )
			{
			aKeyCode=EKeyHome;
			}
		else if ( str==KKeyEnd )
			{
			aKeyCode=EKeyEnd;
			}
		else if ( str==KKeyPageUp )
			{
			aKeyCode=EKeyPageUp;
			}
		else if ( str==KKeyPageDown )
			{
			aKeyCode=EKeyPageDown;
			}
		else if ( str==KKeyInsert )
			{
			aKeyCode=EKeyInsert;
			}
		else if ( str==KKeyLeftArrow )
			{
			aKeyCode=EKeyLeftArrow;
			}
		else if ( str==KKeyRightArrow )
			{
			aKeyCode=EKeyRightArrow;
			}
		else if ( str==KKeyUpArrow )
			{
			aKeyCode=EKeyUpArrow;
			}
		else if ( str==KKeyDownArrow )
			{
			aKeyCode=EKeyDownArrow;
			}
		else if ( str==KKeyLeftShift )
			{
			aKeyCode=EKeyLeftShift;
			}
		else if ( str==KKeyRightShift )
			{
			aKeyCode=EKeyRightShift;
			}
		else if ( str==KKeyLeftAlt )
			{
			aKeyCode=EKeyLeftAlt;
			}
		else if ( str==KKeyRightAlt )
			{
			aKeyCode=EKeyRightAlt;
			}
		else if ( str==KKeyLeftCtrl )
			{
			aKeyCode=EKeyLeftCtrl;
			}
		else if ( str==KKeyRightCtrl )
			{
			aKeyCode=EKeyRightCtrl;
			}
		else if ( str==KKeyLeftFunc )
			{
			aKeyCode=EKeyLeftFunc;
			}
		else if ( str==KKeyRightFunc )
			{
			aKeyCode=EKeyRightFunc;
			}
		else if ( str==KKeyCapsLock )
			{
			aKeyCode=EKeyCapsLock;
			}
		else if ( str==KKeyNumLock )
			{
			aKeyCode=EKeyNumLock;
			}
		else if ( str==KKeyScrollLock )
			{
			aKeyCode=EKeyScrollLock;
			}
		else if ( str==KKeyF1 )
			{
			aKeyCode=EKeyF1;
			}
		else if ( str==KKeyF2 )
			{
			aKeyCode=EKeyF2;
			}
		else if ( str==KKeyF3 )
			{
			aKeyCode=EKeyF3;
			}
		else if ( str==KKeyF4 )
			{
			aKeyCode=EKeyF4;
			}
		else if ( str==KKeyF5 )
			{
			aKeyCode=EKeyF5;
			}
		else if ( str==KKeyF6 )
			{
			aKeyCode=EKeyF6;
			}
		else if ( str==KKeyF7 )
			{
			aKeyCode=EKeyF7;
			}
		else if ( str==KKeyF8 )
			{
			aKeyCode=EKeyF8;
			}
		else if ( str==KKeyF9 )
			{
			aKeyCode=EKeyF9;
			}
		else if ( str==KKeyF10 )
			{
			aKeyCode=EKeyF10;
			}
		else if ( str==KKeyF11 )
			{
			aKeyCode=EKeyF11;
			}
		else if ( str==KKeyF12 )
			{
			aKeyCode=EKeyF12;
			}
		else if ( str==KKeyF13 )
			{
			aKeyCode=EKeyF13;
			}
		else if ( str==KKeyF14 )
			{
			aKeyCode=EKeyF14;
			}
		else if ( str==KKeyF15 )
			{
			aKeyCode=EKeyF15;
			}
		else if ( str==KKeyF16 )
			{
			aKeyCode=EKeyF16;
			}
		else if ( str==KKeyF17 )
			{
			aKeyCode=EKeyF17;
			}
		else if ( str==KKeyF18 )
			{
			aKeyCode=EKeyF18;
			}
		else if ( str==KKeyF19 )
			{
			aKeyCode=EKeyF19;
			}
		else if ( str==KKeyF20 )
			{
			aKeyCode=EKeyF20;
			}
		else if ( str==KKeyF21 )
			{
			aKeyCode=EKeyF21;
			}
		else if ( str==KKeyF22 )
			{
			aKeyCode=EKeyF22;
			}
		else if ( str==KKeyF23 )
			{
			aKeyCode=EKeyF23;
			}
		else if ( str==KKeyF24 )
			{
			aKeyCode=EKeyF24;
			}
		else if ( str==KKeyOff )
			{
			aKeyCode=EKeyOff;
			}
		else if ( str==KKeyIncContrast )
			{
			aKeyCode=EKeyIncContrast;
			}
		else if ( str==KKeyDecContrast )
			{
			aKeyCode=EKeyDecContrast;
			}
		else if ( str==KKeyBacklightOn )
			{
			aKeyCode=EKeyBacklightOn;
			}
		else if ( str==KKeyBacklightOff )
			{
			aKeyCode=EKeyBacklightOff;
			}
		else if ( str==KKeyBacklightToggle )
			{
			aKeyCode=EKeyBacklightToggle;
			}
		else if ( str==KKeySliderDown )
			{
			aKeyCode=EKeySliderDown;
			}
		else if ( str==KKeySliderUp )
			{
			aKeyCode=EKeySliderUp;
			}
		else if ( str==KKeyMenu )
			{
			aKeyCode=EKeyMenu;
			}
		else if ( str==KKeyDictaphonePlay )
			{
			aKeyCode=EKeyDictaphonePlay;
			}
		else if ( str==KKeyDictaphoneStop )
			{
			aKeyCode=EKeyDictaphoneStop;
			}
		else if ( str==KKeyDictaphoneRecord )
			{
			aKeyCode=EKeyDictaphoneRecord;
			}
		else if ( str==KKeyHelp )
			{
			aKeyCode=EKeyHelp;
			}
		else if ( str==KKeyDial )
			{
			aKeyCode=EKeyDial;
			}
		else if ( str==KKeyScreenDimension0 )
			{
			aKeyCode=EKeyScreenDimension0;
			}
		else if ( str==KKeyScreenDimension1 )
			{
			aKeyCode=EKeyScreenDimension1;
			}
		else if ( str==KKeyScreenDimension2 )
			{
			aKeyCode=EKeyScreenDimension2;
			}
		else if ( str==KKeyScreenDimension3 )
			{
			aKeyCode=EKeyScreenDimension3;
			}
		else if ( str==KKeyIncVolume )
			{
			aKeyCode=EKeyIncVolume;
			}
		else if ( str==KKeyDecVolume )
			{
			aKeyCode=EKeyDecVolume;
			}
		else if ( str==KKeyDevice0 )
			{
			aKeyCode=EKeyDevice0;
			}
		else if ( str==KKeyDevice1 )
			{
			aKeyCode=EKeyDevice1;
			}
		else if ( str==KKeyDevice2 )
			{
			aKeyCode=EKeyDevice2;
			}
		else if ( str==KKeyDevice3 )
			{
			aKeyCode=EKeyDevice3;
			}
		else if ( str==KKeyDevice4 )
			{
			aKeyCode=EKeyDevice4;
			}
		else if ( str==KKeyDevice5 )
			{
			aKeyCode=EKeyDevice5;
			}
		else if ( str==KKeyDevice6 )
			{
			aKeyCode=EKeyDevice6;
			}
		else if ( str==KKeyDevice7 )
			{
			aKeyCode=EKeyDevice7;
			}
		else if ( str==KKeyDevice8 )
			{
			aKeyCode=EKeyDevice8;
			}
		else if ( str==KKeyDevice9 )
			{
			aKeyCode=EKeyDevice9;
			}
		else if ( str==KKeyDeviceA )
			{
			aKeyCode=EKeyDeviceA;
			}
		else if ( str==KKeyDeviceB )
			{
			aKeyCode=EKeyDeviceB;
			}
		else if ( str==KKeyDeviceC )
			{
			aKeyCode=EKeyDeviceC;
			}
		else if ( str==KKeyDeviceD )
			{
			aKeyCode=EKeyDeviceD;
			}
		else if ( str==KKeyDeviceE )
			{
			aKeyCode=EKeyDeviceE;
			}
		else if ( str==KKeyDeviceF )
			{
			aKeyCode=EKeyDeviceF;
			}
		else if ( str==KKeyApplication0 )
			{
			aKeyCode=EKeyApplication0;
			}
		else if ( str==KKeyApplication1 )
			{
			aKeyCode=EKeyApplication1;
			}
		else if ( str==KKeyApplication2 )
			{
			aKeyCode=EKeyApplication2;
			}
		else if ( str==KKeyApplication3 )
			{
			aKeyCode=EKeyApplication3;
			}
		else if ( str==KKeyApplication4 )
			{
			aKeyCode=EKeyApplication4;
			}
		else if ( str==KKeyApplication5 )
			{
			aKeyCode=EKeyApplication5;
			}
		else if ( str==KKeyApplication6 )
			{
			aKeyCode=EKeyApplication6;
			}
		else if ( str==KKeyApplication7 )
			{
			aKeyCode=EKeyApplication7;
			}
		else if ( str==KKeyApplication8 )
			{
			aKeyCode=EKeyApplication8;
			}
		else if ( str==KKeyApplication9 )
			{
			aKeyCode=EKeyApplication9;
			}
		else if ( str==KKeyApplicationA )
			{
			aKeyCode=EKeyApplicationA;
			}
		else if ( str==KKeyApplicationB )
			{
			aKeyCode=EKeyApplicationB;
			}
		else if ( str==KKeyApplicationC )
			{
			aKeyCode=EKeyApplicationC;
			}
		else if ( str==KKeyApplicationD )
			{
			aKeyCode=EKeyApplicationD;
			}
		else if ( str==KKeyApplicationE )
			{
			aKeyCode=EKeyApplicationE;
			}
		else if ( str==KKeyApplicationF )
			{
			aKeyCode=EKeyApplicationF;
			}
		else if ( str==KKeyYes )
			{
			aKeyCode=EKeyYes;
			}
		else if ( str==KKeyNo )
			{
			aKeyCode=EKeyNo;
			}
		else if ( str==KKeyIncBrightness )
			{
			aKeyCode=EKeyIncBrightness;
			}
		else if ( str==KKeyDecBrightness )
			{
			aKeyCode=EKeyDecBrightness;
			}
		else if ( str==KKeyKeyboardExtend )
			{
			aKeyCode=EKeyKeyboardExtend;
			}
		else if ( str==KKeyDevice10 )
			{
			aKeyCode=EKeyDevice10;
			}
		else if ( str==KKeyDevice11 )
			{
			aKeyCode=EKeyDevice11;
			}
		else if ( str==KKeyDevice12 )
			{
			aKeyCode=EKeyDevice12;
			}
		else if ( str==KKeyDevice13 )
			{
			aKeyCode=EKeyDevice13;
			}
		else if ( str==KKeyDevice14 )
			{
			aKeyCode=EKeyDevice14;
			}
		else if ( str==KKeyDevice15 )
			{
			aKeyCode=EKeyDevice15;
			}
		else if ( str==KKeyDevice16 )
			{
			aKeyCode=EKeyDevice16;
			}
		else if ( str==KKeyDevice17 )
			{
			aKeyCode=EKeyDevice17;
			}
		else if ( str==KKeyDevice18 )
			{
			aKeyCode=EKeyDevice18;
			}
		else if ( str==KKeyDevice19 )
			{
			aKeyCode=EKeyDevice19;
			}
		else if ( str==KKeyDevice1A )
			{
			aKeyCode=EKeyDevice1A;
			}
		else if ( str==KKeyDevice1B )
			{
			aKeyCode=EKeyDevice1B;
			}
		else if ( str==KKeyDevice1C )
			{
			aKeyCode=EKeyDevice1C;
			}
		else if ( str==KKeyDevice1D )
			{
			aKeyCode=EKeyDevice1D;
			}
		else if ( str==KKeyDevice1E )
			{
			aKeyCode=EKeyDevice1E;
			}
		else if ( str==KKeyDevice1F )
			{
			aKeyCode=EKeyDevice1F;
			}
		else if ( str==KKeyApplication10 )
			{
			aKeyCode=EKeyApplication10;
			}
		else if ( str==KKeyApplication11 )
			{
			aKeyCode=EKeyApplication11;
			}
		else if ( str==KKeyApplication12 )
			{
			aKeyCode=EKeyApplication12;
			}
		else if ( str==KKeyApplication13 )
			{
			aKeyCode=EKeyApplication13;
			}
		else if ( str==KKeyApplication14 )
			{
			aKeyCode=EKeyApplication14;
			}
		else if ( str==KKeyApplication15 )
			{
			aKeyCode=EKeyApplication15;
			}
		else if ( str==KKeyApplication16 )
			{
			aKeyCode=EKeyApplication16;
			}
		else if ( str==KKeyApplication17 )
			{
			aKeyCode=EKeyApplication17;
			}
		else if ( str==KKeyApplication18 )
			{
			aKeyCode=EKeyApplication18;
			}
		else if ( str==KKeyApplication19 )
			{
			aKeyCode=EKeyApplication19;
			}
		else if ( str==KKeyApplication1A )
			{
			aKeyCode=EKeyApplication1A;
			}
		else if ( str==KKeyApplication1B )
			{
			aKeyCode=EKeyApplication1B;
			}
		else if ( str==KKeyApplication1C )
			{
			aKeyCode=EKeyApplication1C;
			}
		else if ( str==KKeyApplication1D )
			{
			aKeyCode=EKeyApplication1D;
			}
		else if ( str==KKeyApplication1E )
			{
			aKeyCode=EKeyApplication1E;
			}
		else if ( str==KKeyApplication1F )
			{
			aKeyCode=EKeyApplication1F;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aKeyCode=(TKeyCode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadGlyphBitmapType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TGlyphBitmapType& aGlyphBitmapType)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KGlyphBitmapTypeDefault )
			{
			aGlyphBitmapType=EDefaultGlyphBitmap;
			}
		else if ( str==KGlyphBitmapTypeMonochrome )
			{
			aGlyphBitmapType=EMonochromeGlyphBitmap;
			}
		else if ( str==KGlyphBitmapTypeAntiAliased )
			{
			aGlyphBitmapType=EAntiAliasedGlyphBitmap;
			}
		else if ( str==KGlyphBitmapTypeSubPixel )
			{
			aGlyphBitmapType=ESubPixelGlyphBitmap;
			}
		else if ( str==KGlyphBitmapTypeFourColourBlend )
			{
			aGlyphBitmapType=EFourColourBlendGlyphBitmap;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aGlyphBitmapType=(TGlyphBitmapType)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadGraphicsOrientation(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsBitGc::TGraphicsOrientation& aGraphicsOrientation)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KGraphicsOrientationNormal )
			{
			aGraphicsOrientation=CFbsBitGc::EGraphicsOrientationNormal;
			}
		else if ( str==KGraphicsOrientationRotated90 )
			{
			aGraphicsOrientation=CFbsBitGc::EGraphicsOrientationRotated90;
			}
		else if ( str==KGraphicsOrientationRotated180 )
			{
			aGraphicsOrientation=CFbsBitGc::EGraphicsOrientationRotated180;
			}
		else if ( str==KGraphicsOrientationRotated270 )
			{
			aGraphicsOrientation=CFbsBitGc::EGraphicsOrientationRotated270;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aGraphicsOrientation=(CFbsBitGc::TGraphicsOrientation)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadLoggingCommand(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsSession::TLoggingCommand& aLoggingCommand)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KLoggingEnable )
			{
			aLoggingCommand=RWsSession::ELoggingEnable;
			}
		else if ( str==KLoggingDisable )
			{
			aLoggingCommand=RWsSession::ELoggingDisable;
			}
		else if ( str==KLoggingStatusDump )
			{
			aLoggingCommand=RWsSession::ELoggingStatusDump;
			}
		else if ( str==KLoggingHeapDump )
			{
			aLoggingCommand=RWsSession::ELoggingHeapDump;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aLoggingCommand=(RWsSession::TLoggingCommand)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadModifierState(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TModifierState& aModifierState)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KModifierStateTurnOnModifier )
			{
			aModifierState=ETurnOnModifier;
			}
		else if ( str==KModifierStateTurnOffModifier )
			{
			aModifierState=ETurnOffModifier;
			}
		else if ( str==KModifierStateToggleModifier )
			{
			aModifierState=EToggleModifier;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aModifierState=(TModifierState)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadPasswordMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPasswordMode& aPasswordMode)
	{
	// Read dither from INI file
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KPasswordModeCancel )
			{
			aPasswordMode=EPasswordCancel;
			}
		else if ( str==KPasswordModeNone )
			{
			aPasswordMode=EPasswordNone;
			}
		else if ( str==KPasswordModeOnceADay )
			{
			aPasswordMode=EPasswordOnceADay;
			}
		else if ( str==KPasswordModeAlways )
			{
			aPasswordMode=EPasswordAlways;
			}
		else if ( str==KPasswordModeAlwaysTriggerNow )
			{
			aPasswordMode=EPasswordAlwaysTriggerNow;
			}
		else if ( str==KPasswordModeOnceADayTriggerNow )
			{
			aPasswordMode=EPasswordOnceADayTriggerNow;
			}
		else
			{
        	TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aPasswordMode=(TPasswordMode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadPenStyle(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TPenStyle& aPenStyle)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KPenStyleNull )
			{
			aPenStyle=CGraphicsContext::ENullPen;
			}
		else if ( str==KPenStyleSolid )
			{
			aPenStyle=CGraphicsContext::ESolidPen;
			}
		else if ( str==KPenStyleDotted )
			{
			aPenStyle=CGraphicsContext::EDottedPen;
			}
		else if ( str==KPenStyleDashed )
			{
			aPenStyle=CGraphicsContext::EDashedPen;
			}
		else if ( str==KPenStyleDotDash )
			{
			aPenStyle=CGraphicsContext::EDotDashPen;
			}
		else if ( str==KPenStyleDotDotDash )
			{
			aPenStyle=CGraphicsContext::EDotDotDashPen;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aPenStyle=(CGraphicsContext::TPenStyle)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadPointerCursorMode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPointerCursorMode& aPointerCursorMode)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KPointerCursorNone )
			{
			aPointerCursorMode=EPointerCursorNone;
			}
		else if ( str==KPointerCursorFixed )
			{
			aPointerCursorMode=EPointerCursorFixed;
			}
		else if ( str==KPointerCursorNormal )
			{
			aPointerCursorMode=EPointerCursorNormal;
			}
		else if ( str==KPointerCursorWindow )
			{
			aPointerCursorMode=EPointerCursorWindow;
			}
		else if ( str==KPointerCursorFirstMode )
			{
			aPointerCursorMode=EPointerCursorFirstMode;
			}
		else if ( str==KPointerCursorLastMode )
			{
			aPointerCursorMode=EPointerCursorLastMode;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aPointerCursorMode=(TPointerCursorMode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadPointerFilter(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPointerFilter& aPointerFilter)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		ret=ConvertToPointerFilter(str, aPointerFilter);
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadPointerFilter(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aPointerFilter)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		TUint	temp=0;
		ret=ConvertToPointerFilter(str, temp);
		if ( ret )
			{
			aPointerFilter=temp;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToPointerFilter(const TDesC& aStr, TPointerFilter& aPointerFilter)
	{
	TBool	ret=ETrue;

	if ( aStr==KPointerFilterEnterExit )
		{
		aPointerFilter=EPointerFilterEnterExit;
		}
	else if ( aStr==KPointerFilterMove )
		{
		aPointerFilter=EPointerFilterMove;
		}
	else if ( aStr==KPointerFilterDrag )
		{
		aPointerFilter=EPointerFilterDrag;
		}
	else if ( aStr==KPointerFilterGenerateSimulatedMove )
		{
		aPointerFilter=EPointerGenerateSimulatedMove;
		}
	else if ( aStr==KPointerFilterMoveEvents )
		{
		aPointerFilter=EPointerMoveEvents;
		}
	else
		{
		TUint	pointerFilter;
		TLex	lex(aStr);
		ret=(lex.Val(pointerFilter, EHex)==KErrNone);
		if ( ret )
			{
			aPointerFilter=(TPointerFilter)pointerFilter;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToPointerFilter(const TDesC& aStr, TUint& aPointerFilter)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToPointerFilter(tempStr, aPointerFilter);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToPointerFilter(tempStr, temp) )
			{
			aPointerFilter|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		TPointerFilter	pointerFilter;
		ret=ConvertToPointerFilter(aStr, pointerFilter);
		if ( ret )
			{
			aPointerFilter=(TUint)pointerFilter;
			}
		}

	return ret;
	}

#if defined(SYMBIAN_WSERV_AND_CONE_MULTIPLE_SCREENS)
TBool CT_GraphicsUtil::ReadPriorities(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TInt& aPriorities)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KPrioritiesAll )
			{
			aPriorities=EAllPriorities;
			}
		else
			{
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, aPriorities);
			}
		}

	return ret;
	}
#endif

TBool CT_GraphicsUtil::ReadPriority(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CActive::TPriority& aPriority)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KPriorityIdle )
			{
			aPriority=CActive::EPriorityIdle;
			}
		else if ( str==KPriorityLow )
			{
			aPriority=CActive::EPriorityLow;
			}
		else if ( str==KPriorityStandard )
			{
			aPriority=CActive::EPriorityStandard;
			}
		else if ( str==KPriorityUserInput )
			{
			aPriority=CActive::EPriorityUserInput;
			}
		else if ( str==KPriorityHigh )
			{
			aPriority=CActive::EPriorityHigh;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aPriority=(CActive::TPriority)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadScreenModeEnforcement(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TScreenModeEnforcement& aScreenModeEnforcement)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KSizeEnforcementNone )
			{
			aScreenModeEnforcement=ESizeEnforcementNone;
			}
		else if ( str==KSizeEnforcementPixelsAndRotation )
			{
			aScreenModeEnforcement=ESizeEnforcementPixelsAndRotation;
			}
		else if ( str==KSizeEnforcementPixelsTwipsAndRotation )
			{
			aScreenModeEnforcement=ESizeEnforcementPixelsTwipsAndRotation;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aScreenModeEnforcement=(TScreenModeEnforcement)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadSpriteInCompare(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aSpriteInCompare)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		TUint	temp=0;
		ret=ConvertToSpriteInCompare(str, temp);
		if ( ret )
			{
			aSpriteInCompare=temp;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToSpriteInCompare(const TDesC& aStr, TUint& aSpriteInCompare)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToSpriteInCompare(tempStr, aSpriteInCompare);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToPointerFilter(tempStr, temp) )
			{
			aSpriteInCompare|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		if ( aStr==KSpriteInCompareRemoveSprite )
			{
			aSpriteInCompare=CWsScreenDevice::ERemoveSprite;
			}
		else if ( aStr==KSpriteInCompareIncludeSprite )
			{
			aSpriteInCompare=CWsScreenDevice::EIncludeSprite;
			}
		else if ( aStr==KSpriteInCompareIncludeTextCursor )
			{
			aSpriteInCompare=CWsScreenDevice::EIncludeTextCursor;
			}
		else
			{
			TLex	lex(aStr);
			ret=(lex.Val(aSpriteInCompare, EHex)==KErrNone);
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadStdScanCode(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TStdScanCode& aStdScanCode)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KStdKeyNull )
			{
			aStdScanCode=EStdKeyNull;
			}
		else if ( str==KStdKeyBackspace )
			{
			aStdScanCode=EStdKeyBackspace;
			}
		else if ( str==KStdKeyTab )
			{
			aStdScanCode=EStdKeyTab;
			}
		else if ( str==KStdKeyEnter )
			{
			aStdScanCode=EStdKeyEnter;
			}
		else if ( str==KStdKeyEscape )
			{
			aStdScanCode=EStdKeyEscape;
			}
		else if ( str==KStdKeySpace )
			{
			aStdScanCode=EStdKeySpace;
			}
		else if ( str==KStdKeyPrintScreen )
			{
			aStdScanCode=EStdKeyPrintScreen;
			}
		else if ( str==KStdKeyPause )
			{
			aStdScanCode=EStdKeyPause;
			}
		else if ( str==KStdKeyHome )
			{
			aStdScanCode=EStdKeyHome;
			}
		else if ( str==KStdKeyEnd )
			{
			aStdScanCode=EStdKeyEnd;
			}
		else if ( str==KStdKeyPageUp )
			{
			aStdScanCode=EStdKeyPageUp;
			}
		else if ( str==KStdKeyPageDown )
			{
			aStdScanCode=EStdKeyPageDown;
			}
		else if ( str==KStdKeyInsert )
			{
			aStdScanCode=EStdKeyInsert;
			}
		else if ( str==KStdKeyDelete )
			{
			aStdScanCode=EStdKeyDelete;
			}
		else if ( str==KStdKeyLeftArrow )
			{
			aStdScanCode=EStdKeyLeftArrow;
			}
		else if ( str==KStdKeyRightArrow )
			{
			aStdScanCode=EStdKeyRightArrow;
			}
		else if ( str==KStdKeyUpArrow )
			{
			aStdScanCode=EStdKeyUpArrow;
			}
		else if ( str==KStdKeyDownArrow )
			{
			aStdScanCode=EStdKeyDownArrow;
			}
		else if ( str==KStdKeyLeftShift )
			{
			aStdScanCode=EStdKeyLeftShift;
			}
		else if ( str==KStdKeyRightShift )
			{
			aStdScanCode=EStdKeyRightShift;
			}
		else if ( str==KStdKeyLeftAlt )
			{
			aStdScanCode=EStdKeyLeftAlt;
			}
		else if ( str==KStdKeyRightAlt )
			{
			aStdScanCode=EStdKeyRightAlt;
			}
		else if ( str==KStdKeyLeftCtrl )
			{
			aStdScanCode=EStdKeyLeftCtrl;
			}
		else if ( str==KStdKeyRightCtrl )
			{
			aStdScanCode=EStdKeyRightCtrl;
			}
		else if ( str==KStdKeyLeftFunc )
			{
			aStdScanCode=EStdKeyLeftFunc;
			}
		else if ( str==KStdKeyRightFunc )
			{
			aStdScanCode=EStdKeyRightFunc;
			}
		else if ( str==KStdKeyCapsLock )
			{
			aStdScanCode=EStdKeyCapsLock;
			}
		else if ( str==KStdKeyNumLock )
			{
			aStdScanCode=EStdKeyNumLock;
			}
		else if ( str==KStdKeyScrollLock )
			{
			aStdScanCode=EStdKeyScrollLock;
			}
		else if ( str==KStdKeyF1 )
			{
			aStdScanCode=EStdKeyF1;
			}
		else if ( str==KStdKeyF2 )
			{
			aStdScanCode=EStdKeyF2;
			}
		else if ( str==KStdKeyF3 )
			{
			aStdScanCode=EStdKeyF3;
			}
		else if ( str==KStdKeyF4 )
			{
			aStdScanCode=EStdKeyF4;
			}
		else if ( str==KStdKeyF5 )
			{
			aStdScanCode=EStdKeyF5;
			}
		else if ( str==KStdKeyF6 )
			{
			aStdScanCode=EStdKeyF6;
			}
		else if ( str==KStdKeyF7 )
			{
			aStdScanCode=EStdKeyF7;
			}
		else if ( str==KStdKeyF8 )
			{
			aStdScanCode=EStdKeyF8;
			}
		else if ( str==KStdKeyF9 )
			{
			aStdScanCode=EStdKeyF9;
			}
		else if ( str==KStdKeyF10 )
			{
			aStdScanCode=EStdKeyF10;
			}
		else if ( str==KStdKeyF11 )
			{
			aStdScanCode=EStdKeyF11;
			}
		else if ( str==KStdKeyF12 )
			{
			aStdScanCode=EStdKeyF12;
			}
		else if ( str==KStdKeyF13 )
			{
			aStdScanCode=EStdKeyF13;
			}
		else if ( str==KStdKeyF14 )
			{
			aStdScanCode=EStdKeyF14;
			}
		else if ( str==KStdKeyF15 )
			{
			aStdScanCode=EStdKeyF15;
			}
		else if ( str==KStdKeyF16 )
			{
			aStdScanCode=EStdKeyF16;
			}
		else if ( str==KStdKeyF17 )
			{
			aStdScanCode=EStdKeyF17;
			}
		else if ( str==KStdKeyF18 )
			{
			aStdScanCode=EStdKeyF18;
			}
		else if ( str==KStdKeyF19 )
			{
			aStdScanCode=EStdKeyF19;
			}
		else if ( str==KStdKeyF20 )
			{
			aStdScanCode=EStdKeyF20;
			}
		else if ( str==KStdKeyF21 )
			{
			aStdScanCode=EStdKeyF21;
			}
		else if ( str==KStdKeyF22 )
			{
			aStdScanCode=EStdKeyF22;
			}
		else if ( str==KStdKeyF23 )
			{
			aStdScanCode=EStdKeyF23;
			}
		else if ( str==KStdKeyF24 )
			{
			aStdScanCode=EStdKeyF24;
			}
		else if ( str==KStdKeyXXX )
			{
			aStdScanCode=EStdKeyXXX;
			}
		else if ( str==KStdKeyComma )
			{
			aStdScanCode=EStdKeyComma;
			}
		else if ( str==KStdKeyFullStop )
			{
			aStdScanCode=EStdKeyFullStop;
			}
		else if ( str==KStdKeyForwardSlash )
			{
			aStdScanCode=EStdKeyForwardSlash;
			}
		else if ( str==KStdKeyBackSlash )
			{
			aStdScanCode=EStdKeyBackSlash;
			}
		else if ( str==KStdKeySemiColon )
			{
			aStdScanCode=EStdKeySemiColon;
			}
		else if ( str==KStdKeySingleQuote )
			{
			aStdScanCode=EStdKeySingleQuote;
			}
		else if ( str==KStdKeyHash )
			{
			aStdScanCode=EStdKeyHash;
			}
		else if ( str==KStdKeySquareBracketLeft )
			{
			aStdScanCode=EStdKeySquareBracketLeft;
			}
		else if ( str==KStdKeySquareBracketRight )
			{
			aStdScanCode=EStdKeySquareBracketRight;
			}
		else if ( str==KStdKeyMinus )
			{
			aStdScanCode=EStdKeyMinus;
			}
		else if ( str==KStdKeyEquals )
			{
			aStdScanCode=EStdKeyEquals;
			}
		else if ( str==KStdKeyNkpForwardSlash )
			{
			aStdScanCode=EStdKeyNkpForwardSlash;
			}
		else if ( str==KStdKeyNkpAsterisk )
			{
			aStdScanCode=EStdKeyNkpAsterisk;
			}
		else if ( str==KStdKeyNkpMinus )
			{
			aStdScanCode=EStdKeyNkpMinus;
			}
		else if ( str==KStdKeyNkpPlus )
			{
			aStdScanCode=EStdKeyNkpPlus;
			}
		else if ( str==KStdKeyNkpEnter )
			{
			aStdScanCode=EStdKeyNkpEnter;
			}
		else if ( str==KStdKeyNkp1 )
			{
			aStdScanCode=EStdKeyNkp1;
			}
		else if ( str==KStdKeyNkp2 )
			{
			aStdScanCode=EStdKeyNkp2;
			}
		else if ( str==KStdKeyNkp3 )
			{
			aStdScanCode=EStdKeyNkp3;
			}
		else if ( str==KStdKeyNkp4 )
			{
			aStdScanCode=EStdKeyNkp4;
			}
		else if ( str==KStdKeyNkp5 )
			{
			aStdScanCode=EStdKeyNkp5;
			}
		else if ( str==KStdKeyNkp6 )
			{
			aStdScanCode=EStdKeyNkp6;
			}
		else if ( str==KStdKeyNkp7 )
			{
			aStdScanCode=EStdKeyNkp7;
			}
		else if ( str==KStdKeyNkp8 )
			{
			aStdScanCode=EStdKeyNkp8;
			}
		else if ( str==KStdKeyNkp9 )
			{
			aStdScanCode=EStdKeyNkp9;
			}
		else if ( str==KStdKeyNkp0 )
			{
			aStdScanCode=EStdKeyNkp0;
			}
		else if ( str==KStdKeyNkpFullStop )
			{
			aStdScanCode=EStdKeyNkpFullStop;
			}
		else if ( str==KStdKeyMenu )
			{
			aStdScanCode=EStdKeyMenu;
			}
		else if ( str==KStdKeyBacklightOn )
			{
			aStdScanCode=EStdKeyBacklightOn;
			}
		else if ( str==KStdKeyBacklightOff )
			{
			aStdScanCode=EStdKeyBacklightOff;
			}
		else if ( str==KStdKeyBacklightToggle )
			{
			aStdScanCode=EStdKeyBacklightToggle;
			}
		else if ( str==KStdKeyIncContrast )
			{
			aStdScanCode=EStdKeyIncContrast;
			}
		else if ( str==KStdKeyDecContrast )
			{
			aStdScanCode=EStdKeyDecContrast;
			}
		else if ( str==KStdKeySliderDown )
			{
			aStdScanCode=EStdKeySliderDown;
			}
		else if ( str==KStdKeySliderUp )
			{
			aStdScanCode=EStdKeySliderUp;
			}
		else if ( str==KStdKeyDictaphonePlay )
			{
			aStdScanCode=EStdKeyDictaphonePlay;
			}
		else if ( str==KStdKeyDictaphoneStop )
			{
			aStdScanCode=EStdKeyDictaphoneStop;
			}
		else if ( str==KStdKeyDictaphoneRecord )
			{
			aStdScanCode=EStdKeyDictaphoneRecord;
			}
		else if ( str==KStdKeyHelp )
			{
			aStdScanCode=EStdKeyHelp;
			}
		else if ( str==KStdKeyOff )
			{
			aStdScanCode=EStdKeyOff;
			}
		else if ( str==KStdKeyDial )
			{
			aStdScanCode=EStdKeyDial;
			}
		else if ( str==KStdKeyIncVolume )
			{
			aStdScanCode=EStdKeyIncVolume;
			}
		else if ( str==KStdKeyDecVolume )
			{
			aStdScanCode=EStdKeyDecVolume;
			}
		else if ( str==KStdKeyDevice0 )
			{
			aStdScanCode=EStdKeyDevice0;
			}
		else if ( str==KStdKeyDevice1 )
			{
			aStdScanCode=EStdKeyDevice1;
			}
		else if ( str==KStdKeyDevice2 )
			{
			aStdScanCode=EStdKeyDevice2;
			}
		else if ( str==KStdKeyDevice3 )
			{
			aStdScanCode=EStdKeyDevice3;
			}
		else if ( str==KStdKeyDevice4 )
			{
			aStdScanCode=EStdKeyDevice4;
			}
		else if ( str==KStdKeyDevice5 )
			{
			aStdScanCode=EStdKeyDevice5;
			}
		else if ( str==KStdKeyDevice6 )
			{
			aStdScanCode=EStdKeyDevice6;
			}
		else if ( str==KStdKeyDevice7 )
			{
			aStdScanCode=EStdKeyDevice7;
			}
		else if ( str==KStdKeyDevice8 )
			{
			aStdScanCode=EStdKeyDevice8;
			}
		else if ( str==KStdKeyDevice9 )
			{
			aStdScanCode=EStdKeyDevice9;
			}
		else if ( str==KStdKeyDeviceA )
			{
			aStdScanCode=EStdKeyDeviceA;
			}
		else if ( str==KStdKeyDeviceB )
			{
			aStdScanCode=EStdKeyDeviceB;
			}
		else if ( str==KStdKeyDeviceC )
			{
			aStdScanCode=EStdKeyDeviceC;
			}
		else if ( str==KStdKeyDeviceD )
			{
			aStdScanCode=EStdKeyDeviceD;
			}
		else if ( str==KStdKeyDeviceE )
			{
			aStdScanCode=EStdKeyDeviceE;
			}
		else if ( str==KStdKeyDeviceF )
			{
			aStdScanCode=EStdKeyDeviceF;
			}
		else if ( str==KStdKeyApplication0 )
			{
			aStdScanCode=EStdKeyApplication0;
			}
		else if ( str==KStdKeyApplication1 )
			{
			aStdScanCode=EStdKeyApplication1;
			}
		else if ( str==KStdKeyApplication2 )
			{
			aStdScanCode=EStdKeyApplication2;
			}
		else if ( str==KStdKeyApplication3 )
			{
			aStdScanCode=EStdKeyApplication3;
			}
		else if ( str==KStdKeyApplication4 )
			{
			aStdScanCode=EStdKeyApplication4;
			}
		else if ( str==KStdKeyApplication5 )
			{
			aStdScanCode=EStdKeyApplication5;
			}
		else if ( str==KStdKeyApplication6 )
			{
			aStdScanCode=EStdKeyApplication6;
			}
		else if ( str==KStdKeyApplication7 )
			{
			aStdScanCode=EStdKeyApplication7;
			}
		else if ( str==KStdKeyApplication8 )
			{
			aStdScanCode=EStdKeyApplication8;
			}
		else if ( str==KStdKeyApplication9 )
			{
			aStdScanCode=EStdKeyApplication9;
			}
		else if ( str==KStdKeyApplicationA )
			{
			aStdScanCode=EStdKeyApplicationA;
			}
		else if ( str==KStdKeyApplicationB )
			{
			aStdScanCode=EStdKeyApplicationB;
			}
		else if ( str==KStdKeyApplicationC )
			{
			aStdScanCode=EStdKeyApplicationC;
			}
		else if ( str==KStdKeyApplicationD )
			{
			aStdScanCode=EStdKeyApplicationD;
			}
		else if ( str==KStdKeyApplicationE )
			{
			aStdScanCode=EStdKeyApplicationE;
			}
		else if ( str==KStdKeyApplicationF )
			{
			aStdScanCode=EStdKeyApplicationF;
			}
		else if ( str==KStdKeyYes )
			{
			aStdScanCode=EStdKeyYes;
			}
		else if ( str==KStdKeyNo )
			{
			aStdScanCode=EStdKeyNo;
			}
		else if ( str==KStdKeyIncBrightness )
			{
			aStdScanCode=EStdKeyIncBrightness;
			}
		else if ( str==KStdKeyDecBrightness )
			{
			aStdScanCode=EStdKeyDecBrightness;
			}
		else if ( str==KStdKeyKeyboardExtend )
			{
			aStdScanCode=EStdKeyKeyboardExtend;
			}
		else if ( str==KStdKeyDevice10 )
			{
			aStdScanCode=EStdKeyDevice10;
			}
		else if ( str==KStdKeyDevice11 )
			{
			aStdScanCode=EStdKeyDevice11;
			}
		else if ( str==KStdKeyDevice12 )
			{
			aStdScanCode=EStdKeyDevice12;
			}
		else if ( str==KStdKeyDevice13 )
			{
			aStdScanCode=EStdKeyDevice13;
			}
		else if ( str==KStdKeyDevice14 )
			{
			aStdScanCode=EStdKeyDevice14;
			}
		else if ( str==KStdKeyDevice15 )
			{
			aStdScanCode=EStdKeyDevice15;
			}
		else if ( str==KStdKeyDevice16 )
			{
			aStdScanCode=EStdKeyDevice16;
			}
		else if ( str==KStdKeyDevice17 )
			{
			aStdScanCode=EStdKeyDevice17;
			}
		else if ( str==KStdKeyDevice18 )
			{
			aStdScanCode=EStdKeyDevice18;
			}
		else if ( str==KStdKeyDevice19 )
			{
			aStdScanCode=EStdKeyDevice19;
			}
		else if ( str==KStdKeyDevice1A )
			{
			aStdScanCode=EStdKeyDevice1A;
			}
		else if ( str==KStdKeyDevice1B )
			{
			aStdScanCode=EStdKeyDevice1B;
			}
		else if ( str==KStdKeyDevice1C )
			{
			aStdScanCode=EStdKeyDevice1C;
			}
		else if ( str==KStdKeyDevice1D )
			{
			aStdScanCode=EStdKeyDevice1D;
			}
		else if ( str==KStdKeyDevice1E )
			{
			aStdScanCode=EStdKeyDevice1E;
			}
		else if ( str==KStdKeyDevice1F )
			{
			aStdScanCode=EStdKeyDevice1F;
			}
		else if ( str==KStdKeyApplication10 )
			{
			aStdScanCode=EStdKeyApplication10;
			}
		else if ( str==KStdKeyApplication11 )
			{
			aStdScanCode=EStdKeyApplication11;
			}
		else if ( str==KStdKeyApplication12 )
			{
			aStdScanCode=EStdKeyApplication12;
			}
		else if ( str==KStdKeyApplication13 )
			{
			aStdScanCode=EStdKeyApplication13;
			}
		else if ( str==KStdKeyApplication14 )
			{
			aStdScanCode=EStdKeyApplication14;
			}
		else if ( str==KStdKeyApplication15 )
			{
			aStdScanCode=EStdKeyApplication15;
			}
		else if ( str==KStdKeyApplication16 )
			{
			aStdScanCode=EStdKeyApplication16;
			}
		else if ( str==KStdKeyApplication17 )
			{
			aStdScanCode=EStdKeyApplication17;
			}
		else if ( str==KStdKeyApplication18 )
			{
			aStdScanCode=EStdKeyApplication18;
			}
		else if ( str==KStdKeyApplication19 )
			{
			aStdScanCode=EStdKeyApplication19;
			}
		else if ( str==KStdKeyApplication1A )
			{
			aStdScanCode=EStdKeyApplication1A;
			}
		else if ( str==KStdKeyApplication1B )
			{
			aStdScanCode=EStdKeyApplication1B;
			}
		else if ( str==KStdKeyApplication1C )
			{
			aStdScanCode=EStdKeyApplication1C;
			}
		else if ( str==KStdKeyApplication1D )
			{
			aStdScanCode=EStdKeyApplication1D;
			}
		else if ( str==KStdKeyApplication1E )
			{
			aStdScanCode=EStdKeyApplication1E;
			}
		else if ( str==KStdKeyApplication1F )
			{
			aStdScanCode=EStdKeyApplication1F;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aStdScanCode=(TStdScanCode)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadTextAlign(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TTextAlign& aTextAlign)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KTextAlignLeft )
			{
			aTextAlign=CGraphicsContext::ELeft;
			}
		else if ( str==KTextAlignCenter )
			{
			aTextAlign=CGraphicsContext::ECenter;
			}
		else if ( str==KTextAlignRight )
			{
			aTextAlign=CGraphicsContext::ERight;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aTextAlign=(CGraphicsContext::TTextAlign)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadTextDirection(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFont::TTextDirection& aTextDirection)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KTextDirectionHorizontal )
			{
			aTextDirection=CFont::EHorizontal;
			}
		else if ( str==KTextDirectionVertical )
			{
			aTextDirection=CFont::EVertical;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aTextDirection=(CFont::TTextDirection)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TPointerEvent::TType& aType)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KTypeButton1Down )
			{
			aType=TPointerEvent::EButton1Down;
			}
		else if ( str==KTypeButton1Up )
			{
			aType=TPointerEvent::EButton1Up;
			}
		else if ( str==KTypeButton2Down )
			{
			aType=TPointerEvent::EButton2Down;
			}
		else if ( str==KTypeButton2Up )
			{
			aType=TPointerEvent::EButton2Up;
			}
		else if ( str==KTypeButton3Down )
			{
			aType=TPointerEvent::EButton3Down;
			}
		else if ( str==KTypeButton3Up )
			{
			aType=TPointerEvent::EButton3Up;
			}
		else if ( str==KTypeDrag )
			{
			aType=TPointerEvent::EDrag;
			}
		else if ( str==KTypeMove )
			{
			aType=TPointerEvent::EMove;
			}
		else if ( str==KTypeButtonRepeat )
			{
			aType=TPointerEvent::EButtonRepeat;
			}
		else if ( str==KTypeSwitchOn )
			{
			aType=TPointerEvent::ESwitchOn;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aType=(TPointerEvent::TType)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TRawEvent::TType& aType)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KTypeNone )
			{
			aType=TRawEvent::ENone;
			}
		else if ( str==KTypePointerMove )
			{
			aType=TRawEvent::EPointerMove;
			}
		else if ( str==KTypePointerSwitchOn )
			{
			aType=TRawEvent::EPointerSwitchOn;
			}
		else if ( str==KTypeKeyDown )
			{
			aType=TRawEvent::EKeyDown;
			}
		else if ( str==KTypeKeyUp )
			{
			aType=TRawEvent::EKeyUp;
			}
		else if ( str==KTypeRedraw )
			{
			aType=TRawEvent::ERedraw;
			}
		else if ( str==KTypeSwitchOn )
			{
			aType=TRawEvent::ESwitchOn;
			}
		else if ( str==KTypeActive )
			{
			aType=TRawEvent::EActive;
			}
		else if ( str==KTypeInactive )
			{
			aType=TRawEvent::EInactive;
			}
		else if ( str==KTypeUpdateModifiers )
			{
			aType=TRawEvent::EUpdateModifiers;
			}
		else if ( str==KTypeButton1Down )
			{
			aType=TRawEvent::EButton1Down;
			}
		else if ( str==KTypeButton1Up )
			{
			aType=TRawEvent::EButton1Up;
			}
		else if ( str==KTypeButton2Down )
			{
			aType=TRawEvent::EButton2Down;
			}
		else if ( str==KTypeButton2Up )
			{
			aType=TRawEvent::EButton2Up;
			}
		else if ( str==KTypeButton3Down )
			{
			aType=TRawEvent::EButton3Down;
			}
		else if ( str==KTypeButton3Up )
			{
			aType=TRawEvent::EButton3Up;
			}
		else if ( str==KTypeSwitchOff )
			{
			aType=TRawEvent::ESwitchOff;
			}
		else if ( str==KTypeKeyRepeat )
			{
			aType=TRawEvent::EKeyRepeat;
			}
		else if ( str==KTypeCaseOpen )
			{
			aType=TRawEvent::ECaseOpen;
			}
		else if ( str==KTypeCaseClose )
			{
			aType=TRawEvent::ECaseClose;
			}
		else if ( str==KTypePointer3DInRange )
			{
			aType=TRawEvent::EPointer3DInRange;
			}
		else if ( str==KTypePointer3DOutOfRange )
			{
			aType=TRawEvent::EPointer3DOutOfRange;
			}
		else if ( str==KTypePointer3DTilt )
			{
			aType=TRawEvent::EPointer3DTilt;
			}
		else if ( str==KTypePointer3DRotation )
			{
			aType=TRawEvent::EPointer3DRotation;
			}
		else if ( str==KTypePointer3DTiltAndMove )
			{
			aType=TRawEvent::EPointer3DTiltAndMove;
			}
		else if ( str==KTypeButton4Down )
			{
			aType=TRawEvent::EButton4Down;
			}
		else if ( str==KTypeButton4Up )
			{
			aType=TRawEvent::EButton4Up;
			}
		else if ( str==KTypeButton5Down )
			{
			aType=TRawEvent::EButton5Down;
			}
		else if ( str==KTypeButton5Up )
			{
			aType=TRawEvent::EButton5Up;
			}
		else if ( str==KTypeButton6Down )
			{
			aType=TRawEvent::EButton6Down;
			}
		else if ( str==KTypeButton6Up )
			{
			aType=TRawEvent::EButton6Up;
			}
		else if ( str==KTypeRestartSystem )
			{
			aType=TRawEvent::ERestartSystem;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aType=(TRawEvent::TType)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadTerminateReason(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RDirectScreenAccess::TTerminationReasons& aReason)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if(str == KETerminateCancel)
			{
			aReason = RDirectScreenAccess::ETerminateCancel;
			}
		else if(str == KETerminateRegion)
			{
			aReason = RDirectScreenAccess::ETerminateRegion;
			}
		else if(str == KETerminateRotation)
			{
			aReason = RDirectScreenAccess::ETerminateRotation;
			}
		else if(str == KETerminateScreenMode)
			{
			aReason = RDirectScreenAccess::ETerminateScreenMode;
			}
		else
			{
			ret = EFalse;
			}
		}
	return ret;
	}

TBool CT_GraphicsUtil::ReadWindowBackupType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aWindowBackupType)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		TUint	temp=0;
		ret=ConvertToWindowBackupType(str, temp);
		if ( ret )
			{
			aWindowBackupType=temp;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToWindowBackupType(const TDesC& aStr, TWindowBackupType& aWindowBackupType)
	{
	TBool	ret=ETrue;
	if ( aStr==KWindowBackupTypeAreaBehind )
		{
		aWindowBackupType=EWindowBackupAreaBehind;
		}
	else if ( aStr==KWindowBackupTypeFullScreen )
		{
		aWindowBackupType=EWindowBackupFullScreen;
		}
	else
		{
		TUint	windowBackupType;
		TLex	lex(aStr);
		ret=(lex.Val(windowBackupType, EHex)==KErrNone);
		if ( ret )
			{
			aWindowBackupType=(TWindowBackupType)windowBackupType;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToWindowBackupType(const TDesC& aStr, TUint& aWindowBackupType)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToWindowBackupType(tempStr, aWindowBackupType);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToWindowBackupType(tempStr, temp) )
			{
			aWindowBackupType|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		TWindowBackupType	windowBackupType;
		ret=ConvertToWindowBackupType(aStr, windowBackupType);
		if ( ret )
			{
			aWindowBackupType=(TUint)windowBackupType;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadWsTransparencyPolicy(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsTransparencyPolicy& aWsTransparencyPolicy)
	{
	// Read displaymode from INI file
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KWsTransparencyPolicyDefault )
			{
			aWsTransparencyPolicy=ETransparencyDefault;
			}
		else if ( str==KWsTransparencyPolicyFreezeUnder )
			{
			aWsTransparencyPolicy=ETransparencyFreezeUnder;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aWsTransparencyPolicy=(TWsTransparencyPolicy)intVal;
				}
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadWsVisibilityChangedEvent(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsVisibilityChangedEvent& aWsVisibilityChangedEvent)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		ret=ConvertToWsVisibilityChangedEvent(str, aWsVisibilityChangedEvent.iFlags);
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToWsVisibilityChangedEvent(const TDesC& aStr, TUint& aWsVisibilityChangedEvent)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToWsVisibilityChangedEvent(tempStr, aWsVisibilityChangedEvent);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToWsVisibilityChangedEvent(tempStr, temp) )
			{
			aWsVisibilityChangedEvent|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		if ( aStr==KWsVisibilityChangedCanBeSeen )
			{
			aWsVisibilityChangedEvent=TWsVisibilityChangedEvent::ECanBeSeen;
			}
		else if ( aStr==KWsVisibilityChangedCantBeSeen )
			{
			aWsVisibilityChangedEvent=TWsVisibilityChangedEvent::ECantBeSeen;
			}
		else if ( aStr==KWsVisibilityChangedPartiallyVisible )
			{
			aWsVisibilityChangedEvent=TWsVisibilityChangedEvent::EPartiallyVisible;
			}
		else if ( aStr==KWsVisibilityChangedNotVisible )
			{
			aWsVisibilityChangedEvent=TWsVisibilityChangedEvent::ENotVisible;
			}
		else if ( aStr==KWsVisibilityChangedFullyVisible )
			{
			aWsVisibilityChangedEvent=TWsVisibilityChangedEvent::EFullyVisible;
			}
		else
			{
			TLex	lex(aStr);
			ret=(lex.Val(aWsVisibilityChangedEvent, EHex)==KErrNone);
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadLongCaptureFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TLongCaptureFlags& aLongCaptureFlags)
	{
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KELongCaptureShortEventImmediately )
			{
			aLongCaptureFlags = ELongCaptureShortEventImmediately;
			}
		else if ( str==KELongCaptureRepeatEvents )
			{
			aLongCaptureFlags = ELongCaptureRepeatEvents;
			}
		else if ( str==KELongCaptureNormal )
			{
			aLongCaptureFlags = ELongCaptureNormal;
			}
		else if ( str==KELongCaptureWaitShort )
			{
			aLongCaptureFlags = ELongCaptureWaitShort;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aLongCaptureFlags=(TLongCaptureFlags)intVal;
				}
			}
		}
	return ret;
	}

TBool CT_GraphicsUtil::ReadSpriteFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TSpriteFlags& aSpriteFlags)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		ret=ConvertToSpriteFlags(str, aSpriteFlags);
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadSpriteFlags(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TUint& aSpriteFlags)
    {
    TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		ret=ConvertToSpriteFlags(str, aSpriteFlags);
		}

	return ret;
    }

TBool CT_GraphicsUtil::ConvertToSpriteFlags(const TDesC& aStr, TSpriteFlags& aSpriteFlags)
	{
	TBool	ret=ETrue;
	if ( aStr==KESpriteFlash )
		{
		aSpriteFlags=ESpriteFlash;
		}
	else if ( aStr==KESpriteNoChildClip )
		{
		aSpriteFlags=ESpriteNoChildClip;
		}
	else if ( aStr==KESpriteNoShadows )
		{
		aSpriteFlags=ESpriteNoShadows;
		}
	else
		{
		TUint	spriteFlag;
		TLex	lex(aStr);
		ret=(lex.Val(spriteFlag, EHex)==KErrNone);
		if ( ret )
			{
			aSpriteFlags=(TSpriteFlags)spriteFlag;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ConvertToSpriteFlags(const TDesC& aStr, TUint& aSpriteFlags)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToSpriteFlags(tempStr, aSpriteFlags);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint	temp;
		if ( ConvertToSpriteFlags(tempStr, temp) )
			{
			aSpriteFlags|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		TSpriteFlags	spriteFlag;
		ret=ConvertToSpriteFlags(aStr, spriteFlag);
		if ( ret )
			{
			aSpriteFlags=(TUint)spriteFlag;
			}
		}

	return ret;
	}

TBool CT_GraphicsUtil::ReadCustomTextCursorAlignment(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsSession::TCustomTextCursorAlignment& aAlignment)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KECustomTextCursorAlignTop )
			{
			aAlignment=RWsSession::ECustomTextCursorAlignTop;
			}
		else if ( str==KECustomTextCursorAlignBaseline )
			{
			aAlignment=RWsSession::ECustomTextCursorAlignBaseline;
			}
		else if ( str==KECustomTextCursorAlignBottom )
			{
			aAlignment=RWsSession::ECustomTextCursorAlignBottom;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, intVal);
			if ( ret )
				{
	        	aAlignment=(RWsSession::TCustomTextCursorAlignment)intVal;
				}
			}
		}

	return ret;
	}

/**
 * Utility method that fetches TAlgStyle pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetAlgStyleL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TAlgStyle*& aData)
	{
	// get AlgStyleData object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<TAlgStyle*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches RDrawableWindow pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetDrawableWindowL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RDrawableWindow*& aData)
	{
	// get CFbsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<RDrawableWindow*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CGraphicsContext::TDrawTextExtendedParam pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetDrawTextExtendedParamL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TDrawTextExtendedParam*& aData)
	{
	// get CFbsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CGraphicsContext::TDrawTextExtendedParam*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CFbsBitmap pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetFbsBitmapL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsBitmap*& aData)
	{
	// get CFbsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CFbsBitmap*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CFbsDevice pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetFbsDeviceL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsDevice*& aData)
	{
	// get CFbsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CFbsDevice*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CFont pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetFontL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFont*& aData)
	{
	// get CFbsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CFont*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CPalette pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetPaletteL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CPalette*& aData)
	{
	// get PaletteData object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CPalette*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CWsBitmap pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsBitmapL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CWsBitmap*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CWsBitmap*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches TWsEvent pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsEventL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsEvent*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<TWsEvent*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches RWsGraphicMsgBuf pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsGraphicMsgBufL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, RWsGraphicMsgBuf*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<RWsGraphicMsgBuf*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches CWsScreenDevice pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsScreenDeviceL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CWsScreenDevice*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CWsScreenDevice*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches TWsGraphicMsgFixedBase pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsGraphicMsgFixedBaseL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsGraphicMsgFixedBase*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<TWsGraphicMsgFixedBase*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches TWsPriorityKeyEvent pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsPriorityKeyEventL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsPriorityKeyEvent*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<TWsPriorityKeyEvent*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

/**
 * Utility method that fetches TWsRedrawEvent pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetWsRedrawEventL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TWsRedrawEvent*& aData)
	{
	// get CWsBitmap data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<TWsRedrawEvent*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

TBool CT_GraphicsUtil::GetTextCursor(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TTextCursor& aTextCursor)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TPtrC	str;

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagTextCursorType);
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, tempStore, str);
	if ( ret )
		{
		if ( str==KETypeRectangle )
			{
			aTextCursor.iType = TTextCursor::ETypeRectangle;
			}
		else if ( str==KETypeHollowRectangle )
			{
			aTextCursor.iType = TTextCursor::ETypeHollowRectangle;
			}
		else if ( str==KETypeLastBasic )
			{
			aTextCursor.iType = TTextCursor::ETypeLastBasic;
			}
		else
			{
        	ret=aDataWrapper.GetIntFromConfig(aSectName, tempStore, aTextCursor.iType);
			}
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagTextCursorFlags);
	if ( aDataWrapper.GetStringFromConfig(aSectName, tempStore, str) )
		{
		if ( str==KEFlagNoFlash )
			{
			aTextCursor.iFlags = TTextCursor::EFlagNoFlash;
			}
		else if ( str==KEFlagClipHorizontal )
			{
			aTextCursor.iFlags = TTextCursor::EFlagClipHorizontal;
			}
		else if ( str==KEFlagClipVertical )
			{
			aTextCursor.iFlags = TTextCursor::EFlagClipVertical;
			}
		else
			{
    	    TInt	intVal=0;
        	ret=aDataWrapper.GetIntFromConfig(aSectName, tempStore, intVal);
			if ( ret )
				{
	        	aTextCursor.iFlags = (TUint)intVal;
				}
			}
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagTextCursorHeight);
	aDataWrapper.GetIntFromConfig(aSectName, tempStore, aTextCursor.iHeight);

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagTextCursorAscent);
	aDataWrapper.GetIntFromConfig(aSectName, tempStore, aTextCursor.iAscent);

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagTextCursorWidth);
	aDataWrapper.GetIntFromConfig(aSectName, tempStore, aTextCursor.iWidth);

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagTextCursorColor);
	aDataWrapper.GetRgbFromConfig(aSectName, tempStore, aTextCursor.iColor);

	return ret;
	}

TBool CT_GraphicsUtil::GetSpriteMemberL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TSpriteMember& aSpriteMember)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TBool	ret=ETrue;

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSpriteMemberBitmap);
	if ( !CT_GraphicsUtil::GetFbsBitmapL(aDataWrapper, aSectName, tempStore, aSpriteMember.iBitmap) )
		{
		ret=EFalse;
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSpriteMemberMaskBitmap);
	if ( !CT_GraphicsUtil::GetFbsBitmapL(aDataWrapper, aSectName, tempStore, aSpriteMember.iMaskBitmap) )
		{
		aSpriteMember.iMaskBitmap=NULL;
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSpriteMemberInvertMask);
	if ( !aDataWrapper.GetBoolFromConfig(aSectName, tempStore, aSpriteMember.iInvertMask) )
		{
		aSpriteMember.iInvertMask=EFalse;
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSpriteMemberDrawMode);
	if ( !CT_GraphicsUtil::ReadDrawMode(aDataWrapper, aSectName, tempStore, aSpriteMember.iDrawMode) )
		{
		ret=EFalse;
		}

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSpriteMemberOffset);
	if ( !aDataWrapper.GetPointFromConfig(aSectName, tempStore, aSpriteMember.iOffset) )
		{
		ret=EFalse;
		}

	TInt	interval;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSpriteMemberInterval);
	if ( aDataWrapper.GetIntFromConfig(aSectName, tempStore, interval) )
		{
		aSpriteMember.iInterval=interval;
		}
	return ret;
	}


TBool CT_GraphicsUtil::GetSpriteMemberListL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CArrayFix<TSpriteMember>& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TSpriteMember						spriteMember;

	aResult.Reset();
	TBool	ok=ETrue;
	for ( TInt index=0; ok; )
		{
		tempStore.Format(KFormatFieldNumber, &aKeyName, ++index);
		ok=CT_GraphicsUtil::GetSpriteMemberL(aDataWrapper, aSectName, tempStore, spriteMember);
		if ( ok )
			{
			aResult.AppendL(spriteMember);
			}
		}

	return aResult.Count()>0;
	}

TBool CT_GraphicsUtil::ReadTypefaceSupport(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TTypefaceSupport& aTypefaceSupport)
	{
	TTypefaceSupport typefaceSupport;
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TPtrC str;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTypefaceFontName);
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, tempStore, str);
	if(ret)
		{
		typefaceSupport.iTypeface.iName = str;


		TBool attri = EFalse;
		tempStore.Format(KFormatEntryField, &aKeyName, &KProportional);
		if(aDataWrapper.GetBoolFromConfig(aSectName, tempStore, attri))
			{
			typefaceSupport.iTypeface.SetIsProportional(attri);
			}
		tempStore.Format(KFormatEntryField, &aKeyName, &KSerif);
		if(aDataWrapper.GetBoolFromConfig(aSectName, tempStore, attri))
			{
			typefaceSupport.iTypeface.SetIsSerif(attri);
			}
		tempStore.Format(KFormatEntryField, &aKeyName, &KSymbol);
		if(aDataWrapper.GetBoolFromConfig(aSectName, tempStore, attri))
			{
			typefaceSupport.iTypeface.SetIsSymbol(attri);
			}
		
		tempStore.Format(KFormatEntryField, &aKeyName, &KNumHeights);
		aDataWrapper.GetIntFromConfig(aSectName, tempStore, typefaceSupport.iNumHeights);

		tempStore.Format(KFormatEntryField, &aKeyName, &KIsScalable);
		aDataWrapper.GetBoolFromConfig(aSectName, tempStore, typefaceSupport.iIsScalable);
	
		tempStore.Format(KFormatEntryField, &aKeyName, &KMaxHeightInTwips);
		aDataWrapper.GetIntFromConfig(aSectName, tempStore, typefaceSupport.iMaxHeightInTwips);
		
		tempStore.Format(KFormatEntryField, &aKeyName, &KMinHeightInTwips);
		aDataWrapper.GetIntFromConfig(aSectName, tempStore, typefaceSupport.iMinHeightInTwips);

		aTypefaceSupport = typefaceSupport;
		}
	return ret;
	}
	
TBool CT_GraphicsUtil::BringAppForegroundL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aSessionKeyName , const TDesC& aAppKeyName)
	{
	TBool result = ETrue;
	RWsSession* iClient( NULL ); //Window Session Client
	TPtrC objectName;
	
	if ( aDataWrapper.GetStringFromConfig(aSectName, aSessionKeyName, objectName) )
		{
		iClient = static_cast<RWsSession*>(aDataWrapper.GetDataObjectL(objectName));
		}
	else
		{
		result = EFalse;
		}
	
	if( !aDataWrapper.GetStringFromConfig(aSectName, aAppKeyName, objectName))
		{
		result = EFalse;
		}
	
	if( result )
		{
		TApaTaskList tasklist(*iClient);
		TApaTask task(tasklist.FindApp(objectName));
		task.BringToForeground();	
		}
	return result;
	}

void CT_GraphicsUtil::EatupMemory(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName)
    {
    // read left size in memory from INI
    TInt	leftsize=0;
	TBool	ret=aDataWrapper.GetIntFromConfig(aSectName, aKeyName, leftsize);
	if ( !ret || leftsize==0)
		return;
	
	// eat memory until fail to avaiable memory is less than is left size  
    TInt nTotalSize=User::Heap().Size();
    TAny* mem=User::AllocZ(nTotalSize);
    
    if (mem)
    	iMemArray.Append(mem);
    
    do {
    	mem=User::AllocZ(leftsize);
		if(mem)
			{		
			iMemArray.Append(mem);
			}
    } while(mem);
    
    // get avaiable memory, we can't log here, because no enough memory to write log
    // so we have to write log after free memory
    iAvaiableSpace=User::Available(iBiggestBlock);
    }

void CT_GraphicsUtil::FreeEatenMemory(CDataWrapperBase& aDataWrapper)
    {    
    for (TInt i=0;i<iMemArray.Count();i++)
    	{
    	TAny* mem=iMemArray[i];
    	User::Free(mem);
    	}	
    
	iMemArray.Reset();
	
	// write memory status after eat memory
	aDataWrapper.INFO_PRINTF3(_L("CT_GraphicsUtil::FreeEatenMemory, before free memory, avaiable=%d byte biggestblock=%d byte"),iAvaiableSpace,iBiggestBlock);
    }

TBool CT_GraphicsUtil::GetFrameInfo(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, TFrameInfo& frameinfo)
	{
	// get iFrameCoordsInPixels from parameters
	aDataWrapper.GetRectFromConfig(aSectName, KFldFrameCoordsInPixels, frameinfo.iFrameCoordsInPixels);
		
	//get iFrameSizeInTwips from parameters
	aDataWrapper.GetSizeFromConfig(aSectName,KFldFrameSizeInTwips,frameinfo.iFrameSizeInTwips);
	
	//get iBitsPerPixel from parameters
	aDataWrapper.GetIntFromConfig(aSectName,KFldBitsPerPixel,frameinfo.iBitsPerPixel);
	
	//get iDelay from parameters
	TInt delay=0;
	TBool ret=aDataWrapper.GetIntFromConfig(aSectName,KFldDelay,delay);
	
	if(ret)
		{
		frameinfo.iDelay=TTimeIntervalMicroSeconds(delay);
		}
	
	//get iFlags from parameters,when set frame info, must set flag.
	ret=ReadFrameInfoFlags(aDataWrapper,aSectName,KFldFlags,frameinfo.iFlags);
	
	//get iOverallSizeInPixels from parameters
	aDataWrapper.GetSizeFromConfig(aSectName,KFldOverallSizeInPixels,frameinfo.iOverallSizeInPixels);
	
	//get iFrameDisplayMode from parameters
	ReadDisplayMode(aDataWrapper,aSectName,KFldFrameDisplayMode,frameinfo.iFrameDisplayMode);
	
	//get iBackgroundColor from parameters
	aDataWrapper.GetRgbFromConfig(aSectName,KFldBackgroundColor,frameinfo.iBackgroundColor);
	
	//get iFrameSizeInPixels from parameters
	aDataWrapper.GetSizeFromConfig(aSectName,KFldFrameSizeInPixels,frameinfo.iFrameSizeInPixels);
	
	//get FrameState from parameters
	TFrameInfo::TFrameInfoState state;
	
	if(GetFrameInfoState(aDataWrapper,aSectName,KFldFrameInfoState,state))
		{
		frameinfo.SetCurrentFrameState(state);
		}
	
	//get current data offset from parameters
	TInt offset;
	
	if(aDataWrapper.GetIntFromConfig(aSectName,KFldCurrentDataOffset,offset))
		{
		frameinfo.SetCurrentDataOffset(offset);
		}
	
	//get frame data offset from parameters
	if(aDataWrapper.GetIntFromConfig(aSectName,KFldFrameDataOffset,offset))
		{
		frameinfo.SetFrameDataOffset(offset);
		}
	
	return ret;
	}

TBool CT_GraphicsUtil::ReadFrameInfoFlags(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName,TUint32& aFlags)
	{
	TPtrC 	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		ret=ConvertToFrameInfoFlags(str, aFlags);
		}

	return ret;
	}

TBool CT_GraphicsUtil::GetShaperFactoryL(CDataWrapperBase & aDataWrapper, const TDesC & aSectName, const TDesC & aKeyName, CShaperFactory *& aData)
	{
	// get CShaperFactory data object from parameters
	TPtrC name;
	TBool ret = aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if (ret)
		{
		// Data object found
		aData = static_cast<CShaperFactory*> (aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

TBool CT_GraphicsUtil::GetRasterizerL(CDataWrapperBase & aDataWrapper, const TDesC & aSectName, const TDesC & aKeyName, COpenFontRasterizer *& aData)
	{
	// get COpenFontRasterizer data object from parameters
	TPtrC name;
	TBool ret = aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if (ret)
		{
		// Data object found
		aData = static_cast<COpenFontRasterizer*> (aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

TBool CT_GraphicsUtil::GetOpenFontGlyphDataL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TOpenFontGlyphData*& aData)
	{
	// get TOpenFontGlyphData data object from parameters
	TPtrC name;
	TBool ret = aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if (ret)
		{
		// Data object found
		aData = static_cast<TOpenFontGlyphData*> (aDataWrapper.GetDataObjectL(name));
		}
	
	return ret;
	}

TBool  CT_GraphicsUtil::ConvertToFrameInfoFlags(const TDesC& aStr,TUint32& aFrameInfoFlags)
	{
	TBool	ret=ETrue;
	TInt	location=aStr.Match(_L("*|*"));
	if( location!=KErrNotFound )
		{
		// Converting Left part of the data
		TPtrC	tempStr=aStr.Left(location);
		ret=ConvertToFrameInfoFlags(tempStr, aFrameInfoFlags);

		// Converting right data can be with another "|"
		tempStr.Set(aStr.Mid(location+1));

		TUint32	temp;
		if ( ConvertToFrameInfoFlags(tempStr, temp) )
			{
			aFrameInfoFlags|=temp;
			}
		else
			{
			ret=EFalse;
			}
		}
	else
		{
		if ( aStr==KEColor )
			{
			aFrameInfoFlags=TFrameInfo::EColor;
			}
		else if ( aStr==KETransparencyPossible )
			{
			aFrameInfoFlags=TFrameInfo::ETransparencyPossible;
			}
		else if ( aStr==KEFullyScaleable )
			{
			aFrameInfoFlags=TFrameInfo::EFullyScaleable;
			}
		else if ( aStr==KEConstantAspectRatio )
			{
			aFrameInfoFlags=TFrameInfo::EConstantAspectRatio;
			}
		else if ( aStr==KECanDither )
			{
			aFrameInfoFlags=TFrameInfo::ECanDither;
			}
		else if ( aStr==KEAlphaChannel )
			{
			aFrameInfoFlags=TFrameInfo::EAlphaChannel;
			}
		else if ( aStr==KELeaveInPlace )
			{
			aFrameInfoFlags=TFrameInfo::ELeaveInPlace;
			}
		else if ( aStr==KERestoreToBackground )
			{
			aFrameInfoFlags=TFrameInfo::ERestoreToBackground;
			}
		else if ( aStr==KERestoreToPrevious )
			{
			aFrameInfoFlags=TFrameInfo::ERestoreToPrevious;
			}
		else if ( aStr==KEPartialDecodeInvalid )
			{
			aFrameInfoFlags=TFrameInfo::EPartialDecodeInvalid;
			}
		else if ( aStr==KEMngMoreFramesToDecode )
			{
			aFrameInfoFlags=TFrameInfo::EMngMoreFramesToDecode;
			}
		else if ( aStr==KEUsesFrameSizeInPixels )
			{
			aFrameInfoFlags=TFrameInfo::EUsesFrameSizeInPixels;
			}
		else
			{
			TUint32	frameinfoFlag;
			TLex	lex(aStr);
			ret=(lex.Val(frameinfoFlag, EHex)==KErrNone);
			if ( ret )
				{
				aFrameInfoFlags=frameinfoFlag;
				}
			}
		}
	return ret;
	}

TBool CT_GraphicsUtil::GetFrameInfoState(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName,TFrameInfo::TFrameInfoState& aState)
	{
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KEFrameInfoUninitialised )
			{
			aState=TFrameInfo::EFrameInfoUninitialised;
			}
		else if ( str==KEFrameInfoProcessingFrameHeader )
			{
			aState=TFrameInfo::EFrameInfoProcessingFrameHeader;
			}
		else if ( str==KEFrameInfoProcessingFrame )
			{
			aState=TFrameInfo::EFrameInfoProcessingFrame;
			}
		else if ( str==KEFrameInfoProcessingComplete)
			{
			aState=TFrameInfo::EFrameInfoProcessingComplete;
			}
		else
			{
			TUint	state;
			TLex	lex(str);
			ret=(lex.Val(state, EHex)==KErrNone);
			if ( ret )
				{
				aState=(TFrameInfo::TFrameInfoState)state;
				}
			}
		}
	
	return ret;
	}


TBool CT_GraphicsUtil::GetWsGraphicAnimationL(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName, TWsGraphicAnimation*& aData)
	{
	// get TWsGraphicAnimation data object from parameters
	TPtrC	name;
	TBool	ret = aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<TWsGraphicAnimation*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}

TBool CT_GraphicsUtil::StartAnimTesterClientL(CDataWrapperBase& aDataWrapper,const TDesC& aSectName)
	{
	TPtrC processName;
	if( !aDataWrapper.GetStringFromConfig(aSectName, KFldProcessName, processName) )
		{
		return EFalse;
		}
	TInt id;
	if( !aDataWrapper.GetIntFromConfig(aSectName, KFldId, id) )
		{
		return EFalse;
		}
	TRect rect;
	if( !aDataWrapper.GetRectFromConfig(aSectName, KFldRect, rect) )
		{
		return EFalse;
		}
	TPtrC objectName;
	if ( !aDataWrapper.GetStringFromConfig(aSectName, KFldWsga, objectName) )
		{
		return EFalse;
		}	
	TWsGraphicAnimation* wsGraphAnim;
	wsGraphAnim = static_cast<TWsGraphicAnimation*>(aDataWrapper.GetDataObjectL(objectName));
		
	_LIT(KSpace, " ");
	TBuf<128> args;
	// Initialize arguments	
	args.Zero();
	args.Append(processName);
	args.Append(KSpace);
	args.AppendNum(0);			// screen number
	args.Append(KSpace);
	args.AppendNum(id);
	args.Append(KSpace);
	args.AppendNum(rect.iTl.iX);
	args.Append(KSpace);
	args.AppendNum(rect.iTl.iY);
	args.Append(KSpace);
	args.AppendNum(rect.iBr.iX);
	args.Append(KSpace);
	args.AppendNum(rect.iBr.iY);
	args.Append(KSpace);
	TBuf<32> data;
	data.Copy(wsGraphAnim->Pckg());
	args.Append(data);
	
	// Launch client process
  	RProcess process;
  	CleanupClosePushL(process);
	User::LeaveIfError(process.Create(processName,args));
  	process.Resume();

  	// Synchronize the process
  	RSemaphore semaphore;
  	CleanupClosePushL(semaphore);
  	TInt err = semaphore.OpenGlobal(processName);
  	if( KErrNotFound == err )
  		{
  		User::LeaveIfError(semaphore.CreateGlobal(processName,0));
  		// Wait until new process finish drawing
  		}
  	else if( KErrNone != err )
  		{
  		User::LeaveIfError(err);
  		}
	semaphore.Wait();
  	CleanupStack::PopAndDestroy(2);  	
  	return ETrue;
	}
  
TBool CT_GraphicsUtil::CloseAnimTesterClientL(CDataWrapperBase& aDataWrapper,const TDesC& aSectName)
	{
	TPtrC processName;
	if( !aDataWrapper.GetStringFromConfig(aSectName, KFldProcessName, processName) )
		{
		return EFalse;
		}

	RSemaphore semaphore;
	CleanupClosePushL(semaphore);
	User::LeaveIfError(semaphore.OpenGlobal(processName));
	// Waken waiting process
	semaphore.Signal();
	CleanupStack::PopAndDestroy();
 	return ETrue;
	}
	
TBool CT_GraphicsUtil::ReadAnimType(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TInt& aType)
	{
	TPtrC	str;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if ( ret )
		{
		if ( str==KEAnimWindows)
			{
			aType=EKWindowAnim;
			}
		else if ( str==KEAnimSprite )
			{
			aType=EKSpriteAnim;
			}
		else if ( str==KEAnimFreeTimer )
			{
			aType=EKFreeTimerWindowAnim;
			}
		else
			ret=EFalse;
		}

	return ret;	
	}

TBool CT_GraphicsUtil::ReadAnimCommand(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, TInt& aCommand)
	{
	TPtrC	str;
		TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
		if ( ret )
			{
			if ( str==KEAnimCmdSync )
				{
				aCommand=ECmdSync;
				}
			else if ( str==KEAnimCmdTestWithoutPara )
				{
				aCommand=ECmdTestWithoutPara;
				}
			else if ( str==KEAnimCmdTestWithPara )
				{
				aCommand=ECmdTestWithPara;
				}
			else if ( str==KEAnimCmdGetLast )
				{
				aCommand=ECmdGetLast;
				}
			else if ( str==KEAnimCmdGetAnimInterval )
				{
				aCommand=ECmdGetAnimInterval;
				}
	//MAnimGeneralFunctions
	    else if (str==KEMAnimECmdRetrieveResult)
				{
				aCommand=ECmdRetrieveResult;
				}
	    else if(str==KEMAnimECmdUtilEatupMemory)
	    		{
	    		aCommand=ECmdUtilEatupMemory;
	    		}
	    else if(str==KEMAnimECmdUtilFreeEatenMemory)
	    		{
	    		aCommand=ECmdUtilFreeEatenMemory;	    	
	    		}
	    else if (str==KEMAnimECmdGeneralAnimate)
				{
				aCommand=ECmdGeneralAnimate;
				}
			else if (str==KEMAnimECmdGeneralClient)
				{
				aCommand=ECmdGeneralClient;
				}	
	    else if (str==KEMAnimECmdGeneralFlashStateOn)
				{
				aCommand=ECmdGeneralFlashStateOn;
				}
	    else if (str==KEMAnimECmdGeneralPanic)
				{
				aCommand=ECmdGeneralPanic;
				}
	    else if (str==KEMAnimECmdGeneralScreenDevice)
				{
				aCommand=ECmdGeneralScreenDevice;
				}
	    else if (str==KEMAnimECmdGeneralExtendedInterface)
				{
				aCommand=ECmdGeneralExtendedInterface;
				}
	     else if (str==KEMAnimECmdGeneralWindowExtension)
				{
				aCommand=ECmdGeneralWindowExtension;
				}
			 else if (str==KEMAnimECmdGeneralNumOfExtInterfaces)
				{
				aCommand=ECmdGeneralNumOfExtInterfaces;
				} 
	     else if (str==KEMAnimECmdGeneralEventExtension)
				{
				aCommand=ECmdGeneralEventExtension;
				}
			 else if ( str==KEMAnimECmdGeneralGetRawEvents)
			 	{
			 	aCommand=ECmdGeneralGetRawEvents;	
			 	} 
	     else if ( str==KEMAnimECmdGeneralPostRawEvent)
			 	{
			 	aCommand=ECmdGeneralPostRawEvent;	
			 	} 
	     else if ( str==KEMAnimECmdGeneralPostKeyEvent)
			 	{
			 	aCommand=ECmdGeneralPostKeyEvent;	
			 	} 
	     else if ( str==KEMAnimECmdGeneralSetSync)
			 	{
			 	aCommand=ECmdGeneralSetSync;	
			 	} 
	      else if ( str==KEMAnimECmdGeneralSync)
			 	{
			 	aCommand=ECmdGeneralSync;	
			 	}
	      else if ( str==KEMAnimECmdGeneralSetInterval)
			 	{
			 	aCommand=ECmdGeneralSetInterval;	
			 	}
	      else if ( str==KEMAnimECmdGeneralSetNextInterval)
			 	{
			 	aCommand=ECmdGeneralSetNextInterval;	
			 	}
	      else if ( str==KEMAnimECmdGeneralSystemTime)
			 	{
			 	aCommand=ECmdGeneralSystemTime;	
			 	}
	      else if ( str==KEMAnimECmdGeneralRegisterForNotis)
			 	{
			 	aCommand=ECmdGeneralRegisterForNotis;	
			 	}
	      else if ( str==KEMAnimECmdGeneralMessage)
			 	{
			 	aCommand=ECmdGeneralMessage;	
			 	} 
	      else if ( str==KEMAnimECmdGeneralDuplicateBitmapL)
			 	{
			 	aCommand=ECmdGeneralDuplicateBitmapL;	
			 	} 
	      else if ( str==KEMAnimECmdGeneralDuplicateFontL)
			 	{
			 	aCommand=ECmdGeneralDuplicateFontL;	
			 	}
	      else if ( str==KEMAnimECmdGeneralCloseFont)
			 	{
			 	aCommand=ECmdGeneralCloseFont;	
			 	}
	      //Opcodes for MAnimSpriteFunctions          
	      else if ( str==KEMAnimECmdSpriteGetSpriteMember)
			 	{
			 	aCommand=ECmdSpriteGetSpriteMember;	
			 	}
	      else if ( str==KEMAnimECmdSpriteUpdateMember)
			 	{
			 	aCommand=ECmdSpriteUpdateMember;	
			 	}
	      else if ( str==KEMAnimECmdSpriteActivate)
			 	{
			 	aCommand=ECmdSpriteActivate;	
			 	}   
	      else if ( str==KEMAnimECmdSpriteSpriteCanBeSeen)
			 	{
			 	aCommand=ECmdSpriteSpriteCanBeSeen;	
			 	} 
	      else if ( str==KEMAnimECmdSpriteSizeChangedL)
			 	{
			 	aCommand=ECmdSpriteSizeChangedL;	
			 	} 
	      else if ( str==KEMAnimECmdSpriteSetPosition)
			 	{
			 	aCommand=ECmdSpriteSetPosition;	
			 	} 
			 	else if (str==KEMAnimECmdGeneralReplyBuf8)
			 	{
			 	aCommand=ECmdGeneralReplyBuf8;
			 	}
			 	else if (str==KEMAnimECmdGeneralReplyBuf16)
			 	{
			 	aCommand=ECmdGeneralReplyBuf16;
			 	}
			 	
	 //MAnimWindowFunctions

			  else if (str==KEMAnimECmdWindowActivateGc)
				{
				aCommand=ECmdWindowActivateGc;
				}
			else if (str==KEMAnimECmdWindowSetRect)	
				{
				aCommand=ECmdWindowSetRect;
				}
			else if (str==KEMAnimECmdWindowWindowSize)
				{
				aCommand=ECmdWindowWindowSize;
				}
			else if (str==KEMAnimECmdWindowIsHidden)
				{
				aCommand=ECmdWindowIsHidden;
				}
			else if (str==KEMAnimECmdWindowSetVisible)
				{
				aCommand=ECmdWindowSetVisible;
				}
			else if (str==KEMAnimECmdWindowInvalidate)
				{
				aCommand=ECmdWindowInvalidate;
				}
			else if (str==KEMAnimECmdWindowParameters)
				{
				aCommand=ECmdWindowParameters;
				}
			else if (str==KEMAnimECmdWindowVisibleRegion)
				{
				aCommand=ECmdWindowVisibleRegion;
				}
			else if (str==KEMAnimECmdWindowIsStarted)
				{
				aCommand=ECmdWindowIsStarted;
				}
			else if(str==KEMAnimECmdRetrieveResult)
				{
				aCommand=ECmdRetrieveResult;
				}
			//Add Opcodes covertion here for MAnimGeneralFunctionsWindowExtension	
				
			else if (str==KEMAnimECmdGfweScreens)
				{
				aCommand=ECmdGfweScreens;
				}
			else if (str==KEMAnimECmdGfweFocusScreens)
				{
				aCommand=ECmdGfweFocusScreens;
				}
			else if (str==KEMAnimECmdGfweSetFocusScreen)
				{
				aCommand=ECmdGfweSetFocusScreen;
				}
			else if (str==KEMAnimECmdGfweWindowGroups)
				{
				aCommand=ECmdGfweWindowGroups;
				}
			else if (str==KEMAnimECmdGfweWindowGroupInfo)
				{
				aCommand=ECmdGfweWindowGroupInfo;
				}
			else if (str==KEMAnimECmdGfweWindowGroupName)
				{
				aCommand=ECmdGfweWindowGroupName;
				}
			else if (str==KEMAnimECmdGfweSetOrdinalPosition)
				{
				aCommand=ECmdGfweSetOrdinalPosition;
				}
			else if (str==KEMAnimECmdGfweIsFocusable)
				{
				aCommand=ECmdGfweIsFocusable;
				}
			else if (str==KEMAnimECmdGfweWindowConfig)
				{
				aCommand=ECmdGfweWindowConfig;
				}
			//Add Opcodes covertion here for MAnimFreeTimerWindowFunctions	
			//*
			///*
			else if (str==KEMAnimECmdFreeTimerWinDeactivateGc)
				{
				aCommand=ECmdFreeTimerWinDeactivateGc;
				}
			else if (str==KEMAnimECmdFreeTimerWinUpdate)
				{
				aCommand=ECmdFreeTimerWinUpdate;
				}
			else
				ret=EFalse;
			}

		return ret;
	}

TBool CT_GraphicsUtil::GetFbsBitGcL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CFbsBitGc*& aData)
	{
	// get CFbsBitGc data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CFbsBitGc*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}


TBool CT_GraphicsUtil::GetTextParametersL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CGraphicsContext::TTextParameters* aParam)
	{
	aParam=new (ELeave) CGraphicsContext::TTextParameters;
	
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
    TInt start;
    TInt end;
    TInt flags;
    TInt enabled;
	TBool	ret=ETrue;
	TBool	dataOK=ETrue;

	tempStore.Format(KFormatEntryField, &aKeyName, &KTagParamEnabled);
	dataOK=aDataWrapper.GetIntFromConfig(aSectName, tempStore, enabled);
	
	//Get TTextParameters::iFlags from ini file or use the default value of it.
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagParamFlags);
	ret=aDataWrapper.GetIntFromConfig(aSectName, tempStore, flags);
    if(ret)
    	{
    	INFO_PRINTF2_UTIL(_L("Getting TTextParameters::iFlags from ini file: iFlags=%d"), flags);
    	aParam->iFlags=flags;
    	}
	else
		{
		INFO_PRINTF1_UTIL(_L("Using default value of TTextParameters::iFlags"));
		}
	//Get TTextParameters::iStart from ini file.
    tempStore.Format(KFormatEntryField, &aKeyName, &KTagParamStart);
	ret=aDataWrapper.GetIntFromConfig(aSectName, tempStore, start);
    if(!ret)
    	{
		if(!dataOK)
			{
	    	INFO_PRINTF1_UTIL(_L("Didn't get TTextParameters::iStart from ini file that won't use TTextParameters"));
	    	return ret;
			}
		ret=ETrue;
    	INFO_PRINTF1_UTIL(_L("Use TTextParameters without data"));
		return ret;
    	}
    else
    	{
        aParam->iStart=start;
        INFO_PRINTF2_UTIL(_L("Getting TTextParameters::iStart from ini file: iStart=%d"),start);
    	}

	//Get TTextParameters::iEnd from ini file.
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagParamEnd);
	ret=aDataWrapper.GetIntFromConfig(aSectName, tempStore, end);
    if(!ret)
    	{
    	if(!dataOK)
    		{
        	INFO_PRINTF1_UTIL(_L("Didn't get TTextParameters::iEnd from ini file that won't use TTextParameters")); 	
    		return ret;
    		}
    	else
    		{
    		ret=ETrue;
        	INFO_PRINTF1_UTIL(_L("Use TTextParameters without data"));
    		return ret;
    		}
    	}
    else
    	{
        aParam->iEnd=end;
        INFO_PRINTF2_UTIL(_L("Getting TTextParameters::iEnd from ini file: iEnd=%d"),end); 
        return ret;
    	}

    }

TBool CT_GraphicsUtil::GetGraphicsOrientation(CDataWrapperBase& aDataWrapper,const TDesC& aSectName, const TDesC& aKeyName,CFbsBitGc::TGraphicsOrientation& aParam)
	{
	TPtrC str;
	TBool ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, str);
	if (ret)
		{
		if ( str==KEGraphicsOrientationNormal )
			{
			aParam=CFbsBitGc::EGraphicsOrientationNormal;
			}
		else if(str==KEGraphicsOrientationRotated90)
			{
			aParam=CFbsBitGc::EGraphicsOrientationRotated90;
			}
		else if(str==KEGraphicsOrientationRotated180)
			{
			aParam=CFbsBitGc::EGraphicsOrientationRotated180;
			}
		else if(str==KEGraphicsOrientationRotated270)
			{
			aParam=CFbsBitGc::EGraphicsOrientationRotated270;
			}
		else
			{
			ret=EFalse;
			}
		}
	return ret;
	}

/**
 * Utility method that fetches CLinkedTypefaceSpecification pointer by command parameter name from INI-file.
 */
TBool CT_GraphicsUtil::GetLinkedTypefaceSpecificationL(CDataWrapperBase& aDataWrapper, const TDesC& aSectName, const TDesC& aKeyName, CLinkedTypefaceSpecification*& aData)
	{
	// get CLinkedTypefaceSpecification data object from parameters
	TPtrC	name;
	TBool	ret=aDataWrapper.GetStringFromConfig(aSectName, aKeyName, name);
	if ( ret )
		{
		// Data object found
		aData=static_cast<CLinkedTypefaceSpecification*>(aDataWrapper.GetDataObjectL(name));
		}

	return ret;
	}
