Author: Karl-Heinz Kübbeler   (kh_kuebbeler@web.de) , Mai 2016, Jun 2020

This is a assembly language version of the Bootloader optiboot
with extendet features:

Write data to the Flash and also to the EEprom memory.

All modifications for a different program length are done automatically.
So you have not to worry about changed program length for selected special
functions or different target processor.
The fuses and the boot loader start address will be changed right without
your help, if you use the ISP=1 parameter with make to transfer the bootloader.

Can use a internal UART controller or use a software emulation for the serial communication.
With the softwre emulation you can free select any of the AVR pins.
The different access time of the selected port will be noticed correctly.

Can support a measurement of the transmitted serial data and select
automatically the right baud rate for the communication with the PC.

The bootloader can also be used with AVR processors, which did not support
bootloades with the hardware. In this case two interrupt vectors of 
every user program are changed. A program verify is usually possible, because
the modification data are saved in the last EEprom data.

With the UART software emulation you can also use only one pin for both,
the Tx and the Rx communication.
You need a special hardware interface to suppress the echo of the
transmission data of the PC (avrdude).
Details you can find in the PDF documentation (../Doku/english/bootloader.pdf).
The intermediate electronic is only required for the PC side for a correctly
running avrdude. The bootloader does not require any additional electronic.

For all supported processors the make call will build a suitable configured bootloader,
which is capable of running. You will see the selected parameters at the terminal.
You can change this default selection by a list of arguments with the make call.
You can see important Information with the building protocol at the screen
and also at the end of the .lst file.

Usually the build of the optiboot bootloader will run at a Linux system.
Together with a installed AVR toolchain and the Cygwin64 package you can also
use the Makefiles at a Windows10 system.
Please see for more information the help_files/Windows_usage.txt .

Optional you can use C-programs instead of the default assembler programs to
build a optiboot bootloader. With this selection the bootloader will
increase in size and will need more bootloader pages. But you can select
the most features also with the C-source. As a rule, however, the assembler
version is better tested and should to be favoured.

The short description Usage.txt and other usefull text files you can find
in the help_files subdirectory.


List of the supported AVR-Prozessors:
==========================
ATmega8/16/32/64/128
ATmega162/163
ATmega164/324/644/1284 (P)
ATmega48/88/168/328  (P)
ATmega165/325/3250/6450
ATmega169/329/649/6490
ATmega640/1280/1281/2560/2561
AT90CAN32/64/128
AT90PWM2/3
ATtiny84/841/861/4413/44/441/461/2313/24/85/45/88/48/1634
ATmega16u4/32u4
ATmega8u2/16u2/32u2
For some processors the finish test is missing!

I look forward for any replies to the "assembly language optiboot" subject.
