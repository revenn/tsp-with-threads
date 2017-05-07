#pragma once

class Krawedz
{
public:
	//wp - poczatek, wk - koniec, waga - koszt przebycia drogi
	const int wp, wk, waga;
	Krawedz(int p, int k, int w) : wp(p), wk(k), waga(w)
	{}
};