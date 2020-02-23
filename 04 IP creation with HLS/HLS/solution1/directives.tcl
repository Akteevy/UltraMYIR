############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_interface -mode axis -register -register_mode both "convolve" str_in
set_directive_interface -mode axis -register -register_mode both "convolve" str_out
set_directive_loop_flatten "convolve/buff_shift"
set_directive_loop_flatten "convolve/mult"
set_directive_stream -dim 1 "convolve" str_in
set_directive_stream -dim 1 "convolve" str_out
set_directive_interface -mode ap_ctrl_none "convolve"
