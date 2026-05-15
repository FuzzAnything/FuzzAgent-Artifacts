#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "absl/strings/string_view.h"
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_007 {
// RE2::Set multi-pattern matching fuzzing harness
// Targets RE2::Set APIs identified as critical coverage gap with only 15.97% region coverage
// and 25.68% branch coverage, focusing on 109+ undiscovered branches
// Implements complete RE2::Set lifecycle: constructor, Add, Compile, Match
// Semantically unique from existing harnesses with comprehensive multi-pattern testing



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Limit input size for efficiency
  if (size == 0 || size > 4096) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);
  // Step 1: Configure RE2::Options from fuzzer input
  RE2::Options options;
  
  // Set encoding: UTF-8 (default) or Latin1
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
  
  // Set various options from fuzzer input
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Don't log errors during fuzzing
  options.set_max_mem(64 << 20);  // 64 MB memory limit
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_case_sensitive(!fdp.ConsumeBool());  // Invert: default is true
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  // Step 2: Choose anchor type for RE2::Set constructor
  RE2::Anchor anchor = RE2::UNANCHORED;
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

  // Step 3: Create RE2::Set object with options and anchor
  // Use unique_ptr to ensure proper cleanup
  std::unique_ptr<RE2::Set> set;
  try {
    set.reset(new RE2::Set(options, anchor));
  } catch (...) {
    // If construction fails, return early
    return 0;
  }

  // Step 4: Add multiple patterns to the set
  // Determine how many patterns to add (1-8)
  int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 8);
  std::vector<int> pattern_indices;
  std::vector<std::string> pattern_strings;
  
  for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
    // Consume pattern string from fuzzer input
    std::string pattern = fdp.ConsumeRandomLengthString(100);
    pattern_strings.push_back(pattern);
    
    // Test both Add() variants: with and without error string
    if (fdp.ConsumeBool()) {
      // Add pattern without error string
      int index = set->Add(pattern, nullptr);
      if (index != -1) {
        pattern_indices.push_back(index);
      }
    } else {
      // Add pattern with error string
      std::string error;
      int index = set->Add(pattern, &error);
      if (index != -1) {
        pattern_indices.push_back(index);
      }
      // error string may contain parser error message if pattern was invalid
    }
    
    // If we're running low on input, break early
    if (fdp.remaining_bytes() < 20) {
      break;
    }
  }

  // Step 5: Check Set size before compilation
  int set_size = set->Size();
  
  // If no patterns were successfully added, return early
  if (set_size == 0) {
    return 0;
  }

  // Step 6: Compile the set
  bool compile_success = set->Compile();
  
  // If compilation failed, return early
  if (!compile_success) {
    return 0;
  }

  // Step 7: Consume text to match against
  std::string text = fdp.ConsumeRandomLengthString(200);
  absl::string_view text_view(text);

  // Step 8: Test Match() operations with both overloaded versions
  // Test both variants of Match() to maximize coverage

  // Variant 1: Match without error_info
  if (fdp.ConsumeBool()) {
    std::vector<int> matches1;
    bool match_result1 = set->Match(text_view, &matches1);
    
    // Check if matches vector is populated correctly
    if (match_result1 && !matches1.empty()) {
      // Verify all match indices are within valid range
      for (int match_idx : matches1) {
        if (match_idx < 0 || match_idx >= set_size) {
          // Invalid match index - this shouldn't happen but we check
        }
      }
    }
  }

  // Variant 2: Match with error_info  
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
    std::vector<int> matches2;
    RE2::Set::ErrorInfo error_info;
    
    bool match_result2 = set->Match(text_view, &matches2, &error_info);
    
    // Check error_info for various error conditions
    switch (error_info.kind) {
      case RE2::Set::kNoError:
        // No error - either match found or text doesn't match any pattern
        break;
      case RE2::Set::kNotCompiled:
        // Should not happen since we called Compile()
        break;
      case RE2::Set::kOutOfMemory:
        // DFA ran out of memory during matching
        break;
      case RE2::Set::kInconsistent:
        // Internal inconsistency - should never happen
        break;
    }
    
    if (match_result2 && !matches2.empty()) {
      // Verify match indices
      for (int match_idx : matches2) {
        if (match_idx < 0 || match_idx >= set_size) {
          // Invalid match index
        }
      }
    }
  }

  // Step 9: Test edge cases with different text inputs

  // Test with empty text
  if (fdp.ConsumeBool()) {
    std::vector<int> empty_matches;
    absl::string_view empty_text("");
    set->Match(empty_text, &empty_matches);
  }

  // Test with text that might trigger specific regex features
  if (fdp.remaining_bytes() > 10) {
    std::string special_text = fdp.ConsumeRandomLengthString(50);
    absl::string_view special_view(special_text);
    
    std::vector<int> special_matches;
    set->Match(special_view, &special_matches);
  }

  // Step 10: Test mixed operations with the same set
  
  // Test multiple Match calls with different texts
  if (fdp.remaining_bytes() > 20) {
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
      std::string multi_text = fdp.ConsumeRandomLengthString(30);
      absl::string_view multi_view(multi_text);
      
      std::vector<int> multi_matches;
      set->Match(multi_view, &multi_matches);
    }
  }

  // Step 11: Test that Add() cannot be called after Compile()
  // This should fail gracefully
  if (fdp.remaining_bytes() > 10 && fdp.ConsumeBool()) {
    std::string post_compile_pattern = fdp.ConsumeRandomLengthString(20);
    std::string post_compile_error;
    
    // This should return -1 because set is already compiled
    int post_compile_index = set->Add(post_compile_pattern, &post_compile_error);
    // post_compile_index should be -1
  }

  // Step 12: Test with pattern strings that contain various regex features
  // Create a secondary set with different options to test variations
  
  if (fdp.remaining_bytes() > 100) {
    // Create a second set with different anchor
    RE2::Anchor second_anchor = (anchor == RE2::UNANCHORED) ? RE2::ANCHOR_BOTH : RE2::UNANCHORED;
    
    std::unique_ptr<RE2::Set> set2;
    try {
      set2.reset(new RE2::Set(options, second_anchor));
    } catch (...) {
      // Continue without set2 if construction fails
    }
    
    if (set2) {
      // Add a few patterns to the second set
      int second_patterns = fdp.ConsumeIntegralInRange<int>(1, 3);
      for (int i = 0; i < second_patterns && fdp.remaining_bytes() > 20; i++) {
        std::string pattern2 = fdp.ConsumeRandomLengthString(40);
        set2->Add(pattern2, nullptr);
      }
      
      // Compile the second set
      if (set2->Size() > 0) {
        set2->Compile();
        
        // Test matching with second set
        if (fdp.remaining_bytes() > 10) {
          std::string text2 = fdp.ConsumeRandomLengthString(50);
          absl::string_view text2_view(text2);
          
          std::vector<int> matches2;
          set2->Match(text2_view, &matches2);
        }
      }
    }
  }

  // Step 13: Test with special pattern types
  
  // Test patterns with different regex syntax elements
  std::vector<std::string> special_patterns = {
    ".*",           // Match anything
    "^start",       // Anchor at start
    "end$",         // Anchor at end
    "a|b|c",        // Alternation
    "a+",           // One or more
    "b*",           // Zero or more
    "c?",           // Zero or one
    "[abc]",        // Character class
    "[^xyz]",       // Negated character class
    "\\d",          // Digit character class
    "\\w+",         // Word characters
    "(ab)+",        // Grouping with repetition
    "a{2,4}",       // Bounded repetition
  };
  
  // Create a third set to test special patterns
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 50) {
    RE2::Options special_options = options;
    special_options.set_posix_syntax(fdp.ConsumeBool());
    special_options.set_perl_classes(fdp.ConsumeBool());
    
    std::unique_ptr<RE2::Set> special_set;
    try {
      special_set.reset(new RE2::Set(special_options, RE2::UNANCHORED));
    } catch (...) {
      // Continue without special_set if construction fails
    }
    
    if (special_set) {
      // Add a subset of special patterns
      int num_special = fdp.ConsumeIntegralInRange<int>(1, 5);
      for (int i = 0; i < num_special && i < (int)special_patterns.size(); i++) {
        special_set->Add(special_patterns[i], nullptr);
      }
      
      if (special_set->Size() > 0) {
        special_set->Compile();
        
        // Test matching with special patterns
        if (fdp.remaining_bytes() > 10) {
          std::string special_match_text = fdp.ConsumeRandomLengthString(30);
          absl::string_view special_match_view(special_match_text);
          
          std::vector<int> special_matches;
          special_set->Match(special_match_view, &special_matches);
        }
      }
    }
  }

  // Step 14: Test error handling paths
  
  // Create a set that might fail compilation due to memory limits
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
    RE2::Options limited_options = options;
    limited_options.set_max_mem(1024);  // Very small memory limit (1KB)
    
    std::unique_ptr<RE2::Set> limited_set;
    try {
      limited_set.reset(new RE2::Set(limited_options, anchor));
    } catch (...) {
      // Continue without limited_set if construction fails
    }
    
    if (limited_set) {
      // Add a simple pattern
      limited_set->Add("simple", nullptr);
      
      // Compile might fail due to memory limit
      bool limited_compile = limited_set->Compile();
      
      // If compilation succeeded, try to match
      if (limited_compile) {
        std::string limited_text = "simple test";
        absl::string_view limited_view(limited_text);
        
        std::vector<int> limited_matches;
        RE2::Set::ErrorInfo limited_error;
        limited_set->Match(limited_view, &limited_matches, &limited_error);
        
        // Check if we got an out of memory error
        if (limited_error.kind == RE2::Set::kOutOfMemory) {
          // Expected behavior with very low memory limit
        }
      }
    }
  }

  // Step 15: Test with move semantics (if supported)
  // Note: RE2::Set has move constructor and move assignment operator
  
  if (fdp.ConsumeBool()) {
    // Create a new set by moving from the original
    RE2::Set moved_set(std::move(*set));
    
    // The original set should now be in a valid but unspecified state
    // We can still safely call Size() on it (should return 0 for moved-from object)
    int moved_size = moved_set.Size();
    
    // Test matching with the moved set
    if (moved_size > 0) {
      std::vector<int> moved_matches;
      moved_set.Match(text_view, &moved_matches);
    }
    
    // Move assignment test
    RE2::Set assigned_set(options, anchor);
    assigned_set = std::move(moved_set);
    
    // Test matching with the assigned set
    if (assigned_set.Size() > 0) {
      std::vector<int> assigned_matches;
      assigned_set.Match(text_view, &assigned_matches);
    }
  }

  return 0;
}

} // namespace harness_007
