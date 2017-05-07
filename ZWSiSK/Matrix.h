#pragma once
#include <stdexcept>

class Matrix
{
public:
	Matrix(unsigned rozmiar)
	{
		this->size = rozmiar;
		this->tab = new int*[rozmiar];

		for (unsigned i = 0; i<rozmiar; ++i)
			this->tab[i] = new int[rozmiar];

		for (unsigned i = 0; i<rozmiar; ++i)
			for (unsigned j = 0; j<rozmiar; ++j)
				this->tab[i][j] = -1;
	}
	~Matrix()
	{
		for (unsigned i = 0; i<size; ++i)
		{
			delete[] this->tab[i];
			this->tab[i] = NULL;
		}
		delete[] this->tab;
		this->tab = NULL;
	}
	int& operator() (unsigned row, unsigned col)
	{
		if (row >= size || col >= size)
			throw std::out_of_range("Zly indeks");
		return this->tab[row][col];
	}

	unsigned size;

private:
	int **tab;
};