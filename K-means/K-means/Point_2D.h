#pragma once

#include <cmath>
#include <stdlib.h>
#include <time.h>     


/// <summary>
/// Typ danych reprezentuj¹cy obiekt na przestrzeni euklidesowej (na osiach x i y). 
/// </summary>
class Point_2D
{

	public:
		/// Wspó³rzêdna x w przestrzeni euklidesowej.
		double x;
		/// Wspó³rzêdna y w przestrzeni euklidesowej.
		double y;

		/// <summary>
		/// Bezparametrowy konstruktor, tworz¹cy obiekt o losowych wspó³rzêdnych x i y.
		/// </summary>
		Point_2D()
		{
			x = rand() % 5;
			y = rand() % 5;
		}

		/// <summary>
		/// Konstruktor z parametrami, tworz¹cy obiekt o okreœlonych wspó³rzêdnych x i y.
		/// </summary>
		/// <param name="_x">Wartoœæ wspó³rzêdnej x.</param>
		/// <param name="_y">Wartoœæ wspó³rzêdnej y.</param>
		Point_2D(double _x, double _y)
		{
			x = _x;
			y = _y;
		}
};

/// <summary>
/// Przeci¹¿ony operator <<, umo¿liwiaj¹cy wypisanie obiektu typu Point_2D na strumieñ wyjœciowy.
/// </summary>
/// <param name="Str">Strumieñ wyjœciowy.</param>
/// <param name="point">Obiek wypisywany.</param>
inline std::ostream & operator<<(std::ostream & Str, const Point_2D& point)
{
	return Str << "(" << point.x << "," << point.y << ")";
};


/// <summary>
/// Obiekt funkcyjny, bêd¹cy miar¹ odleg³oœci pomiêdzy dwoma wartoœciami typu Point_2D.
/// </summary>
struct Point2D_distance 
{
	/// <summary>
	/// Przeci¹¿ony operator (), zwracaj¹cy odleg³oœci pomiêdzy dwoma wartoœciami typu Point_2D.
	/// </summary>
	/// <param name="p1">Pierwszy wartoœæ typu Point_2D.</param>
	/// <param name="p2">Druga wartoœæ typu Point_2D.</param>
	/// <returns>Odleg³oœæ pomiêdzy punktami.</returns>
	inline double operator()(Point_2D p1, Point_2D p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y-p2.y,2));
	}
};

/// <summary>
/// Obiekt funkcyjny, obliczaj¹cy uœrednion¹ wartoœæ z n elementów. 
/// </summary>
struct Point2D_average
{
	/// Wartoœæ domyœlna nowego centroidu typu Point_2D.
	Point_2D newCentroid = Point_2D(0.0, 0.0);

	/// Licznik kumulowanych wartoœci.
	int count = 0;

	/// <summary>
	/// Przeci¹¿ony operator (), wyliczaj¹cy now¹ wartoœæ centroidu uœrednion¹ z n elementów.
	/// </summary>
	/// <param name="oldCentroid">Aktualna wartoœæ centroidu</param>
	inline void operator()(Point_2D &oldCentroid)
	{
		if (count != 0)
		{
			newCentroid.x /= count;
			newCentroid.y /= count;
		}
		else
		{
			newCentroid.x = oldCentroid.x;
			newCentroid.y = oldCentroid.y;
		}

		oldCentroid.x = newCentroid.x;
		oldCentroid.y = newCentroid.y;

		newCentroid = Point_2D(0.0, 0.0);
		count = 0;
	}

	/// <summary>
	/// Przeci¹¿ony operator +=, kumuluj¹cy kolejn¹ wartoœæ.
	/// </summary>
	/// <param name="value">Kolejna kumulowana wartoœæ.</param>
	inline void operator +=(const Point_2D & p)
	{
		newCentroid.x += p.x;
		newCentroid.y += p.y;

		count++;
	}
};