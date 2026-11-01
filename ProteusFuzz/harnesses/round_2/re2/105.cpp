// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::RE2::Set::Add at set.cc:64:15 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Compile at set.cc:103:16 in set.h
// re2::RE2::Set::Size at set.cc:58:15 in set.h
// re2::RE2::Set::Match at set.cc:133:16 in set.h
// re2::RE2::Set::Match at set.cc:137:16 in set.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include "filtered_re2.h"
#include "set.h"
#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Split input into patterns and test text
  // First byte determines split point ratio
  size_t split_point = Size;
  if (Size > 1) {
    split_point = (Data[0] % (Size - 1)) + 1;
  }

  std::vector<std::string> patterns;
  std::string test_text(reinterpret_cast<const char*>(Data + split_point), Size - split_point);

  // Parse patterns from the first part of the input
  std::string pattern_buffer(reinterpret_cast<const char*>(Data), split_point);
  size_t start = 0;
  while (start < pattern_buffer.size()) {
    size_t end = pattern_buffer.find('\n', start);
    if (end == std::string::npos) {
      end = pattern_buffer.size();
    }
    
    std::string pattern = pattern_buffer.substr(start, end - start);
    // Trim null bytes which might appear in fuzz input
    pattern.erase(std::remove(pattern.begin(), pattern.end(), '\0'), pattern.end());
    
    if (!pattern.empty()) {
      patterns.push_back(pattern);
    }
    
    start = end + 1;
    if (end >= pattern_buffer.size()) break;
  }

  // Ensure at least one pattern if possible, or skip if empty
  if (patterns.empty() && split_point > 0) {
    std::string pattern = pattern_buffer;
    pattern.erase(std::remove(pattern.begin(), pattern.end(), '\0'), pattern.end());
    if (!pattern.empty()) {
      patterns.push_back(pattern);
    }
  }

  if (patterns.empty()) return 0;

  // Test RE2::Set
  {
    re2::RE2::Options options;
    re2::RE2::Anchor anchor = re2::RE2::ANCHOR_BOTH;
    re2::RE2::Set set(options, anchor);
    std::string error;
    std::vector<int> matches;
    re2::RE2::Set::ErrorInfo error_info;

    // Add patterns
    for (const auto& pat : patterns) {
      set.Add(absl::string_view(pat), &error);
    }

    // Check size before compile
    int size_before = set.Size();
    (void)size_before;

    // Compile
    bool compile_result = set.Compile();
    (void)compile_result;

    // Check size after compile
    int size_after = set.Size();
    (void)size_after;

    // Match with first overload
    bool match1 = set.Match(absl::string_view(test_text), &matches);
    (void)match1;

    // Match with second overload
    bool match2 = set.Match(absl::string_view(test_text), &matches, &error_info);
    (void)match2;
    (void)error_info;
  }

  // Test FilteredRE2
  {
    int min_atom_len = 1;
    if (Size > 0) {
      min_atom_len = (Data[0] % 5) + 1; // 1 to 5
    }

    re2::FilteredRE2 filtered(min_atom_len);
    re2::RE2::Options options;
    std::string error;
    std::vector<std::string> strings_to_match;
    int id;

    // Add patterns to filtered set
    for (const auto& pat : patterns) {
      filtered.Add(absl::string_view(pat), options, &id);
      (void)id;
      (void)error;
    }

    // Compile filtered set
    filtered.Compile(&strings_to_match);
    (void)strings_to_match;
  }

  return 0;
}