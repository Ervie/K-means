#pragma once

#include "StopConditions.h"
#include <stdexcept>
#include <iterator>
#include <vector>

/// <summary>
/// Klasa implementuj�ca algorytm K-means. 
/// </summary>
/// <type name="T">Typ danych </param>
template <typename T> 
class K_means
{
	public:
		/// Iterator swobodnego dost�pu dla typu danych T.
		typedef typename std::vector<T>::iterator returnIterator;


	private:
		/// Wsp�rz�dne centroid�w (�rodk�w grup).
		T* Centroids;

		/// Tablica przechowuj�ca informacj� o aktualnej przynale�no�ci danych do grup.
		int* currentGroupId;

		/// Tablica przechowuj�ca informacj� o przynale�no�ci danych do grup w nast�pnej iteracji.
		int* nextGroupId;

		/// Macierz warto�ci okre�laj�ca odleg�o�� danych od �rodk�w grup.
		double** distancesMatrix;

		/// Wektor iterator�w wskazuj�cych na kolejne pocz�tki grup.
		returnIterator* returnValues;

		/// Licznik iteracji algorytmu, s�u�y do sprawdzania warunku stopu.
		int iterationCounter;

		/// Loczba grup podanych przez u�ytkownika.
		int groupNumber;

		/// Liczba element�w znajduj�cych si� w zakresie okre�lonym iteratorami pocz�tku i ko�ca.
		long elementCount;
		
		/// Flaga okre�laj�ca, czy zosta� spe�niony warunek stopu.
		bool stopConditionFulfilled;

		/// Zmienna pomocnicza przechowuj�ca informacj� o najmniejszej odleg�o�ci punktu po�r�d wszystkich odleg�o�ci pomi�dzy punktem a centroidami.
		double groupMinimum;

		/// Zmienna pomocnicza u�ywana w trakcie obliczania indeksu elementu wskazywanego przez iterator pocz�tku grupy.
		int groupStartIndex;

	public:
		/// <summary>
		/// Konstruktor bezparametrowy.
		/// </summary>
		K_means<T>()
		{
		}

		/// <summary>
		/// Wy�wietlenie wszystkich element�w w zakresie. Typ element�w musi posiada� przeci��ony/przes�oni�ty operator <<.
		/// </summary>
		/// <param name="first">Pocz�tek zakresu.</param>
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

		/// Dyspozytor Iteratora swobodnego dost�pu. Stanowi zabezpiecznie przed u�yciem funkcji dla innych typ�w iterator�w.
		template <typename Iterator, typename DistancePredicate, typename AveragePredicate>
		Iterator* Group(Iterator first, Iterator last, DistancePredicate &distanceMeasure, AveragePredicate &groupAverage, int maxIteration, int k, StopConditions stopCondition, bool printOutput = false)
		{
			typedef typename std::iterator_traits<Iterator>::iterator_category category;
			return Group(first, last, category(), distanceMeasure, groupAverage, maxIteration, k, stopCondition, printOutput);
		}


private:
		///<summary>
		/// G��wna funkcja implementuj�ca algorytm grupowania metod� k-�rednich.
		///</summary>
		/// <param name="first">Pocz�tek grupowanego zakresu.</param>
		/// <param name="last">Koniec grupowanego zakresu.</param>
		/// <param name="random_access_iterator_tag">Tag dla iteratora swobodnego dost�pu.</param>
		/// <param name="distanceMeasure">Obiekt funkcyjny stanowi�cy miar� odleg�o�ci dla typu T.</param>
		/// <param name="groupAverage">Obiekt funkcyjny b�d�cy funkcj� u�redniaj�c� dla typu T.</param>
		/// <param name="maxIteration">Maksymalna liczba iteracji algorytmu.</param>
		/// <param name="k">Liczba grup.</param>
		/// <param name="stopCondition">Warunek stopu.</param>
		/// <param name="printOutput">Flaga okre�laj�ca, czy informacje o aktualnej iteracji maj� zosta� wyprowadzone na strumie� wyj�ciowy.</param>
		/// <returns>Wektor iterator�w sowobodnego dost�pu wskazuj�cych na kolejne pocz�tki grup pogrupowanych danych.</returns>
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
		/// Sprawdzenie poprawno�ci podanych danych oraz alokacja potrzebnej pami�ci na struktury pomocnicze.
		///</summary>
		void Initialize()
		{
			if (groupNumber < 1)
				throw std::logic_error("Liczba grup jest mniejsza ni� 1");

			if (elementCount < 1)
				throw std::logic_error("Liczba element�w jest mniejsza ni� 1");

			if (elementCount < groupNumber)
				throw std::logic_error("Liczba element�w jest mniejsza ni� liczba grup");

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
		/// Wygenerowanie pocz�tkowych warto�ci centroid�w metod� wyboru losowaych warto�ci po�r�d danych
		///</summary>
		///<parameter name="first">Pocz�tek grupowanego parametru.</parameter>
		template <typename Iterator>
		void AssignStartingPoints(Iterator first)
		{
			for (int i = 0; i < groupNumber; i++)
			{
				Centroids[i] = *(first + (rand() % elementCount));
			}
		}

		///<summary>
		/// Wy�wietlenie informacji o aktualnej przynale�no�ci danych do grup oraz wsp�rz�dnych centroid�w.
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
		/// Dealokacja pami�ci.
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