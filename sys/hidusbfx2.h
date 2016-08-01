/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    hidusbfx2.h
    
Abstract:

    common header file

Author:


Environment:

    kernel mode only

Notes:


Revision History:


--*/
#ifndef _HIDUSBFX2_H_

#define _HIDUSBFX2_H_

#pragma warning(disable:4200)  // suppress nameless struct/union warning
#pragma warning(disable:4201)  // suppress nameless struct/union warning
#pragma warning(disable:4214)  // suppress bit field types other than int warning
#include <initguid.h>
#include <wdm.h>
#include "usbdi.h"
#include "usbdlib.h"

#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)
#include <wdf.h>
#include "wdfusb.h"

#pragma warning(disable:4201)  // suppress nameless struct/union warning
#pragma warning(disable:4214)  // suppress bit field types other than int warning
#include <hidport.h>

#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

typedef UCHAR HID_REPORT_DESCRIPTOR, *PHID_REPORT_DESCRIPTOR;

typedef struct _DEVICE_EXTENSION{

    //
    //WDF handles for USB Target 
    //
    WDFUSBDEVICE      UsbDevice;

    WDFIOTARGET TargetToSendRequestsTo; 
    
    // Rumble state
    int isRumbling;
    int isZeroRumble;

    unsigned short leftRumbleStrength;
    unsigned short rightRumbleStrength;

    unsigned short rumbleGain;

    LONG actuatorSel; // 1 = left, 2 = right

    // Trackpad state
    UCHAR origX;
    UCHAR origY;

    int isTrackpadPressed;
    LARGE_INTEGER firstTrackpadPress;
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_EXTENSION, GetDeviceContext)

//
// driver routine declarations
//
// This type of function declaration is for Prefast for drivers. 
// Because this declaration specifies the function type, PREfast for Drivers
// does not need to infer the type or to report an inference. The declaration
// also prevents PREfast for Drivers from misinterpreting the function type 
// and applying inappropriate rules to the function. For example, PREfast for
// Drivers would not apply rules for completion routines to functions of type
// DRIVER_CANCEL. The preferred way to avoid Warning 28101 is to declare the
// function type explicitly. In the following example, the DriverEntry function
// is declared to be of type DRIVER_INITIALIZE.
//
DRIVER_INITIALIZE DriverEntry;

EVT_WDF_DRIVER_DEVICE_ADD HidFx2EvtDeviceAdd;

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL HidFx2EvtInternalDeviceControl;

EVT_WDF_OBJECT_CONTEXT_CLEANUP HidFx2EvtDriverContextCleanup;

#endif   //_HIDUSBFX2_H_

