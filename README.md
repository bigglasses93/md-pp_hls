# md-pp_hls
HLS model of MD pipeline for non-bond forces calculation

Require: Vivado HLS, or Vitis HLS

Tested on version: 2019.2

How to run:

By GUI: Create a project in HLS, include the source files for synthesis:	

	- C/C++ synthesis source file: md-pp.cpp | select compute_pp as top function
	- testbench (simulation): test_md-pp.cpp
  
By Tcl command line: 

	- Confirm file run_hls.tcl:
    	+ Choose a target device and edit line "set_part {"device name"} 
		+ Comment/uncomment to run a specific task in HLS
			csim_design -- C simulation
			csynth_design -- C synthesis
			cosim_design -- C/RTL cosimulation
			export_design -- export IP that can be used in Vivado
	- After confirming tcl file, run: vivado_hls -f run_hls.tcl
