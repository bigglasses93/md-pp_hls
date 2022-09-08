#include "ap_int.h"
#include <hls_stream.h>
//constant
#define DATA_SIZE 3
#define rcut_sq 1.44
#define grbased_cl_cutoff 1
#define grbased_vdw_cutoff 1
#define vdw_cutoff 1
#define exclusion 1
#define n_atom_type 4 //number of atom types
/*
	Unit of distance: nm
	Cutoff distance: 1.2nm
	Input range: 0-3.0nm --> 2bits integer representation
	r^2 = x^2 + y^2 + z^2 ~ 6bits integer representation
*/
//IO type
typedef ap_fixed<29, 2> inxyz_t;
typedef ap_fixed<32, 2> inq_t;
typedef ap_fixed<32, 12> out_t;
typedef ap_fixed<64, 32> out_phi_t; 
typedef ap_ufixed<23, 6> exclusion_t; 
typedef ap_ufixed<32, 5> vdw_eps_t;
typedef ap_ufixed<32, 1> vdw_sigma_t;
typedef ap_ufixed<32, 2> gc_t;
typedef ap_ufixed<31, 6> sig_6_31_t; //total 31 bits word, 6 bits integer, 25 bits fraction
typedef ap_fixed<29, 6> sig_f1_6_23_t; //F1:6:23
typedef ap_fixed<32, 12> sig_12s_32_t; //F8:24S --> signed, 12 bits integer, 19 bits fraction
typedef ap_ufixed<29, 6> sig_f6_23_t; //F6:23
//typedef ap_ufixed<30, 7> sig_f7_23_t; //F7:23
typedef ap_ufixed<32, 12> sig_12_32_t; //F7:25 --> unsigned, 12 bits integer, 20 bits fraction
typedef ap_ufixed<32, 8> sig_f8_23_t; //F8:23
//typedef ap_int<32> sig_x32_t; //X32


/*void compute_pp(hls::stream<inxyz_t>& x_in, hls::stream<inxyz_t>& y_in, hls::stream<inxyz_t>& z_in,
		hls::stream<inq_t>& qi_in, hls::stream<inq_t>& qj_in,
		hls::stream<int>& ti_in, hls::stream<int>& tj_in,
		hls::stream<out_phi_t>& phi_c, hls::stream<out_phi_t>& phi_v,
		hls::stream<out_t>& fx_out, hls::stream<out_t>& fy_out, hls::stream<out_t>& fz_out);*/

void compute_pp(inxyz_t& x_in, inxyz_t& y_in, inxyz_t& z_in,
		inq_t& qi_in, inq_t& qj_in,
		int& ti_in, int& tj_in,
		out_phi_t& phi_c, out_phi_t& phi_v,
		out_t& fx_out, out_t& fy_out, out_t& fz_out);
