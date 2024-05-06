#include "SphereVTK.h"

SphereVTK::SphereVTK(const char* filename, int Nx, int Ny, int Nz): Nx(Nx), Ny(Ny), Nz(Nz), file(filename) {

}

void SphereVTK::addPoint(double x, double y, double z, int i, int j, int k, complex<double> value)
{
	pointData.push_back(vector<double>{x, y, z});
	indexData.push_back(vector<int>{i, j, k});
	valueData.push_back(value);
}

void SphereVTK::createFile()
{
	createCells();

	file << "# vtk DataFile Version 2.0\n" <<
		"Unstructured Grid Example\n" <<
		"ASCII\n" <<
		"DATASET UNSTRUCTURED_GRID\n" <<
		"POINTS " << pointData.size() << " float\n";

	for (auto item : pointData) {
		file << item[0] << " " << item[1] << " " << item[2] << "\n";
	}

	file << "CELLS " << voxelData.size() + wedgeData.size() << " " << voxelData.size() * 9 + wedgeData.size() * 7 << "\n";

	for (auto item : voxelData) {
		file << "8 "<<item[0] << " " << item[1] << " " << item[2] <<" "<< item[3] << " " << item[4] << " " << item[5] << " " << item[6] << " " << item[7] << " " << "\n";
	}
	for (auto item : wedgeData) {
		file << "6 " << item[0] << " " << item[1] << " " << item[2] << " " << item[3] << " " << item[4] << " " << item[5] << "\n";
	}
	
	file << "CELL_TYPES " << voxelData.size() + wedgeData.size() << "\n";

	for (auto item : voxelData) {
		file << "11\n";
	}
	for (auto item : wedgeData) {
		file << "13\n";
	}

	file << "POINT_DATA " << pointData.size() << "\n";
	file << "SCALARS abs float 1\n"<<
		"LOOKUP_TABLE default\n";

	for (auto item : valueData) {
		file << abs(item) << "\n";
	}

	file.close();
}

void SphereVTK::setSmooth(bool isSmooth)
{
	this->isSmooth = isSmooth;
}

void SphereVTK::createCells()
{
	for (size_t i = 0; i < Nx - 1; i++)
	{
		bool flag = true;

		int indexFirst = findFirstI(i);	//первая точка в слое
		int indexFirstEnd = findEndI(i);	//последняя точка в слое

		int indexSecond = findFirstI(i + 1);	//первая точка в слое
		int indexSecondEnd = findEndI(i + 1);	//последняя точка в слое

		if(indexFirst >= 0 && indexSecond >= 0 && indexFirstEnd>=0 && indexSecondEnd>=0)
		for (size_t currIndex = indexFirst; currIndex <= indexFirstEnd; currIndex++)
		{
			vector<int> data_indexes = indexData[currIndex];
			int i1 = data_indexes[0];
			int i2 = data_indexes[1];
			int i3 = data_indexes[2];

			int p0 = findIndex(i1, i2, i3, indexFirst, indexFirstEnd);
			int p1 = findIndex(i1, i2, i3 + 1, indexFirst, indexFirstEnd);
			int p4 = findIndex(i1, i2 + 1, i3, indexFirst, indexFirstEnd);
			int p5 = findIndex(i1, i2 + 1, i3 + 1, indexFirst, indexFirstEnd);


			int p2 = findIndex(i1 + 1, i2, i3, indexSecond, indexSecondEnd);
			int p3 = findIndex(i1 + 1, i2, i3 + 1, indexSecond, indexSecondEnd);
			int p6 = findIndex(i1 + 1, i2 + 1, i3, indexSecond, indexSecondEnd);
			int p7 = findIndex(i1 + 1, i2 + 1, i3 + 1, indexSecond, indexSecondEnd);

			if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				voxelData.push_back(vector<int>{p0, p1, p2, p3, p4, p5, p6, p7});
			}
			//----------------wedge data-----------------
			else if (
				p0 < 0 &&
				p1 < 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p5, p3, p7, p4, p2, p6});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 < 0 &&
				p5 < 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p7, p1, p3, p6, p0, p2});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 < 0 &&
				p7 < 0
				) {
				wedgeData.push_back(vector<int>{p1, p3, p5, p0, p2, p4});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 < 0 &&
				p3 < 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p1, p7, p5, p0, p6, p4});
			}

			else if (
				p0 >= 0 &&
				p1 < 0 &&
				p2 >= 0 &&
				p3 < 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p0, p5, p4, p2, p7, p6});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 < 0 &&
				p6 >= 0 &&
				p7 < 0
				) {
				wedgeData.push_back(vector<int>{p0, p1, p4, p2, p3, p6});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 < 0 &&
				p5 >= 0 &&
				p6 < 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p0, p1, p5, p2, p3, p7});
			}
			else if (
				p0 < 0 &&
				p1 >= 0 &&
				p2 < 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p4, p1, p5, p6, p3, p7});
			}

			else if (
				p0 < 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 < 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p2, p1, p3, p6, p5, p7});
			}
			else if (
				p0 >= 0 &&
				p1 < 0 &&
				p2 >= 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 < 0 &&
				p6 >= 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p0, p3, p2, p4, p7, p6});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 >= 0 &&
				p3 < 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 >= 0 &&
				p7 < 0
				) {
				wedgeData.push_back(vector<int>{p4, p5, p6, p0, p1, p2});
			}
			else if (
				p0 >= 0 &&
				p1 >= 0 &&
				p2 < 0 &&
				p3 >= 0 &&
				p4 >= 0 &&
				p5 >= 0 &&
				p6 < 0 &&
				p7 >= 0
				) {
				wedgeData.push_back(vector<int>{p4, p5, p7, p0, p1, p3});
			}
			//----------------wedge data-----------------

		}


	}
}

