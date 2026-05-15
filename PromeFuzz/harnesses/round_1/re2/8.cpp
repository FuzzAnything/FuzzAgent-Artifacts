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
#include "re2/re2.h"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Set maximum global replace count using first byte
  int max_replace = static_cast<int>(Data[0]);
  re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(max_replace);

  // Create pattern from remaining data
  const char* pattern_data = reinterpret_cast<const char*>(Data + 1);
  size_t pattern_size = Size - 1;
  
  // Ensure pattern is null-terminated for safety
  char* null_terminated_pattern = new char[pattern_size + 1];
  std::memcpy(null_terminated_pattern, pattern_data, pattern_size);
  null_terminated_pattern[pattern_size] = '\0';
  
  // Create RE2 object
  re2::RE2 re(null_terminated_pattern);
  
  // Call the API functions in specified order
  bool is_ok = re.ok();
  
  re2::Regexp* regexp1 = re.Regexp();
  re2::Regexp* regexp2 = re.Regexp();
  
  int program_size = re.ProgramSize();
  
  // Clean up
  delete[] null_terminated_pattern;
  
  // Use the results to avoid optimization
  (void)is_ok;
  (void)regexp1;
  (void)regexp2;
  (void)program_size;
  
  return 0;
}