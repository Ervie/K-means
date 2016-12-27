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
		Iterator* Centroids;
		int* currentGroupId;
		int* nextGroupId;
		double** distancesMatrix;
		Iterator* returnValues;

		double groupMinimum;

	public:
		typedef typename vector<Iterator>::iterator it;
		/* Template functions disallow declaration in cpp file */

		

		K_means<Iterator>()
		{
			
		}

		void Init(int elementCount, int groupNumber)
		{
			if (elementCount < 1)
				throw std::logic_error("Liczba element�w jest mniejsza ni� 1");

			groupMinimum = DBL_MAX;

			Centroids = new Iterator[groupNumber];
			returnValues = new Iterator[groupNumber];
			currentGroupId = new int[elementCount];
			nextGroupId = new int[elementCount];

			distancesMatrix = new double*[elementCount];

			for (int i = 0; i < groupNumber; i++)
			{
				distancesMatrix[i] = new double[elementCount];
			}

			for (int i = 0; i < elementCount; i++)
			{
				currentGroupId[i] = -1;
				nextGroupId[i] = -1;
			}

			for (int i = 0; i < groupNumber; i++)
			{
				for (int j = 0; j < elementCount; j++)
					distancesMatrix[i][j] = 0;
			}
		}

		void Finish(int elementCount)
		{
			delete Centroids;
			delete currentGroupId;
			delete nextGroupId;

			for (int i = 0; i < elementCount; i++)
			{
				delete distancesMatrix[i];
			}

			delete [] distancesMatrix;
		}

		template <typename DistancePredicate, typename AveragePredicate>
		Iterator* Group(it first, it last, DistancePredicate &distanceMeasure, AveragePredicate &averagingFunction, int maxIteration, int k, StopConditions stopCondition)
		{
			int elementCount = distance(first, last);
			int iterationCounter = 0;
			bool stopConditionFulfilled = false;
			Init(elementCount, k);

			if (first == last)
				return returnValues;

			do
				{
				/* Calculate distancesMatrix */

				for (int i = 0; i < k; i++)
				{
					for (int j = 0; j < elementCount; j++)
					{
						distancesMatrix[i][j] = distanceMeasure(Centroids[i], *(first + j));
					}
				}

				/* Assign elements to this iteration group*/

				for (int i = 0; i < elementCount; i++)
				{
					for (int j = 0; j < k; j++)
					{
						if (distancesMatrix[j][i] < groupMinimum)
						{
							groupMinimum = distancesMatrix[j][i];
							nextGroupId[i] = j;
						}
					}
					groupMinimum = DBL_MAX;
				}

				/* Calculate new centroids */

				for (int i = 0; i < k; i++)
				{
					Centroids[i] = averagingFunction(first, i, nextGroupId, elementCount);
				}

				iterationCounter++;

				/* Check end condition */

				if (stopCondition == MaxIterations)
					stopConditionFulfilled = (iterationCounter >= maxIteration);
				else
				{
					stopConditionFulfilled = true;
					for (int i = 0; i < elementCount; i++)
					{
						if (nextGroupId[i] != currentGroupId[i])
						{
							stopConditionFulfilled = false;
							break;
						}
					}
				}

				/* Copy new cluster id values to old array */

				for (int i = 0; i < elementCount; i++)
					currentGroupId[i] = nextGroupId[i];
			}
			while(!stopConditionFulfilled);

			/* ToDo: sort elements, return iterator positions*/

			Finish(elementCount);
			return returnValues;

		}

};