#include "Menu.h"
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "Cities.h"
#include <string>
#include <Queue>
#include <chrono>
#include <thread>
#include <stack>
#include "Tour.h"
#include <condition_variable>
#include <atomic>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/stack.hpp>

using namespace std;

int NUM_THREADS;
atomic<int> active_threads(0); // 0 to n-1 for array Indexing
//atomic<Tour> csDFSbest(UINT_MAX);
Tour PSbest(UINT_MAX), iDFSbest(UINT_MAX), prDFSbest(UINT_MAX), siDFSbest(UINT_MAX); // globals for best tour so far - initialized high
mutex g_lock;
condition_variable cv;


void iDFS(Tour t) {
	stack<Tour> frontier; //Stack for DFS
	Tour curTour;
	frontier.push(t); //start with just the hometown

	while (!frontier.empty()) {
		curTour = frontier.top();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(PSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				PSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, PSbest)) {
					frontier.push(curTour);
					curTour.remove_last();
				}
			}
		}
	}
	--active_threads;
	cv.notify_all();
}

void PS(Tour t) {
	queue<Tour> frontier;
	Tour curTour;
	frontier.push(t);
	vector<thread> threadPool;

	while (!frontier.empty()) {
		curTour = frontier.front();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(PSbest)) {
				std::lock_guard<std::mutex> lock(g_lock);
				PSbest = curTour;
			}
		}
		//If we don't have enough work for threads, expand tree
		else if (frontier.size() < (NUM_THREADS - active_threads)) {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, PSbest)) {
					frontier.push(curTour);
					curTour.remove_last();
				}
			}
		}
		//if we have available worker threads, make them DFS
		else if (active_threads < NUM_THREADS) {
			++active_threads;
			threadPool.push_back(thread(iDFS, curTour));
		}
		//if not, wait until we do
		else {
			unique_lock<mutex> lock(g_lock);
			cv.wait(lock);
			++active_threads;
			threadPool.push_back(thread(iDFS, curTour));
		}
	}
	for (thread& t : threadPool) {
		t.join();
	}
}

/* Do a depth-first search to find the best tour
* starting with the partially complete tour t
*/
void prDFShelper(Tour& t) {
	if (!t.better_than(prDFSbest)) return; // if we've already seen something better

	// If we've added all the cities and we're still here, this must be
	// the best tour
	if (t.get_num_cities() == t.miasta.rozmiar) {
		std::lock_guard<std::mutex> lock(g_lock);
		prDFSbest = t;
	}
	// Otherwise, try to add the next city and recursively call DFS
	// if successful
	else {
		for (unsigned int i = 0; i < t.miasta.rozmiar; i++) {
			if (t.add_city(i, prDFSbest)) {
				prDFShelper(t);
				t.remove_last();
			}
		}
	}
}


/*Calls prDFShelper and notifies main thread
* on completion
*/
void prDFSwrapper(Tour& t) {
	prDFShelper(t);
	--active_threads;
	cv.notify_all();
}


void prDFS(Tour& t) {
	queue<Tour> frontier;
	Tour curTour;
	frontier.push(t);
	vector<thread> threadPool;

	while (!frontier.empty()) {
		curTour = frontier.front();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(prDFSbest)) {
				prDFSbest = curTour;
			}
		}
		//If we don't have enough work for threads, expand tree
		else if (frontier.size() < (NUM_THREADS - active_threads)) {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, prDFSbest)) {
					Tour copyForQueue(curTour);
					frontier.push(copyForQueue);
					curTour.remove_last();
				}
			}
		}
		//if we have available worker threads, make them DFS
		else if (active_threads < NUM_THREADS) {
			++active_threads;
			threadPool.push_back(thread(prDFSwrapper, curTour));
		}
		//if not, wait until we do
		else {
			unique_lock<mutex> lock(g_lock);
			cv.wait(lock);
			++active_threads;
			threadPool.push_back(thread(prDFSwrapper, curTour));
		}
	}
	for (thread& t : threadPool) {
		t.join();
	}
}

