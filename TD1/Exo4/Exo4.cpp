#include <iostream>
#include <cmath>
#include <random>
using namespace std;

int main() 
{
    default_random_engine aleatoire(random_device{}());
    uniform_real_distribution<double> myUnifRealDist(-1.0, 1.0);

    double iterations;
    double pts;
    cout << "Entrez le nombre d'iterations souhaite: ";
    cin >> iterations;

    for(int i = 0; i<iterations; i++) {
        double x = myUnifRealDist(aleatoire);
        double y = myUnifRealDist(aleatoire);

        if ((x*x + y*y)<1.0) 
	{
            pts++;
        }    
    }

    double pi = (4.0 * pts)/iterations;
    cout << "L'approximation de pi est "<< pi;
    return 0;
}