#pragma once
#include <vector>
#include "Komiwojazer.h"

class Node
{
private:
	bool is_root;
	Node* parent;
	std::vector<Node*> children;
	size_t numer_miasta;
	int koszt;
	std::vector<size_t> trasa_minimalna;

public:
	Komiwojazer* komiwojazer;

	Node(bool is_root, Node* parent, Komiwojazer* komiwojazer, size_t numer_miasta);
	Node(const Node& org);
	~Node();

	Node& operator=(const Node& org);

	size_t get_numer_miasta() { return numer_miasta; }
	int get_koszt() { return koszt; }
	Node* get_parent() { return parent; }
	std::vector<size_t> get_trase_minimalna() { return trasa_minimalna; }

	void zwieksz_koszt(int wartosc);

	Node* stworz_node(size_t numer_miasta_docelowego);
	Node* branch_and_bound(std::vector<size_t> trasa, int&);

	void wstaw_dla_poprzednich(Komiwojazer* child, size_t numer_miasta_docelowego, Node* parent);
};

