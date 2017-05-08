#pragma once
#include <ctime>
#include "Macierz.h"

class Komiwojazer
{
protected:
	Macierz mapa;
public:
	Komiwojazer(size_t rozmiar_mapy);
	Komiwojazer(const Komiwojazer& org);
	~Komiwojazer();

	Komiwojazer& operator=(const Komiwojazer&);

	bool wczytaj_dane_z_pliku(std::string nazwa_pliku);
	void Komiwojazer::generuja_dane(unsigned int ilosc_miast, unsigned int min_koszt, unsigned int max_koszt);
	void wyswietl(size_t szerokosc_pola) { mapa.wyswietl(szerokosc_pola); }

	int* znajdz_min_w_wierszu(size_t wiersz);
	int zredukuj_wiersz(size_t wiersz);
	int zredukuj_wszystkie_wiersze();

	int* znajdz_min_w_kolumnie(size_t kolumna);
	int zredukuj_kolumne(size_t kolumna);
	int zredukuj_wszystkie_kolumny();

	int zredukuj_macierz();

	void wykresl_wiersz(size_t wiersz);
	void wykresl_kolumne(size_t kolumna);

	void wstaw(size_t wiersz, size_t kolumna, int wartosc);
	int get_wartosc(size_t wiersz, size_t kolumna);
	int get_rozmiar_miasta() { return mapa.get_rozmiar(); }
	Macierz get_mapa() { return mapa; }

};

