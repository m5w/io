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

#include "decode.h"

namespace lttoolbox {

auto decode(std::istream &is, std::uint64_t &x) -> decltype(is) {
  char c{0};
  is.get(c);
  return Decoder<0ull>::decode(is, x, c);
}

template <std::size_t n>
auto Decoder<n>::decode(std::istream &is, std::uint64_t &x,
                        const unsigned char c) -> decltype(is) {
  if (c > Decoder<n>::maximum_c)
    return Decoder<n + 1ull>::decode(is, x, c);

  char s[n];
  is.read(s, n);
  x = static_cast<std::uint64_t>(
          static_cast<unsigned char>(c ^ Decoder<n>::mask))
      << (8ull * n);
  copy_least_significant_bytes(x, s, Decoder<n>::s_distance_bit);
  return is;
}

auto Decoder<0ull>::decode(std::istream &is, std::uint64_t &x,
                           const unsigned char c) -> decltype(is) {
  if (c > Decoder<0ull>::maximum_c)
    return Decoder<1ull>::decode(is, x, c);

  x = static_cast<unsigned char>(c);
  return is;
}

auto Decoder<1ull>::decode(std::istream &is, std::uint64_t &x,
                           const unsigned char c) -> decltype(is) {
  if (c > Decoder<1ull>::maximum_c)
    return Decoder<2ull>::decode(is, x, c);

  char s{0};
  is.get(s);
  x = static_cast<std::uint64_t>(
          static_cast<unsigned char>(c ^ Decoder<1ull>::mask))
      << 8ull;
  x |= static_cast<unsigned char>(s);
  return is;
}

auto Decoder<7ull>::decode(std::istream &is, std::uint64_t &x,
                           const unsigned char c) -> decltype(is) {
  if (c > Decoder<7ull>::maximum_c)
    return Decoder<8ull>::decode(is, x, c);

  char s[7ull];
  is.read(s, 7ull);
  x = 0ull;
  copy_least_significant_bytes(x, s, 48ull);
  return is;
}

auto Decoder<8ull>::decode(std::istream &is, std::uint64_t &x,
                           const unsigned char c) -> decltype(is) {
  char s[8ull];
  is.read(s, 8ull);
  x = 0ull;
  copy_least_significant_bytes(x, s, 56ull);
  return is;
}

} // end namespace lttoolbox
