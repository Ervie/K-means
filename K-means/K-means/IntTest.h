#pragma once
#include <cmath>
#include <vector>

/// <summary>
/// Obiekt funkcyjny, b�d�cy miar� odleg�o�ci pomi�dzy dwoma warto�ciami typu int.
/// </summary>
struct Int_distance
{
	/// <summary>
	/// Przeci��ony operator (), zwracaj�cy odleg�o�ci pomi�dzy dwoma warto�ciami typu int.
	/// </summary>
	/// <param name="p1">Pierwszy warto�� typu int.</param>
	/// <param name="p2">Druga warto�� typu int.</param>
	/// <returns>Odleg�o�� pomi�dzy punktami.</returns>
	inline double operator()(int p1, int p2)
	{
		return (double)abs(p1 - p2);
	}
};

/// <summary>
/// Obiekt funkcyjny, obliczaj�cy u�rednion� warto�� z n element�w. 
/// </summary>
struct Int_average
{
	/// Warto�� domy�lna nowego centroidu.
	int newCentroid = 0;

	/// Licznik kumulowanych warto�ci.
	int count = 0;

	/// <summary>
	/// Przeci��ony operator (), wyliczaj�cy now� warto�� centroidu u�rednion� z n element�w.
	/// </summary>
	/// <param name="oldCentroid">Aktualna warto�� centroidu</param>
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

	/// <summary>
	/// Przeci��ony operator +=, kumuluj�cy kolejn� warto��.
	/// </summary>
	/// <param name="value">Kolejna kumulowana warto��.</param>
	inline void operator +=(const int & value)
	{
		newCentroid += value;

		count++;
	}
};