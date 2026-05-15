// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
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
#include <cstddef>
#include <string>
#include <optional>

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t pattern_len = Data[0] % Size;
    if (pattern_len == 0) pattern_len = 1;
    
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_len);
    std::string text;
    if (Size > pattern_len) {
        text.assign(reinterpret_cast<const char*>(Data + pattern_len), Size - pattern_len);
    }

    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // Variables for capturing matches
    std::string captured_str;
    int captured_int = 0;
    std::optional<int> captured_optional;
    absl::string_view captured_view;
    // Remove bool capture as it's not supported by RE2::Arg
    // bool captured_bool = false;

    // 1. FullMatch
    re2::RE2::FullMatch(text, re, &captured_str, &captured_int, &captured_optional);
    re2::RE2::FullMatch(text, re, static_cast<void*>(nullptr));
    re2::RE2::FullMatch(text, re, &captured_view);
    // Try with just string and int
    re2::RE2::FullMatch(text, re, &captured_str, &captured_int);

    // 2. PartialMatch
    re2::RE2::PartialMatch(text, re, &captured_str, &captured_int);
    re2::RE2::PartialMatch(text, re, static_cast<void*>(nullptr));
    re2::RE2::PartialMatch(text, re, &captured_view);
    // Try with optional
    re2::RE2::PartialMatch(text, re, &captured_optional, &captured_str);

    // 3. Consume
    absl::string_view consume_input(text);
    re2::RE2::Consume(&consume_input, re, &captured_str, &captured_int);
    
    absl::string_view consume_input2(text);
    re2::RE2::Consume(&consume_input2, re, static_cast<void*>(nullptr));
    
    absl::string_view consume_input3(text);
    re2::RE2::Consume(&consume_input3, re, &captured_view);

    // 4. FindAndConsume
    absl::string_view find_input(text);
    re2::RE2::FindAndConsume(&find_input, re, &captured_str, &captured_int);
    
    absl::string_view find_input2(text);
    re2::RE2::FindAndConsume(&find_input2, re, static_cast<void*>(nullptr));
    
    absl::string_view find_input3(text);
    re2::RE2::FindAndConsume(&find_input3, re, &captured_view, &captured_optional);

    return 0;
}