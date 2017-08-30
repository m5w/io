// This file is part of write.
//
// write is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// write is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with write.  If not, see <http://www.gnu.org/licenses/>.

#ifndef APERTIUM_LTTOOLBOX_WRITE_H
#define APERTIUM_LTTOOLBOX_WRITE_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

namespace lttoolbox {

auto write(std::basic_ostream<std::uint8_t> &os, const std::uint64_t &x)
    -> decltype(os);

namespace {

template <std::size_t n> class Write {
public:
  static inline auto write(std::basic_ostream<std::uint8_t> &os,
                           const std::uint64_t &x) -> decltype(os);
#if ENABLE_DEBUG
  static const std::string indent;
#endif

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

  static constexpr std::size_t maximum_s_index = n - 1;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
  static constexpr std::uint8_t mask = Write<n + 1>::mask << 1;
#pragma GCC diagnostic pop
};

template <> class Write<1> {
public:
  static inline auto write(std::basic_ostream<std::uint8_t> &os,
                           const std::uint64_t &x) -> decltype(os);
  static constexpr std::uint64_t maximum_x = 0b01111111;
};

template <> class Write<9> {
public:
  static inline auto write(std::basic_ostream<std::uint8_t> &os,
                           const std::uint64_t &x) -> decltype(os);
#if ENABLE_DEBUG
  static const std::string indent;
#endif
  static constexpr std::uint8_t mask = 0b11111111;
};
}

// Copy the (maximum_s_index + 1) least significant bytes of x to s.
//
// This copies the least-significant byte of x to the maximum_s_index-th
// (starting at zero) element of s.  s is a byte array that must have at least
// n = maximum_s_index + 1 elements; otherwise, the behavior of this function
// is undefined.  The second-least-significant byte of x is then copied to the
// (maximum_s_index - 1)-th element of s.  This continues until something is
// copied into the first element of s.  Note that if n is larger than the size
// of x in bytes, then the first (n - sizeof x) bytes of s will be set to zero.
void copy_least_significant_bytes(std::uint8_t *s, std::size_t maximum_s_index,
                                  std::uint64_t x);

#if ENABLE_DEBUG
std::ostream &print_in_write(const std::size_t &n, const std::uint64_t &x,
                             const char *indent = "");
std::ostream &print_x_above_maximum_x(const std::uint64_t &x,
                                      const std::uint64_t &maximum_x,
                                      const char *indent = "");
std::ostream &print_x_not_above_maximum_x(const std::uint64_t &x,
                                          const std::uint64_t &maximum_x,
                                          const char *indent = "");
#endif

} // end namespace lttoolbox

#endif
