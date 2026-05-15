// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count at re2.cc:48:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Regexp at re2.h:340:16 in re2.h
// re2::RE2::Regexp at re2.h:340:16 in re2.h
// re2::RE2::ProgramSize at re2.cc:323:10 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Set maximum global replace count using first byte of input
  int max_replace = static_cast<int>(Data[0]);
  re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(max_replace);

  // Use the remaining input as pattern string
  const char* pattern_data = reinterpret_cast<const char*>(Data + 1);
  size_t pattern_size = Size - 1;
  
  // Ensure pattern is null-terminated
  std::string pattern(pattern_data, pattern_size);
  
  // Create RE2 object with default options
  re2::RE2::Options options;
  re2::RE2 re(pattern, options);
  
  // Check if regex compiled successfully
  bool is_ok = re.ok();
  
  // Get internal regexp pointer (call twice as specified)
  re2::Regexp* regexp1 = re.Regexp();
  re2::Regexp* regexp2 = re.Regexp();
  
  // Get program size
  int program_size = re.ProgramSize();
  
  // Use the results in some way to avoid optimization
  if (is_ok && regexp1 != nullptr && regexp2 != nullptr && program_size > 0) {
    // Do nothing, just prevent optimization
    volatile int dummy = program_size;
    (void)dummy;
  }
  
  return 0;
}