#ifndef CLSTL_UNIQUE_PTR_H
#define CLSTL_UNIQUE_PTR_H

#include <functional>
#include <memory>

namespace clstl {
template <typename T, typename Allocator>
void __unique_ptr_deleter(Allocator *alloc, T *ptr) {
  std::destroy_at(ptr);
  alloc->deallocate(ptr, 1);
}

template <typename T, typename Allocator = std::allocator<T>> class unique_ptr {
public:
  typedef std::function<void(T *)> Deleter;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;

  unique_ptr(T *ptr)
      : m_Ptr(ptr), m_Alloc(), m_Deleter([&](T *ptr) {
          __unique_ptr_deleter<T, Allocator>(&this->m_Alloc, ptr);
        }) {}
  unique_ptr(const unique_ptr<T> &) = delete;
  unique_ptr &operator=(const unique_ptr<T> &) = delete;
  ~unique_ptr() {
    if (this->m_Ptr) {
      std::destroy_at(this->m_Ptr);
      this->m_Alloc.deallocate(this->m_Ptr, 1);
    }
  }

  Deleter &get_deleter() { return this->m_Deleter; }
  const Deleter &get_deleter() const noexcept { return this->m_Deleter; }

  pointer get() { return this->m_Ptr; }
  const_pointer get() const { return this->m_Ptr; }

  reference operator*() { return *this->m_Ptr; }
  const_reference operator*() const { return *this->m_Ptr; }

  pointer operator->() { return this->m_Ptr; }
  const_pointer operator->() const { return this->m_Ptr; }

  operator bool() const { return this->get(); }

  [[nodiscard]] pointer release() {
    T *temp = this->m_Ptr;
    this->m_Ptr = nullptr;
    return temp;
  }

private:
  T *m_Ptr;
  Allocator m_Alloc;
  Deleter m_Deleter;
};

template <typename T, typename Allocator = std::allocator<T>, typename... Args>
unique_ptr<T, Allocator> make_unique(Args &&...args) {
  Allocator alloc;
  T *ptr = alloc.allocate(1);
#if __cplusplus < 202002L
  alloc.construct(ptr, args...);
#else
  std::construct_at(ptr, args...);
#endif
  return unique_ptr<T, Allocator>(ptr);
}
} // namespace clstl

#endif
