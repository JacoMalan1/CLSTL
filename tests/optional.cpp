#include <CLSTL/optional.h>
#include <gtest/gtest.h>

TEST(Optional, TestEmpty) {
  clstl::optional<int> opt;
  ASSERT_FALSE(opt.has_value());
  ASSERT_ANY_THROW(opt.value());
}

TEST(Optional, TestValue) {
  clstl::optional<int> opt = 3;
  ASSERT_TRUE(opt.has_value());
  ASSERT_EQ(opt.value(), 3);
}

TEST(Optional, TestValueOr) {
  clstl::optional<int> opt;
  ASSERT_EQ(opt.value_or(3), 3);
}

TEST(Optional, TestAndThen) {
  clstl::optional<int> opt = 3;
  ASSERT_EQ(opt.and_then([](int x) { return clstl::optional(x + 3); }).value(),
            6);
  clstl::optional<int> opt2;
  ASSERT_FALSE(
      opt2.and_then([](int x) { return clstl::optional(x + 1); }).has_value());
}

struct Entity {
  const char *name;
  uint32_t age;

  Entity(const char *name, uint32_t age) : name(name), age(age) {}
};

TEST(Optional, TestConstruct) {
  clstl::optional<Entity> opt_entity("Hello World!", 3);
  ASSERT_TRUE(opt_entity.has_value());
  ASSERT_EQ(opt_entity->name, "Hello World!");
  ASSERT_EQ(opt_entity->age, 3);
}

TEST(Optional, TestReset) {
  clstl::optional<int> opt = 3;
  ASSERT_TRUE(opt.has_value());
  opt.reset();
  ASSERT_FALSE(opt.has_value());
}
