/*
* Copyright (c) 2009 Symbian Foundation Ltd
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Symbian Foundation Ltd - initial contribution.
* 
* Contributors:
*
* Description:
* Declaration of native window type structure
*/

#include <e32std.h>
#include <w32std.h>

#ifndef EGLOSNATVIEWINDOWTYPE_H
#define EGLOSNATVIEWINDOWTYPE_H

#define KDefaultScreenNumber 0

class REglWindowBase
	{
public:
	//virtual TBool IsValid() const = 0;
	//virtual TSize SizeInPixels() const = 0;
	//virtual TSize SizeInTwips() const = 0;
	//virtual TInt ScreenNumber() const = 0;
protected:
	REglWindowBase();
private:
	friend class TNativeWindowType;
	TBool IsRWindow() const;
private:
	TInt32 iFourByteOffset;
	TAny* iIsRWindow;
	};


class TNativeWindowType
	{
public:
	IMPORT_C TNativeWindowType(EGLNativeWindowType aNativeWindow);
	IMPORT_C TNativeWindowType();
	IMPORT_C void SetNativeWindow(EGLNativeWindowType aNativeWindow);
	IMPORT_C TBool IsValid() const;
	IMPORT_C TSize SizeInPixels() const;
	//virtual TSize SizeInTwips() const = 0;
	IMPORT_C TInt ScreenNumber() const;
	IMPORT_C TBool IsWindow() const;
	
	TSize				iSize; //to keep
	CFbsBitmap* 		iBitmap; //to keep
private:
	REglWindowBase*		iEglWindow;
	TBool 				iIsValid;
	};

#endif //EGLOSCALLBACK_H
