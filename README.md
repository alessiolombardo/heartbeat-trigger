# heartbeat-trigger
Simple heartbeat triggers to visually monitoring the system load (CPU and RAM)

Written in pure C, tiny, fast and simple! For Windows and Linux.

## System Requirements ##
 - **RAM**: <400KB
 - **Disk**: <25KB
 - **Operating System**: Windows NT (tested on Windows XP, Windows 7, Windows 10), Linux (tested on Ubuntu)

## Description ##

The project is inspired by the functionality of the Linux Kernel commonly called [**Led heartbeat trigger**](https://www.kernel.org/doc/Documentation/devicetree/bindings/leds/common.yaml), active in many embedded and single-board computers with a led connected to a GPIO port (such as RapsberryPI, Beaglebone, Acme Systems boards, ...). [See this example](https://www.youtube.com/watch?v=9Z4ZTovtFKk) on Beaglebone. [See this example](https://www.youtube.com/watch?v=1mVWwjhSKhU) on Acme Systems Acqua A5. Unfortunately non-embedded system (Servers, PC, ...) usually don't have a led which can be used for this purpose (and they may not use Linux).

This program generate independently an heartbeat pulse with frequency proportional to system load and redirect it to some "visible" or "audible" hardware indicators.
You can choise to monitoring only **CPU load**, only **RAM load** or **"mixed"** CPU and RAM load ("c", "r" and "m" arguments respectively). Also you can set a lower-bound limit to activate the heartbeat only if the system is overload ("-l" argument).
The pulse is redirected to an RS232 Serial Port, to the Keyboard Indicator Leds and/or to the PC/Console Speaker.

Set "\-\-help" argument to show all info for the three heartbeat modes.

### KEYBOARD Mode ("-k") ###

A "pulse" turn on or off one or more **keyboard indicator leds** (CAPS LOCK, NUM LOCK and/or SCROLL LOCK). You can choise the indicators with "-k" argument and "octal notation" like Linux "chmod" command. If you don't know what I'm talking about don't worry: use "\-\-help" argument. To disable this mode choise "-nk" argument. This mode is active by default.

IMPORTANT NOTE: The software accesses the keyboard at low level and interact only with the led (it does not simulate keystrokes!).

IMPORTANT NOTE: Linux version require root priviledges.

KNOWN BUG: WINDOWS VERSION DOES NOT WORK WITH USB KEYBOARD!

### SERIAL Mode ("-s") ###

This mode interact with a **RS232 Serial Port** ("COM" port on Windows, "ttyS" on Linux).
A "pulse" corresponds to a high level on pin DTR (Data Terminal Ready, pin 4 on DB9 connector). So, you can connect a led between DTR and GND (pin 5 on DB9 connector) to visualize the heartbeat. Don't forget to insert a resistor suitable for about 9-12V (the actual voltage depends on the motherboard).

You can choise the serial port number with "-s" argument. To disable this mode choise "-ns" argument. This mode is active by default.

WARNING: RS232 protocol uses negative voltages. This is not a problem for the leds but if you decide to use other circuits, be careful! Furthermore, you will not be able to use the serial port for other purposes (or maybe yes in some situations...)

IMPORTANT NOTE: Linux version require root priviledges.

### BEEP Mode ("-b") ###

A "pulse" generate a console beep by printing the "BELL" char (0x07 aka "^G" in Windows or "\a" in Linux). This mode is not enabled by default. To enable it choise "-b" argument.

IMPORTANT NOTE: The sound is not necessarily emitted by the PC Speaker but it depends on the audio device used by the system console.

## Testing ##

### KEYBOARD Mode Testing ###

:heavy_check_mark: Dell Latitude E6220, Integrated Keyboard, Windows 10

:heavy_check_mark: Dell Latitude E6220, Integrated Keyboard, Ubuntu 18.04

:heavy_check_mark: Dell Optiplex 990, PS2 Logitech Keyboard, Windows 7

:heavy_check_mark: Dell Optiplex 990, PS2 Logitech Keyboard, Ubuntu 18.04

:heavy_check_mark: HP Elitedesk 800 G1 SFF, PS2 Logitech Keyboard, Windows 7

:heavy_check_mark: HP Elitedesk 800 G1 SFF, PS2 Logitech Keyboard, Ubuntu 20.04

:heavy_check_mark: HP EliteBook 820 G2, Integrated Keyboard, Windows 10

:x: MSI MS-7529, USB Dell Keyboard, Windows 7

:heavy_check_mark: MSI MS-7529, PS2 IBM MODEL M 1391404 (very old) Keyboard, Windows 7

:heavy_check_mark: MSI MS-7529, PS2 Kraun Keyboard, Windows 7

:heavy_check_mark: MSI MS-7529, PS2 Kraun Keyboard, Windows XP

:heavy_check_mark: MSI MS-7529, PS2 Kraun Keyboard, Ubuntu 15.04

:heavy_check_mark: Asus Motherboard, PS2 Kraun Keyboard, Windows XP

### SERIAL Mode Testing  ###

:heavy_check_mark: Dell Optiplex 990, Integrated COM Port (RS232 DB-9), Windows 7

:heavy_check_mark: Dell Optiplex 990, Integrated COM Port (RS232 DB-9), Ubuntu 18.04

:heavy_check_mark: HP Elitedesk 800 G1 SFF, Integrated COM Port (RS232 DB-9), Windows 7

:heavy_check_mark: HP Elitedesk 800 G1 SFF, Integrated COM Port (RS232 DB-9), Ubuntu 20.04

:heavy_check_mark: MSI MS-7529, Integrated COM Port (RS232 DB-9), Windows 7

:heavy_check_mark: Asus Motherboard, Integrated COM Port (RS232 DB-9), Windows XP

### BEEP Mode Testing  ###

:heavy_check_mark: HP Elitedesk 800 G1 SFF, Integrated Sound Card, Windows 7

:heavy_check_mark: HP Elitedesk 800 G1 SFF, Integrated Sound Card, Ubuntu 20.04

:heavy_check_mark: MSI MS-7529, Integrated Sound Card + PC Speaker, Windows 7 (Integrated Sound Card only)

:heavy_check_mark: MSI MS-7529, Integrated Sound Card + PC Speaker, Windows XP (PC Speaker only)

:x: MSI MS-7529, Integrated Sound Card + PC Speaker, Ubuntu 15.04

:x: HP EliteBook 820 G2, Integrated Sound Card, Windows 10
