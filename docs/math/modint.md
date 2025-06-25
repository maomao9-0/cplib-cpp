---
title: ModInt
documentation_of: //library/math/modint.hpp
---

```c++
template <
  typename T
> concept ModInt;
template <
  typename T
> concept StaticModInt;
```

The concept `ModInt<T>` is satisfied if `T` is either `static_modint` or `dynamic_modint`.
`StaticModInt<T>` is only satisfied if `T` is `static_modint`.

## Static Modulo
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
  
- `static_modint(T a);`  
  Initialise modint with value `a`.

### Member functions

- `static static_modint raw(mod_type a);`  
  Constant time optimisation to create `static_modint` if $0\le \texttt{a} < \texttt{mod}$.

- `static constexpr mod_type imod();`  
  Returns the signed value of `mod`.
  
- `static constexpr umod_type umod();`  
  Returns the unsigned value of `mod`.

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

## Dynamic Modulo

```c++
template <
  int id = -1
> struct dynamic_modint;	
```

### Template parameters

- `id` - arbitrary id. Set different id for different mods.

### Constructor

- `dynamic_modint();`	 
  Initialise modint with value $0$.
  
- `dynamic_modint(T a);`  
  Initialise modint with value `a`.

### Member functions

- `static void set_mod(int mod);`  
  Set mod to use.

- `static constexpr int imod();`  
  Returns the signed value of `mod`.
  
- `static constexpr unsigned int umod();`  
  Returns the unsigned value of `mod`.

- `static dynamic_modint raw(int a);`  
  Constant time optimisation to create `dynamic_modint` if $0\le \texttt{a} < \texttt{mod}$.

- `unsigned int val() const;`  
  Return value stored in `dynamic_modint`.

- `dynamic_modint pow(long long p) const;`  
  Returns `dynamic_modint` of the current value raised to the `p`-th power.  
	**Performance**: $O(\log_2 \texttt{p})$

- `dynamic_modint inv() const;`  
  Returns `dynamic_modint` of the modular inverse of the current value.  
  Note that the value stored in `dynamic_modint` has to be co-prime to `mod`. Otherwise, an assertion failure will occur.  
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
