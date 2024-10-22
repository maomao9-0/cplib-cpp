---
title: Multiplicative Function
documentation_of: //library/math/multiplicative_function.hpp
---

# Prefix Sum of Multiplicative Function

Given a multiplicative function $f$, we will use two functions, `lucy_dp` and `min25_sieve`, to calculate $\sum_{i=1}^N f(i)$ in $O\left(\frac{N^{3/4}}{\log N}\right)$ time.

By the nature of the algorithm, we can also obtain the prefix sum of the multiplicative function at the distinct positive integer values of $\left\lfloor\frac{N}{k}\right\rfloor$ as a side-product.

## Lucy DP

```cpp
vector<T> lucy_dp(long long n, vector<T> sumg, G g)
```

**Parameters**

- `T` is the data type that is returned by the function $g$.
- `n` is the upper limit of the prefix sum.
- `sumg` is a vertor of size $2\lfloor\sqrt n\rfloor$.
  
  $\text{sumg}\_k = \begin{cases}\sum_{i=2}^{k+1} g(i) & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=2}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} g(i) & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}$

  In simpler terms, `sumg` contains the prefix sum (excluding $g(1)$) of the function $g$ at the positive integer values of $\left\lfloor\frac{N}{k}\right\rfloor$.
- `g` is the multiplicative function $g$. `g(p)` should return the value of $g(p)$ when evaluated at a prime integer $p$.

`lucy_dp` returns a vector of size $2\lfloor\sqrt n\rfloor$ which is a transformation of `sumg` such that $g(p)$ is included in the prefix sums only if $p$ is prime. In other words, the $k$-th value in the vector returned by `lucy_dp` is equals to

\begin{cases}\sum_{i=2}^{k+1} g(i)[\texttt{is\_prime}(i)] & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=2}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} g(i)[\texttt{is\_prime}(i)] & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}


The time complexity of `lucy_dp` is $O\left(\frac{N^{3/4}}{\log N}\right)$.

## Min25 Sieve

```cpp
vector<T> min25_sieve(long long n, vector<T> sumfp, F f)
```

**Parameters**

- `T` is the data type that is returned by the function $f$.
- `n` is the upper limit of the prefix sum.
- `sumfp` is a vertor of size $2\lfloor\sqrt n\rfloor$. 

  $\text{sumfp}_k = \begin{cases}\sum_{i=2}^{k+1} f(i)[\texttt{is\_prime}(i)] & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=2}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} f(i)[\texttt{is\_prime}(i)] & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}$

  In simpler terms, `sumfp` contains the prefix sum of the function $f$ at the positive integer values of $\left\lfloor\frac{N}{k}\right\rfloor$.
- `f` is the multiplicative function $f$. `f(p, k)` should return the value of $f(p^k)$ where $p$ is a prime. Note that this is sufficient to evaluate $f$ at any positive integer since $f$ is multiplicative.

`min25_sieve` returns a vector of size $2\sqrt n$ which is a transformation of `sumfp` to add back the contribution of $f(x)$ into the prefix sums at non-prime values of $x$ (including $f(1)$). In other words, the $k$-th value in the vector returned by `min25_sieve` is equals to

\begin{cases}\sum_{i=1}^{k+1} f(i) & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=1}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} f(i) & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}


The time complexity of `min25_sieve` is $O\left(\frac{N^{3/4}}{\log N}\right)$.

## Example

Let us try to find the prefix sum of the multiplicative function $f(p^k) = p^2+kp$.

When evaluated at prime values $p$, $f(p) = p^2 + p$. Using $g(p) = p^2 + p$ as the input for `lucy_dp` does not work as $g$ is not multiplicative ($g(2)g(3) = (2^2 + 2)(3^2 + 3) = 72 \neq g(6) = 42$.

Instead, what we can do is to use two different functions $g_1(p) = p$ and $g_2(p) = p^2$. Both $g_1$ and $g_2$ are multiplicative functions, and we can easily get their prefix sums in $O(1)$ time, so they are good candidates to use in `lucy_dp`. We can sum up their results to get the prefix sum of $(p^2 + p)[\texttt{is\_prime}(p)]$, which we can use as `sumfp` for the input of `min25_sieve`.

```c++
int s = floor(sqrt(n));
auto g1 = [&] (long long p) { return p; };
auto g2 = [&] (long long p) { return p * p; };
vector<long long> sumg1(2 * s), sumg2(2 * s);
for (int i = 1; i <= s; i++) {
	auto prefix_sum_g1 = [&] (long long x) { return x * (x + 1) / 2 - 1; };
	sumg1[i - 1] = prefix_sum_g1(i);
	sumg1[2 * s - i] = prefix_sum_g1(n / i);
	auto prefix_sum_g2 = [&] (long long x) { return x * (x + 1) * (2 * x + 1) / 6 - 1; }
	sumg2[i - 1] = prefix_sum_g2(i);
	sumg2[2 * s - i] = prefix_sum_g2(n / i);
}
vector<long long> sumgp1 = lucy_dp(n, sumg1, g1), sumgp2 = lucy_dp(n, sumg2, g2);
vector<long long> sumfp(2 * s);
for (int i = 0; i < 2 * s; i++) {
	sumfp[i] = sumgp1[i] + sumgp2[i];
}
auto f = [&] (long long p, int k) { return p * p + k * p; };
vector<long long> sumf = min25_sieve(n, sumfp, f);
cout << sumf.back() << '\n';
```
