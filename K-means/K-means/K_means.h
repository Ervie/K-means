#pragma once

#include "StopConditions.h"
#include <stdexcept>
#include <iterator>
#include <vector>

using namespace std;

template <typename Iterator> 
class K_means
{
	private:
		Iterator* oldCentroids;
		Iterator* newCentroids;
		int* currentGroupId;
		double** distancesMatrix;
		Iterator* returnValues;

	public:
		typedef typename vector<Iterator>::iterator it;
		/* Template functions disallow declaration in cpp file */

		

		K_means<Iterator>()
		{
			
		}

		void Init(int elementCount, int groupNumber)
		{
			if (elementCount < 1)
				throw std::logic_error("Liczba elementów jest mniejsza ni¿ 1");

			oldCentroids = new Iterator[groupNumber];
			newCentroids = new Iterator[groupNumber];
			returnValues = new Iterator[groupNumber];
			currentGroupId = new int[elementCount];

			distancesMatrix = new double*[elementCount];

			for (int i = 0; i < groupNumber; i++)
			{
				distancesMatrix[i] = new double[elementCount];
			}

			for (int i = 0; i < elementCount; i++)
			{
				currentGroupId[i] = 0;
			}

			for (int i = 0; i < groupNumber; i++)
			{
				for (int j = 0; j < elementCount; j++)
					distancesMatrix[i][j] = 0;
			}
		}

		void Finish(int elementCount)
		{
			delete oldCentroids;
			delete newCentroids;
			delete currentGroupId;

			for (int i = 0; i < elementCount; i++)
			{
				delete distancesMatrix[i];
			}

			delete [] distancesMatrix;
		}

		template <typename DistancePredicate>
		Iterator* Group(it first, it last, DistancePredicate &distanceMeasure, bool averagingFunction, int maxIteration, int k, StopConditions stopCondition)
		{
			Init(distance(first, last), k);

			if (first == last)
				return nullptr;

			Finish(distance(first, last));
			return returnValues;

		}

};