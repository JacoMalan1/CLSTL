#include <CLSTL/hash.h>
#include <cstring>

#define IMPL_INT_HASH(t)                                                       \
  template <> std::size_t hash<t>::operator()(t x) const noexcept { return x; }

namespace clstl {
IMPL_INT_HASH(uint8_t)
IMPL_INT_HASH(int8_t)
IMPL_INT_HASH(int32_t)
IMPL_INT_HASH(uint32_t)
IMPL_INT_HASH(int16_t)
IMPL_INT_HASH(uint16_t)
IMPL_INT_HASH(int64_t)
IMPL_INT_HASH(uint64_t)

template <> std::size_t hash<float>::operator()(float x) const noexcept {
  uint32_t y;
  std::memcpy(&y, &x, sizeof(float));
  return static_cast<std::size_t>(y);
}

template <> std::size_t hash<double>::operator()(double x) const noexcept {
  uint64_t y;
  std::memcpy(&y, &x, sizeof(double));
  return static_cast<std::size_t>(y);
}
} // namespace clstl