void siDFS(Tour t) {
	stack<Tour> frontier; //Stack for DFS
	Tour curTour;
	frontier.push(t); //start with just the hometown

	while (!frontier.empty()) {
		curTour = frontier.top();
		frontier.pop();

		//If tour is complete, check if it's the best
		if (curTour.get_num_cities() == t.miasta.rozmiar) {
			if (curTour.better_than(iDFSbest)) {
				iDFSbest = curTour;
			}
		}
		//If tour is incomplete, check which cities can be added
		else {
			for (unsigned int i = t.miasta.rozmiar - 1; i > 0; --i) {
				//If city isn't already on tour and doesn't exceed best mileage,
				//add it to queue for exploration later.
				if (curTour.add_city(i, iDFSbest)) {
					frontier.push(curTour);
					curTour.remove_last();
				}
			}
		}
	}

}


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
	Macierz *mapa = nullptr;
	Cities *mapa2 = nullptr;
	Tour *t = nullptr;

	std::cout << "Problem komiwojazer - Branch & Bound" << std::endl << std::endl;
	int wybor;

	do
	{

		std::cout << "MENU: " << std::endl;
		std::cout << "1. Wczytaj z pliku." << std::endl;
		std::cout << "2. Generuj dane." << std::endl;
		std::cout << "3. Wyswietl. " << std::endl;
		std::cout << "4. Algorytm B&B." << std::endl;
		std::cout << "5. Przeglad zaupelny." << std::endl;
		std::cout << "6. Alorytm B&B iteracyjnie na watkach." << std::endl;
		std::cout << "7. Algorytm B&B rekurencyjnie na watkach." << std::endl;
		std::cout << "8. Przeglad zupelny na watkach." << std::endl;
		std::cout << "9. Koniec." << std::endl;

		std::cin >> wybor;

	} while (wybor < 1 || wybor > 8);

	std::string plik;
	do
	{

		if (wybor == 1)
		{
			if (tree != nullptr)
				delete tree;
			if (parent != nullptr)
				delete parent;
			if (mapa != nullptr)
				delete mapa;
			if (mapa2 != nullptr)
				delete mapa2;

			komiwojazer = new Komiwojazer(1);
			do
			{
				std::cout << "Podaj nazwe pliku: " << std::endl;
				std::cin >> plik;
			} while (!komiwojazer->wczytaj_dane_z_pliku(plik));
			mapa = new Macierz(komiwojazer->get_mapa());
			mapa2 = new Cities();
			mapa2->wczytaj_dane_z_pliku(plik);
			t = new Tour(*mapa2);

			powrot();
		}
		if (wybor == 2)
		{
			if (tree != nullptr)
				delete tree;
			if (parent != nullptr)
				delete parent;
			if (mapa != nullptr)
				delete mapa;
			if (mapa2 != nullptr)
				delete mapa2;

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
			mapa = new Macierz(komiwojazer->get_mapa());
			mapa2 = new Cities();
			mapa2->generuja_dane(ilosc, min, max);
			t = new Tour(*mapa2);

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
				Komiwojazer* komiwojazer_kopia = new Komiwojazer(*komiwojazer);
				parent = new Node(true, nullptr, komiwojazer_kopia, 0);
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
			if (komiwojazer != nullptr)
			{
				Zachlanny zach(*mapa);

				LARGE_INTEGER performanceCountStart, performanceCountEnd;
				performanceCountStart = start_timer(); //zapamiętujemy czas początkowy

				zach.znajdz_trase();	//mierzenie czasu wyznaczania trasy

				performanceCountEnd = end_timer(); //zapamiętujemy koniec czasu
				double tm = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;

				std::cout << "Czas wykonania: " << tm / 1000.0 << " ms" << std::endl;
			}
			powrot();
		}

		if (wybor == 6)
		{
			std::cout << "Podaj ilosc watkosc: ";
			std::cin >> NUM_THREADS;
			std::chrono::high_resolution_clock::time_point PSStart = std::chrono::high_resolution_clock::now();
			PS(*t);
			std::chrono::high_resolution_clock::time_point PSEnd = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> PSTime = std::chrono::duration_cast<std::chrono::duration<double>>(PSEnd - PSStart);

			std::cout << PSbest;
			cout << "Ilosc watkow: " << NUM_THREADS << '.' << endl;
			cout << "Iterecyjny B&B z watkami: " << PSTime.count() / 1000.0 << " ms." << endl;
			t->clear();
		}

		if (wybor == 7)
		{
			std::cout << "Podaj ilosc watkosc: ";
			std::cin >> NUM_THREADS;
			std::chrono::high_resolution_clock::time_point prDFSStart = std::chrono::high_resolution_clock::now();
			prDFS(*t);
			std::chrono::high_resolution_clock::time_point prDFSEnd = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> prDFSTime = std::chrono::duration_cast<std::chrono::duration<double>>(prDFSEnd - prDFSStart);

			std::cout << prDFSbest;
			cout << "Ilosc watkow: " << NUM_THREADS << '.' << endl;
			cout << "Rekursywny B&B z watkami: " << prDFSTime.count() / 1000.0 << " ms." << endl;
			t->clear();
		}

		if (wybor == 8)
		{
			std::cout << "Podaj ilosc watkosc: ";
			std::cin >> NUM_THREADS;
			std::chrono::high_resolution_clock::time_point siDFSStart = std::chrono::high_resolution_clock::now();
			siDFS(*t);
			std::chrono::high_resolution_clock::time_point siDFSEnd = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> siDFSTime = std::chrono::duration_cast<std::chrono::duration<double>>(siDFSEnd - siDFSStart);

			std::cout << PSbest;
			cout << "Ilosc watkow: " << NUM_THREADS << '.' << endl;
			cout << "Przeglad zupelny na watkach: " << siDFSTime.count() / 1000.0 << " ms." << endl;
			t->clear();
		}

		if (wybor == 9)
		{
			if (tree != nullptr)
				delete tree;
			if (parent != nullptr)
				delete parent;
			if (mapa != nullptr)
				delete mapa;
			return;
		}
		

		std::cout << "MENU: " << std::endl;
		std::cout << "1. Wczytaj z pliku." << std::endl;
		std::cout << "2. Generuj dane." << std::endl;
		std::cout << "3. Wyswietl. " << std::endl;
		std::cout << "4. Algorytm B&B." << std::endl;
		std::cout << "5. Przeglad zaupelny." << std::endl;
		std::cout << "6. Alorytm B&B iteracyjnie na watkach." << std::endl;
		std::cout << "7. Algorytm B&B rekurencyjnie na watkach." << std::endl;
		std::cout << "8. Przeglad zupelny na watkach." << std::endl;
		std::cout << "9. Koniec." << std::endl;

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