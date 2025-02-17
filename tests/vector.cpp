#include <CLSTL/vector.h>
#include <cstring>
#include <gtest/gtest.h>

TEST(Vector, TestEmpty) {
  clstl::vector<int> v;
  ASSERT_EQ(v.capacity(), 0);
  ASSERT_EQ(v.size(), 0);
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
  ASSERT_EQ(std::strcmp(v[0].name, "Hello"), 0);
  ASSERT_EQ(v[0].age, 32);
  ASSERT_EQ(v.front(), v[0]);
  ASSERT_EQ(v.back(), v[0]);
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

  i = 0;
  for (auto x : v) {
    ASSERT_EQ(x, i++);
  }

  i = 0;
  for (auto x : v) {
    ASSERT_EQ(x, i++);
  }
}
