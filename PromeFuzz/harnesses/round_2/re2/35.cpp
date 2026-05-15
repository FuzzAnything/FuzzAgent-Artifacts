// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Arg::Parse at re2.h:920:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
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

static bool ParseInt(const char* str, size_t n, int* out) {
    if (n == 0) return false;
    char* end;
    long val = strtol(str, &end, 10);
    if (end != str + n) return false;
    *out = static_cast<int>(val);
    return true;
}

static bool ParseDouble(const char* str, size_t n, double* out) {
    if (n == 0) return false;
    char* end;
    *out = strtod(str, &end);
    return end == str + n;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into pattern and text parts
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    absl::string_view pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    absl::string_view text(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                          Size - 1 - pattern_len);

    // Try to create RE2 object with the pattern
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;

    // Test PartialMatch with various capture types
    {
        std::string cap1;
        absl::string_view cap2;
        int cap3 = 0;
        double cap4 = 0.0;
        std::optional<int> cap5;
        
        re2::RE2::PartialMatch(text, re, &cap1);
        re2::RE2::PartialMatch(text, re, &cap2);
        re2::RE2::PartialMatch(text, re, &cap3);
        re2::RE2::PartialMatch(text, re, &cap4);
        re2::RE2::PartialMatch(text, re, &cap5);
        re2::RE2::PartialMatch(text, re, nullptr);
        re2::RE2::PartialMatch(text, re, &cap1, &cap2);
        re2::RE2::PartialMatch(text, re, &cap1, &cap2, &cap3);
    }

    // Test FullMatch with various capture types
    {
        std::string cap1;
        absl::string_view cap2;
        int cap3 = 0;
        double cap4 = 0.0;
        std::optional<int> cap5;
        
        re2::RE2::FullMatch(text, re, &cap1);
        re2::RE2::FullMatch(text, re, &cap2);
        re2::RE2::FullMatch(text, re, &cap3);
        re2::RE2::FullMatch(text, re, &cap4);
        re2::RE2::FullMatch(text, re, &cap5);
        re2::RE2::FullMatch(text, re, nullptr);
        re2::RE2::FullMatch(text, re, &cap1, &cap2);
        re2::RE2::FullMatch(text, re, &cap1, &cap2, &cap3);
    }

    // Test Match with different anchors and submatch counts
    {
        // Use a reasonable number of submatches (0-2) to avoid issues
        int nsubmatch = (Data[0] % 3); // 0-2 submatches
        std::vector<absl::string_view> submatches;
        
        if (nsubmatch > 0) {
            submatches.resize(nsubmatch);
        }
        
        // Only call Match if we have valid text and submatches array
        if (!text.empty() && nsubmatch >= 0) {
            // Use UNANCHORED only to avoid complex DFA states
            re.Match(text, 0, text.size(), re2::RE2::UNANCHORED, 
                    nsubmatch > 0 ? submatches.data() : nullptr, nsubmatch);
        }
    }

    // Test FindAndConsume
    {
        absl::string_view input = text;
        std::string cap1;
        absl::string_view cap2;
        int cap3 = 0;
        
        // Reset input each time
        input = text;
        re2::RE2::FindAndConsume(&input, re, &cap1);
        
        input = text;
        re2::RE2::FindAndConsume(&input, re, &cap2);
        
        input = text;
        re2::RE2::FindAndConsume(&input, re, &cap3);
        
        input = text;
        re2::RE2::FindAndConsume(&input, re, &cap1, &cap2);
        
        input = text;
        re2::RE2::FindAndConsume(&input, re, nullptr);
    }

    // Test RE2::Arg::Parse directly
    {
        // Create parser functions that match the expected signature
        static auto int_parser = [](const char* str, size_t n, void* dest) -> bool {
            return ParseInt(str, n, static_cast<int*>(dest));
        };
        
        static auto double_parser = [](const char* str, size_t n, void* dest) -> bool {
            return ParseDouble(str, n, static_cast<double*>(dest));
        };
        
        int int_val = 0;
        double double_val = 0.0;
        
        // Create Arg objects with the parser functions
        re2::RE2::Arg int_arg(&int_val, int_parser);
        re2::RE2::Arg double_arg(&double_val, double_parser);
        
        // Test with substrings of the text
        if (!text.empty()) {
            // Limit the number of iterations to avoid excessive processing
            size_t max_iterations = std::min<size_t>(text.size(), 10);
            for (size_t i = 0; i < max_iterations; ++i) {
                if (i < text.size()) {
                    size_t len = std::min<size_t>((Data[i % Size] % (text.size() - i)) + 1, text.size() - i);
                    if (len > 0) {
                        int_arg.Parse(text.data() + i, len);
                        double_arg.Parse(text.data() + i, len);
                    }
                }
            }
        }
    }

    // Test Consume
    {
        absl::string_view input = text;
        std::string cap1;
        absl::string_view cap2;
        int cap3 = 0;
        
        input = text;
        re2::RE2::Consume(&input, re, &cap1);
        
        input = text;
        re2::RE2::Consume(&input, re, &cap2);
        
        input = text;
        re2::RE2::Consume(&input, re, &cap3);
        
        input = text;
        re2::RE2::Consume(&input, re, &cap1, &cap2);
        
        input = text;
        re2::RE2::Consume(&input, re, nullptr);
    }

    return 0;
}