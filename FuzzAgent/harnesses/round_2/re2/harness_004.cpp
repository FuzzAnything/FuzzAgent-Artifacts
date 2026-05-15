#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_004 {
// RE2::Extract fuzzing harness
// Targets the RE2::Extract API which has 196 undiscovered branches
// Tests text extraction operations with various patterns and rewrite strings



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

  // Consume pattern, text, and rewrite strings from fuzzer input
  // Split input into three parts for pattern, text, and rewrite
  std::string pattern = fdp.ConsumeRandomLengthString(300);
  std::string text = fdp.ConsumeRandomLengthString(300);
  std::string rewrite = fdp.ConsumeRandomLengthString(100);
  
  // If we still have input left, create multiple extraction scenarios
  if (fdp.remaining_bytes() > 10) {
    // Create RE2 object with the pattern and options
    RE2 re(pattern, options);
    
    // If RE2 construction failed, try with a simple pattern
    if (!re.ok()) {
      // Try with a simpler pattern for extraction
      RE2 simple_re(".*", options);
      if (simple_re.ok()) {
        std::string output;
        RE2::Extract(text, simple_re, rewrite, &output);
      }
      return 0;
    }
    
    // Test 1: Basic extraction with the consumed strings
    std::string output1;
    bool success1 = RE2::Extract(text, re, rewrite, &output1);
    
    // Test 2: Test with empty rewrite string
    std::string output2;
    RE2::Extract(text, re, "", &output2);
    
    // Test 3: Test with numeric backreferences if we have capturing groups
    if (re.NumberOfCapturingGroups() > 0) {
      // Create rewrite strings with various backreferences
      std::vector<std::string> rewrite_variants;
      
      // Simple backreference
      rewrite_variants.push_back("\\0");
      
      // Multiple backreferences if we have enough groups
      if (re.NumberOfCapturingGroups() >= 2) {
        rewrite_variants.push_back("\\1");
        rewrite_variants.push_back("\\2");
        rewrite_variants.push_back("\\1-\\2");
      }
      
      if (re.NumberOfCapturingGroups() >= 3) {
        rewrite_variants.push_back("\\1\\2\\3");
      }
      
      // Test each rewrite variant
      for (const auto& rw : rewrite_variants) {
        std::string output;
        RE2::Extract(text, re, rw, &output);
      }
    }
    
    // Test 4: Test with literal rewrite (no backreferences)
    // Consume a literal rewrite string from fuzzer input
    std::string literal_rewrite = fdp.ConsumeRandomLengthString(50);
    std::string literal_output;
    RE2::Extract(text, re, literal_rewrite, &literal_output);
    
    // Test 5: Test with complex rewrite containing escaped characters
    // Consume another rewrite string for complex testing
    std::string complex_rewrite = fdp.ConsumeRandomLengthString(50);
    std::string complex_output;
    RE2::Extract(text, re, complex_rewrite, &complex_output);
    
    // Test 6: Test extraction with different text inputs
    // Use remaining input for additional text variations
    while (fdp.remaining_bytes() > 20) {
      std::string alt_text = fdp.ConsumeRandomLengthString(50);
      std::string alt_output;
      RE2::Extract(alt_text, re, rewrite, &alt_output);
    }
  } else {
    // Minimal test with basic pattern
    RE2 re(".*", options);
    if (re.ok()) {
      std::string output;
      RE2::Extract(text, re, rewrite, &output);
    }
  }

  return 0;
}

} // namespace harness_004
