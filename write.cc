#include "write.h"

namespace lttoolbox {

template <std::size_t n>
std::ostream &Write<n>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr << "-----BEGIN std::ostream &\n"
               "Write<"
            << n << ">::write(std::ostream &os, const std::uint64_t &x)-----\n"
                    "x = "
            << x << "\n";

#endif

  if (x > maximum_x)
    return Write<n + 1>::write(os, x);

  std::cerr << "n = " << n << "\n"
                              "x = "
            << x << "\n";

#if ENABLE_DEBUG

  std::cerr << "-----END std::ostream &\n"
               "Write<"
            << n << ">::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os;
}

std::ostream &Write<1>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr
      << "-----BEGIN std::ostream &\n"
         "Write<1>::write(std::ostream &os, const std::uint64_t &x)-----\n"
         "x = "
      << x << "\n";

#endif

  if (x > maximum_x)
    return Write<2>::write(os, x);

  std::cerr << "n = 1\n"
               "x = "
            << x << "\n";

#if ENABLE_DEBUG

  std::cerr
      << "-----END std::ostream &\n"
         "Write<1>::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os;
}

std::ostream &Write<9>::write(std::ostream &os, const std::uint64_t &x) {

#if ENABLE_DEBUG

  std::cerr
      << "-----BEGIN std::ostream &\n"
         "Write<9>::write(std::ostream &os, const std::uint64_t &x)-----\n"
         "x = "
      << x << "\n";

#endif

  std::cerr << "n = 9\n"
               "x = "
            << x << "\n";

#if ENABLE_DEBUG

  std::cerr
      << "-----END std::ostream &\n"
         "Write<9>::write(std::ostream &os, const std::uint64_t &x)-----\n";

#endif

  return os;
}

std::ostream &write(std::ostream &os, const std::uint64_t &x) {
  return Write<1>::write(os, x);
}

}  // end namespace lttoolbox
