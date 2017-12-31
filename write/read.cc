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

#include "read.h"

namespace lttoolbox {

template <std::size_t n>
auto Read<n>::read(std::istream &is, std::uint64_t &x, const char c)
    -> decltype(is) {
  if (c > Read<n>::maximum_c)
    return Read<n + 1ull>::read(is, x);

  x |= static_cast<unsigned char>(c ^ Read<n>::mask) << (8ull * n);
  char s[n];
  is.read(s, n);
  copy_least_significant_bytes(x, s, Read<n>::maximum_s_index);
  return is;
}

} // end namespace lttoolbox
