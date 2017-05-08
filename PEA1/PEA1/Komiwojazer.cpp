#include "Komiwojazer.h"


Komiwojazer::Komiwojazer(size_t rozmiar_mapy) : mapa(rozmiar_mapy)
{
	
}


Komiwojazer::~Komiwojazer()
{
	
}

Komiwojazer::Komiwojazer(const Komiwojazer& org) : mapa(Macierz(org.mapa))
{

}

Komiwojazer& Komiwojazer::operator = (const Komiwojazer& org)
{
	if (this == &org)
		return *this;

	mapa = org.mapa;
}

bool Komiwojazer::wczytaj_dane_z_pliku(std::string nazwa_pliku)
{
	if (mapa.wczytaj_dane_z_pliku(nazwa_pliku))
	{
		std::cout << "Dane z pliku wczytano poprawnie" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Dane z pliku nie zostaly wczytane poprawnie!!!" << std::endl;
		return false;
	}
		
}

int* Komiwojazer::znajdz_min_w_wierszu(size_t wiersz)
{
	int* min = nullptr;
	
	if (mapa.get_rozmiar() == 1)
		return mapa(0, 0);

	for (size_t i = 0; i < mapa.get_rozmiar(); )
	{
		if (*mapa(wiersz, i) < 0)
			i++;
		else
		{
			min = mapa(wiersz, i);
			break;
		}
			
	}

	//w wierszu same -1 (nieskonczonosci)
	if (min == nullptr)
		return min;

	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		//dla -1 pomijamy
		if (*mapa(wiersz, i) < 0)
			continue;

		if (*mapa(wiersz, i) < *min)
			min = mapa(wiersz, i);
			
	}

	return min;
}

int Komiwojazer::zredukuj_wiersz(size_t wiersz)
{
	if (znajdz_min_w_wierszu(wiersz) == nullptr)
		return 0;

	int min = *znajdz_min_w_wierszu(wiersz);

	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		if (*mapa(wiersz, i) < 0)
			continue;
		else
			*mapa(wiersz, i) -= min;
	}

	return min;
}

int Komiwojazer::zredukuj_wszystkie_wiersze()
{
	int suma_min_w_wierszach = 0;

	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		suma_min_w_wierszach += zredukuj_wiersz(i);
	}

	return suma_min_w_wierszach;
}

int* Komiwojazer::znajdz_min_w_kolumnie(size_t kolumna)
{
	int* min = nullptr;

	if (mapa.get_rozmiar() == 1)
		return mapa(0, 0);

	for (size_t i = 0; i < mapa.get_rozmiar();)
	{
		if (*mapa(i, kolumna) < 0)
			i++;
		else
		{
			min = mapa(i, kolumna);
			break;
		}

	}

	//w kolumnie same -1 (nieskonczonosci)
	if (min == nullptr)
		return min;

	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		//dla -1 pomijamy
		if (*mapa(i, kolumna) < 0)
			continue;

		if (*mapa(i, kolumna) < *min)
			min = mapa(i, kolumna);

	}

	return min;
}

int Komiwojazer::zredukuj_kolumne(size_t kolumna)
{
	if (znajdz_min_w_kolumnie(kolumna) == nullptr)
		return 0;

	int min = *znajdz_min_w_kolumnie(kolumna);

	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		if (*mapa(i, kolumna) < 0)
			continue;
		else
			*mapa(i, kolumna) -= min;
	}

	return min;
}

int Komiwojazer::zredukuj_wszystkie_kolumny()
{
	int suma_min_w_kolumnach = 0;

	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		suma_min_w_kolumnach += zredukuj_kolumne(i);
	}

	return suma_min_w_kolumnach;
}

int Komiwojazer::zredukuj_macierz()
{
	int dolne_ograniczenie = 0;

	dolne_ograniczenie += zredukuj_wszystkie_wiersze();
	dolne_ograniczenie += zredukuj_wszystkie_kolumny();

	return dolne_ograniczenie;
}


void Komiwojazer::wykresl_wiersz(size_t wiersz)
{
	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		*mapa(wiersz, i) = -1;
	}
}

void Komiwojazer::wykresl_kolumne(size_t kolumna)
{
	for (size_t i = 0; i < mapa.get_rozmiar(); i++)
	{
		*mapa(i, kolumna) = -1;
	}
}

void Komiwojazer::wstaw(size_t wiersz, size_t kolumna, int wartosc)
{
	*mapa(wiersz, kolumna) = wartosc;
}

int Komiwojazer::get_wartosc(size_t wiersz, size_t kolumna)
{
	return *mapa(wiersz, kolumna);
}

void Komiwojazer::generuja_dane(unsigned int ilosc_miast, unsigned int min_koszt, unsigned int max_koszt)
{
	mapa.generuja_dane(ilosc_miast, min_koszt, max_koszt);
}
