// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
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
#include <string>
#include <optional>

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t pattern_len = Size / 2;
    size_t text_len = Size - pattern_len;
    
    // Ensure null termination for pattern
    std::string pattern_str(reinterpret_cast<const char*>(Data), pattern_len);
    std::string text_str(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    
    // Create RE2 object from pattern
    re2::RE2 re(pattern_str);
    
    // Variables for capturing matches
    std::string captured_str;
    int captured_int = 0;
    std::optional<int> captured_opt_int;
    absl::string_view captured_view;
    
    // 1. Test FullMatch
    bool full_match_result = re2::RE2::FullMatch(
        text_str, 
        re, 
        &captured_str, 
        &captured_int, 
        &captured_opt_int,
        static_cast<void*>(nullptr)
    );
    
    // 2. Test PartialMatch
    bool partial_match_result = re2::RE2::PartialMatch(
        text_str,
        re,
        &captured_view,
        static_cast<void*>(nullptr),
        &captured_int
    );
    
    // 3. Test Consume
    absl::string_view consume_input = text_str;
    bool consume_result = re2::RE2::Consume(
        &consume_input,
        re,
        &captured_str,
        &captured_int
    );
    
    // 4. Test FindAndConsume
    absl::string_view find_consume_input = text_str;
    bool find_consume_result = re2::RE2::FindAndConsume(
        &find_consume_input,
        re,
        &captured_str,
        &captured_view,
        &captured_opt_int
    );
    
    // Also test with different argument combinations
    re2::RE2::FullMatch(text_str, re, static_cast<void*>(nullptr));
    re2::RE2::PartialMatch(text_str, re, &captured_str);
    
    absl::string_view empty_input = "";
    re2::RE2::Consume(&empty_input, re, static_cast<void*>(nullptr));
    re2::RE2::FindAndConsume(&empty_input, re, static_cast<void*>(nullptr));
    
    // Test with different text types
    const char* text_cstr = text_str.c_str();
    re2::RE2::FullMatch(text_cstr, re, &captured_int);
    re2::RE2::PartialMatch(text_cstr, re, &captured_str);
    
    // Test with different RE2 construction
    const char* pattern_cstr = pattern_str.c_str();
    re2::RE2 re2(pattern_cstr);
    re2::RE2::FullMatch(text_str, re2, &captured_str);
    
    // Clean up - RAII handles most cleanup
    (void)full_match_result;
    (void)partial_match_result;
    (void)consume_result;
    (void)find_consume_result;
    
    return 0;
}