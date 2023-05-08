#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS 

#include "structures.hpp"      
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" 
#include "debogage_memoire.hpp"        

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*) &valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*) &valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*) &texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

Film** ListeFilms::accesElemnts() const
{
	return elements_;
};


span<Film*> ListeFilms::spanListeFilms()
{
	return span(elements_, nElements_);
}

span<Acteur*> spanListeActeurs(const ListeActeurs& listeActeurs)
{
	return span(listeActeurs.elements, listeActeurs.nElements);
}

void ListeFilms::ajouterFilm(Film* film)
{
	if(capacite_ < nElements_ + 1)
	{
		if(capacite_ == 0)
			capacite_ = 1;
		Film** nouvelleListeFilms = new Film * [capacite_ * 2];
		elements_ = nouvelleListeFilms;
		capacite_ *= 2;
	}
	elements_[nElements_++] = film;
}


void ListeFilms::effacerFilm(const Film* film)
{
	for(Film*& filmListe : spanListeFilms())
	{
		if(filmListe->titre == film->titre)
		{
			if(nElements_ > 1)
			{
				filmListe = elements_[nElements_ - 1];
			}
			nElements_--;
		}
	}
}

Acteur* ListeFilms::trouverActeur(string nomActeur)
{
	for(Film* film : spanListeFilms())
	{
		for(Acteur* acteur : spanListeActeurs((*film).acteurs))
		{
			if((*acteur).nom == nomActeur) return acteur;
		}
	}
	return nullptr;
}


{
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = lireUint16(fichier);
	acteur.sexe = lireUint8(fichier);
	if(trouverActeur(acteur.nom) != nullptr)
	{
		return trouverActeur(acteur.nom);
	}
	else
	{
		cout << "Création de " << acteur.nom << endl; //Pour des raisons de débogage
		return new Acteur(acteur);
	}
}

Film* ListeFilms::lireFilm(istream& fichier)
{
	Film film = {};
	film.titre = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = lireUint16(fichier);
	film.recette = lireUint16(fichier);
	film.acteurs.nElements = lireUint8(fichier);
	film.acteurs.capacite = film.acteurs.nElements;
	film.acteurs.elements = new Acteur * [film.acteurs.nElements];
	Film* film_ptr = new Film(film);
	cout << "Création de " << film_ptr->titre << endl; //Pour des raisons de débogage
	for(int i = 0; i < film.acteurs.nElements; i++)
	{
		film_ptr->acteurs.elements[i] = lireActeur(fichier); //Placer l'acteur au bon endroit dans les acteurs du film.
		film_ptr->acteurs.elements[i]->joueDans.ajouterFilm(film_ptr);
	}
	return film_ptr; //Retourner le pointeur vers le nouveau film.
}

ListeFilms::ListeFilms(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	//fichier.exceptions(ios::failbit);

	int nElements = lireUint16(fichier);
	//Créer une liste de films vide.
	elements_ = new Film * [nElements];
	capacite_ = nElements;

	for(int i = 0; i < nElements; i++)
	{
		ajouterFilm(lireFilm(fichier)); //Ajouter le film à la liste.
	}
}


void ListeFilms::detruireFilm(Film* film)
{
	for(Acteur* acteur : spanListeActeurs(film->acteurs))
	{
		acteur->joueDans.effacerFilm(film);
		if(acteur->joueDans.nElements_ == 0)
		{
			cout << "Déstruction de " << acteur->nom << endl; //Pour des raisons de débogage
			acteur->joueDans.~ListeFilms();
			delete[] acteur->joueDans.elements_;
			acteur->joueDans.elements_ = nullptr;
			delete acteur;
			acteur = nullptr;
			film->acteurs.nElements--;
		}
	}
	cout << "Déstruction de " << film->titre << endl; //Pour des raisons de débogage
	effacerFilm(film);
	delete[] film->acteurs.elements;
	film->acteurs.elements = nullptr;
	delete film;
	film = nullptr;
}


ListeFilms::~ListeFilms()
{
	for(Film* film : spanListeFilms())
	{
		detruireFilm(film);
	}
	delete[] elements_;
	elements_ = nullptr;
}


void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}


void afficherFilm(Film film)
{
	cout << "Titre: " << film.titre << endl;
	cout << "Realisateur: " << film.realisateur << endl;
	cout << "Annee de sortie: " << film.anneeSortie << endl;
	cout << "Recette en M$: " << film.recette << endl;
	cout << "Acteurs: " << endl;
	for(Acteur* acteur : spanListeActeurs(film.acteurs))
	{
		afficherActeur(*acteur);
	}
}


void ListeFilms::afficherListeFilms()
{
	static const string ligneDeSeparation = "\n\033[40m═════════════════════════════════\033[0m\n";
	for(Film* film : spanListeFilms())
	{
		//Affiche le film.
		afficherFilm(*film);
		cout << ligneDeSeparation;
	}
}

void ListeFilms::afficherFilmographieActeur(const string& nomActeur)
{
	Acteur* acteur = trouverActeur(nomActeur);
	if(acteur == nullptr)
	{
		cout << "Aucun acteur de ce nom" << endl;
	}
	else
	{
		acteur->joueDans.afficherListeFilms();
	}
}


int main()
{
	bibliotheque_cours::activerCouleursAnsi();
	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	ListeFilms listeFilms("films.bin");
	cout << ligneDeSeparation << "Le premier film de la liste est: ";
	afficherFilm(*listeFilms.accesElemnts()[0]);
	cout << ligneDeSeparation << "Les films sont:" << endl;
	listeFilms.afficherListeFilms()
	listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;
	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl << endl;
	listeFilms.afficherFilmographieActeur("Benedict Cumberbatch");
	listeFilms.detruireFilm(listeFilms.accesElemnts()[0]);
	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	listeFilms.afficherListeFilms();
	cout << "Ces lignes sont pour des raisons de coverture de code  et débogage vous pouvez les ignorer" <<endl;
	listeFilms.afficherFilmographieActeur("Younes Benabbou");
}
