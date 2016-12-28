#pragma once

#include "StopConditions.h"
#include <stdexcept>
#include <iterator>
#include <vector>

using namespace std;

template <typename Iterator> 
class K_means
{
	public:
		typedef typename vector<Iterator>::iterator it;

	private:
		Iterator* Centroids;
		int* currentGroupId;
		int* nextGroupId;
		double** distancesMatrix;
		it* returnValues;

		double groupMinimum;
		int groupStartIndex;

	public:
		/* Template functions disallow declaration in cpp file */

		

		K_means<Iterator>()
		{
			
		}

		void Init(int elementCount, int groupNumber)
		{
			if (elementCount < 1)
				throw std::logic_error("Liczba elementów jest mniejsza ni¿ 1");

			groupMinimum = DBL_MAX;
			groupStartIndex = 0;

			Centroids = new Iterator[groupNumber];
			returnValues = new it[groupNumber];
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

		void Finish(int groupNumber)
		{
			delete Centroids;
			delete currentGroupId;
			delete nextGroupId;

			for (int i = 0; i < groupNumber; i++)
			{
				delete distancesMatrix[i];
			}

			delete [] distancesMatrix;
		}

		void DisplayCollection(it first, it last)
		{
			int elementCount = distance(first, last);

			for (int i = 0; i < elementCount; i++)
			{
				cout <<  "[" << i << "]: " <<*(first + i) << endl;
			}
		}

		template <typename DistancePredicate, typename AveragePredicate>
		it* Group(it first, it last, DistancePredicate &distanceMeasure, AveragePredicate &averagingFunction, int maxIteration, int k, StopConditions stopCondition)
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
					Centroids[i] = averagingFunction(first, i, nextGroupId, elementCount, Centroids[i]);
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

			/* Sort elements, calculate iterator positions*/


			for (int i = 0; i < k; i++)
			{
				returnValues[i] = first + groupStartIndex;

				for (int j = groupStartIndex; j < elementCount; j++)
				{
					if (currentGroupId[j] == i)
					{
						swap(currentGroupId[j], currentGroupId[groupStartIndex]);
						iter_swap(first + j, first + groupStartIndex);
						groupStartIndex++;
					}
				}

			}

			Finish(k);
			return returnValues;

		}

};