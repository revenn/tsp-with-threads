#include "Zachlanny.h"


Zachlanny::Zachlanny(Macierz mapa) : mapa(mapa)
{
	rozmiar = mapa.get_rozmiar();
	koszt = INT_MAX;
	miasta = new size_t[rozmiar];

	for (size_t i = 0; i < rozmiar; i++)
	{
		miasta[i] = i;
	}

}


Zachlanny::~Zachlanny()
{
	delete miasta;
}


void Zachlanny::perm(size_t k)
{
	if (k == 1)
	{
		int aktualny_koszt = oblicz_aktualny_koszt();
		if (aktualny_koszt < koszt)
		{
			koszt = aktualny_koszt;
			trasa.clear();
			for (size_t i = 0; i < rozmiar; i++)
			{
				trasa.push_back(miasta[i]);
			}
			trasa.push_back(0);
		}

	}	
	else
	{
		for (size_t i = 1; i < k; i++)
		{
			swap(miasta, i, k);
			perm(k - 1);
			swap(miasta, i, k);
		}
	}
}
void Zachlanny::swap(size_t* mapa, size_t i, size_t k)
{
	int tym = mapa[i];
	mapa[i] = mapa[k - 1];
	mapa[k - 1] = tym;
}

int Zachlanny::oblicz_aktualny_koszt()
{
	int koszt = 0;
	for (size_t i = 0; i < rozmiar - 1; i++)
	{
		koszt += *mapa(miasta[i], miasta[i + 1]);
	}
	koszt += *mapa(miasta[rozmiar - 1], miasta[0]);

	return koszt;
}

size_t Zachlanny::znajdz_trase()
{

	perm(rozmiar);
	
	std::cout << "Trasa: ";
	for (size_t i = 0; i < trasa.size(); i++)
	{
		std::cout << trasa[i];
		if (i < trasa.size() - 1)
			std::cout << " -> ";
	}
	std::cout << std::endl;

	std::cout << "Koszt: " << koszt << std::endl;

	return 0;
}
