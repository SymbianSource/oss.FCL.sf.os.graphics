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
// EGL sync implementation

#include "eglapi.h"
#include <e32std.h>
#include <e32base.h>
#include <e32cmn.h>
#include <eglext.h>
#include "egltypes.h"

#include "eglsync.h"



const EGLint CEglSync::KSupportedFlags = EGL_SYNC_FLUSH_COMMANDS_BIT_KHR;

// ToDo get rid of these macros
//#define EGL_SET_ERROR(e)                    { Instance().SetError(e); }
//#define EGL_SET_ERROR(e)					{setEglError(e);}  
#define EGL_SET_ERROR(e)
#define TEST_AND_RET(c, r, t)               { if (!(c)) { EGL_TRACE(t); return (r); } }
#define TEST_CLEANUP_RET(c, r, clnp, t)     { if (!(c)) { clnp; EGL_TRACE(t); return (r); } }
#define CHECK_AND_RET(c, e, r, t)           { if (!(c)) { EGL_SET_ERROR(e); EGL_TRACE(t); return (r); } }
#define CHECK_CLEANUP_RET(c, e, r, clnp, t) { if (!(c)) { EGL_SET_ERROR(e); clnp; EGL_TRACE(t); return (r); } }
#define RESET_EGL_ERROR()                   { EGL_SET_ERROR(EGL_SUCCESS); }

// ToDo use EGL Panics & panic messages
void SyncPanic(TInt aPanic)
    {
    _LIT(KPanic, "EGL SYNC");
    User::Panic(KPanic, aPanic);
    }



CEglSync* CEglSync::Create(RFastLock& aFrontLock, EGLSyncKHR aSync, EGLDisplay aDisplay, EGLenum aType)
    {
    CEglSync* sync= new CEglSync(aFrontLock, aSync, aDisplay, aType);
    TEST_AND_RET(sync != NULL, sync, "CEglSync::Create couldn't create the object");
    if (KErrNone == sync->Initialize())
        {
        EGL_TRACE("CEglSync::Create OK aDisplay=%d aSync=%d aSyncType=0x%x", aDisplay, aSync, aType);
        return sync;
        }
    else
        {
        EGL_TRACE("CEglSync::Create FAILED aDisplay=%d aSync=%d aSyncType=0x%x", aDisplay, aSync, aType);
        delete sync;
        return NULL;
        }
    }

TInt CEglSync::Initialize()
    {
    TInt err = KErrNone;
    err = iSyncMutex.CreateLocal();
    TEST_AND_RET(err == KErrNone, err, "CEglSync::Initialize failed to create mutex");
    err = iSyncCondVar.CreateLocal();
    TEST_AND_RET(err == KErrNone, err, "CEglSync::Initialize failed to create cond var");
    return err;
    }


CEglSync::CEglSync(RFastLock& aFrontLock, EGLSyncKHR aSync, EGLDisplay aDisplay, EGLenum aType):
iEglSync(aSync),
iDisplay(aDisplay),
iSyncType(aType),
iSyncState(EGL_UNSIGNALED_KHR),
iDestroyed(EGL_FALSE),
iWaiters(0),
iFrontLock(aFrontLock)
    {

    }

CEglSync::~CEglSync()
    {
    iSyncMutex.Close();
    iSyncCondVar.Close();
    EGL_TRACE("CEglSync::~CEglSync iDisplay=%d iEglSync=%d", iDisplay, iEglSync);
    }

EGLBoolean CEglSync::DestroySyncReady()
    {
    EGL_TRACE("CEglSync::DestroySync iDisplay=%d iEglSync=%d iWaiters=%d", iDisplay, iEglSync, iWaiters);
    iSyncMutex.Wait();
    if (iWaiters == 0)
        {
        EGL_TRACE("CEglSync::DestroySync READY TO DESTROY iDisplay=%d iEglSync=%d iWaiters=%d", iDisplay, iEglSync, iWaiters);
        iDestroyed = EGL_TRUE;
        return EGL_TRUE;
        }
    else if (!iDestroyed)
        {
        EGL_TRACE("CEglSync::DestroySync BROADCAST iDisplay=%d iEglSync=%d iWaiters=%d", iDisplay, iEglSync, iWaiters);
        iSyncCondVar.Broadcast();
        }
    iDestroyed = EGL_TRUE;
    iSyncMutex.Signal();
    return EGL_FALSE;
    }


