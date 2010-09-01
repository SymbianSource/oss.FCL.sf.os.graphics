// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Tables of OpenWF-C strings and extensions, plus access for EGL.
//
//

#include <string.h>
#include <e32def.h>
#include <WF/wfc.h>
#include <KHR/khrplatform.h>
#include "owfextensions.h"
#include "owftypes.h"

/*!
 * Vendor, renderer and version strings
 */
const char* wfc_strings[] =
{
    "Nokia",
    "Sample Implementation",
    "1.0",
};

/*
 * Table of extensions supported, terminated by a NULL.
 */
const char *wfc_extensions[] =
{
    NULL
};

#ifdef __cplusplus
typedef void (*OWFExtensionFunction)(...);
#else
typedef void (*OWFExtensionFunction)();
#endif

typedef struct OWFExtension_tag
{
    const char *name;
    OWFExtensionFunction function;
} OWFExtension;

/*
 * Table of extension function name/pointer pairs, terminated with a NULL name
 * pair (pointer not used for terminating entry).
 */
static OWFExtension OWF_ExtensionFunctionTable[] =
{
    { NULL, NULL }
};

OWF_API_CALL OWFExtensionFunction
owfGetProcAddress(const char *procName)
{
    WFCint ii;
    
    if (!procName)
        {
        return NULL;
        }
    
    for (ii=0; OWF_ExtensionFunctionTable[ii].name != NULL; ii++)
    {
       if (strcmp(procName, OWF_ExtensionFunctionTable[ii].name)==0)
       {
           return OWF_ExtensionFunctionTable[ii].function;
       }
    }
    
    return NULL;
}

/*
 * eglGetProcAddress uses export 1 (implemented as GetExtensionFunctionTable) to
 * fill in a function pointer table. The first entry is the function that looks up
 * a function pointer for a given name. Other entries could be added in the
 * future if coordinated with the EGL implementation.
 */

typedef struct
{
    OWFExtensionFunction (*owfGetProcAddressFn) (const char* procname);
} owfCFunctions;

extern "C" WFC_API_CALL void WFC_APIENTRY
GetExtensionFunctionTable(owfCFunctions* fnTable) WFC_APIEXIT
{
    fnTable->owfGetProcAddressFn = owfGetProcAddress;
}
