#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/set.h"

namespace harness_010 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing
  if (size < 16) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Create RE2::Set object with fuzzed configuration
  RE2::Options options;
  
  // Consume configuration flags from fuzzer input
  uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
  
  // Configure options based on fuzzed input
  if (config_flags & 0x01) options.set_encoding(RE2::Options::EncodingLatin1);
  if (config_flags & 0x02) options.set_posix_syntax(true);
  if (config_flags & 0x04) options.set_longest_match(true);
  if (config_flags & 0x08) options.set_log_errors(false);
  if (config_flags & 0x10) options.set_literal(true);
  if (config_flags & 0x20) options.set_never_nl(true);
  if (config_flags & 0x40) options.set_dot_nl(true);
  // Note: set_never_capture(true) is automatically set by RE2::Set constructor
  
  // Set max memory from fuzzed input (0-64KB range for fuzzing)
  if (fdp.remaining_bytes() >= sizeof(int64_t)) {
    int64_t max_mem = fdp.ConsumeIntegralInRange<int64_t>(0, 65536);
    options.set_max_mem(max_mem);
  }
  
  // Choose anchor type (unanchored or anchored)
  RE2::Anchor anchor = RE2::UNANCHORED;
  if (fdp.ConsumeBool()) {
    anchor = RE2::ANCHOR_BOTH;
  }
  
  // Create RE2::Set object
  RE2::Set set(options, anchor);
  
  // Step 2: Add patterns to the set
  // Determine number of patterns to add (1-10 for reasonable fuzzing)
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 10);
  
  std::vector<std::string> error_strings;
  std::vector<int> pattern_indices;
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
    // Consume pattern string length (1-200 chars)
    size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 200);
    std::string pattern;
    
    // For some patterns, generate invalid regex to test error handling
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= pattern_len) {
      // Valid pattern - consume from fuzzer input
      pattern = fdp.ConsumeBytesAsString(pattern_len);
    } else {
      // Possibly invalid pattern - use random string
      pattern = fdp.ConsumeRandomLengthString(pattern_len);
    }
    
    // Decide whether to capture error string
    std::string* error_ptr = nullptr;
    std::string error;
    if (fdp.ConsumeBool()) {
      error_ptr = &error;
    }
    
    // Add pattern to set
    int index = set.Add(pattern, error_ptr);
    pattern_indices.push_back(index);
    
    if (error_ptr && !error.empty()) {
      error_strings.push_back(error);
    }
  }
  
  // Step 3: Compile the set
  bool compile_success = set.Compile();
  
  // Check size before and after compilation
  int size_before = set.Size();
  
  // Step 4: Test Match() operations with various input strings
  // Only proceed if compilation was successful
  if (compile_success) {
    // Determine number of match tests to perform (1-20)
    size_t num_tests = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_tests && fdp.remaining_bytes() > 0; i++) {
      // Consume test string with various lengths including edge cases
      size_t text_len = 0;
      if (fdp.ConsumeBool()) {
        // Edge case: empty string
        text_len = 0;
      } else if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(size_t)) {
        // Very long string (up to 4096 bytes)
        text_len = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
      } else {
        // Normal string (1-256 bytes)
        text_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
      }
      
      std::string text;
      if (text_len > 0 && fdp.remaining_bytes() >= text_len) {
        text = fdp.ConsumeBytesAsString(text_len);
      } else if (text_len > 0) {
        // Not enough bytes, use random length string
        text = fdp.ConsumeRandomLengthString(text_len);
      }
      
      // Test both Match() overloads
      bool use_vector = fdp.ConsumeBool();
      bool use_error_info = fdp.ConsumeBool();
      
      if (use_vector && use_error_info) {
        // Use Match() with both vector and ErrorInfo
        std::vector<int> matches;
        RE2::Set::ErrorInfo error_info;
        bool match_result = set.Match(text, &matches, &error_info);
        
        // Exercise the ErrorInfo structure
        (void)error_info.kind;
      } else if (use_vector) {
        // Use Match() with vector only
        std::vector<int> matches;
        bool match_result = set.Match(text, &matches);
      } else if (use_error_info) {
        // Use Match() with ErrorInfo only
        RE2::Set::ErrorInfo error_info;
        bool match_result = set.Match(text, nullptr, &error_info);
        (void)error_info.kind;
      } else {
        // Use simple Match() without vector or error info
        bool match_result = set.Match(text, nullptr);
      }
    }
    
    // Test edge cases specifically
    // Test with empty string
    std::vector<int> empty_matches;
    RE2::Set::ErrorInfo empty_error_info;
    bool empty_match = set.Match("", &empty_matches, &empty_error_info);
    
    // Test with very long string (if we have enough bytes)
    if (fdp.remaining_bytes() > 1024) {
      std::string long_text = fdp.ConsumeBytesAsString(1024);
      std::vector<int> long_matches;
      bool long_match = set.Match(long_text, &long_matches);
    }
    
    // Test Unicode strings (UTF-8)
    if (fdp.remaining_bytes() > 100) {
      // Create a UTF-8 string with various characters
      std::string utf8_text;
      // Add some UTF-8 characters
      utf8_text += "Hello ";
      utf8_text += "\xC3\xA9";  // é
      utf8_text += " ";
      utf8_text += "\xE2\x82\xAC";  // €
      utf8_text += " ";
      utf8_text += "\xF0\x9F\x98\x80";  // 😀
      
      // Add remaining fuzzed bytes
      size_t remaining_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
      if (remaining_len > 0 && fdp.remaining_bytes() >= remaining_len) {
        utf8_text += fdp.ConsumeBytesAsString(remaining_len);
      }
      
      std::vector<int> utf8_matches;
      bool utf8_match = set.Match(utf8_text, &utf8_matches);
    }
  }
  
  // Step 5: Test error paths by trying to add patterns after compilation
  // This should fail and trigger error paths
  if (compile_success && fdp.remaining_bytes() > 10) {
    std::string post_compile_pattern = fdp.ConsumeRandomLengthString(10);
    std::string error;
    int post_index = set.Add(post_compile_pattern, &error);
    // Should return -1 after compilation
  }
  
  // Step 6: Test Match() before compilation (should trigger error paths)
  // Create a new set but don't compile it
  if (fdp.remaining_bytes() > 10) {
    RE2::Set uncompiled_set(options, anchor);
    
    // Add a pattern
    std::string pattern = fdp.ConsumeRandomLengthString(10);
    uncompiled_set.Add(pattern, nullptr);
    
    // Try to match before compilation (should trigger DFATAL)
    std::string test_text = fdp.ConsumeRandomLengthString(10);
    std::vector<int> matches;
    RE2::Set::ErrorInfo error_info;
    bool match_result = uncompiled_set.Match(test_text, &matches, &error_info);
  }
  
  // Step 7: Test move semantics (if enough bytes remain)
  if (fdp.remaining_bytes() > 20 && compile_success) {
    // Move the set to test move constructor
    RE2::Set moved_set = std::move(set);
    
    // Test that moved set still works
    if (fdp.remaining_bytes() > 10) {
      std::string test_text = fdp.ConsumeRandomLengthString(10);
      std::vector<int> matches;
      bool match_result = moved_set.Match(test_text, &matches);
    }
    
    // Create another set and test move assignment
    RE2::Set another_set(options, anchor);
    if (fdp.remaining_bytes() > 10) {
      std::string pattern = fdp.ConsumeRandomLengthString(10);
      another_set.Add(pattern, nullptr);
      another_set.Compile();
    }
    
    // Move assign
    another_set = std::move(moved_set);
  }
  
  // Set object will be automatically destroyed when out of scope
  // RE2::Set destructor handles cleanup
  
  return 0;
}

} // namespace harness_010
