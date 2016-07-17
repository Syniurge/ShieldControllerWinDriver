NVIDIA Shield Controller Windows driver
=======================
This small USB filter driver intercepts and tweaks the reported HID Report Descriptor to make DirectInput detect it.

NVIDIA previously published a driver that was only usable if you had a NVIDIA graphics card, and also has issues on its own. Excluding AMD and Intel graphics card owners has made a lot of people extremely displeased. I've bought Tegra hardware (nVidia Shield tablet and Jetson) and used to applaud their open source efforts, but screwing their game controller buyers like this makes me regret my decision.

What was the issue?
--------------
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

The triggers were still not being detected by DirectInput, so the filter driver also changes their "HID usage" from Accelerator and Brake axis to Rx and Ry axis.

Making this driver was helped tremendously by `usbhid-dump`, `hidrd-convert`, Wireshark, and the vague yet helpful instructions that someone who managed to change a USB descriptor gave on the ntdev mailing-list.

Installation and signing issue
--------------
The driver isn't signed (signing a driver costs $260), and on recent Windows versions it's not possible to permanently disable signing checks, so for example on Windows 10 you first have to do the following:

> Click the Start Start menu and select Settings.
> Click Update and Security.
> Click on Recovery.
> Click Restart now under Advanced Startup.
> Click Troubleshoot.
> Click Advanced options.
> Click Startup Settings.
> Click on Restart.
> On the Startup Settings screen press F7 to disable driver signature enforcement.

**This special startup has to be done everytime you want to use the controller (as long as the driver is unsigned). Sorry about that.**

Once the signing check is disabled you can install the driver by right-click on the .inf file and selecting `Install`.

The generic driver still takes precedence over unsigned drivers, so you now have to manually select the driver for you Shield controller in the device manager:

