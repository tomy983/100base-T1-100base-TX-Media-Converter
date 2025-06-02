# 100base-T1-100base-TX-Media-Converter
PCB designed to convert automotive ethernet 100base-T1 to 100base-TX ethernet

This is a corrected version (the picture shows the previous revision). The correction consists of a missing resistor to set the TJA1101B MDIO address.

Another issue I had during bring up was the ferrite bead FB1, which I originally selected of incorrect value. (The TJA1101B was reporting low voltage on register 25 bits 15 and 14 (UV_VDDD_3V3 and UV_VDDA_3V3). I corrected it in the published schematic, but it is untested. On my PCB I've just shorted it.

I've tested 2 of this board with Unshielded and not twisted wire (ideally should be Twisted - UTP) of 8 meters length, reaching a speed of 85Mbps. No dropped packets were observed.
The polarity of the 2 wire ethernet doesn't really matter (the slave will correct accordingly).
After this successful test I've proceeded testing with my PandarQT 3D lidar (which only has 100base-T1 interface) successfully.

No MDIO setup is needed (the configuration is all done via strapping resistors). Just bridge the solder bridges to select autonomous mode and also master or slave as needed.
While not required, if you need MDIO interface to check / set registers, keep in mind that while in autonomous mode the MDIO interface of TJA1101B will not work. Just set it to managed.

Is included the very rough Arduino sketch that I've used to interface with MDIO (bit banging).

![alt text](images/picture.jpg)

![alt text](images/render.png)

Resources used for this project:

- https://www.nxp.com/downloads/en/schematics/RDDRONE-T1ADAPT-SCH.pdf
- https://github.com/NXPHoverGames/RDDRONE-T1ADAPT/blob/main/t1eth10Lpc/src/t1eth10Lpc.c
- https://github.com/ehntoo/100base-t1-converter
- https://www.nxp.com/document/guide/getting-started-with-the-rddrone-t1adapt-evaluation-board:GS-RDDRONE-T1ADAPT
- https://github.com/cioban/arduino-projects/blob/master/smi/smi.ino

