#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"

namespace harness_009 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 32) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Create RE2::Options object with fuzzed configuration
  RE2::Options options;
  
  // Consume configuration flags from fuzzer input
  uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
  
  // Configure options based on fuzzed input
  if (config_flags & 0x01) options.set_encoding(RE2::Options::EncodingLatin1);
  if (config_flags & 0x02) options.set_posix_syntax(true);
  if (config_flags & 0x04) options.set_longest_match(true);
  if (config_flags & 0x08) options.set_log_errors(false);
  if (config_flags & 0x10) options.set_literal(true);
  if (config_flags & 0x20) options.set_never_nl(true);
  if (config_flags & 0x40) options.set_dot_nl(true);
  if (config_flags & 0x80) options.set_never_capture(true);
  
  // Consume more flags for additional options
  if (fdp.remaining_bytes() > 0) {
    uint8_t more_flags = fdp.ConsumeIntegral<uint8_t>();
    if (more_flags & 0x01) options.set_case_sensitive(false);
    if (more_flags & 0x02) options.set_perl_classes(true);
    if (more_flags & 0x04) options.set_word_boundary(true);
    if (more_flags & 0x08) options.set_one_line(true);
    
    // Set max memory from fuzzed input (0-64KB range for fuzzing)
    if (fdp.remaining_bytes() >= sizeof(int64_t)) {
      int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 65536);
      options.set_max_mem(max_mem);
    }
  }
  
  // Determine number of patterns to test (1-5 for reasonable fuzzing)
  size_t num_patterns = 1;
  if (fdp.remaining_bytes() >= sizeof(uint8_t)) {
    num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 5);
  }
  
  // Create RE2 objects with various patterns
  std::vector<std::unique_ptr<RE2>> re2_objects;
  std::vector<std::string> pattern_strings;
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
    // Consume pattern string (some may be invalid to test error paths)
    size_t max_pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    std::string pattern = fdp.ConsumeRandomLengthString(max_pattern_len);
    
    // Create RE2 object with the pattern
    auto re2 = std::make_unique<RE2>(pattern, options);
    re2_objects.push_back(std::move(re2));
    pattern_strings.push_back(pattern);
  }
  
  // If we have at least one RE2 object, test the operations
  for (size_t i = 0; i < re2_objects.size(); i++) {
    RE2* re = re2_objects[i].get();
    
    // Step 2: Test error reporting functions for patterns that failed compilation
    if (!re->ok()) {
      // These are currently 0% covered - exercise them for invalid patterns
      const std::string& error_msg = re->error();
      const std::string& error_arg_msg = re->error_arg();
      
      // Even though pattern is invalid, we can still test some operations
      // but they should fail gracefully
      if (fdp.remaining_bytes() > 0) {
        size_t text_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::string text = fdp.ConsumeRandomLengthString(text_len);
        
        size_t rewrite_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::string rewrite = fdp.ConsumeRandomLengthString(rewrite_len);
        
        // Try Extract - should fail with invalid pattern
        std::string extract_out;
        RE2::Extract(text, *re, rewrite, &extract_out);
        
        // Try Replace - should fail with invalid pattern
        std::string replace_text = text;
        RE2::Replace(&replace_text, *re, rewrite);
        
        // Try GlobalReplace - should fail with invalid pattern
        std::string global_replace_text = text;
        RE2::GlobalReplace(&global_replace_text, *re, rewrite);
      }
      continue; // Skip further testing for invalid patterns
    }
    
    // Pattern is valid, test extraction and replacement operations
    
    // Consume test strings for this pattern
    if (fdp.remaining_bytes() < 10) break;
    
    // Consume text to operate on
    size_t text_len = fdp.ConsumeIntegralInRange<size_t>(1, 200);
    std::string text = fdp.ConsumeRandomLengthString(text_len);
    
    // Consume rewrite string (may include backreferences like \1, \2, etc.)
    size_t rewrite_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    std::string rewrite = fdp.ConsumeRandomLengthString(rewrite_len);
    
    // Step 3: Call RE2::Extract on input strings to extract matched substrings
    std::string extract_result;
    bool extract_success = RE2::Extract(text, *re, rewrite, &extract_result);
    
    // Step 4: Call RE2::Replace on strings to perform single replacements
    std::string replace_text = text;
    bool replace_success = RE2::Replace(&replace_text, *re, rewrite);
    
    // Step 5: Call RE2::GlobalReplace on strings to perform global replacements
    std::string global_replace_text = text;
    int global_replace_count = RE2::GlobalReplace(&global_replace_text, *re, rewrite);
    
    // Test with different rewrite strings that include backreferences
    if (fdp.remaining_bytes() > 20 && re->NumberOfCapturingGroups() > 0) {
      // Create rewrite strings with backreferences
      std::vector<std::string> backref_rewrites;
      backref_rewrites.push_back("\\0");  // Entire match
      backref_rewrites.push_back("\\1");  // First capture group
      backref_rewrites.push_back("\\2");  // Second capture group
      backref_rewrites.push_back("\\0 and \\1");  // Combination
      backref_rewrites.push_back("Prefix \\1 suffix");  // With text
      
      for (const auto& br_rewrite : backref_rewrites) {
        if (fdp.remaining_bytes() < 10) break;
        
        std::string br_extract_result;
        RE2::Extract(text, *re, br_rewrite, &br_extract_result);
        
        std::string br_replace_text = text;
        RE2::Replace(&br_replace_text, *re, br_rewrite);
        
        std::string br_global_replace_text = text;
        RE2::GlobalReplace(&br_global_replace_text, *re, br_rewrite);
      }
    }
    
    // Test with empty rewrite string
    std::string empty_extract_result;
    RE2::Extract(text, *re, "", &empty_extract_result);
    
    std::string empty_replace_text = text;
    RE2::Replace(&empty_replace_text, *re, "");
    
    std::string empty_global_replace_text = text;
    RE2::GlobalReplace(&empty_global_replace_text, *re, "");
  }
  
  // Step 6: Test with additional patterns created from remaining input
  while (fdp.remaining_bytes() > 50) {
    // Create one more pattern to test edge cases
    size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
    std::string pattern = fdp.ConsumeRandomLengthString(pattern_len);
    
    RE2 re(pattern, options);
    
    // Test error functions even for this temporary object
    if (!re.ok()) {
      const std::string& error_msg = re.error();
      const std::string& error_arg_msg = re.error_arg();
    } else {
      // Quick test of operations if pattern is valid
      if (fdp.remaining_bytes() > 20) {
        size_t text_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
        std::string text = fdp.ConsumeRandomLengthString(text_len);
        
        size_t rewrite_len = fdp.ConsumeIntegralInRange<size_t>(1, 30);
        std::string rewrite = fdp.ConsumeRandomLengthString(rewrite_len);
        
        std::string extract_out;
        RE2::Extract(text, re, rewrite, &extract_out);
        
        std::string replace_text = text;
        RE2::Replace(&replace_text, re, rewrite);
        
        std::string global_replace_text = text;
        RE2::GlobalReplace(&global_replace_text, re, rewrite);
      }
    }
  }
  
  // Step 7: Clean up is handled automatically by unique_ptr destructors
  
  return 0;
}

} // namespace harness_009
