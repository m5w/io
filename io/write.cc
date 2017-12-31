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

#include "write.h"

namespace lttoolbox {

auto write(std::ostream &os, const std::uint64_t &x) -> decltype(os) {
  return Write<0ull>::write(os, x);
}

template <std::size_t n>
auto Write<n>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Write<n>::maximum_x)
    return Write<n + 1ull>::write(os, x);

  char s[Write<n>::s_size];
  copy_least_significant_bytes(s + n, s, x);
  s[0ull] |= Write<n>::mask;
  return os.write(s, Write<n>::s_size);
}

auto Write<0ull>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  auto y = maximum_x;
  if (x > Write<0ull>::maximum_x)
    return Write<1ull>::write(os, x);

  return os.put(x);
}

auto Write<7ull>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Write<7ull>::maximum_x)
    return Write<8ull>::write(os, x);

  char s[8ull];
  copy_least_significant_bytes(s + 7ull, s + 1ull, x);
  *s = Write<7ull>::mask;
  return os.write(s, 8ull);
}

auto Write<8ull>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  char s[9ull];
  copy_least_significant_bytes(s + 8ull, s + 1ull, x);
  *s = Write<8ull>::mask;
  return os.write(s, 9ull);
}

} // end namespace lttoolbox
