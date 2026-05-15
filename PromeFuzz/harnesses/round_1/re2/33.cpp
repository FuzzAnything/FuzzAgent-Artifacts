// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::Options::set_word_boundary at re2.h:730:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::perl_classes at re2.h:726:10 in re2.h
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

static void test_options_with_pattern(const uint8_t *data, size_t size, 
                                      const re2::RE2::Options& options) {
  if (size == 0) return;
  
  // Use the input data as pattern string
  std::string pattern(reinterpret_cast<const char*>(data), size);
  
  // Try to construct RE2 with the given options
  // This may fail if pattern is invalid, but that's OK for fuzzing
  re2::RE2 re(pattern, options);
  
  // No need to do anything with the RE2 object - just constructing it
  // exercises the options
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;
  
  // Use first byte to determine boolean values for options
  uint8_t flags = Data[0];
  bool perl_classes_flag = (flags & 0x01) != 0;
  bool never_capture_flag = (flags & 0x02) != 0;
  bool word_boundary_flag = (flags & 0x04) != 0;
  bool dot_nl_flag = (flags & 0x08) != 0;
  bool literal_flag = (flags & 0x10) != 0;
  
  // Test different CannedOptions
  re2::RE2::CannedOptions canned_options[] = {
    re2::RE2::DefaultOptions,
    re2::RE2::Latin1,
    re2::RE2::POSIX,
    re2::RE2::Quiet
  };
  
  for (auto canned_opt : canned_options) {
    // Create Options with canned option
    re2::RE2::Options options(canned_opt);
    
    // Set all the boolean options
    options.set_perl_classes(perl_classes_flag);
    options.set_never_capture(never_capture_flag);
    options.set_word_boundary(word_boundary_flag);
    options.set_dot_nl(dot_nl_flag);
    options.set_literal(literal_flag);
    
    // Test the getter function
    bool retrieved_perl_classes = options.perl_classes();
    (void)retrieved_perl_classes; // Prevent unused variable warning
    
    // Test with pattern (use remaining data for pattern)
    if (Size > 1) {
      test_options_with_pattern(Data + 1, Size - 1, options);
    }
  }
  
  // Also test with default constructor (not using CannedOptions)
  re2::RE2::Options default_options;
  default_options.set_perl_classes(perl_classes_flag);
  default_options.set_never_capture(never_capture_flag);
  default_options.set_word_boundary(word_boundary_flag);
  default_options.set_dot_nl(dot_nl_flag);
  default_options.set_literal(literal_flag);
  
  bool retrieved_perl_classes2 = default_options.perl_classes();
  (void)retrieved_perl_classes2; // Prevent unused variable warning
  
  if (Size > 1) {
    test_options_with_pattern(Data + 1, Size - 1, default_options);
  }
  
  return 0;
}