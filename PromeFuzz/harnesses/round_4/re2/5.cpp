// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
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
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), Size - 1 - pattern_len);

    // Create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // Variables for capturing matches
    std::string captured_str;
    int captured_int = 0;
    std::optional<int> captured_opt;
    absl::string_view captured_view;

    // 1. FullMatch
    re2::RE2::FullMatch(text, re, &captured_str);
    re2::RE2::FullMatch(text, re, &captured_int);
    re2::RE2::FullMatch(text, re, &captured_opt);
    re2::RE2::FullMatch(text, re, &captured_view);
    re2::RE2::FullMatch(text, re, nullptr);
    re2::RE2::FullMatch(text, re, &captured_str, &captured_int);

    // 2. PartialMatch
    re2::RE2::PartialMatch(text, re, &captured_str);
    re2::RE2::PartialMatch(text, re, &captured_int);
    re2::RE2::PartialMatch(text, re, &captured_opt);
    re2::RE2::PartialMatch(text, re, &captured_view);
    re2::RE2::PartialMatch(text, re, nullptr);
    re2::RE2::PartialMatch(text, re, &captured_str, &captured_int);

    // 3. Consume
    absl::string_view consume_input = text;
    re2::RE2::Consume(&consume_input, re, &captured_str);
    
    consume_input = text;
    re2::RE2::Consume(&consume_input, re, &captured_int);
    
    consume_input = text;
    re2::RE2::Consume(&consume_input, re, &captured_opt);
    
    consume_input = text;
    re2::RE2::Consume(&consume_input, re, &captured_view);
    
    consume_input = text;
    re2::RE2::Consume(&consume_input, re, nullptr);
    
    consume_input = text;
    re2::RE2::Consume(&consume_input, re, &captured_str, &captured_int);

    // 4. FindAndConsume
    absl::string_view find_input = text;
    re2::RE2::FindAndConsume(&find_input, re, &captured_str);
    
    find_input = text;
    re2::RE2::FindAndConsume(&find_input, re, &captured_int);
    
    find_input = text;
    re2::RE2::FindAndConsume(&find_input, re, &captured_opt);
    
    find_input = text;
    re2::RE2::FindAndConsume(&find_input, re, &captured_view);
    
    find_input = text;
    re2::RE2::FindAndConsume(&find_input, re, nullptr);
    
    find_input = text;
    re2::RE2::FindAndConsume(&find_input, re, &captured_str, &captured_int);

    return 0;
}