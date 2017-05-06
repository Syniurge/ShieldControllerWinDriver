# NVIDIA Shield Controller Windows driver
This small USB filter driver intercepts and tweaks the HID Report Descriptor to make DirectInput detect it as a gamepad. It also emulates a force feedback device for rumble support in both DirectInput and Xinput games, tweaks the input data of the trackpad to make it usable, and adds support for the volume increment/decrement buttons.

NVIDIA previously released a driver that was bundled with GeForce Experience and only usable by NVIDIA graphics card users, and also suffered from a variety of issues according to forum discussions. Excluding AMD and Intel graphics card owners has made a lot of people extremely displeased. I've bought Tegra hardware (nVidia Shield tablet and Jetson) and used to applaud their open source efforts, but screwing their game controller buyers like this makes me regret my decision.

## What was the issue?
The controller is a HID-compliant game controller, it's supported out-of-the-box on Linux and applications accessing it through raw HID. So theoretically it should be supported by generic Windows drivers, but it wasn't being detected by DirectInput.

By playing with the `vhidmini` driver from the DDK which provides a virtual HID device, I managed to find what prevented the detection inside the HID Report Descriptor:

```cpp
    0x09, 0x34,         /*          Usage (Ry),                */
    0x81, 0x02,         /*          Input (Variable),           */
    0xC0,               /*      End Collection,                 */
    //0xA1, 0x01,         /*      Collection (Application),       */ // <===== The root cause was this "output collection" part of the same report
    //0x19, 0x01,         /*          Usage Minimum (01h),        */
    //0x29, 0x03,         /*          Usage Maximum (03h),        */
    //0x26, 0xFF, 0xFF,   /*          Logical Maximum (-1),       */
    //0x95, 0x03,         /*          Report Count (3),           */
    //0x75, 0x10,         /*          Report Size (16),           */
    //0x91, 0x02,         /*          Output (Variable),          */
    //0xC0,               /*      End Collection,                 */
    0xC0,               /*  End Collection,                     */
    0x05, 0x01,         /*  Usage Page (Desktop),               */
    0x09, 0x02,         /*  Usage (Mouse),                      */
```

Commenting those lines made the virtual device show up in the game controller applet of the configuration panel.

So based on this finding a small lower filter driver under HidUsb was written to modify the descriptor reported to HidUsb, changing "Usage Minimum" and "Usage Maximum" (which are the actual lines preventing the detection) to "Usage". The triggers were still not being detected by DirectInput, so another tweak provided by the filter driver is to change their "HID usage" from Accelerator and Brake axis to Rx and Ry axis.

To support rumble in any game, old and new (while GeForce Experience only supports Xinput games), emulation of a HID Physical Input Device (PID) was added. The hack could be replicated for other controllers that don't bother with PID which is a way too complicated standard for basic gamepad rumble.

Finally, the trackpad input gets tweaked to work like a standard trackpad, and because the HID gamepad client driver doesn't handle volume inc/dec buttons (while Linux picks them up without flinching), a virtual HID consumer control device was added that receives the input from those two buttons. Ironically that device was detected as a gamepad (and poor DirectInput has trouble when two different gamepads have the same IDs), so the above output collection was inserted to get rid of DirectInput.

Making this driver was helped tremendously by `usbhid-dump`, `hidrd-convert`, UsbLyzer, Wireshark, the `gc_n64_usb` firmware source code, and the vague yet helpful instructions that someone who managed to change a USB descriptor gave on the ntdev mailing-list.

## Binaries (Windows 7 and later)
 [Download latest release](../../releases/latest).

## Installation
To install the driver right-click on the .inf file and select `Install`.

Disconnect and reconnect the controller as switching drivers sometimes causes problems. It should now be detected as a DirectInput gamepad, in games, x360ce, etc.
