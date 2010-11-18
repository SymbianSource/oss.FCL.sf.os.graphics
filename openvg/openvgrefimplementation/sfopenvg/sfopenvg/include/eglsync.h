// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// The API provides functionality which allows waiting on the aSync object 
// to become signaled and to change status from signaled to unsignaled or vice
// versa.

/**
 @file
 @internalTechnology 
*/

#ifndef __EGL_SYNC_H_
#define __EGL_SYNC_H_

#include <e32hashtab.h>
#include <e32base.h>

#include "riMiniEGL.h"



NONSHARABLE_CLASS(CEglSync): public CBase 
{
public:
    /*-------------------------------------------------------------------*//*!
     * \brief   Factory function used to create a CEglSync instance
     * \ingroup eglSync
     * \param   aFrontLock Reference to the lock protecting the sync map
     *          aHeap Reference to the Heap to be used by EGL Sync extension
     * \param   aSync the sync id for the new sync to be created
     * \param   aDisplay the display id of the associated display
     * \param   aType the sync type
     * \return  a pointer to the extension instance if successful or NULL otherwise 
     *//*-------------------------------------------------------------------*/
    static CEglSync* Create(RFastLock& aFrontLock, 
                            EGLSyncKHR aSync, 
                            EGLDisplay aDisplay, 
                            EGLenum aType);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Public destructor
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    ~CEglSync();
    
private:
    /*-------------------------------------------------------------------*//*!
     * \brief   Private constructor
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    CEglSync(RFastLock& aFrontLock,
             EGLSyncKHR aSync, 
             EGLDisplay aDisplay, 
             EGLenum aType);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private initialisation method to be used by the factory method
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    TInt Initialize();
    
public:
    /*-------------------------------------------------------------------*//*!
     * \brief   Blocks the calling thread until the specified aSync object 
     * is signaled, or until <aTimeout> nanoseconds have passed. 
     * It is supposed that the lock protecting
     * the sync map is already acquired. The call will release this lock. 
     * \ingroup eglSync
     * \param   aFlags   If the EGL_FLUSH_COMMANDS_BIT_KHR bit is set in <aFlags>
     * and <aSync> is unsignaled when the function is called, then the equivalent
     * of Flush() will be performed for the current API context.   
     * \param   aTimeout The thread will be unblocked when <aTimeout> is expired.
     * If the <aTimeout> is to zero, the function just test the current status 
     * of the aSync object. If the <aTimeout> is set to EGL_FOREVER_KHR, then the 
     * function does not time out. For all other values, <aTimeout> is adjusted to 
     * the closest aValue which may be substantially longer than one nanosecond. 
     * \return  EGL_CONDITION_SATISFIED if <aSync> was signaled before
     * the timeout expired, which includes the case when <aSync> was already 
     * signaled when eglClientWaitSyncKHR was called; EGL_TIMEOUT_EXPIRED_KHR if the 
     * specified timeout period expired before <aSync> was signaled; 
     * EGL_FALSE if an error occurs.
     * \error  EGL_BAD_PARAMETER if <aFlags> does not equal
     * to 0 or EGL_SYNC_FLUSH_COMMAND_BIT_KHR
     *//*-------------------------------------------------------------------*/
    EGLint ClientWaitSync(EGLint aFlags, EGLTimeKHR aTimeout);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Signals or unsignals the reusable aSync object.  
     * \ingroup eglSync
     * \param   aMode    Status of the aSync object. There are two possible states: 
     *  EGL_SIGNALED_KHR and EGL_UNSIGNALED_KHR. 
     * \return  EGL_SUCCESS if an operation was successful; 
     * EGL_BAD_MATCH if the type of <aSync> is not EGL_SYNC_REUSABLE_KHR
     *//*-------------------------------------------------------------------*/
    EGLint SignalSync(EGLenum aMode);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Preapares the sync object for destruction.  
     * \ingroup eglSync
     * \return  EGL_TRUE if the object can be destroyed immediately (the sync lock is not released)
     *          EGL_FALSE if the object cannot be deleted (the sync lock is released) 
     *//*-------------------------------------------------------------------*/
    EGLBoolean DestroySyncReady();
    
    /*-------------------------------------------------------------------*//*!
    * \brief   Query an aAttribute of the aSync object  
     * \ingroup eglSync
    * \param   aDisplay     Identifier of the display which owns the aSync object
    * \param   aValue   Pointer to the aValue for the 
    *           requested aAttribute which will be filled on function return. 
    * \return  EGL_TRUE if an operation was successful and EGL_FALSE otherwise
    * \error   EGL_BAD_ATTRIBUTE if <aAttribute>  does not lie within expected range;
    * EGL_BAD_MATCH if <aAttribute> is not supported 
    * for the type of aSync object passed in <aSync>
    *//*-------------------------------------------------------------------*/
    EGLBoolean GetSyncAttrib(EGLint aAttribute, EGLint *aValue);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Returns the associated display Id  
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLDisplay Display() const { return iDisplay; }
    
    /*-------------------------------------------------------------------*//*!
    * \brief   Returns the sync type  
    * \ingroup eglSync
    *//*-------------------------------------------------------------------*/
    EGLenum Type() const { return iSyncType; }
private:
    const EGLSyncKHR  iEglSync;
    const EGLDisplay  iDisplay;
    const EGLenum     iSyncType;
    EGLenum     iSyncState;
    EGLBoolean  iDestroyed;
    EGLint      iWaiters;
    RMutex      iSyncMutex;
    RCondVar    iSyncCondVar;
    RFastLock&  iFrontLock;
    
    static const EGLint KSupportedFlags;
};


