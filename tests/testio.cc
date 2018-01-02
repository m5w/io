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

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>

#define BOOST_TEST_MODULE testio
#include <boost/test/included/unit_test.hpp>

#include "decode.h"
#include "encode.h"

static inline unsigned int ord(const char &c);
template <class InputIterator>
static auto print(std::ostream &os, InputIterator first, InputIterator last)
    -> decltype(os);
static auto print(std::ostream &os, const std::uint64_t x) -> decltype(os);
template <std::size_t n>
static inline void test(const std::uint64_t x, const std::array<char, n> &s);
template <std::size_t n>
static bool test_encode(const std::uint64_t x, const std::array<char, n> &s);
template <std::size_t n>
static bool test_decode(const std::array<char, n> &s, const std::uint64_t x);

BOOST_AUTO_TEST_CASE(class0_minimum_x) {
  test(0x00ull, std::array<char, 1ull>({'\x00'}));
}

BOOST_AUTO_TEST_CASE(class0_bytewise_endianness) {
  test(0x40ull, std::array<char, 1ull>({'\x40'}));
}

BOOST_AUTO_TEST_CASE(class0_maximum_x) {
  test(0x7full, std::array<char, 1ull>({'\x7f'}));
}

BOOST_AUTO_TEST_CASE(class1_minimum_x) {
  test(0x80ull, std::array<char, 2ull>({'\x80', '\x80'}));
}

