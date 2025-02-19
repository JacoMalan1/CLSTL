#include <CLSTL/array.h>
#include <gtest/gtest.h>

TEST(Array, TestInit) { [[maybe_unused]] clstl::array<int, 3> arr = {1, 2, 3}; }
