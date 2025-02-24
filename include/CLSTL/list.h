#ifndef CLSTL_LIST_H
#define CLSTL_LIST_H

#include <memory>
#include <stdexcept>

namespace clstl {
namespace __internal {
template <typename T> struct list_node {
  list_node *next;
  T value;

  list_node(list_node *next, const T &value) : next(next), value(value) {}
};
} // namespace __internal

template <typename _List> class const_list_iterator {
public:
  using value_type = typename _List::value_type;
  using reference = typename std::add_lvalue_reference<
      typename std::add_const<value_type>::type>::type;
  using pointer = typename std::add_pointer<
      typename std::add_const<value_type>::type>::type;
  using node_ptr = typename std::add_pointer<
      typename std::add_const<typename _List::node_type>::type>::type;

  const_list_iterator(node_ptr ptr) : m_Curr(ptr) {}

  bool operator==(const const_list_iterator &other) {
    return this->m_Curr == other.m_Curr;
  }
  bool operator!=(const const_list_iterator &other) {
    return !this->operator==(other);
  }

  const_list_iterator &operator++() {
    this->m_Curr = this->m_Curr->next;
    return *this;
  }

  const_list_iterator operator++(int) {
    const_list_iterator tmp = *this;

    this->m_Curr = this->m_Curr->next;
    return tmp;
  }

  const_list_iterator &operator+=(std::ptrdiff_t offset) {
    for (std::ptrdiff_t i = 0; i < offset; i++) {
      this->operator++();
    }
    return *this;
  }

  const_list_iterator operator+(std::ptrdiff_t offset) {
    const_list_iterator tmp = *this;
    for (std::ptrdiff_t i = 0; i < offset; i++) {
      tmp++;
    }
    return tmp;
  }

  reference operator*() { return this->m_Curr->value; }
  pointer operator->() { return &this->m_Curr->value; }

  node_ptr current() { return this->m_Curr; }

private:
  node_ptr m_Curr;
};

template <typename _List> class list_iterator {
public:
  using value_type = typename _List::value_type;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using pointer = typename std::add_pointer<value_type>::type;
  using node_ptr = typename std::add_pointer<typename _List::node_type>::type;

  list_iterator(node_ptr ptr) : m_Curr(ptr) {}

  bool operator==(const list_iterator &other) {
    return this->m_Curr == other.m_Curr;
  }
  bool operator!=(const list_iterator &other) {
    return !this->operator==(other);
  }

  list_iterator &operator++() {
    this->m_Curr = this->m_Curr->next;
    return *this;
  }
  list_iterator operator++(int) {
    list_iterator tmp = *this;
    this->m_Curr = this->m_Curr->next;
    return tmp;
  }

  list_iterator &operator+=(std::ptrdiff_t offset) {
    for (std::ptrdiff_t i = 0; i < offset; i++) {
      this->operator++();
    }
    return *this;
  }

  list_iterator operator+(std::ptrdiff_t offset) {
    list_iterator tmp = *this;
    for (std::ptrdiff_t i = 0; i < offset; i++) {
      tmp++;
    }
    return tmp;
  }

  reference operator*() { return this->m_Curr->value; }
  pointer operator->() { return &this->m_Curr->value; }

  node_ptr current() { return this->m_Curr; }

private:
  node_ptr m_Curr;
};

template <typename T,
          typename Allocator = std::allocator<__internal::list_node<T>>>
class list {
public:
  using value_type = T;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using const_reference = typename std::add_lvalue_reference<
      typename std::add_const<value_type>::type>::type;
  using iterator = list_iterator<list<T, Allocator>>;
  using const_iterator = const_list_iterator<list<T, Allocator>>;
  using node_type = __internal::list_node<T>;

  list() : m_Length(0), m_Head(nullptr) {}

  list(const list &other)
      : m_Length(other.m_Length), m_Head(nullptr), m_Alloc(other.m_Alloc) {
    if (!other.m_Head) {
      return;
    }

    auto *other_curr = other.m_Head;
    auto *curr = this->m_Alloc.allocate(1);
    this->m_Head = curr;
    for (std::size_t i = 0; i < this->m_Length; i++) {
      new (curr) __internal::list_node<T>(nullptr, other_curr->value);
      other_curr = other_curr->next;
      if (other_curr) {
        curr->next = this->m_Alloc.allocate(1);
      }
      curr = curr->next;
    }
  }

  list &operator=(const list &other) {
    if (this == &other) {
      return *this;
    }

    if (this->m_Head) {
      auto *curr = this->m_Head;
      while (curr) {
        auto *next = curr->next;
        std::destroy_at(curr);
        this->m_Alloc.deallocate(curr, 1);
        curr = next;
      }
    }

    this->m_Length = other.m_Length;
    this->m_Head = nullptr;
    this->m_Alloc = other.m_Alloc;

    if (!other.m_Head) {
      return *this;
    }

    auto *other_curr = other.m_Head;
    auto *curr = this->m_Alloc.allocate(1);
    this->m_Head = curr;
    for (std::size_t i = 0; i < this->m_Length; i++) {
      new (curr) __internal::list_node<T>(nullptr, other_curr->value);
      other_curr = other_curr->next;
      if (other_curr) {
        curr->next = this->m_Alloc.allocate(1);
      }
      curr = curr->next;
    }

    return *this;
  }

  list(list &&other)
      : m_Length(other.m_Length), m_Head(other.m_Head), m_Alloc(other.m_Alloc) {
  }

