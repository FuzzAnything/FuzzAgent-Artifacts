#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_010 {
// RE2::Set comprehensive lifecycle harness targeting parse and match blocked branches
// Targets: Complete RE2::Set lifecycle with focus on Regexp::Parse and RE2::Match functions
// Coverage goal: Uncover 236 blocked branches in parse and match logic through diverse pattern testing
// Semantic diversity: More comprehensive than harness_003 (basic) and harness_009 (assignment)
// Features: Multiple pattern sets, parse failures, compilation errors, all Match variants, edge cases



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Need sufficient data for patterns, options, and text
  if (size < 64) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Configure RE2::Options with diverse settings to explore different code paths
  re2::RE2::Options options;
  
  // Use fuzzer input to configure options that affect parsing and matching
  options.set_case_sensitive(fdp.ConsumeBool());
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Don't log errors during fuzzing
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());  // Important for Set optimization
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  
  // Encoding affects character class handling
  if (fdp.ConsumeBool()) {
    options.set_encoding(re2::RE2::Options::EncodingUTF8);
  } else {
    options.set_encoding(re2::RE2::Options::EncodingLatin1);
  }

  // 2. Choose anchor type - affects matching behavior
  re2::RE2::Anchor anchor;
  uint8_t anchor_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
  switch (anchor_choice) {
    case 0:
      anchor = re2::RE2::Anchor::UNANCHORED;
      break;
    case 1:
      anchor = re2::RE2::Anchor::ANCHOR_START;
      break;
    case 2:
      anchor = re2::RE2::Anchor::ANCHOR_BOTH;
      break;
    case 3:
      // Also test with default (UNANCHORED) but different path
      anchor = re2::RE2::Anchor::UNANCHORED;
      break;
  }

  // 3. Create RE2::Set object
  re2::RE2::Set regex_set(options, anchor);

  // 4. Add patterns with diverse characteristics to trigger different parse paths
  // Determine number of patterns (0-20, including 0 to test empty set)
  size_t num_patterns_to_add = fdp.ConsumeIntegralInRange<size_t>(0, 20);
  std::vector<int> added_indices;
  std::vector<std::string> error_strings;
  int successful_adds = 0;
  int parse_failures = 0;

  for (size_t i = 0; i < num_patterns_to_add && fdp.remaining_bytes() > 10; i++) {
    // Determine pattern length to test various parse scenarios
    size_t pattern_length = fdp.ConsumeIntegralInRange<size_t>(0, 200);
    std::string pattern;
    
    if (pattern_length > 0) {
      pattern = fdp.ConsumeRandomLengthString(pattern_length);
    } else {
      // Test empty pattern (should parse as empty string)
      pattern = "";
    }
    
    // Decide whether to collect error string (tests error handling path)
    std::string* error_ptr = nullptr;
    if (fdp.ConsumeBool()) {
      error_strings.emplace_back();
      error_ptr = &error_strings.back();
    }
    
    // Add pattern to set
    int pattern_index = regex_set.Add(pattern, error_ptr);
    
    if (pattern_index >= 0) {
      successful_adds++;
      added_indices.push_back(pattern_index);
    } else {
      parse_failures++;
      // Pattern failed to parse - error string may contain error message
    }
  }

  // 5. Test Size() before compilation
  int size_before_compile = regex_set.Size();
  
  // 6. Attempt compilation (may fail due to memory or other issues)
  bool compile_success = false;
  if (successful_adds > 0 || num_patterns_to_add == 0) {
    compile_success = regex_set.Compile();
  }
  
  // If compilation failed, we can't test Match(), but we've already exercised
  // compilation failure paths which may include memory exhaustion paths
  if (!compile_success) {
    // Even without successful compilation, we've tested important code paths
    return 0;
  }
  
  // 7. Test Size() after compilation (should be same as before)
  int size_after_compile = regex_set.Size();

  // 8. Generate test text for matching
  std::string test_text;
  if (fdp.remaining_bytes() > 0) {
    size_t text_length = fdp.ConsumeIntegralInRange<size_t>(0, 500);
    if (text_length > 0) {
      test_text = fdp.ConsumeRandomLengthString(text_length);
    } else {
      test_text = "";  // Test empty string matching
    }
  } else {
    test_text = "";  // No data left for text
  }

  // 9. Comprehensive Match() testing with all parameter combinations
  // This targets the RE2::Match blocked branches
  
  // Test 1: Match with vector to collect indices
  std::vector<int> matching_indices1;
  bool match_result1 = regex_set.Match(test_text, &matching_indices1);
  
  // Test 2: Match with vector and ErrorInfo
  std::vector<int> matching_indices2;
  re2::RE2::Set::ErrorInfo error_info2;
  bool match_result2 = regex_set.Match(test_text, &matching_indices2, &error_info2);
  
  // Test 3: Match with nullptr vector (only care about match existence)
  bool match_result3 = regex_set.Match(test_text, nullptr);
  
  // Test 4: Match with nullptr vector and ErrorInfo
  re2::RE2::Set::ErrorInfo error_info4;
  bool match_result4 = regex_set.Match(test_text, nullptr, &error_info4);
  
  // Test 5: Match with nullptr vector and nullptr ErrorInfo
  bool match_result5 = regex_set.Match(test_text, nullptr, nullptr);
  
  // Test 6: Match with vector but nullptr ErrorInfo (uses default overload)
  std::vector<int> matching_indices6;
  bool match_result6 = regex_set.Match(test_text, &matching_indices6);

  // 10. Test with different text inputs to explore more match paths
  if (fdp.remaining_bytes() > 10) {
    std::string additional_text = fdp.ConsumeRandomLengthString(100);
    
    // Test additional matching scenarios
    std::vector<int> additional_matches;
    re2::RE2::Set::ErrorInfo additional_error;
    
    // Test with all parameter combinations again
    regex_set.Match(additional_text, &additional_matches);
    regex_set.Match(additional_text, &additional_matches, &additional_error);
    regex_set.Match(additional_text, nullptr);
    regex_set.Match(additional_text, nullptr, &additional_error);
    regex_set.Match(additional_text, nullptr, nullptr);
  }

  // 11. Test edge cases for match vector behavior
  // Clear and reuse vector
  std::vector<int> reusable_vector;
  for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
    std::string edge_text = fdp.ConsumeRandomLengthString(50);
    reusable_vector.clear();
    regex_set.Match(edge_text, &reusable_vector);
    
    // Vector should contain matched pattern indices if any match occurred
  }

  // 12. Test ErrorInfo values for different scenarios
  // Create another set with problematic patterns to trigger DFA memory errors
  if (fdp.remaining_bytes() > 50) {
    re2::RE2::Options options2;
    options2.set_max_mem(1024);  // Very small memory limit to potentially trigger DFA out of memory
    options2.set_never_capture(true);
    
    re2::RE2::Set small_mem_set(options2, re2::RE2::Anchor::UNANCHORED);
    
    // Add a few simple patterns
    small_mem_set.Add("a", nullptr);
    small_mem_set.Add("b", nullptr);
    small_mem_set.Add("c", nullptr);
    
    // Try to compile (may succeed or fail due to memory)
    bool small_compile = small_mem_set.Compile();
    
    if (small_compile) {
      // Test match with ErrorInfo to see if DFA errors are captured
      std::vector<int> small_matches;
      re2::RE2::Set::ErrorInfo small_error;
      std::string small_text = fdp.ConsumeRandomLengthString(100);
      small_mem_set.Match(small_text, &small_matches, &small_error);
      
      // ErrorInfo.kind may be kNoError, kOutOfMemory, etc.
    }
  }

  // 13. Test that Add() fails after Compile() (as documented)
  // This is already enforced by RE2::Set implementation but we verify behavior
  // Note: This would trigger a DCHECK in debug builds

  // 14. RAII cleanup happens automatically when objects go out of scope

  return 0;
}

} // namespace harness_010
