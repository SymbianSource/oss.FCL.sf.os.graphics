/* Copyright (c) 2009 The Khronos Group Inc.
 * Portions copyright (c) 2009-2010  Nokia Corporation and/or its subsidiary(-ies)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */


#include "owfdebug.h"
#ifdef DEBUG_LOG

#include <pthread.h>

#include "owfdebug.h"
#include "owftypes.h"
#include "owfdebug.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef OWF_DEBUG_PREFIX
#define OWF_DEBUG_PREFIX "OWF: "
#endif
int xprintf(const char* aFormat, ...);

/* This pair of macros are a standard trick 
 * to convert the content of a macro into a string after expansion
 * TOSTR causes the macro to be expanded, then TOSTR2 applies quotes.
 * This is used because .MMP MACRO does not consistantly process macro value quotes.
 */
#define TOSTR2(x)    #x
#define TOSTR(x)    TOSTR2(x)


/* Filters debug messages based on filename or function name.
 * @param symbol the file or function name string generated by predefined macros
 * @param symlen the length of the string (calculated at compile-time)
 * Defining DEBUG_LOG enables all log file output. Do this in the make or .MMP file
 * The file output can be filtered by filename or by function name:
 * Defining DEBUG_FUNCTION=fnname will filter for all functions STARTING with the name fnname
 * Defining DEBUG_FILE=filename will filter for all functions ending with the name filename
 * 
 * These filters utilise the predefined macros __FILE__ and __FUNCTION__ which are defined in current standards,
 * but may contain platform-specific features such as path names, or linkage decoration,
 * so the filter-string may require some platform-specific tuning.
 * 
 * Note that use of #pragma message may also be platform-specific, and is only present in order to 
 * print clues about the format of the __FILE__ and __FUNCTION__ macros.
 */
int  OWF_Debug_DoLog(const char* symbol,int symlen)
    {
#if defined(DEBUG_LOG_FILE)
#pragma message ("DEBUG filter DEBUG_FILE= '" TOSTR(DEBUG_FILE) "' eg= '" __FILE__ "'")
    if (symlen>=(sizeof(TOSTR(DEBUG_FILE))-1))
        {
        return strncmp( symbol+symlen-(sizeof(TOSTR(DEBUG_FILE))-1) , TOSTR(DEBUG_FILE) , sizeof(TOSTR(DEBUG_FILE))-1 )==0;
        }
    else
        return 0;
#elif defined(DEBUG_LOG_FUNCTION)
#pragma message("DEBUG filter DEBUG_FUNCTION= '" TOSTR(DEBUG_FUNCTION) "' eg= '" __FUNCTION__ "'")
    if (symlen>=(sizeof(TOSTR(DEBUG_FILE))-1))
        {
        return(strncmp(TOSTR(DEBUG_FUNCTION),symbol,sizeof(TOSTR(DEBUG_FUNCTION))-1)==0);
        }
    else
        return 0;
#else
    (void)symbol;
    (void)symlen;
    return 1;
#endif

    }

void OWF_Debug_Print(const char* format, ...)
{    
    va_list                 ap;
    char                    __spager[512];

    va_start(ap, format);
    __spager[0] = 0;
    vsnprintf(__spager, 511, format, ap);
    xprintf("%s %s\n", OWF_DEBUG_PREFIX, __spager);
    va_end(ap);
}


void OWF_Debug_Trace(const char* fmt, ...)
{
    fmt = fmt;
}



void OWF_Debug_TraceIndent()
{
}

void OWF_Debug_TraceUndent()
{
}

void OWF_Debug_TraceEnter(const char* func)
{
    if (func)
    {
        OWF_Debug_Trace("ENTER %s", func);
    }
    OWF_Debug_TraceIndent();
}

void OWF_Debug_TraceExit(const char* func)
{
    OWF_Debug_TraceUndent();
    if (func)
    {
        OWF_Debug_Trace("EXIT %s", func);
    }
}


#ifdef __cplusplus
}
#endif

#else


#endif /* DEBUG */
