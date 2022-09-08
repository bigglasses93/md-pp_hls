############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project md-pp
set_top compute_pp
add_files md-pp.cpp
add_files -tb test_md-pp.cpp
open_solution "solution1"
set_part {xcu280-fsvh2892-2L-e}
create_clock -period 3 -name default
config_sdx -optimization_level none -target none
config_export -vivado_optimization_level 2
set_clock_uncertainty 12.5%
#source "./md-pp/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design -trace_level all
export_design -flow impl -rtl verilog -format ip_catalog
