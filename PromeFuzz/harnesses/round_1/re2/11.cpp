// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::error_code at re2.h:320:13 in re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
// re2::RE2::error_arg at re2.h:324:22 in re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Create a null-terminated pattern string from fuzzer input
  std::string pattern(reinterpret_cast<const char*>(Data), Size);
  
  // Construct RE2 object with the pattern
  re2::RE2 regex(pattern.c_str());
  
  // Call functions in specified order
  bool is_ok = regex.ok();
  re2::RE2::ErrorCode code = regex.error_code();
  const std::string& error_str = regex.error();
  const std::string& error_arg_str = regex.error_arg();
  const std::string& error_str_again = regex.error();
  
  // Use the results to avoid dead code elimination
  static volatile bool dummy = false;
  dummy = is_ok || (code != re2::RE2::NoError) || !error_str.empty() || 
          !error_arg_str.empty() || !error_str_again.empty();
  
  return 0;
}