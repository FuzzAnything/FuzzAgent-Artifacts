#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <optional>
#include "re2/re2.h"

namespace harness_002 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 16) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume RE2::Options settings
  RE2::Options options;
  options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(false);
  options.set_max_mem(64 << 20);
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_case_sensitive(!fdp.ConsumeBool());
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
  
  // Test QuoteMeta function
  std::string quoted = RE2::QuoteMeta(pattern);
  
  // Test Extract function
  std::string rewrite = fdp.ConsumeRandomLengthString(100);
  std::string out;
  bool extract_result = RE2::Extract(text, re, rewrite, &out);
  
  // Test numeric parsing with Hex, Octal, CRadix
  if (fdp.remaining_bytes() > 20) {
    // Create a string that might represent a number
    std::string num_str = fdp.ConsumeRandomLengthString(20);
    
    // Try parsing with different radices
    short s;
    RE2::FullMatch(num_str, "([0-9a-fA-F]+)[uUlL]*", RE2::Hex(&s));
    
    unsigned short us;
    RE2::FullMatch(num_str, "([0-9a-fA-F]+)[uUlL]*", RE2::Hex(&us));
    
    int i;
    RE2::FullMatch(num_str, "([0-9]+)[uUlL]*", &i);
    
    unsigned int ui;
    RE2::FullMatch(num_str, "([0-9]+)[uUlL]*", &ui);
    
    long l;
    RE2::FullMatch(num_str, "([0-7]+)[uUlL]*", RE2::Octal(&l));
    
    unsigned long ul;
    RE2::FullMatch(num_str, "([0-7]+)[uUlL]*", RE2::Octal(&ul));
    
    // Test CRadix which auto-detects base (0x for hex, 0 for octal)
    long long ll;
    RE2::FullMatch(num_str, "([0-9a-fA-FxX]+)[uUlL]*", RE2::CRadix(&ll));
    
    unsigned long long ull;
    RE2::FullMatch(num_str, "([0-9a-fA-FxX]+)[uUlL]*", RE2::CRadix(&ull));
  }
  
  // Test Match function with explicit parameters (lower-level than FullMatch)
  if (fdp.remaining_bytes() > 10) {
    size_t startpos = fdp.ConsumeIntegralInRange<size_t>(0, text.length());
    size_t endpos = fdp.ConsumeIntegralInRange<size_t>(startpos, text.length());
    
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Prepare submatch array
    int nsubmatch = fdp.ConsumeIntegralInRange<int>(0, 5);
    std::vector<absl::string_view> submatches(nsubmatch);
    
    bool match_result = re.Match(text, startpos, endpos, anchor, 
                                 submatches.data(), nsubmatch);
  }
  
  // Test std::optional support (mentioned in RE2 documentation)
  if (re.NumberOfCapturingGroups() > 0) {
    std::optional<int> opt_int;
    std::optional<std::string> opt_str;
    
    absl::string_view sp = text;
    RE2::FullMatch(sp, re, &opt_str, &opt_int);
  }
  
  // Test NamedCapturingGroups and CapturingGroupNames
  auto named_groups = re.NamedCapturingGroups();
  auto group_names = re.CapturingGroupNames();
  
  // Test PossibleMatchRange with different maxlen values
  std::string min, max;
  int maxlen = fdp.ConsumeIntegralInRange<int>(1, 50);
  re.PossibleMatchRange(&min, &max, maxlen);
  
  // Test ProgramSize and related metrics
  int prog_size = re.ProgramSize();
  int rev_prog_size = re.ReverseProgramSize();
  
  // Test ProgramFanout
  std::vector<int> histogram;
  int fanout = re.ProgramFanout(&histogram);
  int rfanout = re.ReverseProgramFanout(&histogram);
  
  return 0;
}

} // namespace harness_002
