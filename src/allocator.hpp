#include <cstddef>
#include <cstdlib>
template <typename T>
class MallocAllocator {
 public:
  using value_type = T;
  MallocAllocator() = default;
  template <class U>
  constexpr MallocAllocator(const MallocAllocator<U>&) noexcept {}
  T* allocate(std::size_t n) {
    return static_cast<T*>(std::malloc(n * sizeof(T)));
  }

  void deallocate(T* p, std::size_t) noexcept { std::free(p); }
};