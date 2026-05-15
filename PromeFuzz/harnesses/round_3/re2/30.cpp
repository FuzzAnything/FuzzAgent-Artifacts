// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Use first byte to derive boolean parameters
  uint8_t config = Data[0];
  bool perl_classes_setting = (config & 0x01) != 0;
  bool word_boundary_setting = (config & 0x02) != 0;
  bool case_sensitive_setting = (config & 0x04) != 0;
  bool literal_setting = (config & 0x08) != 0;
  
  // Use second byte for canned options selection
  uint8_t canned_option = 0;
  if (Size > 1) {
    canned_option = Data[1] % 4; // 4 canned options: 0-3
  }

  // Create Options object with canned option
  re2::RE2::CannedOptions canned = static_cast<re2::RE2::CannedOptions>(canned_option);
  re2::RE2::Options options(canned);

  // Set options based on fuzzed data
  options.set_perl_classes(perl_classes_setting);
  options.set_word_boundary(word_boundary_setting);
  options.set_case_sensitive(case_sensitive_setting);
  options.set_literal(literal_setting);

  // Get option values (exercise getters)
  bool perl_classes_result = options.perl_classes();
  bool posix_syntax_result = options.posix_syntax();

  // Use remaining data as pattern string if available
  if (Size > 2) {
    size_t pattern_size = Size - 2;
    const char* pattern_data = reinterpret_cast<const char*>(Data + 2);
    
    // Ensure pattern is null-terminated and doesn't contain null bytes in the middle
    std::string pattern;
    for (size_t i = 0; i < pattern_size; i++) {
      if (pattern_data[i] == '\0') {
        break;
      }
      pattern.push_back(pattern_data[i]);
    }
    
    // Skip empty patterns
    if (pattern.empty()) {
      return 0;
    }
    
    // Try to create RE2 object with the pattern and options
    re2::RE2 re(pattern, options);
    
    // Only proceed if the RE2 object was constructed successfully
    if (re.ok()) {
      // Exercise the RE2 object with a dummy string
      static const std::string dummy_text = "test string 123";
      bool match_result = re2::RE2::PartialMatch(dummy_text, re);
      
      // Also test FullMatch
      bool full_match_result = re2::RE2::FullMatch(dummy_text, re);
    }
  }

  return 0;
}