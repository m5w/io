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

template <std::size_t n>
std::ostream &Write<n>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr << "-----BEGIN std::ostream &\n"
               "Write<"
            << n << ">::write(std::ostream &os, const std::uint64_t &x)-----\n"
                    "x = "
            << x << "\n";

#endif

  if (x > maximum_x)
    return Write<n + 1>::write(os, x);

  std::cerr << "n = " << n << "\n"
                              "x = "
            << x << "\n";

#if ENABLE_DEBUG

  std::cerr << "-----END std::ostream &\n"
               "Write<"
            << n << ">::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os;
}

std::ostream &Write<1>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr
      << "-----BEGIN std::ostream &\n"
         "Write<1>::write(std::ostream &os, const std::uint64_t &x)-----\n"
         "x = "
      << x << "\n";

#endif

  if (x > maximum_x)
    return Write<2>::write(os, x);

  std::cerr << "n = 1\n"
               "x = "
            << x << "\n";

#if ENABLE_DEBUG

  std::cerr
      << "-----END std::ostream &\n"
         "Write<1>::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os;
}

std::ostream &Write<9>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr
      << "-----BEGIN std::ostream &\n"
         "Write<9>::write(std::ostream &os, const std::uint64_t &x)-----\n"
         "x = "
      << x << "\n";

#endif

  std::cerr << "n = 9\n"
               "x = "
            << x << "\n";

#if ENABLE_DEBUG

  std::cerr
      << "-----END std::ostream &\n"
         "Write<9>::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os;
}

std::ostream &write(std::ostream &os, const std::uint64_t &x) {
  return Write<1>::write(os, x);
}

}  // end namespace lttoolbox
