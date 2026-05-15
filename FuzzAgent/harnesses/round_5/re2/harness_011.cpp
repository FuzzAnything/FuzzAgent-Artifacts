#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "re2/filtered_re2.h"
#include "re2/re2.h"

namespace harness_011 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 32) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Create FilteredRE2 object with potential min_atom_len parameter
  re2::FilteredRE2* filtered_re2 = nullptr;
  if (fdp.ConsumeBool()) {
    // Use default constructor
    filtered_re2 = new re2::FilteredRE2();
  } else {
    // Use constructor with min_atom_len parameter (0-100)
    int min_atom_len = fdp.ConsumeIntegralInRange<int>(0, 100);
    filtered_re2 = new re2::FilteredRE2(min_atom_len);
  }
  
  // Step 2: Create RE2::Options objects with different configurations
  // Create multiple option configurations for diverse pattern addition
  std::vector<RE2::Options> option_configs;
  size_t num_configs = fdp.ConsumeIntegralInRange<size_t>(1, 5);
  
  for (size_t i = 0; i < num_configs && fdp.remaining_bytes() > 0; i++) {
    RE2::Options opts;
    
    // Consume configuration flags from fuzzer input
    uint32_t config_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Set encoding (UTF-8 or Latin1)
    if (config_flags & 0x01) {
      opts.set_encoding(RE2::Options::EncodingLatin1);
    } else {
      opts.set_encoding(RE2::Options::EncodingUTF8);
    }
    
    // Set various option flags based on fuzzed input
    opts.set_posix_syntax((config_flags & 0x02) != 0);
    opts.set_longest_match((config_flags & 0x04) != 0);
    opts.set_log_errors((config_flags & 0x08) != 0);
    opts.set_literal((config_flags & 0x10) != 0);
    opts.set_never_nl((config_flags & 0x20) != 0);
    opts.set_dot_nl((config_flags & 0x40) != 0);
    opts.set_never_capture((config_flags & 0x80) != 0);
    opts.set_case_sensitive((config_flags & 0x100) == 0); // Invert for variety
    
    // Set max memory (0-64MB range for fuzzing)
    if (fdp.remaining_bytes() >= sizeof(int64_t)) {
      int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 64 * 1024 * 1024);
      opts.set_max_mem(max_mem);
    }
    
    // Set posix_syntax-specific options
    if (opts.posix_syntax()) {
      opts.set_perl_classes((config_flags & 0x200) != 0);
      opts.set_word_boundary((config_flags & 0x400) != 0);
      opts.set_one_line((config_flags & 0x800) != 0);
    }
    
    option_configs.push_back(opts);
  }
  
  // Step 3: Call FilteredRE2::Add() multiple times with diverse regex patterns and options
  std::vector<int> pattern_ids;
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 20);
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
    // Choose option configuration for this pattern
    RE2::Options& opts = option_configs[i % option_configs.size()];
    
    // Consume pattern string with various strategies
    std::string pattern;
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (pattern_type) {
      case 0:
        // Empty pattern
        pattern = "";
        break;
      case 1:
        // Simple literal pattern
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 50));
        break;
      case 2:
        // Pattern with regex special characters
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 100));
        // Inject some regex special characters
        if (!pattern.empty()) {
          size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, pattern.size() - 1);
          pattern[pos] = '.';
        }
        break;
      case 3:
        // Pattern with character classes
        pattern = "[";
        pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10));
        pattern += "]";
        break;
      case 4:
        // Pattern with alternation
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20));
        pattern += "|";
        pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20));
        break;
      case 5:
        // Pattern with repetition
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20));
        pattern += "*";
        break;
      case 6:
        // Pattern with grouping
        pattern = "(";
        pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 30));
        pattern += ")";
        break;
      case 7:
        // Pattern with anchors
        pattern = "^";
        pattern += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 30));
        pattern += "$";
        break;
    }
    
    int id = -1;
    RE2::ErrorCode error = filtered_re2->Add(pattern, opts, &id);
    
    // Store pattern ID if addition was successful
    if (error == RE2::NoError && id != -1) {
      pattern_ids.push_back(id);
    }
  }
  
  // Step 4: Call FilteredRE2::Compile() to compile all added patterns
  std::vector<std::string> atoms;
  filtered_re2->Compile(&atoms);
  
  // Step 5: Call FilteredRE2::FirstMatch() with test strings to find first matching pattern
  // Only proceed if we have atoms from compilation
  if (!atoms.empty()) {
    // Generate test strings with various characteristics
    size_t num_test_strings = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_test_strings && fdp.remaining_bytes() > 0; i++) {
      // Consume test string
      std::string test_string;
      uint8_t string_type = fdp.ConsumeIntegral<uint8_t>() % 6;
      
      switch (string_type) {
        case 0:
          // Empty string
          test_string = "";
          break;
        case 1:
          // Short string
          test_string = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 50));
          break;
        case 2:
          // Medium string
          test_string = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(50, 200));
          break;
        case 3:
          // Long string
          test_string = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(200, 1000));
          break;
        case 4:
          // String containing atom matches
          if (!atoms.empty()) {
            size_t atom_idx = fdp.ConsumeIntegralInRange<size_t>(0, atoms.size() - 1);
            test_string = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 50));
            test_string += atoms[atom_idx];
            test_string += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 50));
          } else {
            test_string = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 100));
          }
          break;
        case 5:
          // UTF-8 string with special characters
          test_string = "Hello ";
          test_string += "\xC3\xA9";  // é
          test_string += " ";
          test_string += "\xE2\x82\xAC";  // €
          test_string += " ";
          test_string += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 100));
          break;
      }
      
      // Create atom indices vector for matching
      std::vector<int> atom_indices;
      if (!atoms.empty()) {
        // Determine which atoms to consider as matched
        size_t num_atom_indices = fdp.ConsumeIntegralInRange<size_t>(0, atoms.size());
        
        for (size_t j = 0; j < num_atom_indices; j++) {
          int atom_idx = fdp.ConsumeIntegralInRange<int>(0, atoms.size() - 1);
          atom_indices.push_back(atom_idx);
        }
        
        // Remove duplicates
        std::sort(atom_indices.begin(), atom_indices.end());
        atom_indices.erase(std::unique(atom_indices.begin(), atom_indices.end()), atom_indices.end());
      }
      
      // Test FirstMatch with atom indices
      int first_match = filtered_re2->FirstMatch(test_string, atom_indices);
      
      // Also test SlowFirstMatch (doesn't require atom indices)
      int slow_first_match = filtered_re2->SlowFirstMatch(test_string);
      
      // Step 6: Call FilteredRE2::AllMatches() to find all matching patterns for a string
      std::vector<int> matching_regexps;
      bool all_matches_result = filtered_re2->AllMatches(test_string, atom_indices, &matching_regexps);
      
      // Step 7: Call FilteredRE2::AllPotentials() to get potential pattern IDs
      std::vector<int> potential_regexps;
      filtered_re2->AllPotentials(atom_indices, &potential_regexps);
    }
  }
  
  // Additional tests with edge cases
  
  // Test with empty atom indices
  if (!atoms.empty()) {
    std::vector<int> empty_atom_indices;
    std::string test_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 100));
    
    int first_match_empty = filtered_re2->FirstMatch(test_text, empty_atom_indices);
    std::vector<int> matching_regexps_empty;
    bool all_matches_empty = filtered_re2->AllMatches(test_text, empty_atom_indices, &matching_regexps_empty);
    std::vector<int> potential_regexps_empty;
    filtered_re2->AllPotentials(empty_atom_indices, &potential_regexps_empty);
  }
  
  // Test with all atom indices
  if (!atoms.empty()) {
    std::vector<int> all_atom_indices;
    for (size_t i = 0; i < atoms.size(); i++) {
      all_atom_indices.push_back(i);
    }
    
    std::string test_text_all = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 100));
    
    int first_match_all = filtered_re2->FirstMatch(test_text_all, all_atom_indices);
    std::vector<int> matching_regexps_all;
    bool all_matches_all = filtered_re2->AllMatches(test_text_all, all_atom_indices, &matching_regexps_all);
    std::vector<int> potential_regexps_all;
    filtered_re2->AllPotentials(all_atom_indices, &potential_regexps_all);
  }
  
  // Test GetRE2 method for individual regexps
  if (!pattern_ids.empty()) {
    for (int id : pattern_ids) {
      if (id >= 0 && id < filtered_re2->NumRegexps()) {
        const RE2& re = filtered_re2->GetRE2(id);
        // Test basic operations on the individual RE2 object
        std::string test_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 100));
        bool full_match = RE2::FullMatch(test_text, re);
        bool partial_match = RE2::PartialMatch(test_text, re);
      }
    }
  }
  
  // Test NumRegexps method
  int num_regexps = filtered_re2->NumRegexps();
  
  // Clean up
  delete filtered_re2;
  
  return 0;
}

} // namespace harness_011
