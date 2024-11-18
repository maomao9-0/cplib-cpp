---
title: ModInt
documentation_of: //library/math/modint.hpp
---

```c++
template <
  typename T
> concept ModInt;
```

The concept `ModInt<T>` is satisfied if `T` has static functions `imod()` and `umod()` which returns the modulo with member type `mod_type` and `umod_type` respectively, has functions `pow(long long p)` and `inv()`, and has arithmetic and equality operators (+, -, *, /, ==, !=).

```c++
template <
  auto mod = 998244353
> struct static_modint;	
```

### Template parameters

- `mod` - the value of the modulo.

**Template requirements**

- `mod` has to be a positive integer that is of type `signed_integral` with 64-bits or less.

### Member types

- `mod_type` - the type of `mod`.
- `umod_type` - unsigned type of `mod`.

### Constructor

- `static_modint();`	 
  Initialise modint with value $0$.
  
- `static_modint(T v);`  
  Initialise modint with value `v`.

### Member functions

- `static static_modint raw(mod_type v);`  
  Constant time optimisation to create `static_modint` if $0\le \texttt{v} < \texttt{mod}$.

- `umod_type val() const;`  
  Return value stored in `static_modint`.

- `static_modint pow(long long p) const;`  
  Returns `static_modint` of the current value raised to the `p`-th power.  
	**Performance**: $O(\log_2 \texttt{p})$

- `static_modint inv() const;`  
  Returns `static_modint` of the modular inverse of the current value.  
  Note that the value stored in `static_modint` has to be co-prime to `mod`. Otherwise, an assertion failure will occur.  
	**Performance**: $O(\log_2 \texttt{mod})$

- `operator+`, `operator+=`, `operator-`, `operator-=`, `operator*`, `operator*=`  
	Add / subtract / multiply two modints.  
	**Performance**: $O(1)$

- `operator++`, `operator--`  
	Prefix and postfix increment / decrement modint.  
	**Performance**: $O(1)$

- `operator/`, `operator/=`  
	Divide two modints.  
	**Performance**: $O(\log_2 \texttt{mod})$

- `operator+`, `operator-`  
	Unary operators.  
	**Performance**: $O(1)$
	
- `operator==`, `operator!=`  
	Compare two modints.  
	**Performance**: $O(1)$
	
- `operator<<`, `operator>>`  
	Serializes and deserializes a modint.  
	**Performance**: $O(1)$
