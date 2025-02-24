#include <CLSTL/list.h>
#include <cstring>
#include <gtest/gtest.h>

template struct clstl::__internal::list_node<int>;
template class clstl::list_iterator<clstl::list<int>>;
template class clstl::const_list_iterator<clstl::list<int>>;
template class clstl::list<int>;

TEST(LinkedList, TestEmpty) {
  clstl::list<int> l;
  ASSERT_EQ(l.size(), 0);
  ASSERT_TRUE(l.empty());
}

TEST(LinkedList, TestOne) {
  clstl::list<int> l;
  l.push_back(3);
  ASSERT_EQ(l.size(), 1);
  ASSERT_EQ(l[0], 3);
  ASSERT_ANY_THROW(l.at(1));
  ASSERT_EQ(l.front(), 3);

  const clstl::list<int> &l_ref = l;
  ASSERT_EQ(l_ref.front(), 3);
}

TEST(LinkedList, TestPushFront) {
  clstl::list<int> l;
  for (int i = 0; i < 10; i++) {
    l.push_front(i);
  }

  ASSERT_EQ(l.size(), 10);
  for (int i = 0; i < 10; i++) {
    ASSERT_EQ(l[i], 9 - i);
  }

  clstl::list<int> l2;
  for (int i = 0; i < 10; i++) {
    l2.emplace_front(i);
  }

  ASSERT_EQ(l2.size(), 10);
  for (int i = 0; i < 10; i++) {
    ASSERT_EQ(l2[i], 9 - i);
  }
}

TEST(LinkedList, TestCopyCtor) {
  clstl::list<int> l;
  for (std::size_t i = 0; i < 10; i++) {
    l.push_back(i);
  }
  ASSERT_EQ(l.size(), 10);

  clstl::list<int> l2(l);
  ASSERT_EQ(l2.size(), l.size());
  for (std::size_t i = 0; i < 10; i++) {
    ASSERT_EQ(l2[i], i);
  }
}

TEST(LinkedList, TestCopyAssignment) {
  clstl::list<int> l;
  for (std::size_t i = 0; i < 10; i++) {
    l.push_back(i);
  }
  ASSERT_EQ(l.size(), 10);

  clstl::list<int> l2;
  for (std::size_t i = 1; i < 5; i++) {
    l2.push_back(i);
  }

  l2 = l;
  ASSERT_EQ(l2.size(), l.size());
  for (std::size_t i = 0; i < 10; i++) {
    ASSERT_EQ(l2[i], i);
    ASSERT_EQ(l2.at(i), i);
  }

  clstl::list<int> l3 = clstl::list<int>(clstl::list<int>());
  ASSERT_EQ(l3.size(), 0);

  const clstl::list<int> &l_ref = l;
  ASSERT_ANY_THROW(l_ref.at(10));
  for (std::size_t i = 0; i < 10; i++) {
    ASSERT_EQ(l_ref[i], i);
    ASSERT_EQ(l_ref.at(i), i);
  }
}

struct Entity {
  const char *name;

  Entity(const char *name) : name(name) {}
};

TEST(LinkedList, TestIter) {
  clstl::list<int> l;
  for (std::size_t i = 0; i < 10; i++) {
    l.push_back(i);
  }

  auto it2 = l.begin();
  it2 += 2;
  ASSERT_EQ(*it2, 2);

  std::size_t i = 0;
  for (auto x : l) {
    ASSERT_EQ(x, i++);
  }

  i = 0;
  for (auto it = l.begin(); it != l.end(); it++) {
    ASSERT_EQ(*it, i++);
  }

  clstl::list<Entity> l2;
  l2.emplace_back("Hello World!");
  for (auto it = l2.begin(); it != l2.end(); it++) {
    ASSERT_EQ(std::strcmp(it->name, "Hello World!"), 0);
  }
}

TEST(LinkedList, TestEraseOne) {
  clstl::list<int> l;
  for (int i = 0; i < 10; i++) {
    l.push_back(i);
  }

  l.erase(l.begin());
  ASSERT_EQ(l.size(), 9);
  ASSERT_EQ(l[0], 1);

  l.erase(l.begin() + 3);
  ASSERT_EQ(l.size(), 8);
  ASSERT_EQ(l[3], 5);

  l.erase(l.begin() + (l.size() - 1));
  ASSERT_EQ(l.size(), 7);
  ASSERT_EQ(l[l.size() - 1], 8);
}

TEST(LinkedList, TestEraseMany) {
  clstl::list<int> l;
  for (int i = 0; i < 10; i++) {
    l.push_back(i);
  }

  l.erase(l.begin(), l.begin() + 3);
  ASSERT_EQ(l.size(), 7);
  ASSERT_EQ(l[0], 3);

  l.erase(l.begin(), l.end());
  ASSERT_EQ(l.size(), 0);

  for (int i = 0; i < 10; i++) {
    l.emplace_back(i);
  }

  l.erase(l.begin() + 1, l.end());
  ASSERT_EQ(l.size(), 1);
  ASSERT_EQ(l[0], 0);
}

TEST(LinkedList, TestPop) {
  clstl::list<int> l;
  for (int i = 0; i < 10; i++) {
    l.emplace_back(i);
  }

  for (int i = 0; i < 9; i++) {
    l.pop_back();
    ASSERT_EQ(l.size(), 9 - i);
    ASSERT_EQ(l[l.size() - 1], 8 - i);
  }

  l.pop_back();
  ASSERT_EQ(l.size(), 0);
}
