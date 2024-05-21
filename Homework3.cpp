#include <iostream>
using namespace std;

int main()
{
    char c;
    cout << "Introduceti un caracter: ";
    cin >> c;
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
        cout << c << " este o vocala";
    }
    else {
        cout << c << " nu este o vocala";
    }
}


int main()
{
    int luna;
    cout << "Introduceti o luna: ";
    cin >> luna;
    if (luna == 12 || luna <= 2 && luna > 0) {
        cout << "Anotimpul este iarna";
    }
    else if (luna >= 3 && luna <= 5) {
        cout << "Anotimpul este primavara";
    }
    else if (luna >= 6 && luna <= 8) {
        cout << "Anotimpul este vara";
    }
    else if (luna >= 9 && luna <= 11) {
        cout << "Anotimpul este toamna";
    }
    else cout << "Luna introdusa nu exista.";
}