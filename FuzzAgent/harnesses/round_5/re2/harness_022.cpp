#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_022 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing - need enough for patterns, options, and test strings
  if (size < 128) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Strategy: Target RE2::Set multi-pattern matching operations with semantic differentiation
  // from harness_017 and harness_018. Focus on comprehensive testing of Match() overloads
  // (24 undiscovered branches each) with specifically crafted matching scenarios.
  
  // Step 1: Create RE2::Options with diverse configurations
  RE2::Options options;
  
  // Configure encoding
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingUTF8 
                                         : RE2::Options::EncodingLatin1);
  
  // Configure various boolean flags
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Disable error logging for fuzzing
  
  // Set memory limit
  options.set_max_mem(fdp.ConsumeIntegralInRange<int64_t>(1 << 16, 64 << 20)); // 64KB to 64MB
  
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
  
  // Step 2: Choose anchor type
  RE2::Anchor anchor;
  uint8_t anchor_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
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
  }
  
  // Step 3: Create RE2::Set object
  RE2::Set set(options, anchor);
  
  // Step 4: Add diverse regex patterns as specified in guidance
  std::vector<std::string> patterns;
  std::vector<int> pattern_indices;
  
  // Fixed set of diverse patterns for consistent testing
  const char* fixed_patterns[] = {
    // Simple literals
    "test",
    "hello",
    "world",
    
    // Character classes
    "[A-Za-z]+",
    "[0-9]{3,5}",
    "[[:alpha:]][[:alnum:]]*",
    
    // Quantifiers
    "a{1,3}b{2,4}",
    "x*y+z?",
    
    // Alternation
    "(foo|bar|baz)",
    "(cat|dog|bird|fish)",
    
    // Groups and backreferences
    "(\\d{3})-(\\d{3})",
    "([A-Z])\\1+",  // Repeated capital letters
    
    // Anchors
    "^start",
    "end$",
    "^complete$",
    
    // Special character classes
    "\\w+",
    "\\d+",
    "\\s+",
    
    // Complex patterns
    "([A-Za-z]+):\\s*(\\d+)",
    "\\bword\\b",
    
    // Unicode patterns (if UTF-8 encoding)
    "\\p{L}+",
  };
  
  // Add patterns from the fixed set
  for (size_t i = 0; i < sizeof(fixed_patterns)/sizeof(fixed_patterns[0]); i++) {
    std::string error;
    int index = set.Add(fixed_patterns[i], &error);
    if (index >= 0) {
      patterns.push_back(fixed_patterns[i]);
      pattern_indices.push_back(index);
    }
  }
  
  // Also add some fuzzer-generated patterns for variety
  size_t num_fuzz_patterns = fdp.ConsumeIntegralInRange<size_t>(0, 5);
  for (size_t i = 0; i < num_fuzz_patterns && fdp.remaining_bytes() > 10; i++) {
    size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
    std::string pattern = fdp.ConsumeBytesAsString(pattern_len);
    
    // Ensure pattern is a valid string (replace null bytes)
    for (char& c : pattern) {
      if (c == '\0') c = 'x';
    }
    
    std::string error;
    int index = set.Add(pattern, &error);
    if (index >= 0) {
      patterns.push_back(pattern);
      pattern_indices.push_back(index);
    }
  }
  
  // Step 5: Verify pattern count with Size()
  int pattern_count = set.Size();
  
  // Step 6: Compile the set
  bool compile_success = false;
  if (pattern_count > 0) {
    compile_success = set.Compile();
  }
  
  // Only proceed with matching if compilation succeeded
  if (compile_success) {
    // Step 7: Test Match() with various input strings as specified in guidance
    
    // Test scenario 1: Strings matching no patterns
    {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      
      // Generate strings unlikely to match any pattern
      size_t no_match_len1 = fdp.ConsumeIntegralInRange<size_t>(1, 50);
      std::string no_match1 = fdp.ConsumeBytesAsString(no_match_len1);
      bool result1 = set.Match(no_match1, &matches, &error_info);
      
      // Generate another test string
      size_t no_match_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 50);
      std::string no_match2 = fdp.ConsumeBytesAsString(no_match_len2);
      bool result2 = set.Match(no_match2, &matches, &error_info);
      
      // Test overload without ErrorInfo
      matches.clear();
      size_t no_match_len3 = fdp.ConsumeIntegralInRange<size_t>(1, 50);
      std::string no_match3 = fdp.ConsumeBytesAsString(no_match_len3);
      bool result3 = set.Match(no_match3, &matches);
    }
    
    // Test scenario 2: Strings matching single patterns
    if (patterns.size() > 0) {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      
      // Generate test strings that might match patterns
      // We'll use fuzzer input to generate strings
      for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
        size_t test_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
        std::string test_str = fdp.ConsumeBytesAsString(test_len);
        
        // Alternate between with and without ErrorInfo
        if (i % 2 == 0) {
          bool result = set.Match(test_str, &matches, &error_info);
        } else {
          bool result = set.Match(test_str, &matches);
        }
        matches.clear();
      }
    }
    
    // Test scenario 3: Strings matching multiple patterns
    if (patterns.size() > 1) {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      
      // Generate strings that combine different pattern elements
      // This increases chance of matching multiple patterns
      for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
        // Create strings with alphanumeric content that might match multiple patterns
        size_t alpha_len = fdp.ConsumeIntegralInRange<size_t>(3, 20);
        std::string alpha_part = fdp.ConsumeBytesAsString(alpha_len);
        
        size_t digit_len = fdp.ConsumeIntegralInRange<size_t>(1, 10);
        std::string digit_part = fdp.ConsumeBytesAsString(digit_len);
        
        std::string combined_str = alpha_part + digit_part;
        
        if (i % 2 == 0) {
          bool result = set.Match(combined_str, &matches, &error_info);
        } else {
          bool result = set.Match(combined_str, &matches);
        }
        matches.clear();
      }
    }
    
    // Test scenario 4: Strings with partial matches
    {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      
      // Generate strings that might partially match
      for (int i = 0; i < 2 && fdp.remaining_bytes() > 10; i++) {
        size_t test_len = fdp.ConsumeIntegralInRange<size_t>(10, 100);
        std::string test_str = fdp.ConsumeBytesAsString(test_len);
        
        if (i % 2 == 0) {
          bool result = set.Match(test_str, &matches, &error_info);
        } else {
          bool result = set.Match(test_str, &matches);
        }
        matches.clear();
      }
    }
    
    // Test scenario 5: Empty strings
    {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      
      bool result1 = set.Match("", &matches, &error_info);
      matches.clear();
      
      // Test overload without ErrorInfo
      bool result2 = set.Match("", &matches);
      matches.clear();
    }
    
    // Test scenario 6: Fuzzer-generated test strings
    size_t num_fuzz_tests = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    for (size_t i = 0; i < num_fuzz_tests && fdp.remaining_bytes() > 5; i++) {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      
      size_t test_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
      std::string test_str = fdp.ConsumeBytesAsString(test_len);
      
      // Alternate between with and without ErrorInfo
      if (i % 2 == 0) {
        bool result = set.Match(test_str, &matches, &error_info);
      } else {
        bool result = set.Match(test_str, &matches);
      }
    }
    
    // Test scenario 7: Test NULL parameter combinations for both overloads
    if (fdp.remaining_bytes() > 5) {
      size_t test_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
      std::string test_str = fdp.ConsumeBytesAsString(test_len);
      
      // Test first overload with NULL vector
      bool result1 = set.Match(test_str, nullptr);
      
      // Test second overload with various NULL combinations
      RE2::Set::ErrorInfo error_info;
      bool result2 = set.Match(test_str, nullptr, &error_info);
      
      std::vector<int> matches;
      bool result3 = set.Match(test_str, &matches, nullptr);
      
      bool result4 = set.Match(test_str, nullptr, nullptr);
    }
    
    // Step 8: Verify Size() again after all operations
    int final_size = set.Size();
    
    // Step 9: Test that Add() fails after compilation (as expected)
    if (fdp.remaining_bytes() > 5) {
      std::string error;
      int should_fail = set.Add("post_compile_pattern", &error);
    }
  } else {
    // Test that Match() fails when not compiled
    if (pattern_count > 0 && fdp.remaining_bytes() > 5) {
      std::vector<int> matches;
      RE2::Set::ErrorInfo error_info;
      size_t test_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
      std::string test_str = fdp.ConsumeBytesAsString(test_len);
      bool should_fail = set.Match(test_str, &matches, &error_info);
    }
  }
  
  // Destructor automatically cleans up when set goes out of scope
  
  return 0;
}

} // namespace harness_022
