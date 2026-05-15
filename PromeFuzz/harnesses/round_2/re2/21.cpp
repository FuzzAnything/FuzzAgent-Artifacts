// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create input string from fuzzer data
    std::string input(reinterpret_cast<const char*>(Data), Size);
    absl::string_view text(input);

    // Try different patterns to exercise various regex states
    const char* patterns[] = {
        "(\\d+)",
        "(0x[0-9a-fA-F]+)",
        "(0[0-7]+)",
        "([0-9]+).*([a-z]+)",
        ".*"
    };

    for (const char* pattern : patterns) {
        re2::RE2::Options options;
        re2::RE2 re(pattern, options);
        
        if (!re.ok()) {
            continue; // Skip invalid patterns
        }

        // Test with different numeric types
        {
            int int_val = 0;
            long long_val = 0;
            unsigned int uint_val = 0;
            unsigned long ulong_val = 0;

            // Create Arg objects using different methods
            re2::RE2::Arg args[] = {
                re2::RE2::CRadix(&int_val),
                re2::RE2::Hex(&long_val),
                re2::RE2::Octal(&uint_val),
                re2::RE2::CRadix(&ulong_val)
            };

            const re2::RE2::Arg* arg_ptrs[] = {&args[0], &args[1], &args[2], &args[3]};

            // Test FullMatchN
            re2::RE2::FullMatchN(text, re, arg_ptrs, 4);

            // Test PartialMatchN
            re2::RE2::PartialMatchN(text, re, arg_ptrs, 4);

            // Test FindAndConsumeN
            absl::string_view consume_text = text;
            re2::RE2::FindAndConsumeN(&consume_text, re, arg_ptrs, 4);
        }

        // Test with fewer arguments
        {
            int val1 = 0, val2 = 0;
            re2::RE2::Arg args[] = {
                re2::RE2::CRadix(&val1),
                re2::RE2::Hex(&val2)
            };
            const re2::RE2::Arg* arg_ptrs[] = {&args[0], &args[1]};

            re2::RE2::FullMatchN(text, re, arg_ptrs, 2);
            re2::RE2::PartialMatchN(text, re, arg_ptrs, 2);
            
            absl::string_view consume_text = text;
            re2::RE2::FindAndConsumeN(&consume_text, re, arg_ptrs, 2);
        }

        // Test with single argument
        {
            int val = 0;
            re2::RE2::Arg arg = re2::RE2::Octal(&val);
            const re2::RE2::Arg* arg_ptrs[] = {&arg};

            re2::RE2::FullMatchN(text, re, arg_ptrs, 1);
            re2::RE2::PartialMatchN(text, re, arg_ptrs, 1);
            
            absl::string_view consume_text = text;
            re2::RE2::FindAndConsumeN(&consume_text, re, arg_ptrs, 1);
        }

        // Test with no arguments (just pattern matching)
        re2::RE2::FullMatchN(text, re, nullptr, 0);
        re2::RE2::PartialMatchN(text, re, nullptr, 0);
        
        absl::string_view consume_text = text;
        re2::RE2::FindAndConsumeN(&consume_text, re, nullptr, 0);
    }

    return 0;
}