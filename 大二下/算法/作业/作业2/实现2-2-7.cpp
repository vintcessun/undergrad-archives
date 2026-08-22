#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll n;

static vector<vector<ll>> get_C(const ll n) {
	vector<vector<ll>> C(n + 1, vector<ll>(n + 1, 0));
	for (ll i = 0; i <= n; i++) {
		C[i][0] = 1;
		for (ll j = 1; j <= i; j++) {
			C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
		}
	}
	return C;
}

static ll solve(const ll n, const vector<vector<ll>>& C) {
	vector<ll> f(n + 1);
	f[0] = 1;
	for (ll i = 0; i < n; i++) {
		f[i + 1] = 0;
		for (ll j = 0; j <= i; j++) {
			f[i + 1] += C[i][j] * f[i - j];
		}
	}
	return f[n];
}

int main() {
	while (cin >> n) {
		auto C = get_C(n);
		auto ans = solve(n, C);
		cout << ans << endl;
	}
	return 0;
}