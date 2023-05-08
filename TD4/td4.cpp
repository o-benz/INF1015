/*
* \file   td4.cpp
* \author Omar Benzekri et Ines Kabbaj
* \date   19 mars 2023
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures_td4.hpp"      // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>

#include "cppitertools/range.hpp"
#include "gsl/span"
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).
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
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

// 3. Affichage de la bibliothèque

ostream& operator<< (ostream& o, const Item& item) 
{
	o << "Titre: " << item.getTitre() << endl;
	o << "Annee :" << item.getAnneeSortie() << endl;
	return o;
}

ostream& operator<< (ostream& o, const Item* itemPointeur) 
{
	o << "Titre: " << itemPointeur->getTitre() << endl;
	o << "Annee :" << itemPointeur->getAnneeSortie() << endl;
	return o;
}

ostream& operator<< (ostream& o, const Film& film)
{
	o << static_cast<const Item&>(film);
	o << "Realisateur: " << film.getRealisateur() << endl;
	o << "Recette: " << film.getRecette() << "M$" << endl;

	o << "Acteurs:" << endl;
	for (shared_ptr<Acteur> acteur : film.getActeurs().spanListeActeurs()) 
	{
		o << *acteur;
	}
	return o;
}
	


ostream& operator<< (ostream& o, const Film* filmPointeur) 
{
	
	o << static_cast<const Item*>(filmPointeur);
	o << "Réalisateur: " << filmPointeur->getRealisateur() << endl;
	o << "Recette: " << filmPointeur->getRecette() << "M$" << endl;
	o << "Acteurs: " << endl;

	for (shared_ptr<Acteur> acteur : filmPointeur->getActeurs().spanListeActeurs()) 
	{
    		o << acteur;
	}
	return o;
}

ostream& operator<<(ostream& o, const Livre& livre)
{
	o << static_cast<const Item&>(livre);
	o << "Auteur: " << livre.getAuteur() << endl;
	o << "Copies Vendues: " << livre.getVentes() << " Millions" << endl;
	o << livre.getPages() << " Pages" << endl;
	return o;
}

ostream& operator<<(ostream& o, const Livre* livrePointeur)
{
	o << static_cast<const Item*>(livrePointeur);
	o << "Auteur: " << livrePointeur->getAuteur() << endl;
	o << "Copies Vendues: " << livrePointeur->getVentes() << " Millions" << endl;
	o << livrePointeur->getPages() << " Pages" << endl;
	return o;
}

ostream& operator<< (ostream& os, const Acteur& acteur)
{
	return os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

FilmLivre::FilmLivre(const Film& film, const Livre& livre) : Item(film), Film(film), Livre(livre){}

ostream& operator<<(ostream& o, const FilmLivre& filmlivre)
{
	o << "Auteur: " << filmlivre.getAuteur() << endl;
	o << "Réalisateur: " << filmlivre.getRealisateur() << endl;
	o << "Recette: " << filmlivre.getRecette() << "M$" << endl;
	o << "Copies Vendues: " << filmlivre.getVentes() << " Millions" << endl;
	o << filmlivre.getPages() << " Pages" << endl;

	o << "Acteurs: " << endl;
	for (shared_ptr<Acteur> acteur : filmlivre.getActeurs().spanListeActeurs()) 
	{
    		o << *acteur;
	}
	return o;
}

ostream& operator<<(ostream& o, const FilmLivre* filmlivre)
{
	o << "Auteur: " << filmlivre->getAuteur() << endl;
	o << "Réalisateur: " << filmlivre->getRealisateur() << endl;
	o << "Recette: " << filmlivre->getRecette() << "M$" << endl;
	o << "Copies Vendues: " << filmlivre->getVentes() << " Millions" << endl;
	o << filmlivre->getPages() << " Pages" << endl;

	o << "Acteurs: " << endl;
	for (shared_ptr<Acteur> acteur : filmlivre->getActeurs().spanListeActeurs()) 
	{
		o << acteur;
	}
	return o;
}

void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film*[nouvelleCapacite];
	
	if (elements != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements sera zéro si elements est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements = min(nouvelleCapacite, nElements);
		for (int i : range(nElements))
			nouvelleListe[i] = elements[i];
		delete[] elements;
	}
	
	elements = nouvelleListe;
	capacite = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film* film)
{
	if (nElements == capacite)
		changeDimension(max(1, capacite * 2));
	elements[nElements++] = film;
}

void lireLivre(string fichier, vector<unique_ptr<Item>>& bibliotheque)
{
	ifstream Livres(fichier);
	string element;
	while (getline(Livres, element, '\t'))
	{
		cout << "Creation du livre: " << element << endl;
		Livre livre;

		livre.setTitre(element);

		getline(Livres, element, '\t');
		livre.setAnneeSortie(stoi(element));

		getline(Livres, element, '\t');
		livre.setAuteur(element);

		getline(Livres, element, '\t');
		livre.setVentes(stoi(element));

		getline(Livres, element, '\n');
		livre.setPages(stoi(element));

		cout << "Ajout de:" << livre.getTitre() << endl;
		bibliotheque.push_back(make_unique<Livre>(livre));
	}
}


span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film* film)
{
	for (Film*& element : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film) {
			if (nElements > 1)
				element = elements[nElements - 1];
			nElements--;
			return;
		}
	}
}

shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& acteur : film->getActeurs().enSpan()) {
			if (acteur->nom == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}

shared_ptr<Acteur> lireActeur(istream& fichier, const ListeFilms& listeFilms)
{
	Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = lireUint16 (fichier);
	acteur.sexe           = lireUint8  (fichier);

	shared_ptr<Acteur> acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.nom << endl;
		return make_shared<Acteur>(move(acteur));  // Le move n'est pas nécessaire mais permet de transférer le texte du nom sans le copier.
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->setTitre(lireString(fichier));
	film->setRealisateur(lireString(fichier));
	film->setAnneeSortie(lireUint16 (fichier));
	film->setRecette(lireUint16 (fichier));
	auto nActeurs = lireUint8 (fichier);
	film->setActeurs(ListeActeurs(nActeurs));  // On n'a pas fait de méthode pour changer la taille d'allocation, seulement un constructeur qui prend la capacité.  Pour que cette affectation fonctionne, il faut s'assurer qu'on a un operator= de move pour ListeActeurs.
	cout << "Création Film " << film->getTitre() << endl;

	for ([[maybe_unused]] auto i : range(nActeurs)) 
		film->getActeurs().ajouter(lireActeur(fichier, listeFilms));

	return film;
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int nElements = lireUint16(fichier);

	ListeFilms listeFilms;
	for ([[maybe_unused]] int i : range(nElements)) { //NOTE: On ne peut pas faire un span simple avec ListeFilms::enSpan car la liste est vide et on ajoute des éléments à mesure.
		listeFilms.ajouterFilm(lireFilm(fichier, listeFilms));
	}
	
	return listeFilms;
}

void ListeFilms::detruire(bool possedeLesFilms)
{
	if (possedeLesFilms)
		for (Film* film : enSpan())
			delete film;
	delete[] elements;
}


int main()
{
	#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	#endif
	bibliotheque_cours::activerCouleursAnsi();  
	/*   
	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	ListeFilms listeFilms = creerListe("films.bin");
	
	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	// Le premier film de la liste.  Devrait être Alien.
	cout << *listeFilms[0];

	// Tests chapitre 7:
	ostringstream tamponStringStream;
	tamponStringStream << *listeFilms[0];
	string filmEnString = tamponStringStream.str();
	assert(filmEnString == 
		"Titre: Alien\n"
		"  Réalisateur: Ridley Scott  Année :1979\n"
		"  Recette: 203M$\n"
		"Acteurs:\n"
		"  Tom Skerritt, 1933 M\n"
		"  Sigourney Weaver, 1949 F\n"
		"  John Hurt, 1940 M\n"
	);

	cout << ligneDeSeparation << "Les films sont:" << endl;
	// Affiche la liste des films.  Il devrait y en avoir 7.
	cout << listeFilms;

	listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;

	// Tests chapitres 7-8:
	// Les opérations suivantes fonctionnent.
	Film skylien = *listeFilms[0];
	skylien.titre = "Skylien";
	skylien.acteurs[0] = listeFilms[1]->acteurs[0];
	skylien.acteurs[0]->nom = "Daniel Wroughton Craig";
	cout << ligneDeSeparation
		<< "Les films copiés/modifiés, sont:\n"
		<< skylien << *listeFilms[0] << *listeFilms[1] << ligneDeSeparation;
	assert(skylien.acteurs[0]->nom == listeFilms[1]->acteurs[0]->nom);
	assert(skylien.acteurs[0]->nom != listeFilms[0]->acteurs[0]->nom);

	// Tests chapitre 10:
	auto film955 = listeFilms.trouver([](const auto& f) { return f.recette == 955; });
	cout << "\nFilm de 955M$:\n" << *film955;
	assert(film955->titre == "Le Hobbit : La Bataille des Cinq Armées");
	assert(listeFilms.trouver([](const auto&) { return false; }) == nullptr); // Pour la couveture de code: chercher avec un critère toujours faux ne devrait pas trouver.
	// Exemple de condition plus compliquée: (pas demandé)
	auto estVoyelle = [](char c) { static const string voyelles = "AEUOUYaeiouy"; return voyelles.find(c) != voyelles.npos; };
	auto commenceParVoyelle = [&](const string& x) { return !x.empty() && estVoyelle(x[0]); };
	assert(listeFilms.trouver([&](const auto& f) { return commenceParVoyelle(f.titre); }) == listeFilms[0]);
	assert(listeFilms.trouver([&](const auto& f) { return f.acteurs[0]->nom[0] != 'T'; }) == listeFilms[1]);
	assert(listeFilms.trouver([&](const auto& f) { return commenceParVoyelle(f.titre) && f.acteurs[0]->nom[0] != 'T'; }) == listeFilms[2]);

	// Tests chapitre 9:
	Liste<string> listeTextes(2);
	listeTextes.ajoutelisr(make_shared<string>("Bonjour"));
	listeTextes.ajouter(make_shared<string>("Allo"));
	Liste<string> listeTextes2 = listeTextes;
	listeTextes2[0] = make_shared<string>("Hi");
	*listeTextes2[1] = "Allo!";
	assert(*listeTextes[0] == "Bonjour");
	assert(*listeTextes[1] == *listeTextes2[1]);
	assert(*listeTextes2[0] == "Hi");
	assert(*listeTextes2[1] == "Allo!");
	listeTextes = move(listeTextes2);  // Pas demandé, mais comme j'ai fait la méthode on va la tester; noter que la couverture de code dans VisualStudio ne montre pas la couverture des constructeurs/opérateurs= =default.
	assert(*listeTextes[0] == "Hi" && *listeTextes[1] == "Allo!");

	// Détruit et enlève le premier film de la liste (Alien).
	delete listeFilms[0];
	listeFilms.enleverFilm(listeFilms[0]);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	cout << listeFilms;
	*/

	// 2. Construction de la bibliothèque
	ListeFilms listeFilms = creerListe("films.bin");
	vector <Item&> bibliotheque(listeFilms.size());

	for (Film* film : listeFilms.enSpan())
		bibliotheque.push_back(film);

	lireLivre("livres.txt", bibliotheque);
	

	// Pour une couverture avec 0% de lignes non exécutées:
	listeFilms.enleverFilm(nullptr); // Enlever un film qui n'est pas dans la liste (clairement que nullptr n'y est pas).
	assert(listeFilms.size() == 6);

	// Détruire tout avant de terminer le programme.
	listeFilms.detruire(true);
}
