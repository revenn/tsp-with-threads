#include "Macierz.h"


Macierz::Macierz(size_t wielkosc)
{
	if (wielkosc > 0)
	{
		rozmiar = wielkosc;

		macierz = new int *[rozmiar];
		for (int i = 0; i < rozmiar; i++)
		{
			macierz[i] = new int[rozmiar];
		}

		utworzona = true;
	}
	else
	{
		utworzona = false;
		wielkosc = 0;
		macierz = nullptr;
		std::cerr << "Macierz nie zostala utworzona!" << std::endl;
	}

}


Macierz::~Macierz()
{
	if (macierz != nullptr && rozmiar > 0)
	{
		for (int i = 0; i < rozmiar; i++)
		{
			delete[] macierz[i];
		}
		delete[] macierz;

		macierz = nullptr;
		rozmiar = 0;
	}
}

Macierz::Macierz(const Macierz& org)
{
	if (org.utworzona)
	{
		this->rozmiar = org.rozmiar;

		this->macierz = new int *[this->rozmiar];
		for (int i = 0; i < this->rozmiar; i++)
		{
			this->macierz[i] = new int[this->rozmiar];
		}

		for (size_t i = 0; i < this->rozmiar; i++)
		{
			for (size_t j = 0; j < this->rozmiar; j++)
			{
				this->macierz[i][j] = org.macierz[i][j];
			}
		}

		utworzona = true;
	}
	else
	{
		std::cerr << "Nie mozna kopiowac nieutworzonej macierzy!" << std::endl;
		utworzona = false;
	}

}
Macierz& Macierz::operator=(const Macierz& org)
{
	if (!org.utworzona)
	{
		std::cerr << "Nie mozna przypisywac nie utworzonej macierzy!" << std::endl;
		return *this;
	}

	if (this == &org)
		return *this;

	if (this->macierz != nullptr && this->macierz > 0)
	{
		for (int i = 0; i < rozmiar; i++)
		{
			delete[] macierz[i];
		}
		delete[] macierz;
	}

	this->rozmiar = org.rozmiar;

	this->macierz = new int *[this->rozmiar];
	for (int i = 0; i < this->rozmiar; i++)
	{
		this->macierz[i] = new int[this->rozmiar];
	}

	for (size_t i = 0; i < this->rozmiar; i++)
	{
		for (size_t j = 0; j < this->rozmiar; j++)
		{
			this->macierz[i][j] = org.macierz[i][j];
		}
	}

	return *this;
}

int* Macierz::operator()(size_t index_x, size_t index_y)
{
	if (!this->utworzona)
	{
		std::cerr << "Nie wolno pracowac na nieutworzonej macierzy" << std::endl;
		throw std::out_of_range("Nie wolno pracowac na nieutworzonej macierzy");
	}

	if ((index_x >= 0 && index_x < rozmiar) && (index_y >= 0 && index_y < rozmiar))
		return &macierz[index_x][index_y];
	else
	{
		std::cerr << "Wykroczono poza macierz!" << std::endl;
		throw std::out_of_range("Wykroczono poza macierz!");
	}
}

void Macierz::wyswietl(size_t szerokosc_pola)
{
	if (!this->utworzona)
	{
		std::cerr << "Macierzy nie utworzono!" << std::endl;
		return;
	}

	if (macierz == nullptr)
		return;
	std::cout << "  ";
	for (int i = 0; i < rozmiar; i++)
	{
		std::cout.width(szerokosc_pola);
		std::cout << i << " ";
	}
	std::cout << std::endl;

	if (macierz != nullptr)
	{
		for (int i = 0; i < rozmiar; i++)
		{
			std::cout << i << " ";

			for (int j = 0; j < rozmiar; j++)
			{
				std::cout.width(szerokosc_pola);
				std::cout << macierz[i][j] << " ";
			}

			std::cout << std::endl;
		}
	}
}

bool Macierz::wczytaj_dane_z_pliku(std::string nazwa_pliku)
{
	std::ifstream plik;
	plik.open(nazwa_pliku.c_str());
	if (!plik.good())
		return false;

	int poprzedni_rozmiar = this->rozmiar;
	plik >> this->rozmiar;

	if (this->rozmiar <= 0)
		std::cerr << "Blad odczytu danych!" << std::endl;

	if (poprzedni_rozmiar > 0)
	{
		//usuniecie dotychczasowej mapy miast
		if (macierz != nullptr)
		{
			for (int i = 0; i < poprzedni_rozmiar; i++)
			{
				delete[] macierz[i];
			}
			delete[] macierz;
			macierz = nullptr;
		}
		
	}
		//stworzenie nowej mapy miast na podstawie ilosc miast zapisanej w pliku
		macierz = new int *[rozmiar];
		for (int i = 0; i < rozmiar; i++)
		{
			macierz[i] = new int[rozmiar];
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
			plik >> koszt;
			macierz[i][j] = koszt;
			j++;
		}
		else
			break; //zakoncz wczytywanie danych - wystapil jakis blad (np. nie ma wiecej danych w pliku)

	}
	plik.close();
	this->utworzona = true;
	return true;

}

void Macierz::generuja_dane(unsigned int ilosc_miast, unsigned int min_koszt, unsigned int max_koszt)
{
	int poprzedni_rozmiar = this->rozmiar;
	this->rozmiar = ilosc_miast;

	if (this->rozmiar <= 0)
		std::cerr << "Blad odczytu danych!" << std::endl;

	if (poprzedni_rozmiar > 0)
	{
		//usuniecie dotychczasowej mapy miast
		if (macierz != nullptr)
		{
			for (int i = 0; i < poprzedni_rozmiar; i++)
			{
				delete[] macierz[i];
			}
			delete[] macierz;
			macierz = nullptr;
		}

	}
	//stworzenie nowej mapy miast na podstawie ilosc miast zapisanej w pliku
	macierz = new int *[rozmiar];
	for (int i = 0; i < rozmiar; i++)
	{
		macierz[i] = new int[rozmiar];
	}


	srand(time(NULL));
	for (int i = 0; i < ilosc_miast; i++)
	{
		for (int j = 0; j < ilosc_miast; j++)
		{
			if (i == j)
				macierz[i][j] = -1;
			else
				macierz[i][j] = min_koszt + rand() % (max_koszt - min_koszt + 1);
		}
	}

	this->utworzona = true;
	
}
