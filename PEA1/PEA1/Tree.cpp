#include "Tree.h"


Tree::Tree(Node* parent) : parent(parent)
{
	trasa.push_back(0);
}


Tree::~Tree()
{
	
}

void Tree::wyznacz_trase()
{
	Node* tym = parent;
	int najlepszy_koszt = INT_MAX;

	tym = tym->branch_and_bound(trasa, najlepszy_koszt);

	trasa.push_back(0);

	koszt_calkowity = tym->get_koszt();
	trasa = tym->get_trase_minimalna();
}

void Tree::wyswietl_trase()
{
	std::cout << "Trasa: ";

	for (size_t i = 0; i < trasa.size(); i++)
	{
		std::cout << trasa[i];
		if (i < trasa.size() - 1)
			std::cout << " -> ";
	}

	std::cout << std::endl;
}

void Tree::wyswietl_koszt()
{
	std::cout << "Koszt: " << koszt_calkowity << std::endl;
}
