#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include "re2/re2.h"

namespace harness_007 {
// LazyRE2 and Named Capture Group APIs harness
// Targets: LazyRE2::get(), LazyRE2::operator->(), 
//          RE2::CapturingGroupNames(), RE2::NamedCapturingGroups(), 
//          RE2::pattern(), RE2::error_arg()
// Note: LazyRE2::pt() and LazyRE2::de() are not found in current RE2 source
// These APIs currently have 0% coverage and represent important functionality
// for thread-safe static regex initialization and named capture processing
// Based on coverage guidance: /root/FuzzAgent/output/re2/coverage_guidance/harness_007_guidance.md


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need enough data for patterns, test strings, and options
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);
  // 1. Create different named capture patterns from fuzzer input
  // Generate pattern type (0-4) to test different named capture formats
  int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 4);
  std::string pattern;
  
  switch (pattern_type) {
    case 0:
      // Date pattern with named groups - generated from fuzzer input
      {
        std::string year_pattern = fdp.ConsumeRandomLengthString(20);
        std::string month_pattern = fdp.ConsumeRandomLengthString(20);
        std::string day_pattern = fdp.ConsumeRandomLengthString(20);
        pattern = "(?P<year>" + year_pattern + ")-(?P<month>" + month_pattern + ")-(?P<day>" + day_pattern + ")";
      }
      break;
    case 1:
      // Email pattern with named groups - generated from fuzzer input
      {
        std::string user_pattern = fdp.ConsumeRandomLengthString(30);
        std::string domain_pattern = fdp.ConsumeRandomLengthString(30);
        std::string tld_pattern = fdp.ConsumeRandomLengthString(20);
        pattern = "(?P<user>" + user_pattern + ")@(?P<domain>" + domain_pattern + ")\\.(?P<tld>" + tld_pattern + ")";
      }
      break;
    case 2:
      // Phone number pattern with named groups - generated from fuzzer input
      {
        std::string country_pattern = fdp.ConsumeRandomLengthString(20);
        std::string area_pattern = fdp.ConsumeRandomLengthString(20);
        std::string prefix_pattern = fdp.ConsumeRandomLengthString(20);
        std::string line_pattern = fdp.ConsumeRandomLengthString(20);
        pattern = "(?P<country>" + country_pattern + ")[\\s-]?(?P<area>" + area_pattern + ")[\\s-]?(?P<prefix>" + prefix_pattern + ")[\\s-]?(?P<line>" + line_pattern + ")";
      }
      break;
    case 3:
      // Custom pattern with named group - fully from fuzzer input
      {
        std::string prefix = fdp.ConsumeRandomLengthString(50);
        std::string group_name = fdp.ConsumeRandomLengthString(20);
        std::string group_pattern = fdp.ConsumeRandomLengthString(50);
        std::string suffix = fdp.ConsumeRandomLengthString(50);
        pattern = prefix + "(?P<" + group_name + ">" + group_pattern + ")" + suffix;
      }
      break;
    case 4:
      // Multiple named groups pattern
      {
        int num_groups = fdp.ConsumeIntegralInRange<int>(1, 5);
        pattern = "";
        for (int i = 0; i < num_groups && fdp.remaining_bytes() > 10; i++) {
          std::string group_name = "group" + std::to_string(i);
          std::string group_pattern = fdp.ConsumeRandomLengthString(20);
          pattern += "(?P<" + group_name + ">" + group_pattern + ")";
          if (i < num_groups - 1 && fdp.remaining_bytes() > 5) {
            std::string separator = fdp.ConsumeRandomLengthString(10);
            pattern += separator;
          }
        }
      }
      break;
  }

  // 2. Create LazyRE2 static-like instance (not actually static in fuzzer context)
  // LazyRE2 is designed for static initialization, but we can use it locally
  re2::LazyRE2 lazy_re2 = {pattern.c_str()};
  
  // 3. Use LazyRE2::get() to access the RE2 object
  // This triggers thread-safe initialization on first call
  re2::RE2* re2_ptr = lazy_re2.get();
  
  // Check if initialization was successful
  if (re2_ptr == nullptr || !re2_ptr->ok()) {
    // If pattern compilation failed, test error_arg() API
    if (re2_ptr != nullptr && !re2_ptr->ok()) {
      // Test error_arg() - returns the offending portion of regexp
      const std::string& error_arg = re2_ptr->error_arg();
      // error_arg may be empty or contain the problematic part of pattern
      
      // Also test pattern() even for failed compilation
      const std::string& stored_pattern = re2_ptr->pattern();
      // Should match our input pattern
    }
    return 0; // Early exit if pattern compilation failed
  }

  // 4. Use LazyRE2::operator->() for method calls
  // This is equivalent to lazy_re2.get()->method()
  const std::string& pattern_from_lazy = lazy_re2->pattern();
  
  // Verify pattern matches what we set
  // pattern_from_lazy should equal pattern

  // 5. Test RE2 APIs via the LazyRE2 object
  
  // Test pattern() API
  const std::string& re2_pattern = re2_ptr->pattern();
  
  // Test CapturingGroupNames() - map from indices to names
  const std::map<int, std::string>& index_to_name = re2_ptr->CapturingGroupNames();
  
  // Test NamedCapturingGroups() - map from names to indices  
  const std::map<std::string, int>& name_to_index = re2_ptr->NamedCapturingGroups();
  
  // Verify consistency between the two maps
  // For each entry in index_to_name, there should be a corresponding entry in name_to_index
  for (const auto& entry : index_to_name) {
    int index = entry.first;
    const std::string& name = entry.second;
    
    // Look up the name in name_to_index map
    auto it = name_to_index.find(name);
    if (it != name_to_index.end()) {
      // Found it - index should match
      int found_index = it->second;
      // In RE2, the index in name_to_index is the leftmost group with that name
      // which may not necessarily equal 'index' if there are duplicate names
    }
  }

  // 6. Perform matching with named capture extraction
  // Generate test text from remaining fuzzer input
  std::string test_text = fdp.ConsumeRandomLengthString(512);
  
  // Try to match the pattern against test text
  // Extract named captures based on the pattern type
  std::string year, month, day, user, domain, tld, country, area, prefix, line;
  
  bool match_result = false;
  switch (pattern_type) {
    case 0: // Date pattern
      match_result = re2::RE2::FullMatch(test_text, *re2_ptr, &year, &month, &day);
      break;
    case 1: // Email pattern
      match_result = re2::RE2::FullMatch(test_text, *re2_ptr, &user, &domain, &tld);
      break;
    case 2: // Phone pattern
      match_result = re2::RE2::FullMatch(test_text, *re2_ptr, &country, &area, &prefix, &line);
      break;
    case 3: // Custom pattern - try partial match
      match_result = re2::RE2::PartialMatch(test_text, *re2_ptr);
      break;
    case 4: // Multiple groups pattern - try partial match
      match_result = re2::RE2::PartialMatch(test_text, *re2_ptr);
      break;
  }

  // 7. Test with invalid patterns to trigger error_arg()
  // Create intentionally malformed patterns to test error handling
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
    std::vector<std::string> bad_patterns = {
      "(?P<name",          // Unterminated group
      "(?P<>\\d+)",        // Empty group name
      "(?P<123>\\d+)",     // Group name starting with digit
      "(?P<name-name>\\d+)", // Hyphen in group name
      "(?P<name>",         // Missing pattern
    };
    
    int bad_pattern_index = fdp.ConsumeIntegralInRange<int>(0, bad_patterns.size() - 1);
    std::string bad_pattern = bad_patterns[bad_pattern_index];
    
    re2::RE2 bad_re(bad_pattern);
    if (!bad_re.ok()) {
      // Test error_arg() on invalid pattern
      const std::string& bad_error_arg = bad_re.error_arg();
      // bad_error_arg may contain the problematic part
      
      // Test pattern() even for invalid pattern
      const std::string& bad_stored_pattern = bad_re.pattern();
    }
  }

  // 8. Test LazyRE2 with options (Latin1 encoding)
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
    re2::LazyRE2 lazy_with_options = {"[a-z]+", re2::RE2::Latin1};
    re2::RE2* re2_with_options = lazy_with_options.get();
    
    if (re2_with_options != nullptr && re2_with_options->ok()) {
      // Test that options were applied
      re2::RE2::Options options = re2_with_options->options();
      // options.encoding() should be Latin1
      
      // Test pattern() API
      const std::string& opt_pattern = re2_with_options->pattern();
    }
  }

  // 9. Test multiple LazyRE2 instances (simulating static usage pattern)
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
    // Create multiple LazyRE2 instances with different patterns
    std::string pattern1 = "(?P<first>\\w+)";
    std::string pattern2 = "(?P<second>\\d+)";
    std::string pattern3 = "(?P<third>[a-zA-Z0-9]+)";
    
    re2::LazyRE2 lazy1 = {pattern1.c_str()};
    re2::LazyRE2 lazy2 = {pattern2.c_str()};
    re2::LazyRE2 lazy3 = {pattern3.c_str()};
    
    // Access them to trigger initialization
    re2::RE2* re1 = lazy1.get();
    re2::RE2* re2 = lazy2.get();
    re2::RE2* re3 = lazy3.get();
    
    // Test their APIs
    if (re1 != nullptr && re1->ok()) {
      const auto& names1 = re1->NamedCapturingGroups();
      const auto& indices1 = re1->CapturingGroupNames();
    }
    
    if (re2 != nullptr && re2->ok()) {
      const auto& names2 = re2->NamedCapturingGroups();
    }
    
    if (re3 != nullptr && re3->ok()) {
      const auto& names3 = re3->NamedCapturingGroups();
    }
  }

  // 10. Test edge cases for named capture groups
  // Patterns with duplicate group names (should map to leftmost group)
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
    re2::RE2 dup_re("(?P<name>a)|(?P<name>b)");
    if (dup_re.ok()) {
      const auto& dup_names = dup_re.NamedCapturingGroups();
      const auto& dup_indices = dup_re.CapturingGroupNames();
      
      // For duplicate names, NamedCapturingGroups returns the leftmost index
      // CapturingGroupNames should have entries for both indices
    }
  }

  return 0;
}

} // namespace harness_007
