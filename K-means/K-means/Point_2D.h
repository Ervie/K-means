#pragma once

#include <cmath>
#include <stdlib.h>
#include <time.h>     


/* Class */
class Point_2D
{

	public:

		double x;
		double y;

		Point_2D()
		{
			x = rand() % 5;
			y = rand() % 5;
		}

		Point_2D(double _x, double _y)
		{
			x = _x;
			y = _y;
		}
};

inline std::ostream & operator<<(std::ostream & Str, const Point_2D& p)
{
	return Str << "(" << p.x << "," << p.y << ")";
};

/* Object function for calculating distance */
struct Point2D_distance 
{
	inline double operator()(Point_2D p1, Point_2D p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y-p2.y,2));
	}
};

struct Point2D_average
{
	Point_2D newCentroid = Point_2D(0.0, 0.0);
	int count = 0;

	inline void operator()(Point_2D &oldCentroid)
	{
		if (count != 0)
		{
			newCentroid.x /= count;
			newCentroid.y /= count;
		}
		else
		{
			newCentroid.x = oldCentroid.x;
			newCentroid.y = oldCentroid.y;
		}

		oldCentroid.x = newCentroid.x;
		oldCentroid.y = newCentroid.y;

		newCentroid = Point_2D(0.0, 0.0);
		count = 0;
	}

	inline void operator +=(const Point_2D & p)
	{
		newCentroid.x += p.x;
		newCentroid.y += p.y;

		count++;
	}
};