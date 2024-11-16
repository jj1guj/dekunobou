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

  // 等価比較演算子の定義
  bool operator==(const MallocAllocator& other) const noexcept {
    return true;  // 全てのインスタンスが等価であると仮定
  }

  bool operator!=(const MallocAllocator& other) const noexcept { return false; }

  void deallocate(T* p, std::size_t) noexcept { std::free(p); }
};