EGLint CEglSync::SignalSync(EGLenum aMode)
    {
    EGL_TRACE("CEglSync::SignalSync aMode=%d", aMode);

    iSyncMutex.Wait();
    if (aMode != iSyncState)
        {
        iSyncState = aMode;
        if (aMode == EGL_SIGNALED_KHR)
            {
            iSyncCondVar.Broadcast();
            }
        }

    iSyncMutex.Signal();
    return EGL_SUCCESS;
    }

EGLint CEglSync::ClientWaitSync(EGLint aFlags, EGLTimeKHR aTimeout)
    {
    EGL_TRACE("CEglSync::ClientWaitSync aFlags=%d aTimeout=%d", aFlags, aTimeout);

    CHECK_CLEANUP_RET(!(aFlags & (~KSupportedFlags)),
                  EGL_BAD_PARAMETER,
                  EGL_FALSE,
                  iFrontLock.Signal();,
                  "CEglSync::ClientWaitSync bad flags parameter");

    iSyncMutex.Wait();
    iFrontLock.Signal();

    CHECK_CLEANUP_RET(!iDestroyed ,
                      EGL_BAD_PARAMETER,
                      EGL_FALSE,
                      iSyncMutex.Signal();,
                      "CEglSync::ClientWaitSync sync already destroyed");

    EGLint ret = (EGLint) EGL_FALSE;
    if (iSyncState == EGL_UNSIGNALED_KHR)
        {
        if (aTimeout && (aFlags & EGL_SYNC_FLUSH_COMMANDS_BIT_KHR) && (eglGetCurrentContext() != EGL_NO_CONTEXT))
            {
            switch(eglQueryAPI())
                {
                case EGL_OPENGL_ES_API:
                    //ToDo: make sure these cases are covered
                    //glFlush();
                    break;
                case EGL_OPENVG_API:
                    //ToDo: make sure these cases are covered
                    //vgFlush();
                    break;
                default:
                	SyncPanic(__LINE__);
                }
            }

        switch (aTimeout)
            {
            case 0:
                ret = EGL_TIMEOUT_EXPIRED_KHR;
                break;

            case EGL_FOREVER_KHR:
                {
                EGLint res = KErrNone;
                ++iWaiters;
                EGL_TRACE("CEglSync::ClientWaitSync iSyncCondVar.Wait");
                res = iSyncCondVar.Wait(iSyncMutex);
                --iWaiters;
                if (res == KErrNone)
                    {
                    ret = EGL_CONDITION_SATISFIED_KHR;
                    }
                else
                    {
                    ret = EGL_FALSE;
                    }
                break;
                }

            default:
                {
                    /*
                     * Since the supported range of timeout at function RCondVar::TimedWait(mutex, timeout)
                     * is 0 to KMaxTInt, looping mechanism below is used to support 64bit timeout.
                     */
                    EGLint res = KErrTimedOut;
                    EGL_TRACE("CEglSync::ClientWaitSync iSyncCondVar.TimedWait");
                    for(TInt64 timeoutMicroseconds = aTimeout/1000; (res == KErrTimedOut) && (timeoutMicroseconds > 0); timeoutMicroseconds -= KMaxTInt)
                        {
                        ++iWaiters;
                        res = iSyncCondVar.TimedWait(iSyncMutex, (timeoutMicroseconds > KMaxTInt) ? KMaxTInt:timeoutMicroseconds);
                        --iWaiters;
                        }
                    switch (res)
                        {
                        case KErrNone:
                            ret = EGL_CONDITION_SATISFIED_KHR;
                            break;
                        case KErrTimedOut:
                            ret = EGL_TIMEOUT_EXPIRED_KHR;
                            break;
                        default:
                            ret = EGL_FALSE;
                            break;
                        }
                    break;
                }
            }
        }
    else
        {
            ret = EGL_CONDITION_SATISFIED_KHR;
            EGL_TRACE("CEglSync::ClientWaitSync sync in signalled state");
        }
    if (iDestroyed && (iWaiters == 0))
        {
        EGL_TRACE("CEglSync::ClientWaitSync sync autodestroyed");
        delete this;
        }
    else
        {
        EGL_TRACE("CEglSync::ClientWaitSync release sync mutex");
        iSyncMutex.Signal();
        }
    EGL_TRACE("CEglSync::ClientWaitSync sync exit ret=0x%x", ret);
    return ret;
    }

