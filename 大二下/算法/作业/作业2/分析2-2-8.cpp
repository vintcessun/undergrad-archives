#include <bits/stdc++.h>
using namespace std;

template<class T>
void reverse(T a[], size_t i, size_t j) {
	while (i < j) {
		swap(a[i], a[j]);
		i++, j--;
	}
}

template<class T>
void solve(T a[], size_t n, size_t k) {
	reverse(a, 0, k - 1);
	reverse(a, k, n - 1);
	reverse(a, 0, n - 1);
}

int main() {
	int a[] = { 2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18 };
	cout << "The list a is ";
	for (int i = 0; i < 16; i++) cout << a[i] << ' ';
	cout << '\n';
	solve(a, (size_t)16, (size_t)7);
	cout << "The change list a is ";
	for (int i = 0; i < 16; i++) cout << a[i] << ' ';
	cout << '\n';
	return 0;
}