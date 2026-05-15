// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::case_sensitive at re2.h:723:10 in re2.h
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::case_sensitive at re2.h:723:10 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  // Create options object with different canned options based on input
  uint8_t canned_option = Data[0] % 4;
  re2::RE2::Options options(static_cast<re2::RE2::CannedOptions>(canned_option));

  // Use remaining bytes to determine boolean values for setters
  // We'll cycle through the data to get multiple boolean values
  for (size_t i = 1; i < Size; i++) {
    uint8_t byte = Data[i];
    
    // Extract multiple boolean values from each byte
    bool posix_syntax = byte & 0x01;
    bool case_sensitive = byte & 0x02;
    bool literal = byte & 0x04;
    bool perl_classes = byte & 0x08;
    
    // Call setter functions
    options.set_posix_syntax(posix_syntax);
    options.set_case_sensitive(case_sensitive);
    options.set_literal(literal);
    options.set_perl_classes(perl_classes);
    
    // Call getter functions to verify settings
    bool retrieved_posix = options.posix_syntax();
    bool retrieved_case = options.case_sensitive();
    
    // Use the values to prevent optimization
    (void)retrieved_posix;
    (void)retrieved_case;
    
    // Try different canned options for variety
    if (i % 8 == 0) {
      uint8_t new_canned = byte % 4;
      re2::RE2::Options new_options(static_cast<re2::RE2::CannedOptions>(new_canned));
      
      // Apply settings to new options object
      new_options.set_posix_syntax(posix_syntax);
      new_options.set_case_sensitive(case_sensitive);
      new_options.set_literal(literal);
      new_options.set_perl_classes(perl_classes);
      
      // Call getters on new object
      (void)new_options.posix_syntax();
      (void)new_options.case_sensitive();
    }
  }

  return 0;
}