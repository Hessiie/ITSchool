#include <iostream>
using namespace std;

double rez() {
	double num;
	double r = 1;
	double m = 1;
	cout << "Introduceti un numar: " << endl;
	cin >> num;
	while (m <= num) {
		r *= m;
		m++;
	}
	return r;
}

int main() {
	cout << "Produsul numerelor: " << rez();
}