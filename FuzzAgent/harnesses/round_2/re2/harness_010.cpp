#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <optional>
#include "absl/strings/string_view.h"
#include "re2/re2.h"

namespace harness_010 {
// RE2::Arg argument parsing system fuzzing harness
// Targets RE2::Arg::Parse (0% coverage) and related match functions with 111 undiscovered branches each
// Semantically unique from existing harnesses by focusing exclusively on typed argument parsing
// Implements comprehensive testing of RE2 argument parsing and typed match extraction



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum input size: need pattern + text + some argument data
  if (size < 20) {
    return 0;
  }

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

  // Consume pattern with capture groups for argument extraction
  std::string pattern = fdp.ConsumeRandomLengthString(100);
  
  // Create RE2 object with the pattern and options
  RE2 re(pattern, options);
  
  // If RE2 construction failed, return early
  if (!re.ok()) {
    return 0;
  }

  // Determine number of capture groups in the pattern
  int num_capture_groups = re.NumberOfCapturingGroups();
  if (num_capture_groups < 1) {
    // Need at least one capture group for argument parsing
    // Create a new RE2 object with capture groups
    RE2 capture_re("(\\w+):(\\d+):([\\d\\.]+)", options);
    if (!capture_re.ok()) {
      return 0;
    }
    // We'll use this capture_re for testing instead
    // Note: We can't reassign, so we'll create a reference to use in tests
    RE2& test_re = capture_re;
    num_capture_groups = test_re.NumberOfCapturingGroups();
    
    // Consume text to match against
    std::string text = fdp.ConsumeRandomLengthString(200);
    
    // Run tests with test_re
    // Test 1: Basic types with FullMatchN
    int int_val;
    long long_val;
    double double_val;
    std::string string_val;
    
    RE2::Arg arg_int(&int_val);
    RE2::Arg arg_long(&long_val);
    RE2::Arg arg_double(&double_val);
    RE2::Arg arg_string(&string_val);
    
    const RE2::Arg* args[] = {&arg_int, &arg_long, &arg_double};
    if (num_capture_groups >= 3) {
      RE2::FullMatchN(text, test_re, args, 3);
    }
    
    // Test 2: PartialMatchN with string_view
    absl::string_view string_view_val;
    short short_val;
    RE2::Arg arg_string_view(&string_view_val);
    RE2::Arg arg_short(&short_val);
    
    const RE2::Arg* args_partial[] = {&arg_string_view, &arg_short};
    if (num_capture_groups >= 2) {
      RE2::PartialMatchN(text, test_re, args_partial, 2);
    }
    
    return 0;
  }

  // Consume text to match against - should contain data matching the pattern
  std::string text = fdp.ConsumeRandomLengthString(200);

  // Test 1: RE2::FullMatchN with various argument types
  // Create arrays of RE2::Arg objects for different type combinations
  
  // Simple test with basic types
  if (fdp.remaining_bytes() > sizeof(int) + sizeof(long) + sizeof(double)) {
    int int_val;
    long long_val;
    double double_val;
    std::string string_val;
    std::optional<float> optional_float_val;
    
    RE2::Arg arg_int(&int_val);
    RE2::Arg arg_long(&long_val);
    RE2::Arg arg_double(&double_val);
    RE2::Arg arg_string(&string_val);
    RE2::Arg arg_optional_float(&optional_float_val);
    
    const RE2::Arg* args_simple[] = {&arg_int, &arg_long, &arg_double};
    const RE2::Arg* args_mixed[] = {&arg_string, &arg_int, &arg_optional_float};
    
    // Try FullMatchN with different argument combinations
    if (num_capture_groups >= 3) {
      RE2::FullMatchN(text, re, args_simple, 3);
    }
    
    if (num_capture_groups >= 3) {
      RE2::FullMatchN(text, re, args_mixed, 3);
    }
  }

  // Test 2: RE2::PartialMatchN with string_view and numeric types
  if (fdp.remaining_bytes() > 10) {
    absl::string_view string_view_val;
    short short_val;
    unsigned int uint_val;
    
    RE2::Arg arg_string_view(&string_view_val);
    RE2::Arg arg_short(&short_val);
    RE2::Arg arg_uint(&uint_val);
    
    const RE2::Arg* args_partial[] = {&arg_string_view, &arg_short, &arg_uint};
    
    if (num_capture_groups >= 3) {
      RE2::PartialMatchN(text, re, args_partial, 3);
    }
  }

  // Test 3: RE2::ConsumeN with mutable string_view and typed parsing
  if (fdp.remaining_bytes() > 10) {
    absl::string_view mutable_text(text);
    
    int8_t int8_val;
    uint16_t uint16_val;
    int32_t int32_val;
    uint64_t uint64_val;
    std::optional<double> optional_double_val;
    
    RE2::Arg arg_int8(&int8_val);
    RE2::Arg arg_uint16(&uint16_val);
    RE2::Arg arg_int32(&int32_val);
    RE2::Arg arg_uint64(&uint64_val);
    RE2::Arg arg_optional_double(&optional_double_val);
    
    const RE2::Arg* args_consume[] = {&arg_int8, &arg_uint16, &arg_int32, &arg_uint64, &arg_optional_double};
    
    // Test with different numbers of arguments
    if (num_capture_groups >= 2) {
      RE2::ConsumeN(&mutable_text, re, args_consume, 2);
    }
    
    if (num_capture_groups >= 5) {
      RE2::ConsumeN(&mutable_text, re, args_consume, 5);
    }
  }

