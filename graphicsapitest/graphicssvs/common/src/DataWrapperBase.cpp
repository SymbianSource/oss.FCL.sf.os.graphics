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

/*@{*/
///	Constant Literals used.
_LIT(KIncludeSection,		"include");
_LIT(KFile,					"file%d");
_LIT(KMatch,				"*{*,*}*");
_LIT(KStart,				"{");
_LIT(KSeparator,			",");
_LIT(KEnd,					"}");
_LIT(KDataRead,				"INI READ : %S %S %S");

_LIT(KFormatEntryField,		"%S_%S");

_LIT(KTagFontSpecName,		"name");
_LIT(KTagFontSpecHeight,	"height");

_LIT(KTagPointX,			"x");
_LIT(KTagPointY,			"y");

_LIT(KTagRectTop,			"top");
_LIT(KTagRectLeft,			"left");
_LIT(KTagRectBottom,		"bottom");
_LIT(KTagRectRight,			"right");

_LIT(KTagRgbRed,			"red");
_LIT(KTagRgbGreen,			"green");
_LIT(KTagRgbBlue,			"blue");
_LIT(KTagRgbAlpha,			"alpha");

_LIT(KTagSizeWidth,			"width");
_LIT(KTagSizeHeight,		"height");

_LIT(KTagWsGraphicIdUid,	"uid");
_LIT(KTagWsGraphicIdId,		"id");
_LIT(KTagWsGraphicIdName,	"name");

_LIT(KFormatFieldNumber,	"%S%d");

///	Logging
_LIT(KLogMissingParameter,	"Missing parameter '%S'");
_LIT(KLogRegion,			"Region '%S'");
_LIT(KLogEmpty,				"Empty");
_LIT(KLogRegionsRect,		"Rect[%d] tl=(%d,%d) br=(%d,%d)");
_LIT(KLogRect,				"%S  tl=(%d,%d) br=(%d,%d)");
/*@}*/

CDataWrapperBase::CDataWrapperBase()
:	CDataWrapper()
,	iHandle(0)
,	iId(TWsGraphicId::EUninitialized)
	{
	}

CDataWrapperBase::~CDataWrapperBase()
/**
 * Public destructor
 */
	{
	iInclude.ResetAndDestroy();
	iBuffer.ResetAndDestroy();
	iFs.Close();
	}

void CDataWrapperBase::InitialiseL()
	{
	CDataWrapper::InitialiseL();

	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TPtrC		fileName;
	TBool		moreData=ETrue;
	TBool		index=0;
	while ( moreData )
		{
		tempStore.Format(KFile(), ++index);
		moreData=GetStringFromConfig(KIncludeSection, tempStore, fileName);

		if (moreData)
			{
			CIniData*	iniData=CIniData::NewL(fileName);
			CleanupStack::PushL(iniData);
			iInclude.Append(iniData);
			CleanupStack::Pop(iniData);
			}
		}
	User::LeaveIfError(iFs.Connect());
	}

TBool CDataWrapperBase::GetBoolFromConfig(const TDesC& aSectName,const TDesC& aKeyName,TBool& aResult)
	{
	TBool	ret=EFalse;
	TPtrC	result;
	TRAPD(err, ret=GetCommandStringParameterL(aSectName, aKeyName, result));
	if ( err != KErrNone )
		{
		ret=EFalse;
		}
	if ( ret )
		{
		_LIT(KTrue,"true");
		aResult=(result.FindF(KTrue) != KErrNotFound);
		}

	return ret;
	}

TBool CDataWrapperBase::GetFontSpecFromConfig(const TDesC& aSectName,const TDesC& aKeyName,TFontSpec& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;


	TPtrC	name;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagFontSpecName);
	TBool	ret=GetStringFromConfig(aSectName, tempStore, name);

	TInt	height;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagFontSpecHeight);
	if ( !GetIntFromConfig(aSectName, tempStore, height) )
		{
		ret=EFalse;
		}

	if ( ret )
		{
		aResult=TFontSpec(name, height);
		}

	return ret;
	}

TBool CDataWrapperBase::GetIntFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt& aResult)
	{
	TPtrC	result;
	TBool	ret=EFalse;
	TRAPD(err, ret=GetCommandStringParameterL(aSectName, aKeyName, result));
	if ( err != KErrNone )
		{
		ret=EFalse;
		}
	if ( ret )
		{
		TLex	lex(result);
		ret=(lex.Val(aResult)==KErrNone);
		}

	return ret;
	}

