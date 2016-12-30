#pragma once

#include "StopConditions.h"
#include <stdexcept>
#include <iterator>
#include <vector>


template <typename T> 
class K_means
{
	public:
		typedef typename vector<T>::iterator Iterator;

	private:
		T* Centroids;
		int* currentGroupId;
		int* nextGroupId;
		double** distancesMatrix;
		Iterator* returnValues;

		double groupMinimum;
		int groupStartIndex;

	public:
		/* Template functions disallow declaration in cpp file */
		K_means<T>()
		{
		}

		// Initialize structures and check input parameters
		void Initialize(int elementCount, int groupNumber)
		{
			if (groupNumber < 1)
				throw std::logic_error("Liczba grup jest mniejsza ni¿ 1");

			if (elementCount < 1)
				throw std::logic_error("Liczba elementów jest mniejsza ni¿ 1");

			if (elementCount < groupNumber)
				throw std::logic_error("Liczba elementów jest mniejsza ni¿ liczba grup");

			groupMinimum = DBL_MAX;
			groupStartIndex = 0;

			Centroids = new T[groupNumber];
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

		// Randomly selects starting points of centroids from data collection
		void AssignStartingPoints(int groupNumber, int elementCount, Iterator first)
		{
			for (int i = 0; i < groupNumber; i++)
			{
				Centroids[i] = *(first + (rand() % elementCount));
			}
		}

		// Free allocated memory
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

		// Display all elements from range using output stream (data type must gave << operator)
		void DisplayCollection(Iterator first, Iterator last)
		{
			int elementCount = distance(first, last);

			for (int i = 0; i < elementCount; i++)
			{
				cout <<  "[" << i << "]: " <<*(first + i) << endl;
			}
			cout << endl;
		}

		// Dispatcher for Random Access Iterator
		template <typename Iterator, typename DistancePredicate, typename AveragePredicate>
		Iterator* Group(Iterator first, Iterator last, DistancePredicate &distanceMeasure, AveragePredicate &groupAverage, int maxIteration, int k, StopConditions stopCondition)
		{
			typedef typename std::iterator_traits<Iterator>::iterator_category category;
			return Group(first, last, category(), distanceMeasure, groupAverage, maxIteration, k, stopCondition);
		}

		// Main function, sorts elements in range by their cluster and return array of iterator pointing at begining of each cluster
		template <typename DistancePredicate, typename AveragePredicate>
		Iterator* Group(Iterator first, Iterator last, std::random_access_iterator_tag, DistancePredicate &distanceMeasure, AveragePredicate &groupAverage, int maxIteration, int k, StopConditions stopCondition)
		{
			int elementCount = distance(first, last);
			int iterationCounter = 0;
			bool stopConditionFulfilled = false;

			Initialize(elementCount, k);

			AssignStartingPoints(k, elementCount, first);

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
					Centroids[i] = groupAverage(first, i, nextGroupId, elementCount, Centroids[i]);
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