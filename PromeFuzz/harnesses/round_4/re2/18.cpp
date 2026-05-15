// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
// re2::FilteredRE2::GetRE2 at filtered_re2.h:93:14 in filtered_re2.h
// re2::RE2::error at re2.h:316:22 in re2.h
// re2::FilteredRE2::FirstMatch at filtered_re2.cc:98:18 in filtered_re2.h
// re2::FilteredRE2::AllPotentials at filtered_re2.cc:124:19 in filtered_re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::FilteredRE2::Add at filtered_re2.cc:51:29 in filtered_re2.h
// re2::FilteredRE2::Compile at filtered_re2.cc:70:19 in filtered_re2.h
// re2::FilteredRE2::SlowFirstMatch at filtered_re2.cc:91:18 in filtered_re2.h
// re2::FilteredRE2::NumRegexps at filtered_re2.h:90:7 in filtered_re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>

#include "re2/filtered_re2.h"
#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
  if (Size < 2) {
    return 0;
  }

  // Use first byte to determine min_atom_len (1-255)
  int min_atom_len = Data[0] + 1;
  re2::FilteredRE2 filtered_re2(min_atom_len);
  
  // Use second byte to determine number of patterns to add (0-255)
  size_t num_patterns = Data[1];
  size_t data_offset = 2;
  
  std::vector<int> pattern_ids;
  
  // Add patterns using the fuzzer data
  for (size_t i = 0; i < num_patterns && data_offset < Size; i++) {
    // Extract pattern length (1 byte)
    size_t pattern_len = Data[data_offset] % 64;  // Limit to reasonable size
    data_offset++;
    
    if (data_offset + pattern_len > Size) {
      pattern_len = Size - data_offset;
    }
    
    if (pattern_len > 0) {
      std::string pattern(reinterpret_cast<const char*>(Data + data_offset), pattern_len);
      data_offset += pattern_len;
      
      int pattern_id = -1;
      re2::RE2::Options options;
      options.set_encoding(re2::RE2::Options::EncodingUTF8);
      
      filtered_re2.Add(pattern, options, &pattern_id);
      if (pattern_id >= 0) {
        pattern_ids.push_back(pattern_id);
      }
    }
  }
  
  // Call Compile() to prepare for FirstMatch - requires an output parameter
  std::vector<std::string> strings_to_match;
  filtered_re2.Compile(&strings_to_match);
  
  // Extract text for matching
  size_t text_len = 0;
  if (data_offset < Size) {
    text_len = Data[data_offset] % 256;  // Limit text length
    data_offset++;
  }
  
  if (text_len > Size - data_offset) {
    text_len = Size - data_offset;
  }
  
  absl::string_view text;
  if (text_len > 0) {
    text = absl::string_view(reinterpret_cast<const char*>(Data + data_offset), text_len);
    data_offset += text_len;
  }
  
  // Extract atoms for FirstMatch and AllPotentials
  std::vector<int> atoms;
  if (data_offset < Size) {
    size_t num_atoms = Data[data_offset] % 32;  // Limit number of atoms
    data_offset++;
    
    for (size_t i = 0; i < num_atoms && data_offset < Size; i++) {
      int atom = static_cast<int>(Data[data_offset]);
      atoms.push_back(atom);
      data_offset++;
    }
  }
  
  // Call the target API functions
  static int result;
  
  // 2. SlowFirstMatch - can be called before Compile
  result = filtered_re2.SlowFirstMatch(text);
  
  // 3. NumRegexps
  result = filtered_re2.NumRegexps();
  
  // 4. GetRE2 for valid pattern IDs
  for (int id : pattern_ids) {
    if (id >= 0 && id < filtered_re2.NumRegexps()) {
      const re2::RE2& re2_obj = filtered_re2.GetRE2(id);
      // Access some basic properties
      static std::string error_string;
      error_string = re2_obj.error();
    }
  }
  
  // 5. Add was already called above
  
  // 1. FirstMatch
  result = filtered_re2.FirstMatch(text, atoms);
  
  // 6. AllPotentials
  std::vector<int> potential_regexps;
  filtered_re2.AllPotentials(atoms, &potential_regexps);
  
  return 0;
}