TBool CDataWrapperBase::GetStringFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TPtrC& aResult)
	{
	TBool	ret=EFalse;
	TRAPD(err, ret=GetCommandStringParameterL(aSectName, aKeyName, aResult));
	if ( err != KErrNone )
		{
		ret=EFalse;
		}
	return ret;
	}

TBool CDataWrapperBase::GetHexFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt& aResult)
	{
	TPtrC	result;
	TBool	ret=EFalse;
	TRAPD(err, ret=GetCommandStringParameterL(aSectName, aKeyName, result));
	if ( err != KErrNone )
		{
		ret=EFalse;
		}
	if ( ret )
		{
		TLex	lex(result);
		ret=(lex.Val((TUint &)aResult, EHex)==KErrNone);
		}

	return ret;
	}

TBool CDataWrapperBase::GetPointFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TPoint& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TInt	x;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagPointX);
	TBool	ret=GetIntFromConfig(aSectName, tempStore, x);

	TInt	y;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagPointY);
	if ( !GetIntFromConfig(aSectName, tempStore, y) )
		{
		ret=EFalse;
		}

	if ( ret )
		{
		aResult.SetXY(x, y);
		}

	return ret;
	}

TBool CDataWrapperBase::GetRectFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TRect& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TInt	top;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRectTop);
	TBool	ret=GetIntFromConfig(aSectName, tempStore, top);

	TInt	left;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRectLeft);
	if ( !GetIntFromConfig(aSectName, tempStore, left) )
		{
		ret=EFalse;
		}

	TInt	bottom;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRectBottom);
	if ( !GetIntFromConfig(aSectName, tempStore, bottom) )
		{
		ret=EFalse;
		}

	TInt	right;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRectRight);
	if ( !GetIntFromConfig(aSectName, tempStore, right) )
		{
		ret=EFalse;
		}

	if ( ret )
		{
		aResult.SetRect(left, top, right, bottom);
		}

	return ret;
	}

TBool CDataWrapperBase::GetRegionFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TRegion& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TRect								rect;

	aResult.Clear();
	TBool	moreData=ETrue;
	for ( TInt index=0; moreData; )
		{
		tempStore.Format(KFormatFieldNumber, &aKeyName, ++index);
		moreData=GetRectFromConfig(aSectName, tempStore, rect);
		if ( moreData )
			{
			aResult.AddRect(rect);
			}
		}

	return aResult.Count()>0;
	}

TBool CDataWrapperBase::GetRgbFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TRgb& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TInt	red;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRgbRed);
	TBool	ret=GetIntFromConfig(aSectName, tempStore, red);

	TInt	green;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRgbGreen);
	if ( !GetIntFromConfig(aSectName, tempStore, green) )
		{
		ret=EFalse;
		}

	TInt	blue;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagRgbBlue);
	if ( !GetIntFromConfig(aSectName, tempStore, blue) )
		{
		ret=EFalse;
		}

	if ( ret )
		{
		aResult.SetRed(red);
		aResult.SetGreen(green);
		aResult.SetBlue(blue);

		TInt	alpha;
		tempStore.Format(KFormatEntryField, &aKeyName, &KTagRgbAlpha);
		if ( GetIntFromConfig(aSectName, tempStore, alpha) )
			{
			aResult.SetAlpha(alpha);
			}
		}

	return ret;
	}

TBool CDataWrapperBase::GetSizeFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TSize& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TInt	width;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSizeWidth);
	TBool	ret=GetIntFromConfig(aSectName, tempStore, width);

	TInt	height;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagSizeHeight);
	if ( !GetIntFromConfig(aSectName, tempStore, height) )
		{
		ret=EFalse;
		}

	if ( ret )
		{
		aResult.SetSize(width, height);
		}

	return ret;
	}

TBool CDataWrapperBase::GetRgbListFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt aSize, TRgb*& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TBool	ok=ETrue;
	for ( TInt index=0; (index<aSize) && (ok); )
		{
		tempStore.Format(KFormatFieldNumber, &aKeyName, ++index);
		ok=GetRgbFromConfig(aSectName, tempStore, aResult[index-1]);
		if ( !ok )
			{
			ERR_PRINTF2(KLogMissingParameter, &tempStore);
			SetBlockResult(EFail);
			}
		}

	return ok;
	}

