// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::AllMatches at filtered_re2.cc:112:19 in filtered_re2.h
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
#include "filtered_re2.h"
#include "set.h"
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
  if (Size < 1) {
    return 0;
  }

  // Split input into chunks for patterns and text
  // Use first byte to determine split point roughly
  size_t split = Size / 2;
  if (split == 0) split = 1;
  if (split >= Size) split = Size - 1;

  std::string pattern_data(reinterpret_cast<const char*>(Data), split);
  std::string text_data(reinterpret_cast<const char*>(Data + split), Size - split);

  // Parse patterns: split by null bytes to get multiple patterns
  std::vector<std::string> patterns;
  std::string current_pattern;
  for (char c : pattern_data) {
    if (c == '\0') {
      if (!current_pattern.empty()) {
        patterns.push_back(current_pattern);
        current_pattern.clear();
      }
    } else {
      current_pattern += c;
    }
  }
  if (!current_pattern.empty()) {
    patterns.push_back(current_pattern);
  }

  // If no patterns, try to generate a simple one
  if (patterns.empty()) {
    patterns.push_back("a");
  }

  // Test RE2::Set
  {
    re2::RE2::Options options;
    // Alternate between anchors based on input
    re2::RE2::Anchor anchor = (Data[0] & 1) ? re2::RE2::ANCHOR_START : re2::RE2::ANCHOR_BOTH;
    if (Data[0] & 2) anchor = re2::RE2::UNANCHORED;

    re2::RE2::Set set(options, anchor);
    std::string error;

    // Add patterns
    for (const auto& pat : patterns) {
      set.Add(absl::string_view(pat), &error);
    }

    // Check size before compile
    int size_before = set.Size();

    // Compile
    bool compiled = set.Compile();

    // Check size after compile
    int size_after = set.Size();

    // Test Match overloads
    std::vector<int> matches;
    std::vector<int> matches_with_error;
    re2::RE2::Set::ErrorInfo error_info;

    // Overload 1: Match without error info
    if (compiled) {
      set.Match(absl::string_view(text_data), &matches);
    }

    // Overload 2: Match with error info
    if (compiled) {
      set.Match(absl::string_view(text_data), &matches_with_error, &error_info);
    }

    // Test AllMatches with FilteredRE2
    {
      re2::FilteredRE2 filtered;
      std::vector<int> atoms;
      std::vector<int> matching_regexps;

      // Add atoms (indices of patterns)
      for (int i = 0; i < size_before; ++i) {
        atoms.push_back(i);
      }

      // Call AllMatches
      filtered.AllMatches(absl::string_view(text_data), atoms, &matching_regexps);
    }
  }

  return 0;
}