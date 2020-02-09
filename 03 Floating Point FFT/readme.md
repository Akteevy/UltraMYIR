Floating Point FFT example
==========================
This projects shows how to use the FFT IP core for floating point calculations with AXI DMA and the PS.

![MYD-CZU3EG](http://www.myirtech.com/attached/image/20190702/myd-czu3egnofan.jpg "MYD-CZU3EG")

## Requirements
* Xilinx Vivado and SDK 2018.3. It can be adapted to other releases.
* MYIR MYC-CZU3EG or MYD-CZU3EG. It might work on other boards with Zynq Ultrascale+

## Description
An AXI DMA block is used to fetch data to and from the FFT IP core. Data transfer is controlled by the PS using interrupts. Bare metal (non OS) software is also provided. The FFT length can be adjustable in software.

## Build instructions

#### Hardware (Vivado)
1. Clone or download the repository content
2. (Optional) Copy the contents of the <b>Vivado</b> folder where you want the project to be built
3. Open Vivado 2018.3
4. On the TCL panel, go to the Vivado directory and source build_um03.tcl
5. After the project is built in Vivado, run Generate Bitstream
6. Click File > Export > Export Hardware
7. Click Launch SDK and leave the default options (local)

#### Software (SDK)
8. Click File > New > Application project
9. Name it FSBL and click Next
10. Choose Zynq MP FSBL and click Finish
11. Click File > New > Application Project
12. Name it App and click Next
13. Choose Empty Application and click Finish
14. Copy the content of the <b>SDK</b> folder in this repository to the App sources folder in SDK
15. In App Properties, add m to the linker libraries
16. In the App linker script, set the stack size to 0x4000 and heap size to 0x40000
17. Update SDK (F5) if needed
18. Build the software project.

#### Boot image (SDK)
19. Click Xilinx > Create Boot Image
20. If needed, change the BIF file name or folder with the Browse button to the right
21. If needed, change the Boot file output path with the Browse button to the right
22. On the lower panel add the following files in the following order. Use Add and Up and Down buttons:
    a. FSBL.elf,  partition: bootloader, destination: PS
    b. bitstream, partition: datafile,   destination: PL
    c. App.elf,   partition: datafile,   destination: PS
23. Click Create Image

#### Boot the board
24. Locate the generated BOOT.bin file and copy into a micro SD card
25. Set the board to boot from SD card, and connect a USB cable to communicate with the UART
26. Start a console applicate (TeraTerm, PuTTY, etc)
27. Insert the card into the board and power it up
28. Follow the dialog in the terminal

