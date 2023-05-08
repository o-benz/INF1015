#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

double validationDonnees(double min, double max) {
	int donnee;
	cin >> donnee;
	while (donnee < min || donnee > max) {
		cout << "La valeur est invalide veuillez recommencer svp.";
		cin >> donnee;
	}
	return donnee;
}


void calcul(double pretArgent, double tauxAnnuel, double montantRembourser) {

	int nMois = 0;
	double tauxMensuel;
	double montantRestant;
	double interetMois;
	double sommeInteret = 0;
	

	tauxMensuel = tauxAnnuel / 100 / 12;


	while (pretArgent > montantRembourser) {

		nMois++;

		interetMois = pretArgent * tauxMensuel;

		cout << "Interet pour le mois " << nMois << " : " << interetMois << endl;

		montantRestant = pretArgent + interetMois - montantRembourser;

		cout << "Il reste a payer: " << montantRestant << " " << endl;

		pretArgent = montantRestant;

		sommeInteret += interetMois;
	}

	interetMois = pretArgent * tauxMensuel;
	sommeInteret += interetMois;
	
	nMois++;
	cout << "Interet pour le mois " << nMois << " : " << interetMois << endl;
	
	cout << "Le montant a ete rembourser en: " << nMois << " mois. Et la somme des interets est de: "  << setprecision(2) << sommeInteret << "." << endl;

}

int main() {

	double pretArgent, montantRembourser, tauxAnnuel;


	cout << "Veuillez entrer le montant pretee: ";
	pretArgent = validationDonnees(0, INFINITY);

	cout << "Veuillez entrer le montant rembourser: ";
	montantRembourser = validationDonnees(0, INFINITY);

	cout << "Veuillez entrer le taux d'interet annuel: ";
	tauxAnnuel = validationDonnees(0, 100);

	calcul(pretArgent, tauxAnnuel, montantRembourser);


	return 0;
}