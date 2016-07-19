NVIDIA Shield Controller Windows driver
=======================
This small USB filter driver intercepts and tweaks the HID Report Descriptor to make DirectInput detect it as a gamepad.

NVIDIA previously released a driver that was bundled with GeForce Experience and only usable by NVIDIA graphics card users, and also suffered from a variety of issues according to forum discussions. Excluding AMD and Intel graphics card owners has made a lot of people extremely displeased. I've bought Tegra hardware (nVidia Shield tablet and Jetson) and used to applaud their open source efforts, but screwing their game controller buyers like this makes me regret my decision.

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

So based on this finding a small lower filter driver under HidUsb was written to modify the descriptor reported to HidUsb in order to tweak that collection, simply changing "Usage Minimum" and "Usage Maximum" (which are the actual lines preventing the detection) to "Usage". The triggers were still not being detected by DirectInput, so another tweak provided by the filter driver is to change their "HID usage" from Accelerator and Brake axis to Rx and Ry axis.

Making this driver was helped tremendously by `usbhid-dump`, `hidrd-convert`, Wireshark, and the vague yet helpful instructions that someone who managed to change a USB descriptor gave on the ntdev mailing-list.

Binaries
--------------
 * 64 bits: http://homo-nebulus.fr/shieldcontrollerusbfilter/ShieldControllerDriver_64.zip
 * 32 bits: http://homo-nebulus.fr/shieldcontrollerusbfilter/ShieldControllerDriver_32.zip

Installation and signing issue
--------------

The driver isn't signed (signing a driver would cost me $260), and on recent Windows versions it's not possible to permanently disable signing checks, so for example **on Windows 10** you first have to do the following:

> 1. Click the Start Start menu and select Settings.
> 2. Click Update and Security.
> 3. Click on Recovery.
> 4. Click Restart now under Advanced Startup.
> 5. Click Troubleshoot.
> 6. Click Advanced options.
> 7. Click Startup Settings.
> 8. Click on Restart.
> 9. On the Startup Settings screen press F7 to disable driver signature enforcement.

**On Windows 8**: https://learn.sparkfun.com/tutorials/disabling-driver-signature-on-windows-8/disabling-signed-driver-enforcement-on-windows-8

**This special startup has to be done everytime you want to use the controller (as long as the driver remains unsigned). Sorry about that.**

Once the signing check is disabled you can install the driver by right-clicking on the .inf file and selecting `Install`.

The generic driver still takes precedence over unsigned drivers, so you now have to manually select the driver for your Shield controller in the device manager. First display devices by connection, so you can find your controller easily:

![alt text](https://github.com/Syniurge/ShieldControllerWinDriver/blob/master/doc/DevMgrByConnection.png "DevMgrByConnection")
![alt text](https://github.com/Syniurge/ShieldControllerWinDriver/blob/master/doc/ShieldControllerPID.png "ShieldControllerPID")

Then select the **top** `USB input device` node and choose "Update the driver..", and then:

![alt text](https://github.com/Syniurge/ShieldControllerWinDriver/blob/master/doc/ShieldCtrlDriverStep1.png "ShieldCtrlDriverStep1")
![alt text](https://github.com/Syniurge/ShieldControllerWinDriver/blob/master/doc/ShieldCtrlDriverStep2.png "ShieldCtrlDriverStep2")
![alt text](https://github.com/Syniurge/ShieldControllerWinDriver/blob/master/doc/ShieldCtrlDriverStep3.png "ShieldCtrlDriverStep3")

Finally disconnect and reconnect the controller as switching drivers sometimes causes problems. It should now be detected as a DirectInput gamepad, in games, x360ce, etc.