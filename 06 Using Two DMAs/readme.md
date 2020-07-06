Using Two DMAs
==============

Sometimes we will have more than one DMA engine that need to be synchronized. Here I show how to do that with a simple project where one DMA moves data from MM to AXI-Stream, goes to an AXI-Strean FIFO and the output stream data is read by another DMA and stored back in memory.

# Build the Vivado project
The whole project is rebuilt by a tcl (rebuild_umyir_dma.tcl). Copy it on the folder where the project folder is to be created, open Vivado and browse to that tcl file. The source it with:

source ./rebuild_umyir_dma.tcl

That will create the hardware project.
Generate the bitstream and export the hardware, including the bitstream

# Build the software
After the hardware has been built and exported, launch SDK from Vivado, it will have the hardware platform file.
Create a new application project and select type FSBL and name it FSBL
Create a new application project type Helloworld and name it App
Copy the files from the repository SDK folder and drop them in the App folder in the project.
Compile all the software

# Make the boot image
In SDK, click Xilinx > Create Boot Image and select these components (same order):
- FSBL.elf
- bitstream
- App.elf
Generate the boot file and copy it to a microSD card.
Insert it into the MYIR board, connect a USB cable to see the console output and power the board
