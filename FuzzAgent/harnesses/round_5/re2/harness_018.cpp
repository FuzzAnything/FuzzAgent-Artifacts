#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_018 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 128) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Strategy: Comprehensive testing of RE2::Set operations with semantic
  // differentiation from harness_017. Focus on extensive pattern variation,
  // error handling, edge cases, and thorough coverage of all API methods.
  
  // Step 1: Create RE2::Options with diverse configurations
  RE2::Options options;
  
  // Configure encoding based on fuzzer input
  uint8_t encoding_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
  switch (encoding_choice) {
    case 0:
      options.set_encoding(RE2::Options::EncodingUTF8);
      break;
    case 1:
      options.set_encoding(RE2::Options::EncodingLatin1);
      break;
    case 2:
      // Leave at default (UTF-8)
      break;
    case 3:
      // Test with both encodings by toggling
      options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingUTF8 
                                           : RE2::Options::EncodingLatin1);
      break;
  }
  
  // Configure various boolean flags
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Disable error logging for fuzzing
  
  // Set memory limit with wide range
  int64_t mem_limit = fdp.ConsumeIntegralInRange<int64_t>(1 << 16, 128 << 20); // 64KB to 128MB
  options.set_max_mem(mem_limit);
  
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_case_sensitive(fdp.ConsumeBool());
  
  // When posix_syntax is true, test related flags
  if (options.posix_syntax()) {
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
  }
  
  // Step 2: Choose anchor type with bias toward more interesting options
  RE2::Anchor anchor;
  uint8_t anchor_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
  switch (anchor_choice) {
    case 0:
      anchor = RE2::UNANCHORED;
      break;
    case 1:
      anchor = RE2::ANCHOR_START;
      break;
    case 2:
      anchor = RE2::ANCHOR_BOTH;
      break;
    case 3:
      // Random choice
      anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START, 
        RE2::ANCHOR_BOTH,
      });
      break;
    case 4:
      // Default to UNANCHORED
      anchor = RE2::UNANCHORED;
      break;
  }
  
  // Step 3: Create RE2::Set object
  RE2::Set set(options, anchor);
  
  // Step 4: Test Size() before adding any patterns
  int initial_size = set.Size();
  (void)initial_size; // May be 0, but we want to exercise the call
  
  // Step 5: Add diverse patterns with comprehensive error handling
  std::vector<int> pattern_indices;
  std::vector<std::string> added_patterns;
  std::vector<std::string> error_messages;
  
  // Determine number of patterns to add (including potential invalid ones)
  size_t num_patterns_to_add = fdp.ConsumeIntegralInRange<size_t>(1, 15);
  
  for (size_t i = 0; i < num_patterns_to_add && fdp.remaining_bytes() > 20; i++) {
    // Decide pattern type: valid, invalid, or edge case
    uint8_t pattern_category = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    std::string pattern;
    std::string error_str;
    
    switch (pattern_category) {
      case 0: // Valid simple pattern
        pattern = "[A-Za-z0-9_]+";
        break;
      case 1: // Valid pattern with repetition
        pattern = "a{1," + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10)) + "}";
        break;
      case 2: // Valid pattern with alternation
        pattern = "(foo|bar|baz|qux)";
        break;
      case 3: // Valid pattern with character classes
        pattern = "[[:alpha:]]+";
        break;
      case 4: // Valid pattern with anchors
        pattern = "^start.*end$";
        break;
      case 5: // Valid pattern with groups
        pattern = "(\\d{3})-(\\d{3})-(\\d{4})";
        break;
      case 6: // Possibly invalid pattern (unbalanced parens)
        pattern = "(unbalanced";
        break;
      case 7: // Possibly invalid pattern (invalid escape)
        pattern = "\\z";
        break;
      case 8: // Edge case: empty pattern
        pattern = "";
        break;
      case 9: // Fuzzer-generated pattern
        {
          size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
          pattern = fdp.ConsumeBytesAsString(pattern_len);
          // Ensure it's a valid string (replace null bytes)
          for (char& c : pattern) {
            if (c == '\0') c = 'x';
          }
        }
        break;
    }
    
    // Call Add() with error string
    std::string error;
    int index = set.Add(pattern, &error);
    
    // Store results for later verification
    if (index != -1) {
      pattern_indices.push_back(index);
      added_patterns.push_back(pattern);
    } else {
      error_messages.push_back(error);
    }
  }
  
  // Step 6: Test Size() after adding patterns
  int size_after_add = set.Size();
  (void)size_after_add;
  
  // Step 7: Attempt compilation
  bool compile_success = false;
  if (fdp.ConsumeBool()) {
    // Try compilation
    compile_success = set.Compile();
    
    // Test Size() after compilation
    int size_after_compile = set.Size();
    (void)size_after_compile;
    
    // Only proceed with matching if compilation succeeded
    if (compile_success && pattern_indices.size() > 0) {
      // Step 8: Test both Match() overloads with various input strings
      
      // Test first Match() overload (without ErrorInfo)
      for (int match_test = 0; match_test < 3 && fdp.remaining_bytes() > 10; match_test++) {
        std::vector<int> match_results;
        
        // Generate test string
        size_t test_str_len = fdp.ConsumeIntegralInRange<size_t>(0, 200);
        std::string test_string = fdp.ConsumeBytesAsString(test_str_len);
        
        // Call Match() without ErrorInfo
        bool match_success1 = set.Match(test_string, &match_results);
        (void)match_success1;
        
        // Clear results for next test
        match_results.clear();
      }
      
      // Test second Match() overload (with ErrorInfo)
      for (int match_test = 0; match_test < 3 && fdp.remaining_bytes() > 10; match_test++) {
        std::vector<int> match_results;
        RE2::Set::ErrorInfo error_info;
        
        // Generate test string
        size_t test_str_len = fdp.ConsumeIntegralInRange<size_t>(0, 200);
        std::string test_string = fdp.ConsumeBytesAsString(test_str_len);
        
        // Call Match() with ErrorInfo
        bool match_success2 = set.Match(test_string, &match_results, &error_info);
        (void)match_success2;
        
        // Test different combinations of NULL parameters
        if (fdp.ConsumeBool()) {
          // Test with NULL vector
          bool match_success3 = set.Match(test_string, nullptr, &error_info);
          (void)match_success3;
        }
        
        if (fdp.ConsumeBool()) {
          // Test with NULL ErrorInfo
          std::vector<int> temp_results;
          bool match_success4 = set.Match(test_string, &temp_results, nullptr);
          (void)match_success4;
        }
        
        if (fdp.ConsumeBool()) {
          // Test with both NULL
          bool match_success5 = set.Match(test_string, nullptr, nullptr);
          (void)match_success5;
        }
      }
      
      // Step 9: Test edge cases for matching
      if (fdp.remaining_bytes() > 5) {
        // Test with empty string
        std::vector<int> empty_results;
        RE2::Set::ErrorInfo empty_error;
        bool empty_match = set.Match("", &empty_results, &empty_error);
        (void)empty_match;
        
        // Test with very long string (if enough input)
        if (fdp.remaining_bytes() > 100) {
          size_t long_len = fdp.ConsumeIntegralInRange<size_t>(100, 500);
          std::string long_string = fdp.ConsumeBytesAsString(long_len);
          std::vector<int> long_results;
          RE2::Set::ErrorInfo long_error;
          bool long_match = set.Match(long_string, &long_results, &long_error);
          (void)long_match;
        }
        
        // Test with string containing special characters
        std::string special_chars = "!@#$%^&*()\t\n\r\x01\x02";
        std::vector<int> special_results;
        RE2::Set::ErrorInfo special_error;
        bool special_match = set.Match(special_chars, &special_results, &special_error);
        (void)special_match;
      }
    }
  } else {
    // Test that Match() fails when not compiled
    if (pattern_indices.size() > 0 && fdp.remaining_bytes() > 10) {
      std::vector<int> temp_results;
      RE2::Set::ErrorInfo error_info;
      
      // This should fail because set is not compiled
      std::string test_str = fdp.ConsumeBytesAsString(
          fdp.ConsumeIntegralInRange<size_t>(1, 50));
      bool should_fail = set.Match(test_str, &temp_results, &error_info);
      (void)should_fail; // Expected to be false
    }
  }
  
  // Step 10: Test move semantics (constructor and assignment)
  if (fdp.ConsumeBool() && pattern_indices.size() > 0) {
    // Create a new set with different options for move testing
    RE2::Options alt_options;
    alt_options.set_case_sensitive(!options.case_sensitive());
    alt_options.set_encoding(RE2::Options::EncodingLatin1);
    
    RE2::Set set2(alt_options, RE2::ANCHOR_BOTH);
    
    // Add a simple pattern to set2
    set2.Add("test", nullptr);
    
    // Test move constructor
    RE2::Set set3(std::move(set2));
    int set3_size = set3.Size();
    (void)set3_size;
    
    // Test move assignment (if compilation succeeded)
    if (compile_success) {
      RE2::Set set4(options, anchor);
      set4 = std::move(set);
      int set4_size = set4.Size();
      (void)set4_size;
    }
  }
  
  // Step 11: Test that Add() fails after compilation
  if (compile_success && fdp.remaining_bytes() > 5) {
    std::string error;
    int should_fail_index = set.Add("should_fail", &error);
    (void)should_fail_index; // Should be -1
  }
  
  return 0;
}

} // namespace harness_018
