#ifndef CLSTL_UNORDERED_MAP_H
#define CLSTL_UNORDERED_MAP_H

#define __CLSTL_HASHMAP_BUCKET_SIZE 5

#include <CLSTL/hash.h>
#include <CLSTL/optional.h>
#include <memory>
#include <stdexcept>

namespace clstl {
template <typename F, typename S> struct pair {
  F first;
  S second;

  pair(F first, S second) : first(first), second(second) {}
};

template <typename F, typename S> pair<F, S> make_pair(F first, S second) {
  return pair(first, second);
}

template <typename K, typename V,
          typename Allocator = std::allocator<optional<pair<K, V>>>>
class unordered_map {
public:
  using key_type = K;
  using value_type = pair<K, V>;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using const_reference = typename std::add_const<reference>::type;

  unordered_map() : m_Storage(nullptr), m_Alloc(), m_Size(0), m_Buckets(0) {}
  unordered_map(const unordered_map &other)
      : m_Alloc(other.m_Alloc), m_Size(other.m_Size),
        m_Buckets(other.m_Buckets) { /** TODO: Implement copy-ctor */ }
  unordered_map &operator=(const unordered_map &other) {
    this->m_Alloc = other.m_Alloc;
    this->m_Size = other.m_Size;
    this->m_Buckets = other.m_Buckets;
    /** TODO: Implement assignment */
    return *this;
  }

  ~unordered_map() {
    if (this->m_Storage) {
      this->m_Alloc.deallocate(this->m_Storage,
                               this->m_Buckets * __CLSTL_HASHMAP_BUCKET_SIZE);
    }
  }

  void insert(const value_type &value) {
    if (this->m_Buckets == 0) {
      this->grow(4);
    } else if (this->m_Size == this->m_Buckets * __CLSTL_HASHMAP_BUCKET_SIZE) {
      this->grow(this->m_Buckets * 2);
    }

    std::size_t idx = this->find_index_for(value.first);
    this->m_Size++;
    this->m_Storage[idx] = value;
  }

  reference operator[](const K &key) {
    try {
      std::size_t idx = this->find_index_for(key, false);
      return this->m_Storage[idx].value();
    } catch (std::out_of_range &) {
      throw std::out_of_range("Attempted to index unordered_map out of range.");
    }
  }

  const_reference operator[](const K &key) const {
    try {
      std::size_t idx = this->find_index_for(key, false);
      return this->m_Storage[idx].value();
    } catch (std::out_of_range &) {
      throw std::out_of_range("Attempted to index unordered_map out of range.");
    }
  }

  float load_factor() const {
    if (!this->m_Storage) {
      return 0.0f;
    }

    float count = 0.0f;
    for (std::size_t bucket = 0; bucket < this->m_Buckets; bucket++) {
      for (std::size_t i = 0; i < __CLSTL_HASHMAP_BUCKET_SIZE; i++) {
        if (this->m_Storage[bucket * __CLSTL_HASHMAP_BUCKET_SIZE + i]
                .has_value()) {
          count += 1.0f;
        }
      }
    }

    return count / static_cast<float>(this->m_Buckets);
  }

  float max_load_factor() const {
    return static_cast<float>(__CLSTL_HASHMAP_BUCKET_SIZE);
  }

  reference at(const K &key) { return this->operator[](key); }
  const_reference at(const K &key) const { return this->operator[](key); }

private:
  void grow(std::size_t new_buckets) {
    optional<pair<K, V>> *new_storage =
        this->m_Alloc.allocate(new_buckets * __CLSTL_HASHMAP_BUCKET_SIZE);
    optional<pair<K, V>> *old_storage = this->m_Storage;
    std::size_t old_buckets = this->m_Buckets;

    this->m_Buckets = new_buckets;
    this->m_Storage = new_storage;

    if (this->m_Storage) {
      for (std::size_t bucket = 0; bucket < old_buckets; bucket++) {
        for (std::size_t idx = 0; idx < __CLSTL_HASHMAP_BUCKET_SIZE; idx++) {
          if (old_storage[bucket * __CLSTL_HASHMAP_BUCKET_SIZE + idx]
                  .has_value()) {
            this->insert(old_storage[bucket * __CLSTL_HASHMAP_BUCKET_SIZE + idx]
                             .value());
          }
        }
      }
    }

    this->m_Alloc.deallocate(old_storage,
                             old_buckets * __CLSTL_HASHMAP_BUCKET_SIZE);
  }

  std::size_t find_index_for(const K &key, bool grow = true) {
    hash<key_type> hasher{};
    std::size_t starting_hash = hasher(key) % this->m_Buckets;

    bool found = false;
    std::size_t hash = starting_hash;
    std::size_t idx;
    while (!found) {
      for (std::size_t i = 0; i < __CLSTL_HASHMAP_BUCKET_SIZE; i++) {
        if (!this->m_Storage[hash * __CLSTL_HASHMAP_BUCKET_SIZE + i]
                 .has_value()) {
          idx = i;
          found = true;
        } else if (this->m_Storage[hash * __CLSTL_HASHMAP_BUCKET_SIZE + i]
                       .value()
                       .first == key) {
          idx = i;
          found = true;
        }
      }

      if (found) {
        break;
      }

      hash = (hash + 1) % this->m_Buckets;
      if (hash == starting_hash) {
        if (grow) {
          this->grow(this->m_Buckets * 2);
        } else {
          break;
        }
      }
    }

    if (!found) {
      throw std::out_of_range("No value could be found for the given key");
    }

    return hash * this->m_Buckets + idx;
  }

  std::size_t find_index_for(const K &key,
                             [[maybe_unused]] bool grow = true) const {
    hash<key_type> hasher{};
    std::size_t starting_hash = hasher(key) % this->m_Buckets;

    bool found = false;
    std::size_t hash = starting_hash;
    std::size_t idx;
    while (!found) {
      for (std::size_t i = 0; i < __CLSTL_HASHMAP_BUCKET_SIZE; i++) {
        if (!this->m_Storage[hash * __CLSTL_HASHMAP_BUCKET_SIZE + i]
                 ->has_value()) {
          idx = i;
          found = true;
        } else if (this->m_Storage[hash * __CLSTL_HASHMAP_BUCKET_SIZE + i]
                       .value()
                       .first == key) {
          idx = i;
          found = true;
        }
      }

      if (found) {
        break;
      }

      hash = (hash + 1) % this->m_Buckets;
      if (hash == starting_hash) {
        break;
      }
    }

    if (!found) {
      throw std::out_of_range("No value could be found for the given key");
    }

    return hash * this->m_Buckets + idx;
  }

  optional<pair<K, V>> *m_Storage;
  std::size_t m_Size;
  std::size_t m_Buckets;
  Allocator m_Alloc;
};
} // namespace clstl

#endif
