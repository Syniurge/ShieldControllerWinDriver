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

        // ====== Virtual PID force feedback ======= //

    0x05,0x0F,        //    Usage Page Physical Interface
    0x09,0x92,        //    Usage ES Playing
    0xA1,0x02,        //    Collection Datalink
    0x85,0x20,    //    Report ID 20h
    0x09,0x9F,    //    Usage (Device Paused)
    0x09,0xA0,    //    Usage (Actuators Enabled)
    0x09,0xA4,    //    Usage (Safety Switch)
    0x09,0xA5,    //    Usage (Actuator Override Switch)
    0x09,0xA6,    //    Usage (Actuator Power)
    0x15,0x00,    //    Logical Minimum 0
    0x25,0x01,    //    Logical Maximum 1
    0x35,0x00,    //    Physical Minimum 0
    0x45,0x01,    //    Physical Maximum 1
    0x75,0x01,    //    Report Size 1
    0x95,0x05,    //    Report Count 5
    0x81,0x02,    //    Input (Variable)
    0x95,0x03,    //    Report Count 3
    0x75,0x01,    //    Report Size 1
    0x81,0x03,    //    Input (Constant, Variable)
    0x09,0x94,    //    Usage (Effect Playing)
    0x15,0x00,    //    Logical Minimum 0
    0x25,0x01,    //    Logical Maximum 1
    0x35,0x00,    //    Physical Minimum 0
    0x45,0x01,    //    Physical Maximum 1
    0x75,0x01,    //    Report Size 1
    0x95,0x01,    //    Report Count 1
    0x81,0x02,    //    Input (Variable)
    0x09,0x22,    //    Usage Effect Block Index
    0x15,0x01,    //    Logical Minimum 1
    0x25,0x28,    //    Logical Maximum 28h (40d)
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x28,    //    Physical Maximum 28h (40d)
    0x75,0x07,    //    Report Size 7
    0x95,0x01,    //    Report Count 1
    0x81,0x02,    //    Input (Variable)
    0xC0    ,    // End Collection


    0x09,0x21,    //    Usage Set Effect Report
    0xA1,0x02,    //    Collection Datalink
    0x85,0x21,    //    Report ID 21h
    0x09,0x22,    //    Usage Effect Block Index
    0x15,0x01,    //    Logical Minimum 1
    0x25,0x28,    //    Logical Maximum 28h (40d)
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x28,    //    Physical Maximum 28h (40d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x02,    //    Output (Variable)
    0x09,0x25,    //    Usage Effect Type
    0xA1,0x02,    //    Collection Datalink
    0x09,0x26,    //    Usage ET Constant Force
    0x09,0x27,    //    Usage ET Ramp
    0x09,0x30,    //    Usage ET Square
    0x09,0x31,    //    Usage ET Sine
    0x09,0x32,    //    Usage ET Triangle
    0x09,0x33,    //    Usage ET Sawtooth Up
    0x09,0x34,    //    Usage ET Sawtooth Down
    0x09,0x40,    //    Usage ET Spring
    0x09,0x41,    //    Usage ET Damper
    0x09,0x42,    //    Usage ET Inertia
    0x09,0x43,    //    Usage ET Friction
    0x09,0x28,    //    Usage ET Custom Force Data
    0x25,0x0C,    //    Logical Maximum Ch (12d)
    0x15,0x01,    //    Logical Minimum 1
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x0C,    //    Physical Maximum Ch (12d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x00,    //    Output
    0xC0    ,          //    End Collection

    0x09,0x50,         //    Usage Duration
    0x09,0x54,         //    Usage Trigger Repeat Interval
    0x09,0x51,         //    Usage Sample Period
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x7F,    //    Logical Maximum 7FFFh (32767d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x7F,    //    Physical Maximum 7FFFh (32767d)
    0x66,0x03,0x10,    //    Unit 1003h (4099d)
    0x55,0xFD,         //    Unit Exponent FDh (253d)
    0x75,0x10,         //    Report Size 10h (16d)
    0x95,0x03,         //    Report Count 3
    0x91,0x02,         //    Output (Variable)
    0x55,0x00,         //    Unit Exponent 0
    0x66,0x00,0x00,    //    Unit 0
    0x09,0x52,         //    Usage Gain
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x53,         //    Usage Trigger Button
    0x15,0x01,         //    Logical Minimum 1
    0x25,0x08,         //    Logical Maximum 8
    0x35,0x01,         //    Physical Minimum 1
    0x45,0x08,         //    Physical Maximum 8
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x55,         //    Usage Axes Enable
    0xA1,0x02,         //    Collection Datalink
    0x05,0x01,    //    Usage Page Generic Desktop
    0x09,0x30,    //    Usage X
    0x09,0x31,    //    Usage Y
    0x15,0x00,    //    Logical Minimum 0
    0x25,0x01,    //    Logical Maximum 1
    0x75,0x01,    //    Report Size 1
    0x95,0x02,    //    Report Count 2
    0x91,0x02,    //    Output (Variable)
    0xC0     ,    // End Collection
    0x05,0x0F,    //    Usage Page Physical Interface
    0x09,0x56,    //    Usage Direction Enable
    0x95,0x01,    //    Report Count 1
    0x91,0x02,    //    Output (Variable)
    0x95,0x05,    //    Report Count 5
    0x91,0x03,    //    Output (Constant, Variable)
    0x09,0x57,    //    Usage Direction
    0xA1,0x02,    //    Collection Datalink
    0x0B,0x01,0x00,0x0A,0x00,    //    Usage Ordinals: Instance 1
    0x0B,0x02,0x00,0x0A,0x00,    //    Usage Ordinals: Instance 2
    0x66,0x14,0x00,              //    Unit 14h (20d)
    0x55,0xFE,                   //    Unit Exponent FEh (254d)
    0x15,0x00,                   //    Logical Minimum 0
    0x26,0xFF,0x00,              //    Logical Maximum FFh (255d)
    0x35,0x00,                   //    Physical Minimum 0
    0x47,0xA0,0x8C,0x00,0x00,    //    Physical Maximum 8CA0h (36000d)
    0x66,0x00,0x00,              //    Unit 0
    0x75,0x08,                   //    Report Size 8
    0x95,0x02,                   //    Report Count 2
    0x91,0x02,                   //    Output (Variable)
    0x55,0x00,                   //    Unit Exponent 0
    0x66,0x00,0x00,              //    Unit 0
    0xC0     ,         //    End Collection
    0x05,0x0F,         //    Usage Page Physical Interface
    0x09,0xA7,         //    Usage Undefined
    0x66,0x03,0x10,    //    Unit 1003h (4099d)
    0x55,0xFD,         //    Unit Exponent FDh (253d)
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x7F,    //    Logical Maximum 7FFFh (32767d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x7F,    //    Physical Maximum 7FFFh (32767d)
    0x75,0x10,         //    Report Size 10h (16d)
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x66,0x00,0x00,    //    Unit 0
    0x55,0x00,         //    Unit Exponent 0
    0xC0     ,    //    End Collection
    0x05,0x0F,    //    Usage Page Physical Interface
    0x09,0x5A,    //    Usage Set Envelope Report
    0xA1,0x02,    //    Collection Datalink
    0x85, 0x20,    //    Report ID 20h
    0x09,0x22,         //    Usage Effect Block Index
    0x15,0x01,         //    Logical Minimum 1
    0x25,0x28,         //    Logical Maximum 28h (40d)
    0x35,0x01,         //    Physical Minimum 1
    0x45,0x28,         //    Physical Maximum 28h (40d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x5B,         //    Usage Attack Level
    0x09,0x5D,         //    Usage Fade Level
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x95,0x02,         //    Report Count 2
    0x91,0x02,         //    Output (Variable)
    0x09,0x5C,         //    Usage Attack Time
    0x09,0x5E,         //    Usage Fade Time
    0x66,0x03,0x10,    //    Unit 1003h (4099d)
    0x55,0xFD,         //    Unit Exponent FDh (253d)
    0x26,0xFF,0x7F,    //    Logical Maximum 7FFFh (32767d)
    0x46,0xFF,0x7F,    //    Physical Maximum 7FFFh (32767d)
    0x75,0x10,         //    Report Size 10h (16d)
    0x91,0x02,         //    Output (Variable)
    0x45,0x00,         //    Physical Maximum 0
    0x66,0x00,0x00,    //    Unit 0
    0x55,0x00,         //    Unit Exponent 0
    0xC0     ,            //    End Collection
    0x09,0x5F,    //    Usage Set Condition Report
    0xA1,0x02,    //    Collection Datalink
    0x85,0x03,    //    Report ID 3
    0x09,0x22,    //    Usage Effect Block Index
    0x15,0x01,    //    Logical Minimum 1
    0x25,0x28,    //    Logical Maximum 28h (40d)
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x28,    //    Physical Maximum 28h (40d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x02,    //    Output (Variable)
    0x09,0x23,    //    Usage Parameter Block Offset
    0x15,0x00,    //    Logical Minimum 0
    0x25,0x01,    //    Logical Maximum 1
    0x35,0x00,    //    Physical Minimum 0
    0x45,0x01,    //    Physical Maximum 1
    0x75,0x04,    //    Report Size 4
    0x95,0x01,    //    Report Count 1
    0x91,0x02,    //    Output (Variable)
    0x09,0x58,    //    Usage Type Specific Block Off...
    0xA1,0x02,    //    Collection Datalink
    0x0B,0x01,0x00,0x0A,0x00,    //    Usage Ordinals: Instance 1
    0x0B,0x02,0x00,0x0A,0x00,    //    Usage Ordinals: Instance 2
    0x75,0x02,                   //    Report Size 2
    0x95,0x02,                   //    Report Count 2
    0x91,0x02,                   //    Output (Variable)
    0xC0     ,         //    End Collection
    0x15,0x80,         //    Logical Minimum 80h (-128d)
    0x25,0x7F,         //    Logical Maximum 7Fh (127d)
    0x36,0xF0,0xD8,    //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x09,0x60,         //    Usage CP Offset
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x36,0xF0,0xD8,    //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x09,0x61,         //    Usage Positive Coefficient
    0x09,0x62,         //    Usage Negative Coefficient
    0x95,0x02,         //    Report Count 2
    0x91,0x02,         //    Output (Variable)
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x09,0x63,         //    Usage Positive Saturation
    0x09,0x64,         //    Usage Negative Saturation
    0x75,0x08,         //    Report Size 8
    0x95,0x02,         //    Report Count 2
    0x91,0x02,         //    Output (Variable)
    0x09,0x65,         //    Usage Dead Band
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0xC0     ,    //    End Collection
    0x09,0x6E,    //    Usage Set Periodic Report
    0xA1,0x02,    //    Collection Datalink
    0x85,0x04,                   //    Report ID 4
    0x09,0x22,                   //    Usage Effect Block Index
    0x15,0x01,                   //    Logical Minimum 1
    0x25,0x28,                   //    Logical Maximum 28h (40d)
    0x35,0x01,                   //    Physical Minimum 1
    0x45,0x28,                   //    Physical Maximum 28h (40d)
    0x75,0x08,                   //    Report Size 8
    0x95,0x01,                   //    Report Count 1
    0x91,0x02,                   //    Output (Variable)
    0x09,0x70,                   //   Usage Magnitude
    0x15,0x00,                   //    Logical Minimum 0
    0x26,0xFF,0x00,              //    Logical Maximum FFh (255d)
    0x35,0x00,                   //    Physical Minimum 0
    0x46,0x10,0x27,              //    Physical Maximum 2710h (10000d)
    0x75,0x08,                   //    Report Size 8
    0x95,0x01,                   //    Report Count 1
    0x91,0x02,                   //    Output (Variable)
    0x09,0x6F,                   //   Usage Offset
    0x15,0x80,                   //    Logical Minimum 80h (-128d)
    0x25,0x7F,                   //    Logical Maximum 7Fh (127d)
    0x36,0xF0,0xD8,              //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,              //    Physical Maximum 2710h (10000d)
    0x95,0x01,                   //    Report Count 1
    0x91,0x02,                   //    Output (Variable)
    0x09,0x71,                   //   Usage Phase
    0x66,0x14,0x00,              //    Unit 14h (20d)
    0x55,0xFE,                   //    Unit Exponent FEh (254d)
    0x15,0x00,                   //    Logical Minimum 0
    0x26,0xFF,0x00,              //    Logical Maximum FFh (255d)
    0x35,0x00,                   //    Physical Minimum 0
    0x47,0xA0,0x8C,0x00,0x00,    //    Physical Maximum 8CA0h (36000d)
    0x91,0x02,                   //    Output (Variable)
    0x09,0x72,                   //   Usage Period
    0x26,0xFF,0x7F,              //    Logical Maximum 7FFFh (32767d)
    0x46,0xFF,0x7F,              //    Physical Maximum 7FFFh (32767d)
    0x66,0x03,0x10,              //    Unit 1003h (4099d)
    0x55,0xFD,                   //    Unit Exponent FDh (253d)
    0x75,0x10,                   //    Report Size 10h (16d)
    0x95,0x01,                   //    Report Count 1
    0x91,0x02,                   //    Output (Variable)
    0x66,0x00,0x00,              //    Unit 0
    0x55,0x00,                   //    Unit Exponent 0
    0xC0     ,    // End Collection
    0x09,0x73,    //    Usage Set Constant Force Rep...
    0xA1,0x02,    //    Collection Datalink
    0x85,0x05,         //    Report ID 5
    0x09,0x22,         //    Usage Effect Block Index
    0x15,0x01,         //    Logical Minimum 1
    0x25,0x28,         //    Logical Maximum 28h (40d)
    0x35,0x01,         //    Physical Minimum 1
    0x45,0x28,         //    Physical Maximum 28h (40d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x70,         //    Usage Magnitude
    0x16,0x01,0xFF,    //    Logical Minimum FF01h (-255d)
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x36,0xF0,0xD8,    //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x75,0x10,         //    Report Size 10h (16d)
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0xC0     ,    //    End Collection
    0x09,0x74,    //    Usage Set Ramp Force Report
    0xA1,0x02,    //    Collection Datalink
    0x85,0x06,         //    Report ID 6
    0x09,0x22,         //    Usage Effect Block Index
    0x15,0x01,         //    Logical Minimum 1
    0x25,0x28,         //    Logical Maximum 28h (40d)
    0x35,0x01,         //    Physical Minimum 1
    0x45,0x28,         //    Physical Maximum 28h (40d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x75,         //    Usage Ramp Start
    0x09,0x76,         //    Usage Ramp End
    0x15,0x80,         //    Logical Minimum 80h (-128d)
    0x25,0x7F,         //    Logical Maximum 7Fh (127d)
    0x36,0xF0,0xD8,    //    Physical Minimum D8F0h (-10000d)
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x75,0x08,         //    Report Size 8
    0x95,0x02,         //    Report Count 2
    0x91,0x02,         //    Output (Variable)
    0xC0     ,    //    End Collection
    0x09,0x68,    //    Usage Custom Force Data Rep...
    0xA1,0x02,    //    Collection Datalink
    0x85,0x07,         //    Report ID 7
    0x09,0x22,         //    Usage Effect Block Index
    0x15,0x01,         //    Logical Minimum 1
    0x25,0x28,         //    Logical Maximum 28h (40d)
    0x35,0x01,         //    Physical Minimum 1
    0x45,0x28,         //    Physical Maximum 28h (40d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x6C,         //    Usage Custom Force Data Offset
    0x15,0x00,         //    Logical Minimum 0
    0x26,0x10,0x27,    //    Logical Maximum 2710h (10000d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x75,0x10,         //    Report Size 10h (16d)
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x69,         //    Usage Custom Force Data
    0x15,0x81,         //    Logical Minimum 81h (-127d)
    0x25,0x7F,         //    Logical Maximum 7Fh (127d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x00,    //    Physical Maximum FFh (255d)
    0x75,0x08,         //    Report Size 8
    0x95,0x0C,         //    Report Count Ch (12d)
    0x92,0x02,0x01,    //       Output (Variable, Buffered)
    0xC0     ,    //    End Collection
    0x09,0x66,    //    Usage Download Force Sample
    0xA1,0x02,    //    Collection Datalink
    0x85,0x08,         //    Report ID 8
    0x05,0x01,         //    Usage Page Generic Desktop
    0x09,0x30,         //    Usage X
    0x09,0x31,         //    Usage Y
    0x15,0x81,         //    Logical Minimum 81h (-127d)
    0x25,0x7F,         //    Logical Maximum 7Fh (127d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x00,    //    Physical Maximum FFh (255d)
    0x75,0x08,         //    Report Size 8
    0x95,0x02,         //    Report Count 2
    0x91,0x02,         //    Output (Variable)
    0xC0     ,   //    End Collection
    0x05,0x0F,   //    Usage Page Physical Interface
    0x09,0x77,   //    Usage Effect Operation Report
    0xA1,0x02,   //    Collection Datalink
    0x85,0x0A,    //    Report ID Ah (10d)
    0x09,0x22,    //    Usage Effect Block Index
    0x15,0x01,    //    Logical Minimum 1
    0x25,0x28,    //    Logical Maximum 28h (40d)
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x28,    //    Physical Maximum 28h (40d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x02,    //    Output (Variable)
    0x09,0x78,    //    Usage Operation
    0xA1,0x02,    //    Collection Datalink
    0x09,0x79,    //    Usage Op Effect Start
    0x09,0x7A,    //    Usage Op Effect Start Solo
    0x09,0x7B,    //    Usage Op Effect Stop
    0x15,0x01,    //    Logical Minimum 1
    0x25,0x03,    //    Logical Maximum 3
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x00,    //    Output
    0xC0     ,         //    End Collection
    0x09,0x7C,         //    Usage Loop Count
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x00,    //    Physical Maximum FFh (255d)
    0x91,0x02,         //    Output (Variable)
    0xC0     ,    //    End Collection
    0x09,0x90,    //    Usage PID State Report (PID Block Free Report)
    0xA1,0x02,    //    Collection Datalink
    0x85,0x0B,    //    Report ID Bh (11d)
    0x09,0x22,    //    Usage Effect Block Index
    0x25,0x28,    //    Logical Maximum 28h (40d)
    0x15,0x01,    //    Logical Minimum 1
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x28,    //    Physical Maximum 28h (40d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x02,    //    Output (Variable)
    0xC0     ,    //    End Collection
    0x09,0x96,    //    Usage DC Disable Actuators (PID Device Control)
    0xA1,0x02,    //    Collection Datalink
    0x85,0x0C,    //    Report ID Ch (12d)
    0x09,0x97,    //    Usage DC Stop All Effects (DC Enable Actuators)
    0x09,0x98,    //    Usage DC Device Reset (DC Disable Actuators)
    0x09,0x99,    //    Usage DC Device Pause (DC Stop All Effects)
    0x09,0x9A,    //    Usage DC Device Continue (DC Device Reset?)
    0x09,0x9B,    //    Usage PID Device State (DC Device Pause)
    0x09,0x9C,    //    Usage DS Actuators Enabled
    0x15,0x01,    //    Logical Minimum 1
    0x25,0x06,    //    Logical Maximum 6
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0x91,0x00,    //    Output
    0xC0     ,    //    End Collection
    0x09,0x7D,    //    Usage PID Pool Report (Device Gain Report)
    0xA1,0x02,    //    Collection Datalink
    0x85,0x0D,         //    Report ID Dh (13d)
    0x09,0x7E,         //    Usage RAM Pool Size (Device Gain)
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0x10,0x27,    //    Physical Maximum 2710h (10000d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0xC0     ,            //    End Collection
    0x09,0x6B,    //    Usage Set Custom Force Report
    0xA1,0x02,    //    Collection Datalink
    0x85,0x0E,         //    Report ID Eh (14d)
    0x09,0x22,         //    Usage Effect Block Index
    0x15,0x01,         //    Logical Minimum 1
    0x25,0x28,         //    Logical Maximum 28h (40d)
    0x35,0x01,         //    Physical Minimum 1
    0x45,0x28,         //    Physical Maximum 28h (40d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x6D,         //    Usage Sample Count
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x00,    //    Logical Maximum FFh (255d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x00,    //    Physical Maximum FFh (255d)
    0x75,0x08,         //    Report Size 8
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x09,0x51,         //    Usage Sample Period
    0x66,0x03,0x10,    //    Unit 1003h (4099d)
    0x55,0xFD,         //    Unit Exponent FDh (253d)
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x7F,    //    Logical Maximum 7FFFh (32767d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x7F,    //    Physical Maximum 7FFFh (32767d)
    0x75,0x10,         //    Report Size 10h (16d)
    0x95,0x01,         //    Report Count 1
    0x91,0x02,         //    Output (Variable)
    0x55,0x00,         //    Unit Exponent 0
    0x66,0x00,0x00,    //    Unit 0
    0xC0     ,    //    End Collection
    0x09,0xAB,    //    Usage Undefined << Create New Effect Report
    0xA1,0x02,    //    Collection Datalink
    0x85,0x09,    //    Report ID 9
    0x09,0x25,    //    Usage Effect Type
    0xA1,0x02,    //    Collection Datalink
    0x09,0x26,    //    Usage ET Constant Force
    0x09,0x27,    //    Usage ET Ramp
    0x09,0x30,    //    Usage ET Square
    0x09,0x31,    //    Usage ET Sine
    0x09,0x32,    //    Usage ET Triangle
    0x09,0x33,    //    Usage ET Sawtooth Up
    0x09,0x34,    //    Usage ET Sawtooth Down
    0x09,0x40,    //    Usage ET Spring
    0x09,0x41,    //    Usage ET Damper
    0x09,0x42,    //    Usage ET Inertia
    0x09,0x43,    //    Usage ET Friction
    0x09,0x28,    //    Usage ET Custom Force Data
    0x25,0x0C,    //    Logical Maximum Ch (12d)
    0x15,0x01,    //    Logical Minimum 1
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x0C,    //    Physical Maximum Ch (12d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0xB1,0x00,    //    Feature
    0xC0     ,    // End Collection
    0x05,0x01,         //    Usage Page Generic Desktop
    0x09,0x3B,         //    Usage Byte Count
    0x15,0x00,         //    Logical Minimum 0
    0x26,0xFF,0x01,    //    Logical Maximum 1FFh (511d)
    0x35,0x00,         //    Physical Minimum 0
    0x46,0xFF,0x01,    //    Physical Maximum 1FFh (511d)
    0x75,0x0A,         //    Report Size Ah (10d)
    0x95,0x01,         //    Report Count 1
    0xB1,0x02,         //    Feature (Variable)
    0x75,0x06,         //    Report Size 6
    0xB1,0x01,         //    Feature (Constant)
    0xC0     ,    //    End Collection
    0x05,0x0F,    //    Usage Page Physical Interface
    0x09,0x89,    //    Usage Block Load Status (PID Block Load Report)
    0xA1,0x02,    //    Collection Datalink
    0x85, 0x20,    //    Report ID 20h (32d)
    0x09,0x22,    //    Usage Effect Block Index
    0x25,0x28,    //    Logical Maximum 28h (40d)
    0x15,0x01,    //    Logical Minimum 1
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x28,    //    Physical Maximum 28h (40d)
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0xB1,0x02,    //    Feature (Variable)
    0x09,0x8B,    //    Usage Block Load Full (Block Load Status)
    0xA1,0x02,    //    Collection Datalink
    0x09,0x8C,    //    Usage Block Load Error
    0x09,0x8D,    //    Usage Block Handle
    0x09,0x8E,    //    Usage PID Block Free Report
    0x25,0x03,    //    Logical Maximum 3
    0x15,0x01,    //    Logical Minimum 1
    0x35,0x01,    //    Physical Minimum 1
    0x45,0x03,    //    Physical Maximum 3
    0x75,0x08,    //    Report Size 8
    0x95,0x01,    //    Report Count 1
    0xB1,0x00,    //    Feature
    0xC0     ,                   // End Collection
    0x09,0xAC,                   //    Usage Undefined
    0x15,0x00,                   //    Logical Minimum 0
    0x27,0xFF,0xFF,0x00,0x00,    //    Logical Maximum FFFFh (65535d)
    0x35,0x00,                   //    Physical Minimum 0
    0x47,0xFF,0xFF,0x00,0x00,    //    Physical Maximum FFFFh (65535d)
    0x75,0x10,                   //    Report Size 10h (16d)
    0x95,0x01,                   //    Report Count 1
    0xB1,0x00,                   //    Feature
    0xC0     ,    //    End Collection
    0x09,0x7F,    //    Usage ROM Pool Size (PID Pool Report?)
    0xA1,0x02,    //    Collection Datalink
    0x85,0x03,                   //    Report ID 3
    0x09,0x80,                   //    Usage ROM Effect Block Count (RAM Pool Size?)
    0x75,0x10,                   //    Report Size 10h (16d)
    0x95,0x01,                   //    Report Count 1
    0x15,0x00,                   //    Logical Minimum 0
    0x35,0x00,                   //    Physical Minimum 0
    0x27,0xFF,0xFF,0x00,0x00,    //    Logical Maximum FFFFh (65535d)
    0x47,0xFF,0xFF,0x00,0x00,    //    Physical Maximum FFFFh (65535d)
    0xB1,0x02,                   //    Feature (Variable)
    0x09,0x83,                   //    Usage PID Pool Move Report (Simultaneous Effects Max?)
    0x26,0xFF,0x00,              //    Logical Maximum FFh (255d)
    0x46,0xFF,0x00,              //    Physical Maximum FFh (255d)
    0x75,0x08,                   //    Report Size 8
    0x95,0x01,                   //    Report Count 1
    0xB1,0x02,                   //    Feature (Variable)
    0x09,0xA9,                   //    Usage Undefined (Device Managed Pool?)
    0x09,0xAA,                   //    Usage Undefined (Shared Parameter Blocks?)
    0x75,0x01,                   //    Report Size 1
    0x95,0x02,                   //    Report Count 2
    0x15,0x00,                   //    Logical Minimum 0
    0x25,0x01,                   //    Logical Maximum 1
    0x35,0x00,                   //    Physical Minimum 0
    0x45,0x01,                   //    Physical Maximum 1
    0xB1,0x02,                   //    Feature (Variable)
    0x75,0x06,                   //    Report Size 6
    0x95,0x01,                   //    Report Count 1
    0xB1,0x03,                   //    Feature (Constant, Variable)
    0xC0,    //    End Collection

        // ====== End of virtual PID force feedback ======= //

    0xC0,               /*  End Collection,                     */

    0x05, 0x0C,         /*  Usage Page (Consumer),              */
    0x09, 0x01,         /*  Usage (Consumer Control),           */  // Virtual consumer control device
    0xA1, 0x01,         /*  Collection (Application),           */
    0x85, 0x1E,         /*      Report ID (30),                 */
    0x15, 0x00,         /*      Logical Minimum (0),            */
    0x25, 0x01,         /*      Logical Maximum (1),            */
    0x75, 0x03,         /*      Report Size (3),                */
    0x95, 0x01,         /*      Report Count (1),               */
    0x81, 0x03,         /*      Input (Constant, Variable),     */
    0x75, 0x01,         /*      Report Size (1),                */
    0x95, 0x02,         /*      Report Count (2),               */
    //0x09, 0xE2,         /*      Usage (Mute),                   */
    0x09, 0xE9,         /*      Usage (Volume Inc),             */
    0x09, 0xEA,         /*      Usage (Volume Dec),             */
    //0x09, 0x30,         /*      Usage (Power),                  */
    //0x0A, 0x24, 0x02,   /*      Usage (AC Back),                */
    //0x0A, 0x23, 0x02,   /*      Usage (AC Home),                */
    0x81, 0x02,         /*      Input (Variable),               */
    0x75, 0x03,         /*      Report Size (3),                */
    0x95, 0x01,         /*      Report Count (1),               */
    0x81, 0x03,         /*      Input (Constant, Variable),     */
    0xA1, 0x01,         /*      Collection (Application),       */
        0x19, 0x01,         /*          Usage Minimum (01h),        */ // HACK: Without this deliberately DirectInput-incompatible collection the customer control device would get detected as a gamepad, go figure..
        0x29, 0x03,         /*          Usage Maximum (03h),        */
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
    0x35, 0x81,                   //    Physical Minimum -127
    0x45, 0x7F,                   //    Physical Maximum 127
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
    WDFDEVICE           device;
    PDEVICE_EXTENSION   devContext = NULL;

    device = WdfIoTargetGetDevice(Target);
    devContext = GetDeviceContext(device);

    USHORT UrbFunction = (USHORT)(ptrdiff_t)Context;

    PURB pUrb = (PURB)IoGetCurrentIrpStackLocation(WdfRequestWdmGetIrp(Request))->Parameters.Others.Argument1;

    switch (UrbFunction) 
    {

    case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER:
    {
        struct _URB_BULK_OR_INTERRUPT_TRANSFER *req = (struct _URB_BULK_OR_INTERRUPT_TRANSFER *)pUrb;

        PUCHAR buf = (PUCHAR)USBPcapURBGetBufferPointer(req->TransferBufferLength,
            req->TransferBuffer, req->TransferBufferMDL);

        if (req->TransferBufferLength != 16)
            break;

        if (buf[0] == 0x01) {
            // Mirror consumer control buttons in the consumer control virtual device, because the HID game controller client driver
            // doesn't know how to handle them (while Linux has no problem picking them up).
            UCHAR ccState = buf[2] & 0x18;

            if (devContext->lastCCState != ccState) {
                buf[0] = 0x1E; // 30
                buf[1] = devContext->lastCCState = ccState;
                req->TransferBufferLength = 2;
            }
        } else if (buf[0] == 0x02) {
            // Tweak trackpad interrupts
            UCHAR x = buf[2];
            UCHAR y = buf[4];

            SHORT* diffX = (SHORT*) &buf[2];
            SHORT* diffY = (SHORT*) &buf[4];

            if (buf[1] & 0x08) {
                if (devContext->isTrackpadPressed) {
                    SHORT sqrX = (SHORT)x - (SHORT)devContext->origX;
                    SHORT sqrY = ((SHORT)y - (SHORT)devContext->origY) * 2;

                    *diffX = (sqrX > 0 ? sqrX : -sqrX) * sqrX;
                    *diffY = (sqrY > 0 ? sqrY : -sqrY) * sqrY;
                }
                else {
                    devContext->isTrackpadPressed = TRUE;
                    *diffX = 0;
                    *diffY = 0;
                    //KeQuerySystemTime(&devContext->firstTrackpadPress);
                }
                devContext->origX = x;
                devContext->origY = y;
            }
            else {
                devContext->isTrackpadPressed = FALSE;
                *diffX = 0;
                *diffY = 0;

                //LARGE_INTEGER lastTrackpadPress;
                //KeQuerySystemTime(&lastTrackpadPress);

                //if ((lastTrackpadPress.QuadPart - devContext->firstTrackpadPress.QuadPart) < 2500000) // 0.25s
                //    buf[1] |= 1;
            }
        }

        break;
    }

    case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE:
    case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE:
    {
        if (pUrb->UrbHeader.Function == URB_FUNCTION_CONTROL_TRANSFER)
        {
            struct _URB_BULK_OR_INTERRUPT_TRANSFER* pTransfer = (struct _URB_BULK_OR_INTERRUPT_TRANSFER*) pUrb;

            DbgPrint("pDescriptorRequest->TransferBufferLength = %d\n", pTransfer->TransferBufferLength);

            if (pTransfer->TransferBufferLength == 34) { // HID Descriptor
                PUCHAR buf = (PUCHAR)USBPcapURBGetBufferPointer(pTransfer->TransferBufferLength,
                    pTransfer->TransferBuffer, pTransfer->TransferBufferMDL);

                ASSERT(sizeof(G_DefaultReportDescriptor) < 65536);
                buf[26] = (UCHAR)(sizeof(G_DefaultReportDescriptor) >> 8); // already translated to big endian by the bus driver (FIXME little endian)
                buf[25] = (UCHAR)(sizeof(G_DefaultReportDescriptor) & 0xFF);
            }
            else if (pTransfer->TransferBufferLength == 241) { // HID Report Descriptor
                                                               // NOTE: Reallocating TransferBuffer is useless because it's a pointer provided by the upper driver.
                                                               // But since we reported a sizeof(G_DefaultReportDescriptor) length, it should be allocated the right size.
                                                               // Only the lower USB driver set TransferBufferLength back to 241, the original Report Descriptor size, which can be misleading.
                pTransfer->TransferBufferLength = sizeof(G_DefaultReportDescriptor);

                PUCHAR buf = (PUCHAR)USBPcapURBGetBufferPointer(pTransfer->TransferBufferLength,
                    pTransfer->TransferBuffer, pTransfer->TransferBufferMDL);

                RtlCopyMemory(&buf[0], &G_DefaultReportDescriptor[0], sizeof(G_DefaultReportDescriptor));
            }
        }
    }

    default:
        break;
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

typedef struct _URBBACKUP
{
    USHORT OldValue;

    ULONG OldTransferBufferLength;
    PVOID OldTransferBuffer;
    PMDL OldTransferBufferMDL;
} URBBACKUP, *PURBBACKUP;

VOID
NvShieldIoTranslationComplete(
    IN WDFREQUEST Request,
    IN WDFIOTARGET Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS Params,
    IN WDFCONTEXT Context
)
{
    UNREFERENCED_PARAMETER(Target);

    PURBBACKUP urbBackup = (PURBBACKUP)Context;

    PURB pUrb = (PURB)IoGetCurrentIrpStackLocation(WdfRequestWdmGetIrp(Request))->Parameters.Others.Argument1;
    struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST *req = (struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST *)pUrb;

    ExFreePool(req->TransferBuffer);

    req->Value = urbBackup->OldValue;
    req->TransferBuffer = urbBackup->OldTransferBuffer;
    req->TransferBufferMDL = urbBackup->OldTransferBufferMDL;
    req->TransferBufferLength = urbBackup->OldTransferBufferLength;

    ExFreePool(urbBackup);

    WdfRequestComplete(Request, Params->IoStatus.Status);
}

static NTSTATUS
updateRumble(
    IN WDFREQUEST   Request,
    PDEVICE_EXTENSION   devContext,
    struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST *req
)
{
    NTSTATUS status = STATUS_SUCCESS;

    unsigned short leftRumble = devContext->isRumbling ? devContext->leftRumbleStrength : 0;
    unsigned short rightRumble = devContext->isRumbling ? devContext->rightRumbleStrength : 0;

    if (leftRumble == 0 && rightRumble == 0) {
        if (devContext->isZeroRumble) {
            // only translate to a zero output report once
            WdfRequestComplete(Request, status);
            return status;
        }

        devContext->isZeroRumble = TRUE;
    } else
        devContext->isZeroRumble = FALSE;

    const unsigned outputReportSize = 7;

    PUCHAR tBuf = (PUCHAR)ExAllocatePoolWithTag(
        NonPagedPool,
        outputReportSize,
        (ULONG)((((ULONG)'N') << 16) + (((ULONG)'V') << 8) + 'T')
    );

    tBuf[0] = 0x01; // Report ID
    tBuf[1] = leftRumble & 0xFF;
    tBuf[2] = (leftRumble >> 8) & 0xFF;
    tBuf[3] = rightRumble & 0xFF;
    tBuf[4] = (rightRumble >> 8) & 0xFF;
    tBuf[5] = 0;
    tBuf[6] = 0;

    PURBBACKUP urbBackup = (PURBBACKUP)ExAllocatePoolWithTag(
        NonPagedPool,
        sizeof(URBBACKUP),
        (ULONG)((((ULONG)'N') << 16) + (((ULONG)'V') << 8) + 'B')
    );

    urbBackup->OldValue = req->Value;
    urbBackup->OldTransferBuffer = req->TransferBuffer;
    urbBackup->OldTransferBufferMDL = req->TransferBufferMDL;
    urbBackup->OldTransferBufferLength = req->TransferBufferLength;

    // Hack the urb before forwarding it
    req->Value = 0x0201;
    req->TransferBuffer = tBuf;
    req->TransferBufferMDL = NULL;
    req->TransferBufferLength = outputReportSize;

    WdfRequestFormatRequestUsingCurrentType(Request);

    WdfRequestSetCompletionRoutine(Request,
        NvShieldIoTranslationComplete,
        (WDFCONTEXT)urbBackup);

    if (!WdfRequestSend(Request, devContext->TargetToSendRequestsTo, NULL)) {
        status = WdfRequestGetStatus(Request);
        WdfRequestComplete(Request, status);
    }

    return status;
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

        case URB_FUNCTION_CLASS_INTERFACE:
        {
            struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST *req = (struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST *)pUrb;

            if (req->Request == 0x01 /*GET_REPORT*/)
            {
                if (req->Value == 0x0303)
                {
                    PUCHAR buf = (PUCHAR)USBPcapURBGetBufferPointer(req->TransferBufferLength,
                        req->TransferBuffer, req->TransferBufferMDL);
                    buf[0] = 0x03; // Report ID
                    buf[1] = 1;
                    buf[2] = 1; // doesn't seem rational to me, but that's what gc_n64_usb does
                    buf[3] = 0xff;
                    buf[4] = 1;
                    WdfRequestComplete(Request, STATUS_SUCCESS);
                    return;
                }
                else if (req->Value == 0x0320) // Block Load Status
                {
                    PUCHAR buf = (PUCHAR)USBPcapURBGetBufferPointer(req->TransferBufferLength,
                        req->TransferBuffer, req->TransferBufferMDL);
                    buf[0] = 0x20; // Report ID
                    buf[1] = 0x1;
                    buf[2] = 0x1;
                    buf[3] = 10;
                    buf[4] = 10;
                    WdfRequestComplete(Request, STATUS_SUCCESS);
                    return;
                }
            }
            else if (req->Request == 0x09 /*SET_REPORT*/) 
            {
                PUCHAR buf = (PUCHAR)USBPcapURBGetBufferPointer(req->TransferBufferLength,
                    req->TransferBuffer, req->TransferBufferMDL);

                int completeReq = FALSE;

                if (req->Value == 0x020C)
                {
                    switch (buf[1]) {
                        case 0x02: // Disable actuators
                        case 0x03: // Stop all effects
                        case 0x04: // Device reset
                            devContext->actuatorSel = 1;
                            devContext->isRumbling = FALSE;
                            status = updateRumble(Request, devContext, req);
                            return;
                        default:
                            break;
                    }
                }
                else if (req->Value == 0x020D) // Device gain
                {
                    devContext->rumbleGain = (USHORT)buf[1];
                    status = updateRumble(Request, devContext, req);
                    return;
                }
                else if (req->Value == 0x0309) // Set effect type (TODO do what gc_n64_usb does?)
                {
                    completeReq = TRUE;
                }
                else if (req->Value == 0x0205) // Set constant force
                {
                    LONG actSel = InterlockedXor(&devContext->actuatorSel, 1);

                    short force = (short)buf[2] + ((short)buf[3] << 8);
                    if (force < 0)
                        force = -force;

                    if (actSel == 1) {
                        devContext->leftRumbleStrength = force * devContext->rumbleGain;
                        completeReq = TRUE; 
                            // We only do one translation per two set constant force reports or the right URB 
                            // may be handled by the USB bus driver before the left URB, resulting in setting 
                            // incorrect rumble values.
                            // This is especially felt when the rumble is supposed to be set to zero, and isn't.
                    }
                    else {
                        devContext->rightRumbleStrength = force * devContext->rumbleGain;
                        status = updateRumble(Request, devContext, req);
                        return;
                    }
                }
                else if (req->Value == 0x0221) // Set effect
                {
                    completeReq = TRUE;
                }
                else if (req->Value == 0x020A) // Effect operation
                {
                    /* Byte 0 : report ID
                    * Byte 1 : bit 7=rom flag, bits 6-0=effect block index
                    * Byte 2 : Effect operation
                    * Byte 3 : Loop count */
                    //_loop_count = data[3] << 3;

#define EFFECT_OP_START			1
#define EFFECT_OP_START_SOLO	2
#define EFFECT_OP_STOP			3

                    switch (buf[1] & 0x7F) // Effect block index
                    {
                    case 1: // constant force
                    case 3: // square
                    case 4: // sine
                        switch (buf[2]) // effect operation
                        {
                        case EFFECT_OP_START:
                        case EFFECT_OP_START_SOLO:
                        case EFFECT_OP_STOP:
                            devContext->isRumbling = (buf[2] != EFFECT_OP_STOP);
                            status = updateRumble(Request, devContext, req);
                            return;
                            
                        }
                        break;

                    case 2: // ramp
                    case 5: // triangle
                    case 6: // sawtooth up
                    case 7: // sawtooth down
                    case 8: // spring
                    case 9: // damper
                    case 10: // inertia
                    case 11: // friction
                    case 12: // custom force data
                        break;
                    }

                    completeReq = TRUE;
                }
                else if (req->Value == 0x020B) // PID Block Free Report/Effect Block Index
                {
                    completeReq = TRUE;
                }

                if (completeReq) {
                    WdfRequestComplete(Request, STATUS_SUCCESS);
                    return;
                }
            }
        }

        case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE:
        case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE:
        case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER:
            {
                WdfRequestFormatRequestUsingCurrentType(Request);

                WdfRequestSetCompletionRoutine(Request,
                    NvShieldIoInternalDeviceControlComplete,
                    (WDFCONTEXT)pUrb->UrbHeader.Function);

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


