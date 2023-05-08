/*
* \file   td5.cpp
* \author Omar Benzekri et Ines Kabbaj
* \date   2 avril 2023
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures_td5.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include <forward_list>
#include <map>
#include <memory>
#include <numeric>
#include <functional>
#include <map>
#include <numeric>
#include "cppitertools/range.hpp"
#include "cppitertools/enumerate.hpp"
#include "gsl/span"

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif

#if __has_include("bibliotheque_cours.hpp")
#include "bibliotheque_cours.hpp"
#define BIBLIOTHEQUE_COURS_INCLUS
using bibliotheque_cours::cdbg;
#else
auto& cdbg = clog;
#endif

#if __has_include("verification_allocation.hpp")
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#endif

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	// cdbg.setTee(&clog);  // Décommenter cette ligne pour que cdbg affiche sur la console en plus de la "Sortie" du débogueur.
	
	bibliotheque_cours::executerGoogleTest(argc, argv); // Attention de ne rien afficher avant cette ligne, sinon l'Explorateur de tests va tenter de lire votre affichage comme un résultat de test.
	#endif
}

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

span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

//void ListeFilms::enleverFilm(const Film* film)  // Pas utile dans ce TD
//{
//	for (Film*& element : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
//		if (element == film) {
//			if (nElements > 1)
//				element = elements[nElements - 1];
//			nElements--;
//			return;
//		}
//	}
//}
//]

shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& acteur : film->acteurs.enSpan()) {
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
		std :: cout << "Création Acteur " << acteur.nom << endl;
		return make_shared<Acteur>(move(acteur));  // Le move n'est pas nécessaire mais permet de transférer le texte du nom sans le copier.
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->titre       = lireString(fichier);
	film->realisateur = lireString(fichier);
	film->anneeSortie = lireUint16 (fichier);
	film->recette     = lireUint16 (fichier);
	auto nActeurs = lireUint8 (fichier);
	film->acteurs = ListeActeurs(nActeurs);  // On n'a pas fait de méthode pour changer la taille d'allocation, seulement un constructeur qui prend la capacité.  Pour que cette affectation fonctionne, il faut s'assurer qu'on a un operator= de move pour ListeActeurs.
	std :: cout << "Création Film " << film->titre << endl;

	for ([[maybe_unused]] auto i : range(nActeurs)) {  // On peut aussi mettre nElements avant et faire un span, comme on le faisait au TD précédent.
		film->acteurs.ajouter(lireActeur(fichier, listeFilms));
	}

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
void ListeFilms::detruire()
{
	delete[] elements;
}

ostream& operator<< (ostream& os, const Acteur& acteur)
{
	return os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

ostream& operator<< (ostream& os, const Affichable& affichable)
{
	affichable.afficherSur(os);
	return os;
}

void Item::afficherSur(ostream& os) const
{
	os << "Titre: " << titre << "  Année:" << anneeSortie << endl;
}

void Film::afficherSpecifiqueSur(ostream& os) const
{
	os << "  Réalisateur: " << realisateur << endl;
	os << "  Recette: " << recette << "M$" << endl;
	os << "Acteurs:" << endl;
	for (auto&& acteur : acteurs.enSpan())
		os << *acteur;
}

void Film::afficherSur(ostream& os) const
{
	Item::afficherSur(os);
	Film::afficherSpecifiqueSur(os);
}

void Livre::afficherSpecifiqueSur(ostream& os) const
{
	os << "  Auteur: " << auteur << endl;
	os << "  Vendus: " << copiesVendues << "M  Pages: " << nPages << endl;
}

void Livre::afficherSur(ostream& os) const
{
	Item::afficherSur(os);
	Livre::afficherSpecifiqueSur(os);
}

void FilmLivre::afficherSur(ostream& os) const
{
	Item::afficherSur(os);
	os << "Combo:" << endl;
	// L'affichage comme l'exemple sur Discord est accepté, ici on montre comment on pourrait séparer en deux méthodes pour ne pas avoir le même titre d'Item affiché plusieurs fois.
	Film::afficherSpecifiqueSur(os);
	os << "Livre:" << endl;
	Livre::afficherSpecifiqueSur(os);
}

void Item::lireDe(istream& is)
{
	is >> quoted(titre) >> anneeSortie;
}
void Livre::lireDe(istream& is)
{
	Item::lireDe(is);
	is >> quoted(auteur) >> copiesVendues >> nPages;
}
Livre::Livre(istream& is) {
	lireDe(is);
}

template <typename T>
//void afficherListeItems(span<unique_ptr<Item>> listeItems)
void afficherListeItems(T& listeItems)
{
	static const string ligneDeSeparation = "\033[32m────────────────────────────────────────\033[0m\n";
	cout << ligneDeSeparation;
	for (auto&& item : listeItems) {
		cout << *item << ligneDeSeparation;
	}
}

#pragma region "Exemples de tests unitaires"//{
#ifdef TEST
// Pas demandés dans ce TD mais sert d'exemple.

TEST(tests_ListeFilms, tests_simples) {
	ListeFilms li;
	EXPECT_EQ(li.size(), 0);
	EXPECT_EQ(li.capacity(), 0);
	Film a, b, c;
	li.ajouterFilm(&a);
	li.ajouterFilm(&b);
	li.ajouterFilm(&c);
	EXPECT_EQ(li.size(), 3);
	EXPECT_GE(li.capacity(), 3);
	EXPECT_EQ(li[0], &a);
	EXPECT_EQ(li[1], &b);
	EXPECT_EQ(li[2], &c);
	li.detruire();
}

template <> struct Film::accessible_pour_tests_par<::testing::Test> {
	template <typename T> // Template pour ne pas avoir une version const et non const.
	static auto& realisateur(T&& film) { return film.realisateur; }
};

TEST(tests_ListeFilms, trouver) {
	using AccesFilmPourTests = Film::accessible_pour_tests_par<::testing::Test>;
	ListeFilms li;
	Film films[3];
	string realisateurs[] = {"a","b","c","e"};
	for (auto&& [i,f] : enumerate(films)) {
		AccesFilmPourTests::realisateur(f) = realisateurs[i];
		li.ajouterFilm(&f);
	}
	for (auto&& [i,r] : enumerate(realisateurs)) {
		Film* film = li.trouver([&](const Film& f) { return AccesFilmPourTests::realisateur(f) == r; });
		// Le << après EXPECT_... permet d'afficher quelque chose en cas d'échec du test. Dans ce cas-ci, on veut savoir pour quel i ça a échoué.
		EXPECT_EQ(film, i < size(films) ? &films[i] : nullptr) << "  pour i=" << i;
	}
	li.detruire();
}

#endif
#pragma endregion//}

int main(int argc, char* argv[])
{
	initialiserBibliothequeCours(argc, argv);

	//int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Detected memory leak" de "4 bytes" affiché dans la "Sortie", qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	vector<Item*> items;
	
	{
		ListeFilms listeFilms = creerListe("films.bin");
		for (auto&& film : listeFilms.enSpan())
			items.push_back(film);  // On transert la possession.
		listeFilms.detruire();
	}

	{
		ifstream fichier("livres.txt");
		fichier.exceptions(ios::failbit);  // Pas demandé mais permet de savoir s'il y a une erreur de lecture.
		while (!ws(fichier).eof())
			items.push_back(new Livre(fichier));
	}
	
	// items.push_back(make_shared<FilmLivre>(dynamic_cast<Film&>(*items[4]), dynamic_cast<Livre&>(*items[9])));  // On ne demandait pas de faire une recherche; serait direct avec la matière du TD5.

	afficherListeItems(items);

	// TODO 1.1 - Copier les pointeurs du vecteur d’items final de la biliothèque dans une forward_list
	forward_list<Item*> listeOO;	
	for (auto element = items.rbegin(); element != items.rend(); ++element) {
		listeOO.push_front(*element);
	}
	listeOO.reverse();
	
	cout << ligneDeSeparation << endl;
	cout << "1.1 Afficher la forward_list ordre original :" << endl;
	afficherListeItems(listeOO);

	// TODO 1.2 - Copier la liste qui est en ordre original à l’envers
	forward_list<Item*> listeOI;		
	for (auto& element : listeOO)
		listeOI.push_front(element);
	
	cout << ligneDeSeparation << endl;
	cout << "1.2 Afficher la forward_list à l'envers :" << endl;
	afficherListeItems(listeOI);

	// TODO 1.3 - Copier la liste qui est en ordre original dans le même ordre qu’elle est, en O(n) dans une autre forward_list sans passer par un conteneur intermédiaire ni avec une fonction récursive
	forward_list<Item*> autreListeOO;	
	forward_list<Item*>::iterator i;
	i = autreListeOO.before_begin();
	for (auto& element : listeOO)
		i = autreListeOO.insert_after(i, element);
	
	cout << ligneDeSeparation << endl;
	cout << "1.3 Afficher la copie de la forward_list dans l'ordre original : " << endl;
	afficherListeItems(autreListeOO);


	// TODO 1.4 - Copier la liste qui est en ordre original à l’envers dans un vector avec les mêmes contraintes que ci-dessus
	// TODO indiquer le niveau de complexite : O(n)
	int tailleVecteur = items.size();					// O(1)
	vector<Item*> vecteurInverse(tailleVecteur);		// O(1)
	int x = tailleVecteur-1;							// O(1)
	for (auto& element : listeOO)   			    	// O(n)
		vecteurInverse[x--] = element;					// O(1)

	cout << ligneDeSeparation << endl;
	cout << "1.4 Afficher le vecteur a l'envers : " << endl;
	afficherListeItems(vecteurInverse);
	
	// TODO 1.5 - Afficher les acteurs du premier film (Alien)
	cout << ligneDeSeparation << endl;
	cout << "1.5 Affichage des acteurs du premier film : " << endl << endl;

	Film* film = dynamic_cast<Film*>(items[0]);
	cout << "Les acteurs du film " << film->titre << " sont : " << endl;
	for (auto&& acteur : film->acteurs)
		cout << *acteur << endl;	

	// TODO 2.1 - Conteneur qui trie les éléments par ordre alphabétique
	map<string, Item*> mapItems;
	for (auto& element : items)
		mapItems.insert(pair<string, Item*>(element->titre, element));

	cout << ligneDeSeparation << endl;
	cout << "2.1 Affichage du mapItems : " << endl << endl;
	for (const auto& [cle, valeur] : mapItems)
		cout << "Clé : " << cle << endl << "Valeur : " << *valeur << endl;

	// TODO 2.2 - Conteneur qui permet de trouver les items par titre
	unordered_map<string, Item*> unorderedMapItems;
	for (auto& element : items)
		unorderedMapItems.insert(pair<string, Item*>(element->titre, element));

	cout << ligneDeSeparation << endl;
	cout << "2.2 On cherche le film The Hobbit dans le conteneur d'item :" << endl << endl;
	auto recherche = unorderedMapItems.find("The Hobbit");
	if (recherche->first == "The Hobbit")
		cout << "Oeuvre : " << recherche->first << endl << "Informations : " << *recherche->second << endl;
	else
		cout << "L'item n'a pas été trouvé" << endl;

	// TODO 3.1 - Utilisez l’algorithme copy_if ou copy pour copier les items qui « sont des » Film de liste faite en 1.1 vers un
	// vector, en gardant l’ordre, en une ligne de programme. Vous avez le droit d’écrire une fonction
	// d’adaptation, d’une ligne, qui permet de passer un « range » au lieu de deux itérateurs à ces algorithmes 
	vector<Item*> vecteurFilm;
	copy_if(listeOO.begin(), listeOO.end(), back_inserter(vecteurFilm), [](Item* item) { return dynamic_cast<Film*>(item) != nullptr; });

	cout << ligneDeSeparation << endl;
	cout << "3.1 Affichage du vecteur de film : " << endl << endl;
	afficherListeItems(vecteurFilm);

	// TODO 3.2 - Faites la somme des recettes des films trouvés ci-dessus, en une ligne (pas de « for »). 
	int sommeRecette = accumulate(vecteurFilm.begin(), vecteurFilm.end(), 0, [](int somme, Item* item) { return somme + dynamic_cast<Film*>(item)->recette; });

	cout << ligneDeSeparation << endl;
	cout << "3.2 Somme des recettes des films : " << sommeRecette << endl << endl;

}