TBool CDataWrapperBase::GetPointListFromConfig(const TDesC& aSectName, const TDesC& aKeyName, TInt aSize, TPoint*& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TBool	ok=ETrue;
	for ( TInt index=0; (index<aSize) && (ok); )
		{
		tempStore.Format(KFormatFieldNumber, &aKeyName, ++index);
		ok=GetPointFromConfig(aSectName, tempStore, aResult[index-1]);
		if ( !ok )
			{
			ERR_PRINTF2(KLogMissingParameter, &tempStore);
			SetBlockResult(EFail);
			}
		}

	return ok;
	}

TBool CDataWrapperBase::GetPointListFromConfigL(const TDesC& aSectName, const TDesC& aKeyName, CArrayFix<TPoint>& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;
	TPoint								point;

	aResult.Reset();
	TBool	ok=ETrue;
	for ( TInt index=0; ok; )
		{
		tempStore.Format(KFormatFieldNumber, &aKeyName, ++index);
		ok=GetPointFromConfig(aSectName, tempStore, point);
		if ( ok )
			{
			aResult.AppendL(point);
			}
		}

	return aResult.Count()>0;
	}

TBool CDataWrapperBase::GetCommandStringParameterL(const TDesC& aSectName, const TDesC& aKeyName, TPtrC& aResult)
	{
	TBool	ret=EFalse;

	if ( aSectName.Length()!=0 )
		{
		ret=CDataWrapper::GetStringFromConfig(aSectName, aKeyName, aResult);

		for ( TInt index=iInclude.Count(); (index>0) && (!ret); )
			{
			ret=iInclude[--index]->FindVar(aSectName, aKeyName, aResult);
			}
		}

	if ( ret )
		{
		if ( aResult.Match(KMatch)!=KErrNotFound )
			{
			//	We have an entry of the format
			//	entry =*{section,entry}*
			//	where * is one or more characters
			//	We need to construct this from other data in the ini file replacing {*,*}
			//	with the data from
			//	[section]
			//	entry =some_value
			HBufC*	buffer=HBufC::NewLC(aResult.Length());
			buffer->Des().Copy(aResult);

			TInt	startLength=KStart().Length();
			TInt	sparatorLength=KSeparator().Length();
			TInt	endLength=KEnd().Length();
			TInt	bufferLength;
			TInt	start;
			TInt	sparator;
			TInt	end;
			TPtrC	remaining;
			TLex	lex;
			do
				{
				bufferLength=buffer->Length();
				start=buffer->Find(KStart);

				remaining.Set(buffer->Des().Right(bufferLength-start-startLength));
				sparator=remaining.Find(KSeparator);
				remaining.Set(remaining.Right(remaining.Length()-sparator-sparatorLength));
				sparator += (start + startLength);

				end=remaining.Find(KEnd) + sparator + sparatorLength;

				TPtrC	sectionName(buffer->Ptr()+start+startLength, sparator-start-startLength);
				TPtrC	keyName(buffer->Ptr()+sparator+sparatorLength, end-sparator-sparatorLength);
				sectionName.Set(TLex(sectionName).NextToken());
				keyName.Set(TLex(keyName).NextToken());

				TInt	entrySize=0;
				TPtrC	entryData;
				TBool	found=CDataWrapper::GetStringFromConfig(sectionName, keyName, entryData);
				for ( TInt index=iInclude.Count(); (index>0) && (!found);  )
					{
					found=iInclude[--index]->FindVar(sectionName, keyName, entryData);
					}
				if ( found )
					{
					entrySize=entryData.Length();
					}

				TInt	newLength=start + bufferLength - end - endLength + entrySize;
				HBufC*	bufferNew=HBufC::NewLC(newLength);
				bufferNew->Des().Copy(buffer->Ptr(), start);
				if ( entrySize>0 )
					{
					bufferNew->Des().Append(entryData);
					}
				bufferNew->Des().Append(buffer->Ptr() + end + endLength, bufferLength - end - endLength);
				CleanupStack::Pop(bufferNew);
				CleanupStack::PopAndDestroy(buffer);
				buffer=bufferNew;
				CleanupStack::PushL(buffer);
				}
			while ( buffer->Match(KMatch)!=KErrNotFound );
			iBuffer.Append(buffer);
			CleanupStack::Pop(buffer);
			aResult.Set(*buffer);
			INFO_PRINTF4(KDataRead, &aSectName, &aKeyName , &aResult);
			}
		}

	return ret;
	}

