// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
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
#include "re2.h"
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

static std::string extract_string_from_fuzz_data(const uint8_t *Data, size_t Size, size_t offset) {
  if (offset >= Size) return "";
  size_t len = 0;
  while (offset + len < Size && Data[offset + len] != '\0') {
    len++;
  }
  // Limit string size to prevent excessive memory usage
  if (len > 1024) len = 1024;
  return std::string(reinterpret_cast<const char*>(Data + offset), len);
}

static bool extract_bool_from_fuzz_data(const uint8_t *Data, size_t Size, size_t offset) {
  if (offset >= Size) return false;
  return Data[offset] % 2 == 0;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Extract potential pattern strings from fuzz data
  std::string pattern1 = extract_string_from_fuzz_data(Data, Size, 0);
  std::string pattern2 = extract_string_from_fuzz_data(Data, Size, 100);
  std::string pattern3 = extract_string_from_fuzz_data(Data, Size, 200);

  // Extract boolean flags
  bool case_sensitive = extract_bool_from_fuzz_data(Data, Size, 300);
  bool use_latin1 = extract_bool_from_fuzz_data(Data, Size, 301);
  bool use_posix = extract_bool_from_fuzz_data(Data, Size, 302);

  // Test 1: RE2::Options construction and manipulation
  {
    re2::RE2::Options opts(re2::RE2::DefaultOptions);
    
    // Test set_case_sensitive
    opts.set_case_sensitive(case_sensitive);
    
    // Test set_encoding
    re2::RE2::Options::Encoding enc = use_latin1 ? re2::RE2::Options::EncodingLatin1 : re2::RE2::Options::EncodingUTF8;
    opts.set_encoding(enc);
    
    // Test ParseFlags
    int flags = opts.ParseFlags();
    (void)flags; // Suppress unused variable warning
    
    // Test Copy
    re2::RE2::Options opts2(re2::RE2::DefaultOptions);
    opts2.Copy(opts);
  }

  // Test 2: RE2 construction and options retrieval
  {
    re2::RE2::Options opts(re2::RE2::DefaultOptions);
    opts.set_case_sensitive(case_sensitive);
    re2::RE2::Options::Encoding enc = use_latin1 ? re2::RE2::Options::EncodingLatin1 : re2::RE2::Options::EncodingUTF8;
    opts.set_encoding(enc);
    
    re2::RE2 re(pattern1, opts);
    
    // Test options() method
    const re2::RE2::Options& retrieved_opts = re.options();
    (void)retrieved_opts; // Suppress unused variable warning
  }

  // Test 3: FilteredRE2::Add
  {
    re2::FilteredRE2 filtered;
    int id1, id2, id3;
    
    re2::RE2::Options opts(re2::RE2::DefaultOptions);
    opts.set_case_sensitive(case_sensitive);
    re2::RE2::Options::Encoding enc = use_latin1 ? re2::RE2::Options::EncodingLatin1 : re2::RE2::Options::EncodingUTF8;
    opts.set_encoding(enc);
    
    // Add patterns with different options
    re2::RE2::ErrorCode err1 = filtered.Add(pattern1, opts, &id1);
    (void)err1;
    
    re2::RE2::Options opts2(re2::RE2::DefaultOptions);
    opts2.set_case_sensitive(!case_sensitive);
    re2::RE2::ErrorCode err2 = filtered.Add(pattern2, opts2, &id2);
    (void)err2;
    
    re2::RE2::Options opts3(re2::RE2::DefaultOptions);
    opts3.set_encoding(use_posix ? re2::RE2::Options::EncodingLatin1 : re2::RE2::Options::EncodingUTF8);
    re2::RE2::ErrorCode err3 = filtered.Add(pattern3, opts3, &id3);
    (void)err3;
  }

  // Test 4: Edge cases with empty and malformed patterns
  {
    re2::RE2::Options opts(re2::RE2::DefaultOptions);
    opts.set_case_sensitive(true);
    
    // Empty pattern
    re2::RE2 re_empty("", opts);
    const re2::RE2::Options& opts_empty = re_empty.options();
    (void)opts_empty;
    
    // Malformed pattern
    re2::RE2 re_malformed("[unclosed", opts);
    const re2::RE2::Options& opts_malformed = re_malformed.options();
    (void)opts_malformed;
    
    // Very long pattern (truncated by extract_string_from_fuzz_data)
    re2::RE2 re_long(pattern1, opts);
    const re2::RE2::Options& opts_long = re_long.options();
    (void)opts_long;
  }

  // Test 5: Different canned options
  {
    re2::RE2::Options opts_latin1(re2::RE2::Latin1);
    re2::RE2::Options opts_posix(re2::RE2::POSIX);
    re2::RE2::Options opts_quiet(re2::RE2::Quiet);
    
    opts_latin1.set_case_sensitive(case_sensitive);
    opts_posix.set_case_sensitive(!case_sensitive);
    opts_quiet.set_case_sensitive(use_latin1);
    
    re2::RE2 re1(pattern1, opts_latin1);
    re2::RE2 re2_obj(pattern2, opts_posix);
    re2::RE2 re3(pattern3, opts_quiet);
    
    (void)re1.options();
    (void)re2_obj.options();
    (void)re3.options();
  }

  return 0;
}