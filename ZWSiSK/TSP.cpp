#include "TSP.h"

long long TSP::testuj(int size)
{
	TSP *k = new TSP();
	//k->losuj(size);

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// Activity to be timed
	//////////////////////////////////std::vector<int>* a = k->BoundAndBranch();
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	delete k;
	//////////delete a;
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