#ifndef APERTIUM_LTTOOLBOX_WRITE_H
#define APERTIUM_LTTOOLBOX_WRITE_H

#include <cstdint>
#include <ostream>

namespace lttoolbox {

std::ostream &write(std::ostream &os, std::uint64_t x) {
  return os;
}

}  // end namespace lttoolbox
