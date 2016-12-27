#include <iostream>
#include "K_means.h"
#include "StopConditions.h"

#include "Point_2D.h"
#include "IntTest.h"

#include <vector>
#include <algorithm> 

using namespace std;

void IntegerScenario();
void Point2DScenario();

int main(int argc, char** argv)
{
	srand(time(NULL));

	//IntegerScenario();

	Point2DScenario();

	cin.get();
	return 0;
}

void IntegerScenario()
{
	K_means<int> k_means;

	int* result;

	vector<int> vec = vector<int>();

	vec.push_back(3);
	vec.push_back(-5);
	vec.push_back(10);
	vec.push_back(4);
	vec.push_back(0);
	vec.push_back(-6);
	vec.push_back(-9);
	vec.push_back(9);

	result = k_means.Group(vec.begin(), vec.end(), Int_distance(), Int_average(), 3, 2, MaxIterations);
}

void Point2DScenario()
{
	K_means<Point_2D> k_means;

	Point_2D* result;

	vector<Point_2D> vec = vector<Point_2D>();

	vec.push_back(Point_2D(0, 0));
	vec.push_back(Point_2D(3, 3));
	vec.push_back(Point_2D(-2, 0));
	vec.push_back(Point_2D(-3, 5));
	vec.push_back(Point_2D(2, 1));
	vec.push_back(Point_2D(0, 4));
	vec.push_back(Point_2D(-5, 5));
	vec.push_back(Point_2D(-2, -3));

	result = k_means.Group(vec.begin(), vec.end(), Point2D_distance(), Point2D_average(), 4, 3, StableState);
}