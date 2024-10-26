---
title: Multiset Hash
documentation_of: //library/hashing/multiset_hash.hpp
---

```c++
template <
    ModInt modint = static_modint<(1ll << 61) - 1>,
    size_t num_bases = 1,
    size_t CACHE = 1'000'000
> struct MultisetHash;
```

### Template parameters

- `modint` - the type of `ModInt` to be used for hashing.
- `num_bases` - the number of bases to use.
- `CACHE` - the number of powers to precompute.

### Member types

- `mint` - same as `modint`.

### Member functions

- `MultisetHash insert(long long a, long long cnt = 1) const;`  
	Returns copy of `MultisetHash` after inserting `cnt` copies of `a` into multiset.  
	**Performance**: $O(\texttt{num\_bases})$ if $\texttt{a} < \texttt{CACHE}$, otherwise, $O(\texttt{num\_bases}\cdot \log_2 \texttt{a})$.
	
- `MultisetHash erase(long long a, long long cnt = 1) const;`  
	Returns copy of `MultisetHash` after erasing `cnt` copies of `a` into multiset.  
	**Performance**: $O(\texttt{num\_bases})$ if $\texttt{a} < \texttt{CACHE}$, otherwise, $O(\texttt{num\_bases}\cdot \log_2 \texttt{a})$.
	
- `MultisetHash offset(long long delta) const;`  
	Returns copy of `MultisetHash` after adding `delta` to all elements in the multiset.  
	**Performance**: $O(\texttt{num\_bases})$ if $\texttt{delta} < \texttt{CACHE}$, otherwise, $O(\texttt{num\_bases}\cdot \log_2 \texttt{delta})$.
	
- `array<typename mint::umod_type, num_bases> get_v() const`  
	Returns an array containing the hash of the multiset under each base.  
	**Performance**: $O(\texttt{num\_bases})$.
	
- `operator+`, `operator+=`, `operator-`, `operator-=`  
	Add / subtract two multisets.  
	**Performance**: $O(\texttt{num\_bases})$.
	
- `operator==`, `operator!=`  
	Compare two multisets.  
	**Performance**: $O(\texttt{num\_bases})$.
