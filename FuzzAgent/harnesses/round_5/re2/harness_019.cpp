#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_019 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  // We need pattern, text, and various options configuration
  if (size < 32) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Strategy: Focus on comprehensive testing of RE2::Options configuration
  // and how different option combinations affect regex matching behavior.
  // This harness differs from existing ones by:
  // 1. Systematically testing option interactions
  // 2. Focusing on edge cases with option combinations
  // 3. Testing how options affect different types of regex patterns
  
  // Step 1: Consume pattern and text strings first
  std::string pattern = fdp.ConsumeRandomLengthString(500);
  std::string text = fdp.ConsumeRandomLengthString(1000);
  
  // Step 2: Create multiple RE2::Options configurations to test
  
  // Configuration 1: Default options (as baseline)
  RE2::Options options1;
  // Default: UTF-8, case_sensitive=true, etc.
  
  // Configuration 2: Latin1 encoding
  RE2::Options options2;
  options2.set_encoding(RE2::Options::EncodingLatin1);
  options2.set_case_sensitive(fdp.ConsumeBool());
  
  // Configuration 3: Case insensitive with various flags
  RE2::Options options3;
  options3.set_case_sensitive(false);  // Case insensitive
  options3.set_literal(fdp.ConsumeBool());
  options3.set_never_nl(fdp.ConsumeBool());
  options3.set_dot_nl(fdp.ConsumeBool());
  
  // Configuration 4: POSIX syntax mode with associated flags
  RE2::Options options4;
  options4.set_posix_syntax(true);
  options4.set_perl_classes(fdp.ConsumeBool());
  options4.set_word_boundary(fdp.ConsumeBool());
  options4.set_one_line(fdp.ConsumeBool());
  options4.set_longest_match(fdp.ConsumeBool());
  
  // Configuration 5: Literal mode (treat pattern as literal string)
  RE2::Options options5;
  options5.set_literal(true);
  options5.set_case_sensitive(fdp.ConsumeBool());
  
  // Configuration 6: Never capture mode
  RE2::Options options6;
  options6.set_never_capture(true);
  options6.set_case_sensitive(fdp.ConsumeBool());
  
  // Configuration 7: Complex combination (testing interaction effects)
  RE2::Options options7;
  options7.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingUTF8 
                                         : RE2::Options::EncodingLatin1);
  options7.set_posix_syntax(fdp.ConsumeBool());
  options7.set_longest_match(fdp.ConsumeBool());
  options7.set_log_errors(false);  // Disable logging for fuzzing
  options7.set_max_mem(fdp.ConsumeIntegralInRange<int64_t>(1 << 16, 64 << 20));  // 64KB to 64MB
  options7.set_literal(fdp.ConsumeBool());
  options7.set_never_nl(fdp.ConsumeBool());
  options7.set_dot_nl(fdp.ConsumeBool());
  options7.set_never_capture(fdp.ConsumeBool());
  options7.set_case_sensitive(fdp.ConsumeBool());
  
  // When posix_syntax is true, also set posix-specific flags
  if (options7.posix_syntax()) {
    options7.set_perl_classes(fdp.ConsumeBool());
    options7.set_word_boundary(fdp.ConsumeBool());
    options7.set_one_line(fdp.ConsumeBool());
  }
  
  // Step 3: Test each configuration with the pattern
  
  // Helper function to test a single configuration
  auto test_configuration = [&text](const std::string& pattern, 
                                   const RE2::Options& options, 
                                   const char* config_name) {
    // Create RE2 object
    RE2 re(pattern, options);
    
    // Skip if pattern is invalid
    if (!re.ok()) {
      return;
    }
    
    // Test basic matching operations
    
    // FullMatch
    RE2::FullMatch(text, re);
    
    // PartialMatch  
    RE2::PartialMatch(text, re);
    
    // Test with string_view
    absl::string_view text_view(text);
    RE2::Consume(&text_view, re);
    
    // Reset string_view
    text_view = text;
    RE2::FindAndConsume(&text_view, re);
    
    // Test capture groups if pattern has any
    if (re.NumberOfCapturingGroups() > 0 && !options.never_capture()) {
      std::string capture_str;
      int capture_int;
      double capture_double;
      
      absl::string_view text_view2(text);
      RE2::FullMatch(text_view2, re, &capture_str, &capture_int, &capture_double);
      
      text_view2 = text;
      RE2::PartialMatch(text_view2, re, &capture_str);
    }
    
    // Test Replace operations
    std::string replace_text = text;
    std::string replacement = "REPL";
    RE2::Replace(&replace_text, re, replacement);
    
    replace_text = text;
    RE2::GlobalReplace(&replace_text, re, replacement);
  };
  
  // Test all configurations
  test_configuration(pattern, options1, "default");
  test_configuration(pattern, options2, "latin1");
  test_configuration(pattern, options3, "case_insensitive");
  test_configuration(pattern, options4, "posix_syntax");
  test_configuration(pattern, options5, "literal");
  test_configuration(pattern, options6, "never_capture");
  test_configuration(pattern, options7, "complex");
  
  // Step 4: Test special pattern cases with different options
  
  // Empty pattern
  std::string empty_pattern = "";
  test_configuration(empty_pattern, options1, "empty_pattern_default");
  test_configuration(empty_pattern, options7, "empty_pattern_complex");
  
  // Very long pattern (edge case)
  std::string long_pattern;
  if (fdp.remaining_bytes() > 100) {
    long_pattern = fdp.ConsumeRandomLengthString(500);
    test_configuration(long_pattern, options1, "long_pattern_default");
    test_configuration(long_pattern, options7, "long_pattern_complex");
  }
  
  // Pattern with special characters
  std::string special_pattern = ".*?\\d+\\w{3,5}[a-z]|(foo|bar)";
  test_configuration(special_pattern, options1, "special_default");
  test_configuration(special_pattern, options4, "special_posix");  // POSIX might handle differently
  
  // Pattern with Unicode (if UTF-8 encoding)
  if (fdp.remaining_bytes() > 50) {
    std::string unicode_pattern = fdp.ConsumeRandomLengthString(100);
    // Test with UTF-8 encoding
    RE2::Options unicode_options;
    unicode_options.set_encoding(RE2::Options::EncodingUTF8);
    unicode_options.set_case_sensitive(false);
    test_configuration(unicode_pattern, unicode_options, "unicode_utf8");
    
    // Test same pattern with Latin1 encoding
    RE2::Options latin1_options;
    latin1_options.set_encoding(RE2::Options::EncodingLatin1);
    latin1_options.set_case_sensitive(false);
    test_configuration(unicode_pattern, latin1_options, "unicode_latin1");
  }
  
  // Step 5: Test option interaction effects
  
  // Test case_sensitive interaction with literal mode
  if (fdp.remaining_bytes() > 20) {
    std::string simple_pattern = fdp.ConsumeRandomLengthString(20);
    
    // Case sensitive literal
    RE2::Options cs_literal;
    cs_literal.set_literal(true);
    cs_literal.set_case_sensitive(true);
    test_configuration(simple_pattern, cs_literal, "cs_literal");
    
    // Case insensitive literal  
    RE2::Options ci_literal;
    ci_literal.set_literal(true);
    ci_literal.set_case_sensitive(false);
    test_configuration(simple_pattern, ci_literal, "ci_literal");
  }
  
  // Test never_nl and dot_nl interaction
  if (fdp.remaining_bytes() > 20) {
    std::string dot_pattern = "a.b";
    std::string text_with_newline = "a\nb";
    
    // dot_nl = false (default) - dot doesn't match newline
    RE2::Options dot_nl_false;
    dot_nl_false.set_dot_nl(false);
    test_configuration(dot_pattern, dot_nl_false, "dot_nl_false");
    
    // dot_nl = true - dot matches newline
    RE2::Options dot_nl_true;
    dot_nl_true.set_dot_nl(true);
    test_configuration(dot_pattern, dot_nl_true, "dot_nl_true");
    
    // never_nl = true - never match newline
    RE2::Options never_nl_true;
    never_nl_true.set_never_nl(true);
    test_configuration(dot_pattern, never_nl_true, "never_nl_true");
  }
  
  // Step 6: Test memory limit option
  if (fdp.remaining_bytes() > 10) {
    // Test with very small memory limit
    RE2::Options small_mem;
    small_mem.set_max_mem(1024);  // 1KB - very small
    test_configuration(pattern, small_mem, "small_mem");
    
    // Test with large memory limit
    RE2::Options large_mem;
    large_mem.set_max_mem(256 << 20);  // 256MB
    test_configuration(pattern, large_mem, "large_mem");
  }
  
  // Step 7: Test error handling option
  RE2::Options no_log_errors;
  no_log_errors.set_log_errors(false);
  test_configuration(pattern, no_log_errors, "no_log_errors");
  
  // Step 8: Exercise additional API functions related to options
  
  // Test options() method to retrieve options from RE2 object
  RE2 re(pattern, options7);
  if (re.ok()) {
    const RE2::Options& retrieved_options = re.options();
    
    // Verify some option values match (just exercise the API)
    bool encoding_match = (retrieved_options.encoding() == options7.encoding());
    bool case_sensitive_match = (retrieved_options.case_sensitive() == options7.case_sensitive());
    bool literal_match = (retrieved_options.literal() == options7.literal());
    
    (void)encoding_match;
    (void)case_sensitive_match;
    (void)literal_match;
  }
  
  return 0;
}

} // namespace harness_019
