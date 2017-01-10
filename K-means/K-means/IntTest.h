#pragma once
#include <cmath>
#include <vector>

struct Int_distance
{
	inline double operator()(int p1, int p2)
	{
		return (double)abs(p1 - p2);
	}
};



struct Int_average
{
	int newCentroid = 0;
	int count = 0;

	inline void operator()(int &oldCentroid)
	{
		if (count != 0)
			newCentroid = (int)(newCentroid / count);
		else
			newCentroid = oldCentroid;

		oldCentroid = newCentroid;

		newCentroid = 0;
		count = 0;
	}

	inline void operator +=(const int & p)
	{
		newCentroid += p;

		count++;
	}
};