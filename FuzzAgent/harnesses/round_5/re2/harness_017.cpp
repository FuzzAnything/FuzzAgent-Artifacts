#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_017 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing - need enough for patterns, options, and test strings
  if (size < 256) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Strategy: Target RE2::Set operations comprehensively with semantic differentiation
  // from existing harnesses. Focus on testing move assignment operator and both
  // Match overloads extensively, along with all specified APIs.
  
  // Step 1: Create and configure RE2::Options object with various flags
  RE2::Options options;
  
  // Configure encoding (UTF-8 or Latin-1)
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingUTF8 
                                         : RE2::Options::EncodingLatin1);
  
  // Configure various boolean flags based on fuzzer input
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Disable error logging for fuzzing
  options.set_max_mem(fdp.ConsumeIntegralInRange<int64_t>(1 << 20, 64 << 20)); // 1MB to 64MB
  
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());  // Important flag for RE2::Set
  options.set_case_sensitive(fdp.ConsumeBool());
  
  // These flags are only consulted when posix_syntax == true
  if (options.posix_syntax()) {
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
  }
  
  // Step 2: Choose anchor type
  RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
    RE2::UNANCHORED,
    RE2::ANCHOR_START, 
    RE2::ANCHOR_BOTH,
  });
  
  // Step 3: Create RE2::Set object with configured options
  RE2::Set set1(options, anchor);
  
  // Step 4: Add multiple regex patterns using RE2::Set::Add with error strings
  std::vector<std::string> patterns;
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 10);
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 50; i++) {
    // Generate different types of regex patterns
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    std::string pattern;
    
    switch (pattern_type) {
      case 0:
        // Simple word pattern
        pattern = "\\w+";
        break;
      case 1:
        // Digit pattern
        pattern = "\\d+";
        break;
      case 2:
        // Character class pattern
        pattern = "[A-Za-z0-9_]+";
        break;
      case 3:
        // Complex pattern with alternation
        pattern = "(foo|bar|baz)";
        break;
      case 4:
        // Pattern with repetition
        pattern = "a{1,5}b{2,4}";
        break;
      case 5:
        // Pattern with groups (but never_capture might affect these)
        pattern = "(\\d+)-(\\w+)";
        break;
      case 6:
        // Pattern with anchors
        pattern = "^start.*end$";
        break;
      case 7:
        // Pattern with special characters
        pattern = "[!@#$%^&*()]+";
        break;
    }
    
    // Add the pattern to the set
    std::string error;
    int index = set1.Add(pattern, &error);
    // Index can be -1 if pattern is invalid, that's OK
    if (index >= 0) {
      patterns.push_back(pattern);
    }
  }
  
  // Step 5: Call RE2::Set::Compile
  bool compiled = false;
  if (!patterns.empty()) {
    compiled = set1.Compile();
    
    if (compiled) {
      // Step 6: Test RE2::Set::Size
      int set_size = set1.Size();
      // set_size should equal number of successfully added patterns
      
      // Step 7: Perform matches against various input strings using both Match overloads
      size_t num_test_strings = fdp.ConsumeIntegralInRange<size_t>(1, 20);
      
      for (size_t i = 0; i < num_test_strings && fdp.remaining_bytes() > 20; i++) {
        // Generate test string
        std::string test_string = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 500));
        
        // Test Match overload without ErrorInfo
        std::vector<int> matches1;
        bool matched1 = set1.Match(test_string, &matches1);
        
        // Test Match overload with ErrorInfo  
        std::vector<int> matches2;
        RE2::Set::ErrorInfo error_info;
        bool matched2 = set1.Match(test_string, &matches2, &error_info);
        
        // The two Match calls should return the same result
        // (though matches vectors might be in different order)
      }
      
      // Step 8: Test move assignment operator
      // Create a second options object with different settings
      RE2::Options options2;
      options2.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingUTF8 : RE2::Options::EncodingLatin1);
      options2.set_case_sensitive(fdp.ConsumeBool());
      options2.set_never_capture(fdp.ConsumeBool());
      
      RE2::Anchor anchor2 = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
      });
      
      // Create second set
      RE2::Set set2(options2, anchor2);
      
      // Add a different pattern to set2
      std::string pattern2 = fdp.ConsumeRandomLengthString(100);
      std::string error2;
      set2.Add(pattern2, &error2);
      set2.Compile();
      
      // Test that set2 works before move
      std::string test_before = fdp.ConsumeRandomLengthString(50);
      std::vector<int> matches_before;
      set2.Match(test_before, &matches_before);
      
      // Perform move assignment: set2 = std::move(set1)
      set2 = std::move(set1);
      
      // After move, set2 should have set1's patterns
      int size_after_move = set2.Size();
      
      // Test that moved set still works
      if (size_after_move > 0) {
        std::string test_after = fdp.ConsumeRandomLengthString(50);
        std::vector<int> matches_after;
        RE2::Set::ErrorInfo error_info_after;
        set2.Match(test_after, &matches_after, &error_info_after);
      }
      
      // set1 should now be in a moved-from state
      // It's undefined to use it, but we can check Size() (should be 0 or unspecified)
    }
  }
  
  return 0;
}

} // namespace harness_017
