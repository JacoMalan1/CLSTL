#ifndef CLSTL_OPTIONAL_H
#define CLSTL_OPTIONAL_H

#include <cstddef>
#include <cstdint>
#include <exception>
#include <type_traits>
#include <utility>

namespace clstl {
class bad_optional_access : public std::exception {
public:
  bad_optional_access(const char *what);
  const char *what() const noexcept override;

private:
  const char *m_What;
};

namespace __internal {
template <typename T> union opt_storage {
  uint8_t _dummy;
  T _value;

  opt_storage() : _dummy() {}
  opt_storage(const T &value) : _value(std::move(value)) {}
  opt_storage(T &&value) : _value(std::move(value)) {}

  opt_storage &operator=(const T &value) {
    this->_value = value;
    return *this;
  }

  opt_storage &operator=(std::nullptr_t) {
    this->_dummy = 0;
    return *this;
  }

  template <typename... Args>
  opt_storage(Args &&...args) : _value(std::forward<Args>(args)...) {}
  ~opt_storage() {};
};
} // namespace __internal

template <typename T> class optional {
public:
  using value_type = T;
  using reference = typename std::add_lvalue_reference<value_type>::type;
  using const_reference = typename std::add_lvalue_reference<
      typename std::add_const<value_type>::type>::type;
  using pointer = typename std::add_pointer<value_type>::type;
  using const_pointer = typename std::add_const<pointer>::type;

  optional() : m_HasValue(false), m_Value() {}
  optional(T &&value) : m_HasValue(true), m_Value(value) {}
  optional(const T &value) : m_Value(value), m_HasValue(true) {}
  optional(const optional &other)
      : m_Value(other.m_HasValue ? other.value() : std::nullptr_t()),
        m_HasValue(other.m_HasValue) {}

  template <typename... Args>
  optional(Args &&...args)
      : m_HasValue(true), m_Value(std::forward<Args>(args)...) {}
  ~optional() = default;

  optional &operator=(const T &val) {
    this->m_HasValue = true;
    this->m_Value = val;
    return *this;
  }

  optional &operator=(const optional &other) {
    this->m_HasValue = other.m_HasValue;
    if (this->m_HasValue) {
      this->m_Value = other.value();
    } else {
      this->m_Value = std::nullptr_t();
    }
    return *this;
  }

  bool has_value() const { return this->m_HasValue; }
  reference value() {
    if (!this->m_HasValue) {
      throw bad_optional_access(
          "Attempted to call value() on optional with no value");
    }

    return this->m_Value._value;
  }

  const_reference value() const {
    if (!this->m_HasValue) {
      throw bad_optional_access(
          "Attempted to call value() on optional with no value");
    }

    return this->m_Value._value;
  }

  pointer operator->() noexcept { return &this->m_Value._value; }
  const_pointer operator->() const noexcept { return &this->m_Value._value; }

  reference operator*() noexcept { return this->m_Value._value; }
  const_reference operator*() const noexcept { return this->m_Value._value; }

  template <typename U = typename std::remove_cv<T>::type>
  value_type value_or(U &&default_value) const {
    return this->m_HasValue ? this->m_Value._value : default_value;
  }

  template <typename U = typename std::remove_cv<T>::type>
  value_type value_or(U &&default_value) {
    return this->m_HasValue ? this->m_Value._value : default_value;
  }

  operator bool() const { return this->m_HasValue; }

  void reset() {
    this->m_HasValue = false;
    this->m_Value = std::nullptr_t();
  }

  template <typename... Args> void emplace(Args &&...args) {
    this->m_HasValue = true;
    this->m_Value =
        __internal::opt_storage<value_type>(std::forward<Args>(args)...);
  }

  template <typename F> auto and_then(F &&f) {
    if (this->m_HasValue) {
      return f(this->m_Value._value);
    } else {
      return optional<value_type>();
    }
  }

  template <typename F> auto and_then(F &&f) const {
    if (this->m_HasValue) {
      return f(this->m_Value._value);
    } else {
      return optional<value_type>();
    }
  }

private:
  bool m_HasValue;
  __internal::opt_storage<value_type> m_Value;
};
} // namespace clstl

#endif
