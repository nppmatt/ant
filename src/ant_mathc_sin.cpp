/* ant/src/ant.cpp */

#include "include/numerics.hpp"
#include "Eigen/Dense"

#include <iostream>
#include <cmath>

#define FINE 10
#define WAVE_STEPS 40

int main() {
	constexpr unsigned int cellCount = 2 << FINE;
	constexpr double courant = 1.0;
	constexpr double waveProp = sqrt(mu_0 * epsilon_0);

	Eigen::VectorXd E_z(cellCount);
	Eigen::VectorXd H_y(cellCount);
	E_z.setZero();
	H_y.setZero();

	constexpr unsigned int sourcePos = 500;
	constexpr unsigned int maxTime = 10000;
	constexpr unsigned int snapshotSpacing = 10;
	constexpr unsigned int probeSpacing = 10;
	for (unsigned int timeStep = 0; timeStep < maxTime; ++timeStep) {
		/* Right absorbing boundary condition for H. */
		H_y(cellCount-1) = H_y(cellCount-2);

		/* Update magnetic field. */
		for (unsigned int mm = 0; mm < cellCount-1; ++mm) {
			H_y(mm) = H_y(mm) + (E_z(mm+1) - E_z(mm)) / fsImp;
		}

		/* Correct H next to TFSF BC of source. */
		H_y(sourcePos-1) -= exp( -(timeStep - 30.0) * (timeStep - 30.0) / 100.0 ) / fsImp;

		/* Left absorbing boundary condition for E. */
		E_z(0) = E_z(1);

		/* Update electric field. */
		for (unsigned int mm = 1; mm < cellCount; ++mm) {
			E_z(mm) = E_z(mm) + (H_y(mm) - H_y(mm-1)) * fsImp;
		}

		/* Additive source. */
		E_z(sourcePos) += sin(2 * pi / WAVE_STEPS * (courant * timeStep - waveProp * sourcePos));

		/* Temporal coarsening after solving. */
		if (timeStep % snapshotSpacing == 0) {
			/* Spatial coarsening after solving. */
			for (unsigned int probe = 0; probe < cellCount; probe += probeSpacing) {
				std::cout << E_z(probe) << ",";
			}
		}
		std::cout << std::endl;
	}

	return 0;
}

/* ant/src/ant.cpp */
