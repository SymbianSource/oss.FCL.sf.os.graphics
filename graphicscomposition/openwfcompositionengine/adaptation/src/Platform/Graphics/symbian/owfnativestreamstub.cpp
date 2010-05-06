// Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Native Stream Stubs
// 
//

#include <WF/wfc.h>
#include <graphics/symbianstream.h>
#include <graphics/surfacemanager.h>
#include "owfnativestream.h"

#ifdef __cplusplus
extern "C" {
#endif

static RSurfaceManager& SurfaceManagerSingleton()
        {
        static RSurfaceManager surfaceManager;
        static int err=surfaceManager.Open();
        return surfaceManager;
        }

static khronos_int32_t max(khronos_int32_t aLhs, khronos_int32_t aRhs)
{
    if (aLhs > aRhs) {
        return aLhs;
    }
    return aRhs;
}



static TSurfaceId createSurface(khronos_int32_t width,
        khronos_int32_t height,
        const OWF_IMAGE_FORMAT*   format,
        khronos_int32_t nbufs)
        {
        TSurfaceId surface = TSurfaceId::CreateNullId();
        RSurfaceManager::TSurfaceCreationAttributesBuf bf;
        RSurfaceManager::TSurfaceCreationAttributes& b = bf();
    
        TBool premultiplied = format->premultiplied;
        OWF_PIXEL_FORMAT pixelFormat = format->pixelFormat;
        khronos_int32_t bytesPerPixel = 0;
// we don't support linear mode
        if (format->linear)
            {
            return surface;
            }
        switch(pixelFormat)
            {
            case OWF_IMAGE_RGB565:
                b.iPixelFormat = EUidPixelFormatRGB_565;
                bytesPerPixel = 2;
                b.iAlignment = max(4,format->rowPadding);
                break;
            case OWF_IMAGE_ARGB8888:
                {
                if (premultiplied)
                    {
                    b.iPixelFormat = EUidPixelFormatARGB_8888_PRE;
                    }
                else
                    {
                    b.iPixelFormat = EUidPixelFormatARGB_8888;
                    }
                bytesPerPixel = 4;
                b.iAlignment = format->rowPadding;
                break;
                }
            case OWF_IMAGE_XRGB8888 :
                b.iPixelFormat = EUidPixelFormatXRGB_8888;
                bytesPerPixel = 4;
                b.iAlignment = max(4,format->rowPadding);
                break;
            case OWF_IMAGE_L8 :
                b.iPixelFormat = EUidPixelFormatA_8;
                bytesPerPixel = 1;
                b.iAlignment = max(4,format->rowPadding);
                break;
            case OWF_IMAGE_L1 :
                b.iPixelFormat = EUidPixelFormatL_1;
                bytesPerPixel = -8;
                b.iAlignment = max(4,format->rowPadding);
                break;
            case OWF_IMAGE_UYVY:
                b.iPixelFormat = EUidPixelFormatYUV_422Interleaved;
                bytesPerPixel = 1;
                b.iAlignment = max(4,format->rowPadding);
                break;
            default:
                return surface;
            }   
        
        b.iSize.iWidth = width;
        b.iSize.iHeight = height;
        b.iBuffers = nbufs;                             // number of buffers in the surface
        b.iOffsetToFirstBuffer = 0;                     // way of reserving space before the surface pixel data
        if (bytesPerPixel >= 0)
            {
            b.iStride = bytesPerPixel * width;  // number of bytes between start of one line and start of next  
            }
        else
            {
            b.iStride = (width-(bytesPerPixel+1)) / (-bytesPerPixel);
            }
        b.iContiguous = OWF_FALSE;
        b.iMappable = OWF_TRUE;
        
        TInt err=SurfaceManagerSingleton().CreateSurface(bf, surface);
        if (err<0)
            {
            surface=TSurfaceId::CreateNullId();
            }
        return surface;
        }

static void WipeWriteBuffer(SymbianStreamType ns)
        {
        khronos_int32_t height,stride;
        SymbianStreamGetHeader(ns,NULL,&height,&stride,NULL,NULL);
        SymbianStreamBuffer buffer;
        if( KErrNone != SymbianStreamAcquireWriteBuffer(ns, &buffer))
            {
            return;
            }
        void* pixels = NULL;
        // If we fail to get buffer pointer, we proceed without initial wipe
        if( KErrNone != SymbianStreamGetBufferPointer(ns,buffer,&pixels))
            {
            return;
            }
        memset(pixels,0,height*stride);
        SymbianStreamReleaseWriteBuffer(ns,buffer);
        }

/*!---------------------------------------------------------------------------
 *  Create new off-screen image stream.
 *
 *  \param width            Stream image buffer width
 *  \param height           Stream image buffer height
 *  \param imageFormat      Stream image buffer format
 *  \param nbufs            Number of image buffers to allocate
 *
 *  \param Handle to newly created stream or OWF_INVALID_HANDLe if no
 *  stream could be created.
 *----------------------------------------------------------------------------*/
OWF_API_CALL OWFNativeStreamType
owfNativeStreamCreateImageStream(OWFint width,
                                 OWFint height,
                                 const OWF_IMAGE_FORMAT* format,
                                 OWFint nbufs)
        {
        TSurfaceId surface;
        
        surface = createSurface(width, height, format, nbufs);
        if (surface.IsNull())
            {
            return WFC_INVALID_HANDLE;
            }
        SymbianStreamType ns=NULL;  //No more error checking required...
        SymbianStreamAcquire(&surface,&ns);
        
        SurfaceManagerSingleton().CloseSurface(surface);
        if (ns)
            {
            WipeWriteBuffer(ns);
            }
        return (OWFNativeStreamType)ns;
        }
/*!---------------------------------------------------------------------------
 * Converts from external WFC native stream handle type to internal OWF native stream handle type.
 * The internal handle MUST be persistant. The external handle may already be persistant.
 * This method may fail, either due to memory, or due to the stream object not being supported by the compositor
 * @param publicStream The publicly defined stream handle
 * @param error			Pointer to store error code - optionally can be NULL
 * @return OWFNativeStreamType an equivalent internal stream handle
 *----------------------------------------------------------------------------**/
OWF_API_CALL OWFNativeStreamType owfNativeStreamFromWFC(WFCNativeStreamType publicStream,OWF_STREAM_ERROR* errorReturn)
	{
	SymbianStreamType rvss;
	TInt err = SymbianStreamAcquire(reinterpret_cast<SymbianStreamType>(publicStream),&rvss);
	if (err)
		{
		if (errorReturn)
		    {
		    *errorReturn = (err == KErrNoMemory) ? OWF_STREAM_ERROR_OUT_OF_MEMORY : OWF_STREAM_ERROR_INVALID_STREAM;
		    }
		return OWF_INVALID_HANDLE;
		}
	OWF_IMAGE_FORMAT format;
	OWFNativeStreamType rvowf=reinterpret_cast<OWFNativeStreamType>(rvss);
	owfNativeStreamGetHeader(rvowf,NULL,NULL,NULL,&format,NULL);
	if (format.pixelFormat==OWF_IMAGE_NOT_SUPPORTED)
		{
		SymbianStreamRemoveReference(rvss);
		if (errorReturn)
			{
			*errorReturn=OWF_STREAM_ERROR_INVALID_STREAM;
			}
		return OWF_INVALID_HANDLE; 
		}
	if (errorReturn)
		{
		*errorReturn=OWF_STREAM_ERROR_NONE;
		}
	return rvowf;
	}



/*!---------------------------------------------------------------------------
 *  Increase stream's reference count
 *
 *  \param stream           Stream handle
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamAddReference(OWFNativeStreamType stream)
        {
        SymbianStreamAddReference((SymbianStreamType)stream);
        }

/*!---------------------------------------------------------------------------
 *  Decrease stream's reference count
 *
 *  \param stream           Stream handle
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamRemoveReference(OWFNativeStreamType stream)
        {
        SymbianStreamRemoveReference((SymbianStreamType)stream);
        }

/*!----------------------------------------------------------------------------
 *  Destroy stream. The stream isn't necessarily immediately destroyed, but
 *  only when it's reference count reaches zero.
 *
 *  \param stream           Stream handle
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamDestroy(OWFNativeStreamType stream)
        {
        SymbianStreamRemoveReference((SymbianStreamType)stream);
        }

static TInt PixelFormatConversion(TUidPixelFormat aBaseFormat, OWF_IMAGE_FORMAT& aFormat)
        {
        // Non-alpha formats have the premultiplied flag set because the
        // code handles them more quickly that way.
        switch(aBaseFormat)
            {
            case EUidPixelFormatRGB_565:
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_TRUE;
                aFormat.rowPadding = 2;
                aFormat.pixelFormat = OWF_IMAGE_RGB565;
                break;
            case EUidPixelFormatARGB_8888_PRE:
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_TRUE;
                aFormat.rowPadding = 4;
                aFormat.pixelFormat = OWF_IMAGE_ARGB8888;
                break;
            case EUidPixelFormatARGB_8888:
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_FALSE;
                aFormat.rowPadding = 4;
                aFormat.pixelFormat = OWF_IMAGE_ARGB8888;
                break;
            case EUidPixelFormatXRGB_8888 :
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_TRUE;
                aFormat.rowPadding = 4;
                aFormat.pixelFormat = OWF_IMAGE_XRGB8888;
                break;
            case EUidPixelFormatA_8 :
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_FALSE;
                aFormat.rowPadding = 4;
                aFormat.pixelFormat = OWF_IMAGE_L8;          
                break;
            case EUidPixelFormatL_1 :
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_FALSE;
                aFormat.rowPadding = 4;
                aFormat.pixelFormat = OWF_IMAGE_L1;          
                break;

            case EUidPixelFormatYUV_422Interleaved :
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_FALSE;
                aFormat.rowPadding = 1;
                aFormat.pixelFormat = OWF_IMAGE_UYVY;          
                break;

            default:
                aFormat.linear = OWF_FALSE;
                aFormat.premultiplied = OWF_FALSE;
                aFormat.rowPadding = 0;
                aFormat.pixelFormat = OWF_IMAGE_NOT_SUPPORTED;
                return KErrNotSupported;
            }
        return KErrNone;
        }

/*!---------------------------------------------------------------------------
 * Get stream's image header
 *
 * \param stream            Stream handle
 * \param width             Stream width
 * \param height            Stream height
 * \param stride            Stream stride
 * \param format            Stream format
 * \param pixelSize         Stream pixelSize
 *
 * All the parameters above, except stream handle, are pointers to locations
 * where the particular value should be written to. Passing in a NULL
 * pointer means that the particular values is of no interest to the caller.
 *
 * E.g. to query only width & height one would call this function with
 * parameters (stream_handle, &width, &height, NULL, NULL, NULL);
 *
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamGetHeader(OWFNativeStreamType stream,
                           OWFint* width,
                           OWFint* height,
                           OWFint* stride,
                           OWF_IMAGE_FORMAT* format,
                           OWFint* pixelSize)
        {
        SymOwfPixelFormat symFormat;
        SymbianStreamGetHeader((SymbianStreamType)stream,
                                width,
                                height,
                                stride,
                                format?&symFormat:(SymOwfPixelFormat*)NULL,
                                pixelSize);
        if (format)
            {   //translate format. If error then OWF_IMAGE_NOT_SUPPORTED is set.
            PixelFormatConversion(symFormat,*format);
            }
        }

/*!---------------------------------------------------------------------------
 *  Acquire read buffer from stream
 *
 *  \param stream           Stream handle
 *
 *  \return Handle to next readable (unread since last write)
 *  buffer from the stream or OWF_INVALID_HANDLE if no unread buffers
 *  are available.
 *----------------------------------------------------------------------------*/
OWF_API_CALL OWFNativeStreamBuffer
owfNativeStreamAcquireReadBuffer(OWFNativeStreamType stream)
        {
        OWFNativeStreamBuffer returnBuffer;
        SymbianStreamAcquireReadBuffer((SymbianStreamType)stream,(SymbianStreamBuffer*)&returnBuffer);
        return returnBuffer;
        }

/*!---------------------------------------------------------------------------
 *  Release read buffer.
 *
 *  \param stream           Stream handle
 *  \param buf              Buffer handle
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamReleaseReadBuffer(OWFNativeStreamType stream,
                                 OWFNativeStreamBuffer buf)
        {
        SymbianStreamReleaseReadBuffer((SymbianStreamType)stream,(SymbianStreamBuffer)buf);
        }

/*!---------------------------------------------------------------------------
 *  Acquires writable buffer from a stream. The caller has exclusive access
 *  to returned buffer until the buffer is commited to stream by
 *  calling ReleaseWriteBuffer.
 *
 *  \param stream           Stream handle
 *
 *  \return Handle to next writable buffer or OWF_INVALID_HANDLE if no such
 *  buffer is available.
 *----------------------------------------------------------------------------*/
OWF_API_CALL OWFNativeStreamBuffer
owfNativeStreamAcquireWriteBuffer(OWFNativeStreamType stream)
        {
        OWFNativeStreamBuffer returnBuffer;
        SymbianStreamAcquireWriteBuffer((SymbianStreamType)stream,(SymbianStreamBuffer*)&returnBuffer);
        return returnBuffer;
        }

/*!---------------------------------------------------------------------------
 *  Commit write buffer to stream.
 *
 *  \param stream           Stream handle
 *  \param buf              Buffer handle
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamReleaseWriteBuffer(OWFNativeStreamType stream,
                                  OWFNativeStreamBuffer buf,
                                  EGLDisplay dpy,
                                  EGLSyncKHR sync)
        {
        //We do not use the sync object mechanism to indicate that content has been consumed.
        //See the SUS notification support
        (void)dpy;
        (void)sync;
        SymbianStreamReleaseWriteBuffer((SymbianStreamType)stream,(SymbianStreamBuffer)buf);
        }

/*!---------------------------------------------------------------------------
 *  Register stream content observer (append to chain). The observer will
 *  receive buffer modification event from the stream whenever a buffer is
 *  committed.
 *
 *  \param stream           Stream handle
 *  \param observer         Stream observer
 *  \param data             data to pass to observer callback
 *                          function when event is dispatched.
 *----------------------------------------------------------------------------*/
OWF_API_CALL OWF_STREAM_ERROR
owfNativeStreamAddObserver(OWFNativeStreamType stream,
                           OWFStreamCallback observer,
                           void* data)
        {
        int err = SymbianStreamAddObserver((SymbianStreamType)stream,(SymbianStreamCallback)observer,data);
        switch (err)
            {
            case KErrBadHandle:    return OWF_STREAM_ERROR_INVALID_STREAM;
            case KErrNotFound:     return OWF_STREAM_ERROR_INVALID_OBSERVER;
            case KErrGeneral:      return OWF_STREAM_ERROR_OUT_OF_MEMORY;
            default:               return OWF_STREAM_ERROR_NONE;
            }
        }

/*!---------------------------------------------------------------------------
 *  Remove stream content observer.
 *
 *  \param stream           Stream handle
 *  \param observer         Observer to remove
 *  \param data             client data supplied for registration
 *
 *  \param Zero if the observer was removed successfully, otherwise non-zero
 *  (OWF_STREAM_ERROR_INVALID_STREAM if the stream is invalid;
 *   OWF_STREAM_ERROR_INVALID_OBSERVER if the observer is invalid.)
 *----------------------------------------------------------------------------*/
OWF_API_CALL OWF_STREAM_ERROR
owfNativeStreamRemoveObserver(OWFNativeStreamType stream,
                              OWFStreamCallback observer,
                              void* data)
        {
        SYMOWF_DEFAULT_EVENT_PARAM eventPar;
        eventPar.length = sizeof(eventPar);
        eventPar.event = ESOWF_NoEvent;
        observer(SYMBIAN_INVALID_HANDLE, ESOWF_ObserverReturnDefaultEvent, NULL, &eventPar);
        return (OWF_STREAM_ERROR)SymbianStreamRemoveObserver((SymbianStreamType)stream,data,(type_SymbianStreamEventBits)eventPar.event);
        }

/*!---------------------------------------------------------------------------
 *  Enable/disable stream content notifications.
 *
 *  \param stream           Stream handle
 *  \param send             Boolean value indicating whether the stream should
 *                          send content notifications to its observers.
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamEnableUpdateNotifications(OWFNativeStreamType /*stream*/,
                                         OWFboolean /*send*/)
    {
    //SymbianStream always enables update notifications.
    //It can deal efficiently with the situation where there are no registered observers.
    }



