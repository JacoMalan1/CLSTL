#include <CLSTL/optional.h>
#include <gtest/gtest.h>

template union clstl::__internal::opt_storage<int>;
template class clstl::optional<int>;

TEST(Optional, TestEmpty) {
  clstl::optional<int> opt;
  ASSERT_FALSE(opt.has_value());
  ASSERT_ANY_THROW(opt.value());

  const clstl::optional<int> &c_opt = opt;
  ASSERT_ANY_THROW(c_opt.value());
}

TEST(Optional, TestValue) {
  clstl::optional<int> opt = 3;
  ASSERT_TRUE(opt.has_value());
  ASSERT_EQ(opt.value(), 3);

  const clstl::optional<int> &c_opt = opt;
  ASSERT_TRUE(c_opt.has_value());
  ASSERT_EQ(c_opt.value(), 3);
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
  clstl::optional<Entity> opt_entity = Entity("Hello World!", 3);
  ASSERT_TRUE(opt_entity.has_value());
  ASSERT_EQ(opt_entity->name, "Hello World!");
  ASSERT_EQ(opt_entity->age, 3);
  ASSERT_EQ((*opt_entity).name, "Hello World!");
  ASSERT_EQ((*opt_entity).age, 3);

  const clstl::optional<Entity> &c_opt_entity = opt_entity;
  ASSERT_TRUE(c_opt_entity.has_value());
  ASSERT_EQ(c_opt_entity->name, "Hello World!");
  ASSERT_EQ(c_opt_entity->age, 3);
  ASSERT_EQ((*c_opt_entity).name, "Hello World!");
  ASSERT_EQ((*c_opt_entity).age, 3);

  clstl::optional<Entity> opt2 = Entity("Hi", 4);
  opt_entity = opt2;
  ASSERT_TRUE(opt_entity.has_value());
  ASSERT_EQ(opt_entity->name, "Hi");
  ASSERT_EQ(opt_entity->age, 4);
  Entity e("Hi", 4);
  opt2 = e;
  ASSERT_TRUE(opt2.has_value());
  ASSERT_EQ(opt2->name, "Hi");
  ASSERT_EQ(opt2->age, 4);

  clstl::optional<Entity> opt3 = opt2;
  ASSERT_TRUE(opt3.has_value());
  ASSERT_EQ(opt3->name, "Hi");
  ASSERT_EQ(opt3->age, 4);

  clstl::optional<Entity> opt4;
  ASSERT_FALSE(opt4.has_value());
  clstl::optional<Entity> opt5 = opt4;
  ASSERT_FALSE(opt5.has_value());
  opt5 = opt4;
  ASSERT_FALSE(opt5.has_value());
}

TEST(Optional, TestReset) {
  clstl::optional<int> opt = 3;
  ASSERT_TRUE(opt.has_value());
  opt.reset();
  ASSERT_FALSE(opt.has_value());
}
