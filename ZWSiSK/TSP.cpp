#pragma once

#include "TSP.h"

long long TSP::testuj(int size)
{
	TSP *k = new TSP();
	k->losuj(size);

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// Activity to be timed
	std::vector<int>* a = k->BoundAndBranch();
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;



	//
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	delete k;
	delete a;
	return ElapsedMicroseconds.QuadPart;
}

void TSP::test()
{
	std::ofstream plik("Test18-20.txt");
	plik << "ilosc miast;czas;" << std::endl;
	for (int n = 18; n <= 20; n += 2)
	{
		for (int i = 0; i<100; ++i)
		{
			plik << n << " " << testuj(n) << std::endl;
		}

	}
	plik.close();
}

bool TSP::wczytaj(std::string path)
{
	std::fstream plik;
	plik.open(path, std::ios::in);
	if (plik.good())
	{
		std::string line;
		getline(plik, line);

		int rozmiar = stoi(line); //konwert string->int

		//sprawdzanie poprawnosci rozmiaru/ilosci miast
		if (rozmiar<0)
		{
			std::cout << "Rozmiar ujemny\n";
			plik.close();
			return false;
		}

		//Jezeli istnieje to zastepuje aktualny graf
		delete macierz;
		macierz = new Matrix(rozmiar);
		size = rozmiar;

		std::string temp;

		for (int i = 0; i<rozmiar; ++i)
		{
			//Bedzie przechowywac wierzcholek poczatkowy, koncowy i wage krawedzi
			getline(plik, line);
			std::stringstream stream(line);

			int j = 0;
			//przesuwanie po kolejnych bajtach
			while (stream >> temp)
			{
				//if (i == j)
				//	++j;
				int waga = stoi(temp);

				//koszt nie mo¿e byæ ujemny!
				if (i != j && waga<0)
				{
					delete macierz;
					std::cout << "Waga ujemna\n";
					plik.close();
					return false;
				}
				(*macierz)(i, j) = waga;
				//macierz->waga += waga;
				++j;
			}
		}
		plik.close();
		return true;
	}
	else
	{
		plik.close();
		return false;
	}
}
void TSP::losuj(unsigned rozmiar)
{
	if (rozmiar > 0)
	{
		//Zastepuje istniejacy graf
		delete macierz;
		macierz = new Matrix(rozmiar);
		size = rozmiar;

		int waga = 0;

		srand(time(NULL));

		for (int a = 0; a < rozmiar; ++a)
			for (int i = 0; i < rozmiar; ++i)
			{
			if (a == i) // na przekatnej bêdzie -1
				waga = -1;
			else
				waga = rand() % 99 + 1;

			//krawedzie skierowane
			(*macierz)(a, i) = waga;
			//macierz->waga += waga;
			}
	}
}
void TSP::wyswietl(Matrix* s)
{
	if (size>0)
	{
		std::cout << "Macierz\n\n" << "   ";
		for (int i = 0; i<size; i++)
			std::cout << std::setw(1) << "(" << i << ")";

		std::cout << std::endl;

		for (int i = 0; i<size; i++)
		{
			std::cout << std::setw(1) << "(" << i << ")";
			for (int j = 0; j<size; j++)
			{
				/*int waga = -1;
				if ((*s)(i, j)) // jezeli X x Y nie jest nullem
				waga = (*s)(i, j)->waga;*/

				std::cout << std::setw(3) << (*s)(i, j);
			}
			std::cout << std::endl;
		}
		//std::cout << "Waga calkowita: " << s->waga << std::endl;
	}
	else
		std::cout << "Graf nie zawiera wierzcholkow\n";
}

std::vector<int>* TSP::BoundAndBranch()
{
	std::vector<int>* najlepszaTrasa = new std::vector<int>();
	std::vector<int> dzieci, aktualnaTrasa;

	// zaczynamy od korzenia
	aktualnaTrasa.push_back(poczatek);
	int kosztNajlepszejTrasy = INT_MAX;

	// poczatku nie wrzucamy do kontenera dzieci bo to rodzic
	for (int i = 0; i < size; i++)
	{
		if (i == poczatek)
			continue;

		dzieci.push_back(i);
	}

	// wywolanie przegladu
	przeszukiwanie(macierz, &aktualnaTrasa, najlepszaTrasa, &dzieci, &kosztNajlepszejTrasy);

	return najlepszaTrasa;
}

