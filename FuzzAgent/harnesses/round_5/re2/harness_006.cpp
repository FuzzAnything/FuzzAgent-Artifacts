#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_006 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for basic operations
  if (size < 4) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume pattern string from fuzzer input
  std::string pattern = fdp.ConsumeRandomLengthString(999);
  
  // Consume which CannedOptions to use
  // RE2::CannedOptions enum values: DefaultOptions = 0, Latin1, POSIX, Quiet
  uint8_t options_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
  RE2::CannedOptions canned_options;
  switch (options_choice) {
    case 0:
      canned_options = RE2::DefaultOptions;
      break;
    case 1:
      canned_options = RE2::Latin1;
      break;
    case 2:
      canned_options = RE2::POSIX;
      break;
    case 3:
      canned_options = RE2::Quiet;
      break;
    default:
      canned_options = RE2::DefaultOptions;
      break;
  }
  
  // Create LazyRE2 instance with pattern and CannedOptions
  // Note: LazyRE2 is designed for static initialization, but we can use it
  // as a local variable to test the API
  LazyRE2 lazy_re = {pattern.c_str(), canned_options};
  
  // 1. Call get() method to obtain RE2 pointer (triggers lazy initialization)
  RE2* re_ptr = lazy_re.get();
  
  // Check if pattern compilation succeeded
  if (re_ptr == nullptr) {
    // Pattern compilation failed, which is a valid test case
    return 0;
  }
  
  // 2. Use operator*() to dereference and obtain RE2 reference
  RE2& re_ref = *lazy_re;
  
  // 3. Use operator->() to access RE2 methods directly
  // Verify both pointer and reference point to same object
  if (&re_ref != re_ptr) {
    // This should never happen, but check anyway
    return 0;
  }
  
  // Exercise RE2 methods through operator->()
  bool ok = lazy_re->ok();
  std::string pattern_from_re = lazy_re->pattern();
  RE2::Options options_from_re = lazy_re->options();
  int num_capturing_groups = lazy_re->NumberOfCapturingGroups();
  
  // Test actual regex matching using the obtained RE2 object
  // Consume text to match against
  std::string text = fdp.ConsumeRandomLengthString(999);
  
  // Test FullMatch
  RE2::FullMatch(text, *lazy_re);
  
  // Test PartialMatch
  RE2::PartialMatch(text, *lazy_re);
  
  // Test with string_view
  absl::string_view text_view = text;
  RE2::FullMatch(text_view, *lazy_re);
  
  // Test Consume
  absl::string_view consume_text = text;
  RE2::Consume(&consume_text, *lazy_re);
  
  // Test FindAndConsume
  absl::string_view find_text = text;
  RE2::FindAndConsume(&find_text, *lazy_re);
  
  // If there are capturing groups, test extraction
  if (num_capturing_groups > 0) {
    // Simple extraction tests
    absl::string_view extract_text = text;
    std::string captured;
    RE2::PartialMatch(extract_text, *lazy_re, &captured);
  }
  
  // Test Replace functions
  std::string replace_text = text;
  std::string replacement = fdp.ConsumeRandomLengthString(100);
  RE2::Replace(&replace_text, *lazy_re, replacement);
  
  // Test GlobalReplace
  replace_text = text;
  RE2::GlobalReplace(&replace_text, *lazy_re, replacement);
  
  // Test PossibleMatchRange
  std::string min, max;
  lazy_re->PossibleMatchRange(&min, &max, /*maxlen=*/9);
  
  // Exercise other API functions through operator->
  lazy_re->NamedCapturingGroups();
  lazy_re->CapturingGroupNames();
  
  // Test concurrent access simulation by calling get() multiple times
  // This tests the thread-safe initialization
  for (int i = 0; i < 3; i++) {
    RE2* re_ptr2 = lazy_re.get();
    if (re_ptr2 != re_ptr) {
      // Should always return same pointer
      return 0;
    }
  }
  
  // Test with different patterns based on remaining input
  // Create another LazyRE2 with different CannedOptions
  if (fdp.remaining_bytes() > 10) {
    std::string pattern2 = fdp.ConsumeRandomLengthString(999);
    uint8_t options_choice2 = fdp.ConsumeIntegral<uint8_t>() % 4;
    RE2::CannedOptions canned_options2;
    switch (options_choice2) {
      case 0:
        canned_options2 = RE2::DefaultOptions;
        break;
      case 1:
        canned_options2 = RE2::Latin1;
        break;
      case 2:
        canned_options2 = RE2::POSIX;
        break;
      case 3:
        canned_options2 = RE2::Quiet;
        break;
      default:
        canned_options2 = RE2::DefaultOptions;
        break;
    }
    
    LazyRE2 lazy_re2 = {pattern2.c_str(), canned_options2};
    
    // Exercise the new instance
    RE2* re_ptr2 = lazy_re2.get();
    if (re_ptr2 != nullptr) {
      // Quick test of operator* and operator->
      RE2& re_ref2 = *lazy_re2;
      bool ok2 = lazy_re2->ok();
      
      // Test matching with second pattern
      std::string text2 = fdp.ConsumeRandomLengthString(999);
      RE2::PartialMatch(text2, *lazy_re2);
    }
  }
  
  // Also test with simple patterns that should always compile
  // to ensure we exercise the API even with invalid patterns
  if (fdp.remaining_bytes() > 5) {
    // Try some known simple patterns
    static const char* simple_patterns[] = {
      ".*",
      "\\w+", 
      "\\d+",
      "a*b*c*",
      "[a-z]+",
      "(abc)+",
      "^test$",
      "a|b|c"
    };
    
    uint8_t pattern_idx = fdp.ConsumeIntegral<uint8_t>() % 8;
    const char* simple_pattern = simple_patterns[pattern_idx];
    
    // Use DefaultOptions for simple patterns
    LazyRE2 lazy_simple = {simple_pattern, RE2::DefaultOptions};
    
    // Exercise the simple pattern
    RE2* simple_ptr = lazy_simple.get();
    if (simple_ptr != nullptr) {
      // Test operator-> on simple pattern
      bool simple_ok = lazy_simple->ok();
      std::string simple_text = fdp.ConsumeRandomLengthString(999);
      RE2::PartialMatch(simple_text, *lazy_simple);
    }
  }
  
  return 0;
}

} // namespace harness_006
