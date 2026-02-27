#include <CLSTL/algorithm.h>
#include <CLSTL/vector.h>
#include <gtest/gtest.h>
#include <random>

TEST(Algorithm, Swap) {
  clstl::vector<int> ints = {5, 4, 3, 2, 1};
  clstl::__swap(ints.begin(), ints.begin()[1]);

  ASSERT_EQ(ints[0], 4);
  ASSERT_EQ(ints[1], 5);
  ASSERT_EQ(ints[2], 3);
  ASSERT_EQ(ints[3], 2);
  ASSERT_EQ(ints[4], 1);
  ASSERT_EQ(ints.size(), 5);
}

TEST(Algorithm, SortBasic) {
  clstl::vector<int> ints = {5, 4, 3, 2, 1};
  std::size_t before_size = ints.size();

  clstl::sort(ints.begin(), ints.end());
  ASSERT_EQ(before_size, ints.size());

  for (std::size_t i = 0; i < ints.size() - 1; i++) {
    ASSERT_LE(ints[i], ints[i + 1]);
  }
}

TEST(Algorithm, SortRandom) {
  clstl::vector<int> data;
  data.reserve(10);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 1000);

  for (std::size_t i = 0; i < 10; i++) {
    data.push_back(dist(gen));
  }

  std::size_t before_size = data.size();
  clstl::sort(data.begin(), data.end());
  ASSERT_EQ(before_size, data.size());

  for (std::size_t i = 0; i < data.size() - 1; i++) {
    ASSERT_LE(data.at(i), data.at(i + 1));
  }
}

TEST(Algorithm, ForEachBasic) {
  clstl::vector<int> data = {0, 1, 2, 3, 4};
  clstl::vector<bool> test = {false, false, false, false, false};

  clstl::for_each(data.begin(), data.end(), [&](int i) { test[i] = true; });
  for (std::size_t i = 0; i < test.size(); i++) {
    ASSERT_TRUE(test[i]);
  }
}
