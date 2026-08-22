#include <bits/stdc++.h>
using namespace std;

template<class T>
requires totally_ordered<T>
void binary_search(T a[], const T& x, size_t left, size_t right, size_t& i, size_t& j) {
	size_t middle;
	while(left<=right){
		middle = (left + right) / 2;
		if (x == a[middle]) {
			i = middle, j = middle;
			return;
		}
		if (x > a[middle]) {
			left = middle + 1;
		}
		else {
			right = middle - 1;
		}
	}
	i = right;
	j = left;
}

int main() {
	int a[] = { 2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18 };
	cout << "The list a is ";
	for (int i = 0; i < 16; i++) cout << a[i] << ' ';
	cout << '\n';
	size_t i, j;
	binary_search(a, 8, (size_t)0, (size_t)15, i, j);
	cout << "The found i = " << i << ", j = " << j << "\n";
	binary_search(a, 10, (size_t)0, (size_t)15, i, j);
	cout << "The found i = " << i << ", j = " << j << "\n";
	return 0;
}