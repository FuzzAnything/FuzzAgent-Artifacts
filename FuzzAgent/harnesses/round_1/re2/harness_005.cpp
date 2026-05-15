#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/filtered_re2.h"
#include "re2/re2.h"

namespace harness_005 {
// FilteredRE2 API harness
// Targets: FilteredRE2::Add, FilteredRE2::Compile, FilteredRE2::FirstMatch, 
//          FilteredRE2::AllMatches, FilteredRE2::SlowFirstMatch, FilteredRE2::AllPotentials,
//          FilteredRE2::NumRegexps
// These APIs currently have 0% coverage and represent 50+ branches of undiscovered complexity



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need enough data for patterns, options, and test strings
  if (size < 64) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Create FilteredRE2 instance with optional min_atom_len parameter
  int min_atom_len = fdp.ConsumeIntegralInRange<int>(1, 10);
  re2::FilteredRE2 filtered_re2(min_atom_len);

  // 2. Determine how many patterns to add (1-10 patterns)
  int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 10);

  // 3. Prepare vectors to store pattern IDs and patterns
  std::vector<int> pattern_ids;
  std::vector<std::string> patterns;

  // 4. Configure RE2::Options with various flags from fuzzer input
  re2::RE2::Options options;
  
  // Consume boolean flags for RE2::Options
  options.set_case_sensitive(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_log_errors(fdp.ConsumeBool());
  options.set_literal(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());

  // 5. Add patterns to FilteredRE2
  for (int i = 0; i < num_patterns; i++) {
    if (fdp.remaining_bytes() < 8) break;
    
    // Consume pattern string
    std::string pattern = fdp.ConsumeRandomLengthString(128);
    patterns.push_back(pattern);
    
    int pattern_id = -1;
    re2::RE2::ErrorCode error_code = filtered_re2.Add(pattern, options, &pattern_id);
    
    if (error_code == re2::RE2::NoError && pattern_id != -1) {
      pattern_ids.push_back(pattern_id);
    }
  }

  // If no patterns were successfully added, exit early
  if (pattern_ids.empty()) {
    return 0;
  }

  // 6. Compile the patterns to get filter strings (atoms)
  std::vector<std::string> strings_to_match;
  filtered_re2.Compile(&strings_to_match);

  // 7. Generate test text for matching from remaining fuzzer input
  std::string test_text = fdp.ConsumeRandomLengthString(512);

  // 8. Test SlowFirstMatch (doesn't require compilation)
  int slow_match = filtered_re2.SlowFirstMatch(test_text);

  // 9. Test FirstMatch (requires compilation and atoms)
  // Generate atom indices based on strings_to_match
  std::vector<int> atom_indices;
  if (!strings_to_match.empty()) {
    // Determine how many atoms to use (0 to all)
    int num_atoms_to_use = fdp.ConsumeIntegralInRange<int>(0, strings_to_match.size());
    
    // Generate random atom indices
    for (int i = 0; i < num_atoms_to_use; i++) {
      if (fdp.remaining_bytes() < 1) break;
      int atom_idx = fdp.ConsumeIntegralInRange<int>(0, strings_to_match.size() - 1);
      atom_indices.push_back(atom_idx);
    }
    
    if (!atom_indices.empty()) {
      int first_match = filtered_re2.FirstMatch(test_text, atom_indices);
      // Result can be -1 (no match) or a valid pattern index
    }
  }

  // 10. Test AllMatches
  std::vector<int> matching_regexps;
  if (!atom_indices.empty()) {
    bool all_matches_success = filtered_re2.AllMatches(test_text, atom_indices, &matching_regexps);
    // all_matches_success indicates if the operation completed (not if matches were found)
  }

  // 11. Test AllPotentials
  std::vector<int> potential_regexps;
  if (!atom_indices.empty()) {
    filtered_re2.AllPotentials(atom_indices, &potential_regexps);
  }

  // 12. Use NumRegexps and GetRE2 for validation
  int num_regexps = filtered_re2.NumRegexps();
  
  // Validate that NumRegexps matches the number of successfully added patterns
  if (num_regexps > 0 && num_regexps <= pattern_ids.size()) {
    // Test GetRE2 for some random pattern IDs
    int test_pattern_id = fdp.ConsumeIntegralInRange<int>(0, num_regexps - 1);
    if (test_pattern_id < num_regexps) {
      const re2::RE2& re2_obj = filtered_re2.GetRE2(test_pattern_id);
      // The RE2 object can be used for additional validation if needed
    }
  }

  // 13. Test edge cases: empty text, empty atoms
  if (fdp.remaining_bytes() > 0) {
    // Test with empty text
    std::string empty_text = "";
    filtered_re2.SlowFirstMatch(empty_text);
    
    // Test with empty atom indices
    std::vector<int> empty_atoms;
    int empty_match = filtered_re2.FirstMatch(test_text, empty_atoms);
    
    std::vector<int> empty_matches;
    bool empty_all_matches = filtered_re2.AllMatches(test_text, empty_atoms, &empty_matches);
    
    std::vector<int> empty_potentials;
    filtered_re2.AllPotentials(empty_atoms, &empty_potentials);
  }

  return 0;
}

} // namespace harness_005
