#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check
  if (size < 8) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume RE2::Options settings from fuzzer input
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
  
  // Consume pattern and text strings
  std::string pattern = fdp.ConsumeRandomLengthString(999);
  std::string text = fdp.ConsumeRandomLengthString(999);
  
  // Create RE2 object
  RE2 re(pattern, options);
  if (!re.ok()) {
    return 0;  // Invalid pattern, nothing to test
  }
  
  // Test various matching functions with different argument types
  
  // Basic matching without capture groups
  RE2::FullMatch(text, re);
  RE2::PartialMatch(text, re);
  
  absl::string_view sp = text;
  RE2::Consume(&sp, re);
  sp = text;  // Reset
  RE2::FindAndConsume(&sp, re);
  
  // Test with capture groups if the pattern has any
  if (re.NumberOfCapturingGroups() > 0) {
    // Try with various data types
    absl::string_view sp2 = text;
    short s;
    RE2::FullMatch(sp2, re, &s);
    
    sp2 = text;
    long l;
    RE2::PartialMatch(sp2, re, &l);
    
    sp2 = text;
    float f;
    RE2::Consume(&sp2, re, &f);
    
    sp2 = text;
    double d;
    RE2::FindAndConsume(&sp2, re, &d);
    
    sp2 = text;
    std::string str;
    RE2::FullMatch(sp2, re, &str);
    
    sp2 = text;
    absl::string_view sv;
    RE2::PartialMatch(sp2, re, &sv);
  }
  
  // Test Replace functions
  std::string replace_text = text;
  std::string replacement = fdp.ConsumeRandomLengthString(100);
  RE2::Replace(&replace_text, re, replacement);
  
  replace_text = text;
  RE2::GlobalReplace(&replace_text, re, replacement);
  
  // Test PossibleMatchRange
  std::string min, max;
  re.PossibleMatchRange(&min, &max, /*maxlen=*/9);
  
  // Exercise other API functions
  re.NamedCapturingGroups();
  re.CapturingGroupNames();
  RE2::QuoteMeta(pattern);
  
  return 0;
}

} // namespace harness_000
