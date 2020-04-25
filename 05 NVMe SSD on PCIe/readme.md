Configure the UltraMyir to use an NVMe SSD
==========================================

The Ultramyir board by MYIR has a Zynq Ultrascale+ MPSoC with a PCIe slot. In this tutorial we will learn how to configure the PS to use the PCIe port. As end point I will use a Samsung 970 Pro SSD, although any other brand should not make a difference. Note that an adapter board is needed to adapt the M.2 interface to PCIe. These adapters are readily available and unexpensive.

## Hardware (Vivado)

* Clone the project and copy the Vivado folder
* Open Vivado 2019.2 and source the build_me.tcl file
* After the project is built, you can make the bitstream and analyze it.

## Petalinux

* Clone the BSP file nvme.bsp into a Linux machine
* Create a petalinux project sourcing that BSP
* Create the boot image

