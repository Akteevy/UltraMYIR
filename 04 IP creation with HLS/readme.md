IP creation with HLS
====================

In this example a convolution IP core is built with HLS.

The IP core is then added to a vivado project where a Zynq PS sends data to it using a DMA and receives the processed result.

The IP core is configured to receive and produce data in AXI Stream format.

## opening the HLS project

* Clone the folder HLS in this repository to an empty folder
* Start Vivado HLS, select File > Open Project and navigate to that folder

## Creating the HLS project from scratch

* Start Vivado HLS and create a new project
* Choose the project name and location
* Enter the top function name
* (optional) Add the top file top.cpp 
* Leave the period as 10 ns, enter 2 as uncertainty and select xczu3eg-sfvc784-1-e as part
* Click Finish
* In the Explorer panel, right click on 'Source' and select 'New File'. Name it top.cpp
* Right click on 'Test Bench' and select 'New File'. Name it test.cpp



