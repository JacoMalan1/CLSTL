#include <CLSTL/shared_ptr.h>
#include <gtest/gtest.h>

struct Entity {
  bool *dealloc;

  Entity(bool *dealloc) : dealloc(dealloc) {}
  ~Entity() { *this->dealloc = true; }

  bool is_dealloc() { return *this->dealloc; }
};

TEST(SharedPointer, TestAlloc) {
  clstl::shared_ptr<int> p = clstl::make_shared<int>(32);
  ASSERT_EQ(*p, 32);
  ASSERT_EQ(p.use_count(), 1);
}

TEST(SharedPointer, TestSharing) {
  bool dealloc = false;
  {
    auto p = clstl::make_shared<Entity>(&dealloc);
    ASSERT_EQ(p.use_count(), 1);

    {
      clstl::shared_ptr<Entity> p2 = p;
      ASSERT_EQ(p.use_count(), 2);
      ASSERT_EQ(p2.use_count(), 2);
      ASSERT_FALSE(dealloc);
      ASSERT_FALSE(p->is_dealloc());
      ASSERT_FALSE(p2->is_dealloc());
    }

    ASSERT_EQ(p.use_count(), 1);
    ASSERT_FALSE(dealloc);
    ASSERT_FALSE(p->is_dealloc());
  }

  ASSERT_TRUE(dealloc);
}
