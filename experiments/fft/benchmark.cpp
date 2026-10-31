// Build/run instructions and methodology are in README.md.
#include "baseline.hpp"
#include "candidates.hpp"
#include "library/math/poly.hpp"
#include <bits/stdc++.h>
using namespace std;
using cd = complex<double>;
using cl = complex<long double>;
using Clock = chrono::steady_clock;
double elapsed(Clock::time_point t) {
  return chrono::duration<double, milli>(Clock::now() - t).count();
}
volatile double sink;
void reference(vector<cl> &a) {
  fft_experiment::permute(a);
  int n = a.size();
  for (int k = 1; k < n; k *= 2) {
    vector<cl> w(k);
    for (int j = 0; j < k; ++j)
      w[j] = polar(1.0L, acosl(-1.0L) * j / k);
    w[0] = {1, 0};
    if (k > 1)
      w[k / 2] = {0, 1};
    for (int i = 0; i < n; i += 2 * k)
      for (int j = 0; j < k; ++j) {
        auto x = a[i + j], y = a[i + j + k] * w[j];
        a[i + j] = x + y;
        a[i + j + k] = x - y;
      }
  }
}
template <class T, class F> void transform(string name, int lg, F f) {
  int n = 1 << lg;
  mt19937_64 rng(0x904321);
  vector<complex<T>> input(n);
  for (auto &x : input)
    x = {T(int(rng() % 2000001) - 1000000) / T(1000000),
         T(int(rng() % 2000001) - 1000000) / T(1000000)};
  auto a = input;
  auto start = Clock::now();
  f(a);
  double cold = elapsed(start);
  vector<double> times;
  for (int rep = 0; rep < 9; ++rep) {
    a = input;
    start = Clock::now();
    f(a);
    times.push_back(elapsed(start));
    sink = double(a[n / 3].real());
  }
  sort(times.begin(), times.end());
  vector<cl> ref(input.begin(), input.end());
  reference(ref);
  long double err = 0, norm = 0, mx = 0;
  for (int i = 0; i < n; ++i) {
    auto e = abs(cl(a[i]) - ref[i]);
    err += e * e;
    norm += std::norm(ref[i]);
    mx = max(mx, e);
  }
  cout << "transform," << name << ',' << lg << ',' << cold << ',' << times[4]
       << ',' << sqrtl(err / norm) << ',' << mx << '\n';
}
// norm overload with a distinct name avoids shadowing the std function.
int main(int argc, char **argv) {
  string name = argc > 1 ? argv[1] : "baseline";
  int lo = argc > 2 ? atoi(argv[2]) : 10, hi = argc > 3 ? atoi(argv[3]) : 20;
  cout << setprecision(10);
  for (int lg = lo; lg <= hi; lg += 2) {
    if (name == "baseline")
      transform<double>(name, lg, [](auto &a) { fft_baseline::fft(a); });
    if (name == "r2-tree")
      transform<double>(name, lg, [](auto &a) { fft_experiment::fft<2>(a); });
    if (name == "r2-trig")
      transform<double>(name, lg,
                        [](auto &a) { fft_experiment::fft<2, false, 1>(a); });
    if (name == "r2-double-trig")
      transform<double>(name, lg,
                        [](auto &a) { fft_experiment::fft<2, false, 2>(a); });
    if (name == "r2-fma")
      transform<double>(name, lg,
                        [](auto &a) { fft_experiment::fft<2, true>(a); });
    if (name == "r4-tree")
      transform<double>(name, lg, [](auto &a) { fft_experiment::fft<4>(a); });
    if (name == "r4-fma")
      transform<double>(name, lg,
                        [](auto &a) { fft_experiment::fft<4, true>(a); });
    if (name == "split")
      transform<double>(name, lg, [](auto &a) { fft_experiment::fft<3>(a); });
    if (name == "split-fma")
      transform<double>(name, lg,
                        [](auto &a) { fft_experiment::fft<3, true>(a); });
    if (name == "long-double")
      transform<long double>(name, lg,
                             [](auto &a) { fft_experiment::fft<2>(a); });
    if (name == "production")
      transform<double>(name, lg,
                        [](auto &a) { maomao90::internal::poly::fft::fft(a); });
  }
}
