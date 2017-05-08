#include "Node.h"


Node::Node(bool is_root, Node* parent, Komiwojazer* komiwojazer, size_t numer_miasta) : is_root(is_root), parent(parent), komiwojazer(komiwojazer), trasa_minimalna(komiwojazer->get_rozmiar_miasta())
{
	if (is_root)
	{
		numer_miasta = 0;
		koszt = this->komiwojazer->zredukuj_macierz();
	}	
	else
	{
		this->numer_miasta = numer_miasta;
		if (parent->koszt == -1)
		{
			koszt = -1;
			this->komiwojazer->zredukuj_macierz();
		}
		else
			koszt = parent->koszt + this->komiwojazer->zredukuj_macierz();
	}
		
}

Node::Node(const Node& org)
{
	komiwojazer = new Komiwojazer(*org.komiwojazer);
	is_root = org.is_root;
	parent = org.parent;
	for (size_t i = 0; i < org.children.size(); i++)
	{
		children.push_back(org.children.at(i));
	}
	numer_miasta = org.numer_miasta;
	koszt = org.koszt;
}

Node& Node::operator=(const Node& org)
{
	if (this == &org)
		return *this;

	is_root = org.is_root;
	parent = org.parent;
	for (size_t i = 0; i < org.children.size(); i++)
	{
		children.push_back(org.children.at(i));
	}
	numer_miasta = org.numer_miasta;
	koszt = org.koszt;
	komiwojazer = new Komiwojazer(*org.komiwojazer);
}


Node::~Node()
{

	for (size_t i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
	
	delete komiwojazer;
		
}

//Tworzy nowy wierzcholek dla rodzica i oblicza dla niego dolne ograniczenie
Node* Node::stworz_node(size_t numer_miasta_docelowego)
{
	if (numer_miasta_docelowego == numer_miasta)
		return nullptr;

	Komiwojazer* child = new Komiwojazer(*komiwojazer);

	int wartosc = child->get_wartosc(numer_miasta, numer_miasta_docelowego);

	child->wykresl_wiersz(numer_miasta);
	child->wykresl_kolumne(numer_miasta_docelowego);
	if (parent != nullptr)
		this->wstaw_dla_poprzednich(child, numer_miasta_docelowego, parent);
	child->wstaw(numer_miasta_docelowego, numer_miasta, -1);

	Node* nowy = new Node(false, this, child, numer_miasta_docelowego);
	nowy->zwieksz_koszt(wartosc);

	return nowy;

}

//Wynacza optymalna trase za pomoca metody Branch & Bound.
Node* Node::branch_and_bound(std::vector<size_t> trasa, int& najlepszy)
{
	bool found;
	Node* aktualny;
	static Node* najlepszy_node = nullptr;
	std::vector<size_t> tym(trasa.size());

	//Zapamietanie dotychczasowo wynzaczonej trasy
	for (size_t i = 0; i < trasa.size(); i++)
	{
		tym[i] = trasa[i];
	}

	//Stworzenie dzieci dla konkretnego wierzcholka.
	for (size_t i = 0; i < this->komiwojazer->get_rozmiar_miasta(); i++)
	{
		for (size_t j = 0; j < tym.size(); j++)
		{
			//Spelnienie warunku, ze kazde miasto ma byc odwiedzone jednokrotnie.
			if (i == tym.at(j))
			{
				found = true;
				break;
			}
			else
				found = false;
		}

		if (found)
			continue;
		else
		{
			//Tworzenie nowego wierzcholka wraz z obliczeniem LB dla niego.
			aktualny = this->stworz_node(i);
			tym.push_back(aktualny->get_numer_miasta());
			
			if (aktualny == nullptr)
				continue;

			//Sprawdzenie czy koszt danego wierzcholka <= UB (najlepszego znalezionego do tej pory rozwiazania)
			if (aktualny->get_koszt() <= najlepszy)
			{
				if (tym.size() == this->komiwojazer->get_rozmiar_miasta())
				{
					najlepszy = aktualny->get_koszt();
					najlepszy_node = aktualny;

					for (size_t k = 0; k < trasa_minimalna.size(); k++)
					{
						najlepszy_node->trasa_minimalna[k] = tym[k];
					}
					najlepszy_node->trasa_minimalna.push_back(0);
				}
				//Rekrencyjne wywolanie. 
				aktualny->branch_and_bound(tym, najlepszy);

			}
			else
			{
				tym.pop_back();
				continue;
			}
			tym.pop_back();
			
		}

	}

	//Zwracamy wierzcholek o najlepszm koszcie i optymalnej trasie.
	return najlepszy_node;
}

void Node::zwieksz_koszt(int wartosc)
{
	if (wartosc == -1)
		koszt = -1;
	else
		koszt += wartosc;
}


void Node::wstaw_dla_poprzednich(Komiwojazer* child, size_t numer_miasta_docelowego, Node* parent)
{
	child->wstaw(numer_miasta_docelowego, parent->get_numer_miasta(), -1);
	if (parent->get_parent() != nullptr)
		wstaw_dla_poprzednich(child, numer_miasta_docelowego, parent->get_parent());
}