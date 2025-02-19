#include <CLSTL/hash.h>
#include <gtest/gtest.h>
#include <limits>

#define TEST_HASH_INT(t, n)                                                    \
  ASSERT_EQ(clstl::hash<t>{}(n % std::numeric_limits<t>::max()),               \
            n % std::numeric_limits<t>::max());
#define TEST_HASH_FOR_ALL_INTS(n)                                              \
  TEST_HASH_INT(uint8_t, n)                                                    \
  TEST_HASH_INT(int8_t, n)                                                     \
  TEST_HASH_INT(uint16_t, n)                                                   \
  TEST_HASH_INT(int16_t, n)                                                    \
  TEST_HASH_INT(uint32_t, n)                                                   \
  TEST_HASH_INT(int32_t, n)                                                    \
  TEST_HASH_INT(uint64_t, n)                                                   \
  TEST_HASH_INT(int64_t, n)

TEST(Hash, TestInts) {
  for (std::size_t i = 0; i < 65536; i++) {
    TEST_HASH_FOR_ALL_INTS(i)
  }
}

struct Entity {
  const char *name;
  int32_t age;
};

void foo() {}
