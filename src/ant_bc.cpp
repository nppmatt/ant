#include <iostream>
#include <cmath>
#include "Eigen/Dense"

#define FINE 10

int main() {
	const unsigned int cellCount = 2 << FINE;
	const double fsImp = 376.73031367;

	Eigen::VectorXd E_z(cellCount);
	Eigen::VectorXd H_y(cellCount);
	E_z.setZero();
	H_y.setZero();

	const unsigned int maxTime = 10000;
	const unsigned int snapshotSpacing = 10;
	const unsigned int probeSpacing = 10;
	for (unsigned int qTime = 0; qTime < maxTime; ++qTime) {
		/* Right absorbing boundary condition for H. */
		H_y(cellCount-1) = H_y(cellCount-2);

		/* Update magnetic field. */
		for (unsigned int mm = 0; mm < cellCount-1; ++mm) {
			H_y(mm) = H_y(mm) + (E_z(mm+1) - E_z(mm)) / fsImp;
		}

		/* Left absorbing boundary condition for E. */
		E_z(0) = E_z(1);

		/* Update electric field. */
		for (unsigned int mm = 1; mm < cellCount; ++mm) {
			E_z(mm) = E_z(mm) + (H_y(mm) - H_y(mm-1)) * fsImp;
		}

		/* Additive source. */
		E_z(512) += exp( -(qTime - 30.0) * (qTime - 30.0) / 100.0 );

		/* Temporal coarsening after solving. */
		if (qTime % snapshotSpacing == 0) {
			/* Spatial coarsening after solving. */
			for (unsigned int probe = 0; probe < cellCount; probe += probeSpacing) {
				std::cout << E_z(probe) << ",";
			}
		}
		std::cout << std::endl;
	}

	return 0;
}
