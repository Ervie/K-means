#pragma once
using namespace std;

/// <summary>
/// Możliwe warunki stopu (zatrzymania algorytmu grupowania K-means.
/// </summary>
enum StopConditions
{
	/// Maksymalna liczba iteracji.
	MaxIterations = 0,
	/// Stan stabilny (brak przejść pomiędzy grupami w trakcie następujących po sobie iteracji).
	StableState = 1,
	/// Uwzględnienie obu warunków.
	Both = 2
};