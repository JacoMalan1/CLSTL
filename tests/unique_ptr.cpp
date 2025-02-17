#include <CLSTL/unique_ptr.h>
#include <gtest/gtest.h>

struct Entity {
  bool *dealloc;

  Entity(bool *dealloc) : dealloc(dealloc) {}
  ~Entity() { *this->dealloc = true; }

  bool is_dealloc() { return *this->dealloc; }
};

TEST(UniquePtr, TestNull) { clstl::unique_ptr<int> p(nullptr); }

TEST(UniquePtr, TestAlloc) {
  bool dealloc = false;

  {
    auto p = clstl::make_unique<Entity>(&dealloc);
    ASSERT_FALSE(p->is_dealloc());
  }

  ASSERT_TRUE(dealloc);
}

TEST(UniquePtr, TestRelease) {
  bool dealloc = false;
  auto p = clstl::make_unique<Entity>(&dealloc);
  auto *ptr = p.release();
  ASSERT_FALSE(dealloc);
  auto &del = p.get_deleter();
  del(ptr);
  ASSERT_TRUE(dealloc);
}
