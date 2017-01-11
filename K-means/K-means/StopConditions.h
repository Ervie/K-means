#pragma once
using namespace std;

/// <summary>
/// Mo�liwe warunki stopu (zatrzymania algorytmu grupowania K-means.
/// </summary>
enum StopConditions
{
	/// Maksymalna liczba iteracji.
	MaxIterations = 0,
	/// Stan stabilny (brak przej�� pomi�dzy grupami w trakcie nast�puj�cych po sobie iteracji).
	StableState = 1,
	/// Uwzgl�dnienie obu warunk�w.
	Both = 2
};