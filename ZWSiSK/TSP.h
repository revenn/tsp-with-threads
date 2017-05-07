#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "windows.h"

#include "Matrix.h"

class TSP
{
public:
	TSP()
	{
		this->macierz = NULL;
		this->size = 0;
		this->poczatek = 0;
	}

	virtual ~TSP()
	{
		delete this->macierz;
	}

	bool wczytaj(std::string path);
	void losuj(unsigned rozmiar);
	void wyswietl(Matrix* s);
	void menu();

	/*void przeszukiwanie(Matrix*, std::vector<int>*, std::vector<int>*, std::vector<int>*, int*);
	int lowBound(Matrix*, std::vector<int>*);
	int koszt(std::vector<int> *, Matrix *);*/

	long long testuj(int);
	void test();

	Matrix *macierz;
	int size;
	int poczatek;
};