/*!---------------------------------------------------------------------------
 *  Return pointer to stream buffer's pixel data. The buffer must be
 *  a valid read/write buffer.
 *
 *  \param stream           Stream handle
 *  \param buffer           Buffer handle
 *
 *  \return Pointer to buffers pixel data.
 *----------------------------------------------------------------------------*/
OWF_API_CALL void*
owfNativeStreamGetBufferPtr(OWFNativeStreamType stream,
                            OWFNativeStreamBuffer buffer)
        {
        void* returnPointer = NULL;
        SymbianStreamGetBufferPointer((SymbianStreamType)stream,buffer,&returnPointer);
        return returnPointer;
        }

/*!---------------------------------------------------------------------------
 *  Set/reset stream's protection flag. This flag is used for preventing the
 *  user from deleting a stream that s/he doesn't really own or should
 *  not twiddle with too much (on-screen context's target stream, for example)
 *
 *  \param stream           Stream handle
 *  \param flag             Protection status
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfNativeStreamSetProtectionFlag(OWFNativeStreamType stream,
                                 OWFboolean flag)
        {
        SymbianStreamSetProtectionFlag((SymbianStreamType)stream,flag);
        }

/*!---------------------------------------------------------------------------
 *  Set/reset stream's flip state. When the flip state is OWF_TRUE, the stream
 *  attributes returned by owfNativeStreamGetHeader are affected as follows:
 *  -# The width and height attributes are swapped.
 *  -# The stride attribute is derived from the height value instead of the
 *  width.
 *
 *  Note: the attributes of the underlying surface as retrieved using Surface
 *  Manager are not affected by this flag.
 *
 *  \param stream           Stream handle
 *  \param flip             New flip state
 *----------------------------------------------------------------------------*/
OWF_API_CALL void
owfSetStreamFlipState(OWFNativeStreamType stream, OWFboolean flip)
    {
    SymbianStreamSetFlipState((SymbianStreamType)stream, flip);
    }

#ifdef __cplusplus
}
#endif


