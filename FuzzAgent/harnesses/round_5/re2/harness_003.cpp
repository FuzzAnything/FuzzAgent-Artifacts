#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  // We need pattern, text, rewrite strings, and options
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
  
  // Test 1: CheckRewriteString - validate rewrite strings
  // Consume rewrite string for validation
  std::string rewrite_string = fdp.ConsumeRandomLengthString(200);
  std::string error;
  bool check_result = re.CheckRewriteString(rewrite_string, &error);
  
  // Test MaxSubmatch static function
  int max_submatch = RE2::MaxSubmatch(rewrite_string);
  
  // Test 2: Replace operations
  // Consume text to perform replacements on
  std::string text_for_replace = fdp.ConsumeRandomLengthString(1000);
  std::string text_copy = text_for_replace;
  
  // Try Replace operation
  bool replace_result = RE2::Replace(&text_copy, re, rewrite_string);
  
  // Test 3: GlobalReplace operations
  // Reset text and try GlobalReplace
  text_copy = text_for_replace;
  int global_replace_count = RE2::GlobalReplace(&text_copy, re, rewrite_string);
  
  // Test 4: Rewrite operations with various rewrite templates
  // First, we need to get some submatch results to use with Rewrite()
  // Perform a match to get submatch array
  int num_captures = re.NumberOfCapturingGroups();
  if (num_captures > 0 && fdp.remaining_bytes() > 20) {
    // Consume text to match against
    std::string match_text = fdp.ConsumeRandomLengthString(500);
    
    // Prepare submatch array
    std::vector<absl::string_view> submatches(num_captures + 1);  // +1 for full match
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });
    
    // Try to match
    bool match_result = re.Match(match_text, 0, match_text.length(), 
                                 anchor, submatches.data(), submatches.size());
    
    if (match_result) {
      // Now we have submatches, test Rewrite function
      std::string rewrite_output;
      bool rewrite_result = re.Rewrite(&rewrite_output, rewrite_string, 
                                       submatches.data(), submatches.size());
      
      // Also test with different rewrite strings
      if (fdp.remaining_bytes() > 50) {
        std::string rewrite_string2 = fdp.ConsumeRandomLengthString(100);
        std::string rewrite_output2;
        re.Rewrite(&rewrite_output2, rewrite_string2, 
                   submatches.data(), submatches.size());
      }
    }
  }
  
  // Test 5: Additional Replace/GlobalReplace with different text
  if (fdp.remaining_bytes() > 100) {
    std::string additional_text = fdp.ConsumeRandomLengthString(500);
    std::string additional_rewrite = fdp.ConsumeRandomLengthString(100);
    
    std::string text_copy2 = additional_text;
    RE2::Replace(&text_copy2, re, additional_rewrite);
    
    text_copy2 = additional_text;
    RE2::GlobalReplace(&text_copy2, re, additional_rewrite);
  }
  
  // Test 6: Extract function (related to rewrite operations)
  if (fdp.remaining_bytes() > 100) {
    std::string extract_text = fdp.ConsumeRandomLengthString(500);
    std::string extract_rewrite = fdp.ConsumeRandomLengthString(100);
    std::string extract_output;
    bool extract_result = RE2::Extract(extract_text, re, extract_rewrite, &extract_output);
  }
  
  return 0;
}

} // namespace harness_003
