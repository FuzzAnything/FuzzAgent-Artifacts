#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "re2/re2.h"

namespace harness_012 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 32) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Create various RE2 objects with different patterns
  
  // Consume patterns from fuzzer input
  std::vector<std::string> patterns;
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 10);
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
    // Generate different types of patterns
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    std::string pattern;
    
    switch (pattern_type) {
      case 0:
        // Simple literal pattern
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 50));
        break;
      case 1:
        // Pattern with capture groups
        pattern = "(" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20)) + 
                  ")+(" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20)) + ")";
        break;
      case 2:
        // Pattern with character classes
        pattern = "[" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  "]+\\d{" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 5)) + "}";
        break;
      case 3:
        // Pattern with alternation
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 15)) + 
                  "|" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 15));
        break;
      case 4:
        // Pattern with quantifiers
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  "{1," + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10)) + "}";
        break;
      case 5:
        // Pattern with anchors
        pattern = "^" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20)) + "$";
        break;
      case 6:
        // Pattern with backreferences (simplified)
        pattern = "(" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5)) + 
                  ")\\1";
        break;
      case 7:
        // Empty or very short pattern (edge case)
        if (fdp.ConsumeBool()) {
          pattern = "";
        } else {
          pattern = fdp.ConsumeRandomLengthString(1);
        }
        break;
    }
    
    patterns.push_back(pattern);
  }
  
  // Create RE2 objects with different options
  std::vector<std::unique_ptr<RE2>> re2_objects;
  
  for (const auto& pattern : patterns) {
    if (fdp.remaining_bytes() == 0) break;
    
    RE2::Options options;
    
    // Configure options based on fuzzed input
    uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
    
    if (config_flags & 0x01) options.set_encoding(RE2::Options::EncodingLatin1);
    if (config_flags & 0x02) options.set_posix_syntax(true);
    if (config_flags & 0x04) options.set_longest_match(true);
    if (config_flags & 0x08) options.set_log_errors(false);
    if (config_flags & 0x10) options.set_literal(true);
    if (config_flags & 0x20) options.set_never_nl(true);
    if (config_flags & 0x40) options.set_dot_nl(true);
    if (config_flags & 0x80) options.set_never_capture(true);
    options.set_case_sensitive((config_flags & 0x100) == 0); // Default true
    
    // Set max memory
    if (fdp.remaining_bytes() >= sizeof(int64_t)) {
      int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 64 * 1024 * 1024);
      options.set_max_mem(max_mem);
    }
    
    // Set posix_syntax-specific options
    if (options.posix_syntax()) {
      if (fdp.ConsumeBool()) options.set_perl_classes(true);
      if (fdp.ConsumeBool()) options.set_word_boundary(true);
      if (fdp.ConsumeBool()) options.set_one_line(true);
    }
    
    auto re2_obj = std::make_unique<RE2>(pattern, options);
    if (re2_obj->ok()) {
      re2_objects.push_back(std::move(re2_obj));
    }
  }
  
  if (re2_objects.empty()) return 0;
  
  // Step 2: Test Extract API: Use RE2::Extract to extract substrings from input text
  
  // Consume text for extraction
  std::string extract_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 500));
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(5)) && fdp.remaining_bytes() > 0; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Consume rewrite string with possible backreferences
    std::string rewrite;
    uint8_t rewrite_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (rewrite_type) {
      case 0:
        // Simple replacement
        rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 50));
        break;
      case 1:
        // Rewrite with backreferences
        rewrite = "\\0";  // Whole match
        break;
      case 2:
        // Rewrite with specific group references
        if (re2.NumberOfCapturingGroups() > 0) {
          rewrite = "\\1";
          if (re2.NumberOfCapturingGroups() > 1 && fdp.ConsumeBool()) {
            rewrite += " and \\2";
          }
        } else {
          rewrite = fdp.ConsumeRandomLengthString(20);
        }
        break;
      case 3:
        // Empty rewrite
        rewrite = "";
        break;
    }
    
    std::string extract_result;
    RE2::Extract(extract_text, re2, rewrite, &extract_result);
    
    // Test CheckRewriteString
    std::string rewrite_error;
    re2.CheckRewriteString(rewrite, &rewrite_error);
    
    // Test MaxSubmatch
    int max_submatch = RE2::MaxSubmatch(rewrite);
  }
  
  // Step 3: Test Replace API: Use RE2::Replace to perform single replacements on strings
  
  // Consume text for replacement
  std::string replace_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 500));
  std::string original_replace_text = replace_text; // Keep copy
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(5)) && fdp.remaining_bytes() > 0; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Consume rewrite string
    std::string rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 100));
    
    // Test Replace
    bool replace_success = RE2::Replace(&replace_text, re2, rewrite);
    
    // Reset for next test
    replace_text = original_replace_text;
  }
  
  // Step 4: Test GlobalReplace API: Use RE2::GlobalReplace to perform global replacements
  
  std::string global_replace_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 500));
  std::string original_global_text = global_replace_text;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(5)) && fdp.remaining_bytes() > 0; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Consume rewrite string
    std::string rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 100));
    
    // Test GlobalReplace
    int num_replacements = RE2::GlobalReplace(&global_replace_text, re2, rewrite);
    
    // Reset for next test
    global_replace_text = original_global_text;
  }
  
  // Step 5: Combine operations: Extract -> Replace -> GlobalReplace on same data
  
  if (fdp.remaining_bytes() > 100) {
    std::string combined_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(50, 300));
    std::string original_combined = combined_text;
    
    // Use first valid RE2 object
    const auto& re2 = *re2_objects[0];
    
    // Extract operation
    std::string extract_rewrite = fdp.ConsumeRandomLengthString(50);
    std::string extracted;
    RE2::Extract(combined_text, re2, extract_rewrite, &extracted);
    
    // Replace operation
    std::string replace_rewrite = fdp.ConsumeRandomLengthString(50);
    RE2::Replace(&combined_text, re2, replace_rewrite);
    
    // GlobalReplace operation on modified text
    std::string global_rewrite = fdp.ConsumeRandomLengthString(50);
    RE2::GlobalReplace(&combined_text, re2, global_rewrite);
  }
  
  // Step 6: Test edge cases: empty strings, no matches, overlapping matches, capture group references
  
  // Edge case 1: Empty text
  std::string empty_text = "";
  std::string empty_result;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(2)); i++) {
    const auto& re2 = *re2_objects[i];
    std::string rewrite = fdp.ConsumeRandomLengthString(20);
    
    RE2::Extract(empty_text, re2, rewrite, &empty_result);
    RE2::Replace(&empty_text, re2, rewrite);
    RE2::GlobalReplace(&empty_text, re2, rewrite);
  }
  
  // Edge case 2: Text with no possible matches
  std::string no_match_text = "XYZ123";
  std::string no_match_result;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(2)); i++) {
    const auto& re2 = *re2_objects[i];
    std::string rewrite = fdp.ConsumeRandomLengthString(20);
    
    RE2::Extract(no_match_text, re2, rewrite, &no_match_result);
    
    std::string temp = no_match_text;
    RE2::Replace(&temp, re2, rewrite);
    
    temp = no_match_text;
    RE2::GlobalReplace(&temp, re2, rewrite);
  }
  
  // Edge case 3: Test with various rewrite strings including malformed ones
  std::string edge_text = fdp.ConsumeRandomLengthString(100);
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(3)) && fdp.remaining_bytes() > 0; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Test various rewrite strings
    std::vector<std::string> edge_rewrites = {
      "",                    // Empty
      "\\",                 // Just backslash
      "\\999",              // Invalid group reference
      "\\1\\2\\3\\4\\5\\6\\7\\8\\9\\10", // Many group references
      "test\\g",            // Invalid escape
      "\\0\\0\\0",          // Multiple whole match references
    };
    
    for (const auto& rewrite : edge_rewrites) {
      std::string result;
      RE2::Extract(edge_text, re2, rewrite, &result);
      
      std::string temp = edge_text;
      RE2::Replace(&temp, re2, rewrite);
      
      temp = edge_text;
      RE2::GlobalReplace(&temp, re2, rewrite);
    }
  }
  // Step 7: Test helper APIs: RE2::Match, RE2::FullMatch, RE2::PartialMatch
  
  // Test RE2::Match (low-level matching interface)
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(3)) && fdp.remaining_bytes() > 0; i++) {
    const auto& re2 = *re2_objects[i];
    
    std::string match_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 200));
    
    // Prepare submatch array
    int nsubmatch = std::min(10, 1 + re2.NumberOfCapturingGroups());
    std::vector<absl::string_view> submatches(nsubmatch);
    
    // Test with different anchor types
    RE2::Anchor anchors[] = {RE2::UNANCHORED, RE2::ANCHOR_START, RE2::ANCHOR_BOTH};
    
    for (auto anchor : anchors) {
      re2.Match(match_text, 0, match_text.size(), anchor, submatches.data(), nsubmatch);
    }
  }
  
  // Test RE2::FullMatch and RE2::PartialMatch with various argument types
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(2)) && fdp.remaining_bytes() > 0; i++) {
    const auto& re2 = *re2_objects[i];
    
    std::string match_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 200));
    
    // Test FullMatch with different argument types
    bool full_match_result;
    std::string str_result;
    int int_result;
    absl::string_view sv_result;
    
    // Test with no capture arguments
    full_match_result = RE2::FullMatch(match_text, re2);
    
    // Test with single capture argument (if pattern has capture groups)
    if (re2.NumberOfCapturingGroups() > 0) {
      RE2::FullMatch(match_text, re2, &str_result);
      RE2::PartialMatch(match_text, re2, &int_result);
      RE2::FullMatch(match_text, re2, &sv_result);
      
      // Test with multiple arguments
      if (re2.NumberOfCapturingGroups() > 1) {
        std::string str1, str2;
        RE2::FullMatch(match_text, re2, &str1, &str2);
        
        std::string str3;
        int int1;
        RE2::PartialMatch(match_text, re2, &str3, &int1);
      }
    }
  }
  
  return 0;
}

} // namespace harness_012
