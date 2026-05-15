#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_004 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  // We need pattern, text, and various numeric/string values for captures
  if (size < 32) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume RE2::Options settings
  RE2::Options options;
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);  // Don't log errors during fuzzing
  options.set_max_mem(64 << 20);  // 64 MB memory limit
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_case_sensitive(!fdp.ConsumeBool());  // Default is true
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  
  // Consume pattern string for RE2 object
  std::string pattern = fdp.ConsumeRandomLengthString(500);
  
  // Create RE2 object with pattern
  RE2 re(pattern, options);
  
  // Validate RE2 object using ok() - if invalid, nothing to test
  if (!re.ok()) {
    return 0;
  }
  
  // Consume input text to match against
  std::string input_text = fdp.ConsumeRandomLengthString(1000);
  absl::string_view input_view(input_text);
  
  // Determine number of capture groups to test (0, 1, 2, 3+)
  // Use fuzzed data to decide, but limit to reasonable number
  uint8_t capture_group_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
  int num_captures = 0;
  
  switch (capture_group_choice) {
    case 0:
      num_captures = 0;
      break;
    case 1:
      num_captures = 1;
      break;
    case 2:
      num_captures = 2;
      break;
    case 3:
      num_captures = 3 + (fdp.ConsumeIntegral<uint8_t>() % 3);  // 3-5 captures
      break;
  }
  
  // Limit captures based on remaining input
  if (fdp.remaining_bytes() < (size_t)(num_captures * 10)) {
    return 0;
  }
  
  // Prepare arrays for Arg objects
  std::vector<RE2::Arg> argv(num_captures);
  std::vector<const RE2::Arg*> args(num_captures);
  
  // Prepare storage for capture results
  std::vector<std::string> string_results(num_captures);
  std::vector<int> int_results(num_captures);
  std::vector<double> double_results(num_captures);
  
  // Initialize Arg objects with different types based on fuzzed data
  for (int i = 0; i < num_captures; i++) {
    // Choose type for this capture: 0=string, 1=int, 2=double
    uint8_t type_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (type_choice) {
      case 0:  // String capture
        argv[i] = &string_results[i];
        break;
      case 1:  // Integer capture
        argv[i] = &int_results[i];
        break;
      case 2:  // Double capture
        argv[i] = &double_results[i];
        break;
    }
    args[i] = &argv[i];
  }
  
  // Test 1: ConsumeN - match from beginning of string and consume matched portion
  if (num_captures > 0) {
    absl::string_view consume_input = input_view;  // Copy for ConsumeN
    RE2::ConsumeN(&consume_input, re, args.data(), num_captures);
  } else {
    absl::string_view consume_input = input_view;  // Copy for ConsumeN
    RE2::ConsumeN(&consume_input, re, nullptr, 0);
  }
  
  // Test 2: FindAndConsumeN - find and consume matches within string
  if (num_captures > 0) {
    absl::string_view find_consume_input = input_view;  // Copy for FindAndConsumeN
    RE2::FindAndConsumeN(&find_consume_input, re, args.data(), num_captures);
  } else {
    absl::string_view find_consume_input = input_view;  // Copy for FindAndConsumeN
    RE2::FindAndConsumeN(&find_consume_input, re, nullptr, 0);
  }
  
  // Test 3: PartialMatchN - match anywhere in string
  if (num_captures > 0) {
    RE2::PartialMatchN(input_view, re, args.data(), num_captures);
  } else {
    RE2::PartialMatchN(input_view, re, nullptr, 0);
  }
  
  // Test 4: FullMatchN - match entire string
  if (num_captures > 0) {
    RE2::FullMatchN(input_view, re, args.data(), num_captures);
  } else {
    RE2::FullMatchN(input_view, re, nullptr, 0);
  }
  
  // Also test with different numeric parsing options
  if (num_captures >= 2 && fdp.remaining_bytes() > 20) {
    // Create specialized Arg objects for numeric parsing
    int hex_value, octal_value, cr_value;
    RE2::Arg hex_arg = RE2::Hex(&hex_value);
    RE2::Arg octal_arg = RE2::Octal(&octal_value);
    RE2::Arg cr_arg = RE2::CRadix(&cr_value);
    
    const RE2::Arg* special_args[] = { &hex_arg, &octal_arg, &cr_arg };
    
    // Test with patterns that might match numbers
    std::string num_pattern = fdp.ConsumeRandomLengthString(100);
    RE2 num_re(num_pattern, options);
    if (num_re.ok()) {
      RE2::PartialMatchN(input_view, num_re, special_args, 3);
    }
  }
  
  return 0;
}

} // namespace harness_004
