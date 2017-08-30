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

auto write(std::basic_ostream<std::uint8_t> &os, const std::uint64_t &x)
    -> decltype(os) {
  return Write<1>::write(os, x);
}

template <std::size_t n>
auto Write<n>::write(std::basic_ostream<std::uint8_t> &os,
                     const std::uint64_t &x) -> decltype(os) {
#if ENABLE_DEBUG
  print_in_write(n, x, indent.c_str());
#endif

  if (x > maximum_x) {
#if ENABLE_DEBUG
    print_x_above_maximum_x(x, maximum_x, indent.c_str());
#endif
    return Write<n + 1>::write(os, x);
  }

#if ENABLE_DEBUG
  print_x_not_above_maximum_x(x, maximum_x, indent.c_str());
#endif
  std::uint8_t s[n];
  copy_least_significant_bytes(s, maximum_s_index, x);
  s[0] |= mask;
  return os.write(s, n);
}

#if ENABLE_DEBUG
template <std::size_t n> const std::string Write<n>::indent((n - 1) * 2, ' ');
#endif

auto Write<1>::write(std::basic_ostream<std::uint8_t> &os,
                     const std::uint64_t &x) -> decltype(os) {
#if ENABLE_DEBUG
  print_in_write(1, x);
#endif

  if (x > maximum_x) {
#if ENABLE_DEBUG
    print_x_above_maximum_x(x, maximum_x);
#endif
    return Write<2>::write(os, x);
  }

#if ENABLE_DEBUG
  print_x_not_above_maximum_x(x, maximum_x);
#endif
  return os.put(x);
}

auto Write<9>::write(std::basic_ostream<std::uint8_t> &os,
                     const std::uint64_t &x) -> decltype(os) {
#if ENABLE_DEBUG
  print_in_write(9, x, indent.c_str());
#endif
  std::uint8_t s[9];
  copy_least_significant_bytes(s + 1, 7, x);
  *s = mask;
  return os.write(s, 9);
}

#if ENABLE_DEBUG
const std::string Write<9>::indent(16, ' ');
#endif

void copy_least_significant_bytes(std::uint8_t *s, std::size_t maximum_s_index,
                                  std::uint64_t x) {
  for (;;) {
    std::uint8_t byte = x;
    s[maximum_s_index] = byte;

    if (maximum_s_index == 0)
      break;

    x >>= 8;
    --maximum_s_index;
  }
}

#if ENABLE_DEBUG
std::ostream &print_in_write(const std::size_t &n, const std::uint64_t &x,
                             const char *indent) {
  return std::cerr << '\n'
                   << indent << "+-auto Write<" << n << ">::write(\n"
                   << indent << "|     std::basic_ostream<std::uint8_t>\n"
                   << indent << "|         &os,\n"
                   << indent << "|     const std::uint64_t\n"
                   << indent << "|         &x)\n"
                   << indent << "|     -> decltype(\n"
                   << indent << "|         os)’:\n"
                   << indent << "+-x is " << x << '\n';
}
#endif

} // end namespace lttoolbox
