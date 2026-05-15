#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_009 {
// RE2 LazyRE2 lazy compilation lifecycle fuzzing harness
// Targets LazyRE2 class with 0% coverage - implements exact lifecycle sequence
// Semantically unique from harness_008 by directly testing LazyRE2 braced initialization,
// get() method, dereference operators, and RE2 object usage patterns
// Based on coverage guidance: constructor with pattern string, Init() initialization,
// get() method to trigger compilation, dereference operator*(), pointer operator->(),
// using obtained RE2 object for pattern matching



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum input size: need pattern string + some options + text to match
  if (size < 10) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Test 1: Basic LazyRE2 lifecycle with braced initialization
  // Consume pattern string first (variable size)
  std::string pattern1 = fdp.ConsumeRandomLengthString(50);
  
  // Consume options choice (fixed size)
  uint8_t options_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
  
  // Create LazyRE2 with braced initialization based on options choice
  // Note: LazyRE2 has private constructor but supports braced initialization
  re2::LazyRE2 lazy_re2_1;
  
  // Initialize fields directly (as would be done with braced initialization)
  // This simulates: static LazyRE2 re = {"pattern", RE2::Latin1};
  lazy_re2_1.pattern_ = pattern1.c_str();
  
  // Set options based on fuzzer input
  switch (options_choice) {
    case 0:
      // Default UTF-8 encoding
      lazy_re2_1.options_ = re2::RE2::CannedOptions();
      break;
    case 1:
      // Latin1 encoding
      lazy_re2_1.options_ = re2::RE2::Latin1;
      break;
    case 2:
      // Case insensitive with UTF-8 (use Latin1 as proxy)
      lazy_re2_1.options_ = re2::RE2::Latin1;
      break;
    case 3:
      // Default with various flags (simulated)
      lazy_re2_1.options_ = re2::RE2::CannedOptions();
      break;
  }
  
  // Test LazyRE2::get() method to trigger lazy compilation
  // This should call Init() internally via absl::call_once
  re2::RE2* re2_ptr1 = lazy_re2_1.get();
  
  // If RE2 construction failed (pattern invalid), skip further tests
  if (re2_ptr1 == nullptr || !re2_ptr1->ok()) {
    // Still test operator* and operator-> even if nullptr (though shouldn't happen)
    // For safety, check before dereferencing
    if (re2_ptr1 != nullptr) {
      // Test dereference operator*()
      re2::RE2& re2_ref = *lazy_re2_1;
      
      // Test pointer operator->()
      std::string pattern_copy = lazy_re2_1->pattern();
      bool ok_status = lazy_re2_1->ok();
    }
    
    // Continue to next test scenario
  } else {
    // RE2 object is valid, perform comprehensive testing
    
    // Test 1a: Dereference operator*()
    re2::RE2& re2_ref = *lazy_re2_1;
    
    // Test 1b: Pointer operator->() for member access
    std::string retrieved_pattern = lazy_re2_1->pattern();
    re2::RE2::Options retrieved_options = lazy_re2_1->options();
    bool is_ok = lazy_re2_1->ok();
    std::string error_msg = lazy_re2_1->error();
    int num_groups = lazy_re2_1->NumberOfCapturingGroups();
    
    // Test 1c: Use the RE2 object for pattern matching operations
    // Consume text to match against
    std::string text1 = fdp.ConsumeRandomLengthString(100);
    
    // Test FullMatch
    re2::RE2::FullMatch(text1, *re2_ptr1);
    
    // Test PartialMatch  
    re2::RE2::PartialMatch(text1, *re2_ptr1);
    
    // Test Match with start/end positions if we have enough text
    if (text1.size() > 10) {
      size_t start_pos = fdp.ConsumeIntegralInRange<size_t>(0, text1.size() / 2);
      size_t end_pos = fdp.ConsumeIntegralInRange<size_t>(start_pos, text1.size());
      re2::RE2::Anchor anchor = fdp.ConsumeBool() ? re2::RE2::ANCHOR_START : re2::RE2::UNANCHORED;
      
      // Match is an instance method, not static
      std::vector<absl::string_view> matches(1);
      re2_ptr1->Match(text1, start_pos, end_pos, anchor, matches.data(), matches.size());
    }
  }

  // Test 2: Multiple LazyRE2 instances with different patterns and options
  // Determine how many LazyRE2 patterns to test (1-3)
  int num_lazy_patterns = fdp.ConsumeIntegralInRange<int>(1, 3);
  
  for (int i = 0; i < num_lazy_patterns && fdp.remaining_bytes() > 20; i++) {
    // Consume pattern for this LazyRE2
    std::string pattern = fdp.ConsumeRandomLengthString(40);
    
    // Consume options
    uint8_t opts = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Create LazyRE2 with braced initialization simulation
    re2::LazyRE2 lazy_re2;
    lazy_re2.pattern_ = pattern.c_str();
    lazy_re2.options_ = (opts == 0) ? re2::RE2::CannedOptions() : re2::RE2::Latin1;
    
    // Trigger compilation via get()
    re2::RE2* re2_ptr = lazy_re2.get();
    
    if (re2_ptr != nullptr && re2_ptr->ok()) {
      // Test all access methods
      
      // Test get() method returns same pointer
      re2::RE2* re2_ptr_again = lazy_re2.get();
      
      // Test operator*
      re2::RE2& re2_ref = *lazy_re2;
      
      // Test operator->
      std::string pat = lazy_re2->pattern();
      
      // Perform matching operation with remaining text
      if (fdp.remaining_bytes() > 10) {
        std::string text = fdp.ConsumeRandomLengthString(50);
        
        // Choose operation type
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (op_type) {
          case 0:
            re2::RE2::FullMatch(text, *re2_ptr);
            break;
          case 1:
            re2::RE2::PartialMatch(text, *re2_ptr);
            break;
          case 2:
            // Test Replace operations - Replace takes 3 arguments, not 4
            if (fdp.remaining_bytes() > 10) {
              std::string rewrite = fdp.ConsumeRandomLengthString(20);
              std::string str_to_replace = text;
              re2::RE2::Replace(&str_to_replace, *re2_ptr, rewrite);
            }
            break;
        }
      }
    }
  }

  // Test 3: LazyRE2 edge cases and special patterns
  // Test with empty pattern
  if (fdp.remaining_bytes() > 5) {
    re2::LazyRE2 empty_lazy;
    empty_lazy.pattern_ = "";
    empty_lazy.options_ = re2::RE2::CannedOptions();
    
    re2::RE2* empty_ptr = empty_lazy.get();
    if (empty_ptr != nullptr) {
      // Even if construction fails, test APIs
      bool empty_ok = empty_ptr->ok();
      std::string empty_error = empty_ptr->error();
    }
  }

  // Test 4: Complex pattern scenarios that might trigger different compilation paths
  // Consume remaining input for complex pattern testing
  if (fdp.remaining_bytes() > 30) {
    std::string complex_pattern = fdp.ConsumeRandomLengthString(80);
    std::string complex_text = fdp.ConsumeRemainingBytesAsString();
    
    if (!complex_pattern.empty() && !complex_text.empty()) {
      // Create LazyRE2 with the complex pattern
      re2::LazyRE2 complex_lazy;
      complex_lazy.pattern_ = complex_pattern.c_str();
      
      // Choose encoding
      bool use_latin1 = fdp.ConsumeBool();
      complex_lazy.options_ = use_latin1 ? re2::RE2::Latin1 : re2::RE2::CannedOptions();
      
      // Trigger compilation
      re2::RE2* complex_ptr = complex_lazy.get();
      
      if (complex_ptr != nullptr && complex_ptr->ok()) {
        // Test comprehensive RE2 API usage through LazyRE2 interface
        
        // 1. Basic pattern properties
        std::string pattern_str = complex_ptr->pattern();
        re2::RE2::Options options = complex_ptr->options();
        int capturing_groups = complex_ptr->NumberOfCapturingGroups();
        
        // 2. Matching operations
        bool full_match = re2::RE2::FullMatch(complex_text, *complex_ptr);
        bool partial_match = re2::RE2::PartialMatch(complex_text, *complex_ptr);
        
        // 3. Program size (internal metric)
        int program_size = complex_ptr->ProgramSize();
        
        // 4. Possible match range if we have capturing groups
        if (capturing_groups > 0) {
          std::string min_match, max_match;
          complex_ptr->PossibleMatchRange(&min_match, &max_match, 10);
        }
        
        // 5. Test through dereference operators
        re2::RE2& complex_ref = *complex_lazy;
        std::string pattern_via_op = complex_lazy->pattern();
        
        // 6. Test RE2::Match with captured substrings (instance method)
        if (capturing_groups > 0 && complex_text.size() > 10) {
          std::vector<absl::string_view> matches(capturing_groups + 1);
          complex_ptr->Match(complex_text, 0, complex_text.size(), re2::RE2::UNANCHORED,
                            matches.data(), matches.size());
        }
      }
    }
  }

  // Note: We don't delete the RE2 objects created by LazyRE2::Init
  // This is by design - LazyRE2 never deletes the RE2 objects it constructs
  // This is a feature to allow use for global and function static variables

  return 0;
}

} // namespace harness_009