  ~list() {
    if (!this->m_Head) {
      return;
    }

    auto *curr = this->m_Head;
    while (curr) {
      auto *next = curr->next;
      std::destroy_at(curr);
      this->m_Alloc.deallocate(curr, 1);
      curr = next;
    }
  }

  bool empty() const { return this->m_Head == nullptr; }

  std::size_t size() const { return this->m_Length; }
  void push_back(const T &elem) {
    this->m_Length++;
    if (this->m_Head == nullptr) {
      this->m_Head = this->m_Alloc.allocate(1);
      new (this->m_Head) __internal::list_node<T>(nullptr, elem);
      return;
    }

    auto *curr = this->m_Head;
    while (curr->next != nullptr) {
      curr = curr->next;
    }

    curr->next = this->m_Alloc.allocate(1);
    new (curr->next) __internal::list_node<T>(nullptr, elem);
  }

  void push_front(const T &elem) {
    auto *new_node = this->m_Alloc.allocate(1);
    new (new_node) __internal::list_node<T>(this->m_Head, elem);
    this->m_Head = new_node;
    this->m_Length++;
  }

  template <typename... Args> void emplace_front(Args &&...args) {
    auto *new_node = this->m_Alloc.allocate(1);
    new (new_node)
        __internal::list_node<T>(this->m_Head, T(std::forward<Args>(args)...));
    this->m_Head = new_node;
    this->m_Length++;
  }

  reference front() { return this->m_Head->value; }
  const_reference front() const { return this->m_Head->value; }

  void pop_front() {
    auto *head = this->m_Head;
    this->m_Head = head->next;
    std::destroy_at(head);
    this->m_Alloc.deallocate(head, 1);
  }

  void pop_back() {
    auto *curr = this->m_Head;
    if (!curr->next) {
      std::destroy_at(curr);
      this->m_Alloc.deallocate(curr, 1);
      this->m_Head = nullptr;
      this->m_Length--;
      return;
    }

    while (curr->next->next) {
      curr = curr->next;
    }

    std::destroy_at(curr->next);
    this->m_Alloc.deallocate(curr->next, 1);
    curr->next = nullptr;
    this->m_Length--;
  }

  template <typename... Args> void emplace_back(Args &&...args) {
    this->m_Length++;
    if (this->m_Head == nullptr) {
      this->m_Head = this->m_Alloc.allocate(1);
      new (this->m_Head)
          __internal::list_node<T>(nullptr, T(std::forward<Args>(args)...));
      return;
    }

    auto *curr = this->m_Head;
    while (curr->next != nullptr) {
      curr = curr->next;
    }

    curr->next = this->m_Alloc.allocate(1);
    new (curr->next)
        __internal::list_node<T>(nullptr, T(std::forward<Args>(args)...));
  }

  reference at(std::size_t index) {
    if (index >= this->m_Length) {
      throw std::out_of_range("Attempted to index list out of range");
    }

    auto *curr = this->m_Head;
    for (std::size_t i = 0; i < index; i++) {
      curr = curr->next;
    }

    return curr->value;
  }

  const_reference at(std::size_t index) const {
    if (index >= this->m_Length) {
      throw std::out_of_range("Attempted to index list out of range");
    }

    auto *curr = this->m_Head;
    for (std::size_t i = 0; i < index; i++) {
      curr = curr->next;
    }

    return curr->value;
  }

  reference operator[](std::size_t index) {
    auto *curr = this->m_Head;
    for (std::size_t i = 0; i < index; i++) {
      curr = curr->next;
    }

    return curr->value;
  }

  const_reference operator[](std::size_t index) const {
    auto *curr = this->m_Head;
    for (std::size_t i = 0; i < index; i++) {
      curr = curr->next;
    }

    return curr->value;
  }

  iterator begin() { return iterator(this->m_Head); }
  iterator end() { return iterator(nullptr); }

  const_iterator begin() const { return const_iterator(this->m_Head); }
  const_iterator end() const { return const_iterator(nullptr); }

  void erase(iterator pos) {
    if (pos.current() == this->m_Head) {
      this->pop_front();
      this->m_Length--;
      return;
    }

    __internal::list_node<T> *prev = nullptr;
    auto *curr = this->m_Head;
    while (curr) {
      if (curr == pos.current()) {
        break;
      }

      prev = curr;
      curr = curr->next;
    }

    prev->next = curr->next;
    this->m_Length--;
    std::destroy_at(curr);
    this->m_Alloc.deallocate(curr, 1);
  }

  void erase(iterator first, iterator last) {
    __internal::list_node<T> *prev = nullptr;
    auto *curr = this->m_Head;
    __internal::list_node<T> *first_node_prev = nullptr;
    __internal::list_node<T> *last_node = nullptr;
    __internal::list_node<T> *last_node_prev = nullptr;
    while (curr) {
      if (curr == first.current()) {
        first_node_prev = prev;
      }

      if (curr == last.current()) {
        last_node_prev = prev;
        last_node = curr;
        break;
      }

      prev = curr;
      curr = curr->next;
    }

    if (last_node) {
      last_node_prev->next = nullptr;
    } else {
      last_node_prev = prev;
      last_node_prev->next = nullptr;
    }

    if (!first_node_prev) {
      curr = this->m_Head;
      this->m_Head = last_node;
    } else {
      curr = first_node_prev->next;
      first_node_prev->next = last_node;
    }

    while (curr) {
      auto *next = curr->next;
      std::destroy_at(curr);
      this->m_Alloc.deallocate(curr, 1);
      curr = next;
      this->m_Length--;
    }
  }

private:
  std::size_t m_Length;
  __internal::list_node<T> *m_Head;
  Allocator m_Alloc;
};
} // namespace clstl

#endif
