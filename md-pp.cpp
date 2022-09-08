#include "md-pp.h"
#include "table.h"
#include <hls_stream.h>
#include <math.h>
#include "hls_math.h"
#ifndef __SYNTHESIS__
	#include <iostream>
	using namespace std;
#endif

ap_ufixed<4, 2> alpha = 3.5;
ap_ufixed<32, 3> gc_coef = 3.9493; // 2alpha/sqrt(pi)
//ap_fixed<32, 12> ke_cl = 8.98755; //coulomb constant

void compute_pp(inxyz_t& x_in, inxyz_t& y_in, inxyz_t& z_in,
		inq_t& qi_in, inq_t& qj_in,
		int& ti_in, int& tj_in,
		out_phi_t& phi_c, out_phi_t& phi_v,
		out_t& fx_out, out_t& fy_out, out_t& fz_out){
#pragma HLS interface ap_fifo port=x_in,y_in,z_in,qi_in,qj_in,ti_in,tj_in,phi_c,phi_v,fx_out,fy_out,fz_out depth=32

#pragma HLS INLINE
#pragma HLS DATAFLOW

	//r^2 = x^2 + y^2 + z^2
	sig_6_31_t sig_r_power2 = x_in * x_in + y_in*y_in + z_in*z_in;
	//check if in cutoff region
	if(sig_r_power2 > rcut_sq){
		phi_c = 0;
		phi_v = 0;
		fx_out = 0;
		fy_out = 0;
		fz_out = 0;
		#ifndef __SYNTHESIS__
			cout << "Pair distance is larger than cutoff distance" << endl;
		#endif
		return;
	}

	// calculate 1/r, 1/r^2
	sig_6_31_t sig_r_inv = hls::rsqrt(sig_r_power2);
	sig_6_31_t sig_r_power2_inv = sig_r_inv * sig_r_inv;
#ifndef __SYNTHESIS__
	//cout << "1/r = " << sig_r_inv << endl;
#endif

	//qi x qj
	inq_t qij = qi_in * qj_in;
	sig_f1_6_23_t qij_cutoff = qij * grbased_cl_cutoff;

	// calculate coulomb force: F=1/(rij^3)*qi*qj * gc(alpha*rij^2)
	// In MDG4A, gc is an error function, gc(alpha*r^2)=erfc(alpha*r^2) + 2alpha*r/sqrt(pi)*e^(-alpha^2*r^2); 
	sig_6_31_t sig_r = hls::sqrt(sig_r_power2);
	sig_6_31_t xd = alpha * sig_r;
	sig_12s_32_t xd_x2_neg = -xd * xd;
	sig_12s_32_t gc = hls::erfc(xd) + gc_coef*hls::exp(xd_x2_neg)*sig_r;
	sig_12s_32_t sig_cl_f = qij_cutoff*gc*sig_r_inv*sig_r_inv*sig_r_inv;

	// for simplicity, may consider gc(alpha*r^2) to be Coulomb constant ke_cl
	// referred from Coulomb force calculation formula: F=ke*qi*qj/(rij^2)
	//sig_f8_24s_t sig_cl_f = ke_cl*qij_cutoff*sig_r_inv*sig_r_inv;

#ifndef __SYNTHESIS__
	//cout << "gc = " << gc << endl;
	//cout << "Coulomb force = " << sig_cl_f << endl;
#endif	

	//van der Waals coefficient calculation
	//ti, tj determine the type of atom i, atom j
	int ti = ti_in;
	int tj = tj_in;
	vdw_eps_t eps_i, eps_j;
	eps_i = eps_v[ti];
	eps_j = eps_v[tj];
	sig_12_32_t eps_ij = eps_i * eps_j;
	eps_ij = hls::sqrt(eps_ij);
	vdw_sigma_t sigma_i, sigma_j;
	sigma_i = sigma_v[ti];
	sigma_j = sigma_v[tj];
	sig_12_32_t sigma_ij = (sigma_i+sigma_j)/2;
#ifndef __SYNTHESIS__
	//cout << "eij = " << eps_ij << endl;
	//cout << "sigma_ij = " << sigma_ij << endl;
#endif
	//van der Waals force calculation
	sig_12_32_t vdw_inv = sigma_ij * sigma_ij * sig_r_power2_inv; // *(sigma^2)/(r^2)
	sig_12s_32_t sig_vdw_f_6 = vdw_inv * vdw_inv * vdw_inv; //cout << "xd ^ 6 = " << sig_vdw_f_6 << endl;
	sig_12s_32_t sig_vdw_f_12 = sig_vdw_f_6 * sig_vdw_f_6; //cout << "xd ^ 12 = " << sig_vdw_f_12 << endl;
	sig_12s_32_t sig_vdw_f_sub = 12*sig_vdw_f_12 - 6*sig_vdw_f_6; //cout << "sub = " << sig_vdw_f_sub << endl;
	sig_12s_32_t sig_vdw_f = eps_ij * sig_r_power2_inv * sig_vdw_f_sub;
	
#ifndef __SYNTHESIS__
	//cout << "Van der Waals force = " << sig_vdw_f << endl;
#endif

	phi_c = sig_cl_f;
	phi_v = sig_vdw_f;
	//combine two forces
	sig_12s_32_t sig_sum_f = sig_cl_f + sig_vdw_f;
	fx_out = sig_sum_f*x_in;
	fy_out = sig_sum_f*y_in;
	fz_out = sig_sum_f*z_in;
}

