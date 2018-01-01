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

#ifndef APERTIUM_LTTOOLBOX_READ_H
#define APERTIUM_LTTOOLBOX_READ_H

#include <cstddef>
#include <cstdint>

#include <istream>

#include "mask.h"

namespace lttoolbox {

// Read a value encoded in Apertium binary format from `is` into `x` and then
// return `is`.
//
// This function's signature is similar to that of the following functions,
//
//   basic_istream<charT,traits>& operator>>(bool& n);
//   basic_istream<charT,traits>& operator>>(short& n);
//   basic_istream<charT,traits>& operator>>(unsigned short& n);
//   basic_istream<charT,traits>& operator>>(int& n);
//   basic_istream<charT,traits>& operator>>(unsigned int& n);
//   basic_istream<charT,traits>& operator>>(long& n);
//   basic_istream<charT,traits>& operator>>(unsigned long& n);
//   basic_istream<charT,traits>& operator>>(long long& n);
//   basic_istream<charT,traits>& operator>>(unsigned long long& n);
//
// except that the this function is not a member of `std::istream`.
//
// One can encode an integer from 0 to 2**64 - 1 in Apertium binary format.
// Thus, this function takes a reference to an unsigned 64-bit integer as an
// argument.  However, one does not need to interpret a value encoded in
// Apertium binary format as an unsigned 64-bit integer -- the data type simply
// provides 64 bits of space.
//
// Q: Why would one use 64 bits of space to encode a value that one could
// encode in fewer bits?
//
// Apertium binary format uses the fewest possible number of bytes to encode an
// unsigned 64-bit integer assuming leading zeros.  For integers between 0 and
// 2**7 - 1 (inclusive), it uses 1 byte.  For integers between 2**7 and
// 2**14 - 1 (inclusive), it uses 2 bytes.  This pattern of encoding 7 bits of
// information per byte continues up to and including encoding integers between
// 2**49 and 2**56 - 1 (inclusive) in 8 bytes.  However, Apertium binary format
// encodes integers between 2**56 and 2**64 - 1 (inclusive) in 9 bytes.
//
// Q: How does this function know how many bytes to read?
//
// Since Apertium binary format must use at least one byte to encode any
// value[1], this function first reads one byte from `is`.  The number of
// leading ones in this byte is the number of subsequent bytes that this
// function must then read to decode the value.  This design is similar to that
// of UTF-8.  There may be 0 leading ones, but, when there are between 0 and 7
// (inclusive) leading ones, there is a terminating zero.  All of the
// subsequent bits, including those in subsequent bytes, are the literal bits
// of the value.  It is important to note that this sequence of bits is
// bytewise big-endian (and bitwise little-endian).  However, when there are 8
// leading ones -- and thus each of the first byte's bits is one -- there is no
// terminating zero, and thus the 8 subsequent bytes are the literal bytes of
// the value (and, again, are bytewise big-endian and bitwise little-endian).
//
// When there are 0 leading ones, as in the following example,
//
//   0b0.......
//
// there is a terminating zero followed by 7 bits.  Thus, Apertium binary
// format encodes all of the integers that ASCII encodes as ASCII encodes them.
// When there is 1 leading one,
//
//   0b10...... ........
//
// there is a terminating zero, followed by 6 bits, followed by 1 byte.  When
// there are 7 leading ones,
//
//   0b11111110 ........ ........ ........ ........ ........ ........ ........
//
// there is a terminating zero followed by 7 bytes.
auto read(std::istream &is, std::uint64_t &x) -> decltype(is);

namespace {

// Return the maximum value (interpreted as an unsigned char) of the first byte
// of a value encoded in Apertium binary format that's mask is `mask`.
static constexpr unsigned char get_maximum_c(const unsigned char mask) {
  return ~static_cast<unsigned char>(
      (static_cast<unsigned char>(~mask) + 1ull) >> 1ull);
}

template <std::size_t n> class Read {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char mask = get_mask(n);
  static constexpr unsigned char maximum_c = get_maximum_c(Read<n>::mask);
  static constexpr std::size_t s_distance_bit = 8ull * (n - 1ull);
};

template <> class Read<0ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char maximum_c = get_maximum_c(get_mask(0ull));
};

template <> class Read<1ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char mask = get_mask(1ull);
  static constexpr unsigned char maximum_c = get_maximum_c(Read<1ull>::mask);
};

template <> class Read<7ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
  static constexpr unsigned char maximum_c = get_maximum_c(get_mask(7ull));
};

template <> class Read<8ull> {
public:
  static inline auto read(std::istream &is, std::uint64_t &x,
                          const unsigned char c) -> decltype(is);
};

static inline void copy_least_significant_bytes(std::uint64_t &x,
                                                const char *s,
                                                std::size_t s_distance_bit) {
  for (; s_distance_bit != 0ull; s_distance_bit -= 8ull) {
    x |= static_cast<std::uint64_t>(static_cast<unsigned char>(*s))
         << s_distance_bit;
    ++s;
  }

  x |= static_cast<unsigned char>(*s);
}
}

} // end namespace lttoolbox

#endif
