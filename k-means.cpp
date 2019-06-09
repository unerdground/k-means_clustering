// clusters.cpp : 
//

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// representation of point;
// x,y - coordinates on XY
// cluster - assigned cluster, by default -1(not assigned)
struct point {
	double x, y;
	int cluster = -1;
};


int main()
{

	std::vector<point> init; //vector of points
	unsigned int t, k; // t - number of algorithm iterations, k - amount of clusters
	std::string filename; // string with name of input file
	std::cin >> t >> k >> filename; //input 
	std::ifstream file(filename); 
	// reading coordinates of points
	while (!file.eof()) {
		point tmp;
		file >> tmp.x >> tmp.y;
		init.push_back(tmp);
	}
	file.close();

	// finding bounding box of points
	// !!convertion from double to int, data loss!!
	//
	auto tmp = std::max_element(init.begin(), init.end(), [](point a, point b) {return a.x < b.x; });
	int max_x = tmp->x;
	tmp = std::max_element(init.begin(), init.end(), [](point a, point b) {return a.y < b.y; });
	int max_y = tmp->y;
	tmp = std::min_element(init.begin(), init.end(), [](point a, point b) {return a.x < b.x; });
	int min_x = tmp->x;
	tmp = std::min_element(init.begin(), init.end(), [](point a, point b) {return a.y < b.y; });
	int min_y = tmp->y;

	// initial centers with rand coordinates within bounding box
	std::vector<point> centers(k);
	for (unsigned int i = 0; i < k; i++) {
		centers[i].x = rand() % (max_x - min_x + 1) + min_x;
		centers[i].y = rand() % (max_y - min_y + 1) + min_y;
	}

	// loop for number of iterations
	for (unsigned int i = 0; i < t; i++) {
		// calculate distance from point to centers for every point 
		for (unsigned int j = 0; j < init.size(); j++) {
			double* dists = new double[k];
			for (unsigned int p = 0; p < k; p++) {
				double a = std::abs(init[j].y - centers[p].y);	// length in y-axis
				double b = std::abs(init[j].x - centers[p].x);	// length in x-axis
				dists[p] = std::sqrt(std::pow(a, 2) + std::pow(b, 2));	// distance from point to center
			}
			// assign cluster with closest center
			init[j].cluster = std::min_element(dists, dists + k) - dists;
			delete[] dists;
		}
		// calculating new centers
		std::unique_ptr<double[]> sum_x(new double[k], std::default_delete<double[]>());
		std::unique_ptr<double[]> sum_y(new double[k], std::default_delete<double[]>());
		std::unique_ptr<int[]> count = std::make_unique<int[]>(k);
		// init with 0
		for (unsigned int p = 0; p < k; p++) {
			sum_x[p] = 0;
			sum_y[p] = 0;
			count[p] = 0;
		}
		for (unsigned int f = 0; f < init.size(); f++) {
			sum_x[init[f].cluster] += init[f].x;
			sum_y[init[f].cluster] += init[f].y;
			count[init[f].cluster]++;
		}
		// set new centers to average coordinate of points in cluster
		for (unsigned int f = 0; f < k; f++) {
			centers[f].x =sum_x[f] / count[f];
			centers[f].y = sum_y[f] / count[f];
		}
	}

	// output to file in format "<point number> <cluster number>"
	std::ofstream ofile("out.txt");
	for (unsigned int i = 0; i < init.size(); i++) {
		ofile << i << " " << init[i].cluster << "\n";
	}
	ofile.close();
}

