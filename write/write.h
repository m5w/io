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

namespace lttoolbox {

auto write(std::ostream &os, const std::uint64_t &x) -> decltype(os);

namespace {

template <std::size_t n> class Write {
public:
  static inline auto write(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);

#define WRITE(N)                                                              \
  static constexpr char mask =                                                \
      ~static_cast<char>((1ull << (9ull - N)) - 1ull);                        \
  static constexpr std::uint64_t maximum_x =                                  \
      ((static_cast<unsigned char>(~mask) + 1ull) << (8ull * (N - 1) - 1)) -  \
      1ull

  WRITE(n);
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
  WRITE(8ull);

#undef WRITE
};

template <> class Write<9ull> {
public:
  static inline auto write(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr char mask = ~static_cast<char>(0ull);
};
}

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

} // end namespace lttoolbox

#endif
