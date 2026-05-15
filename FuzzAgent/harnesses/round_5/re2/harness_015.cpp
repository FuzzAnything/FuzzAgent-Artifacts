#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "re2/re2.h"

namespace harness_015 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 128) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Strategy: Target RE2 string replacement operations with semantic differentiation
  // from existing harnesses by focusing on the interaction between Extract, Replace,
  // GlobalReplace, and Rewrite operations, and testing various rewrite templates
  // including backreference patterns.
  
  // Step 1: Generate multiple regex patterns targeting different regex features
  std::vector<std::string> patterns;
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 6);
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 50; i++) {
    uint8_t pattern_category = fdp.ConsumeIntegral<uint8_t>() % 8;
    std::string pattern;
    
    switch (pattern_category) {
      case 0:
        // Capture group patterns for testing backreferences in rewrite operations
        {
          std::string word_part = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 8));
          std::string num_part = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 5));
          std::string special_part = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          pattern = "(\\w+)\\s+(\\d+)\\s+(\\S+)";
          // Use fuzzed data but keep the capture group structure
          pattern = "(" + word_part + ")\\s+(\\d+)\\s+(" + special_part + ")";
        }
        break;
      case 1:
        // Alternation pattern for testing multiple match possibilities
        {
          std::string alt1 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          std::string alt2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          std::string alt3 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          std::string alt4 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          pattern = alt1 + "|" + alt2 + "|" + alt3 + "|" + alt4;
        }
        break;
      case 2:
        // Repetition patterns for testing GlobalReplace
        {
          std::string char1 = fdp.ConsumeRandomLengthString(1);
          std::string char2 = fdp.ConsumeRandomLengthString(1);
          std::string char3 = fdp.ConsumeRandomLengthString(1);
          pattern = char1 + "+\\s+" + char2 + "{2,4}\\s+" + char3 + "*";
        }
        break;
      case 3:
        // Character class patterns
        {
          int min_digits = fdp.ConsumeIntegralInRange<int>(1, 5);
          int max_digits = fdp.ConsumeIntegralInRange<int>(min_digits + 1, 8);
          int min_letters = fdp.ConsumeIntegralInRange<int>(1, 6);
          int max_letters = fdp.ConsumeIntegralInRange<int>(min_letters + 1, 10);
          int min_special = fdp.ConsumeIntegralInRange<int>(1, 4);
          pattern = "[0-9]{" + std::to_string(min_digits) + "," + std::to_string(max_digits) + "}\\s+[A-Za-z]{" + 
                   std::to_string(min_letters) + "," + std::to_string(max_letters) + "}\\s+[!@#$%^&*()]{" + 
                   std::to_string(min_special) + "}";
        }
        break;
      case 4:
        // Word boundary patterns
        {
          std::string word1 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
          std::string word2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
          std::string word3 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
          pattern = "\\b" + word1 + "\\b|\\b" + word2 + "\\b|\\b" + word3 + "\\b";
        }
        break;
      case 5:
        // Simple literal patterns
        pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 15));
        break;
      case 6:
        // Empty pattern (edge case)
        pattern = "";
        break;
      case 7:
        // Complex nested groups for advanced backreference testing
        {
          std::string base_word = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          int repeat_min = fdp.ConsumeIntegralInRange<int>(1, 3);
          int repeat_max = fdp.ConsumeIntegralInRange<int>(repeat_min, 5);
          pattern = "((" + base_word + ")-?(\\d+)){" + std::to_string(repeat_min) + "," + std::to_string(repeat_max) + "}";
        }
        break;
    }
    
    patterns.push_back(pattern);
  }
  
  if (patterns.empty()) return 0;
  
  // Step 2: Create RE2 objects with various RE2::Options configurations
  std::vector<std::unique_ptr<RE2>> re2_objects;
  
  for (const auto& pattern : patterns) {
    if (fdp.remaining_bytes() < 20) break;
    
    RE2::Options options;
    
    // Configure options to explore different code paths in replacement operations
    uint8_t option_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // Encoding affects pattern interpretation
    if (option_mask & 0x01) {
      options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 : RE2::Options::EncodingUTF8);
    }
    
    // Matching options that affect replacement behavior
    if (option_mask & 0x02) options.set_longest_match(fdp.ConsumeBool());
    if (option_mask & 0x04) options.set_case_sensitive(!fdp.ConsumeBool());
    if (option_mask & 0x08) options.set_literal(fdp.ConsumeBool());
    if (option_mask & 0x10) options.set_never_nl(fdp.ConsumeBool());
    if (option_mask & 0x20) options.set_dot_nl(fdp.ConsumeBool());
    if (option_mask & 0x40) options.set_never_capture(fdp.ConsumeBool());
    
    // Disable error logging during fuzzing
    options.set_log_errors(false);
    
    // Set memory limit
    if (fdp.remaining_bytes() >= sizeof(int)) {
      int max_mem = fdp.ConsumeIntegralInRange<int>(4096, 4 * 1024 * 1024);
      options.set_max_mem(max_mem);
    }
    
    // Create RE2 object
    auto re2_obj = std::make_unique<RE2>(pattern, options);
    if (re2_obj->ok() || pattern.empty()) {
      re2_objects.push_back(std::move(re2_obj));
    }
  }
  
  if (re2_objects.empty()) return 0;
  
  // Step 3: Test RE2::Extract with various rewrite templates
  // Generate text for extraction
  std::string extract_text;
  uint8_t extract_text_type = fdp.ConsumeIntegral<uint8_t>() % 5;
  
  switch (extract_text_type) {
    case 0:
      // Text with capture group matches
      {
        std::string word1 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
        std::string num1 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string word2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(4, 10));
        std::string num2 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string word3 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 12));
        std::string num3 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        extract_text = word1 + " " + num1 + " " + word2 + " " + num2 + " " + word3 + " " + num3;
      }
      break;
    case 1:
      // Text with repetition
      {
        std::string repeated_char = fdp.ConsumeRandomLengthString(1);
        std::string repeated_text = repeated_char + repeated_char + repeated_char + " ";
        extract_text = repeated_text + fdp.ConsumeRandomLengthString(100);
      }
      break;
    case 2:
      // Mixed case text for case-sensitive testing
      {
        std::string part1 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 15));
        std::string part2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 15));
        std::string part3 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 15));
        // Convert to mixed case
        std::transform(part1.begin(), part1.end(), part1.begin(), ::tolower);
        if (!part1.empty()) part1[0] = ::toupper(part1[0]);
        std::transform(part2.begin(), part2.end(), part2.begin(), ::toupper);
        std::transform(part3.begin(), part3.end(), part3.begin(), ::tolower);
        if (!part3.empty()) part3[0] = ::toupper(part3[0]);
        extract_text = part1 + " " + part2 + " " + part3;
      }
      break;
    case 3:
      // Text with special characters
      {
        std::string email_local = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 10));
        std::string email_domain = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 12));
        std::string phone_part1 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string phone_part2 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string phone_part3 = std::to_string(fdp.ConsumeIntegralInRange<int>(1000, 9999));
        std::string currency = "$" + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1000)) + "." + 
                               std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99));
        std::string tag = "#" + fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
        extract_text = email_local + "@" + email_domain + ".com " + phone_part1 + "-" + phone_part2 + "-" + 
                      phone_part3 + " " + currency + " " + tag;
      }
      break;
    case 4:
      // Random text
      extract_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(50, 300));
      break;
  }
  
  // Test Extract with each RE2 object using various rewrite templates
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(4)) && fdp.remaining_bytes() > 20; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Generate rewrite string for Extract
    uint8_t extract_rewrite_type = fdp.ConsumeIntegral<uint8_t>() % 7;
    std::string extract_rewrite;
    
    switch (extract_rewrite_type) {
      case 0:
        // Simple rewrite
        extract_rewrite = fdp.ConsumeRandomLengthString(20);
        break;
      case 1:
        // Rewrite with backreferences
        if (re2.NumberOfCapturingGroups() > 0) {
          extract_rewrite = "Matched: \\0, Group1: \\1";
          if (re2.NumberOfCapturingGroups() > 1) {
            extract_rewrite += ", Group2: \\2";
          }
        } else {
          extract_rewrite = fdp.ConsumeRandomLengthString(20);
        }
        break;
      case 2:
        // Empty rewrite
        extract_rewrite = "";
        break;
      case 3:
        // Rewrite with multiple backreferences
        extract_rewrite = "\\0-\\1-\\2-\\3";
        break;
      case 4:
        // Rewrite with escaped characters
        extract_rewrite = "Result: \\\\ \\t \\n";
        break;
      case 5:
        // Rewrite with only backreferences
        extract_rewrite = "\\1\\2\\3";
        break;
      case 6:
        // Complex rewrite pattern
        extract_rewrite = "${1}-${2} [\\0]";
        break;
    }
    
    std::string extract_result;
    RE2::Extract(extract_text, re2, extract_rewrite, &extract_result);
    
    // Also test CheckRewriteString
    std::string rewrite_error;
    re2.CheckRewriteString(extract_rewrite, &rewrite_error);
  }
  
  // Step 4: Test RE2::Replace with single replacement operations
  std::string replace_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(100, 400));
  std::string original_replace_text = replace_text;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(3)) && fdp.remaining_bytes() > 20; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Generate rewrite for Replace
    std::string replace_rewrite;
    uint8_t replace_rewrite_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (replace_rewrite_type) {
      case 0:
        replace_rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 15));
        break;
      case 1:
        replace_rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(5, 30));
        break;
      case 2:
        replace_rewrite = ""; // Empty replacement (deletion)
        break;
      case 3:
        replace_rewrite = "\\0"; // Replace with whole match (backreference is okay)
        break;
      case 4:
        if (re2.NumberOfCapturingGroups() > 0) {
          replace_rewrite = "\\1"; // Replace with first capture group (backreference is okay)
        } else {
          replace_rewrite = fdp.ConsumeRandomLengthString(1); // Single character replacement
        }
        break;
      case 5:
        replace_rewrite = "\\1-\\2"; // Replace with concatenated groups (backreference is okay)
        break;
    }
    
    // Perform Replace operation
    bool replace_success = RE2::Replace(&replace_text, re2, replace_rewrite);
    
    // Reset text for next iteration
    replace_text = original_replace_text;
  }
  
  // Step 5: Test RE2::GlobalReplace with multiple replacements
  // Create text likely to have multiple matches
  std::string global_text;
  uint8_t global_text_type = fdp.ConsumeIntegral<uint8_t>() % 4;
  
  switch (global_text_type) {
    case 0:
      // Text with repeated patterns
      {
        std::string repeated_word = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
        std::string filler_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(20, 100));
        global_text = repeated_word + " " + repeated_word + " " + repeated_word + " " + 
                     filler_text + " " + repeated_word + " " + repeated_word;
      }
      break;
    case 1:
      // Text with numeric sequences
      {
        std::string num1 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num2 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num3 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num4 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num5 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num6 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num7 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        std::string num8 = std::to_string(fdp.ConsumeIntegralInRange<int>(100, 999));
        global_text = num1 + " " + num2 + " " + num3 + " " + num1 + " " + num2 + " " + num3 + " " + num7 + " " + num8;
      }
      break;
    case 2:
      // Text with mixed patterns
      {
        std::string base = fdp.ConsumeRandomLengthString(8);
        global_text = base + " " + base + " " + base + " " + fdp.ConsumeRandomLengthString(100);
      }
      break;
    case 3:
      // Random text
      global_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(150, 500));
      break;
  }
  
  std::string original_global_text = global_text;
  
  for (size_t i = 0; i < std::min(re2_objects.size(), size_t(3)) && fdp.remaining_bytes() > 20; i++) {
    const auto& re2 = *re2_objects[i];
    
    // Generate rewrite for GlobalReplace
    std::string global_rewrite = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(0, 40));
    
    // Perform GlobalReplace operation and get replacement count
    int num_replacements = RE2::GlobalReplace(&global_text, re2, global_rewrite);
    
    // Reset text for next iteration
    global_text = original_global_text;
  }
  
  // Step 6: Test RE2::Rewrite operation with captured submatches
  // This requires actual matches to have submatches for rewriting
  if (fdp.remaining_bytes() > 100) {
    // Use the first RE2 object
    const auto& re2 = *re2_objects[0];
    
    // Generate text to match against
    std::string rewrite_match_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(80, 200));
    
    // Try to get submatches
    int num_captures = re2.NumberOfCapturingGroups();
    if (num_captures > 0) {
      std::vector<absl::string_view> submatches(num_captures + 1); // +1 for full match
      
      // Try matching with different anchors
      RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
      });
      
      bool match_result = re2.Match(rewrite_match_text, 0, rewrite_match_text.length(),
                                   anchor, submatches.data(), submatches.size());
      
      if (match_result) {
        // Now test Rewrite with various rewrite strings
        std::vector<std::string> rewrite_strings;
        rewrite_strings.push_back("Rewritten: \\0");
        rewrite_strings.push_back("\\1");
        rewrite_strings.push_back("\\1-\\2");
        rewrite_strings.push_back("Prefix-\\0-Suffix");
        rewrite_strings.push_back(fdp.ConsumeRandomLengthString(30));
        
        for (const auto& rewrite_str : rewrite_strings) {
          std::string rewrite_output;
          bool rewrite_success = re2.Rewrite(&rewrite_output, rewrite_str,
                                            submatches.data(), submatches.size());
        }
      }
    }
  }
  
  // Step 7: Test MaxSubmatch static function
  if (fdp.remaining_bytes() > 30) {
    std::vector<std::string> test_rewrites = {
      "",
      "\\0",
      "\\1",
      "\\1\\2\\3\\4\\5",
      "test\\1test\\2test",
      "\\999",  // Invalid group reference
      fdp.ConsumeRandomLengthString(50)
    };
    
    for (const auto& rewrite : test_rewrites) {
      int max_submatch = RE2::MaxSubmatch(rewrite);
    }
  }
  
  // Step 8: Test combined operations sequence
  if (fdp.remaining_bytes() > 150) {
    const auto& re2 = *re2_objects[0];
    
    // Create a text and apply all operations in sequence
    std::string combined_text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(100, 300));
    std::string working_text = combined_text;
    
    // Sequence 1: Extract -> Replace -> GlobalReplace
    std::string extract_rewrite = "Extracted: \\0";
    std::string extracted;
    RE2::Extract(working_text, re2, extract_rewrite, &extracted);
    
    std::string replace_rewrite = "R";
    RE2::Replace(&working_text, re2, replace_rewrite);
    
    std::string global_rewrite = "G";
    RE2::GlobalReplace(&working_text, re2, global_rewrite);
    
    // Sequence 2: GlobalReplace -> Extract on modified text
    std::string global_text2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(100, 250));
    RE2::GlobalReplace(&global_text2, re2, "MODIFIED");
    
    std::string extract_result2;
    RE2::Extract(global_text2, re2, "\\0", &extract_result2);
  }
  
  // Step 9: Test edge cases
  // Test with empty strings
  std::string empty_text = "";
  std::string empty_result;
  std::string empty_rewrite = fdp.ConsumeRandomLengthString(10);
  
  const auto& re2_edge = *re2_objects[0];
  RE2::Extract(empty_text, re2_edge, empty_rewrite, &empty_result);
  
  std::string temp_empty = "";
  RE2::Replace(&temp_empty, re2_edge, empty_rewrite);
  RE2::GlobalReplace(&temp_empty, re2_edge, empty_rewrite);
  
  return 0;
}

} // namespace harness_015
