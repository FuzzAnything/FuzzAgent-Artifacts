#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/set.h"

namespace harness_003 {
// RE2::Set multi-pattern matching harness
// Targets: RE2::Set constructor, Set::Add, Set::Compile, Set::Size, Set::Match (both overloads)
// These APIs currently have 0% coverage and represent 118+ branches of undiscovered complexity



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need at least some data for pattern and text
  if (size < 16) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Create RE2::Options with fuzzer-driven settings
  re2::RE2::Options options;
  
  // Configure options based on fuzzer input
  options.set_case_sensitive(fdp.ConsumeBool());
  options.set_posix_syntax(fdp.ConsumeBool());
  options.set_longest_match(fdp.ConsumeBool());
  options.set_log_errors(fdp.ConsumeBool());
  options.set_literal(fdp.ConsumeBool());
  options.set_never_nl(fdp.ConsumeBool());
  options.set_dot_nl(fdp.ConsumeBool());
  options.set_never_capture(fdp.ConsumeBool());
  options.set_perl_classes(fdp.ConsumeBool());
  options.set_word_boundary(fdp.ConsumeBool());
  options.set_one_line(fdp.ConsumeBool());
  
  // Encoding choice (UTF-8 or Latin-1)
  if (fdp.ConsumeBool()) {
    options.set_encoding(re2::RE2::Options::EncodingUTF8);
  } else {
    options.set_encoding(re2::RE2::Options::EncodingLatin1);
  }

  // 2. Create RE2::Set with options and anchor type
  // Choose anchor type based on fuzzer input
  re2::RE2::Anchor anchor = re2::RE2::Anchor::UNANCHORED;
  uint8_t anchor_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
  switch (anchor_choice) {
    case 0:
      anchor = re2::RE2::Anchor::UNANCHORED;
      break;
    case 1:
      anchor = re2::RE2::Anchor::ANCHOR_START;
      break;
    case 2:
      anchor = re2::RE2::Anchor::ANCHOR_BOTH;
      break;
  }

  // Create the RE2::Set object
  re2::RE2::Set regex_set(options, anchor);

  // 3. Add patterns to the set
  // Determine number of patterns to add (1-5 patterns)
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 5);
  std::vector<int> pattern_indices;
  std::vector<std::string> error_strings(num_patterns);

  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
    // Consume pattern string
    std::string pattern = fdp.ConsumeRandomLengthString(128);
    
    // Decide whether to use error string parameter (nullptr or actual string)
    std::string* error_ptr = nullptr;
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
      error_ptr = &error_strings[i];
    }
    
    // Add pattern to set
    int pattern_index = regex_set.Add(pattern, error_ptr);
    pattern_indices.push_back(pattern_index);
    
    // Check if pattern was successfully added (index >= 0)
    // Negative index indicates parsing error
  }

  // 4. Compile the set
  bool compile_success = regex_set.Compile();
  
  // Only proceed with matching if compilation was successful
  if (!compile_success) {
    // Compilation failed, return early
    return 0;
  }

  // 5. Verify set size
  int set_size = regex_set.Size();
  // set_size should match number of successfully added patterns

  // 6. Match text against compiled set using both overloads
  
  // Consume text to match
  std::string text_to_match = fdp.ConsumeRandomLengthString(256);
  
  // Prepare vector for matching indices
  std::vector<int> matching_indices;
  
  // First overload: Match without ErrorInfo parameter
  bool match_result1 = regex_set.Match(text_to_match, &matching_indices);
  
  // Clear vector for second test
  matching_indices.clear();
  
  // Second overload: Match with ErrorInfo parameter
  re2::RE2::Set::ErrorInfo error_info;
  bool match_result2 = regex_set.Match(text_to_match, &matching_indices, &error_info);
  
  // Test edge cases:
  // - Match with nullptr vector (vector parameter can be nullptr)
  bool match_result3 = regex_set.Match(text_to_match, nullptr);
  
  // - Match with nullptr vector and nullptr error_info
  bool match_result4 = regex_set.Match(text_to_match, nullptr, nullptr);
  
  // - Match with error_info but no vector
  re2::RE2::Set::ErrorInfo error_info2;
  bool match_result5 = regex_set.Match(text_to_match, nullptr, &error_info2);

  // 7. RAII cleanup happens automatically when regex_set goes out of scope
  
  return 0;
}

} // namespace harness_003
