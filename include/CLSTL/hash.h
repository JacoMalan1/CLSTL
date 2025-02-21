#ifndef CLSTL_HASH_H
#define CLSTL_HASH_H

#include <cstddef>
#include <cstdint>

namespace clstl {
template <typename Key> class hash {
public:
  hash() = default;
  std::size_t operator()(Key) const noexcept = delete;
};

template <> std::size_t hash<int8_t>::operator()(int8_t x) const noexcept;
template <> std::size_t hash<uint8_t>::operator()(uint8_t x) const noexcept;
template <> std::size_t hash<int32_t>::operator()(int32_t x) const noexcept;
template <> std::size_t hash<uint32_t>::operator()(uint32_t x) const noexcept;
template <> std::size_t hash<int16_t>::operator()(int16_t x) const noexcept;
template <> std::size_t hash<uint16_t>::operator()(uint16_t x) const noexcept;
template <> std::size_t hash<int64_t>::operator()(int64_t x) const noexcept;
template <> std::size_t hash<uint64_t>::operator()(uint64_t x) const noexcept;
template <> std::size_t hash<float>::operator()(float x) const noexcept;
template <> std::size_t hash<double>::operator()(double x) const noexcept;
} // namespace clstl

#endif
