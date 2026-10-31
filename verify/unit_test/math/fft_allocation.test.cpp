#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <new>
#include <numbers>
#include <thread>
#include <vector>

#include "library/math/poly.hpp"
using namespace std;
namespace fft_impl = maomao90::internal::poly::fft;

// Fail one allocation in the calling thread, without affecting thread startup
// or allocations from other threads. Resume normal allocation after throwing.
thread_local int allocations_before_failure = -1;
[[gnu::noinline]] void *operator new(size_t size) {
  if (allocations_before_failure == 0) {
    allocations_before_failure = -1;
    throw bad_alloc();
  }
  if (allocations_before_failure > 0)
    --allocations_before_failure;
  if (void *memory = malloc(size ? size : 1))
    return memory;
  throw bad_alloc();
}
[[gnu::noinline]] void operator delete(void *memory) noexcept { free(memory); }
[[gnu::noinline]] void operator delete(void *memory, size_t) noexcept {
  free(memory);
}

void check_transform(const vector<complex<double>> &input,
                     const vector<complex<double>> &actual) {
  int n = input.size();
  for (int k = 0; k < n; ++k) {
    complex<long double> expected{};
    for (int j = 0; j < n; ++j) {
      auto w = polar(1.0L, 2 * numbers::pi_v<long double> * ((j * k) % n) / n);
      expected += complex<long double>(input[j]) * w;
    }
    assert(abs(complex<long double>(actual[k]) - expected) < 1e-10L);
  }
}

void check_allocation_failure(int budget) {
  vector<complex<double>> small(8), input(256);
  for (int i = 0; i < (int)input.size(); ++i)
    input[i] = {double(i % 7 - 3), double(i % 11 - 5)};
  small.assign(input.begin(), input.begin() + 8);
  auto transformed_small = small;
  fft_impl::fft(transformed_small);
  auto actual = input;
  allocations_before_failure = budget;
  bool failed = false;
  try {
    fft_impl::fft(actual);
  } catch (const bad_alloc &) {
    failed = true;
    // All cache allocations occur before mutating the transform input.
    assert(actual == input);
  }
  allocations_before_failure = -1;
  assert(failed == (budget < 7));
  if (failed) {
    // Already completed root layers remain usable after a failed growth.
    transformed_small = small;
    fft_impl::fft(transformed_small);
    check_transform(small, transformed_small);
    // Then resume construction from the last completed layer.
    fft_impl::fft(actual);
  }
  check_transform(input, actual);
}

int main() {
  for (int budget = 0; budget <= 7; ++budget) {
    // A fresh thread gives each trial an independent root cache.
    thread trial([budget] { check_allocation_failure(budget); });
    trial.join();
  }
  // Also extend same-type caches concurrently to check independent ownership.
  vector<thread> workers;
  for (int i = 0; i < 4; ++i)
    workers.emplace_back([] { check_allocation_failure(7); });
  for (auto &worker : workers)
    worker.join();
  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
