---
title: Hash Map
documentation_of: //library/data_structure/hashmap.hpp
---

```c++
template <
    typename K,
    typename T,
    typename Hash = internal::hashing::HashObject<K>,
    bool KEEP_HISTORY = false,
    int LG = 20
> struct HashMap;
```

### Template parameters

- `K` - the type of the keys.
- `T` - the type of the values.
- `Hash` - 	a class with overloaded `()` operator accepting parameter with type `K` and returning the `size_t` hash of the parameter.
- `KEEP_HISTORY` - if `true`, history of insertions is maintained in a vector to speed up `for_each` and `clear` at the cost of additional memory.
- `LG` - the number of buckets is $2^\text{LG}$.

**Template requirements**

- $2^\text{LG}$ should be larger than the number of insertions made into the hashmap. Otherwise, it might result in an infinite loop.

### Member functions

- `T& operator[](const K &k);`
	Access, modify or insert key `k`.
	**Performance**: $O(1)$.
	
- `bool erase(const K &k);`
	Erase key `k`. Returns `false` if key `k` is not found and `true` otherwise.
	**Perfomance**: $O(1)$.
	
- `bool contains(const K &k) const;` 
	Returns whether key `k` has been accessed by operator `[]` before.
	**Performance**: $O(1)$.
	
- `void for_each(F f);`
  Calls function `f(key, value)` on each element in the hashmap.
	**Performance**: $O(\text{num\_insert})$ if `KEEP_HISTORY`, otherwise, $O\left(\frac{2^\text{LG}}{\omega}\right)$.
	
- `void clear();`
	Clear the hashmap. 
	**Performance**: $O(\text{num\_insert})$ if `KEEP_HISTORY`, otherwise, $O\left(\frac{2^\text{LG}}{\omega}\right)$.
