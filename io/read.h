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

// Decode a value encoded in Apertium binary format from `is` into `x` and then
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
// One can encode any binary value 64 or fewer bits in size in Apertium binary
// format.
//
// Q: Why are there not overloads for smaller data types?  Is this not
//    inefficient?
//
// Is this inefficient?  Yes and no.  It is computationally inefficient, but it
// does not affect the number of bytes used to encode a value.  That number
// depends only on the value to be encoded.  Such overloads should be added in
// a future version.
//
// Q: Why is it computationally inefficient not to have overloads for smaller
//    data types?
//
// The first step of encoding a value in Apertium binary format is to determine
// its "class".  When interpreted as an unsigned integer, a value between 0 and
// 2**7 - 1 (inclusive) is in the 0th class.  A value between 2**7 and 2**14 -
// 1 (inclusive) is in the 1st class.  This pattern continues up to and
// including a value between 2**49 and 2**56 - 1 (inclusive) being in the 7th
// class, but a value any larger, between 2**56 and 2**64 - 1 (inclusive) is in
// the 8th class.  While a 64-bit value could be in any one of those classes,
// an 8-bit value, for example, could be in only the 0th or 1st class.  Thus,
// when encoding an 8-bit value, the encoding function may make an unnecessary
// check to determine whether the value is in the 2nd class -- this would
// happen if the value were between 2**7 and 2**8 - 1 (inclusive).  It is
// similar for other values less than 64 bits in size.[1]
//
//    1.  ^ It would not be computationally inefficient not to have an overload
//          for a data type between 57 and 63 bits in size, were such a data
//          type to exist.
//
// Q: What is the significance of a value's class, and
// Q: how does a value affect the number of bytes used to encode it?
//
// Apertium binary format is designed to encode an individual value in as few
// bytes as possible, often doing so in fewer bytes than are used to store the
// value in memory.  What allows this is assuming that any unwritten bits of a
// value are equal to zero and more significant than all of the value's other
// bits.  Thus, if a value to be encoded has any contiguous, most-significant
// bits that are equal to zero ("leading zeros"), the encoding function may be
// able to leave some or all of them unwritten.  How many of a value's leading
// zeros that the encoding function leaves unwritten depends on how many
// leading zeros that the value has, as the encoding function must encode a
// value in an (integral) number of bytes.  When the encoding function leaves
// some or all of a value's leading zeros unwritten, it does not need to write
// as many bytes, and the decoding function will not need to read as many bytes
// to decode the value.  Since I/O is a bottleneck for speed, this makes
// encoding and decoding a value in Apertium binary format quite fast --
// perhaps faster than writing and reading the value without any encoding,
// respectively.[2]
//
// A value with more leading zeros than another value is, with both values
// interpreted as unsigned integers, less than the other value.  Of course, a
// lesser unsigned integer does not always have more leading zeros than a
// greater unsigned integer, but, if an unsigned integer is sufficiently lesser
// than another unsigned integer, this is true.  A value encoded in fewer bytes
// than than another value has more leading zeros than the other value.  Of
// course, a value with more leading zeros is not always encoded in fewer bytes
// than a value with with fewer leading zeros, but, likewise, if a value has
// sufficiently more leading zeros than another value, this is true.
//
// A value in the i - 1th class has sufficiently more leading zeros than a
// value in the ith class such that the value is encoded in 1 fewer byte.  1
// byte is used to encode a value in the 0th class.  This function always reads
// at least 1 byte; then, if i is greater than 0, the function reads i bytes.
// Thus, this function always reads a total of i + 1 bytes to decode a value in
// the ith class.
//
//    2.  ^ This hypothesis is untested.  This should come down to whether the
//          time saved by writing fewer bytes is greater than the time required
//          to format the value for writing.
//
// Q: How does this function determine a value's class from the first byte that
//    the function reads?
//
// The number of contiguous, most-significant bits equal to one ("leading
// ones") in the first byte is the value's class.  This design is similar to
// that of UTF-8.  When there are between 0 and 7 leading ones in the first
// byte, the bit immediately less significant than the least significant of the
// leading ones is equal to zero ("terminating zero").  When there are 0
// leading ones in the first byte, the terminating zero is the first byte's
// most-significant bit.  All of the first byte's less-significant bits and all
// of the bits in the following i bytes are the literal bits of the value.
// This entire sequence of bits is bytewise big-endian (and bitwise
// little-endian).  However, when there are 8 leading ones in the first byte --
// and thus each of the first byte's bits is equal to one -- there is no
// terminating zero. The 8 following bytes are the literal bytes of the value
// (and, again, are bytewise big-endian and bitwise little-endian).
//
// When there are 0 leading ones in the first byte, as in the following
// example,
//
//   0b0.......
//
// there is a terminating zero followed by 7 literal bits of a value.  Recall
// that a value with 0 leading ones is in the 0th class and thus between 0 and
// 2**7 - 1 (inclusive), which is the range of numbers that ASCII encodes.  One
// could also observe that there are 7 literal bits of a value, and 7 bits can
// store an unsigned integer between 0 and 2**7 - 1 (inclusive).  Thus,
// Apertium binary format encodes all of the integers that ASCII encodes as
// ASCII encodes them.  When there is 1 leading one,
//
//   0b10...... ........
//
// there is a terminating zero followed by 6 literal bits and then 1 literal
// byte of a value.  When there are 7 leading ones,
//
//   0b11111110 ........ ........ ........ ........ ........ ........ ........
//
// there is a terminating zero followed by 7 literal bytes of a value.
//
// Q: Why is there no terminating zero when there are 8 leading ones in the
//    first byte?
//
// Since Apertium binary format can encode a value up to and including 64 bits
// in size, the 8 following bytes can store any value that one may encode.
// Thus, there is no 9th class, and so the first byte needs to indicate only
// that the value is not in the 7th class.  If the first byte is `0b11111110`,
// the value is in the 7th class, and if the any of the leading ones were
// instead equal to zero, that would indicate that the value is in a lesser
// class.  This leaves only `0b11111111`.
auto read(std::istream &is, std::uint64_t &x) -> decltype(is);

namespace {

// Return the maximum value (interpreted as an unsigned integer) of a first
// byte of an Apertium binary format encoding for which the given leading ones
// are still the leadings ones.
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