EGLBoolean CEglSync::GetSyncAttrib(EGLint aAttribute, EGLint *aValue)
    {
    EGL_TRACE("CEglSync::GetSyncAttrib aAttribute=0x%x aValue", aValue);

    CHECK_AND_RET(aValue,
                  EGL_BAD_PARAMETER,
                  EGL_FALSE,
                  "CEglSync::GetSyncAttrib undefined parameter aValue is NUL");

    CHECK_AND_RET((aAttribute == EGL_SYNC_TYPE_KHR) || (aAttribute == EGL_SYNC_STATUS_KHR),
                  EGL_BAD_ATTRIBUTE,
                  EGL_FALSE,
                  "CEglSync::GetSyncAttrib unnexpected attribute parameter");


    iSyncMutex.Wait();
    EGLBoolean ret = EGL_FALSE;
    switch(aAttribute)
        {
        case EGL_SYNC_TYPE_KHR:
            {
            *aValue = iSyncType;
            ret = EGL_TRUE;
            break;
            }
        case EGL_SYNC_STATUS_KHR:
            {
            *aValue = iSyncState;
            ret = EGL_TRUE;
            break;
            }
        default:
            {
            EGL_SET_ERROR(EGL_BAD_ATTRIBUTE);
            }
        }
    iSyncMutex.Signal();
    return ret;
    }


CEglSyncExtension* CEglSyncExtension::Create(OpenVGRI::EGL& aEglInstance)
    {
    EGL_TRACE("CEglSyncExtension::Create");
    CEglSyncExtension* eglSyncExt= new CEglSyncExtension(aEglInstance);
    if (eglSyncExt && eglSyncExt->Construct())
        {
        return eglSyncExt;
        }

    delete eglSyncExt;
    return NULL;
    }

EGLBoolean CEglSyncExtension::Construct()
    {
    if  (iEglSyncMapLock.CreateLocal() == KErrNone)
        {
        EGL_TRACE("CEglSyncExtension::Construct OK");
        return EGL_TRUE;
        }

    EGL_TRACE("CEglSyncExtension::Construct FAILED");
    return EGL_FALSE;
    }

CEglSyncExtension::CEglSyncExtension(OpenVGRI::EGL& aEglInstance):
iEglSyncMap(),
iEglSyncId(0)/*,
iEglInstance(aEglInstance)*/
    {

    }

CEglSyncExtension::~CEglSyncExtension()
    {
    if (iEglSyncMapLock.Handle())
        {
        iEglSyncMapLock.Wait();
        }

    EglDestroySync(EGL_NO_DISPLAY);

    iEglSyncMap.Close();
    iEglSyncMapLock.Close();
    }

EXPORT_C TInt CEglSyncExtension::EglSyncDisplayCreate(EGLDisplay aDisplay)
    {
        return KErrNone;
    }

void CEglSyncExtension::EglDestroySync(EGLDisplay aDisplay)
	{
	EGL_TRACE("CEglSyncExtension::EglDestroySync");
	REglSyncHashMap::TIter iter(iEglSyncMap);

	while (iter.NextKey())
		{
		CEglSync* const* syncpp = NULL;
		syncpp = iter.CurrentValue();
		if (syncpp && *syncpp && ((aDisplay == EGL_NO_DISPLAY) || (*syncpp)->Display() == aDisplay))
			{
			const EGLint* key = iter.CurrentKey();
			EGLPANIC_ASSERT(key, EEglPanicTemp);
			if (aDisplay != EGL_NO_DISPLAY)
				{
				// we expect that the map is to be closed after removing the elements
				// therefore we can skip to remove the syncs from map
				if (KErrNone != iEglSyncMap.Remove(*key))
					{
					SyncPanic(__LINE__);
					}
				}
			if ((*syncpp)->DestroySyncReady())
				{
				delete *syncpp;
				}
			}
		}
	}

