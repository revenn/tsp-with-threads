#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Cities
{
public:
	unsigned int rozmiar;
	int **mileage_table;

	Cities()
	{
	}

	~Cities()
	{
	}

	bool wczytaj_dane_z_pliku(std::string nazwa_pliku)
	{
		std::ifstream plik;
		plik.open(nazwa_pliku.c_str());
		if (!plik.good())
			return false;

		int poprzedni_rozmiar = this->rozmiar;
		plik >> this->rozmiar;

		if (this->rozmiar <= 0)
			std::cout << "Blad odczytu danych!" << std::endl;

		if (poprzedni_rozmiar > 0)
		{
			//usuniecie dotychczasowej mapy miast
			if (mileage_table != nullptr)
			{
				for (int i = 0; i < poprzedni_rozmiar; i++)
				{
					delete[] mileage_table[i];
				}
				delete[] mileage_table;
				mileage_table = nullptr;
			}

		}
		//stworzenie nowej mapy miast na podstawie ilosc miast zapisanej w pliku
		mileage_table = new int *[rozmiar];
		for (int i = 0; i < rozmiar; i++)
		{
			mileage_table[i] = new int[rozmiar];
		}


		int i = 0, j = 0;
		while (true)
		{
			int koszt;
			if (plik.good())
			{
				if (j == rozmiar)
				{
					j = 0;
					i++;
				}
				if (i == rozmiar)
					break;
				if (i == j)
					koszt = 0;
				else
					plik >> koszt;
				mileage_table[i][j] = koszt;
				j++;
			}
			else
				break; //zakoncz wczytywanie danych - wystapil jakis blad (np. nie ma wiecej danych w pliku)

		}
		plik.close();
		return true;

	}

	void generuja_dane(unsigned int ilosc_miast, unsigned int min_koszt, unsigned int max_koszt)
	{
		int poprzedni_rozmiar = this->rozmiar;
		this->rozmiar = ilosc_miast;

		if (this->rozmiar <= 0)
			std::cerr << "Blad odczytu danych!" << std::endl;

		if (poprzedni_rozmiar > 0)
		{
			//usuniecie dotychczasowej mapy miast
			if (mileage_table != nullptr)
			{
				for (int i = 0; i < poprzedni_rozmiar; i++)
				{
					delete[] mileage_table[i];
				}
				delete[] mileage_table;
				mileage_table = nullptr;
			}

		}
		//stworzenie nowej mapy miast na podstawie ilosc miast zapisanej w pliku
		mileage_table = new int *[rozmiar];
		for (int i = 0; i < rozmiar; i++)
		{
			mileage_table[i] = new int[rozmiar];
		}


		srand(time(NULL));
		for (int i = 0; i < ilosc_miast; i++)
		{
			for (int j = 0; j < ilosc_miast; j++)
			{
				if (i == j)
					mileage_table[i][j] = 0;
				else
					mileage_table[i][j] = min_koszt + rand() % (max_koszt - min_koszt + 1);
			}
		}

	}
};

