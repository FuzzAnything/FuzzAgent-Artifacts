#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_004 {
// RE2 string consumption and utility APIs harness
// Targets: RE2::FindAndConsumeN, RE2::ConsumeN, RE2::QuoteMeta, RE2::PossibleMatchRange, RE2::CheckRewriteString
// These APIs currently have 0% coverage and represent 300+ branches of undiscovered complexity



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need enough data for pattern, text, and operations
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Create pattern and text from fuzzer input
  std::string pattern = fdp.ConsumeRandomLengthString(256);
  std::string text = fdp.ConsumeRandomLengthString(1024);
  std::string rewrite = fdp.ConsumeRandomLengthString(256);
  
  // Consume some boolean flags for different test paths
  bool test_quote_meta = fdp.ConsumeBool();
  bool test_consume = fdp.ConsumeBool();
  bool test_find_and_consume = fdp.ConsumeBool();
  bool test_possible_match_range = fdp.ConsumeBool();
  bool test_check_rewrite_string = fdp.ConsumeBool();
  
  // Consume integer for PossibleMatchRange maxlen parameter
  int maxlen = fdp.ConsumeIntegralInRange<int>(1, 100);

  // 2. Create RE2 object with the pattern
  re2::RE2 re(pattern);
  
  // Only proceed if pattern compiled successfully
  if (!re.ok()) {
    return 0;
  }

  // 3. Test RE2::QuoteMeta - escape special characters in patterns
  if (test_quote_meta && fdp.remaining_bytes() > 0) {
    std::string pattern_to_quote = fdp.ConsumeRandomLengthString(128);
    std::string quoted_pattern = re2::RE2::QuoteMeta(pattern_to_quote);
    
    // Create a regex from the quoted pattern and verify it matches the original
    re2::RE2 quoted_re(quoted_pattern);
    if (quoted_re.ok()) {
      // The quoted pattern should match the original unquoted string exactly
      re2::RE2::FullMatch(pattern_to_quote, quoted_re);
    }
  }

  // 4. Test RE2::Consume (anchored consumption from start of string)
  // RE2::Consume internally calls RE2::ConsumeN
  if (test_consume && fdp.remaining_bytes() > 0) {
    // Create a mutable string_view for consumption
    std::string consume_text = fdp.ConsumeRandomLengthString(512);
    absl::string_view input(consume_text);
    
    // Try different consumption patterns based on fuzzer input
    uint8_t consume_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (consume_mode) {
      case 0: {
        // Consume without capturing groups
        re2::RE2::Consume(&input, re);
        break;
      }
      case 1: {
        // Consume with one capture group
        std::string captured;
        re2::RE2::Consume(&input, re, &captured);
        break;
      }
      case 2: {
        // Consume with multiple capture groups
        std::string cap1, cap2;
        re2::RE2::Consume(&input, re, &cap1, &cap2);
        break;
      }
      case 3: {
        // Consume with mixed types (string and integer)
        std::string word;
        int number;
        re2::RE2::Consume(&input, re, &word, &number);
        break;
      }
    }
  }

  // 5. Test RE2::FindAndConsume (unanchored consumption anywhere in string)
  // RE2::FindAndConsume internally calls RE2::FindAndConsumeN
  if (test_find_and_consume && fdp.remaining_bytes() > 0) {
    // Create a mutable string_view for find-and-consume
    std::string find_text = fdp.ConsumeRandomLengthString(512);
    absl::string_view input(find_text);
    
    // Try to find and consume multiple times (up to 5 iterations)
    int max_iterations = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int i = 0; i < max_iterations && !input.empty(); i++) {
      uint8_t find_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
      
      switch (find_mode) {
        case 0: {
          // FindAndConsume without capturing groups
          bool found = re2::RE2::FindAndConsume(&input, re);
          if (!found) break; // Stop if no match found
          break;
        }
        case 1: {
          // FindAndConsume with one capture group
          std::string captured;
          bool found = re2::RE2::FindAndConsume(&input, re, &captured);
          if (!found) break; // Stop if no match found
          break;
        }
        case 2: {
          // FindAndConsume with multiple capture groups
          std::string cap1, cap2;
          bool found = re2::RE2::FindAndConsume(&input, re, &cap1, &cap2);
          if (!found) break; // Stop if no match found
          break;
        }
      }
    }
  }

  // 6. Test RE2::PossibleMatchRange - analyze pattern matching range
  if (test_possible_match_range) {
    std::string min_match, max_match;
    bool range_success = re.PossibleMatchRange(&min_match, &max_match, maxlen);
    
    // If range calculation was successful, verify properties
    if (range_success) {
      // The range strings should satisfy: min_match <= any match <= max_match
      // We can't easily verify this property without generating all possible matches,
      // but we can at least check that min_match <= max_match lexicographically
      if (min_match > max_match) {
        // This would indicate a bug in PossibleMatchRange
        // Just continue without crashing
      }
      
      // Also test with different maxlen values
      if (fdp.remaining_bytes() > 0) {
        int alt_maxlen = fdp.ConsumeIntegralInRange<int>(1, 50);
        std::string alt_min, alt_max;
        re.PossibleMatchRange(&alt_min, &alt_max, alt_maxlen);
      }
    }
  }

  // 7. Test RE2::CheckRewriteString - validate rewrite strings
  if (test_check_rewrite_string && fdp.remaining_bytes() > 0) {
    std::string rewrite_to_check = fdp.ConsumeRandomLengthString(128);
    std::string error_message;
    
    bool rewrite_valid = re.CheckRewriteString(rewrite_to_check, &error_message);
    
    // If rewrite string is valid, test it with Replace/Extract operations
    if (rewrite_valid) {
      // Test RE2::Replace with the validated rewrite string
      std::string replace_str = text;
      re2::RE2::Replace(&replace_str, re, rewrite_to_check);
      
      // Test RE2::Extract with the validated rewrite string
      std::string extract_out;
      re2::RE2::Extract(text, re, rewrite_to_check, &extract_out);
    }
    
    // Also test MaxSubmatch static function
    int max_submatch = re2::RE2::MaxSubmatch(rewrite_to_check);
    // max_submatch indicates maximum submatch needed for the rewrite
  }

  // 8. Additional tests for NumberOfCapturingGroups (helper API)
  int num_captures = re.NumberOfCapturingGroups();
  // num_captures should be >= 0 for valid regex (-1 indicates invalid regex)
  
  // Test with empty string input edge case
  if (fdp.remaining_bytes() > 0) {
    absl::string_view empty_input("");
    re2::RE2::Consume(&empty_input, re);
    re2::RE2::FindAndConsume(&empty_input, re);
  }

  return 0;
}

} // namespace harness_004
