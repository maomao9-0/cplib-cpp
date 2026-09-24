#define PROBLEM                                                                \
  "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"

#include "library/math/poly.hpp"
using namespace maomao90;
using mint = static_modint<>;
using poly = Poly<mint>;

void print(const poly &a) {
  for (int i = 0; i < a.size(); ++i)
    cout << a[i] << ' ';
  cout << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int d;
  long long k;
  cin >> d >> k;
  vector<mint> initial(d), recurrence(d);
  for (auto &x : initial)
    cin >> x;
  for (auto &x : recurrence)
    cin >> x;
  cout << poly::linear_recurrence(initial, recurrence, k) << '\n';
}
