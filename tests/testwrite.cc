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

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>

#define BOOST_TEST_MODULE testwrite
#include <boost/test/included/unit_test.hpp>

#include "write.h"

#if ENABLE_DEBUG

template <class T> auto ord(const T &c) -> decltype(+c) { return +c; }

template <class InputIterator>
void print(InputIterator first, InputIterator last) {
  const auto &flags = std::cerr.setf(std::cerr.hex, std::cerr.basefield);

  // Set next because it only affects output when the field width of
  // ``std::cerr`` is set.  The field width is reset after each ASCII value is
  // printed, so, while this does have an effect on the representation of all
  // the ASCII values, it does not affect them "contiguously" -- it has an
  // effect on one ASCII value, then no longer affects the output, and then
  // affects another ASCII value.
  const auto &fill = std::cerr.fill('0');

  // A space must be printed after every other ASCII value -- except the last
  // one.  Therefore, the last ASCII value must be printed after the loop, and
  // the loop must print each of the preceeding pairs of ASCII values, each
  // pair followed by a space.  The loop must end with one element remaining.
  --last;

  for (; first != last; ++first) {
    std::cerr << std::setw(2) << ord(*first);
    ++first;

    if (first == last)
      break;

    std::cerr << std::setw(2) << ord(*first);
    std::cerr << ' ';
  }

  std::cerr << std::setw(2) << ord(*first);

  // Reset the properties of ``std::cerr`` in the order in which they were set.
  std::cerr.fill(fill);
  std::cerr.flags(flags);
}

#endif

template <std::size_t N>
bool test_write_write(const std::uint64_t &x, std::array<unsigned char, N> s) {
  std::basic_stringstream<unsigned char> os;
  lttoolbox::write(os, x);
  const std::basic_string<unsigned char> &str = os.str();

#if ENABLE_DEBUG

  std::cerr << "\n"
               "diff s str\n";

#endif

  if (std::equal(str.cbegin(), str.cend(), s.cbegin(), s.cend()))
    return true;

#if ENABLE_DEBUG

  std::cerr << "< ";
  print(s.cbegin(), s.cend());
  std::cerr << "\n"
               "---\n"
               "> ";
  print(str.cbegin(), str.cend());
  std::cerr << '\n';

#endif

  return false;
}

BOOST_AUTO_TEST_CASE(testwrite_test_write_write) {
  BOOST_CHECK(test_write_write(0x00, std::array<unsigned char, 1>({0x00})));
  BOOST_CHECK(test_write_write(0x7F, std::array<unsigned char, 1>({0x7f})));
  BOOST_CHECK(
      test_write_write(0x80, std::array<unsigned char, 2>({0x80, 0x80})));
  BOOST_CHECK(
      test_write_write(0x20'40, std::array<unsigned char, 2>({0xa0, 0x40})));
  BOOST_CHECK(
      test_write_write(0x3f'ff, std::array<unsigned char, 2>({0xbf, 0xff})));
  BOOST_CHECK(test_write_write(
      0x40'00, std::array<unsigned char, 3>({0xc0, 0x40, 0x00})));
  BOOST_CHECK(test_write_write(
      0x10'20'40, std::array<unsigned char, 3>({0xd0, 0x20, 0x40})));
  BOOST_CHECK(test_write_write(
      0x1f'ff'ff, std::array<unsigned char, 3>({0xdf, 0xff, 0xff})));
  BOOST_CHECK(test_write_write(
      0x20'00'00, std::array<unsigned char, 4>({0xe0, 0x20, 0x00, 0x00})));
  BOOST_CHECK(test_write_write(
      0x08'10'20'40, std::array<unsigned char, 4>({0xe8, 0x10, 0x20, 0x40})));
  BOOST_CHECK(test_write_write(
      0x0f'ff'ff'ff, std::array<unsigned char, 4>({0xef, 0xff, 0xff, 0xff})));
  BOOST_CHECK(test_write_write(
      0x10'00'00'00,
      std::array<unsigned char, 5>({0xf0, 0x10, 0x00, 0x00, 0x00})));
  BOOST_CHECK(test_write_write(
      0x04'08'10'20'40,
      std::array<unsigned char, 5>({0xf4, 0x08, 0x10, 0x20, 0x40})));
  BOOST_CHECK(test_write_write(
      0x07'ff'ff'ff'ff,
      std::array<unsigned char, 5>({0xf7, 0xff, 0xff, 0xff, 0xff})));
  BOOST_CHECK(test_write_write(
      0x08'00'00'00'00,
      std::array<unsigned char, 6>({0xf8, 0x08, 0x00, 0x00, 0x00, 0x00})));
  BOOST_CHECK(test_write_write(
      0x02'04'08'10'20'40,
      std::array<unsigned char, 6>({0xfa, 0x04, 0x08, 0x10, 0x20, 0x40})));
  BOOST_CHECK(test_write_write(
      0x03'ff'ff'ff'ff'ff,
      std::array<unsigned char, 6>({0xfb, 0xff, 0xff, 0xff, 0xff, 0xff})));
  BOOST_CHECK(test_write_write(
      0x04'00'00'00'00'00, std::array<unsigned char, 7>(
                               {0xfc, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00})));
  BOOST_CHECK(test_write_write(
      0x01'02'04'08'10'20'40, std::array<unsigned char, 7>(
                                  {0xfd, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40})));
  BOOST_CHECK(test_write_write(
      0x01'ff'ff'ff'ff'ff'ff, std::array<unsigned char, 7>(
                                  {0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff})));
  BOOST_CHECK(
      test_write_write(0x02'00'00'00'00'00'00,
                       std::array<unsigned char, 8>(
                           {0xfe, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));
  BOOST_CHECK(
      test_write_write(0x80'01'02'04'08'10'20,
                       std::array<unsigned char, 8>(
                           {0xfe, 0x80, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20})));
  BOOST_CHECK(
      test_write_write(0xff'ff'ff'ff'ff'ff'ff,
                       std::array<unsigned char, 8>(
                           {0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff})));
  BOOST_CHECK(test_write_write(
      0x01'00'00'00'00'00'00'00,
      std::array<unsigned char, 9>(
          {0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));
  BOOST_CHECK(test_write_write(
      0x80'01'02'04'08'10'20'40,
      std::array<unsigned char, 9>(
          {0xff, 0x80, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40})));
  BOOST_CHECK(test_write_write(
      0xff'ff'ff'ff'ff'ff'ff'ff,
      std::array<unsigned char, 9>(
          {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff})));
}
