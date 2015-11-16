#include <iostream>
using namespace std;

void print_test_cp_rm(int s) {
	for(int i = 0; i < 30; ++i) {
		cout << "cp /home/joao/workspace/SO/File-System-Simulator/" << s << "mb.txt /\n";
		cout << "time 1\n";
		cout << "rm /" << s << "mb.txt\n";
		cout << "time 1\n";
	}
}

void print_teste_7() {
	for(int i = 0; i < 30; ++i) {
		for (int j = 0; j < 99; ++j) {
			cout << "mkdir /a";
			for (int k = 0; k < j; ++k) {
				cout << "/a";
			}
			cout << endl;
		}
		cout << "time 0\n";
		cout << "rmdir /a\n";
		cout << "time 1\n";
	}
}

void print_teste_8() {
	for(int i = 0; i < 30; ++i) {
		for (int j = 0; j < 99; ++j) {
			cout << "mkdir /a";
			for (int k = 0; k < j; ++k) {
				cout << "/a";
			}
			cout << endl;
			for (int l = 0; l < 200; ++l) {
				cout << "touch /a";
				for (int k = 0; k < j; ++k) {
					cout << "/a";
				}
				cout << "/b" << l << endl;
			}
		}
		cout << "time 0\n";
		cout << "rmdir /a\n";
		cout << "time 1\n";
	}
}

int main() {
	cout << "mount /tmp/hd0\n";

	/* hd vazio */

	/* teste 1 e 4 */
	cout << "time 0\n";
	print_test_cp_rm(1);
	/* teste 2 e 5 */
	print_test_cp_rm(10);
	/* teste 3 e 6 */
	print_test_cp_rm(30);

	print_teste_7();
	print_teste_8();

	/* hd 10MB */
	cout << "mkdir /10mb1\n";
	cout << "cp /home/joao/workspace/SO/File-System-Simulator/10mb.txt /10mb1\n";


	/* teste 1 e 4 */
	cout << "time 0\n";
	print_test_cp_rm(1);
	/* teste 2 e 5 */
	print_test_cp_rm(10);
	/* teste 3 e 6 */
	print_test_cp_rm(30);

	print_teste_7();
	print_teste_8();

	/* hd 50MB */
	cout << "mkdir /10mb2\n";
	cout << "cp /home/joao/workspace/SO/File-System-Simulator/10mb.txt /10mb2\n";
	cout << "mkdir /30mb\n";
	cout << "cp /home/joao/workspace/SO/File-System-Simulator/30mb.txt /30mb\n";

	/* teste 1 e 4 */
	cout << "time 0\n";
	print_test_cp_rm(1);
	/* teste 2 e 5 */
	print_test_cp_rm(10);
	/* teste 3 e 6 */
	print_test_cp_rm(30);

	print_teste_7();
	print_teste_8();

}