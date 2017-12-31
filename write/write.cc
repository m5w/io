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

#include "write.h"

namespace lttoolbox {

auto write(std::ostream &os, const std::uint64_t &x) -> decltype(os) {
  return Write<1ull>::write(os, x);
}

template <std::size_t n>
auto Write<n>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Write<n>::maximum_x)
    return Write<n + 1ull>::write(os, x);

  char s[n];
  copy_least_significant_bytes(s + maximum_s_index, s, x);
  s[0ull] |= Write<n>::mask;
  return os.write(s, n);
}

auto Write<1ull>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Write<1ull>::maximum_x)
    return Write<2ull>::write(os, x);

  return os.put(x);
}

auto Write<8ull>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  if (x > Write<8ull>::maximum_x)
    return Write<9ull>::write(os, x);

  char s[8ull];
  copy_least_significant_bytes(s + 7ull, s + 1ull, x);
  *s = Write<8ull>::mask;
  return os.write(s, 8ull);
}

auto Write<9ull>::write(std::ostream &os, const std::uint64_t &x)
    -> decltype(os) {
  char s[9ull];
  copy_least_significant_bytes(s + 8ull, s + 1ull, x);
  *s = Write<9ull>::mask;
  return os.write(s, 9ull);
}

} // end namespace lttoolbox
