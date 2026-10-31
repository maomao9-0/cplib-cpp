# Scalar FFT accuracy and speed experiments

The selected implementation uses mixed radix-4/radix-2 butterflies, extended-
precision root generation, and **three half-size complex FFTs for real
convolution**. Each real input is transformed separately. Modular convolution
centers both its residues and split digits. This was the best speed/accuracy
tradeoff among the implementations measured here; it is not a claim of optimality
on every processor or input.

## Research

Primary sources consulted:

- [FFTW accuracy commentary](https://www.fftw.org/accuracy/comments.html):
  inaccurate twiddle recurrences can dominate butterfly roundoff. Compute roots
  accurately; avoid a long chain of double-precision multiplications.
- [FFTW measurement methodology](https://www.fftw.org/accuracy/method.html):
  compare forward transforms with a higher-precision reference and report norm
  errors. Round trips alone can hide correlated errors.
- [KACTL FFT](https://github.com/kth-competitive-programming/kactl/blob/main/content/numerical/FastFourierTransform.h)
  and [split modular convolution](https://github.com/kth-competitive-programming/kactl/blob/main/content/numerical/FastFourierTransformMod.h):
  these are the algorithms used by the starting implementation. Its binary tree
  of `long double` roots is already a good accuracy choice.
- [Simon Lindholm's analysis](https://github.com/simonlindholm/fft-precision/blob/master/fft-precision.md):
  packed-input error depends on the sum of the squared operand norms, rather
  than only their product. Thus unequal operand magnitudes matter. The analysis
  also explains why directly evaluating double-precision sine/cosine is worse
  than generating roots in extended precision.
- [Johnson and Frigo, modified split-radix FFT](https://www.fftw.org/~athena/papers/newsplit-pub.pdf):
  fewer arithmetic operations do not necessarily mean less elapsed time. This
  experiment implements ordinary split-radix, not the paper's recursively
  rescaled modified algorithm.
- [Percival's FFT error analysis](https://www.daemonology.net/papers/fft.pdf):
  empirical successes are not unconditional integer-rounding guarantees.

## Implementations compared

`baseline.hpp` preserves the original transform. `baseline_convolution.hpp`
preserves the original convolution algorithms from commit `8c8de73`.
`candidates.hpp` implements:

1. Radix-2 with on-the-fly bit reversal instead of allocating a reversal vector.
2. Radix-2 with binary-tree long-double roots, direct long-double trigonometry,
   or direct double trigonometry.
3. Radix-4 with independently rounded first/third twiddle powers.
4. Recursive split-radix after bit reversal.
5. Scalar FMA multiplication variants of radix-2, radix-4, and split-radix.
6. All-long-double radix-2 arithmetic.

`convolution.cpp` compares the original packed square, a factored spectral
product, power-of-two balancing before packing, three separate full-size FFTs,
and three half-size FFTs. The last packs even/odd terms of each operand,
recovers their real spectra, multiplies those spectra, and repacks the output's
even/odd spectra for the inverse transform. This candidate is retained for
comparison. The final implementation forms the even/odd products directly at
the half-size grid, avoiding subtractive recovery and extra root setup. Neither
packs different operands into the same FFT.

`split.cpp` compares original nonnegative digits, centered residues, and centered
residues with centered low digits. All four split-product lanes are measured
before rounding, against exact integer dot products at selected positions.

## Measurement conditions

Intel Core Ultra 9 275HX, Linux x86-64, GCC 16.2.1. This platform has 64-bit
significand `long double` and 53-bit significand `double`. Final portable runs use
`-O2`; native runs use `-O3 -march=native`. **Both disable loop and SLP
vectorization and floating-point contraction**, with
`-fno-tree-vectorize -fno-tree-slp-vectorize -ffp-contract=off`. No SIMD intrinsics
are used. Explicit FMA candidates use scalar `std::fma`; native builds have
hardware support. No fast-math flags are used.

Final `*-generic.csv` and `*-native.csv` runs are pinned to logical CPU 0 and run
sequentially. Their matching `.env.txt` files record compiler, flags, and machine.
Initial candidate screening files (`*-candidates-native.csv` and
`transform-odd-native.csv`) were unpinned; treat small timing differences there
as inconclusive. The original candidate matrix measured first calls after
incremental cache growth. Final transform runs start a **new process for each
size**, so `first_ms` includes truly cold root setup and `median_ms` is the median
of nine subsequent transforms. Input copies are outside transform timing.

Convolution timings include all allocations, packing, multiplication,
reconstruction, and output construction. They report the median of five calls
after one warmup. Backend timings alternate baseline/updated measurement order
and use nine calls each. Frequencies and OS scheduling are uncontrolled; these
are measurements on one machine, not universal speed guarantees.

All random generators have fixed seeds. Transform lengths span every power of
two from 256 through 2,097,152 in the final comparison, including both radix
parities. Convolution input lengths are 128, 2,048, 32,768, and 524,288, with the
second operand 17 terms shorter. Patterns include signed random, positive
constant, alternating, sparse impulse, cancellation-heavy, and unequal-magnitude
inputs. The latter multiplies one operand by 2^35 and the other by 2^-35, leaving
the exact output unchanged.

## Results

Portable scalar timings (milliseconds):

| Workload | Original | Selected | Speedup |
| --- | ---: | ---: | ---: |
| FFT, length 2^16, warm | 1.382 | 0.968 | 1.43x |
| FFT, length 2^20, warm | 36.247 | 19.985 | 1.81x |
| FFT, length 2^21, warm | 114.605 | 58.958 | 1.94x |
| FFT, length 2^20, cold | 80.952 | 70.588 | 1.15x |
| Real convolution, input length 2^19, random | 101.139 | 42.054 | 2.40x |
| Real convolution, input length 2^19, unequal magnitudes | 96.935 | 40.632 | 2.39x |
| Integer convolution, input length 2^19 | 94.663 | 42.127 | 2.25x |
| Modular convolution, input length 2^19, modulus 1000000007 | 174.829 | 125.028 | 1.40x |

At length 1,024 the portable first call was 0.052 ms originally
and 0.054 ms with the selected implementation. Cold timings include
allocator and thread-local initialization and are noisier than warmed timings.

Maximum absolute error at 71 sampled output positions, input length 2^19:

| Pattern | Original | Selected |
| --- | ---: | ---: |
| Random | 2.38e-07 | 2.46e-07 |
| Positive constants | 0.000122 | 0.000122 |
| Alternating | 3.47e-05 | 7.18e-06 |
| Unequal magnitudes | 1.3e+13 | 2.46e-07 |
| Sparse impulse | 3.38e-08 | 4.55e-13 |
| Cancellation-heavy | 0.297 | 1.61e-05 |

These are sampled absolute errors, not proven worst-case bounds. Ordinary
random/positive inputs do not get orders of magnitude more accurate. The major
improvement is removing the interaction between operands with different
magnitudes.

Independent 113-bit reference results at transform length 2^14:

| Input | Original relative L2 | Selected relative L2 |
| --- | ---: | ---: |
| Random complex | 2.496e-16 | 2.358e-16 |
| Impulse | 1.517e-16 | 1.517e-16 |
| Complex tone | 2.350e-16 | 1.725e-16 |

The quad reference uses Boost `cpp_bin_float_quad`, independently generated
trigonometric roots, and radix-2 butterflies. It tests lengths 32, 1,024, and
16,384 with random, constant, alternating, impulse, and tone inputs. Constant
and alternating transforms are exact for these dyadic inputs. Large transform
timing files use a separate long-double FFT with direct trigonometric roots as
the reference, not quad arithmetic. The long-double candidate/reference
comparison cannot measure absolute long-double accuracy; use `accuracy-quad.csv`
for that. The selected long-double random transform's relative L2 error at
length 2^14 is 2.84e-19 against quad.

For modular splitting, 108 cases cover lengths 2^10, 2^16, 2^19, and 2^20;
moduli 998244353, 1000000007, and 2147483647; random, maximum-residue, and
adversarial digit patterns; and three representations. Across this matrix:

| Representation | Largest sampled lane error | Incorrect sampled rounded lanes |
| --- | ---: | ---: |
| Nonnegative residues/digits | 0.75 | 28 |
| Centered residues | 0.09375 | 0 |
| Centered residues and low digits | 0.078125 | 0 |

These use the same updated FFT, isolating the representation change. On random
inputs of length 2^20 at modulus 2147483647, the lane error drops from 0.1875 to
0.000244. Maximum residues become -1 and gain a much larger improvement. The
per-lane bound remains empirical; larger or unfavorable inputs can still fail.

Why other approaches were not selected:

- Direct long-double trig roots increase first-use time substantially without
  improving double-transform error over the existing binary-tree generation.
  Direct double trig roughly doubles relative L2 error at large sizes.
- Split-radix gives slightly lower random-transform error, but is generally
  slower than radix-4, especially at small/medium sizes.
- Scalar FMA slightly improves random-transform norms, but did not consistently
  improve convolution speed or cancellation error. In the initial half-size
  convolution comparison at length 2^19, ordinary radix-4 had cancellation error
  9.92e-6 versus 4.00e-5 for the FMA variant. A fused operation does not guarantee
  a more accurate final answer for every input.
- Power-of-two balancing fixes much of the unequal-magnitude problem, but still
  shares a transform and is slower than the half-size method. Separate full-size
  transforms are robust to operand imbalance but require substantially more work.
- All-long-double butterflies improve accuracy greatly but cost several times
  as much. They remain available by choosing long-double coefficients rather
  than imposing that cost on every double/integer product.

## Implementation details and limits

Root generation retains only its widest extended-precision layer, and computes
third powers by indexing/sign changes in that layer rather than multiplying
rounded roots. Quadrantal roots are exact. On this ABI the double root cache
uses about 40 bytes per maximum transform element, versus 48 previously; the
per-call 4-byte reversal array is also removed. Each thread owns its cache.

Real convolution preserves long-double input precision. Previously even
long-double inputs were converted to double. Integer reconstruction now rounds
the fractional part after truncation: adding 0.5 first can change an already
integral odd double near 2^52. That bug is covered independently of FFT error.

The implementation is scalar standard C++20. Standard compiler defaults may
contract arithmetic or vectorize caller builds; the reported comparisons
explicitly disable both. All transform lengths must be powers of two, at most
2^30, and finite intermediate values are required. Empty transforms and singleton
transforms are supported. Floating backends do not promise exact products for
arbitrary coefficients. Not every integer above 2^53 is representable in
`double`, regardless of FFT quality. Use the NTT/CRT backend within its documented
range when exact integer/modular results are required. Long-double precision and performance are platform dependent.

## Regression review

A second review found and fixed these additional cases:

- Form the output's even/odd spectra directly as `Ae*Be + w*Ao*Bo` and
  `Ae*Bo + Ao*Be`, where `w` is a half-size Fourier root. Recovering the odd
  spectrum by subtracting full spectral products lost small coefficients next
  to large ones. Multiplication by an impulse now preserves adjacent `1e16`
  and `1` coefficients exactly in the targeted regression. This also halves the
  root-cache length needed for real convolution.
- Apply fractional-part rounding to unsigned split components too. Squaring a
  single coefficient `((1ULL << 26) + 1) << 15` previously introduced an erroneous
  `2^30` term through the old `+0.5` rounding step.
- Widen unsigned split reconstruction before reducing to the coefficient type,
  and use unsigned multiplication in the quadratic kernel. This avoids premature
  float-to-narrow-integer conversions and signed `int` overflow from promotion
  of `uint16_t` operands. Tests cover narrow coefficients across the quadratic
  threshold, unsigned wraparound, and 128-bit reconstruction.
- Track the last completed root layer separately from allocated vector sizes.
  Failed cache growth must not make subsequent transforms consume incomplete
  roots. Fault injection checks every growth allocation, recovery, and concurrent
  independent caches.
- Reject `bool` coefficient backends explicitly, since `vector<bool>` proxies
  do not support the required coefficient operations. Clarify that supported
  modular integer types require modulus at least 2.

The final convolution/backend CSVs are refreshed after these fixes. The initial
candidate matrix remains unchanged as a record of the alternatives tested.

## Verification

- `verify/unit_test/math/fft.test.cpp`: direct long-double DFT checks for float,
  double, and long double; transform round trips; alternating radix parity and
  cache growth/shrinkage; naive and exact NTT/CRT comparisons; empty inputs and
  lengths around the quadratic threshold; sparse, negative, cancellation,
  magnitude-ratio 2^70 and approximately 1e400 cases; long-double preservation;
  complex convolution; unsigned wraparound; narrow and 128-bit reconstruction;
  dynamic modulus changes; large modular rounding failure; even/odd imbalance;
  aliasing through `Poly::operator*=`; and integer rounding near 2^52.
- `fft_fuzz.test.cpp`: 1,200 independent integer/real reference cases, including
  highly unequal lengths and reciprocal power-of-two scaling, plus 200 modular
  reference cases across prime and composite dynamic moduli.
- `fft_allocation.test.cpp`: injected allocation failures, resumed cache growth,
  and concurrent transforms with independent per-thread caches.
- `validate.cpp`: 12,115,404 output coefficients compared against exact NTT/CRT,
  covering six sizes up to 524,289 and four input families for integer, real,
  and modular products. `validation.txt` records the results.
- All math unit drivers, all four convolution drivers, and all formal-series
  drivers are run with `oj-verify` against **locally generated official Library
  Checker cases**, including FFT-killer cases. These are not remote judge
  submissions.
- All 23 selected drivers passed, totaling 594 official case runs.
  `verification.txt` records the per-driver counts.
- GCC AddressSanitizer/UndefinedBehaviorSanitizer, float-cast-overflow checks, and
  Clang builds cover the FFT, fuzz, and allocation-failure regressions. A native
  scalar build with floating-point contraction enabled also passes the FFT
  regression driver. Leak detection is disabled because the environment's
  ptrace setup prevents LeakSanitizer from running; ASan/UBSan remain enabled.

## Reproduction

From the repository root (GCC, Python 3, and Boost headers required):

```sh
python3 experiments/fft/run.py --suite transform --variants baseline production
python3 experiments/fft/run.py --suite convolution --variants baseline production
python3 experiments/fft/run.py --suite backends
python3 experiments/fft/run.py --suite transform --native
python3 experiments/fft/run.py --suite convolution --native
python3 experiments/fft/run.py --suite backends --native
python3 experiments/fft/run.py --suite split
python3 experiments/fft/run.py --suite accuracy
python3 experiments/fft/run.py --suite validate
```

Omit `--variants` for the full candidate matrix. Each command pins itself to CPU
0; use `--cpu` to select another allowed CPU. Run suites sequentially on an idle
machine. The runner builds in a temporary directory and saves CSV/metadata here.
Re-running a suite overwrites that suite's current results.

```sh
oj-verify run --timeout 900 -j 2 \
  verify/unit_test/math/*.test.cpp \
  verify/yosupo/convolution/*.test.cpp \
  verify/yosupo/fps/*.test.cpp
```
