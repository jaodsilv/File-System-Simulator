#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
	long long int size;
	cin >> size;
	size = size * 470712;
	srand(time(NULL));
	for (long long int i = 1; i < size; ++i) {
		cout << (char) rand() % 94 + 33;
	}
	cout << endl;
}