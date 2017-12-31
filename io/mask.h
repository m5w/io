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

#ifndef APERTIUM_LTTOOLBOX_MASK_H
#define APERTIUM_LTTOOLBOX_MASK_H

#include <cstddef>

namespace lttoolbox {

constexpr unsigned char get_mask(const std::size_t n) {
  return ~static_cast<unsigned char>((1ull << (8ull - n)) - 1ull);
}

} // end namespace lttoolbox

#endif
