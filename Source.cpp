#include <iostream>
#include <fstream>
#include "SphereVTK.h"

using namespace std;

double len(
	double x_1, double y_1, double z_1,
	double x_2, double y_2, double z_2) {
	return sqrt(pow(x_1 - x_2, 2) + pow(y_1 - y_2, 2) + pow(z_1 - z_2, 2));
}

int main() {
	double radius = 5;

	double c_x = 1, c_y = 1, c_z = 1;

	int N_x = 20, N_y = 20, N_z = 20;

	double h_x = (2. * radius) / N_x, h_z = (2. * radius) / N_z, h_y = (2. * radius) / N_y;

	double Ax = c_x - radius;
	double Ay = c_y - radius;
	double Az = c_z - radius;

	ofstream file("test.txt");
	file << "X Y Z\n";
	SphereVTK sphere("ttt.vtk", N_x, N_y, N_z);

	sphere.setSmooth(true);

	for (size_t i = 0; i < N_x; i++)
	{
		cout << i << endl;
		double x_1 = Ax + i * h_x;
		double x_c = x_1 + h_x / 2.;
		double x_2 = x_1 + h_x;
		for (size_t j = 0; j < N_y; j++)
		{
			double y_1 = Ay + j * h_y;
			double y_c = y_1 + h_y / 2.;
			double y_2 = y_1 + h_y;
			for (size_t k = 0; k < N_z; k++)
			{
				double z_1 = Az + k * h_z;
				double z_c = z_1 + h_z / 2.;
				double z_2 = z_1 + h_z;
				
				if (len(c_x, c_y, c_z, x_c, y_c, z_c) < radius) {
					sphere.addPoint(x_c, y_c, z_c, i, j, k, rand());
					file << x_c << " " << y_c << " " << z_c << endl;
				}
			}
		}
	}
	sphere.createFile();
	file.close();
	return 0;
}