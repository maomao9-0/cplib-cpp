---
title: Hashing Custom Hash
documentation_of: //library/hashing/hashing_custom_hash.hpp
---

```c++
template <
    typename T
> concept Hashing;
```

The concept `Hashing<T>` is satisfied if `T` has member type `mint` satisfying `Modint<mint>`, and it has a function `get_v` returning an `Iterable` with member type `value_type` equals to `mint::umod_type`

```c++
template <
    Hashing T
> struct HashingCustomHash;
```

Constructs a hash object for type `T` that can be used as template parameter for `unordered_map`, `unordered_set`, `maomao90::hashmap`, etc.

**Example**

```c++
MultisetHash<> multiset_hash;
unordered_set<MultisetHash, HashingCustomHash<MultisetHash<>>> st;
st.insert(multiset_hash);
```
