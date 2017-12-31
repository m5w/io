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

#include "read.h"

namespace lttoolbox {

auto read(std::istream &is, std::uint64_t &x) -> decltype(is) {
  return Read<0ull>::read(is, x, is.get());
}

std::uint64_t read(std::istream &is) {
  std::uint64_t x;
  read(is, x);
  return x;
}

template <std::size_t n>
auto Read<n>::read(std::istream &is, std::uint64_t &x, const unsigned char c)
    -> decltype(is) {
  if (c > Read<n>::maximum_c)
    return Read<n + 1ull>::read(is, x, c);

  x = static_cast<std::uint64_t>(static_cast<unsigned char>(c ^ Read<n>::mask))
      << (8ull * n);
  char s[n];
  is.read(s, n);
  copy_least_significant_bytes(x, s, Read<n>::s_distance_bit);
  return is;
}

auto Read<0ull>::read(std::istream &is, std::uint64_t &x,
                      const unsigned char c) -> decltype(is) {
  if (c > Read<0ull>::maximum_c)
    return Read<1ull>::read(is, x, c);

  x = static_cast<unsigned char>(c);
  return is;
}

auto Read<1ull>::read(std::istream &is, std::uint64_t &x,
                      const unsigned char c) -> decltype(is) {
  if (c > Read<1ull>::maximum_c)
    return Read<2ull>::read(is, x, c);

  x = static_cast<std::uint64_t>(
          static_cast<unsigned char>(c ^ Read<1ull>::mask))
      << 8ull;
  x |= static_cast<unsigned char>(is.get());
  return is;
}

auto Read<7ull>::read(std::istream &is, std::uint64_t &x,
                      const unsigned char c) -> decltype(is) {
  if (c > Read<7ull>::maximum_c)
    return Read<8ull>::read(is, x, c);

  x = 0ull;
  char s[7ull];
  is.read(s, 7ull);
  copy_least_significant_bytes(x, s, 48ull);
  return is;
}

auto Read<8ull>::read(std::istream &is, std::uint64_t &x,
                      const unsigned char c) -> decltype(is) {
  x = 0ull;
  char s[8ull];
  is.read(s, 8ull);
  copy_least_significant_bytes(x, s, 56ull);
  return is;
}

} // end namespace lttoolbox
