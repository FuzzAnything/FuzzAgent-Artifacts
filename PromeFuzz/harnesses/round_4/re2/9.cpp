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

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Set maximum global replace count for fuzzing safety
  int replace_count = static_cast<int>(Data[0] % 256);
  re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(replace_count);

  // Create pattern from fuzzer input
  std::string pattern(reinterpret_cast<const char*>(Data), Size);
  
  // Create RE2 object with the pattern
  re2::RE2 re(pattern.c_str());
  
  // Check if regex compiled successfully
  bool is_ok = re.ok();
  
  // Get internal regexp pointer (call twice as requested)
  re2::Regexp* regexp1 = re.Regexp();
  re2::Regexp* regexp2 = re.Regexp();
  
  // Get program size
  int program_size = re.ProgramSize();
  
  // Use the variables to avoid optimization
  (void)is_ok;
  (void)regexp1;
  (void)regexp2;
  (void)program_size;
  
  return 0;
}