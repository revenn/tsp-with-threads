#pragma once
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Tree.h"
#include "Zachlanny.h"
#include "Macierz.h"

class Menu
{
public:
	Menu();
	~Menu();

	static void uruchom();
	static void powrot();

	static void testy();

	static LARGE_INTEGER start_timer();
	static LARGE_INTEGER end_timer();
};

