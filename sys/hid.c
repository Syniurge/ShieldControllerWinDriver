/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    hid.c

Abstract:

    Code for handling HID related requests

Author:


Environment:

    kernel mode only

Revision History:

--*/

#include <hidusbfx2.h>

#ifdef ALLOC_PRAGMA
    #pragma alloc_text( PAGE, HidFx2SetFeature)
    #pragma alloc_text( PAGE, HidFx2GetFeature)
    #pragma alloc_text( PAGE, SendVendorCommand)
    #pragma alloc_text( PAGE, GetVendorData)
#endif

HID_REPORT_DESCRIPTOR       G_DefaultReportDescriptor[] = {
    0x05, 0x01,         /*  Usage Page (Desktop),               */
    0x15, 0x00,         /*  Logical Minimum (0),                */
    0x09, 0x05,         /*  Usage (Gamepad),                    */
    0xA1, 0x01,         /*  Collection (Application),           */
    0x85, 0x01,         /*      Report ID (1),                  */
    0x05, 0x09,         /*      Usage Page (Button),            */
    0x15, 0x00,         /*      Logical Minimum (0),            */
    0x25, 0x01,         /*      Logical Maximum (1),            */
    0x75, 0x01,         /*      Report Size (1),                */
    0x95, 0x0A,         /*      Report Count (10),              */
    0x09, 0x01,         /*      Usage (01h),                    */
    0x09, 0x02,         /*      Usage (02h),                    */
    0x09, 0x04,         /*      Usage (04h),                    */
    0x09, 0x05,         /*      Usage (05h),                    */
    0x09, 0x07,         /*      Usage (07h),                    */
    0x09, 0x08,         /*      Usage (08h),                    */
    0x09, 0x0E,         /*      Usage (0Eh),                    */
    0x09, 0x0F,         /*      Usage (0Fh),                    */
    0x09, 0x09,         /*      Usage (09h),                    */
    0x09, 0x0C,         /*      Usage (0Ch),                    */
    0x81, 0x02,         /*      Input (Variable),               */
    0x05, 0x0C,         /*      Usage Page (Consumer),          */
    0x95, 0x06,         /*      Report Count (6),               */
    0x09, 0xE2,         /*      Usage (Mute),                   */
    0x09, 0xE9,         /*      Usage (Volume Inc),             */
    0x09, 0xEA,         /*      Usage (Volume Dec),             */
    0x09, 0x30,         /*      Usage (Power),                  */
    0x0A, 0x24, 0x02,   /*      Usage (AC Back),                */
    0x0A, 0x23, 0x02,   /*      Usage (AC Home),                */
    0x81, 0x02,         /*      Input (Variable),               */
    0x05, 0x01,         /*      Usage Page (Desktop),           */
    0x09, 0x39,         /*      Usage (Hat Switch),             */
    0x25, 0x07,         /*      Logical Maximum (7),            */
    0x35, 0x00,         /*      Physical Minimum (0),           */
    0x46, 0x0E, 0x01,   /*      Physical Maximum (270),         */
    0x65, 0x14,         /*      Unit (Degrees),                 */
    0x75, 0x04,         /*      Report Size (4),                */
    0x95, 0x01,         /*      Report Count (1),               */
    0x81, 0x02,         /*      Input (Variable),               */
    0x81, 0x03,         /*      Input (Constant, Variable),     */
    0x09, 0x01,         /*      Usage (Pointer),                */
    0xA1, 0x00,         /*      Collection (Physical),          */
    0x75, 0x10,         /*          Report Size (16),           */
    //0x95, 0x04,         /*          Report Count (4),           */
    0x95, 0x06,         /*          Report Count (6),           */
    0x15, 0x00,         /*          Logical Minimum (0),        */
    0x26, 0xFF, 0xFF,   /*          Logical Maximum (65536),       */
    0x35, 0x00,         /*          Physical Minimum (0),       */
    0x46, 0xFF, 0xFF,   /*          Physical Maximum (65536),      */
    0x09, 0x30,         /*          Usage (X),                  */
    0x09, 0x31,         /*          Usage (Y),                  */
    0x09, 0x32,         /*          Usage (Z),                  */
    0x09, 0x35,         /*          Usage (Rz),                 */
    //0x81, 0x02,         /*          Input (Variable),           */
    //0x05, 0x02,         /*          Usage Page (Simulation),    */
    //0x95, 0x02,         /*          Report Count (2),           */
    //0x09, 0xC5,         /*          Usage (C5h),                */
    //0x09, 0xC4,         /*          Usage (C4h),                */ // Brake and accelerators turned to Rx and Ry below
    0x09, 0x33,         /*          Usage (Rx),                */ 
    0x09, 0x34,         /*          Usage (Ry),                */
    0x81, 0x02,         /*          Input (Variable),           */
    0xC0,               /*      End Collection,                 */
    0xA1, 0x01,         /*      Collection (Application),       */
    //0x19, 0x01,         /*          Usage Minimum (01h),        */ // Using "Usage Minimum" and "Maximum" prevents the gamepad from being recognized as one by DirectInput, go figure..
    //0x29, 0x03,         /*          Usage Maximum (03h),        */
    0x09, 0x01,         /*          Usage (01h),                 */   // left rumble
    0x09, 0x02,          /*          Usage (02h),                 */   // right rumble
    0x09, 0x03,         /*          Usage (03h),                 */
    0x15, 0x00,         /*          Logical Minimum (0),        */
    0x26, 0xFF, 0xFF,   /*          Logical Maximum (65536),       */
    0x95, 0x03,         /*          Report Count (3),           */
    0x75, 0x10,         /*          Report Size (16),           */
    0x91, 0x02,         /*          Output (Variable),          */
    0xC0,               /*      End Collection,                 */
    0xC0,               /*  End Collection,                     */
    0x05, 0x01,         /*  Usage Page (Desktop),               */
    0x09, 0x02,         /*  Usage (Mouse),                      */
    0xA1, 0x01,         /*  Collection (Application),           */
    0x85, 0x02,         /*      Report ID (2),                  */
    0x09, 0x01,         /*      Usage (Pointer),                */
    0xA1, 0x00,         /*      Collection (Physical),          */
    0x05, 0x09,         /*          Usage Page (Button),        */
    0x19, 0x01,         /*          Usage Minimum (01h),        */
    0x29, 0x03,         /*          Usage Maximum (03h),        */
    0x25, 0x01,         /*          Logical Maximum (1),        */
    0x75, 0x01,         /*          Report Size (1),            */
    0x95, 0x03,         /*          Report Count (3),           */
    0x81, 0x02,         /*          Input (Variable),           */
    0x05, 0x09,         /*          Usage Page (Button),        */
    0x09, 0x05,         /*          Usage (05h),                */
    0x95, 0x01,         /*          Report Count (1),           */
    0x81, 0x02,         /*          Input (Variable),           */
    0x75, 0x04,         /*          Report Size (4),            */
    0x81, 0x01,         /*          Input (Constant),           */
    0x05, 0x01,         /*          Usage Page (Desktop),       */
    0x09, 0x30,         /*          Usage (X),                  */
    0x09, 0x31,         /*          Usage (Y),                  */
    0x15, 0x81,         /*          Logical Minimum (-127),     */
    0x25, 0x7F,         /*          Logical Maximum (127),      */
    0x75, 0x10,         /*          Report Size (16),           */
    0x95, 0x02,         /*          Report Count (2),           */
    0x81, 0x06,         /*          Input (Variable, Relative), */
    0xC0,               /*      End Collection,                 */
    0xC0,               /*  End Collection,                     */
    0x06, 0xDE, 0xFF,   /*  Usage Page (FFDEh),                 */
    0x09, 0x01,         /*  Usage (01h),                        */
    0xA1, 0x01,         /*  Collection (Application),           */
    0x05, 0xFF,         /*      Usage Page (FFh),               */
    0x19, 0x01,         /*      Usage Minimum (01h),            */
    0x29, 0x40,         /*      Usage Maximum (40h),            */
    0x85, 0xFD,         /*      Report ID (253),                */
    0x15, 0x00,         /*      Logical Minimum (0),            */
    0x25, 0xFF,         /*      Logical Maximum (-1),           */
    0x95, 0x40,         /*      Report Count (64),              */
    0x75, 0x08,         /*      Report Size (8),                */
    0x81, 0x02,         /*      Input (Variable),               */
    0xC0,               /*  End Collection,                     */
    0x06, 0xDE, 0xFF,   /*  Usage Page (FFDEh),                 */
    0x09, 0x03,         /*  Usage (03h),                        */
    0xA1, 0x01,         /*  Collection (Application),           */
    0x19, 0x01,         /*      Usage Minimum (01h),            */
    0x29, 0x40,         /*      Usage Maximum (40h),            */
    0x85, 0xFC,         /*      Report ID (252),                */
    0x95, 0x40,         /*      Report Count (64),              */
    0x75, 0x08,         /*      Report Size (8),                */
    0xB1, 0x02,         /*      Feature (Variable),             */
    0xC0                /*  End Collection                      */

};

