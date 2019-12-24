# heartbeat-trigger
Simple heartbeat triggers to visually monitoring the system load (CPU and RAM)

Written in pure C, fast and simple! Ideal for easy monitoring of system load! Only for Windows OS (for now!).
The project is inspired by the functionality of the Linux Kernel commonly called **"Led heartbeat trigger"** (https://www.kernel.org/doc/Documentation/devicetree/bindings/leds/common.txt), active in many embedded and single-board computers with a led connected to a GPIO port (such as RapsberryPI, Beaglebone, Acme Systems boards, ...). https://www.youtube.com/watch?v=9Z4ZTovtFKk for an example on Beaglebone. https://www.youtube.com/watch?v=1mVWwjhSKhU for an example in Acme Systems Acqua A5. Unfortunately non-embedded system (Servers, PC, ...) usually don't have a led which can be used for this purpose (and they may not use Linux).

This program generate independently an heartbeat pulse with frequency proportional to system load and redirect it to some "visible" hardware indicators.
You can choise to monitoring only CPU load, only RAM load or "mixed" CPU and RAM load ("c", "r" and "m" parameters respectively). Also you can set a lower-bound limit to activate the heartbeat only if the system is overload ("-l" parameter).
The pulse is redirected to an RS232 Serial Port or to the Keyboard Indicator Leds.

Set "--help" parameter to show all info for the two versions of the program.

## RS232 Serial Port ("COM" on Windows, "ttyS" on Linux) - Pin DTR ##
A "pulse" corresponds to a high level on pin DTR (Data Terminal Ready, pin 4 on DB9 connector). So, you can connect a led between DTR and GND (pin 5 on DB9 connector) to visualize the heartbeat. Don't forget to insert a resistor suitable for about 9-12V (the actual voltage depends on the motherboard).

WARNING: RS232 protocol uses negative voltages. This is not a problem for the leds but if you decide to use other circuits, be careful! Furthermore, you will not be able to use the serial port for other purposes (or maybe yes in some situations...)

You can choise the serial port number with "-p" parameter.

Source code: *"heartbeat_win_serial.c"* (requires *"heartbeat_win_loads.c"*).


## Keyboard Indicator Leds (CAPS LOCK, NUM LOCK and/or SCROLL LOCK) ##

A "pulse" turn on or off one or more keyboard indicator leds. You can choise the indicators with "-k" parameter and "octal notation" like Linux "chmod" command. If you don't know what I'm talking about don't worry: use the "--help" parameter.

IMPORTANT NOTE: The software accesses the keyboard at low level and interact only with the led (it does not simulate keystrokes!).

Source code: *"heartbeat_win_keyboard.c"* (requires *"heartbeat_win_loads.c"*).


## Testing

### Serial Port Testing

:heavy_check_mark: Dell Optiplex 990, Integrated COM Port (RS232 DB-9), Windows 7

:question: MSI MS-7529, Windows 7

### Keyboard Testing:

:heavy_check_mark: Dell Latitude E6220, Integrated Keyboard, Windows 10

:heavy_check_mark: Dell Optiplex 990, PS2 Logitech Keyboard, Windows 7

:question: MSI MS-7529, Windows 7

## To Do list:
- [x] Serial Port - Windows
- [ ] Serial Port - Linux
- [X] Keyboard Led - Windows
- [ ] Keyboard Led - Linux
- [ ] Disk Usage (???)
- [ ] Speaker sound (???)
