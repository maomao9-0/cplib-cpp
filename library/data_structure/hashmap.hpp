#pragma once

#include <bitset>
#include <vector>

#include "library/internal/hashing.hpp"

namespace maomao90 {
using namespace std;

/**
 * A hash table.
 *
 * Collisions are resolved using linear probing.
 *
 * Time complexity: \f$O(1)\f$.
 *
 * Space complexity: \f$O(n)\f$ with a large constant, where \f$n\f$ is the
 * number of insertions into the hashmap.
 *
 * @tparam K the type of the key.
 * @tparam T the type of the value.
 * @tparam Hash the type of the hash object, which overloads the function call
 *     operator `size_t ::(const T&)` and returns the hash of the key.
 * @tparam KEEP_HISTORY should be `true` if iteration over key-value pairs is
 *     needed.
 * @tparam LG the number of buckets stored is \f$2^{LG}\f$. It should be at
 *     least twice the number of insertions into the hashmap.
 */
template <typename K, typename T,
          typename Hash = internal::hashing::HashObject<K>,
          bool KEEP_HISTORY = false, int LG = 20>
struct HashMap {
  /**
   * Accesses the value associated with the given key `k`.
   *
   * If the key does not exist in the map, a new entry is created with the
   * default-constructed value `T()`, and a reference to that value is returned.
   *
   * This function can be used for lookup, insertion or modification of values,
   * since the reference is being returned.
   *
   * @param k the key whose associated value is to be accessed.
   * @returns a reference to the value associated with `k`.
   */
  constexpr T &operator[](const K &k) {
    int index = get_index(k);
    if (!vis[index]) {
      vis[index] = 1;
      key[index] = k;
      value[index] = T();
      if constexpr (KEEP_HISTORY) {
        history.push_back(index);
      }
    }
    return value[index];
  }

  /**
   * Removes the element with the specified key `k` from the hashmap.
   *
   * If the key does not exist, the hashmap remains unchanged and the function
   * returns `false`.
   *
   * @param k the key of the element to erase.
   * @returns `true` if an element associated with key `k` was found and
   *     removed, `false` otherwise.
   */
  constexpr bool erase(const K &k) {
    int index = get_index(k);
    if (!vis[index]) {
      return 0;
    }
    vis[index] = 0;
    return 1;
  }

  /**
   * Checks whether the hashmap contains an element with the specified key `k`.
   *
   * @param k the key to check for.
   * @returns `true` if an element with key `k` exists, `false` otherwise.
   */
  constexpr bool contains(const K &k) const {
    int index = get_index(k);
    return vis[index];
  }

  /**
   * Applies the given function `f` to each key-value pair in the hashmap.
   *
   * The function `f` must be callable with arguments `(const K&, T&)`. Each
   * key–value pair is passed to `f` exactly once in an unspecified order.
   *
   * Time complexity: \f$O(n)\f$ if `KEEP_HISTORY` is `true`, where \f$n\f$ is
   * the number of insertions; otherwise \f$O(2^{LG})\f$.
   *
   * @tparam F the callable type accepts `(const K&, T&)`.
   * @param f the function to apply to each key-value pair.
   */
  template <typename F>
    requires requires(F f, const K &k, T &v) { f(k, v); }
  constexpr void for_each(F f) {
    if constexpr (KEEP_HISTORY) {
      for (int i : history) {
        if (vis[i]) {
          f(key[i], value[i]);
        }
      }
    } else {
      for (int i = 0; i < MOD; i++) {
        if (vis[i]) {
          f(key[i], value[i]);
        }
      }
    }
  }

  /**
   * Removes all elements from the hashmap.
   *
   * Time complexity: \f$O(n)\f$ if `KEEP_HISTORY` is `true`, where \f$n\f$ is
   * the number of insertions; otherwise \f$O\left(\frac{2^{LG}}{64}\right)\f$.
   */
  constexpr void clear() {
    if constexpr (KEEP_HISTORY) {
      for (int i : history) {
        vis[i] = 0;
      }
      history.clear();
    } else {
      vis.reset();
    }
  }

private:
  static constexpr int MOD = 1 << LG;
  bitset<MOD> vis;
  K key[MOD];
  T value[MOD];
  vector<int> history;

  constexpr int get_index(const K &k) const {
    unsigned long long hash = Hash()(k);
    int index = hash >> (64 - LG);
    while (vis[index] && key[index] != k) {
      index = (index + 1) & (MOD - 1);
    }
    return index;
  }
};
} // namespace maomao90
