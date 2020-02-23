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
* In the Explorer panel, right click on 'Source' and select 'New File'. Name it convolve.cpp
* Right click on 'Test Bench' and select 'New File'. Name it testbench.cpp
* Copy the contents of convolve.cpp in this repository into your convolve.cpp
* Do the same with testbench.cpp
* Run 'C Synthesis'. The process should end without errors and show a report
* Run 'Run C Simulation'. The process should end without error and printing the results of the test
* Run 'Export RTL'. Select Format: IP Catalog and tick Vivado Synthesis. click OK.
* The IP core will be generated in solution1/impl/ip




