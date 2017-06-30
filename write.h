#ifndef APERTIUM_LTTOOLBOX_WRITE_H
#define APERTIUM_LTTOOLBOX_WRITE_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>

namespace lttoolbox {

std::ostream &write(std::ostream &os, const std::uint64_t &x);

namespace {

template <std::size_t n> class Write {
public:
  static inline std::ostream &write(std::ostream &os, const std::uint64_t &x);

  // The previous Write's maximum_x must be left-shifted by seven, and seven
  // ones must be inserted from the right.  However, left-shifting introduces
  // zeros from the right.  Since maximum_x is a sequence of ones, though,
  // incrementing it yields a single one followed by the same number of zeros
  // as there were originally of ones.  Left-shifting this "head" by seven
  // inserts seven zeros from the right, and finally decrementing the new value
  // makes the leading one a zero and all the subsequent zeros ones -- the
  // original value has been left-shifted by seven, and seven ones have been
  // inserted from the right.  The relative complexity of this all does not
  // matter, as this is still O(1) and all must happen at compile-time.
  static constexpr std::uint64_t maximum_x =
      ((Write<n - 1>::maximum_x + 1) << 7) - 1;
};

template <> class Write<1> {
public:
  static inline std::ostream &write(std::ostream &os, const std::uint64_t &x);
  static constexpr std::uint64_t maximum_x = 0b01111111;
};

template <> class Write<9> {
public:
  static inline std::ostream &write(std::ostream &os, const std::uint64_t &x);
};

}

} // end namespace lttoolbox

#endif
