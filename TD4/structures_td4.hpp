/*
* \file   structures_td4.cpp
* \author Omar Benzekri et Ines Kabbaj
* \date   19 mars 2023
*/

#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using gsl::span;
using namespace std;

struct Acteur; 
using ListeActeurs = Liste<Acteur>;

class Affichage
{
public:
	friend ostream& operator<< (ostream& o, const Item& item);
	friend ostream& operator<< (ostream& o, const Item* itemPointeur);
	friend ostream& operator<<(ostream& o, const Film& film);
	friend ostream& operator<< (ostream& o, const Film* filmPointeur);
	friend ostream& operator<<(ostream& o, const Livre& livre);
	friend ostream& operator<<(ostream& o, const Livre* livrePointeur);
	friend ostream& operator<<(ostream& o, const FilmLivre& filmlivre);
	friend ostream& operator<<(ostream& o, const FilmLivre* filmlivre);
};

class Item : public Affichage {
public:
	Item() = default;
	Item(string titre, int anneeSortie): titre_(titre), anneeSortie_(anneeSortie) {};
	string getTitre() const { return titre_;}
	int getAnneeSortie() const { return anneeSortie_;}
	void setTitre(string titre) { titre_ = titre; }
	void setAnneeSortie(int anneeSortie) { anneeSortie_ = anneeSortie;}
private:
	string titre_ = "default";
	int anneeSortie_ = 0;
};

class Film : virtual public Item
{
public:
	Film() = default;
	Film(string realisateur, int recette, ListeActeurs acteurs): Item(getTitre(), getAnneeSortie()), realisateur_(realisateur), recette_(recette), acteurs_(acteurs){};
	string getRealisateur() const { return realisateur_;}
	int getRecette() const { return recette_;}
	ListeActeurs getActeurs() const { return acteurs_;}
	void setRealisateur(string realisateur) { realisateur_ = realisateur; }
	void setRecette(int recette) { recette_ = recette;}
	void setActeurs(ListeActeurs acteurs) { acteurs_ = acteurs;}
private:
	string realisateur_ = ""; 
	int recette_=0;
	ListeActeurs acteurs_;
};

class Livre : virtual public Item 
{
public:
	Livre() = default;
	Livre(string auteur, int nVentes, int nPages) : Item(getTitre(), getAnneeSortie()), auteur_(auteur), nVentes_(nVentes), nPages_(nPages){};
	string getAuteur() const { return auteur_;}
	int getVentes() const { return nVentes_;}
	int getPages() const { return nPages_;}
	void setAuteur(string auteur) { auteur_ = auteur; }
	void setVentes(int nVentes) { nVentes_ = nVentes;}
	void setPages(int nPages) { nPages_ = nPages;}
private:
	string auteur_ = "";
	int nVentes_ = 0, nPages_ = 0;
};

class FilmLivre : public Film, public Livre
{
	FilmLivre(const Film& film, const Livre& livre);
};

struct Acteur
{
	friend ostream& operator<< (ostream& os, const Acteur& acteur);
	string nom = ""; 
	int anneeNaissance=0; 
	char sexe='\0';
};

class ListeFilms {
public:
	ListeFilms() = default;
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	void detruire(bool possedeLesFilms = false);
	Film*& operator[] (int index) { assert(0 <= index && index < nElements);  return elements[index]; }
	Film* trouver(const function<bool(const Film&)>& critere) {
		for (auto& film : enSpan())
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

template <typename T>
class Liste {
public:
	Liste() = default;
	explicit Liste(int capaciteInitiale) :  // explicit n'est pas matière à ce TD, mais c'est un cas où c'est bon de l'utiliser, pour ne pas qu'il construise implicitement une Liste à partir d'un entier, par exemple "maListe = 4;".
		capacite_(capaciteInitiale),
		elements_(make_unique<shared_ptr<T>[]>(capacite_))
	{
	}
	Liste(const Liste<T>& autre) :
		capacite_(autre.nElements_),
		nElements_(autre.nElements_),
		elements_(make_unique<shared_ptr<T>[]>(nElements_))
	{
		for (int i = 0; i < nElements_; ++i)
			elements_[i] = autre.elements_[i];
	}
	//NOTE: On n'a pas d'operator= de copie, ce n'était pas nécessaire pour répondre à l'énoncé. On aurait facilement pu faire comme dans les notes de cours et définir l'operator= puis l'utiliser dans le constructeur de copie.
	//NOTE: Nos constructeur/operator= de move laissent la liste autre dans un état pas parfaitement valide; il est assez valide pour que la destruction et operator= de move fonctionnent, mais il ne faut pas tenter d'ajouter, de copier ou d'accéder aux éléments de cette liste qui "n'existe plus". Normalement le move sur les classes de la bibliothèque standard C++ laissent les objets dans un "valid but unspecified state" (https://en.cppreference.com/w/cpp/utility/move). Pour que l'état soit vraiment valide, on devrait remettre à zéro la capacité et nombre d'éléments de l'autre liste.
	Liste(Liste<T>&&) = default;  // Pas nécessaire, mais puisque c'est si simple avec unique_ptr...
	Liste<T>& operator= (Liste<T>&&) noexcept = default;  // Utilisé pour l'initialisation dans lireFilm.

	void ajouter(shared_ptr<T> element)
	{
		assert(nElements_ < capacite_);  // Comme dans le TD précédent, on ne demande pas la réallocation pour ListeActeurs...
		elements_[nElements_++] = move(element);
	}

	// Noter que ces accesseurs const permettent de modifier les éléments; on pourrait vouloir des versions const qui retournent des const shared_ptr, et des versions non const qui retournent des shared_ptr.  En C++23 on pourrait utiliser "deducing this".
	shared_ptr<T>& operator[] (int index) const { assert(0 <= index && index < nElements_); return elements_[index]; }
	span<shared_ptr<T>> enSpan() const { return span(elements_.get(), nElements_); }

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};
