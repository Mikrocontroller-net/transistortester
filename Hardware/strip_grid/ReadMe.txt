
You find a strip grid board design for the TansistorTester in the
file ttester_strip_grid.diy. The result you can find in the PDF file
TTester_strip.pdf .
The design was made with the software diy-layout-creator (DIYLC).
Because this program is written in Java language, it should run on 
different operating systems.
For further details take a look to the home page: http://code.google.com/p/diy-layout-creator

All details as automatic power off, the crystal and the precision voltage reference are integrated.
But the pin assignment of the ATmega was changed for this layout!
The needed program changes becomes active with the Makefile option STRIP_GRID_BOARD.
The program version with standard assignment can not run with this board !!
On the other side a program version for the strip grid board does not run
with a standard board!

You can cut out the needed size from a standard 100mm x 100mm strip grid board.
You should first drill the fixing holes and then solder the wire straps.
The strip line should be disconnected on all denoted places.
The disconnection points are out of raster at the ISP-plug and at the REF1
voltage reference!

The board layout is shown with the voltage regulator IC2 (78L05), a alternativ
layout for a low drop regulator as MCP1702-5002 is shown beside the printed board.
The pin layout of these regulators is not identical!
The resistor R1 (680 Ohm) is mounted under the ATmega socket and should be soldered
as one of the first parts. You should use a insolating tube to avoid a fault by short circuit.
The GND signal is routed to the ATmega from both sides and is additional shorted under the
socket.
The resistor R10 is the only one with vertical mounting.
If the potentiometer VR1 is covered with the LCD-display, the potentiometer can be
mounted to the soldering side. If VR1 is covered, you can adjust the voltage without
the LCD with a voltmeter or you can connect the LCD with cable for adjusting.

If all components are mounted without the LCD and the ATmega, you should check
the connections with a ohmmeter or a continuity checker.
The GND signal should have connections to the indended pins (ATmega, ISP and LCD).
Also the VCC signal should have connections only to the right pins.
After this check you can connect the battery and check the VCC voltage with the start button
pressed. If you find the correct 5V, you can assemble the ATmega and the LCD.

If the ATmega is not yet programmed, you have to program the microcontroller with the
on board ISP interface.

For methode of fault finding you should read the hints in the PDF documentation.

Good luck for the reproduction of the board!

Karl-Heinz
