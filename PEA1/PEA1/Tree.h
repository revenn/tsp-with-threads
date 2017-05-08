#pragma once
#include <vector>
#include "Node.h"

class Tree
{
private:
	Node* parent;
	std::vector<size_t> trasa;
	int koszt_calkowity;
	
public:
	Tree(Node* parent);
	~Tree();

	void wyznacz_trase();
	void wyswietl_trase();
	void wyswietl_koszt();
};

