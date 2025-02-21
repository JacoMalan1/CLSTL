#ifndef CLSTL_PAIR_H
#define CLSTL_PAIR_H

namespace clstl {
template <typename F, typename S> struct pair {
  F first;
  S second;

  pair() = delete;
  pair(const F &first, const S &second) : first(first), second(second) {}
  pair(F &&first, S &&second) : first(first), second(second) {}
};

template <typename F, typename S>
pair<F, S> make_pair(const F &first, const S &second) {
  return pair(first, second);
}

template <typename F, typename S> pair<F, S> make_pair(F &&first, S &&second) {
  return pair(first, second);
}
} // namespace clstl

#endif
