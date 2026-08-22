#include <bits/stdc++.h>
using namespace std;

unordered_map<int, int> m;
int n, k;

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	cin >> n;
	while (n--) {
		cin >> k;
		m[k]++;
	}
	int M = 0, s;
	for (const auto& e : m) {
		if (e.second > M) {
			M = e.second;
			s = e.first;
		}
	}
	cout << s << '\n' << M << '\n';
	return 0;
}