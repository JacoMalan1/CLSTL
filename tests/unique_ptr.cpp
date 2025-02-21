#include <CLSTL/unique_ptr.h>
#include <gtest/gtest.h>

template class clstl::unique_ptr<int>;

struct Entity {
  bool *dealloc;

  Entity(bool *dealloc) : dealloc(dealloc) {}
  ~Entity() { *this->dealloc = true; }

  bool is_dealloc() const { return *this->dealloc; }
};

TEST(UniquePtr, TestNull) { clstl::unique_ptr<int> p(nullptr); }

TEST(UniquePtr, TestAlloc) {
  bool dealloc = false;

  {
    auto p = clstl::make_unique<Entity>(&dealloc);
    ASSERT_TRUE(p);
    ASSERT_TRUE(p.get());
    ASSERT_FALSE(p->is_dealloc());
    ASSERT_FALSE((*p).is_dealloc());

    const auto &p_ref = p;
    ASSERT_TRUE(p_ref);
    ASSERT_TRUE(p_ref.get());
    ASSERT_FALSE(p_ref->is_dealloc());
    ASSERT_FALSE((*p_ref).is_dealloc());
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

  dealloc = false;
  auto p2 = clstl::make_unique<Entity>(&dealloc);
  const auto &p2_ref = p2;
  const auto &del2 = p2_ref.get_deleter();
  auto *ptr2 = p2.release();
  del2(ptr2);
  ASSERT_TRUE(dealloc);
}