NONSHARABLE_CLASS(CEglSyncExtension): public CBase
    {
public:
    /*-------------------------------------------------------------------*//*!
     * \brief   Factory function used to create a CEglSyncExtension instance
     * \ingroup eglSync
     * \param   aEglInstance Reference to the Egl Instance implementation object
     * \return  a pointer to the extension instance if successful or NUL otherwise 
     *//*-------------------------------------------------------------------*/
    static CEglSyncExtension* Create(OpenVGRI::EGL& aEglInstance);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Public destructor
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    ~CEglSyncExtension();
private:
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private initialisation method to be used by the factory method
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLBoolean Construct();
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private constructor
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    CEglSyncExtension(OpenVGRI::EGL& aEglInstance);
    
public:    
    /*-------------------------------------------------------------------*//*!
     * \brief   Create a aSync object for the specified display.
     * \ingroup api
     * \param   aDisplay     Identifier of the display which will own the aSync object
     * \param   type    Type of the aSync object. EGL_SYNC_REUSABLE_KHR is only supported
     * \param   attrib_list Attribute-aValue list specifying attributes of the aSync 
     * object, terminated by an aAttribute entry EGL_NONE
     * \return  Handle for the created aSync object if successful, EGL_NO_SYNC_KHR otherwise
     * \error   EGL_BAD_DISPLAY if <aDisplay> is not a name of a valid EGLDisplay;
     * EGL_NOT_INITIALIZED if the display object associated 
     * with the <aDisplay> has not been initialized;
     * EGL_BAD_ATTRIBUTE if <attrib_list> is neither NULL nor empty (containing only EGL_NONE) or 
     * if <type> is not a supported type of aSync object;
     * EGL_BAD_ALLOC if the memory allocation related to aSync object is not successful
     * \note    If <type> is EGL_SYNC_REUSABLE_KHR, a reusable aSync object is created. 
     * In this case <attrib_list> must be NULL or empty (containing only EGL_NONE).
     *  *//*-------------------------------------------------------------------*/
    static EGLSyncKHR eglCreateSyncKHR(EGLDisplay aDisplay, 
                                       EGLenum aType, 
                                       const EGLint* aAttribList);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Destroy a aSync object and free memory associated with it
     * \ingroup api
     * \param   aDisplay     Identifier of the display which owns the aSync object
     * \param   aSync    Sync object handle. 
     * \return  EGL_TRUE if deletion was successful and EGL_FALSE otherwise
     * \error   EGL_BAD_DISPLAY if <aDisplay> is not a name of a valid EGLDisplay;
     * EGL_NOT_INITIALIZED if the display object associated 
     * with the <aDisplay> has not been initialized;
     * EGL_BAD_PARAMETER if <aSync> is not a valid aSync object for <aDisplay>
     * \note    If any eglClientWaitSyncKHR commands are blocking on <aSync> when 
     * eglDestroySyncKHR is called, they will be woken up, as if <aSync> were signaled. 
     * If no errors are generated, <aSync> will no longer be the handle of a valid aSync object.
     *//*-------------------------------------------------------------------*/
    static EGLBoolean eglDestroySyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Blocks the calling thread until the specified aSync object 
     * is signaled, or until <aTimeout> nanoseconds have passed.  
     * \ingroup api
     * \param   aDisplay     Identifier of the display which owns the aSync object.
     * \param   aSync    Sync object handle. 
     * \param   aFlags   If the EGL_FLUSH_COMMANDS_BIT_KHR bit is set in <aFlags>
     * and <aSync> is unsignaled when the function is called, then the equivalent
     * of Flush() will be performed for the current API context.   
     * \param   aTimeout The thread will be unblocked when <aTimeout> is expired.
     * If the <aTimeout> is to zero, the function just test the current status 
     * of the aSync object. If the <aTimeout> is set to EGL_FOREVER_KHR, then the 
     * function does not time out. For all other values, <aTimeout> is adjusted to 
     * the closest aValue which may be substantially longer than one nanosecond. 
     * \return  EGL_CONDITION_SATISFIED if <aSync> was signaled before
     * the timeout expired, which includes the case when <aSync> was already 
     * signaled when eglClientWaitSyncKHR was called; EGL_TIMEOUT_EXPIRED_KHR if the 
     * specified timeout period expired before <aSync> was signaled; 
     * EGL_FALSE if an error occurs.
     * \error   EGL_BAD_DISPLAY if <aDisplay> is not a name of a valid EGLDisplay;
     * EGL_NOT_INITIALIZED if the display object associated w
     * ith the <aDisplay> has not been initialized;
     * EGL_BAD_PARAMETER if <aSync> is not a valid aSync object for <aDisplay> or 
     * if <aFlags> does not equal to 0 or EGL_SYNC_FLUSH_COMMAND_BIT_KHR
     * Note\    More than one eglClientWaitSyncKHR may 
     * be outstanding on the same <aSync> at any given time. 
     * When there are multiple threads blocked on the same <aSync> and the aSync object is signaled, 
     * all such threads are released, but the order in which they are released is not defined.
     * If a aSync object is destroyed while an eglClientWaitSyncKHR is blocking on that object, 
     * eglClientWaitSyncKHR will unblock and return immediately, just as if the aSync object 
     * had been signaled prior to being destroyed.
     *//*-------------------------------------------------------------------*/
    static EGLint eglClientWaitSyncKHR(EGLDisplay aDisplay, 
                                       EGLSyncKHR aSync, 
                                       EGLint aFlags, 
                                       EGLTimeKHR aTimeout);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Signals or unsignals the reusable aSync object.  
     * \ingroup api
     * \param   aDisplay     Identifier of the display which owns the aSync object.
     * \param   aSync    Sync object handle. 
     * \param   aMode    Status of the aSync object. There are two possible states: 
     *  EGL_SIGNALED_KHR and EGL_UNSIGNALED_KHR. 
     * \return  EGL_TRUE if an operation was successful and EGL_FALSE otherwise.
     *//*-------------------------------------------------------------------*/
    static EGLBoolean eglSignalSyncKHR(EGLDisplay aDisplay, 
                                       EGLSyncKHR aSync, 
                                       EGLenum aMode);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Query an aAttribute of the aSync object  
     * \ingroup api
     * \param   aDisplay     Identifier of the display which owns the aSync object
     * \param   aSync    Sync object handle. 
     * \param   aAttribute   An aAttribute to be retrieved. 
     * \param   aValue   Pointer to the aValue for the requested aAttribute 
     *          which will be filled on function return. 
     * \return  EGL_TRUE if an operation was successful and EGL_FALSE otherwise
     * \error   EGL_BAD_DISPLAY if <aDisplay> is not a name of a valid EGLDisplay;
     * EGL_NOT_INITIALIZED if the display object associated 
     * with the <aDisplay> has not been initialized;
     * EGL_BAD_PARAMETER if <aSync> is not a valid aSync object for <aDisplay> or if <aValue> is not 
     * a valid pointer; EGL_BAD_ATTRIBUTE if <aAttribute>  does not lie within expected range;
     * EGL_BAD_MATCH if <aAttribute> is not supported for the type of aSync object passed in <aSync>
     *//*-------------------------------------------------------------------*/
    static EGLBoolean eglGetSyncAttribKHR(EGLDisplay aDisplay, 
                                          EGLSyncKHR aSync, 
                                          EGLint aAttribute, 
                                          EGLint* aValue);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Signals or unsignals the reusable aSync object.  
     * \ingroup api
     * \param   aDisplay     Identifier of the display which owns the aSync object.
     * \param   aSync    Sync object handle. 
     * \param   aSync    Status of the aSync object. There are two possible states: 
     *  EGL_SIGNALED_KHR and EGL_UNSIGNALED_KHR. 
     * \return  EGL_SUCCESS if an operation was successful; 
     * EGL_BAD_DISPLAY if <aDisplay> is not a name of a valid EGLDisplay; 
     * EGL_NOT_INITIALIZED if the display object associated with the <aDisplay> has not 
     * been initialized; EGL_BAD_PARAMETER if <aSync> is not a valid aSync object for <aDisplay> 
     * or if <aSync> does not lie within expected range;
     * EGL_BAD_MATCH if the type of <aSync> is not EGL_SYNC_REUSABLE_KHR
     * \note    This function does not generate an error.
     *//*-------------------------------------------------------------------*/
    static EGLint egl_Private_SignalSyncNOK(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode);
    
public:
    /*-------------------------------------------------------------------*//*!
     * \brief   Initialisation request when a new display is created  
     * \ingroup eglSync
     * \param  aEglDisplay a display identifier 
     * \return  EGL_SUCCESS if successful; 
     *//*-------------------------------------------------------------------*/
    IMPORT_C TInt EglSyncDisplayCreate(EGLDisplay aDisplay);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Request to destroy all syncs associated with a display as preamble
     *          of destroying the specified display  
     * \ingroup eglSync
     * \param  aEglDisplay a display identifier 
     *//*-------------------------------------------------------------------*/
    void EglSyncDisplayDestroy(EGLDisplay aDisplay);
    
private:
    /*-------------------------------------------------------------------*//*!
     * \brief   Query and request to lock a specified display   
     * \ingroup eglSync
     * \param   aEglDisplay a display identifier 
     * \return  EGL_SUCCESS if successful;
     *          EGL_BAD_DISPLAY is not a name of a valid EGLDisplay
     *          EGL_NOT_INITIALIZED if the display object associated 
     *          with the <aEglDisplay> has not been initialized 
     *//*-------------------------------------------------------------------*/
    EGLint FindAndLockDisplay(EGLDisplay aDisplay);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Releases the lock associated with a valid EGLDisplay   
     * \ingroup eglSync
     * \param   aEglDisplay a display identifier 
     *//*-------------------------------------------------------------------*/
    void ReleaseDisplayLock(EGLDisplay aDisplay);
    
private:
    /*-------------------------------------------------------------------*//*!
     * \brief   Private implementation for <eglCreateSyncKHR> public static interface   
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLSyncKHR EglCreateSyncKHR(EGLDisplay aDisplay, EGLenum aType, const EGLint* aAttribList);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private implementation for <eglDestroySyncKHR> public static interface   
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLBoolean EglDestroySyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private implementation for <eglClientWaitSyncKHR> public static interface   
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLint EglClientWaitSyncKHR(EGLDisplay aDisplay, 
                                EGLSyncKHR aSync, 
                                EGLint aFlags, 
                                EGLTimeKHR aTimeout);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private implementation for <eglSignalSyncKHR> public static interface
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLBoolean EglSignalSyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private implementation for <eglGetSyncAttribKHR> public static interface
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLBoolean EglGetSyncAttribKHR(EGLDisplay aDisplay, 
                                   EGLSyncKHR aSync, 
                                   EGLint aAttribute, 
                                   EGLint* aValue);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Private implementation for <egl_Private_SignalSyncNOK> public static interface
     * \ingroup eglSync
     *//*-------------------------------------------------------------------*/
    EGLint Egl_Private_SignalSyncNOK(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode);
    
private:
    /*-------------------------------------------------------------------*//*!
     * \brief   Creates a sync and inserts it in the sync map   
     * \ingroup eglSync
     * \param   aEglDisplay the id of the associated display 
     * \param   aType The sync type 
     * \return  the id of the sync created
     *//*-------------------------------------------------------------------*/
    EGLSyncKHR CreateSync(EGLDisplay aDisplay, EGLenum aType);
    
    /*-------------------------------------------------------------------*//*!
     * \brief   Destroy a sync associated with a display   
     * \ingroup eglSync
     * \param   aEglDisplay the id of the associated display 
     *//*-------------------------------------------------------------------*/
    void EglDestroySync(EGLDisplay aDisplay);
    
private:
    typedef RHashMap<EGLint,CEglSync*> REglSyncHashMap;
    
    REglSyncHashMap iEglSyncMap;
    RFastLock       iEglSyncMapLock;
    EGLint          iEglSyncId;
    };

#endif /* __EGL_SYNC_H_ */
