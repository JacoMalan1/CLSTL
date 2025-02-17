#ifndef CLSTL_SHARED_PTR_H
#define CLSTL_SHARED_PTR_H

#include <atomic>
#include <limits>
#include <memory>
#include <stdexcept>

namespace clstl {
template <typename T, typename Allocator> struct _inner {
  T data;
  std::atomic_size_t strong_count;
  std::atomic_size_t weak_count;
  Allocator alloc;
};

template <typename T, typename Allocator = std::allocator<T>> class shared_ptr {
public:
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;

  shared_ptr(const shared_ptr<T, Allocator> &other) : m_Inner(other.m_Inner) {
    if (this->m_Inner->strong_count.fetch_add(1, std::memory_order_acquire) ==
        std::numeric_limits<std::size_t>::max()) {
      throw std::runtime_error(
          "Reached maximum value for size_t in shared_ptr");
    }
  }

  shared_ptr &operator=(const shared_ptr<T, Allocator> &other) {
    this->m_Inner = other.m_Inner;
    if (this->m_Inner->strong_count.fetch_add(1, std::memory_order_acquire) ==
        std::numeric_limits<std::size_t>::max()) {
      throw std::runtime_error(
          "Reached maximum value for size_t in shared_ptr");
    }
  }

  ~shared_ptr() {
    if (this->m_Inner->strong_count.fetch_sub(1, std::memory_order_release) ==
        1) {
      typename Allocator::template rebind<_inner<T, Allocator>>::other alloc =
          this->m_Inner->alloc;
      std::destroy_at(this->m_Inner);
      alloc.deallocate(this->m_Inner, 1);
    }
  }

  pointer operator->() { return &this->m_Inner->data; }
  const_pointer operator->() const { return &this->m_Inner->data; }

  reference operator*() { return this->m_Inner->data; }
  const_reference operator*() const { return this->m_Inner->data; }

  pointer get() { return &this->m_Inner->data; }
  const_pointer get() const { return &this->m_Inner->data; }

  operator bool() const { return this->get(); }

  template <typename U, typename _Alloc, typename... Args>
  friend shared_ptr<U, _Alloc> make_shared(Args &&...args);

  std::size_t use_count() const noexcept {
    if (!this->m_Inner) {
      return 0;
    }

    return this->m_Inner->strong_count.load(std::memory_order_relaxed);
  }

private:
  shared_ptr(_inner<T, Allocator> *inner) : m_Inner(inner) {}

  _inner<T, Allocator> *m_Inner;
};

template <typename T, typename Allocator = std::allocator<T>, typename... Args>
shared_ptr<T, Allocator> make_shared(Args &&...args) {
  typename Allocator::template rebind<_inner<T, Allocator>>::other alloc;
  _inner<T, Allocator> *inner = alloc.allocate(1);
  inner->strong_count.store(1, std::memory_order_acquire);
  inner->weak_count.store(0, std::memory_order_acquire);

#if __cplusplus < 202002L
  alloc.construct(&inner->data, args...);
#else
  std::construct_at(&inner->data, args...);
#endif

  inner->alloc = std::move(alloc);

  return shared_ptr(inner);
}
} // namespace clstl

#endif
