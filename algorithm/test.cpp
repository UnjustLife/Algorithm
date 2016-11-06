#include"Tree.h"
#include<iostream>
#include<time.h>
#define N 10
using namespace std;

void func() {
	RedBlackTree tree;

	int elements[N];
	int stern = N-1;
	for (int i = 0; i < N; ++i) {
		elements[i] = i;
	}

	for (stern = N-1; stern >= 0; stern--) {
		
		int k = rand() % (stern + 1);
		tree.insert(elements[k]);
		elements[k] = elements[stern];
	}
	
	tree.printDLR();
	tree.printLDR();
	tree.printLRD();
	tree.selfInseption();
	tree.printTree();

	for (int i = 0; i < N; ++i) {
		elements[i] = i;
	}

	for (stern = N-1; stern >= N-1; stern--) {

		int k = rand() % (stern + 1);
		tree.remove(elements[k]);
		elements[k] = elements[stern];
	}
	tree.printDLR();
	tree.printLDR();
	tree.printLRD();
	tree.selfInseption();
	tree.printTree();

	return;
}
int main() {
	srand(time(0));
	
	for (int i = 0; i < 10; ++i) {
		func();
		cout << endl;
	}

	system("pause");
	return 0;
}