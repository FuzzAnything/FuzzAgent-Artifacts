// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "re2/re2.h"
#include <cstdint>
#include <string>
#include <optional>
#include <cstring>

static std::string create_pattern_from_data(const uint8_t *Data, size_t Size) {
    if (Size == 0) return ".";
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

static std::string create_text_from_data(const uint8_t *Data, size_t Size) {
    if (Size == 0) return "";
    return std::string(reinterpret_cast<const char*>(Data), Size);
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into pattern and text parts
    size_t pattern_size = Size / 2;
    size_t text_size = Size - pattern_size;
    
    std::string pattern = create_pattern_from_data(Data, pattern_size);
    std::string text = create_text_from_data(Data + pattern_size, text_size);
    
    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    
    // Variables for capturing matches
    std::string captured_str;
    absl::string_view captured_view;
    int captured_int = 0;
    std::optional<int> captured_opt;
    
    // 1. Test FullMatch
    bool full_match_result = re2::RE2::FullMatch(text, re, &captured_str);
    bool full_match_result2 = re2::RE2::FullMatch(text, re, &captured_view);
    bool full_match_result3 = re2::RE2::FullMatch(text, re, &captured_int);
    bool full_match_result4 = re2::RE2::FullMatch(text, re, &captured_opt);
    bool full_match_result5 = re2::RE2::FullMatch(text, re, nullptr);
    
    // 2. Test PartialMatch
    bool partial_match_result = re2::RE2::PartialMatch(text, re, &captured_str);
    bool partial_match_result2 = re2::RE2::PartialMatch(text, re, &captured_view);
    bool partial_match_result3 = re2::RE2::PartialMatch(text, re, &captured_int);
    bool partial_match_result4 = re2::RE2::PartialMatch(text, re, &captured_opt);
    bool partial_match_result5 = re2::RE2::PartialMatch(text, re, nullptr);
    
    // 3. Test Consume
    absl::string_view consume_input = text;
    bool consume_result = re2::RE2::Consume(&consume_input, re, &captured_str);
    
    absl::string_view consume_input2 = text;
    bool consume_result2 = re2::RE2::Consume(&consume_input2, re, &captured_view);
    
    absl::string_view consume_input3 = text;
    bool consume_result3 = re2::RE2::Consume(&consume_input3, re, &captured_int);
    
    absl::string_view consume_input4 = text;
    bool consume_result4 = re2::RE2::Consume(&consume_input4, re, nullptr);
    
    // 4. Test FindAndConsume
    absl::string_view find_input = text;
    bool find_result = re2::RE2::FindAndConsume(&find_input, re, &captured_str);
    
    absl::string_view find_input2 = text;
    bool find_result2 = re2::RE2::FindAndConsume(&find_input2, re, &captured_view);
    
    absl::string_view find_input3 = text;
    bool find_result3 = re2::RE2::FindAndConsume(&find_input3, re, &captured_int);
    
    absl::string_view find_input4 = text;
    bool find_result4 = re2::RE2::FindAndConsume(&find_input4, re, nullptr);
    
    // Try multiple iterations of FindAndConsume to explore more states
    absl::string_view find_input5 = text;
    int iterations = 0;
    while (re2::RE2::FindAndConsume(&find_input5, re, &captured_str) && iterations < 10) {
        iterations++;
    }
    
    // Test with different text variations
    std::string text_variant = text + " additional text";
    bool variant_result = re2::RE2::PartialMatch(text_variant, re, &captured_str);
    
    // Test with empty text
    bool empty_result = re2::RE2::PartialMatch("", re, nullptr);
    
    // Test with pattern that might have special characters
    std::string special_pattern = pattern + ".*";
    re2::RE2 re_special(special_pattern);
    bool special_result = re2::RE2::PartialMatch(text, re_special, &captured_str);
    
    // Use the results to avoid optimization
    (void)full_match_result;
    (void)full_match_result2;
    (void)full_match_result3;
    (void)full_match_result4;
    (void)full_match_result5;
    (void)partial_match_result;
    (void)partial_match_result2;
    (void)partial_match_result3;
    (void)partial_match_result4;
    (void)partial_match_result5;
    (void)consume_result;
    (void)consume_result2;
    (void)consume_result3;
    (void)consume_result4;
    (void)find_result;
    (void)find_result2;
    (void)find_result3;
    (void)find_result4;
    (void)variant_result;
    (void)empty_result;
    (void)special_result;
    
    return 0;
}