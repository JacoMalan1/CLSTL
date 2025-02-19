#include <CLSTL/string.h>
#include <gtest/gtest.h>
#include <sstream>

TEST(String, TestEmpty) { clstl::string s; }

TEST(String, TestFromCStr) {
  const char *x = "Hello World!";
  clstl::string s = x;

  ASSERT_EQ(s.length(), std::strlen(x));
  ASSERT_EQ(s.capacity(), s.length() + 1);
  for (std::size_t i = 0; i < s.length(); i++) {
    ASSERT_EQ(s[i], x[i]);
  }

  std::stringstream stream;
  stream << s;
  ASSERT_EQ(stream.str(), "Hello World!");
  ASSERT_EQ(std::strcmp(s.c_str(), x), 0);
}

TEST(String, TestCopy) {
  const char *x = "Hello World!";
  clstl::string s = x;
  clstl::string s2 = s;

  ASSERT_EQ(s.length(), s2.length());
  ASSERT_EQ(s, s2);

  clstl::string s3 = "Hi";
  s2 = s3;
  ASSERT_EQ(s2.length(), s3.length());
  ASSERT_EQ(s2, s3);

  const char *y = "Hello World";
  s3 = y;
  ASSERT_EQ(s3.length(), std::strlen(y));
  for (std::size_t i = 0; i < std::strlen(y); i++) {
    ASSERT_EQ(s3[i], y[i]);
  }
}

TEST(String, TestConcat) {
  clstl::string a = "Hello ";
  clstl::string b = "World!";

  clstl::string c = "Hello World!";
  clstl::string d = a + b;

  ASSERT_EQ(c.length(), d.length());
  ASSERT_EQ(d, c);
}

TEST(String, TestClear) {
  clstl::string x = "Hello World!";
  clstl::string y;

  x.clear();
  ASSERT_EQ(x, y);
}

TEST(String, TestBoundsChecks) {
  const char *s = "Hello";
  clstl::string x = s;
  for (std::size_t i = 0; i < x.length(); i++) {
    ASSERT_EQ(x.at(i), s[i]);
  }

  ASSERT_ANY_THROW(x.at(x.length()));
}
