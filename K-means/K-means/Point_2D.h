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

typedef vector<Point_2D>::iterator it;

/* Object function for calculating distance */
struct Point2D_distance 
{
	inline double operator()(Point_2D p1, Point_2D p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y-p2.y,2));
	}
};

/* Averaging function */
//Point_2D Point2D_average(Point_2D data[])
//{
//	Point_2D result(0,0);
//	int count;
//
//	for (int i = 0; i < sizeof(data)/sizeof(data[0]) ; i++)
//	{
//		
//	}
//
//	return result;
//}

struct Point2D_average
{
	inline Point_2D operator()(it start, int groupId, int* currentGroupId, int elementCount)
	{
		Point_2D newCentroid = Point_2D(0.0, 0.0);
		int count = 0;

		for (int i = 0; i < elementCount; i++)
		{
			if (currentGroupId[i] == groupId)
			{
				newCentroid.x += (start + i)->x;
				newCentroid.y += (start + i)->y;
				count++;
			}
		}

		newCentroid.x /= count;
		newCentroid.y /= count;

		return newCentroid;
	}
};