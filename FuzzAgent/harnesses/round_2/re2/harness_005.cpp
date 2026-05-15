#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_005 {
// RE2 pattern substitution operations fuzzing harness
// Targets RE2::CheckRewriteString (14 undiscovered branches, 0% coverage), 
// GlobalReplace, Replace, and Rewrite functions with 350+ undiscovered complexity points
// Focuses specifically on string replacement and rewriting operations



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Limit input size for efficiency
  if (size == 0 || size > 4096)
    return 0;

  FuzzedDataProvider fdp(data, size);

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

  // Consume pattern from fuzzer input
  std::string pattern = fdp.ConsumeRandomLengthString(200);
  
  // Create RE2 object with the pattern and options
  RE2 re(pattern, options);
  
  // If RE2 construction failed, return early
  if (!re.ok()) {
    return 0;
  }

  // Consume text to operate on
  std::string text = fdp.ConsumeRandomLengthString(200);
  
  // Test 1: CheckRewriteString with various rewrite strings
  // Generate different types of rewrite strings to test CheckRewriteString
  
  // Simple rewrite string
  std::string simple_rewrite = fdp.ConsumeRandomLengthString(50);
  std::string error1;
  bool check1 = re.CheckRewriteString(simple_rewrite, &error1);
  
  // Rewrite string with backreferences
  std::string with_backrefs;
  size_t backref_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
  for (size_t i = 0; i < backref_count; i++) {
    if (fdp.ConsumeBool()) {
      with_backrefs += "\\";
      with_backrefs += std::to_string(fdp.ConsumeIntegralInRange<int>(0, 9));
    } else {
      with_backrefs += fdp.ConsumeRandomLengthString(5);
    }
  }
  std::string error2;
  bool check2 = re.CheckRewriteString(with_backrefs, &error2);
  
  // Rewrite string with escaped backslashes
  std::string escaped;
  size_t escape_len = fdp.ConsumeIntegralInRange<size_t>(0, 20);
  for (size_t i = 0; i < escape_len; i++) {
    if (fdp.ConsumeBool()) {
      escaped += "\\\\";
    } else {
      escaped += fdp.ConsumeRandomLengthString(3);
    }
  }
  std::string error3;
  bool check3 = re.CheckRewriteString(escaped, &error3);
  
  // Test 2: Replace operation with validated rewrite strings
  if (check1) {
    std::string replace_text = text;
    bool replace_success = RE2::Replace(&replace_text, re, simple_rewrite);
    // Replace modifies in-place, success indicates at least one replacement occurred
  }
  
  // Test 3: GlobalReplace operation with validated rewrite strings
  if (check2) {
    std::string global_text = text;
    int replace_count = RE2::GlobalReplace(&global_text, re, with_backrefs);
    // replace_count indicates number of replacements made
  }
  
  // Test 4: Extract operation (uses rewrite internally)
  if (check3) {
    std::string extract_output;
    bool extract_success = RE2::Extract(text, re, escaped, &extract_output);
  }
  
  // Test 5: Direct Rewrite function usage
  // This requires constructing a match vector first
  if (re.NumberOfCapturingGroups() > 0) {
    // Create a simple match scenario
    absl::string_view match_text = text;
    std::vector<absl::string_view> matches;
    
    // For simplicity, use empty matches array - Rewrite will fail but we test the path
    std::string rewrite_output;
    bool rewrite_success = re.Rewrite(&rewrite_output, simple_rewrite, 
                                     matches.data(), matches.size());
  }
  
  // Test 6: MaxSubmatch static function
  int max_submatch = RE2::MaxSubmatch(simple_rewrite);
  
  // Test 7: Test with empty rewrite string
  std::string empty_rewrite = "";
  std::string error_empty;
  bool check_empty = re.CheckRewriteString(empty_rewrite, &error_empty);
  if (check_empty) {
    std::string empty_replace_text = text;
    RE2::Replace(&empty_replace_text, re, empty_rewrite);
    
    std::string empty_global_text = text;
    RE2::GlobalReplace(&empty_global_text, re, empty_rewrite);
  }
  
  // Test 8: Test with rewrite strings containing only backreferences
  std::string only_backrefs;
  int num_groups = re.NumberOfCapturingGroups();
  if (num_groups > 0) {
    for (int i = 0; i <= std::min(num_groups, 3); i++) {
      only_backrefs += "\\";
      only_backrefs += std::to_string(i);
    }
    std::string error_backrefs;
    bool check_backrefs = re.CheckRewriteString(only_backrefs, &error_backrefs);
    
    if (check_backrefs) {
      std::string backref_text = text;
      RE2::Replace(&backref_text, re, only_backrefs);
    }
  }
  
  // Test 9: Test with malformed rewrite strings (should fail CheckRewriteString)
  std::string malformed = "\\a";  // Invalid escape sequence
  std::string error_malformed;
  bool check_malformed = re.CheckRewriteString(malformed, &error_malformed);
  // Should return false with error message
  
  // Test 10: Test with rewrite strings that reference non-existent groups
  std::string nonexistent_ref = "\\99";  // Assuming we don't have 99 groups
  std::string error_nonexistent;
  bool check_nonexistent = re.CheckRewriteString(nonexistent_ref, &error_nonexistent);
  // Should return false if we don't have that many capturing groups
  
  // Test 11: Test Replace and GlobalReplace with empty pattern (should match everything)
  std::string empty_pattern_text = text;
  RE2 empty_re("", options);
  if (empty_re.ok()) {
    std::string empty_pattern_replace = text;
    RE2::Replace(&empty_pattern_replace, empty_re, "X");
    
    std::string empty_pattern_global = text;
    RE2::GlobalReplace(&empty_pattern_global, empty_re, "Y");
  }
  
  // Test 12: Test with literal patterns (literal flag set)
  if (options.literal()) {
    std::string literal_text = text;
    RE2::Replace(&literal_text, re, "literal_replacement");
  }
  
  // Test 13: Test case-insensitive replacements
  if (!options.case_sensitive()) {
    std::string case_text = text;
    RE2::GlobalReplace(&case_text, re, "case_insensitive");
  }
  
  // Test 14: Test with posix syntax patterns
  if (options.posix_syntax()) {
    std::string posix_text = text;
    RE2::Replace(&posix_text, re, "posix_replacement");
  }

  return 0;
}

} // namespace harness_005