void CEglSyncExtension::EglSyncDisplayDestroy(EGLDisplay aDisplay)
    {
    iEglSyncMapLock.Wait();
    EglDestroySync(aDisplay);
    iEglSyncMapLock.Signal();
    }

EGLSyncKHR CEglSyncExtension::EglCreateSyncKHR(EGLDisplay aDisplay, EGLenum aType, const EGLint* aAttribList)
    {
    EGL_TRACE("CEglSyncExtension::EglCreateSyncKHR aDisplay=%d, aType=%d", aDisplay, aType);
    RESET_EGL_ERROR();

    CHECK_AND_RET(aType == EGL_SYNC_REUSABLE_KHR,
                  EGL_BAD_ATTRIBUTE,
                  EGL_NO_SYNC_KHR,
                  "CEglSyncExtension::EglCreateSyncKHR unsupported sync type");

    CHECK_AND_RET((aAttribList == NULL) || (*aAttribList == EGL_NONE),
                  EGL_BAD_ATTRIBUTE,
                  EGL_NO_SYNC_KHR,
                  "CEglSyncExtension::EglCreateSyncKHR invalid attribute list");

    EGLint lockErr = FindAndLockDisplay(aDisplay);

    CHECK_AND_RET(lockErr == EGL_SUCCESS,
                      lockErr,
                      EGL_NO_SYNC_KHR,
                      "CEglSyncExtension::EglCreateSyncKHR couldn't find and lock display");



    iEglSyncMapLock.Wait();
    ReleaseDisplayLock(aDisplay);

    EGLSyncKHR dispRet = CreateSync(aDisplay, aType);

    iEglSyncMapLock.Signal();

    EGL_TRACE("CEglSyncExtension::EglCreateSyncKHR EXIT aDisplay=%d, sync=%d", aDisplay, dispRet);
    return dispRet;
    }

EGLBoolean CEglSyncExtension::EglDestroySyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync)
    {
    EGL_TRACE("CEglSyncExtension::EglDestroySyncKHR aDisplay=%d, aSync=%d", aDisplay, aSync);

    RESET_EGL_ERROR();

    EGLint lockErr = FindAndLockDisplay(aDisplay);

    CHECK_AND_RET(lockErr == EGL_SUCCESS,
                      lockErr,
                      EGL_FALSE,
                      "CEglSyncExtension::EglDestroySyncKHR couldn't find and lock display");


    iEglSyncMapLock.Wait();
    ReleaseDisplayLock(aDisplay);

    CEglSync** sync = iEglSyncMap.Find((EGLint)aSync);
    CHECK_CLEANUP_RET(sync && (*sync) && ((*sync)->Display() == aDisplay),
                      EGL_BAD_PARAMETER,
                      EGL_FALSE,
                      iEglSyncMapLock.Signal(),
                      "CEglSyncDisplayMap::EglDestroySyncKHR cannot find the sync");

    if (KErrNone != iEglSyncMap.Remove((EGLint)aSync))
        {
		SyncPanic(__LINE__);
        }

    if ((*sync)->DestroySyncReady())
        {
        delete *sync;
        }

    iEglSyncMapLock.Signal();


    EGL_TRACE("CEglSyncExtension::EglDestroySyncKHR EXIT aDisplay=%d, aSync=%d ret=%s", aDisplay, aSync, "EGL_TRUE");
    return EGL_TRUE;
    }

