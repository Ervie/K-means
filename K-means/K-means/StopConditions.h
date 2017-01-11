#pragma once
using namespace std;

/// <summary>
/// Mo¿liwe warunki stopu (zatrzymania algorytmu grupowania K-means.
/// </summary>
enum StopConditions
{
	/// Maksymalna liczba iteracji.
	MaxIterations = 0,
	/// Stan stabilny (brak przejœæ pomiêdzy grupami w trakcie nastêpuj¹cych po sobie iteracji).
	StableState = 1,
	/// Uwzglêdnienie obu warunków.
	Both = 2
};