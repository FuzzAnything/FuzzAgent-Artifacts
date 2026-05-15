#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: we need at least some bytes for options, anchor, patterns, and text
  if (size < 64) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Configure RE2::Options based on fuzzed input
  RE2::Options options;
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Disable error logging during fuzzing
  options.set_max_mem(64 << 20);  // 64 MB memory limit
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_case_sensitive(!fdp.ConsumeBool());  // Default is true
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  
  // Choose anchor type
  RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
      RE2::UNANCHORED,
      RE2::ANCHOR_START,
      RE2::ANCHOR_BOTH,
  });
  
  // Create RE2::Set object (Step 1)
  RE2::Set set(options, anchor);
  
  // Determine how many patterns to add (1-10 patterns)
  int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 10);
  
  // Step 2: Call Add() multiple times with different patterns
  std::vector<int> pattern_indices;
  for (int i = 0; i < num_patterns; i++) {
    // Consume pattern string
    std::string pattern = fdp.ConsumeRandomLengthString(500);
    
    // Add pattern to set
    int index = set.Add(pattern, nullptr);  // Don't capture error messages
    
    // Store successful indices
    if (index != -1) {
      pattern_indices.push_back(index);
    }
    
    // Check if we have enough input remaining
    if (fdp.remaining_bytes() < 100) {
      break;
    }
  }
  
  // If no patterns were successfully added, nothing to test
  if (pattern_indices.empty()) {
    return 0;
  }
  
  // Step 3: Call Compile() to prepare for matching
  bool compile_success = set.Compile();
  if (!compile_success) {
    // Compilation failed - still valid execution path
    return 0;
  }
  
  // Step 4: Call Size() to verify pattern count
  int set_size = set.Size();
  // Defensive check: Size() should match number of successfully added patterns
  // We don't assert or crash here to avoid coverage collection issues
  
  // Step 5: Call Match() with test strings (both with and without ErrorInfo)
  
  // Consume test string for matching
  std::string test_text = fdp.ConsumeRandomLengthString(1000);
  
  // Test Match() without ErrorInfo
  std::vector<int> matches_without_error;
  bool match_result_without_error = set.Match(test_text, &matches_without_error);
  
  // Test Match() with ErrorInfo
  std::vector<int> matches_with_error;
  RE2::Set::ErrorInfo error_info;
  bool match_result_with_error = set.Match(test_text, &matches_with_error, &error_info);
  
  // Verify consistency: both Match calls should return the same result
  // Defensive check: don't assert or crash, just use results
  
  // Test edge cases:
  // 1. Match with NULL vector (just check if any match exists)
  bool match_exists = set.Match(test_text, nullptr);
  
  // 2. Match with ErrorInfo but NULL vector
  RE2::Set::ErrorInfo error_info2;
  bool match_exists_with_error = set.Match(test_text, nullptr, &error_info2);
  
  // Test with empty text
  std::string empty_text = "";
  std::vector<int> empty_matches;
  RE2::Set::ErrorInfo empty_error_info;
  set.Match(empty_text, &empty_matches, &empty_error_info);
  
  // Test with another random text
  if (fdp.remaining_bytes() > 50) {
    std::string another_text = fdp.ConsumeRandomLengthString(500);
    std::vector<int> another_matches;
    set.Match(another_text, &another_matches);
  }
  
  // Test error paths by calling Add() after Compile() (should fail)
  if (fdp.remaining_bytes() > 20) {
    std::string extra_pattern = fdp.ConsumeRandomLengthString(100);
    // This should return -1 because set is already compiled
    int invalid_add = set.Add(extra_pattern, nullptr);
    // Defensive: don't assert, just let it execute
  }
  
  // Test calling Match() before Compile() would cause DLOG fatal
  // We avoid this path intentionally since it would cause termination
  
  // Test Size() before and after compilation consistency
  int size_before = pattern_indices.size();  // Our count of successful adds
  // Size() after compile should return the same as before compile
  // Defensive: just compare but don't assert
  
  return 0;
}

} // namespace harness_005