EGLBoolean CEglSyncExtension::EglSignalSyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode)
    {
    EGL_TRACE("CEglSyncExtension::EglSignalSyncKHR aDisplay=%d, aSync=%d aMode=%d", aDisplay, aSync, aMode);
    EGLBoolean ret = EGL_FALSE;

    RESET_EGL_ERROR();

    CHECK_AND_RET((aMode == EGL_SIGNALED_KHR) || (aMode == EGL_UNSIGNALED_KHR),
                   EGL_BAD_PARAMETER,
                   ret,
                   "CEglSyncExtension::EglSignalSyncKHR unsupported sync mode");

    EGLint lockErr = FindAndLockDisplay(aDisplay);

    CHECK_AND_RET(lockErr == EGL_SUCCESS,
                      lockErr,
                      EGL_FALSE,
                      "CEglSyncExtension::EglSignalSyncKHR couldn't find and lock display");


    iEglSyncMapLock.Wait();
    ReleaseDisplayLock(aDisplay);

    CEglSync** sync= iEglSyncMap.Find((EGLint)aSync);

    CHECK_CLEANUP_RET(sync && (*sync) && ((*sync)->Display()==aDisplay),
                      EGL_BAD_PARAMETER,
                      EGL_FALSE,
                      iEglSyncMapLock.Signal(),
                      "CEglSyncExtension::EglSignalSyncKHR cannot find the lock");

    TEST_CLEANUP_RET((*sync)->Display() == EGL_SYNC_REUSABLE_KHR,
                     EGL_BAD_MATCH,
                     iEglSyncMapLock.Signal(),
                     "CEglSyncExtension::EglSignalSyncKHR bad sync type");

    EGLint signalRet = (*sync)->SignalSync(aMode);

    if (signalRet != EGL_SUCCESS)
        {
        EGL_SET_ERROR(ret);
        EGL_TRACE("CEglSyncExtension::EglSignalSyncKHR failed (%d)", ret);
        ret = EGL_FALSE;
        }
    else
        {
        ret = EGL_TRUE;
        }

    iEglSyncMapLock.Signal();

    EGL_TRACE("CEglSyncExtension::EglSignalSyncKHR EXIT aDisplay=%d, aSync=%d ret=%s", aDisplay, aSync, ret ? "EGL_TRUE":"EGL_FALSE");
    return ret;
    }

EGLint CEglSyncExtension::Egl_Private_SignalSyncNOK(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode)
    {
    EGL_TRACE("CEglSyncExtension::EglPrivateSignalSyncKHR aDisplay=%d, aSync=%d aMode=%d", aDisplay, aSync, aMode);

    TEST_AND_RET((aMode == EGL_SIGNALED_KHR) || (aMode == EGL_UNSIGNALED_KHR),
                 EGL_BAD_PARAMETER,
                 "CEglSyncExtension::EglPrivateSignalSyncKHR unsupported sync mode");

    EGLint lockErr = FindAndLockDisplay(aDisplay);

    TEST_AND_RET(lockErr == EGL_SUCCESS,
                     lockErr,
                     "CEglSyncExtension::EglPrivateSignalSyncKHR couldn't find and lock display");



    iEglSyncMapLock.Wait();
    ReleaseDisplayLock(aDisplay);

    CEglSync** sync= iEglSyncMap.Find((EGLint)aSync);

    TEST_CLEANUP_RET(sync && (*sync) && ((*sync)->Display()==aDisplay),
                     EGL_BAD_PARAMETER,
                     iEglSyncMapLock.Signal(),
                     "CEglSyncExtension::EglPrivateSignalSyncKHR cannot find the lock");

    EGLint ret = (*sync)->SignalSync(aMode);

    if (ret != EGL_SUCCESS)
        {
        EGL_TRACE("CEglSyncExtension::EglPrivateSignalSyncKHR failed (%d)", ret);
        }

    iEglSyncMapLock.Signal();

    EGL_TRACE("CEglSyncExtension::EglPrivateSignalSyncKHR EXIT aDisplay=%d, aSync=%d ret=%d", aDisplay, aSync, ret);
    return ret;
    }