static PVOID USBPcapURBGetBufferPointer(ULONG length,
    PVOID buffer,
    PMDL  bufferMDL)
{
    ASSERT((length == 0) ||
        ((length != 0) && (buffer != NULL || bufferMDL != NULL)));

    if (length == 0)
    {
        return NULL;
    }
    else if (buffer != NULL)
    {
        return buffer;
    }
    else if (bufferMDL != NULL)
    {
        PVOID address = MmGetSystemAddressForMdlSafe(bufferMDL,
            NormalPagePriority);
        return address;
    }
    else
    {
        //DkDbgStr("Invalid buffer!");
        return NULL;
    }
}

VOID
NvShieldIoInternalDeviceControlComplete(
    IN WDFREQUEST Request,
    IN WDFIOTARGET Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS Params,
    IN WDFCONTEXT Context
)
{
    UNREFERENCED_PARAMETER(Target);

    USHORT UrbFunction = (USHORT)(ptrdiff_t)Context;

    if (UrbFunction == URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE) {
        PURB pUrb = (PURB)IoGetCurrentIrpStackLocation(WdfRequestWdmGetIrp(Request))->Parameters.Others.Argument1;

        if (pUrb->UrbHeader.Function == URB_FUNCTION_CONTROL_TRANSFER)
        {
            struct _URB_BULK_OR_INTERRUPT_TRANSFER* pTransfer = (struct _URB_BULK_OR_INTERRUPT_TRANSFER*) pUrb;

            DbgPrint("pDescriptorRequest->TransferBufferLength = %d\n", pTransfer->TransferBufferLength);

            char* buf = (char*)USBPcapURBGetBufferPointer(pTransfer->TransferBufferLength,
                pTransfer->TransferBuffer, pTransfer->TransferBufferMDL);

            if (pTransfer->TransferBufferLength == 34) { // HID Descriptor
                buf[25] = (char) sizeof(G_DefaultReportDescriptor);
            }
            else if (pTransfer->TransferBufferLength == 241) { // HID Report Descriptor
#if 0
                                                               //if ((buf[0] == 0x05) && (buf[1] == 0x00) && (buf[2] == 0x01) && (buf[3] == 0x05)) {
                                                               //    DbgPrint("HID Report Descriptor detected\n", pDescriptorRequest->TransferBufferLength);
                                                               //}

                                                               //char copybuf[sizeof(G_DefaultReportDescriptor)];
                                                               //RtlCopyMemory(&copybuf[0], &buf[138], sizeof(G_DefaultReportDescriptor) - 138);
                                                               //RtlCopyMemory(&buf[122], &copybuf[0], sizeof(G_DefaultReportDescriptor) - 138);

                                                               //pDescriptorRequest->TransferBufferLength -= sizeof(G_DefaultReportDescriptor) - 138;
#endif

                RtlZeroMemory(&buf[0], 241);
                RtlCopyMemory(&buf[0], &G_DefaultReportDescriptor[0], sizeof(G_DefaultReportDescriptor));
                pTransfer->TransferBufferLength = sizeof(G_DefaultReportDescriptor);
            }
        }
    }

    WdfRequestComplete(Request, Params->IoStatus.Status);
}

