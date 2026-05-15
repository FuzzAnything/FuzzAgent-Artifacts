// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
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

static bool ParseInt(const char* str, size_t n, void* out) {
    if (n == 0) return false;
    char* endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str + n) {
        *static_cast<int*>(out) = static_cast<int>(val);
        return true;
    }
    return false;
}

static bool ParseDouble(const char* str, size_t n, void* out) {
    if (n == 0) return false;
    char* endptr;
    double val = strtod(str, &endptr);
    if (endptr == str + n) {
        *static_cast<double*>(out) = val;
        return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into pattern and text
    size_t pattern_len = Size / 2;
    size_t text_len = Size - pattern_len;
    
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    
    // Try to create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // Variables for capturing matches
    std::string captured_str;
    absl::string_view captured_sv;
    int captured_int = 0;
    double captured_double = 0.0;
    std::optional<int> captured_opt_int;
    std::optional<std::string> captured_opt_str;
    
    // Test PartialMatch
    re2::RE2::PartialMatch(text, re, &captured_str);
    re2::RE2::PartialMatch(text, re, &captured_sv);
    re2::RE2::PartialMatch(text, re, &captured_int);
    re2::RE2::PartialMatch(text, re, &captured_double);
    re2::RE2::PartialMatch(text, re, &captured_opt_int);
    re2::RE2::PartialMatch(text, re, &captured_opt_str);
    re2::RE2::PartialMatch(text, re, nullptr);
    re2::RE2::PartialMatch(text, re, &captured_str, &captured_int);

    // Test FindAndConsume
    absl::string_view input_sv = text;
    re2::RE2::FindAndConsume(&input_sv, re, &captured_str);
    input_sv = text;
    re2::RE2::FindAndConsume(&input_sv, re, &captured_int);
    input_sv = text;
    re2::RE2::FindAndConsume(&input_sv, re, &captured_str, &captured_int);

    // Test Match
    absl::string_view submatch[4];
    re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, submatch, 4);
    re.Match(text, 0, text.size(), re2::RE2::ANCHOR_START, submatch, 2);
    re.Match(text, 0, text.size(), re2::RE2::ANCHOR_BOTH, submatch, 1);
    re.Match(text, 1, text.size() - 1, re2::RE2::UNANCHORED, submatch, 0);

    // Test Arg::Parse using the correct constructor signature
    re2::RE2::Arg int_arg(&captured_int, ParseInt);
    re2::RE2::Arg double_arg(&captured_double, ParseDouble);
    
    const char* test_str = "123";
    int_arg.Parse(test_str, 3);
    double_arg.Parse(test_str, 3);

    // Test Consume
    input_sv = text;
    re2::RE2::Consume(&input_sv, re, &captured_str);
    input_sv = text;
    re2::RE2::Consume(&input_sv, re, &captured_int);
    input_sv = text;
    re2::RE2::Consume(&input_sv, re, &captured_str, &captured_int);

    // Test FullMatch
    re2::RE2::FullMatch(text, re, &captured_str);
    re2::RE2::FullMatch(text, re, &captured_sv);
    re2::RE2::FullMatch(text, re, &captured_int);
    re2::RE2::FullMatch(text, re, &captured_double);
    re2::RE2::FullMatch(text, re, &captured_opt_int);
    re2::RE2::FullMatch(text, re, &captured_opt_str);
    re2::RE2::FullMatch(text, re, nullptr);
    re2::RE2::FullMatch(text, re, &captured_str, &captured_int);

    return 0;
}