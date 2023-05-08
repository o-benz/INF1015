#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Dictionnaire {
	string word;
	string nature;
	string definition;
};



int main() {
	string longestWord = "a";
	ifstream myfile;
	Dictionnaire dictionnaire[4];
	myfile.open("dictionnaire.txt");
	for (int i = 0; i < 4; i++) {
		getline(myfile, dictionnaire[i].word, '\t');
		getline(myfile, dictionnaire[i].nature, '\t');
		getline(myfile, dictionnaire[i].definition, '\n');
		if (longestWord.size() < (dictionnaire[i].word).size()) {
			longestWord = dictionnaire[i].word;
		}
	}
	cout << longestWord;

	myfile.close();
}