#ifndef CLSTL_ALGORITHM_H
#define CLSTL_ALGORITHM_H

#include <functional>
#include <type_traits>

namespace clstl {
template <typename I>
typename I::difference_type __left_child(typename I::difference_type i) {
  return 2 * i + 1;
}

template <typename I> void __swap(I a, I b) {
  typename I::value_type tmp = *a;
  *a = *b;
  *b = tmp;
}

template <typename I, typename Comparator>
void sort(I begin, I end, Comparator comparator) {
  static_assert(
      std::is_convertible<Comparator,
                          std::function<bool(typename I::value_type,
                                             typename I::value_type)>>::value);
  auto start = (end - begin) / 2;
  auto endIdx = end - begin;

  while (endIdx > 1) {
    if (start > 0) {
      start = start - 1;
    } else {
      endIdx = endIdx - 1;
      __swap(begin[endIdx], begin);
    }

    auto root = start;
    while (__left_child<I>(root) < endIdx) {
      auto child = __left_child<I>(root);
      if (child + 1 < endIdx &&
          comparator(*(begin[child]), *(begin[child + 1]))) {
        child++;
      }

      if (comparator(*(begin[root]), *(begin[child]))) {
        __swap(begin[root], begin[child]);
        root = child;
      } else {
        break;
      }
    }
  }
}

template <typename I> void sort(I begin, I end) {
  clstl::sort(begin, end,
       [](typename I::value_type a, typename I::value_type b) { return a < b; });
}

template<typename I, typename Op> void for_each(I begin, I end, Op operation) {
  static_assert(std::is_convertible<Op, std::function<void(typename I::value_type)>>::value);

  I curr = begin;
  while (curr != end) {
    operation(*curr++);
  }
}
} // namespace clstl

#endif
