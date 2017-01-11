#pragma once
#include <cmath>
#include <vector>

/// <summary>
/// Obiekt funkcyjny, bêd¹cy miar¹ odleg³oœci pomiêdzy dwoma wartoœciami typu int.
/// </summary>
struct Int_distance
{
	/// <summary>
	/// Przeci¹¿ony operator (), zwracaj¹cy odleg³oœci pomiêdzy dwoma wartoœciami typu int.
	/// </summary>
	/// <param name="p1">Pierwszy wartoœæ typu int.</param>
	/// <param name="p2">Druga wartoœæ typu int.</param>
	/// <returns>Odleg³oœæ pomiêdzy punktami.</returns>
	inline double operator()(int p1, int p2)
	{
		return (double)abs(p1 - p2);
	}
};

/// <summary>
/// Obiekt funkcyjny, obliczaj¹cy uœrednion¹ wartoœæ z n elementów. 
/// </summary>
struct Int_average
{
	/// Wartoœæ domyœlna nowego centroidu.
	int newCentroid = 0;

	/// Licznik kumulowanych wartoœci.
	int count = 0;

	/// <summary>
	/// Przeci¹¿ony operator (), wyliczaj¹cy now¹ wartoœæ centroidu uœrednion¹ z n elementów.
	/// </summary>
	/// <param name="oldCentroid">Aktualna wartoœæ centroidu</param>
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
	/// Przeci¹¿ony operator +=, kumuluj¹cy kolejn¹ wartoœæ.
	/// </summary>
	/// <param name="value">Kolejna kumulowana wartoœæ.</param>
	inline void operator +=(const int & value)
	{
		newCentroid += value;

		count++;
	}
};