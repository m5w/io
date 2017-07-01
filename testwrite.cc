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

#include <cstddef>
#include <cstdint>
#include <fstream>

#include "write.h"

int main() {
  std::ofstream os("testwrite.out");
  std::uint64_t minimum_x = 0b00000000;
  std::uint64_t maximum_x = 0b01111111;

  for (std::size_t n = 1; n < 9; ++n) {
    lttoolbox::write(os, minimum_x);
    lttoolbox::write(os, maximum_x);
    ++maximum_x;
    minimum_x = maximum_x;
    maximum_x = (maximum_x << 7) - 1;
  }

  lttoolbox::write(os, minimum_x);
  maximum_x  = (maximum_x << 1) + 1;
  lttoolbox::write(os, maximum_x);
}
