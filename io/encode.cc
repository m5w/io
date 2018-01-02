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

#include "encode.h"

namespace lttoolbox {

auto encode(std::ostream &os, const std::uint64_t &x) -> decltype(os) {
  return Encoder<0ull>::encode(os, x);
}

template <std::size_t n>
auto Encoder<n>::encode(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Encoder<n>::maximum_x)
    return Encoder<n + 1ull>::encode(os, x);

  char s[Encoder<n>::s_size];
  copy_least_significant_bytes(s + n, s, x);
  s[0ull] |= Encoder<n>::mask;
  return os.write(s, Encoder<n>::s_size);
}

auto Encoder<0ull>::encode(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  auto y = maximum_x;
  if (x > Encoder<0ull>::maximum_x)
    return Encoder<1ull>::encode(os, x);

  return os.put(x);
}

auto Encoder<7ull>::encode(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Encoder<7ull>::maximum_x)
    return Encoder<8ull>::encode(os, x);

  char s[8ull];
  copy_least_significant_bytes(s + 7ull, s + 1ull, x);
  *s = Encoder<7ull>::mask;
  return os.write(s, 8ull);
}

auto Encoder<8ull>::encode(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  char s[9ull];
  copy_least_significant_bytes(s + 8ull, s + 1ull, x);
  *s = Encoder<8ull>::mask;
  return os.write(s, 9ull);
}

} // end namespace lttoolbox