void TSP::przeszukiwanie(Matrix* matrix, std::vector<int>* aktualnaTrasa, std::vector<int>* najlepszaTrasa, std::vector<int>* dzieci, int* kosztNajlepszejTrasy)
{
	if (!dzieci->empty())
	{
		//Obliczanie dolnej granicy wezla
		int dolnaGranica = lowBound(matrix, aktualnaTrasa);

		//Przerywamy, jezeli nie otrzymamy rozwiazania o koszcie nizszym od dolnej granicy
		if (*kosztNajlepszejTrasy <= dolnaGranica)
			return;

		for (int i = 0; i < dzieci->size(); i++)
		{
			//wybieramy i usuwamy dziecko z listy
			int wezel = dzieci->at(i);
			dzieci->erase(dzieci->begin() + i);
			aktualnaTrasa->push_back(wezel); // dopisujemy do aktualnej trasy dzieci

			//przeszukiwanie wglab
			przeszukiwanie(matrix, aktualnaTrasa, najlepszaTrasa, dzieci, kosztNajlepszejTrasy);

			//wracamy po galezi w gore = przywracamy dziecko na liste
			dzieci->insert(dzieci->begin() + i, wezel);
			aktualnaTrasa->pop_back();
		}
	}
	else //jestesmy w lisciu
	{
		//porownujemy z dotad najlepszym rozwiazaniem
		int aktualnyKoszt = koszt(aktualnaTrasa, matrix);
		if (aktualnyKoszt < *kosztNajlepszejTrasy)
		{
			delete najlepszaTrasa;
			najlepszaTrasa = new std::vector<int>(*aktualnaTrasa);

			*kosztNajlepszejTrasy = aktualnyKoszt;
		}

	}
}

int TSP::koszt(std::vector<int> *aktualnaTrasa, Matrix *matrix)
{
	int wynik = 0;

	if (aktualnaTrasa->size() <= 1)
	{
		wynik = INT_MAX;
		return wynik;
	}

	for (int i = 0; i < aktualnaTrasa->size() - 1; i++)
	{
		wynik += (*matrix)(aktualnaTrasa->at(i), aktualnaTrasa->at(i + 1));
	}

	//zamykanie trasy
	wynik += (*matrix)(aktualnaTrasa->back(), aktualnaTrasa->front());

	return wynik;
}

int TSP::lowBound(Matrix* matrix, std::vector<int>* aktualnaTrasa)
{
	int bound = 0;

	std::vector<bool> odwiedzoneWierzcholki(size);
	std::vector<int> pozostaleWierzcholki;

	//Wyznaczanie krawedzi odwiedzonych
	for (int i = 0; i < aktualnaTrasa->size() - 1; i++)
	{
		odwiedzoneWierzcholki[aktualnaTrasa->at(i)] = true;
		bound += (*matrix)(aktualnaTrasa->at(i), aktualnaTrasa->at(i + 1));
	}

	odwiedzoneWierzcholki[aktualnaTrasa->back()] = true;

	//Wierzcholki, ktore nie znajduja sie w przebytej trasie
	for (int i = 0; i < odwiedzoneWierzcholki.size(); i++)
		if (!odwiedzoneWierzcholki[i])
			pozostaleWierzcholki.push_back(i);

	//Sprawdzamy krawedzie wychodzace z ostatniego wierzcholka aktualnej trasy
	int minKrawedz = INT_MAX;
	int minI = 0;

	for (int i = 0; i < pozostaleWierzcholki.size(); i++)
	{
		if ((*matrix)(aktualnaTrasa->back(), pozostaleWierzcholki[i]) < minKrawedz) //szukamy minimum
		{
			minKrawedz = (*matrix)(aktualnaTrasa->back(), pozostaleWierzcholki[i]);
			minI = i;
		}

	}

	bound += minKrawedz;
	pozostaleWierzcholki.erase(pozostaleWierzcholki.begin() + minI);

	//Umozliwiamy sprawdzenie krawedzi do poczatku podrozy
	pozostaleWierzcholki.push_back(aktualnaTrasa->at(0));

	minKrawedz = INT_MAX;

	//dodajemy do granicy minimalne krawedzie dla pozostalych wierzcholkow
	for (int i = 0; i < pozostaleWierzcholki.size(); i++)
	{
		for (int j = 0; j < pozostaleWierzcholki.size() - 1; j++)
		{
			if (i == j) // -1
				continue;
			if ((*matrix)(pozostaleWierzcholki[j], pozostaleWierzcholki[i]) < minKrawedz)
				minKrawedz = (*matrix)(pozostaleWierzcholki[j], pozostaleWierzcholki[i]);
		}
		bound += minKrawedz;
	}

	return bound;
}

std::vector<int>* TSP::przegladZupelny()
{
	std::vector<int>* najlepszaTrasa = new std::vector<int>();

	return najlepszaTrasa;
}