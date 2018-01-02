// This file is part of io.
//
// io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with io.  If not, see <http://www.gnu.org/licenses/>.

#ifndef APERTIUM_LTTOOLBOX_ENCODE_H
#define APERTIUM_LTTOOLBOX_ENCODE_H

#include <cstddef>
#include <cstdint>

#include <ostream>
#include <string>

#include "mask.h"

namespace lttoolbox {

auto encode(std::ostream &os, const std::uint64_t &x) -> decltype(os);

namespace {

static constexpr std::uint64_t get_maximum_x(const std::size_t n,
                                             const unsigned char mask) {
  return ((static_cast<unsigned char>(~mask) + 1ull) << (8ull * n - 1ull)) -
         1ull;
}

template <std::size_t n> class Encoder {
public:
  static inline auto encode(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr unsigned char mask = get_mask(n);
  static constexpr std::uint64_t maximum_x =
      get_maximum_x(n, Encoder<n>::mask);
  static constexpr std::size_t s_size = n + 1ull;
};

template <> class Encoder<0ull> {
public:
  static inline auto encode(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr std::uint64_t maximum_x =
      ((static_cast<unsigned char>(~0ull) + 1ull) >> 1ull) - 1ull;
};

template <> class Encoder<7ull> {
public:
  static inline auto encode(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr unsigned char mask = get_mask(7ull);
  static constexpr std::uint64_t maximum_x =
      get_maximum_x(7ull, Encoder<7ull>::mask);
};

template <> class Encoder<8ull> {
public:
  static inline auto encode(std::ostream &os, const std::uint64_t &x)
      -> decltype(os);
  static constexpr unsigned char mask = static_cast<unsigned char>(~0ull);
};

static inline void copy_least_significant_bytes(char *s_rbegin, char *const s,
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
