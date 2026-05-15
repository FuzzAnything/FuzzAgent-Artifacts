#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "absl/strings/string_view.h"
#include "re2/filtered_re2.h"
#include "re2/re2.h"

namespace harness_006 {
// RE2 FilteredRE2 multi-regex matching fuzzing harness
// Targets FilteredRE2 functionality with prefilter optimization
// Focuses on uncovered APIs: FirstMatch (12 undiscovered branches), 
// SlowFirstMatch (4 undiscovered branches), Add (9/10 undiscovered branches),
// Compile (8/12 undiscovered branches), AllPotentials (uncovered)
// Implements the complete FilteredRE2 lifecycle for semantic diversity



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Limit input size for efficiency
  if (size == 0 || size > 4096) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Create FilteredRE2 object (constructor)
  // Use default constructor for this harness
  re2::FilteredRE2 filtered_re2;

  // Determine how many regex patterns to add (1-5)
  int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 5);
  std::vector<int> pattern_ids;
  std::vector<RE2::Options> pattern_options;

  // Add multiple regex patterns with different options
  for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
    // Configure RE2 options from fuzzer input
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
    
    pattern_options.push_back(options);
    
    // Consume pattern string from fuzzer input
    // Use ConsumeRandomLengthString with reasonable limit
    std::string pattern = fdp.ConsumeRandomLengthString(100);
    
    // Call FilteredRE2::Add with pattern and options
    int pattern_id = -1;
    RE2::ErrorCode error = filtered_re2.Add(pattern, options, &pattern_id);
    
    if (pattern_id != -1) {
      pattern_ids.push_back(pattern_id);
    }
    
    // If we're running low on input, break early
    if (fdp.remaining_bytes() < 20) {
      break;
    }
  }

  // If no patterns were successfully added, return early
  if (pattern_ids.empty()) {
    return 0;
  }

  // Call FilteredRE2::Compile to build the prefilter
  std::vector<std::string> atoms;
  filtered_re2.Compile(&atoms);
  
  // At this point, the FilteredRE2 is compiled and ready for matching
  // Now we need to test the matching operations

  // Consume text to match against
  std::string text = fdp.ConsumeRandomLengthString(200);
  
  // Prepare atoms vector for FirstMatch and AllPotentials
  // The atoms vector from Compile contains strings that need to be found in text
  // For fuzzing, we'll simulate which atoms are found based on the text
  std::vector<int> matched_atoms;
  
  // Simple simulation: check if each atom appears in the text (case-insensitive)
  // This is a simplification; in real usage, the caller would do proper string matching
  for (size_t i = 0; i < atoms.size(); i++) {
    // Check if atom appears in text (case-insensitive check)
    // Convert both to lowercase for simple case-insensitive check
    std::string atom_lower = atoms[i];
    std::string text_lower = text;
    
    // Simple lowercase conversion (ASCII only for fuzzing)
    for (char& c : atom_lower) {
      if (c >= 'A' && c <= 'Z') c += 32;
    }
    for (char& c : text_lower) {
      if (c >= 'A' && c <= 'Z') c += 32;
    }
    
    if (text_lower.find(atom_lower) != std::string::npos) {
      matched_atoms.push_back(static_cast<int>(i));
    }
  }

  // Test 1: Call FilteredRE2::FirstMatch with input text and filter vector
  // This requires Compile to have been called (which we did)
  int first_match_result = -1;
  if (filtered_re2.NumRegexps() > 0 && !matched_atoms.empty()) {
    first_match_result = filtered_re2.FirstMatch(text, matched_atoms);
  }

  // Test 2: Call FilteredRE2::SlowFirstMatch as fallback
  // This doesn't require Compile and doesn't use atoms
  int slow_first_match_result = filtered_re2.SlowFirstMatch(text);
  
  // Test 3: Call FilteredRE2::AllPotentials to get all matching regex indices
  // This returns all regexps that pass the filter (may still not match)
  std::vector<int> potential_regexps;
  if (!matched_atoms.empty()) {
    filtered_re2.AllPotentials(matched_atoms, &potential_regexps);
  }

  // Test 4: Also test AllMatches if we have atoms
  std::vector<int> matching_regexps;
  if (!matched_atoms.empty()) {
    filtered_re2.AllMatches(text, matched_atoms, &matching_regexps);
  }

  // Additional testing: Test with empty atoms vector
  // This should return no matches but shouldn't crash
  std::vector<int> empty_atoms;
  filtered_re2.FirstMatch(text, empty_atoms);
  
  std::vector<int> empty_potentials;
  filtered_re2.AllPotentials(empty_atoms, &empty_potentials);
  
  std::vector<int> empty_matches;
  filtered_re2.AllMatches(text, empty_atoms, &empty_matches);

  // Test edge cases: text that likely won't match any pattern
  std::string unlikely_text = "xyz123abc789";
  filtered_re2.SlowFirstMatch(unlikely_text);
  
  if (!matched_atoms.empty()) {
    filtered_re2.FirstMatch(unlikely_text, matched_atoms);
  }

  // Test with very short text
  std::string short_text = "a";
  filtered_re2.SlowFirstMatch(short_text);
  
  // Test destructor cleanup happens automatically when filtered_re2 goes out of scope

  return 0;
}

} // namespace harness_006
