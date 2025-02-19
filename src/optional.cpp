#include <CLSTL/optional.h>

namespace clstl {
bad_optional_access::bad_optional_access(const char *what) : m_What(what) {}
const char *bad_optional_access::what() const noexcept { return this->m_What; }
} // namespace clstl
