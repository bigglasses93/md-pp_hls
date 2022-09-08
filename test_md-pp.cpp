#include "md-pp.h"
#include <stdio.h>
#include <string>
#define sample_cnt 5
using namespace std;

int main(){
	inxyz_t x_in[sample_cnt] = {0.5, 0.0, -0.5, 0.5, 1};
	inxyz_t y_in[sample_cnt] = {0.5, 0.0, 0.5, 0.4, 0.5};
	inxyz_t z_in[sample_cnt] = {0.5, 1.0, 0.5, 0.6, 0.5};
	inq_t qi_in[sample_cnt] = {1.0, -1.0, 0.0, 0.0, -1.0};
	inq_t qj_in[sample_cnt] = {1.0, 1.0, 1.0, -1.0, -1.0};
	int ti_in[sample_cnt] = {1, 0, 0, 2, 3};
	int tj_in[sample_cnt] = {1, 0, 0, 2, 3};
	out_phi_t phi_c[sample_cnt], phi_v[sample_cnt];
	out_t fx_out[sample_cnt], fy_out[sample_cnt], fz_out[sample_cnt];
	for(int i=0; i<sample_cnt; i++){
		std::cout << "Test " << i << ": ";
		std::cout << "Atom i type " << ti_in[i] << ", atom j type " << tj_in[i] << endl;
		std::cout << "x_in = " << x_in[i] << " nm, y_in = " << y_in[i] << " nm, z_in = " << z_in[i] << " nm" <<endl;
		std::cout << "qi_in = " << qi_in[i] << ", qj_in = " << qj_in[i] << endl;
		compute_pp(x_in[i], y_in[i], z_in[i],
				qi_in[i], qj_in[i],
				ti_in[i], tj_in[i],
				phi_c[i], phi_v[i],
				fx_out[i], fy_out[i], fz_out[i]);
		std::cout << "fx = " << fx_out[i] << std::endl;
		std::cout << "fy = " << fy_out[i] << std::endl;
		std::cout << "fz = " << fz_out[i] << std::endl;
		std::cout << "F_coulomb = " << phi_c[i] << std::endl;
		std::cout << "F_vdW = " << phi_v[i] << std::endl << endl;
	}

	return 0;
}