TBool CDataWrapperBase::GetWsGraphicIdFromConfigL(const TDesC& aSectName, const TDesC& aKeyName, TWsGraphicId& aResult)
	{
	TBuf<KMaxTestExecuteCommandLength>	tempStore;

	TInt	id;
	tempStore.Format(KFormatEntryField, &aKeyName, &KTagWsGraphicIdUid);
	TBool	ret=GetIntFromConfig(aSectName, tempStore, id);

	if ( ret )
		{
		TUid	uid=TUid::Uid(id);
		aResult.Set(uid);
		}
	else
		{
		tempStore.Format(KFormatEntryField, &aKeyName, &KTagWsGraphicIdId);
		ret=GetIntFromConfig(aSectName, tempStore, id);
		if ( ret )
			{
			aResult.Set(id);
			}
		else
			{
			tempStore.Format(KFormatEntryField, &aKeyName, &KTagWsGraphicIdName);
			TPtrC	name;
			ret=GetStringFromConfig(aSectName, tempStore, name);
			if ( ret )
				{
				CDataWrapperBase*	wrapper=static_cast<CDataWrapperBase*>(GetDataWrapperL(name));
				aResult=wrapper->GetId();
				}
			}
		}

	return ret;
	}

    TBool CDataWrapperBase::GetDateTimeFromConfig(const TDesC& aSectName,  TDateTime& aResult)
    	{
    	TInt year , month , day , hour , minute, second, microsecond ;
    	
    	// Fields
    	_LIT(KYear,			"year");
    	_LIT(KMonth,		"month");
    	_LIT(KDay,      	"day");
    	_LIT(KHour,         "hour");
    	_LIT(KMinute,		"minute");
    	_LIT(KSecond,		"second");
    	_LIT(KMicrosecond,  "microsecond");

    	TBool ret = ETrue ; 
    	if ( !GetIntFromConfig( aSectName, KYear , year))
    		{
			ret=EFalse;
			}
    	if ( !GetIntFromConfig( aSectName, KMonth , month))
    		{
			ret=EFalse;
			}     	 
    	if ( !GetIntFromConfig( aSectName, KDay, day))
    		{
			ret=EFalse;
			} 
    	if ( !GetIntFromConfig( aSectName, KHour, hour))
    		{
			ret=EFalse;
			} 
    	if ( !GetIntFromConfig( aSectName, KMinute , minute))
    		{
			ret=EFalse;
			}  
    	if ( !GetIntFromConfig( aSectName, KSecond , second))
    		{
			ret=EFalse;
			}  
    	if ( !GetIntFromConfig( aSectName, KMicrosecond , microsecond))
    		{
			ret=EFalse;
			} 
    	if ( ret )
			{
			TMonth amonth ;
			switch (month) 
			  {
			  case 1:
			       amonth = EJanuary ;
			       break ;
			  case 2:
			       amonth = EFebruary  ;
		           break ;
			  case 3 :
			       amonth = EMarch  ;
			       break ;
			  case 4: 
			       amonth = EApril ;
			       break ;
			  case 5:
			       amonth = EMay  ;
		           break ;
			  case 6 :
			       amonth = EJune ;
			       break ;
			  case 7 :
			       amonth = EJuly ;
			       break ;
			  case 8 :
			       amonth = EAugust ;
			       break ;
			  case 9 :
			       amonth = ESeptember ;
			       break ;
			  case 10 :
			       amonth = EOctober ;
			       break ;
			  case 11 :
			       amonth = ENovember ;
			       break ;
			  case 12:
			       amonth = EDecember ;
			       break ;
			  default :
			       return ret ;
			  }
			
			aResult.Set(year,amonth ,day,hour,minute,second,microsecond);
			}
    	 
    	return ret ;
    	}
void CDataWrapperBase::LogRegion(const TDesC& aMessage, const TRegion& aRegion)
	{
	INFO_PRINTF2(KLogRegion, &aMessage);
	TInt	indCount = aRegion.Count();
	if ( indCount==0 )
		{
		INFO_PRINTF1(KLogEmpty);
		}
	else
		{
		const TRect*	rect=aRegion.RectangleList();
		for ( TInt index=0; index<indCount; ++index )
			{
			INFO_PRINTF6(KLogRegionsRect, index, rect[index].iTl.iX, rect[index].iTl.iY, rect[index].iBr.iX, rect[index].iBr.iY);
			}
		}
	}

void CDataWrapperBase::LogRect(const TDesC& aMessage, const TRect& aRect)
	{
	INFO_PRINTF6(KLogRect, &aMessage, aRect.iTl.iX, aRect.iTl.iY, aRect.iBr.iX, aRect.iBr.iY);
	}