VOID
NvShieldForwardRequest(
    IN WDFDEVICE Device,
    IN WDFREQUEST Request
)
{
    WDF_REQUEST_SEND_OPTIONS options;
    PDEVICE_EXTENSION          devContext;
    NTSTATUS                 status;

    //
    // Get the context that we setup during DeviceAdd processing
    //
    devContext = GetDeviceContext(Device);

    //
    // We're just going to be passing this request on with 
    //  zero regard for what happens to it. Therefore, we'll
    //  use the WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET option
    //
    WDF_REQUEST_SEND_OPTIONS_INIT(
        &options,
        WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET);


    //
    // And send it!
    // 
    if (!WdfRequestSend(Request,
        devContext->TargetToSendRequestsTo,
        &options)) {

        //
        // Oops! Something bad happened, complete the request
        //
        status = WdfRequestGetStatus(Request);
        WdfRequestComplete(Request, status);
    }
    return;
}


VOID
HidFx2EvtInternalDeviceControl(
    IN WDFQUEUE     Queue,
    IN WDFREQUEST   Request,
    IN size_t       OutputBufferLength,
    IN size_t       InputBufferLength,
    IN ULONG        IoControlCode
    )
/*++

Routine Description:

    This event is called when the framework receives 
    IRP_MJ_INTERNAL DEVICE_CONTROL requests from the system.

Arguments:

    Queue - Handle to the framework queue object that is associated
            with the I/O request.
    Request - Handle to a framework request object.

    OutputBufferLength - length of the request's output buffer,
                        if an output buffer is available.
    InputBufferLength - length of the request's input buffer,
                        if an input buffer is available.

    IoControlCode - the driver-defined or system-defined I/O control code
                    (IOCTL) that is associated with the request.
Return Value:

    VOID

--*/

