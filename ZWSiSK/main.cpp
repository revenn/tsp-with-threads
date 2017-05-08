#include <iostream>
#include <string>
#include "TSP.h"

int main(int argc, char* argv[])
{
	int choose;
	TSP* a = new TSP();

	do
	{
		system("cls");
		std::cout << "TRAVELLING SALESMAN PROBLEM\nWybierz, co zrobic:\n\n1 - Zbuduj z pliku\n2 - Utworz losowo\n3 - Wyswietl\n4 - Algorytm B&B bez wielowatkowosci\n5 - Algorytm przegladu zupelnego bez wielowatkowosci\n6 - Zakoncz" << std::endl;
		std::cin >> choose;

		switch (choose)
		{
		case 1:
		{
			std::string path;
			std::cout << "Podaj nazwe pliku z jego rozszerzeniem\n";
			std::cin >> path;
			if (a->wczytaj(path))
			{
				std::cout << "Utworzono pomyslnie!\n";
				a->wyswietl(a->macierz);
			}
			else
				std::cout << "Odczyt pliku nie powiodl sie\n";
			system("Pause");
			break;
		}
		case 2:
		{
			int size;
			std::cout << "Podaj ilosc wierzcholkow grafu\n";
			std::cin >> size;
			if (size>0)
			{
				a->losuj(size);
				std::cout << "Utworzono pomyslnie!\n";
				a->wyswietl(a->macierz);
			}
			else
				std::cout << "Blad! Rozmiar i gestosc musza byc wieksze od zera. Operacji nie wykonano.\n";
			system("Pause");
			break;
		}
		case 3:
		{
			a->wyswietl(a->macierz);
			system("Pause");
			break;
		}
		case 4:
		{
			std::vector<int>* wynik = a->BoundAndBranch();
			for (int i = 0; i < wynik->size(); ++i)
				std::cout << wynik->at(i) << " -> ";

			std::cout << wynik->at(0) << std::endl;
			std::cout << "Koszt trasy wynosi: " << a->koszt(wynik, a->macierz) << std::endl;

			delete(wynik);
			system("Pause");
			break;
		}
		case 5:
		{
			break;
		}

		case 9:
		{
			a->test();
		}
		}

	} while (choose != 6);

	return 0;
}