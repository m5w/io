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

#ifndef APERTIUM_LTTOOLBOX_READ_H
#define APERTIUM_LTTOOLBOX_READ_H

#include <cstddef>
#include <cstdint>

#include <istream>

#include "mask.h"

namespace lttoolbox {

auto read(std::istream &is, std::uint64_t &x) -> decltype(is);
std::uint64_t read(std::istream &is);

namespace {

static constexpr unsigned char get_maximum_c(const unsigned char mask) {
  return ~static_cast<unsigned char>(
      (static_cast<unsigned char>(~mask) + 1ull) >> 1ull);
}

template <std::size_t n> class Read {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char mask = get_mask(n);
  static constexpr unsigned char maximum_c = get_maximum_c(Read<n>::mask);
  static constexpr std::size_t maximum_s_index = n - 1ull;
};

template <> class Read<0ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char maximum_c = get_maximum_c(get_mask(0ull));
};

template <> class Read<1ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char mask = get_mask(1ull);
  static constexpr unsigned char maximum_c = get_maximum_c(Read<1ull>::mask);
};

template <> class Read<7ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char maximum_c = get_maximum_c(get_mask(7ull));
};

template <> class Read<8ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
};

static inline void copy_least_significant_bytes(std::uint64_t &x,
                                                const char *s,
                                                std::size_t maximum_s_index) {
  for (; maximum_s_index != 0ull; --maximum_s_index) {
    x |= static_cast<std::uint64_t>(static_cast<unsigned char>(*s))
         << (8ull * maximum_s_index);
    ++s;
  }

  x |= static_cast<unsigned char>(*s);
}
}

} // end namespace lttoolbox

#endif
