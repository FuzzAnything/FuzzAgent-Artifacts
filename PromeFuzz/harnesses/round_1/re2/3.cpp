// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
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
#include <cstring>
#include <string>
#include <optional>

struct CustomType {
    int value;
    bool ParseFrom(const char* data, size_t size) {
        if (size == 0) return false;
        value = static_cast<int>(data[0]);
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split input: first byte for pattern, rest for text
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len >= Size) pattern_len = Size - 1;
    
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                     Size - 1 - pattern_len);
    
    // Create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;
    
    // Prepare capture variables
    int captured_int = 0;
    std::string captured_str;
    absl::string_view captured_view;
    std::optional<int> captured_opt;
    CustomType captured_custom;
    
    // 1. FullMatch
    re2::RE2::FullMatch(text, re, 
                       &captured_int,
                       &captured_str,
                       &captured_view,
                       &captured_opt,
                       &captured_custom,
                       (void*)nullptr);
    
    // 2. PartialMatch
    re2::RE2::PartialMatch(text, re,
                          &captured_int,
                          &captured_str,
                          &captured_view);
    
    // 3. Consume
    absl::string_view consume_input(text);
    re2::RE2::Consume(&consume_input, re,
                     &captured_int,
                     &captured_str);
    
    // 4. FindAndConsume
    absl::string_view find_consume_input(text);
    re2::RE2::FindAndConsume(&find_consume_input, re,
                            &captured_int,
                            &captured_str);
    
    // Additional test with different argument combinations
    // Test with only string capture
    re2::RE2::FullMatch(text, re, &captured_str);
    
    // Test with only optional
    re2::RE2::PartialMatch(text, re, &captured_opt);
    
    // Test with no captures
    absl::string_view consume_input2(text);
    re2::RE2::Consume(&consume_input2, re);
    
    // Test with custom type
    absl::string_view find_consume_input2(text);
    re2::RE2::FindAndConsume(&find_consume_input2, re, &captured_custom);
    
    return 0;
}