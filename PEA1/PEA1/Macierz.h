#pragma once
#include <iostream>
#include <fstream>
#include <ctime>

class Macierz
{
private:
	int** macierz;
	size_t rozmiar;
	bool utworzona;
public:
	Macierz(size_t wielkosc);
	Macierz(const Macierz&);
	~Macierz();

	Macierz& operator=(const Macierz&);
	int* operator()(size_t index_x, size_t index_y);
	
	size_t get_rozmiar() { return rozmiar; }
	bool wczytaj_dane_z_pliku(std::string nazwa_pliku);
	void generuja_dane(unsigned int ilosc_miast, unsigned int min_koszt, unsigned int max_koszt);
	void wyswietl(size_t szerokosc_pola);
};