EGLint CEglSyncExtension::EglClientWaitSyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLint aFlags, EGLTimeKHR aTimeout)
    {
    EGL_TRACE("CEglSyncExtension::EglClientWaitSyncKHR aDisplay=%d, aSync=%d aFlags=0x%x, aTimeout=%ld", aDisplay, aSync, aFlags, aTimeout);

    RESET_EGL_ERROR();

    EGLint lockErr = FindAndLockDisplay(aDisplay);

    CHECK_AND_RET(lockErr == EGL_SUCCESS,
                      lockErr,
                      EGL_FALSE,
                      "CEglSyncExtension::EglClientWaitSyncKHR couldn't find and lock display");

    

    iEglSyncMapLock.Wait();
    ReleaseDisplayLock(aDisplay);

    CEglSync** sync= iEglSyncMap.Find((EGLint)aSync);

    CHECK_CLEANUP_RET(sync && (*sync) && ((*sync)->Display()==aDisplay),
                      EGL_BAD_PARAMETER,
                      EGL_FALSE,
                      iEglSyncMapLock.Signal(),
                      "CEglSyncExtension::EglClientWaitSyncKHR cannot find the sync");

    EGLint ret = (*sync)->ClientWaitSync(aFlags, aTimeout);


    EGL_TRACE("CEglSyncExtension::EglClientWaitSyncKHR EXIT aDisplay=%d, aSync=%d ret=%d", aDisplay, aSync, ret);
    return ret;
    }

EGLBoolean CEglSyncExtension::EglGetSyncAttribKHR(EGLDisplay aDisplay,
                                                  EGLSyncKHR aSync,
                                                  EGLint aAttribute,
                                                  EGLint* aValue)
    {
    EGL_TRACE("CEglSyncExtension::EglGetSyncAttribKHR aDisplay=%d, aSync=%d aAttribute=0x%x, aValue=0x%x", aDisplay,
                                                                                                           aSync,
                                                                                                           aAttribute,
                                                                                                           aValue);
    RESET_EGL_ERROR();

    EGLint lockErr = FindAndLockDisplay(aDisplay);

    CHECK_AND_RET(lockErr == EGL_SUCCESS,
                      lockErr,
                      EGL_FALSE,
                      "CEglSyncExtension::EglGetSyncAttribKHR couldn't find and lock display");


    iEglSyncMapLock.Wait();
    ReleaseDisplayLock(aDisplay);

    CEglSync** sync= iEglSyncMap.Find((EGLint)aSync);

    CHECK_CLEANUP_RET(sync && (*sync) && ((*sync)->Display()==aDisplay),
                      EGL_BAD_PARAMETER,
                      EGL_FALSE,
                      iEglSyncMapLock.Signal(),
                      "CEglSyncExtension::EglGetSyncAttribKHR cannot find the lock");


    EGLint ret = (*sync)->GetSyncAttrib(aAttribute, aValue);

    iEglSyncMapLock.Signal();



    EGL_TRACE("CEglSyncExtension::EglGetSyncAttribKHR EXIT aDisplay=%d, aSync=%d ret=%s", aDisplay, aSync, ret ? "EGL_TRUE":"EGL_FALSE");
    return ret;
    }

EGLint CEglSyncExtension::FindAndLockDisplay(EGLDisplay aDisplay)
    {
    return OpenVGRI::getEGL()->FindAndLockDisplay(aDisplay);
    }

void CEglSyncExtension::ReleaseDisplayLock(EGLDisplay aDisplay)
    {
	OpenVGRI::getEGL()->ReleaseDisplayLock(aDisplay);
    }

EGLSyncKHR CEglSyncExtension::CreateSync(EGLDisplay aDisplay, EGLenum aType)
    {
    CEglSync* sync = CEglSync::Create(iEglSyncMapLock,
                                      (EGLSyncKHR)(++iEglSyncId),
                                      aDisplay,
                                      aType);

    CHECK_AND_RET(sync != NULL,
                  EGL_BAD_ALLOC,
                  EGL_NO_SYNC_KHR,
                  "CEglSyncDisplayMap::CreateSync failed to create sync");

    TInt err = KErrNone;
    err = iEglSyncMap.Insert(iEglSyncId, sync);

    CHECK_CLEANUP_RET(err == KErrNone,
                      EGL_BAD_ALLOC,
                      EGL_NO_SYNC_KHR,
                      delete sync,
                      "CEglSyncDisplayMap::CreateSync insert failed");

    return (EGLSyncKHR)iEglSyncId;
    }

