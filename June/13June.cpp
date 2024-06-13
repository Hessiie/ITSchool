#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <set>
#include <map>
#include <string>

using namespace std;

int length(char* str) {
	if (*str == '\0')
		return 0;
	return 1 + length(str + 1);
}

int upper(char* str, int i = 0) {
	if (str[i] == '\0')
		return 0;
	if (isupper(str[i])) {
		return str[i];
	}
	return upper(str, i + 1);
}

bool prime(int n, int d = 2) {
	if (n <= 2)
		return false;
	if (n % d == 0)
		return false;
	if (d * d > n)
		return true;
	return prime(n, d + 1);
}

int main() {
	char str[] = "meowMeow";
	cout << "Lungimea sirului este de: " << length(str) << " caractere." << endl;
	char upp = upper(str);
	if (upp == 0)
		cout << "Sirul de caractere nu contine o majuscula" << endl;
	else
		cout << "Prima majuscula gasita este: " << upp << " ." << endl;
	int n;
	cout << "Introduceti numarul: " << endl;
	cin >> n;
	if (prime(n))
		cout << "Numarul este prim." << endl;
	else
		cout << "Numarul nu este prim." << endl;
	return 0;
}