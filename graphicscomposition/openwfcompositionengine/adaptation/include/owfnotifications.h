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
// Symbian notifications
// 
//

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \brief Initialises the Symbian specific device data, including
 *  the SUS observers
 *
 *  \param source the source to be initialised
 *  \return OWF_TRUE id successfull
 */
OWF_API_CALL OWFboolean
owfSymDeviceInitialise(WFC_IMAGE_PROVIDER* source);

/*!
 *  \brief Destroys the Symbian specific device data, including
 *  the SUS observers
 *
 *  \param device the device to be destroyed
 *  \param source the source to be initialised
 *  \param position in the device list
 */
OWF_API_CALL void
owfSymDeviceDestroy(WFC_DEVICE* device, WFC_IMAGE_PROVIDER* source, WFCint position);

/*!
 *  \brief Notifies the stream associated with element that the read buffer has been aquired
 *
 *  Used to solve the initial availabilty request
 *
 *  \param context The context corresponding to the ongoing composition
 *  \param element The element that composition has just started for
 *
 *  \return OWF_TRUE id successfull
 */
OWF_API_CALL OWFboolean
owfSymElementNotifications(WFC_CONTEXT* context, WFC_ELEMENT* element);

/*!
 *  \brief Resets the visibility state associated with each source
 *
 *  Used after each commit
 *
 *  \param context The context
 */
OWF_API_CALL void
owfSymDeviceResetVisibilityState(WFC_CONTEXT* context);

/*!
 *  \brief Processes all notifications for a context immediately after the context has been composed 
 *
 *  \param context The context coresponding to the caller composer
 */
OWF_API_CALL WFCint
owfSymProcessAllNotifications(WFC_CONTEXT* context);

#ifdef __cplusplus
}
#endif
