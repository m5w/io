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

#include "read.h"
#include "write.h"

static inline unsigned int ord(const char &c);

template <class InputIterator>
static void print(InputIterator first, InputIterator last);

template <std::size_t n>
static bool test_write_x_s(const std::uint64_t x,
                           const std::array<char, n> &s);

template <std::size_t n>
static void test_read_s_x(const std::array<char, n> &s, const std::uint64_t x);

BOOST_AUTO_TEST_CASE(test_write) {
  BOOST_CHECK(test_write_x_s(0x00ull, std::array<char, 1ull>({'\x00'})));
  BOOST_CHECK(test_write_x_s(0x40ull, std::array<char, 1ull>({'\x40'})));
  BOOST_CHECK(test_write_x_s(0x7full, std::array<char, 1ull>({'\x7f'})));
  BOOST_CHECK(
      test_write_x_s(0x80ull, std::array<char, 2ull>({'\x80', '\x80'})));
  BOOST_CHECK(
      test_write_x_s(0x20'10ull, std::array<char, 2ull>({'\xa0', '\x10'})));
  BOOST_CHECK(
      test_write_x_s(0x3f'ffull, std::array<char, 2ull>({'\xbf', '\xff'})));
  BOOST_CHECK(test_write_x_s(
      0x40'00ull, std::array<char, 3ull>({'\xc0', '\x40', '\x00'})));
  BOOST_CHECK(test_write_x_s(
      0x10'08'04ull, std::array<char, 3ull>({'\xd0', '\x08', '\x04'})));
  BOOST_CHECK(test_write_x_s(
      0x1f'ff'ffull, std::array<char, 3ull>({'\xdf', '\xff', '\xff'})));
  BOOST_CHECK(test_write_x_s(
      0x20'00'00ull,
      std::array<char, 4ull>({'\xe0', '\x20', '\x00', '\x00'})));
  BOOST_CHECK(test_write_x_s(
      0x08'04'02'01ull,
      std::array<char, 4ull>({'\xe8', '\x04', '\x02', '\x01'})));
  BOOST_CHECK(test_write_x_s(
      0x0f'ff'ff'ffull,
      std::array<char, 4ull>({'\xef', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_x_s(
      0x10'00'00'00ull,
      std::array<char, 5ull>({'\xf0', '\x10', '\x00', '\x00', '\x00'})));
  BOOST_CHECK(test_write_x_s(
      0x04'02'01'80'40ull,
      std::array<char, 5ull>({'\xf4', '\x02', '\x01', '\x80', '\x40'})));
  BOOST_CHECK(test_write_x_s(
      0x07'ff'ff'ff'ffull,
      std::array<char, 5ull>({'\xf7', '\xff', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_x_s(
      0x08'00'00'00'00ull, std::array<char, 6ull>({'\xf8', '\x08', '\x00',
                                                   '\x00', '\x00', '\x00'})));
  BOOST_CHECK(
      test_write_x_s(0x02'01'80'40'20'10ull,
                     std::array<char, 6ull>(
                         {'\xfa', '\x01', '\x80', '\x40', '\x20', '\x10'})));
  BOOST_CHECK(
      test_write_x_s(0x03'ff'ff'ff'ff'ffull,
                     std::array<char, 6ull>(
                         {'\xfb', '\xff', '\xff', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(
      test_write_x_s(0x04'00'00'00'00'00ull,
                     std::array<char, 7ull>({'\xfc', '\x04', '\x00', '\x00',
                                             '\x00', '\x00', '\x00'})));
  BOOST_CHECK(
      test_write_x_s(0x01'80'40'20'10'08'04ull,
                     std::array<char, 7ull>({'\xfd', '\x80', '\x40', '\x20',
                                             '\x10', '\x08', '\x04'})));
  BOOST_CHECK(
      test_write_x_s(0x01'ff'ff'ff'ff'ff'ffull,
                     std::array<char, 7ull>({'\xfd', '\xff', '\xff', '\xff',
                                             '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_x_s(
      0x02'00'00'00'00'00'00ull,
      std::array<char, 8ull>(
          {'\xfe', '\x02', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'})));
  BOOST_CHECK(test_write_x_s(
      0x80'40'20'10'08'04'02ull,
      std::array<char, 8ull>(
          {'\xfe', '\x80', '\x40', '\x20', '\x10', '\x08', '\x04', '\x02'})));
  BOOST_CHECK(test_write_x_s(
      0xff'ff'ff'ff'ff'ff'ffull,
      std::array<char, 8ull>(
          {'\xfe', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff'})));
  BOOST_CHECK(test_write_x_s(
      0x01'00'00'00'00'00'00'00ull,
      std::array<char, 9ull>({'\xff', '\x01', '\x00', '\x00', '\x00', '\x00',
                              '\x00', '\x00', '\x00'})));
  BOOST_CHECK(test_write_x_s(
      0x80'40'20'10'08'04'02'01ull,
      std::array<char, 9ull>({'\xff', '\x80', '\x40', '\x20', '\x10', '\x08',
                              '\x04', '\x02', '\x01'})));
  BOOST_CHECK(test_write_x_s(
      0xff'ff'ff'ff'ff'ff'ff'ffull,
      std::array<char, 9ull>({'\xff', '\xff', '\xff', '\xff', '\xff', '\xff',
                              '\xff', '\xff', '\xff'})));
}

BOOST_AUTO_TEST_CASE(test_read) {
  test_read_s_x(std::array<char, 1ull>({'\x00'}), 0x00ull);
  test_read_s_x(std::array<char, 1ull>({'\x40'}), 0x40ull);
  test_read_s_x(std::array<char, 1ull>({'\x7f'}), 0x7full);
  test_read_s_x(std::array<char, 2ull>({'\x80', '\x80'}), 0x80ull);
  test_read_s_x(std::array<char, 2ull>({'\xa0', '\x10'}), 0x20'10ull);
  test_read_s_x(std::array<char, 2ull>({'\xbf', '\xff'}), 0x3f'ffull);
  test_read_s_x(std::array<char, 3ull>({'\xc0', '\x40', '\x00'}), 0x40'00ull);
  test_read_s_x(std::array<char, 3ull>({'\xd0', '\x08', '\x04'}),
                0x10'08'04ull);
  test_read_s_x(std::array<char, 3ull>({'\xdf', '\xff', '\xff'}),
                0x1f'ff'ffull);
  test_read_s_x(std::array<char, 4ull>({'\xe0', '\x20', '\x00', '\x00'}),
                0x20'00'00ull);
  test_read_s_x(std::array<char, 4ull>({'\xe8', '\x04', '\x02', '\x01'}),
                0x08'04'02'01ull);
  test_read_s_x(std::array<char, 4ull>({'\xef', '\xff', '\xff', '\xff'}),
                0x0f'ff'ff'ffull);
  test_read_s_x(
      std::array<char, 5ull>({'\xf0', '\x10', '\x00', '\x00', '\x00'}),
      0x10'00'00'00ull);
  test_read_s_x(
      std::array<char, 5ull>({'\xf4', '\x02', '\x01', '\x80', '\x40'}),
      0x04'02'01'80'40ull);
  test_read_s_x(
      std::array<char, 5ull>({'\xf7', '\xff', '\xff', '\xff', '\xff'}),
      0x07'ff'ff'ff'ffull);
  test_read_s_x(
      std::array<char, 6ull>({'\xf8', '\x08', '\x00', '\x00', '\x00', '\x00'}),
      0x08'00'00'00'00ull);
  test_read_s_x(
      std::array<char, 6ull>({'\xfa', '\x01', '\x80', '\x40', '\x20', '\x10'}),
      0x02'01'80'40'20'10ull);
  test_read_s_x(
      std::array<char, 6ull>({'\xfb', '\xff', '\xff', '\xff', '\xff', '\xff'}),
      0x03'ff'ff'ff'ff'ffull);
  test_read_s_x(std::array<char, 7ull>(
                    {'\xfc', '\x04', '\x00', '\x00', '\x00', '\x00', '\x00'}),
                0x04'00'00'00'00'00ull);
  test_read_s_x(std::array<char, 7ull>(
                    {'\xfd', '\x80', '\x40', '\x20', '\x10', '\x08', '\x04'}),
                0x01'80'40'20'10'08'04ull);
  test_read_s_x(std::array<char, 7ull>(
                    {'\xfd', '\xff', '\xff', '\xff', '\xff', '\xff', '\xff'}),
                0x01'ff'ff'ff'ff'ff'ffull);
  test_read_s_x(std::array<char, 8ull>({'\xfe', '\x02', '\x00', '\x00', '\x00',
                                        '\x00', '\x00', '\x00'}),
                0x02'00'00'00'00'00'00ull);
  test_read_s_x(std::array<char, 8ull>({'\xfe', '\x80', '\x40', '\x20', '\x10',
                                        '\x08', '\x04', '\x02'}),
                0x80'40'20'10'08'04'02ull);
  test_read_s_x(std::array<char, 8ull>({'\xfe', '\xff', '\xff', '\xff', '\xff',
                                        '\xff', '\xff', '\xff'}),
                0xff'ff'ff'ff'ff'ff'ffull);
  test_read_s_x(std::array<char, 9ull>({'\xff', '\x01', '\x00', '\x00', '\x00',
                                        '\x00', '\x00', '\x00', '\x00'}),
                0x01'00'00'00'00'00'00'00ull);
  test_read_s_x(std::array<char, 9ull>({'\xff', '\x80', '\x40', '\x20', '\x10',
                                        '\x08', '\x04', '\x02', '\x01'}),
                0x80'40'20'10'08'04'02'01ull);
  test_read_s_x(std::array<char, 9ull>({'\xff', '\xff', '\xff', '\xff', '\xff',
                                        '\xff', '\xff', '\xff', '\xff'}),
                0xff'ff'ff'ff'ff'ff'ff'ffull);
}

unsigned int ord(const char &c) { return static_cast<unsigned char>(c); }

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

  std::size_t length = last - first;

  if (length % 2ull == 1ull) {
    if (length == 1ull) {
      std::cerr << std::setw(2) << ord(*first) << std::setw(0);
      goto finally;
    }

    std::cerr << std::setw(2) << ord(*first) << std::setw(0) << ' ';
    ++first;
  }

  // A space must be printed after every other ASCII value -- except the last
  // one.  Therefore, the last ASCII value must be printed after the loop, and
  // the loop must print each of the preceeding pairs of ASCII values, each
  // pair followed by a space.  The loop must end with two elements remaining.
  last -= 2ull;

  for (; first != last; first += 2ull) {
    std::cerr << std::setw(2) << ord(*first) << ord(*(first + 1ull))
              << std::setw(0) << ' ';
  }

  std::cerr << std::setw(2) << ord(*first) << ord(*(first + 1ull))
            << std::setw(0);

finally:

  // Reset the properties of ``std::cerr`` in the order in which they were set.
  std::cerr.fill(fill);
  std::cerr.flags(flags);
}

template <std::size_t n>
bool test_write_x_s(const std::uint64_t x, const std::array<char, n> &s) {
  std::stringstream os;
  lttoolbox::write(os, x);
  const std::string &str = os.str();

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

template <std::size_t n>
void test_read_s_x(const std::array<char, n> &s, const std::uint64_t x) {
  std::stringstream is;
  is.write(s.data(), n);
  BOOST_CHECK_EQUAL(x, lttoolbox::read(is));
}
