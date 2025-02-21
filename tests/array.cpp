#include <CLSTL/array.h>
#include <array>
#include <gtest/gtest.h>

template class clstl::array<int, 32>;
template class clstl::array_iter<int>;

struct Entity {
  const char *name;
  int age;

  Entity(const char *name, int age) : name(name), age(age) {}
  Entity(const Entity &other) : name(other.name), age(other.age) {}
  Entity &operator=(const Entity &other) {
    this->name = other.name;
    this->age = other.age;
    return *this;
  }
};

TEST(Array, TestInit) {
  clstl::array<int, 3> arr = {1, 2, 3};
  ASSERT_FALSE(arr.empty());
  ASSERT_EQ(arr.size(), 3);
  for (std::size_t i = 0; i < arr.size(); i++) {
    ASSERT_EQ(arr[i], i + 1);
    ASSERT_EQ(arr.at(i), i + 1);
  }

  ASSERT_ANY_THROW(arr.at(3));

  for (std::size_t i = 0; i < arr.size(); i++) {
    ASSERT_EQ(arr.data()[i], i + 1);
  }

  std::size_t i = 1;
  for (auto it = arr.begin(); it != arr.end(); it++) {
    ASSERT_EQ(it, it);
    ASSERT_EQ(*it, i++);
  }

  i = 1;
  for (auto it = arr.begin(); it != arr.end(); it++) {
    const auto &it2 = it;
    ASSERT_EQ(*it2, i++);
  }

  i = 1;
  for (auto it = arr.begin(); it != arr.end(); it += 1) {
    ASSERT_EQ(*it, i++);
  }

  i = 1;
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    ASSERT_EQ(*it, i++);
  }

  ASSERT_EQ(arr.front(), 1);
  ASSERT_EQ(arr.back(), 3);

  const clstl::array<int, 3> &c_arr = arr;
  ASSERT_EQ(c_arr.front(), 1);
  ASSERT_EQ(c_arr.back(), 3);
  for (std::size_t i = 0; i < arr.size(); i++) {
    ASSERT_EQ(c_arr.at(i), i + 1);
    ASSERT_EQ(c_arr.data()[i], i + 1);
    ASSERT_EQ(c_arr[i], i + 1);
  }
  ASSERT_ANY_THROW(c_arr.at(3));
}

TEST(Array, TestComplex) {
  Entity e1("H", 1);
  Entity e2("E", 2);
  Entity e3("L", 3);
  clstl::array<Entity, 3> arr = {e1, e2, e3};

  std::size_t i = 1;
  for (auto it = arr.begin(); it != arr.end(); it++) {
    const auto &it_ref = it;
    ASSERT_EQ(it->age, i);
    ASSERT_EQ(it_ref->age, i++);
  }
}
