/* ant/src/ant.cpp */

#include "include/ant.hpp"
#include "Eigen/Dense"

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/format.h"

#include <cmath>
#include <omp.h>
#include <iostream>
#include <fstream>

#define FINE 9
#define WAVE_STEPS 20

/* 2D FDTD Simulation, aka a TM^z simulation (Transverse Magnetic-z) */
int main() {
	constexpr unsigned int gridLength = 2 << FINE;
	constexpr double courant = 1.0 / sqrt(2.0);
	constexpr double waveProp = sqrt(mu_0 * epsilon_0);

	/* The fields we would like to update in a staggered manner (Yee). */
	Eigen::MatrixXd E_z(gridLength, gridLength);
	Eigen::MatrixXd H_x(gridLength, gridLength-1);
	Eigen::MatrixXd H_y(gridLength-1, gridLength);

	/* Scalar fields that transfer field quanta to their transverse pair (torque-y).
	 * Subscript convention is to indicate source field first and destination after.
	 * */
	Eigen::MatrixXd C_eze(gridLength, gridLength);
	Eigen::MatrixXd C_ezh(gridLength, gridLength);
	Eigen::MatrixXd C_hxe(gridLength, gridLength-1);
	Eigen::MatrixXd C_hxh(gridLength, gridLength-1);
	Eigen::MatrixXd C_hye(gridLength-1, gridLength);
	Eigen::MatrixXd C_hyh(gridLength-1, gridLength);

	/* E-field update coefficients.
	 * All coefficients make a free-space/vacuum assumption.
	 * */
	#pragma omp parallel for
	for (unsigned int mm = 0; mm < gridLength; ++mm) {
		for (unsigned int nn = 0; nn < gridLength; ++nn) {
			C_eze(mm, nn) = 1.0;
			C_ezh(mm, nn) = courant * fsImp;
		}
	}	

	/* H-field update coefficients. */
	#pragma omp parallel for
	for (unsigned int mm = 0; mm < gridLength; ++mm) {
		for (unsigned int nn = 0; nn < gridLength-1; ++nn) {
			C_hxh(mm, nn) = 1.0;
			C_hxe(mm, nn) = courant / fsImp;
		}
	}	
	#pragma omp parallel for
	for (unsigned int mm = 0; mm < gridLength-1; ++mm) {
		for (unsigned int nn = 0; nn < gridLength; ++nn) {
			C_hyh(mm, nn) = 1.0;
			C_hye(mm, nn) = courant / fsImp;
		}
	}	

	/* More simulation parameters. */
	constexpr unsigned int src_x = 1022;
	constexpr unsigned int arrayDelta = 16;

	// center
	constexpr unsigned int src7_y = 512;

	constexpr unsigned int src6_y = src7_y - arrayDelta/2;
	constexpr unsigned int src5_y = src6_y - arrayDelta;
	constexpr unsigned int src4_y = src5_y - arrayDelta;
	constexpr unsigned int src3_y = src4_y - arrayDelta;
	constexpr unsigned int src2_y = src3_y - arrayDelta;
	constexpr unsigned int src1_y = src2_y - arrayDelta;
	constexpr unsigned int src0_y = src1_y - arrayDelta;
	constexpr unsigned int srca_y = src0_y - arrayDelta;

	constexpr unsigned int src8_y = src7_y + arrayDelta/2;
	constexpr unsigned int src9_y = src8_y + arrayDelta;
	constexpr unsigned int src10_y = src9_y + arrayDelta;
	constexpr unsigned int src11_y = src10_y + arrayDelta;
	constexpr unsigned int src12_y = src11_y + arrayDelta;
	constexpr unsigned int src13_y = src12_y + arrayDelta;
	constexpr unsigned int src14_y = src13_y + arrayDelta;
	constexpr unsigned int src15_y = src14_y + arrayDelta;
	constexpr unsigned int maxTime = 6000;
	constexpr unsigned int snapshotSpacing = 10;

	/* For snapshots. */
	const static Eigen::IOFormat CSVFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", "\n");

	/* Beginning FDTD iteration scheme proper. */
	for (unsigned int timeStep = 0; timeStep < maxTime; ++timeStep) {
		/* Update magnetic field. */
		#pragma omp parallel for
		for (unsigned int mm = 0; mm < gridLength; ++mm) {
			for (unsigned int nn = 0; nn < gridLength-1; ++nn) {
				H_x(mm,nn) = C_hxh(mm,nn) * H_x(mm,nn) 
					- C_hxe(mm,nn) * ( E_z(mm,nn+1) - E_z(mm,nn) );
			}
		}
		for (unsigned int mm = 0; mm < gridLength-1; ++mm) {
			#pragma omp parallel for
			for (unsigned int nn = 0; nn < gridLength; ++nn) {
				H_y(mm,nn) = C_hyh(mm,nn) * H_y(mm,nn) 
					+ C_hye(mm,nn) * ( E_z(mm+1,nn) - E_z(mm,nn) );
			}
		}

		/* Update electric field. */
		for (unsigned int mm = 1; mm < gridLength-1; ++mm) {
			for (unsigned int nn = 1; nn < gridLength-1; ++nn) {
				E_z(mm,nn) = C_eze(mm,nn) * E_z(mm,nn)
					+ C_ezh(mm,nn)
					* (( H_y(mm,nn) - H_y(mm-1,nn) ) - ( H_x(mm,nn) - H_x(mm,nn-1) ));
			}
		}

		/* Zero out borders of magnetic field as a BC (cheating). */
		#pragma omp parallel for
		for (unsigned int mm = 0; mm < gridLength; ++mm) {
			H_x(mm,0) = 0.0;
			H_x(mm,gridLength-2) = 0.0;
		}
		#pragma omp parallel for
		for (unsigned int nn = 0; nn < gridLength-1; ++nn) {
			H_x(0,nn) = 0.0;
			H_x(gridLength-1,nn) = 0.0;
		}
		#pragma omp parallel for
		for (unsigned int mm = 0; mm < gridLength-1; ++mm) {
			H_y(mm,0) = 0.0;
			H_y(mm,gridLength-1) = 0.0;
		}
		#pragma omp parallel for
		for (unsigned int nn = 0; nn < gridLength; ++nn) {
			H_y(0,nn) = 0.0;
			H_y(gridLength-2,nn) = 0.0;
		}

		/* Additive source. */
		E_z(src_x, src1_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src2_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src3_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src4_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src5_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src6_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));

		E_z(src_x, src8_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src9_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src10_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src11_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src12_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src13_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));

		E_z(src_x, src14_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src15_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, src0_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));
		E_z(src_x, srca_y) += sin_table_0_00001_LERP(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp));

		/* Take snapshot with temporal coarsening after solving. */
		if (timeStep % snapshotSpacing == 0) {
			std::string outFileName = fmt::format("out/result_{:06d}.csv", (int)(timeStep / snapshotSpacing));
			std::ofstream file(outFileName.c_str());
			file << E_z.format(CSVFormat);
		}
	}

	return 0;
}

/* ant/src/ant.cpp */
