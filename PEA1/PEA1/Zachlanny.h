#pragma once
#include "Macierz.h"
#include <vector>
#include <iostream>

class Zachlanny
{
public:
	Zachlanny(Macierz mapa);
	~Zachlanny();

	void perm(size_t k);
	void swap(size_t* mapa, size_t i, size_t k);

	int oblicz_aktualny_koszt();
	size_t znajdz_trase();

private:
	Macierz mapa;
	std::vector<size_t> trasa;
	int rozmiar;
	size_t* miasta;
	int koszt;
};

