#pragma once

#include "StopConditions.h"
#include <stdexcept>
#include <iterator>
#include <vector>

/// <summary>
/// Klasa implementuj¹ca algorytm K-means. 
/// </summary>
/// <type name="T">Typ danych </param>
template <typename T> 
class K_means
{
	public:
		/// Iterator swobodnego dostêpu dla typu danych T.
		typedef typename std::vector<T>::iterator returnIterator;


	private:
		/// Wspó³rzêdne centroidów (œrodków grup).
		T* Centroids;

		/// Tablica przechowuj¹ca informacjê o aktualnej przynale¿noœci danych do grup.
		int* currentGroupId;

		/// Tablica przechowuj¹ca informacjê o przynale¿noœci danych do grup w nastêpnej iteracji.
		int* nextGroupId;

		/// Macierz wartoœci okreœlaj¹ca odleg³oœæ danych od œrodków grup.
		double** distancesMatrix;

		/// Wektor iteratorów wskazuj¹cych na kolejne pocz¹tki grup.
		returnIterator* returnValues;

		/// Licznik iteracji algorytmu, s³u¿y do sprawdzania warunku stopu.
		int iterationCounter;

		/// Loczba grup podanych przez u¿ytkownika.
		int groupNumber;

		/// Liczba elementów znajduj¹cych siê w zakresie okreœlonym iteratorami pocz¹tku i koñca.
		long elementCount;
		
		/// Flaga okreœlaj¹ca, czy zosta³ spe³niony warunek stopu.
		bool stopConditionFulfilled;

		/// Zmienna pomocnicza przechowuj¹ca informacjê o najmniejszej odleg³oœci punktu poœród wszystkich odleg³oœci pomiêdzy punktem a centroidami.
		double groupMinimum;

		/// Zmienna pomocnicza u¿ywana w trakcie obliczania indeksu elementu wskazywanego przez iterator pocz¹tku grupy.
		int groupStartIndex;

	public:
		/// <summary>
		/// Konstruktor bezparametrowy.
		/// </summary>
		K_means<T>()
		{
		}

		/// <summary>
		/// Wyœwietlenie wszystkich elementów w zakresie. Typ elementów musi posiadaæ przeci¹¿ony/przes³oniêty operator <<.
		/// </summary>
		/// <param name="first">Pocz¹tek zakresu.</param>
		/// <param name="first">Koniec zakresu.</param>
		template <typename Iterator>
		void DisplayCollection(Iterator first, Iterator last)
		{
			int elementCount = distance(first, last);

			for (int i = 0; i < elementCount; i++)
			{
				cout <<  "[" << i << "]: " <<*(first + i) << endl;
			}
			cout << endl;
		}

		/// Dyspozytor Iteratora swobodnego dostêpu. Stanowi zabezpiecznie przed u¿yciem funkcji dla innych typów iteratorów.
		template <typename Iterator, typename DistancePredicate, typename AveragePredicate>
		Iterator* Group(Iterator first, Iterator last, DistancePredicate &distanceMeasure, AveragePredicate &groupAverage, int maxIteration, int k, StopConditions stopCondition, bool printOutput = false)
		{
			typedef typename std::iterator_traits<Iterator>::iterator_category category;
			return Group(first, last, category(), distanceMeasure, groupAverage, maxIteration, k, stopCondition, printOutput);
		}


private:
		///<summary>
		/// G³ówna funkcja implementuj¹ca algorytm grupowania metod¹ k-œrednich.
		///</summary>
		/// <param name="first">Pocz¹tek grupowanego zakresu.</param>
		/// <param name="last">Koniec grupowanego zakresu.</param>
		/// <param name="random_access_iterator_tag">Tag dla iteratora swobodnego dostêpu.</param>
		/// <param name="distanceMeasure">Obiekt funkcyjny stanowi¹cy miarê odleg³oœci dla typu T.</param>
		/// <param name="groupAverage">Obiekt funkcyjny bêd¹cy funkcj¹ uœredniaj¹c¹ dla typu T.</param>
		/// <param name="maxIteration">Maksymalna liczba iteracji algorytmu.</param>
		/// <param name="k">Liczba grup.</param>
		/// <param name="stopCondition">Warunek stopu.</param>
		/// <param name="printOutput">Flaga okreœlaj¹ca, czy informacje o aktualnej iteracji maj¹ zostaæ wyprowadzone na strumieñ wyjœciowy.</param>
		/// <returns>Wektor iteratorów sowobodnego dostêpu wskazuj¹cych na kolejne pocz¹tki grup pogrupowanych danych.</returns>
		template <typename Iterator, typename DistancePredicate, typename AveragePredicate>
		returnIterator* Group(Iterator first, Iterator last, std::random_access_iterator_tag, DistancePredicate &distanceMeasure, AveragePredicate &groupAverage, int maxIteration, int k, StopConditions stopCondition, bool printOutput)
		{
			elementCount = distance(first, last);
			groupNumber = k;

			iterationCounter = 0;
			stopConditionFulfilled = false;

			Initialize();

			AssignStartingPoints(first);

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
					for (int j = 0; j < elementCount; j++)
					{
						if (nextGroupId[j] == i)
						{
							groupAverage += *(first + j);
						}
					}

					groupAverage(Centroids[i]);
				}

				iterationCounter++;

				/* Check end condition */

				if (stopCondition == MaxIterations || stopCondition == Both)
					stopConditionFulfilled = (iterationCounter >= maxIteration);
				else if (stopCondition == StableState || stopCondition == Both)
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


				/* Print information about current iteration */

				if (printOutput)
					DisplayCurrentIterationState();

			} while (!stopConditionFulfilled);

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

			Finish();
			return returnValues;

		}

		///<summary>
		/// Sprawdzenie poprawnoœci podanych danych oraz alokacja potrzebnej pamiêci na struktury pomocnicze.
		///</summary>
		void Initialize()
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
			returnValues = new returnIterator[groupNumber];
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

		///<summary>
		/// Wygenerowanie pocz¹tkowych wartoœci centroidów metod¹ wyboru losowaych wartoœci poœród danych
		///</summary>
		///<parameter name="first">Pocz¹tek grupowanego parametru.</parameter>
		template <typename Iterator>
		void AssignStartingPoints(Iterator first)
		{
			for (int i = 0; i < groupNumber; i++)
			{
				Centroids[i] = *(first + (rand() % elementCount));
			}
		}

		///<summary>
		/// Wyœwietlenie informacji o aktualnej przynale¿noœci danych do grup oraz wspó³rzêdnych centroidów.
		///</summary>
		void DisplayCurrentIterationState()
		{
			cout << "Iteration " << iterationCounter << ":" << endl;

			cout << "\tGroups:" << endl;

			for (int i = 0; i < elementCount; i++)
			{
				cout << "\t\tElement [" << i << "]: " << currentGroupId[i] << endl;
			}

			cout << "\tCentroids:" << endl;

			for (int i = 0; i < groupNumber; i++)
			{
				cout << "\t\tCentroid [" << i << "]: " << Centroids[i] << endl;
			}

			cout << endl;
		}

		///<summary>
		/// Dealokacja pamiêci.
		///</summary>
		void Finish()
		{
			delete[] Centroids;
			delete[] currentGroupId;
			delete[] nextGroupId;

			for (int i = 0; i < groupNumber; i++)
			{
				delete[] distancesMatrix[i];
			}

			delete [] distancesMatrix;
		}
};