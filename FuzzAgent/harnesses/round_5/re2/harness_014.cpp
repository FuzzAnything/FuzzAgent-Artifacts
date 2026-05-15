#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "re2/re2.h"

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 64) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Create RE2 objects with complex regex patterns targeting undiscovered branches
  
  // We'll create patterns that specifically exercise complex regex features
  // to hit the undiscovered branches in Extract, Replace, and GlobalReplace
  
  std::vector<std::string> patterns;
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 8);
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 20; i++) {
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 12;
    std::string pattern;
    
    switch (pattern_type) {
      case 0:
        // Complex pattern with nested capture groups - targets group reference logic
        pattern = "((" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  ")*(" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  ")+){" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 3)) + "}";
        break;
      case 1:
        // Pattern with lookahead/lookbehind simulation (not actual but complex)
        pattern = "(?=" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  ").*?(?<=" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + ")";
        break;
      case 2:
        // Pattern with complex character classes and ranges
        pattern = "[a-z" + fdp.ConsumeRandomLengthString(5) + 
                  "A-Z" + fdp.ConsumeRandomLengthString(5) + 
                  "0-9]{" + std::to_string(fdp.ConsumeIntegralInRange<int>(2, 10)) + "}";
        break;
      case 3:
        // Pattern with alternation and grouping - targets branch logic
        pattern = "(" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8)) + 
                  "|" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8)) + 
                  "|" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8)) + ")";
        break;
      case 4:
        // Pattern with backreferences (if supported) or repeated groups
        pattern = "(" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5)) + 
                  ")\\1?";
        break;
      case 5:
        // Pattern with word boundaries and anchors - targets anchor logic
        pattern = "\\b" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 15)) + 
                  "\\B|^" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  "$";
        break;
      case 6:
        // Pattern with optional groups and greedy/reluctant quantifiers
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10)) + 
                  "(?:+" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5)) + 
                  ")?.*?" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5)) + 
                  "+";
        break;
      case 7:
        // Pattern with Unicode character classes (if UTF-8)
        pattern = "\\p{" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5)) + 
                  "}";
        break;
      case 8:
        // Pattern with repetition ranges
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 8)) + 
                  "{" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 5)) + "," + 
                  std::to_string(fdp.ConsumeIntegralInRange<int>(6, 20)) + "}";
        break;
      case 9:
        // Pattern with escaped characters and special sequences
        pattern = "\\" + fdp.ConsumeRandomLengthString(1) + 
                  "\\d\\s\\w\\D\\S\\W";
        break;
      case 10:
        // Empty pattern (edge case)
        pattern = "";
        break;
      case 11:
        // Very simple pattern (counterbalance to complex ones)
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 20));
        break;
    }
    
    patterns.push_back(pattern);
  }
  
  // Create RE2 objects with various options to target different code paths
  std::vector<std::unique_ptr<RE2>> re2_objects;
  
  for (const auto& pattern : patterns) {
    if (fdp.remaining_bytes() < 10) break;
    
    RE2::Options options;
    
    // Configure options to explore different code paths in Extract/Replace/GlobalReplace
    uint8_t option_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Set encoding - affects how patterns are processed
    if (option_flags & 0x01) {
      options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 : RE2::Options::EncodingUTF8);
    }
    
    // Set matching options that affect Extract/Replace behavior
    if (option_flags & 0x02) options.set_longest_match(fdp.ConsumeBool());
    if (option_flags & 0x04) options.set_case_sensitive(!fdp.ConsumeBool()); // Test case-insensitive
    
    // Options that affect pattern compilation and matching
    if (option_flags & 0x08) options.set_literal(fdp.ConsumeBool());
    if (option_flags & 0x10) options.set_never_nl(fdp.ConsumeBool());
    if (option_flags & 0x20) options.set_dot_nl(fdp.ConsumeBool());
    if (option_flags & 0x40) options.set_never_capture(fdp.ConsumeBool());
    
    // Always disable error logging during fuzzing
    options.set_log_errors(false);
    
    // Set memory limit - affects internal compilation
    if (fdp.remaining_bytes() >= sizeof(int)) {
      int max_mem = fdp.ConsumeIntegralInRange<int>(1024, 8 * 1024 * 1024); // 1KB to 8MB
      options.set_max_mem(max_mem);
    }
    
    // Create RE2 object
    auto re2_obj = std::make_unique<RE2>(pattern, options);
    if (re2_obj->ok() || pattern.empty()) { // Include empty patterns for edge case testing
      re2_objects.push_back(std::move(re2_obj));
    }
  }
  
  if (re2_objects.empty()) return 0;
  
  // Step 2: Test RE2::Extract with complex rewrite strings targeting backreference logic
  
  // Generate input text with various structures to match against patterns
  std::string extract_text;
  uint8_t text_type = fdp.ConsumeIntegral<uint8_t>() % 6;
  
  switch (text_type) {
    case 0:
      // Text with repeated patterns (good for testing backreferences)
      extract_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(20, 200));
      // Duplicate parts to create repetition
      if (extract_text.length() > 10) {
        extract_text = extract_text.substr(0, 10) + extract_text.substr(0, 10) + extract_text;
      }
      break;
    case 1:
      // Text with numeric sequences (good for digit patterns)
      extract_text = "12345" + fdp.ConsumeRandomLengthString(50) + "67890" + fdp.ConsumeRandomLengthString(50);
      break;
    case 2:
      // Text with mixed case (good for case-sensitive/insensitive testing)
      extract_text = fdp.ConsumeRandomLengthString(100);
      std::transform(extract_text.begin(), extract_text.end(), extract_text.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      extract_text[0] = std::toupper(extract_text[0]); // Ensure some uppercase
      break;
    case 3:
      // Text with whitespace patterns
      extract_text = "   " + fdp.ConsumeRandomLengthString(80) + "\t\t" + 
                     fdp.ConsumeRandomLengthString(80) + "\n\n";
      break;
    case 4:
      // Text with special characters
      extract_text = "!@#$%" + fdp.ConsumeRandomLengthString(90) + "^&*()" + 
                     fdp.ConsumeRandomLengthString(90);
      break;
    case 5:
      // Empty or very short text
      if (fdp.ConsumeBool()) {
        extract_text = "";
      } else {
        extract_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5));
      }
      break;
  }
  
  // Test Extract with each RE2 object
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(5)) && fdp.remaining_bytes() > 10; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Create rewrite strings that exercise different Extract logic
    uint8_t rewrite_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    std::string rewrite;
    
    switch (rewrite_type) {
      case 0:
        // Rewrite with multiple backreferences
        rewrite = "\\0 - whole match";
        break;
      case 1:
        // Rewrite with specific capture group references
        if (re2.NumberOfCapturingGroups() > 0) {
          rewrite = "Group1: \\1";
          if (re2.NumberOfCapturingGroups() > 1) {
            rewrite += ", Group2: \\2";
          }
          if (re2.NumberOfCapturingGroups() > 2) {
            rewrite += ", Group3: \\3";
          }
        } else {
          rewrite = fdp.ConsumeRandomLengthString(30);
        }
        break;
      case 2:
        // Rewrite with escaped characters
        rewrite = "Result: \\\\ \\t \\n \\\" \\'";
        break;
      case 3:
        // Rewrite with mixed content
        rewrite = fdp.ConsumeRandomLengthString(20) + " \\0 " + fdp.ConsumeRandomLengthString(20);
        break;
      case 4:
        // Empty rewrite
        rewrite = "";
        break;
      case 5:
        // Rewrite with only backreferences
        rewrite = "\\1\\2\\3\\4\\5";
        break;
      case 6:
        // Rewrite with invalid backreferences (should be handled gracefully)
        rewrite = "\\999\\1000\\invalid";
        break;
      case 7:
        // Rewrite with complex pattern
        rewrite = "${" + fdp.ConsumeRandomLengthString(10) + "}";
        break;
    }
    
    std::string extract_result;
    RE2::Extract(extract_text, re2, rewrite, &extract_result);
    
    // Also test CheckRewriteString to cover its logic
    std::string rewrite_error;
    re2.CheckRewriteString(rewrite, &rewrite_error);
  }
  
  // Step 3: Test RE2::Replace with overlapping and nested patterns
  
  std::string replace_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(50, 300));
  std::string original_replace_text = replace_text;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(4)) && fdp.remaining_bytes() > 10; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Create rewrite strings for Replace
    std::string rewrite;
    uint8_t replace_rewrite_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (replace_rewrite_type) {
      case 0:
        rewrite = "REPLACED";
        break;
      case 1:
        rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 50));
        break;
      case 2:
        rewrite = ""; // Empty replacement
        break;
      case 3:
        rewrite = "\\0"; // Replace with whole match
        break;
      case 4:
        if (re2.NumberOfCapturingGroups() > 0) {
          rewrite = "\\1"; // Replace with first capture group
        } else {
          rewrite = "X";
        }
        break;
    }
    
    // Test Replace
    bool replace_success = RE2::Replace(&replace_text, re2, rewrite);
    
    // Reset for next iteration
    replace_text = original_replace_text;
  }
  
  // Step 4: Test RE2::GlobalReplace with text designed for multiple matches
  
  // Create text likely to have multiple matches
  std::string global_text;
  uint8_t global_text_type = fdp.ConsumeIntegral<uint8_t>() % 4;
  
  switch (global_text_type) {
    case 0:
      // Text with repeated words
      global_text = "test test test " + fdp.ConsumeRandomLengthString(100) + " test test";
      break;
    case 1:
      // Text with numeric sequences
      global_text = "123 456 789 " + fdp.ConsumeRandomLengthString(120) + " 111 222 333";
      break;
    case 2:
      // Text with pattern repetitions
      {
        std::string base = fdp.ConsumeRandomLengthString(10);
        global_text = base + " " + base + " " + base + " " + fdp.ConsumeRandomLengthString(100);
      }
      break;
    case 3:
      // Random text
      global_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(100, 400));
      break;
  }
  
  std::string original_global_text = global_text;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(4)) && fdp.remaining_bytes() > 10; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Create rewrite for GlobalReplace
    std::string rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 50));
    
    // Test GlobalReplace
    int num_replacements = RE2::GlobalReplace(&global_text, re2, rewrite);
    
    // Reset for next iteration
    global_text = original_global_text;
  }
  
  // Step 5: Test combined operations and edge cases
  
  if (fdp.remaining_bytes() > 50) {
    // Test sequence: Extract -> Replace -> GlobalReplace on same data
    std::string combined_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(80, 200));
    std::string original_combined = combined_text;
    
    // Use first valid RE2 object
    const auto& re2 = *re2_objects[0];
    
    // First Extract
    std::string extract_rewrite = "Extracted: \\0";
    std::string extracted;
    RE2::Extract(combined_text, re2, extract_rewrite, &extracted);
    
    // Then Replace
    std::string replace_rewrite = fdp.ConsumeRandomLengthString(20);
    RE2::Replace(&combined_text, re2, replace_rewrite);
    
    // Finally GlobalReplace on modified text
    std::string global_rewrite = fdp.ConsumeRandomLengthString(20);
    RE2::GlobalReplace(&combined_text, re2, global_rewrite);
    
    // Test with empty strings
    std::string empty_text = "";
    std::string empty_result;
    std::string empty_rewrite = fdp.ConsumeRandomLengthString(10);
    
    RE2::Extract(empty_text, re2, empty_rewrite, &empty_result);
    
    std::string temp_empty = "";
    RE2::Replace(&temp_empty, re2, empty_rewrite);
    RE2::GlobalReplace(&temp_empty, re2, empty_rewrite);
  }
  
  // Step 6: Test with rewrite strings that exercise MaxSubmatch logic
  
  if (fdp.remaining_bytes() > 20) {
    // Test various rewrite strings to cover MaxSubmatch branches
    std::vector<std::string> test_rewrites = {
      "",
      "\\0",
      "\\1",
      "\\1\\2\\3\\4\\5\\6\\7\\8\\9\\10",
      "test\\g",
      "\\",
      "\\\\",
      "test\\1test\\2test",
      "\\999",  // Invalid group reference
    };
    
    std::string test_text = fdp.ConsumeRandomLengthString(50);
    
    for (const auto& rewrite : test_rewrites) {
      for (size_t i = 0; i < std::min(re2_objects.size(), size_t(2)); i++) {
        const auto& re2 = *re2_objects[i];
        
        std::string result;
        RE2::Extract(test_text, re2, rewrite, &result);
        
        // Test MaxSubmatch
        int max_submatch = RE2::MaxSubmatch(rewrite);
        
        std::string temp = test_text;
        RE2::Replace(&temp, re2, rewrite);
        
        temp = test_text;
        RE2::GlobalReplace(&temp, re2, rewrite);
      }
    }
  }
  
  return 0;
}

} // namespace harness_014
