#include <CLSTL/vector.h>
#include <cstring>
#include <gtest/gtest.h>

template class clstl::vector<int>;
template class clstl::vector_iter<clstl::vector<int>>;
template class clstl::const_vector_iter<clstl::vector<int>>;
template class clstl::vector_reverse_iter<clstl::vector<int>>;

TEST(Vector, TestEmpty) {
  clstl::vector<int> v;
  ASSERT_EQ(v.capacity(), 0);
  ASSERT_EQ(v.size(), 0);
  ASSERT_TRUE(v.empty());
}

TEST(Vector, TestOne) {
  struct Entity {
    const char *name;
    uint32_t age;

    Entity(const char *name, uint32_t age) : name(name), age(age) {}
    bool operator==(const Entity &other) const {
      return this->age == other.age && std::strcmp(this->name, other.name) == 0;
    }
  };

  clstl::vector<Entity> v;
  v.emplace_back("Hello", 32);

  const clstl::vector<Entity> &cv = v;

  ASSERT_EQ(std::strcmp(v[0].name, "Hello"), 0);
  ASSERT_EQ(v[0].age, 32);
  ASSERT_EQ(v.at(0).age, 32);
  ASSERT_ANY_THROW(v.at(1));
  ASSERT_EQ(v.front(), v[0]);
  ASSERT_EQ(v.back(), v[0]);

  ASSERT_EQ(std::strcmp(cv[0].name, "Hello"), 0);
  ASSERT_EQ(cv[0].age, 32);
  ASSERT_EQ(cv.at(0).age, 32);
  ASSERT_ANY_THROW(cv.at(1));
  ASSERT_EQ(cv.front(), cv[0]);
  ASSERT_EQ(cv.back(), cv[0]);
}

TEST(Vector, TestPop) {
  clstl::vector<int> v = {1, 2, 3};
  ASSERT_EQ(v.size(), 3);
  int i = 1;
  for (auto x : v) {
    ASSERT_EQ(x, i++);
  }

  v.pop_back();
  ASSERT_EQ(v.size(), 2);
  i = 1;
  for (auto x : v) {
    ASSERT_EQ(x, i++);
  }
}

TEST(Vector, TestGrow) {
  clstl::vector<int> v;
  ASSERT_EQ(v.capacity(), 0);

  v.push_back(1);
  ASSERT_EQ(v.capacity(), 1);
  ASSERT_EQ(v.size(), 1);

  v.push_back(2);
  ASSERT_EQ(v.capacity(), 2);
  ASSERT_EQ(v.size(), 2);

  v.push_back(3);
  ASSERT_EQ(v.capacity(), 4);
  ASSERT_EQ(v.size(), 3);
  v.push_back(4);
  ASSERT_EQ(v.capacity(), 4);
  ASSERT_EQ(v.size(), 4);

  v.push_back(5);
  ASSERT_EQ(v.capacity(), 8);
  ASSERT_EQ(v.size(), 5);
  v.push_back(6);
  ASSERT_EQ(v.capacity(), 8);
  ASSERT_EQ(v.size(), 6);
  v.push_back(7);
  ASSERT_EQ(v.capacity(), 8);
  ASSERT_EQ(v.size(), 7);
  v.push_back(8);
  ASSERT_EQ(v.capacity(), 8);
  ASSERT_EQ(v.size(), 8);
}

TEST(Vector, TestReserve) {
  clstl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.reserve(2);
  ASSERT_EQ(v.capacity(), 4);
}

TEST(Vector, TestResize) {
  clstl::vector<int> v;
  v.push_back(32);
  v.resize(10);
  ASSERT_EQ(v.capacity(), 10);
  ASSERT_EQ(v.size(), 10);

  for (std::size_t i = 1; i < 10; i++) {
    ASSERT_EQ(v[i], 0);
  }

  ASSERT_EQ(v[0], 32);

  v.resize(1);
  ASSERT_EQ(v.capacity(), 10);
  ASSERT_EQ(v.size(), 1);
  ASSERT_EQ(v[0], 32);
}

TEST(Vector, TestIterator) {
  clstl::vector<int> v;
  for (int i = 0; i < 10; i++) {
    v.push_back(i);
  }

  int i = 0;
  for (auto it = v.begin(); it != v.end(); it++) {
    ASSERT_EQ(*it, i++);
  }

  for (auto it = v.rbegin(); it != v.rend(); it++) {
    ASSERT_EQ(*it, --i);
  }

  const clstl::vector<int> &cv = v;
  i = 0;
  for (auto it = cv.begin(); it != cv.end(); it++) {
    ASSERT_EQ(*it, i++);
  }

  i = 0;
  for (auto it = cv.cbegin(); it != cv.cend(); it++) {
    ASSERT_EQ(*it, i++);
  }

  i = 0;
  for (auto x : v) {
    ASSERT_EQ(x, i++);
  }

  i = 0;
  for (auto x : v) {
    ASSERT_EQ(x, i++);
  }
}

TEST(Vector, TestClear) {
  clstl::vector<int> v = {1, 2, 3};
  ASSERT_EQ(v.size(), 3);
  v.clear();
  ASSERT_EQ(v.size(), 0);
}

TEST(Vector, TestCopy) {
  clstl::vector<int> v = {1, 2, 3};
  ASSERT_EQ(v.size(), 3);
  clstl::vector<int> v2 = v;
  ASSERT_EQ(v2.size(), 3);
  for (std::size_t i = 0; i < v2.size(); i++) {
    ASSERT_EQ(v2[i], i + 1);
  }
}
