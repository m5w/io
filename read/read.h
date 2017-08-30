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
#include <iostream>
#include <istream>

namespace lttoolbox {

auto read(std::basic_istream<std::uint8_t> &in, std::uint64_t &val)
    -> decltype(in);

namespace {

template <std::size_t n> class Read {
public:
  static inline auto read(std::basic_istream<std::uint8_t> &in,
                          std::uint64_t &val) -> decltype(in);

  static constexpr std::uint8_t mask = Read<n - 1>::mask >> 1;
};

}

} // end namespace lttoolbox

#endif