  // Test 4: RE2::FindAndConsumeN with various numeric radices
  if (fdp.remaining_bytes() > 10) {
    absl::string_view find_text(text);
    
    int hex_val;
    int octal_val;
    int cradix_val;  // C-style radix (0 for octal, 0x for hex)
    float float_val;
    std::string another_string_val;
    
    // Create arguments with different radices using RE2::Hex, RE2::Octal, RE2::CRadix
    RE2::Arg arg_hex = RE2::Hex(&hex_val);
    RE2::Arg arg_octal = RE2::Octal(&octal_val);
    RE2::Arg arg_cradix = RE2::CRadix(&cradix_val);
    RE2::Arg arg_float(&float_val);
    RE2::Arg arg_another_string(&another_string_val);
    
    const RE2::Arg* args_find[] = {&arg_hex, &arg_octal, &arg_cradix, &arg_float, &arg_another_string};
    
    // Try different argument counts
    if (num_capture_groups >= 3) {
      RE2::FindAndConsumeN(&find_text, re, args_find, 3);
    }
    
    if (num_capture_groups >= 5) {
      RE2::FindAndConsumeN(&find_text, re, args_find, 5);
    }
  }

  // Test 5: Direct RE2::Arg::Parse testing with various data types
  // Consume test strings and parse them directly
  if (fdp.remaining_bytes() > 50) {
    // Consume strings to parse
    std::string parse_str1 = fdp.ConsumeRandomLengthString(20);
    std::string parse_str2 = fdp.ConsumeRandomLengthString(20);
    std::string parse_str3 = fdp.ConsumeRandomLengthString(20);
    
    // Test parsing into different types
    int64_t int64_val;
    uint32_t uint32_val;
    double another_double_val;
    std::optional<int32_t> optional_int32_val;
    
    RE2::Arg arg_int64(&int64_val);
    RE2::Arg arg_uint32(&uint32_val);
    RE2::Arg arg_another_double(&another_double_val);
    RE2::Arg arg_optional_int32(&optional_int32_val);
    
    // Direct Parse calls
    arg_int64.Parse(parse_str1.c_str(), parse_str1.size());
    arg_uint32.Parse(parse_str2.c_str(), parse_str2.size());
    arg_another_double.Parse(parse_str3.c_str(), parse_str3.size());
    arg_optional_int32.Parse(parse_str1.c_str(), parse_str1.size());
    
    // Test with nullptr (should succeed)
    RE2::Arg null_arg(nullptr);
    null_arg.Parse(parse_str1.c_str(), parse_str1.size());
    
    // Test with empty string
    arg_int64.Parse("", 0);
    arg_optional_int32.Parse("", 0);
  }

  // Test 6: Edge cases and error conditions
  if (fdp.remaining_bytes() > 10) {
    // Test with patterns that have too few capture groups for the arguments
    std::string simple_pattern = "(\\d+)";
    RE2 simple_re(simple_pattern, options);
    
    if (simple_re.ok()) {
      int val1, val2, val3;
      RE2::Arg arg1(&val1), arg2(&val2), arg3(&val3);
      const RE2::Arg* too_many_args[] = {&arg1, &arg2, &arg3};
      
      // This should fail (pattern has 1 group, we're asking for 3)
      RE2::FullMatchN(text, simple_re, too_many_args, 3);
    }
    
    // Test with type mismatch (trying to parse non-numeric string as number)
    std::string non_numeric_text = fdp.ConsumeRandomLengthString(50);
    int should_fail_val;
    RE2::Arg arg_should_fail(&should_fail_val);
    
    // Create a pattern that captures arbitrary text
    RE2 text_re("(.*)", options);
    if (text_re.ok()) {
      const RE2::Arg* text_args[] = {&arg_should_fail};
      RE2::FullMatchN(non_numeric_text, text_re, text_args, 1);
    }
  }

  // Test 7: Mixed argument types with the template-based interface (FullMatch, PartialMatch)
  // This exercises the conversion from template arguments to RE2::Arg arrays internally
  if (fdp.remaining_bytes() > 20) {
    std::string mixed_text = fdp.ConsumeRandomLengthString(100);
    
    int mixed_int;
    std::string mixed_string;
    std::optional<long> mixed_optional_long;
    double mixed_double;
    
    // Use the template-based interface (converts to RE2::Arg internally)
    RE2::FullMatch(mixed_text, re, &mixed_int, &mixed_string);
    RE2::PartialMatch(mixed_text, re, &mixed_optional_long, &mixed_double);
    
    // Test with absl::string_view
    absl::string_view mixed_string_view;
    unsigned short mixed_ushort;
    RE2::FullMatch(mixed_text, re, &mixed_string_view, &mixed_ushort);
  }

  return 0;
}

} // namespace harness_010