int SphereVTK::findIndex(int _i, int _j, int _k)
{
	for (size_t i = 0; i < indexData.size(); i++)
	{
		if (indexData[i][0] == _i &&
			indexData[i][1] == _j &&
			indexData[i][2] == _k)
			return i;
	}
	return -1;
}

int SphereVTK::findIndex(int _i, int _j, int _k, int begin, int end)
{
	for (size_t i = begin; i <= end; i++)
	{
		if (indexData[i][0] == _i &&
			indexData[i][1] == _j &&
			indexData[i][2] == _k)
			return i;
	}
	return -1;
}

int SphereVTK::findFirstI(int _i)
{
	for (size_t i = 0; i < indexData.size(); i++)
	{
		if (indexData[i][0] == _i)
			return i;
	}
	return -1;
}

int SphereVTK::findEndI(int _i)
{
	bool is_begin_find = false;
	
	for (size_t i = 0; i < indexData.size(); i++)
	{
		if (!is_begin_find && indexData[i][0] == _i) is_begin_find = true;
		if (is_begin_find && indexData[i][0] != _i) return i - 1;
		if (is_begin_find && i == indexData.size() - 1) return i;
	}
	return -1;
}

vector<int> SphereVTK::findMinInLayer(int _i)
{
	bool is_begin_find = false;
	vector<int> min{ -1, -1, -1 };
	for (size_t i = 0; i < indexData.size(); i++)
	{
		if (!is_begin_find && indexData[i][0] == _i) { 
			is_begin_find = true;
			min[0] = indexData[i][0];
			min[1] = indexData[i][1];
			min[2] = indexData[i][2];
		}
		if (is_begin_find && indexData[i][0] != _i) return min;

		if (min[0] > indexData[i][0]) min[0] = indexData[i][0];
		if (min[1] > indexData[i][1]) min[1] = indexData[i][1];
		if (min[2] > indexData[i][2]) min[2] = indexData[i][2];

	}
	return min;
}

vector<int> SphereVTK::findMaxInLayer(int _i)
{
	bool is_begin_find = false;
	vector<int> max{ -1, -1, -1 };
	for (size_t i = 0; i < indexData.size(); i++)
	{
		if (!is_begin_find && indexData[i][0] == _i) {
			is_begin_find = true;
			max[0] = indexData[i][0];
			max[1] = indexData[i][1];
			max[2] = indexData[i][2];
		}
		if (is_begin_find && indexData[i][0] != _i) return max;

		if (max[0] < indexData[i][0]) max[0] = indexData[i][0];
		if (max[1] < indexData[i][1]) max[1] = indexData[i][1];
		if (max[2] < indexData[i][2]) max[2] = indexData[i][2];

	}
	return max;
}
