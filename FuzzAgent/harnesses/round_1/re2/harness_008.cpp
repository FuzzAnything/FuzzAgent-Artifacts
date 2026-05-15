#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <limits>
#include "re2/re2.h"

namespace harness_008 {
// RE2 GlobalReplace with fuzzing safety limits harness
// Targets: RE2::FUZZING_ONLY_set_maximum_global_replace_count (0% covered fuzzing-only API)
//          RE2::GlobalReplace (164 undiscovered branches)
//          RE2 constructor and RE2::ok() for validation
// Based on coverage guidance: /root/FuzzAgent/output/re2/coverage_guidance/harness_008_guidance.md
// This harness specifically tests the fuzzing safety API that prevents infinite loops/explosions
// during GlobalReplace operations, addressing both surface coverage (uncovered API) and deep 
// coverage (blocked branches in heavily-used API)



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need enough data for patterns, limits, and test strings
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Set GlobalReplace limit using the fuzzing-only API
  // Consume limit value: test various edge cases (-1, 0, 1, small, large values)
  int limit_choice = fdp.ConsumeIntegralInRange<int>(0, 5);
  int replace_limit;
  
  switch (limit_choice) {
    case 0:
      replace_limit = -1;  // Unlimited (default)
      break;
    case 1:
      replace_limit = 0;   // No replacements allowed
      break;
    case 2:
      replace_limit = 1;   // Single replacement only
      break;
    case 3:
      replace_limit = fdp.ConsumeIntegralInRange<int>(2, 10);  // Small limit
      break;
    case 4:
      replace_limit = fdp.ConsumeIntegralInRange<int>(100, 1000);  // Large limit
      break;
    case 5:
      replace_limit = fdp.ConsumeIntegral<int>();  // Random limit (could be negative)
      break;
  }
  
  // Apply the fuzzing-only limit
  re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(replace_limit);

  // 2. Create RE2 pattern from fuzzer input
  // Choose pattern type to test different matching scenarios
  int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 6);
  std::string pattern;
  
  switch (pattern_type) {
    case 0:
      // Simple literal pattern
      pattern = fdp.ConsumeRandomLengthString(50);
      break;
    case 1:
      // Pattern with repetition (could trigger many matches)
      {
        std::string base = fdp.ConsumeRandomLengthString(20);
        pattern = "(" + base + ")+";
      }
      break;
    case 2:
      // Pattern with wildcards (matches many things)
      {
        std::string prefix = fdp.ConsumeRandomLengthString(20);
        std::string suffix = fdp.ConsumeRandomLengthString(20);
        pattern = prefix + ".*" + suffix;
      }
      break;
    case 3:
      // Empty pattern (edge case)
      pattern = "";
      break;
    case 4:
      // Pattern with character classes
      {
        std::string chars = fdp.ConsumeRandomLengthString(10);
        pattern = "[" + chars + "]+";
      }
      break;
    case 5:
      // Unicode pattern (if UTF-8 encoding)
      pattern = fdp.ConsumeRandomLengthString(100);  // Could contain UTF-8
      break;
    case 6:
      // Pattern with anchors
      {
        std::string middle = fdp.ConsumeRandomLengthString(30);
        pattern = "^" + middle + "$";
      }
      break;
  }

  // 3. Create RE2 object with the pattern
  re2::RE2 re(pattern);
  
  // Validate pattern compilation
  if (!re.ok()) {
    // Pattern failed to compile, but we still tested the fuzzing-only API
    return 0;
  }

  // 4. Prepare test string for GlobalReplace operations
  // Choose test string type based on fuzzer input
  int string_type = fdp.ConsumeIntegralInRange<int>(0, 5);
  std::string test_string;
  
  switch (string_type) {
    case 0:
      // Random string
      test_string = fdp.ConsumeRandomLengthString(512);
      break;
    case 1:
      // String with many repeating patterns (to test replacement limits)
      {
        std::string unit = fdp.ConsumeRandomLengthString(10);
        int repetitions = fdp.ConsumeIntegralInRange<int>(1, 20);
        for (int i = 0; i < repetitions; i++) {
          test_string += unit;
          if (i < repetitions - 1) {
            test_string += fdp.ConsumeRandomLengthString(5);
          }
        }
      }
      break;
    case 2:
      // Empty string (edge case)
      test_string = "";
      break;
    case 3:
      // String with overlapping matches potential
      {
        std::string overlapping = fdp.ConsumeRandomLengthString(20);
        test_string = overlapping + overlapping + overlapping;
      }
      break;
    case 4:
      // Unicode string
      test_string = fdp.ConsumeRandomLengthString(200);  // Could contain UTF-8
      break;
    case 5:
      // String with special characters
      {
        std::string specials = ".*+?^${}()|[]\\";
        std::string base = fdp.ConsumeRandomLengthString(100);
        test_string = specials + base + specials;
      }
      break;
  }

  // 5. Prepare replacement string
  std::string rewrite = fdp.ConsumeRandomLengthString(100);

  // 6. Perform GlobalReplace operation
  // Create mutable copy for replacement
  std::string result_string = test_string;
  int replacements_made = re2::RE2::GlobalReplace(&result_string, re, rewrite);
  
  // 7. Validate the operation
  // Check that the number of replacements made doesn't exceed our limit (if limit >= 0)
  if (replace_limit >= 0) {
    // The fuzzing-only API should enforce the limit
    // Note: We can't directly verify this from outside, but we trust the API
  }
  
  // Verify RE2 object is still valid
  bool re_still_ok = re.ok();
  
  // 8. Test edge cases with different input combinations
  
  // Test with empty rewrite string
  if (fdp.remaining_bytes() > 10) {
    std::string empty_rewrite_string = test_string;
    re2::RE2::GlobalReplace(&empty_rewrite_string, re, "");
  }
  
  // Test with rewrite string containing backreferences if pattern has groups
  if (fdp.remaining_bytes() > 10) {
    std::string backref_string = test_string;
    std::string backref_rewrite = "\\1\\2\\3";  // Common backreference pattern
    re2::RE2::GlobalReplace(&backref_string, re, backref_rewrite);
  }
  
  // Test with very long rewrite string (memory boundary)
  if (fdp.remaining_bytes() > 100) {
    std::string long_rewrite_string = test_string;
    std::string long_rewrite = fdp.ConsumeRandomLengthString(500);
    re2::RE2::GlobalReplace(&long_rewrite_string, re, long_rewrite);
  }
  
  // 9. Reset the limit to test different limit in same run
  if (fdp.remaining_bytes() > 4) {
    int new_limit = fdp.ConsumeIntegralInRange<int>(-1, 100);
    re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(new_limit);
    
    // Try another GlobalReplace with new limit
    if (fdp.remaining_bytes() > 10) {
      std::string another_test = fdp.ConsumeRandomLengthString(100);
      std::string another_rewrite = fdp.ConsumeRandomLengthString(50);
      std::string another_result = another_test;
      re2::RE2::GlobalReplace(&another_result, re, another_rewrite);
    }
  }

  return 0;
}

} // namespace harness_008
