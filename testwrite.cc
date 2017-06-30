#include <cstddef>
#include <cstdint>
#include <iostream>

#include "write.h"

int main() {
  std::uint64_t minimum_x = 0b00000000;
  std::uint64_t maximum_x = 0b01111111;

  for (std::size_t n = 1; n < 9; ++n) {
    lttoolbox::write(std::cout, minimum_x);
    lttoolbox::write(std::cout, maximum_x);
    ++maximum_x;
    minimum_x = maximum_x;
    maximum_x = (maximum_x << 7) - 1;
  }

  lttoolbox::write(std::cout, minimum_x);
  maximum_x  = (maximum_x << 1) + 1;
  lttoolbox::write(std::cout, maximum_x);
}