EGLSyncKHR CEglSyncExtension::eglCreateSyncKHR(EGLDisplay aDisplay, EGLenum aType, const EGLint* aAttribList)
    {
    CEglSyncExtension* eglSyncInstance = (CEglSyncExtension*)OpenVGRI::getEGL()->EGLSyncExtension();
    CHECK_AND_RET(eglSyncInstance,
                  EGL_NOT_INITIALIZED,
                  EGL_NO_SYNC_KHR,
                  "CEglSyncExtension::eglCreateSyncKHR extension instance not created");

    return eglSyncInstance->EglCreateSyncKHR(aDisplay, aType, aAttribList);
    }

EGLBoolean CEglSyncExtension::eglDestroySyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync)
    {
    CEglSyncExtension* eglSyncInstance = (CEglSyncExtension*)OpenVGRI::getEGL()->EGLSyncExtension();
    CHECK_AND_RET(eglSyncInstance,
                  EGL_NOT_INITIALIZED,
                  EGL_FALSE,
                 "CEglSyncExtension::eglDestroySyncKHR extension instance not created");

    return eglSyncInstance->EglDestroySyncKHR(aDisplay, aSync);
    }

EGLint CEglSyncExtension::eglClientWaitSyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLint aFlags, EGLTimeKHR aTimeout)
    {
    CEglSyncExtension* eglSyncInstance = (CEglSyncExtension*)OpenVGRI::getEGL()->EGLSyncExtension();
    CHECK_AND_RET(eglSyncInstance,
                  EGL_NOT_INITIALIZED,
                  EGL_FALSE,
                 "CEglSyncExtension::eglGetSyncAttribKHR extension instance not created");

    return eglSyncInstance->EglClientWaitSyncKHR(aDisplay, aSync, aFlags, aTimeout);
    }

EGLBoolean CEglSyncExtension::eglSignalSyncKHR(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode)
    {
    CEglSyncExtension* eglSyncInstance = (CEglSyncExtension*)OpenVGRI::getEGL()->EGLSyncExtension();
    CHECK_AND_RET(eglSyncInstance,
                  EGL_NOT_INITIALIZED,
                  EGL_FALSE,
                 "CEglSyncExtension::eglSignalSyncKHR extension instance not created");

    return eglSyncInstance->EglSignalSyncKHR(aDisplay, aSync, aMode);
    }


EGLBoolean CEglSyncExtension::eglGetSyncAttribKHR(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLint aAttribute, EGLint* aValue)
    {
    CEglSyncExtension* eglSyncInstance = (CEglSyncExtension*)OpenVGRI::getEGL()->EGLSyncExtension();
    CHECK_AND_RET(eglSyncInstance,
                  EGL_NOT_INITIALIZED,
                  EGL_FALSE,
                 "CEglSyncExtension::eglGetSyncAttribKHR extension instance not created");

    return eglSyncInstance->EglGetSyncAttribKHR(aDisplay, aSync, aAttribute, aValue);
    }

EGLint CEglSyncExtension::egl_Private_SignalSyncNOK(EGLDisplay aDisplay, EGLSyncKHR aSync, EGLenum aMode)
    {
    CEglSyncExtension* eglSyncInstance = (CEglSyncExtension*)OpenVGRI::getEGL()->EGLSyncExtension();
    CHECK_AND_RET(eglSyncInstance,
                  EGL_NOT_INITIALIZED,
                  EGL_NOT_INITIALIZED,
                  "CEglSyncExtension::egl_Private_SignalSyncNOK extension instance not created");

    return eglSyncInstance->Egl_Private_SignalSyncNOK(aDisplay, aSync, aMode);
    }
