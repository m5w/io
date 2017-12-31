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

#include <ostream>

#include "mask.h"

namespace lttoolbox {

auto write(std::ostream &os, const std::uint64_t &x) -> decltype(os);

namespace {

static constexpr std::uint64_t get_maximum_x(const std::size_t n,
                                             const char mask) {
  return ((static_cast<unsigned char>(~mask) + 1ull)
          << (8ull * (n - 1ull) - 1ull)) -
         1ull;
}

template <std::size_t n> class Write {
public:
  static inline auto write(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr char mask = get_mask(n);
  static constexpr std::uint64_t maximum_x = get_maximum_x(n, Write<n>::mask);
  static constexpr std::size_t maximum_s_index = n - 1ull;
};

template <> class Write<1ull> {
public:
  static inline auto write(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr std::uint64_t maximum_x =
      ((static_cast<unsigned char>(~static_cast<char>(0ull)) + 1ull) >> 1ull) -
      1ull;
};

template <> class Write<8ull> {
public:
  static inline auto write(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr char mask = get_mask(8ull);
  static constexpr std::uint64_t maximum_x =
      get_maximum_x(8ull, Write<8ull>::mask);
};

template <> class Write<9ull> {
public:
  static inline auto write(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr char mask = ~static_cast<char>(0ull);
};

// Copy the n = s_rbegin - s + 1 least significant bytes of x to s.
//
// This copies the least-significant byte of x to *s_rbegin, which is the (n -
// 1)-th (starting at zero) element of s.  s is a byte array that must have at
// least n elements; otherwise, the behavior of this function is undefined.
// The second-least-significant byte of x is then copied to *(s_rbegin - 1),
// which is the (n - 2)-th element of s.  This continues until something is
// copied into the first element of s.  Note that if n is larger than the size
// of x in bytes, then the first (n - sizeof x) bytes of s will be set to zero.
void copy_least_significant_bytes(char *s_rbegin, char *const s,
                                  std::uint64_t x);

void copy_least_significant_bytes(char *s_rbegin, char *const s,
                                  std::uint64_t x) {
  for (;;) {
    *s_rbegin = x;

    if (s_rbegin == s)
      break;

    x >>= 8ull;
    --s_rbegin;
  }
}
}

} // end namespace lttoolbox

#endif
