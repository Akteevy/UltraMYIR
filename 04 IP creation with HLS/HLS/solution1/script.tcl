############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project convolution
set_top convolve
add_files convolution/convolve.cpp
add_files -tb convolution/testbench.cpp
open_solution "solution1"
set_part {xczu3cg-sfvc784-1-e} -tool vivado
create_clock -period 10 -name default
config_export -format ip_catalog -rtl verilog
source "./convolution/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -flow syn -rtl verilog -format ip_catalog