{
    NTSTATUS            status = STATUS_SUCCESS;
    WDFDEVICE           device;
    PDEVICE_EXTENSION   devContext = NULL;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    device = WdfIoQueueGetDevice(Queue);
    devContext = GetDeviceContext(device);

    if (IoControlCode == IOCTL_INTERNAL_USB_SUBMIT_URB)
    {
        PURB pUrb = (PURB)IoGetCurrentIrpStackLocation(WdfRequestWdmGetIrp(Request))->Parameters.Others.Argument1;

        switch (pUrb->UrbHeader.Function)
        {

        case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE:
        case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE:
            {
                //struct _URB_CONTROL_DESCRIPTOR_REQUEST* descRequest = (struct _URB_CONTROL_DESCRIPTOR_REQUEST*)pUrb;

                WdfRequestFormatRequestUsingCurrentType(Request);

                WdfRequestSetCompletionRoutine(Request,
                    NvShieldIoInternalDeviceControlComplete,
                    (WDFCONTEXT)URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE);

                if (!WdfRequestSend(Request, devContext->TargetToSendRequestsTo, NULL)) {
                    // Oops! Something bad happened, complete the request
                    status = WdfRequestGetStatus(Request);
                    WdfRequestComplete(Request, status);
                }
                return;
            }

            default:
                break;

        }
    }

    NvShieldForwardRequest(WdfIoQueueGetDevice(Queue), Request);
}


