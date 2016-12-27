#pragma once

#include <cmath>

class Point_2D
{

	public:

		double x;
		double y;


		Point_2D(double _x, double _y)
		{
			x = _x;
			y = _y;
		}
};


struct Point2D_distance 
{
	double operator()(Point_2D p1, Point_2D p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y-p2.y,2));
	}
};