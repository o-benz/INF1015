#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
#include <ostream>
#include <iostream>
#include <functional>
using namespace std;
using gsl::span;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }

	Film& operator[](int i) {
		return *elements[i]; 
	}

	Film* chercherFilm(function<bool(Film*)> critere);

private:
	void changeDimension(int nouvelleCapacite);
	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

// Convertir ListeActeurs en class Liste template pour pouvoir contenir autre chose que des Acteur,
// puis déclarer un équivalent (ligne à placer après la fin définition de la classe Liste):
// using ListeActeurs = Liste<Acteur>;
// (Le programme devrait encore fonctionner avec aucune modification autre que dans ListeActeur devenu
// Liste) 
template <typename T>
class Liste {
public:
	int capacite, nElements;
	unique_ptr<shared_ptr<T>[]> elements;

	Liste() {
		capacite = 0;
		nElements = 0;
		elements = nullptr;
	}

	Liste(int nouvelleCapacite, const int nouveauNElements) {
		capacite = nouvelleCapacite;
		nElements = nouveauNElements;
		elements = make_unique<shared_ptr<T>[]>(nElements);
	}

	Liste(const Liste& autre) {
		capacite = autre.capacite;
		nElements = autre.nElements;
		elements = make_unique<shared_ptr<T>[]>(nElements);
		if (!autre.elements)
		{
			return;
		}
		for (int i = 0; i < nElements; ++i)
		{
			elements[i] = autre.elements[i];
		}
	}

	Liste& operator=(const Liste& autre) {
		capacite = autre.capacite;
		nElements = autre.nElements;
		if (!autre.elements)
		{
			return *this;
		}
		for (int i = 0; i < nElements; i++)
		{
			elements[i] = autre.elements[i];
		}
		return *this;
	}
	//Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	shared_ptr<T>& operator[](int i) {
		return elements[i];
	}
};

using ListeActeurs = Liste<Acteur>;

// struct ListeActeurs {
// 	int capacite, nElements;
// 	unique_ptr<shared_ptr<Acteur>[]> elements;

// 	ListeActeurs() {
// 		capacite = 0;
// 		nElements = 0;
// 		elements = nullptr;
// 	}

// 	ListeActeurs(int nouvelleCapacite, const int nouveauNElements) {
// 		capacite = nouvelleCapacite;
// 		nElements = nouveauNElements;
// 		elements = make_unique<shared_ptr<Acteur>[]>(nElements);
// 	}

// 	ListeActeurs(const ListeActeurs& autre) {
// 		capacite = autre.capacite;
// 		nElements = autre.nElements;
// 		elements = make_unique<shared_ptr<Acteur>[]>(nElements);
// 		if (!autre.elements) 
// 		{
// 			return;
// 		}
// 		for (int i = 0; i < nElements; ++i) 
// 		{
// 			elements[i] = autre.elements[i];
// 		}
// 	}

// 	ListeActeurs& operator=(const ListeActeurs& autre) {
// 		capacite = autre.capacite;
// 		nElements = autre.nElements;
// 		if (!autre.elements) 
// 		{
// 			return *this;
// 		}
// 		for (int i = 0; i < nElements; i++) 
// 		{
// 			elements[i] = autre.elements[i];
// 		}
// 		return *this;
// 	}
// 	//Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	
// };

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;

	friend ostream& operator<<(ostream& os, const Film& film) {
		os << "Titre: " << film.titre << endl;
		os << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
		os << "  Recette: " << film.recette << "M$" << endl;
		return os;
	}

	Film& operator=(Film& autre) {
		titre = autre.titre;
		realisateur = autre.realisateur;
		anneeSortie = autre.anneeSortie;
		recette = autre.recette;
		acteurs = autre.acteurs;
		return *this;
	}

	Film& operator=(string autre) {
		titre = autre;
		return *this;
	}
};

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;

	friend ostream& operator<<(ostream& os, const Acteur& acteur) {
		cout << "Nom: " << acteur.nom << endl; 
		cout << "  Année de naissance: " << acteur.anneeNaissance << endl;
		cout << "  Sexe: " << acteur.sexe << endl;
		return os;
	}

	Acteur& operator=(Acteur& autre) {
		nom = autre.nom;
		anneeNaissance = autre.anneeNaissance;
		sexe = autre.sexe;
		return *this;
	}

	Acteur& operator=(string autre) {
		nom = autre;
		return *this;}
	//ListeFilms joueDans;
};
