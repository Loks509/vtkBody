#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include <fstream>

using namespace std;
class SphereVTK
{
public:
	SphereVTK(const char* filename, int Nx, int Ny, int Nz);
	void addPoint(double x, double y, double z, int i, int j, int k, complex<double> value);
	void createFile();
	void setSmooth(bool isSmooth);
private:
	ofstream file;
	size_t Nx, Ny, Nz;
	vector<vector<double>> pointData;
	vector<vector<int>> indexData;
	vector<complex<double>> valueData;
	vector<vector<int>> voxelData;
	vector<vector<int>> pyramidData;
	vector<vector<int>> wedgeData;

	void createCells();
	int findIndex(int i, int j, int k);
	int findIndex(int i, int j, int k, int begin, int end);
	int findFirstI(int _i);
	int findEndI(int _i);
	bool isSmooth = false;

	vector<int> findMinInLayer(int _i);
	vector<int> findMaxInLayer(int _i);
};

