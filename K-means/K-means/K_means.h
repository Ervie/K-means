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

		typedef typename vector<Iterator>::iterator it;
	public:
		/* Template functions disallow declaration in cpp file */

		

		K_means<Iterator>()
		{
			
		}

		template <typename DistancePredicate>
		Iterator* Group(it first, it last, DistancePredicate &distanceMeasure, bool averagingFunction, int maxIteration, int k, StopConditions stopCondition)
		{

			/*if (first == last)
				return nullptr;*/

			/*if (first - last < k)
				throw std::logic_error("Liczba grup jest wiêksza ni¿ liczba elementów");*/

			return nullptr;
		}

};