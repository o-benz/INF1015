
#include <iostream>
#include <math.h>

using namespace std;


double validationDonnees(double min, double max) {
	double donnee;
	cin >> donnee;
	while (donnee < min || donnee > max) {
		cout << "La valeur est invalide veuillez recommencer svp.";
		cin >> donnee;
	}
	return donnee;
}



double calculHauteur(double hauteurInitaile, int nRebonds, double coefficientRebond) {
	
	const int gravite = 9.81;
	double hauteurAtteinte;
	double vitesseInitiale;
	double vitesseApresRebond;

	vitesseInitiale = sqrt(2 * gravite * hauteurInitaile);
	vitesseApresRebond = coefficientRebond * vitesseInitiale;

	for (int i = 2; i <= nRebonds; i++) {

		vitesseApresRebond = vitesseApresRebond * coefficientRebond;
	}

	hauteurAtteinte = (pow(vitesseApresRebond, 2) / (2 * gravite));

	cout << "La hauteur atteinte est de: " << hauteurAtteinte;


	return hauteurAtteinte;
}


int main() {

	
	double hauteurInitiale;
	int nRebonds;
	double coefficientRebond;

	
		cout << "Veuillez entrer la Hauteur initale: " << endl;
		hauteurInitiale = validationDonnees(0, INFINITY);

		cout << "Veuillez entrer le nombre de rebonds:  " << endl;
		nRebonds = validationDonnees(0, INFINITY);
		
		cout << "Veuillez entrer le coefficient de rebond: " << endl;
		coefficientRebond = validationDonnees(-1, 1);
	
		calculHauteur(hauteurInitiale, nRebonds, coefficientRebond);

	return 0;
}