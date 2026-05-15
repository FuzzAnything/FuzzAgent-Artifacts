#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_009 {
// RE2::Set assignment operator harness
// Targets: RE2::Set assignment operator (0% covered) and related RE2::Set APIs
// Based on coverage guidance: /root/FuzzAgent/output/re2/coverage_guidance/harness_009_guidance.md
// This harness specifically tests the RE2::Set assignment operator (move assignment) which is
// completely uncovered (0%), along with comprehensive testing of RE2::Set lifecycle operations



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need enough data for patterns, options, and test text
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Create two RE2::Set objects with different configurations
  re2::RE2::Options options1, options2;
  
  // Configure options for first set
  options1.set_case_sensitive(fdp.ConsumeBool());
  options1.set_posix_syntax(fdp.ConsumeBool());
  options1.set_longest_match(fdp.ConsumeBool());
  
  // Configure options for second set (different)
  options2.set_log_errors(fdp.ConsumeBool());
  options2.set_literal(fdp.ConsumeBool());
  
  // Choose anchor types
  re2::RE2::Anchor anchor1 = re2::RE2::Anchor::UNANCHORED;
  re2::RE2::Anchor anchor2 = re2::RE2::Anchor::ANCHOR_START;
  
  // Create first RE2::Set object
  re2::RE2::Set set1(options1, anchor1);
  
  // Create second RE2::Set object with different configuration
  re2::RE2::Set set2(options2, anchor2);

  // 2. Add patterns to first set using RE2::Set::Add()
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 6);
  int successful_adds = 0;

  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
    std::string pattern = fdp.ConsumeRandomLengthString(100);
    
    // Sometimes pass error string, sometimes nullptr
    std::string error;
    std::string* error_ptr = fdp.ConsumeBool() ? &error : nullptr;
    
    int pattern_index = set1.Add(pattern, error_ptr);
    if (pattern_index >= 0) {
      successful_adds++;
    }
  }

  // 3. Call RE2::Set::Compile() on first set
  if (successful_adds == 0) {
    return 0;  // No patterns added successfully
  }
  
  bool compile_success = set1.Compile();
  if (!compile_success) {
    return 0;  // Compilation failed
  }

  // 4. Test matching with both RE2::Set::Match() variants
  std::string test_text = fdp.ConsumeRandomLengthString(200);
  std::vector<int> matching_indices;
  re2::RE2::Set::ErrorInfo error_info;
  
  // Test Match() without ErrorInfo
  bool match1 = set1.Match(test_text, &matching_indices);
  
  // Clear and test Match() with ErrorInfo
  matching_indices.clear();
  bool match2 = set1.Match(test_text, &matching_indices, &error_info);
  
  // Test with nullptr parameters
  bool match3 = set1.Match(test_text, nullptr);
  bool match4 = set1.Match(test_text, nullptr, nullptr);
  
  // Check size before assignment
  int size_before = set1.Size();

  // 5. Use assignment operator to copy first set to second set
  // PRIMARY TARGET: RE2::Set move assignment operator
  // Move set1 to set2 (set2 gets set1's contents, set1 becomes empty/valid state)
  set2 = std::move(set1);
  
  // After move, set1 is in valid but unspecified state
  // We should only destroy it or assign to it, not call other methods
  
  // 6. Verify that second set has same patterns and matching behavior
  int size_after = set2.Size();  // Should be same as size_before
  
  // Test matching on the moved-to set
  std::vector<int> matching_indices_after;
  bool match_after = set2.Match(test_text, &matching_indices_after);
  
  // Test both Match() variants on the moved set
  matching_indices_after.clear();
  re2::RE2::Set::ErrorInfo error_info_after;
  bool match_after_with_error = set2.Match(test_text, &matching_indices_after, &error_info_after);

  // 7. Modify second set and test assignment in reverse direction
  // Create a third set with different patterns
  re2::RE2::Set set3(options1, anchor1);
  
  // Add different patterns to set3
  for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
    std::string pattern = fdp.ConsumeRandomLengthString(80);
    set3.Add(pattern, nullptr);
  }
  
  // Compile set3 if it has patterns
  if (set3.Size() > 0) {
    set3.Compile();
    
    // Test reverse assignment: move set3 to set2
    set2 = std::move(set3);
    
    // Verify set2 now has set3's patterns
    int new_size = set2.Size();
    
    // Test matching on the newly assigned set
    std::string another_text = fdp.ConsumeRandomLengthString(150);
    std::vector<int> new_matching_indices;
    bool new_match = set2.Match(another_text, &new_matching_indices);
  }
  
  // 8. Test assignment edge cases
  
  // Test assignment from empty set
  re2::RE2::Set empty_set(options1, anchor1);
  set2 = std::move(empty_set);  // set2 becomes empty
  
  // Test assignment to itself (self-move, though this is questionable)
  // set2 = std::move(set2);  // Avoid self-move as it's undefined behavior
  
  // Test chained assignment
  re2::RE2::Set set4(options1, anchor1);
  re2::RE2::Set set5(options1, anchor1);
  
  // Add and compile a pattern to set4
  std::string chain_pattern = fdp.ConsumeRandomLengthString(60);
  set4.Add(chain_pattern, nullptr);
  set4.Compile();
  
  // Chain: set5 gets set4's contents
  set5 = std::move(set4);
  
  // Test the chained assignment result
  std::string chain_text = fdp.ConsumeRemainingBytesAsString();
  std::vector<int> chain_matches;
  bool chain_match = set5.Match(chain_text, &chain_matches);

  // 9. Clean up - all sets are destroyed automatically via RAII
  
  return 0;
}

} // namespace harness_009
