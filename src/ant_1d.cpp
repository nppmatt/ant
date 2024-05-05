#include <iostream>
#include <cmath>
#include "Eigen/Dense"

#define FINE 8

int main() {
	const unsigned int cellCount = 2 << FINE;
	const double imp0 = 377.0;

	Eigen::VectorXd E_z(cellCount);
	Eigen::VectorXd H_y(cellCount);
	E_z.setZero();
	H_y.setZero();

	const unsigned int maxTime = 1000;
	for (unsigned int qTime = 0; qTime < maxTime; ++qTime) {
		for (unsigned int mm = 0; mm < cellCount-1; ++mm) {
			H_y(mm) = H_y(mm) + (E_z(mm+1) - E_z(mm)) / imp0;
		}

		for (unsigned int mm = 1; mm < cellCount; ++mm) {
			E_z(mm) = E_z(mm) + (H_y(mm) - H_y(mm-1)) * imp0;
		}

		E_z(0) = exp( -(qTime - 30.0) * (qTime - 30.0) / 100.0 );
		std::cout << E_z(50) << std::endl;
	}

	return 0;
}
