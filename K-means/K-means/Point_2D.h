#pragma once

#include <cmath>
#include <stdlib.h>
#include <time.h>     


/// <summary>
/// Typ danych reprezentuj�cy obiekt na przestrzeni euklidesowej (na osiach x i y). 
/// </summary>
class Point_2D
{

	public:
		/// Wsp�rz�dna x w przestrzeni euklidesowej.
		double x;
		/// Wsp�rz�dna y w przestrzeni euklidesowej.
		double y;

		/// <summary>
		/// Bezparametrowy konstruktor, tworz�cy obiekt o losowych wsp�rz�dnych x i y.
		/// </summary>
		Point_2D()
		{
			x = rand() % 5;
			y = rand() % 5;
		}

		/// <summary>
		/// Konstruktor z parametrami, tworz�cy obiekt o okre�lonych wsp�rz�dnych x i y.
		/// </summary>
		/// <param name="_x">Warto�� wsp�rz�dnej x.</param>
		/// <param name="_y">Warto�� wsp�rz�dnej y.</param>
		Point_2D(double _x, double _y)
		{
			x = _x;
			y = _y;
		}
};

/// <summary>
/// Przeci��ony operator <<, umo�liwiaj�cy wypisanie obiektu typu Point_2D na strumie� wyj�ciowy.
/// </summary>
/// <param name="Str">Strumie� wyj�ciowy.</param>
/// <param name="point">Obiek wypisywany.</param>
inline std::ostream & operator<<(std::ostream & Str, const Point_2D& point)
{
	return Str << "(" << point.x << "," << point.y << ")";
};


/// <summary>
/// Obiekt funkcyjny, b�d�cy miar� odleg�o�ci pomi�dzy dwoma warto�ciami typu Point_2D.
/// </summary>
struct Point2D_distance 
{
	/// <summary>
	/// Przeci��ony operator (), zwracaj�cy odleg�o�ci pomi�dzy dwoma warto�ciami typu Point_2D.
	/// </summary>
	/// <param name="p1">Pierwszy warto�� typu Point_2D.</param>
	/// <param name="p2">Druga warto�� typu Point_2D.</param>
	/// <returns>Odleg�o�� pomi�dzy punktami.</returns>
	inline double operator()(Point_2D p1, Point_2D p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y-p2.y,2));
	}
};

/// <summary>
/// Obiekt funkcyjny, obliczaj�cy u�rednion� warto�� z n element�w. 
/// </summary>
struct Point2D_average
{
	/// Warto�� domy�lna nowego centroidu typu Point_2D.
	Point_2D newCentroid = Point_2D(0.0, 0.0);

	/// Licznik kumulowanych warto�ci.
	int count = 0;

	/// <summary>
	/// Przeci��ony operator (), wyliczaj�cy now� warto�� centroidu u�rednion� z n element�w.
	/// </summary>
	/// <param name="oldCentroid">Aktualna warto�� centroidu</param>
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
	/// Przeci��ony operator +=, kumuluj�cy kolejn� warto��.
	/// </summary>
	/// <param name="value">Kolejna kumulowana warto��.</param>
	inline void operator +=(const Point_2D & p)
	{
		newCentroid.x += p.x;
		newCentroid.y += p.y;

		count++;
	}
};