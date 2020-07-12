# heartbeat-trigger
Simple heartbeat triggers to visually monitoring the system load (CPU and RAM)

Written in pure C, tiny, fast and simple! Ideal for easy monitoring of system load! For Windows and Linux.

## System Requirements ##
RAM: <400KB

Disk: <25KB

Operating System: Windows NT (Tested only on Windows 7, Windows 10), Linux (Tested only on Ubuntu 18.04)


## Description ##

The project is inspired by the functionality of the Linux Kernel commonly called **"Led heartbeat trigger"** (https://www.kernel.org/doc/Documentation/devicetree/bindings/leds/common.txt), active in many embedded and single-board computers with a led connected to a GPIO port (such as RapsberryPI, Beaglebone, Acme Systems boards, ...). https://www.youtube.com/watch?v=9Z4ZTovtFKk for an example on Beaglebone. https://www.youtube.com/watch?v=1mVWwjhSKhU for an example on Acme Systems Acqua A5. Unfortunately non-embedded system (Servers, PC, ...) usually don't have a led which can be used for this purpose (and they may not use Linux).

This program generate independently an heartbeat pulse with frequency proportional to system load and redirect it to some "visible" hardware indicators.
You can choise to monitoring only CPU load, only RAM load or "mixed" CPU and RAM load ("c", "r" and "m" parameters respectively). Also you can set a lower-bound limit to activate the heartbeat only if the system is overload ("-l" parameter).
The pulse is redirected to an RS232 Serial Port or to the Keyboard Indicator Leds.

Set "--help" parameter to show all info for the two versions of the program.

### RS232 Serial Port ("COM" on Windows, "ttyS" on Linux) - Pin DTR ###
A "pulse" corresponds to a high level on pin DTR (Data Terminal Ready, pin 4 on DB9 connector). So, you can connect a led between DTR and GND (pin 5 on DB9 connector) to visualize the heartbeat. Don't forget to insert a resistor suitable for about 9-12V (the actual voltage depends on the motherboard).

WARNING: RS232 protocol uses negative voltages. This is not a problem for the leds but if you decide to use other circuits, be careful! Furthermore, you will not be able to use the serial port for other purposes (or maybe yes in some situations...)

You can choise the serial port number with "-p" parameter.

Source code for Windows: *"heartbeat_win_serial.c"* (requires *"heartbeat_win_loads.c"*).

Source code for Linux: *"heartbeat_linux_serial.c"* (requires *"heartbeat_linux_loads.c"*).

IMPORTANT NOTE: Linux version require root priviledges.

### Keyboard Indicator Leds (CAPS LOCK, NUM LOCK and/or SCROLL LOCK) ###

A "pulse" turn on or off one or more keyboard indicator leds. You can choise the indicators with "-k" parameter and "octal notation" like Linux "chmod" command. If you don't know what I'm talking about don't worry: use the "--help" parameter.

Source code for Windows: *"heartbeat_win_keyboard.c"* (requires *"heartbeat_win_loads.c"*).

Source code for Linux: *"heartbeat_linux_keyboard.c"* (requires *"heartbeat_linux_loads.c"*).

IMPORTANT NOTE: The software accesses the keyboard at low level and interact only with the led (it does not simulate keystrokes!).

IMPORTANT NOTE: Linux version require root priviledges.

KNOWN BUG: WINDOWS VERSION DOES NOT WORK WITH USB KEYBOARD!

## Testing

### RS232 Serial Port Testing

:heavy_check_mark: Dell Optiplex 990, Integrated COM Port (RS232 DB-9), Windows 7

:heavy_check_mark: Dell Optiplex 990, Integrated COM Port (RS232 DB-9), Ubuntu 18.04

:heavy_check_mark: MSI MS-7529, Integrated COM Port (RS232 DB-9), Windows 7

:heavy_check_mark: Asus Motherboard, Integrated COM Port (RS232 DB-9), Windows XP

### Keyboard Indicator Leds Testing:

:heavy_check_mark: Dell Latitude E6220, Integrated Keyboard, Windows 10

:heavy_check_mark: Dell Latitude E6220, Integrated Keyboard, Ubuntu 18.04

:heavy_check_mark: Dell Optiplex 990, PS2 Logitech Keyboard, Windows 7

:heavy_check_mark: Dell Optiplex 990, PS2 Logitech Keyboard, Ubuntu 18.04

:heavy_check_mark: HP EliteBook 820, Integrated Keyboard, Windows 10

:x: MSI MS-7529, USB Dell Keyboard, Windows 7

:heavy_check_mark: MSI MS-7529, PS2 IBM MODEL M 1391404 (very old) Keyboard, Windows 7

:heavy_check_mark: Asus Motherboard, PS2 Kraun Keyboard, Windows XP

## To Do list:
- [X] Serial Port - Windows
- [X] Keyboard Led - Windows
- [X] Serial Port - Linux
- [X] Keyboard Led - Linux
- [ ] Disk Usage
- [ ] Speaker sound
