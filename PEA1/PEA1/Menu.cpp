#include "Menu.h"


Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::uruchom()
{
	Komiwojazer* komiwojazer = nullptr;
	Node *parent = nullptr;
	Tree *tree = nullptr;

	std::cout << "Problem komiwojazer - Branch & Bound" << std::endl << std::endl;
	int wybor;

	do
	{

		std::cout << "MENU: " << std::endl;
		std::cout << "1. Wczytaj z pliku." << std::endl;
		std::cout << "2. Generuj dane." << std::endl;
		std::cout << "3. Wyswietl. " << std::endl;
		std::cout << "4. Wykonaj algorytm." << std::endl;
		std::cout << "5. Koniec." << std::endl;

		std::cin >> wybor;

	} while (wybor < 1 || wybor > 5);

	std::string plik;
	do
	{

		if (wybor == 1)
		{
			if (tree != nullptr)
				delete tree;
			if (parent != nullptr)
				delete parent;

			komiwojazer = new Komiwojazer(1);
			do
			{
				std::cout << "Podaj nazwe pliku: " << std::endl;
				std::cin >> plik;
			} while (!komiwojazer->wczytaj_dane_z_pliku(plik));


			powrot();
		}
		if (wybor == 2)
		{
			if (tree != nullptr)
				delete tree;
			if (parent != nullptr)
				delete parent;

			plik = "";

			komiwojazer = new Komiwojazer(1);
			int ilosc, min, max;
			std::cout << "Podaj ilosc miast: " << std::endl;
			std::cin >> ilosc;
			std::cout << "Podaj minimalny koszt: " << std::endl;
			std::cin >> min;
			std::cout << "Podaj maksymalny koszt: " << std::endl;
			std::cin >> max;
			komiwojazer->generuja_dane(ilosc, min, max);

			powrot();
		}

		if (wybor == 3)
		{
			if (komiwojazer != nullptr)
				komiwojazer->wyswietl(4);
			powrot();
		}

		if (wybor == 4)
		{
			if (komiwojazer != nullptr)
			{
				parent = new Node(true, nullptr, komiwojazer, 0);
				tree = new Tree(parent);

				LARGE_INTEGER performanceCountStart, performanceCountEnd;
				performanceCountStart = start_timer(); //zapamiętujemy czas początkowy

				tree->wyznacz_trase();	//mierzenie czasu wyznaczania trasy

				performanceCountEnd = end_timer(); //zapamiętujemy koniec czasu
				double tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;

				tree->wyswietl_trase();
				tree->wyswietl_koszt();
				std::cout << "Czas wykonania: " << tm / 1000.0 << " ms" <<std::endl;
			}
			powrot();
		}

		if (wybor == 5)
		{
			if (tree != nullptr)
				delete tree;
			if (parent != nullptr)
				delete parent;
			return;
		}
		

		std::cout << "MENU: " << std::endl;
		std::cout << "1. Wczytaj z pliku." << std::endl;
		std::cout << "2. Generuj dane." << std::endl;
		std::cout << "3. Wyswietl. " << std::endl;
		std::cout << "4. Wykonaj algorytm." << std::endl;
		std::cout << "5. Koniec." << std::endl;

		std::cin >> wybor;

	} while (true);

}

void Menu::powrot()
{
	std::cout << "Nacisnij dowolny klawisz by powrocic do menu." << std::endl;
	_getch();
}

LARGE_INTEGER Menu::start_timer()
{
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return
		start;
}

LARGE_INTEGER Menu::end_timer()
{
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return
		stop;
}

void Menu::testy()
{
	double sredni_czas = 0.;
	const size_t iteracje = 100;
	Komiwojazer* komiwojazer = nullptr;
	Node *parent = nullptr;
	Tree *tree = nullptr;

	for (size_t i = 0; i < iteracje; i++)
	{
		system("cls");
		std::cout << i << '%';

		komiwojazer = new Komiwojazer(1);
		komiwojazer->generuja_dane(5, 10, 100);

		parent = new Node(true, nullptr, komiwojazer, 0);
		tree = new Tree(parent);

		LARGE_INTEGER performanceCountStart, performanceCountEnd;
		performanceCountStart = start_timer(); //zapamiętujemy czas początkowy

		tree->wyznacz_trase();	//mierzenie czasu wyznaczania trasy

		performanceCountEnd = end_timer(); //zapamiętujemy koniec czasu
		double tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;

		sredni_czas += tm;

		if (tree != nullptr)
			delete tree;
		if (parent != nullptr)
			delete parent;
	}

	sredni_czas /= iteracje;

	system("cls");
	//sredni czas szukania optymalnej drogi dla jednej macierzy
	std::cout << "Sredni czas przy " << iteracje << " iteracjach wynosi: " << (sredni_czas / 1000.0 ) << " ms" << std::endl;

	_getch();
}