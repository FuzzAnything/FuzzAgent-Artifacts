#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_003 {
// RE2 basic fuzzing harness using public API only
// Tests RE2 constructor, options, and basic matching functions
// Simplified version without absl::string_view dependencies



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
  std::string pattern = fdp.ConsumeRandomLengthString(999);
  
  // Create RE2 object with the pattern and options
  RE2 re(pattern, options);
  
  // If RE2 construction failed, return early
  if (!re.ok()) {
    return 0;
  }
  
  // Consume text to match against
  std::string text = fdp.ConsumeRandomLengthString(999);
  
  // Test various matching functions
  
  // Test FullMatch
  RE2::FullMatch(text, re);
  
  // Test PartialMatch  
  RE2::PartialMatch(text, re);
  
  // Test Replace
  std::string replace_text = text;
  RE2::Replace(&replace_text, re, "");
  
  // Test GlobalReplace
  std::string global_text = text;
  RE2::GlobalReplace(&global_text, re, "");
  
  // Test PossibleMatchRange
  std::string min_match, max_match;
  re.PossibleMatchRange(&min_match, &max_match, /*maxlen=*/9);
  
  // Test if we have capturing groups
  if (re.NumberOfCapturingGroups() > 0) {
    // Test with capturing groups for various types
    
    // Test with short
    short short_val;
    RE2::FullMatch(text, re, &short_val);
    
    // Test with long
    long long_val;
    RE2::PartialMatch(text, re, &long_val);
    
    // Test with float
    std::string float_text = text;
    float float_val;
    // Note: Can't use Consume without absl::string_view
    
    // Test with double  
    std::string double_text = text;
    double double_val;
    // Note: Can't use FindAndConsume without absl::string_view
  }
  
  return 0;
}

} // namespace harness_003
