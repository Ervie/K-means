#include <iostream>
#include "K_means.h"
#include "StopConditions.h"
#include "Point_2D.h"

#include <vector>
#include <algorithm> 

using namespace std;

int main(int argc, char** argv)
{
	srand(time(NULL));

	K_means<Point_2D> k_means;

	Point_2D* result;

	vector<Point_2D> vec = vector<Point_2D>();

	vec.push_back(Point_2D(0, 0));
	vec.push_back(Point_2D(3, 3));
	vec.push_back(Point_2D(-2, 0));

	result = k_means.Group(vec.begin(), vec.end(), Point2D_distance(), true, 23, 3, MaxIterations);
	
	cin.get();
	return 0;
}