#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <limits>
#include "re2/re2.h"

namespace harness_007 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for basic operations
  if (size < 16) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Create RE2::Options object and configure regex options
  RE2::Options options;
  
  // Consume configuration flags from fuzzer input
  uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
  
  // Configure options based on fuzzed input
  if (config_flags & 0x01) options.set_encoding(RE2::Options::EncodingLatin1);
  if (config_flags & 0x02) options.set_posix_syntax(true);
  if (config_flags & 0x04) options.set_longest_match(true);
  if (config_flags & 0x08) options.set_log_errors(false);
  if (config_flags & 0x10) options.set_literal(true);
  if (config_flags & 0x20) options.set_never_nl(true);
  if (config_flags & 0x40) options.set_dot_nl(true);
  if (config_flags & 0x80) options.set_never_capture(true);
  
  // Consume more flags for additional options
  if (fdp.remaining_bytes() > 0) {
    uint8_t more_flags = fdp.ConsumeIntegral<uint8_t>();
    if (more_flags & 0x01) options.set_case_sensitive(false);
    if (more_flags & 0x02) options.set_perl_classes(true);
    if (more_flags & 0x04) options.set_word_boundary(true);
    if (more_flags & 0x08) options.set_one_line(true);
    
    // Set max memory from fuzzed input (0-64KB range for fuzzing)
    if (fdp.remaining_bytes() >= sizeof(int64_t)) {
      int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 65536);
      options.set_max_mem(max_mem);
    }
  }
  
  // Step 2: Create RE2 object with pattern using the options
  std::string pattern = fdp.ConsumeRandomLengthString(999);
  RE2 re(pattern, options);
  
  // If pattern compilation failed, we can still test edge cases
  if (!re.ok()) {
    // Even with invalid pattern, we can test the fuzzing-only API
    // and some edge cases with empty/default patterns
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(0);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(1);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(10);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(1000);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(-1);
    return 0;
  }
  
  // Step 3: Call FUZZING_ONLY_set_maximum_global_replace_count with various limits
  // Consume limit values from fuzzer input
  if (fdp.remaining_bytes() >= sizeof(int)) {
    int limit1 = fdp.ConsumeIntegral<int>();
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(limit1);
    
    // Also test specific edge cases
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(0);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(1);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(10);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(100);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(1000);
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(-1);  // Unlimited
    
    // Test overflow scenarios
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(std::numeric_limits<int>::max());
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(std::numeric_limits<int>::min());
  }
  
  // Step 4: Perform GlobalReplace operations on test strings with the configured limit
  std::string text = fdp.ConsumeRandomLengthString(999);
  std::string replacement = fdp.ConsumeRandomLengthString(100);
  
  // Make a copy for GlobalReplace
  std::string global_text = text;
  int global_replace_count = RE2::GlobalReplace(&global_text, re, replacement);
  
  // Test with empty pattern
  if (fdp.remaining_bytes() > 10) {
    std::string empty_pattern = "";
    RE2 empty_re(empty_pattern, options);
    if (empty_re.ok()) {
      std::string empty_text = fdp.ConsumeRandomLengthString(100);
      std::string empty_replacement = fdp.ConsumeRandomLengthString(50);
      RE2::GlobalReplace(&empty_text, empty_re, empty_replacement);
    }
  }
  
  // Test with empty replacement
  std::string text2 = text;
  RE2::GlobalReplace(&text2, re, "");
  
  // Test with zero limit (should do 0 replacements)
  RE2::FUZZING_ONLY_set_maximum_global_replace_count(0);
  std::string text3 = text;
  int zero_limit_count = RE2::GlobalReplace(&text3, re, replacement);
  
  // Test with limit 1
  RE2::FUZZING_ONLY_set_maximum_global_replace_count(1);
  std::string text4 = text;
  int limit1_count = RE2::GlobalReplace(&text4, re, replacement);
  
  // Test with limit 10 (small N)
  RE2::FUZZING_ONLY_set_maximum_global_replace_count(10);
  std::string text5 = text;
  int limit10_count = RE2::GlobalReplace(&text5, re, replacement);
  
  // Test with large limit
  RE2::FUZZING_ONLY_set_maximum_global_replace_count(10000);
  std::string text6 = text;
  int large_limit_count = RE2::GlobalReplace(&text6, re, replacement);
  RE2::FUZZING_ONLY_set_maximum_global_replace_count(-1);  // Reset to unlimited
  
  std::string single_text = text;
  bool single_replaced = RE2::Replace(&single_text, re, replacement);
  
  // Test empty pattern for single replace
  if (fdp.remaining_bytes() > 5) {
    std::string empty_pattern2 = "";
    RE2 empty_re2(empty_pattern2, options);
    if (empty_re2.ok()) {
      std::string empty_text2 = fdp.ConsumeRandomLengthString(100);
      RE2::Replace(&empty_text2, empty_re2, replacement);
    }
  }
  
  // Test empty replacement for single replace
  std::string text7 = text;
  RE2::Replace(&text7, re, "");
  
  // Step 6: Test edge cases
  // Edge case: pattern that matches everything
  std::string dot_star_pattern = ".*";
  RE2 dot_star_re(dot_star_pattern, options);
  if (dot_star_re.ok()) {
    std::string dot_star_text = fdp.ConsumeRandomLengthString(200);
    RE2::GlobalReplace(&dot_star_text, dot_star_re, "X");
    RE2::Replace(&dot_star_text, dot_star_re, "Y");
  }
  
  // Edge case: pattern with backreferences in replacement
  std::string capture_pattern = "(a)(b)(c)";
  RE2 capture_re(capture_pattern, options);
  if (capture_re.ok()) {
    std::string capture_text = "abcabcabc";
    RE2::GlobalReplace(&capture_text, capture_re, "\\3\\2\\1");
    RE2::Replace(&capture_text, capture_re, "\\1\\2\\3");
  }
  
  // Test with different limit values in sequence
  std::vector<int> test_limits = {0, 1, 2, 5, 10, 50, 100, -1};
  for (int limit : test_limits) {
    RE2::FUZZING_ONLY_set_maximum_global_replace_count(limit);
    std::string limit_text = text;
    RE2::GlobalReplace(&limit_text, re, replacement);
  }
  
  // Reset to default for other tests
  RE2::FUZZING_ONLY_set_maximum_global_replace_count(-1);
  
  return 0;
}

} // namespace harness_007
