#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <vector>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include "algorithms.h"

using namespace std;
#define SIZE 10
#define ARRAY { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }

inline void separate() {
	cout << endl;
}

void print(vector<int>& v) {
	for (size_t i = 0; i < SIZE; ++i)
		cout << v[i] << ' ';

	separate();
};

void print(int* a) {
	for (int i = 0; i < SIZE; ++i)
		std::cout << a[i] << ' ';

	separate();
}

void print(forward_list<int>& l) {
	for (auto i = l.begin(); i != l.end(); ++i)
		std::cout << *i << ' ';
	
	separate();
}

int main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/* 1 - Sorting */

	// 1a
	vector<int> v = ARRAY;
	random_shuffle(v.begin(), v.end());
	print(v);
	sort(v.begin(), v.end());
	print(v);

	separate();

	// 1b
	int a[] = ARRAY;
	random_shuffle(a, a + SIZE);
	print(a);
	sort(a, a + SIZE);
	print(a);

	separate();

	// 1c
	random_shuffle(v.begin(), v.end());
	print(v);
	sort(v.rbegin(), v.rend());
	print(v);

	separate();

	// 1d
	random_shuffle(a, a + SIZE);
	print(a);
	sort(a, a + SIZE, [](int l, int r) { return l > r; });
	print(a);

	separate();

	/* 2 - Remove even numbers */

	print(v);
	v.erase(remove_if(v.begin(), v.end(), [](int l) { return l % 2 == 0; }), v.end());
	print(v);

	separate();

	/* 3 - Forward Sorting */

	forward_list<int> l = ARRAY;
	print(l);
	ForwardSort(l.begin(), l.end());
	print(l);
	ForwardSort(l.begin(), l.end(), [](int l, int r) {return l > r; });
	print(l);
	system("pause");
}