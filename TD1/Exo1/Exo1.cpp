#include <iostream>
#include <math.h>
using namespace std;

int main()
{
	int nombre;
	int i;
	bool estpremier = true;

	cout << "Entrer un nombre entier : ";
	cin >> nombre;

	for (i = 2; i <= sqrt(nombre); i++)
		if (nombre % i == 0) {
			estpremier = false;
				break;
	}

	for (i = 3; i <= sqrt(nombre); i += 2)
		if (nombre % i == 0) {
			estpremier = false;
			break;
		}

	if (estpremier == true)
		cout << "Ce nombre est premier";
	else
		cout << "Ce nombre n'est pas premier car il est divisble par " << i << ".";

	return 0;

}