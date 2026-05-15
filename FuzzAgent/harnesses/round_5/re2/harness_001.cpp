#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"
#include "re2/filtered_re2.h"

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 16) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume RE2::Options settings
  RE2::Options options;
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);
  options.set_max_mem(64 << 20);
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_case_sensitive(!fdp.ConsumeBool());
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  
  // Consume anchor type
  RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
      RE2::UNANCHORED,
      RE2::ANCHOR_START,
      RE2::ANCHOR_BOTH,
  });
  
  // Test RE2::Set functionality
  RE2::Set set(options, anchor);
  
  // Add some patterns to the set
  std::vector<std::string> patterns;
  int pattern_count = fdp.ConsumeIntegralInRange<int>(1, 5);
  
  for (int i = 0; i < pattern_count && fdp.remaining_bytes() > 10; i++) {
    std::string pattern = fdp.ConsumeRandomLengthString(100);
    patterns.push_back(pattern);
    
    std::string error;
    int index = set.Add(pattern, &error);
    // index can be -1 if pattern is invalid, that's OK
  }
  
  // Try to compile the set if we added any patterns
  if (patterns.size() > 0) {
    bool compiled = set.Compile();
    if (compiled) {
      // Test matching against some text
      std::string text = fdp.ConsumeRandomLengthString(500);
      std::vector<int> matches;
      set.Match(text, &matches);
      
      // Also test with error_info
      RE2::Set::ErrorInfo error_info;
      std::vector<int> matches2;
      set.Match(text, &matches2, &error_info);
    }
    
    // Test Size() method
    int set_size = set.Size();
  }
  
  // Test FilteredRE2 functionality
  re2::FilteredRE2 filter;
  
  // Add some patterns to FilteredRE2
  int filter_pattern_count = fdp.ConsumeIntegralInRange<int>(1, 5);
  
  for (int i = 0; i < filter_pattern_count && fdp.remaining_bytes() > 10; i++) {
    std::string pattern = fdp.ConsumeRandomLengthString(100);
    int id = -1;
    RE2::ErrorCode error = filter.Add(pattern, options, &id);
    // error can be non-zero if pattern is invalid, that's OK
  }
  
  // Try to compile FilteredRE2
  if (filter.NumRegexps() > 0) {
    std::vector<std::string> atoms;
    filter.Compile(&atoms);
    
    // Test matching with some atoms
    if (!atoms.empty()) {
      std::string text = fdp.ConsumeRandomLengthString(500);
      
      // Create a vector of atom indices (simulate some atoms matching)
      std::vector<int> matched_atoms;
      int atom_count = fdp.ConsumeIntegralInRange<int>(0, std::min(5, (int)atoms.size()));
      for (int i = 0; i < atom_count; i++) {
        matched_atoms.push_back(i % atoms.size());
      }
      
      // Test FirstMatch
      int first_match = filter.FirstMatch(text, matched_atoms);
      
      // Test AllMatches
      std::vector<int> matching_regexps;
      bool all_matches_result = filter.AllMatches(text, matched_atoms, &matching_regexps);
      
      // Test AllPotentials
      std::vector<int> potential_regexps;
      filter.AllPotentials(matched_atoms, &potential_regexps);
      
      // Test SlowFirstMatch
      int slow_first_match = filter.SlowFirstMatch(text);
    }
  }
  
  // Test individual RE2 objects from FilteredRE2
  if (filter.NumRegexps() > 0) {
    // Get a RE2 object from the filter
    const RE2& re = filter.GetRE2(0);
    
    // Test some basic operations on it
    std::string test_text = fdp.ConsumeRandomLengthString(200);
    RE2::FullMatch(test_text, re);
    RE2::PartialMatch(test_text, re);
  }
  
  return 0;
}

} // namespace harness_001