BOOST_AUTO_TEST_CASE(class1_bytewise_endianness) {
  test(0x20'10ull, std::array<char, 2ull>({'\xa0', '\x10'}));
}

BOOST_AUTO_TEST_CASE(class1_maximum_x) {
  test(0x3f'ffull, std::array<char, 2ull>({'\xbf', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class2_minimum_x) {
  test(0x40'00ull, std::array<char, 3ull>({'\xc0', '\x40', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class2_bytewise_endianness) {
  test(0x10'08'04ull, std::array<char, 3ull>({'\xd0', '\x08', '\x04'}));
}

BOOST_AUTO_TEST_CASE(class2_maximum_x) {
  test(0x1f'ff'ffull, std::array<char, 3ull>({'\xdf', '\xff', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class3_minimum_x) {
  test(0x20'00'00ull,
       std::array<char, 4ull>({'\xe0', '\x20', '\x00', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class3_bytewise_endianness) {
  test(0x08'04'02'01ull,
       std::array<char, 4ull>({'\xe8', '\x04', '\x02', '\x01'}));
}

BOOST_AUTO_TEST_CASE(class3_maximum_x) {
  test(0x0f'ff'ff'ffull,
       std::array<char, 4ull>({'\xef', '\xff', '\xff', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class4_minimum_x) {
  test(0x10'00'00'00ull,
       std::array<char, 5ull>({'\xf0', '\x10', '\x00', '\x00', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class4_bytewise_endianness) {
  test(0x04'02'01'80'40ull,
       std::array<char, 5ull>({'\xf4', '\x02', '\x01', '\x80', '\x40'}));
}

BOOST_AUTO_TEST_CASE(class4_maximum_x) {
  test(0x07'ff'ff'ff'ffull,
       std::array<char, 5ull>({'\xf7', '\xff', '\xff', '\xff', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class5_minimum_x) {
  test(0x08'00'00'00'00ull, std::array<char, 6ull>({'\xf8', '\x08', '\x00',
                                                    '\x00', '\x00', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class5_bytewise_endianness) {
  test(0x02'01'80'40'20'10ull,
       std::array<char, 6ull>(
           {'\xfa', '\x01', '\x80', '\x40', '\x20', '\x10'}));
}

BOOST_AUTO_TEST_CASE(class5_maximum_x) {
  test(0x03'ff'ff'ff'ff'ffull,
       std::array<char, 6ull>(
           {'\xfb', '\xff', '\xff', '\xff', '\xff', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class6_minimum_x) {
  test(0x04'00'00'00'00'00ull,
       std::array<char, 7ull>(
           {'\xfc', '\x04', '\x00', '\x00', '\x00', '\x00', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class6_bytewise_endianness) {
  test(0x01'80'40'20'10'08'04ull,
       std::array<char, 7ull>(
           {'\xfd', '\x80', '\x40', '\x20', '\x10', '\x08', '\x04'}));
}

BOOST_AUTO_TEST_CASE(class6_maximum_x) {
  test(0x01'ff'ff'ff'ff'ff'ffull,
       std::array<char, 7ull>(
           {'\xfd', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class7_minimum_x) {
  test(0x02'00'00'00'00'00'00ull,
       std::array<char, 8ull>(
           {'\xfe', '\x02', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class7_bytewise_endianness) {
  test(0x80'40'20'10'08'04'02ull,
       std::array<char, 8ull>(
           {'\xfe', '\x80', '\x40', '\x20', '\x10', '\x08', '\x04', '\x02'}));
}

BOOST_AUTO_TEST_CASE(class7_maximum_x) {
  test(0xff'ff'ff'ff'ff'ff'ffull,
       std::array<char, 8ull>(
           {'\xfe', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff'}));
}

BOOST_AUTO_TEST_CASE(class8_minimum_x) {
  test(0x01'00'00'00'00'00'00'00ull,
       std::array<char, 9ull>({'\xff', '\x01', '\x00', '\x00', '\x00', '\x00',
                               '\x00', '\x00', '\x00'}));
}

BOOST_AUTO_TEST_CASE(class8_bytewise_endianness) {
  test(0x80'40'20'10'08'04'02'01ull,
       std::array<char, 9ull>({'\xff', '\x80', '\x40', '\x20', '\x10', '\x08',
                               '\x04', '\x02', '\x01'}));
}

BOOST_AUTO_TEST_CASE(class8_maximum_x) {
  test(0xff'ff'ff'ff'ff'ff'ff'ffull,
       std::array<char, 9ull>({'\xff', '\xff', '\xff', '\xff', '\xff', '\xff',
                               '\xff', '\xff', '\xff'}));
}

unsigned int ord(const char &c) { return static_cast<unsigned char>(c); }

template <class InputIterator>
auto print(std::ostream &os, InputIterator first, InputIterator last)
    -> decltype(os) {
  if (first == last)
    return os;

  const auto flags{os.setf(os.hex, os.basefield)};

  // Set this next because it affects output only when the field width of `os`
  // is set.  The field width is reset after each ASCII value is printed, so,
  // while this affects the representation of all of the ASCII values, it does
  // not do so "contiguously" -- that is, it affects the representation of one
  // ASCII value, then no longer affects the output, and then affects the
  // representation of another ASCII value.
  const auto fill{os.fill('0')};

  if ((last - first) % 2ull == 1ull) {
    --last;

    for (; first != last; first += 2ull)
      os << std::setw(2) << ord(*first) << std::setw(2) << ord(*(first + 1))
         << ' ';

    os << std::setw(2) << ord(*first);
  } else {
    last -= 2ull;

    for (; first != last; first += 2ull)
      os << std::setw(2) << ord(*first) << std::setw(2) << ord(*(first + 1))
         << ' ';

    os << std::setw(2) << ord(*first) << std::setw(2) << ord(*(first + 1));
  }

  // Reset the properties of `os` in the order in which they were set.
  os.fill(fill);
  os.flags(flags);
  return os;
}

static auto print(std::ostream &os, const std::uint64_t x) -> decltype(os) {
  const auto &flags{os.setf(os.hex, os.basefield)};
  os.setf(os.showbase);
  const auto &fill{os.fill('0')};
  os << x;
  os.fill(fill);
  os.flags(flags);
  return os;
}

template <std::size_t n>
void test(const std::uint64_t x, const std::array<char, n> &s) {
  BOOST_CHECK(test_encode(x, s));
  BOOST_CHECK(test_decode(s, x));
}

template <std::size_t n>
bool test_encode(const std::uint64_t x, const std::array<char, n> &s) {
  std::ostringstream os{};
  lttoolbox::encode(os, x);
  const std::string &encoded{os.str()};

#if ENABLE_DEBUG

  std::cerr << "diff s encoded\n";

#endif

  if (std::equal(encoded.cbegin(), encoded.cend(), s.cbegin(), s.cend()))
    return true;

#if ENABLE_DEBUG

  if (encoded.size() == 0ull) {
    std::cerr << "1d0\n"
                 "< ";
    print(std::cerr, s.cbegin(), s.cend());
    std::cerr << '\n';
  } else {
    std::cerr << "1c1\n"
                 "< ";
    print(std::cerr, s.cbegin(), s.cend());
    std::cerr << "\n"
                 "---\n"
                 "> ";
    print(std::cerr, encoded.cbegin(), encoded.cend());
    std::cerr << '\n';
  }

#endif

  return false;
}

template <std::size_t n>
bool test_decode(const std::array<char, n> &s, const std::uint64_t x) {
  std::istringstream is{{s.data(), n}};
  std::uint64_t decoded{0ull};
  lttoolbox::decode(is, decoded);

#if ENABLE_DEBUG

  std::cerr << "diff x decoded\n";

#endif

  if (decoded == x)
    return true;

#if ENABLE_DEBUG

  std::cerr << "1c1\n"
               "< ";
  print(std::cerr, x);
  std::cerr << "\n"
               "---\n"
               "> ";
  print(std::cerr, decoded);
  std::cerr << '\n';

#endif

  return false;
}
