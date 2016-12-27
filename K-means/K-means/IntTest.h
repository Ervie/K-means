#pragma once
#include <cmath>
#include <vector>

typedef vector<int>::iterator intIt;

struct Int_distance
{
	inline double operator()(int p1, int p2)
	{
		return (double)abs(p1 - p2);
	}
};



struct Int_average
{
	inline double operator()(intIt start, int groupId, int* currentGroupId, int elementCount)
	{
		int newCentroid = 0;
		int count = 0;

		for (int i = 0; i < elementCount; i++)
		{
			if (currentGroupId[i] == groupId)
			{
				newCentroid += *(start + i);
				count++;
			}
		}

		newCentroid / count;

		return newCentroid;
	}
};