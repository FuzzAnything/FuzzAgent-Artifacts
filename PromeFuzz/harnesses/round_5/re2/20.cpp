// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
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
#include <cstddef>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    // Use first byte as separator index
    size_t separator = Data[0] % Size;
    if (separator == 0) {
        separator = 1; // Ensure at least 1 byte for pattern
    }

    std::string pattern(reinterpret_cast<const char*>(Data + 1), separator - 1);
    std::string text(reinterpret_cast<const char*>(Data + separator), Size - separator);

    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    
    // Test NumberOfCapturingGroups
    int num_captures = re.NumberOfCapturingGroups();

    // Test FullMatch with different argument counts
    {
        absl::string_view text_view(text);
        switch (num_captures) {
            case 0:
                re2::RE2::FullMatch(text_view, re);
                break;
            case 1: {
                std::string capture1;
                re2::RE2::FullMatch(text_view, re, &capture1);
                break;
            }
            case 2: {
                std::string capture1, capture2;
                re2::RE2::FullMatch(text_view, re, &capture1, &capture2);
                break;
            }
            default: {
                std::string capture1, capture2, capture3;
                re2::RE2::FullMatch(text_view, re, &capture1, &capture2, &capture3);
                break;
            }
        }
    }

    // Test FullMatchN
    {
        absl::string_view text_view(text);
        int n = num_captures;
        if (n > 3) n = 3;
        if (n < 0) n = 0;
        
        std::vector<re2::RE2::Arg> args(n);
        std::vector<const re2::RE2::Arg*> arg_ptrs(n);
        for (int i = 0; i < n; ++i) {
            arg_ptrs[i] = &args[i];
        }
        re2::RE2::FullMatchN(text_view, re, arg_ptrs.data(), n);
    }

    // Test Consume
    {
        absl::string_view input(text);
        switch (num_captures) {
            case 0:
                re2::RE2::Consume(&input, re);
                break;
            case 1: {
                std::string capture1;
                re2::RE2::Consume(&input, re, &capture1);
                break;
            }
            case 2: {
                std::string capture1, capture2;
                re2::RE2::Consume(&input, re, &capture1, &capture2);
                break;
            }
            default: {
                std::string capture1, capture2, capture3;
                re2::RE2::Consume(&input, re, &capture1, &capture2, &capture3);
                break;
            }
        }
    }

    // Test FindAndConsume
    {
        absl::string_view input(text);
        switch (num_captures) {
            case 0:
                re2::RE2::FindAndConsume(&input, re);
                break;
            case 1: {
                std::string capture1;
                re2::RE2::FindAndConsume(&input, re, &capture1);
                break;
            }
            case 2: {
                std::string capture1, capture2;
                re2::RE2::FindAndConsume(&input, re, &capture1, &capture2);
                break;
            }
            default: {
                std::string capture1, capture2, capture3;
                re2::RE2::FindAndConsume(&input, re, &capture1, &capture2, &capture3);
                break;
            }
        }
    }

    // Test Match with different parameters
    if (re.ok()) {
        absl::string_view text_view(text);
        int nsubmatch = num_captures + 1;
        if (nsubmatch < 0) nsubmatch = 0;
        if (nsubmatch > 10) nsubmatch = 10; // Limit to avoid excessive allocation
        
        std::vector<absl::string_view> submatch(nsubmatch);
        
        // Test with different anchors
        re.Match(text_view, 0, text_view.size(), re2::RE2::UNANCHORED, 
                 submatch.data(), nsubmatch);
        re.Match(text_view, 0, text_view.size(), re2::RE2::ANCHOR_START, 
                 submatch.data(), nsubmatch);
        re.Match(text_view, 0, text_view.size(), re2::RE2::ANCHOR_BOTH, 
                 submatch.data(), nsubmatch);
        
        // Test with different start positions
        if (text_view.size() > 0) {
            size_t startpos = Data[0] % text_view.size();
            re.Match(text_view, startpos, text_view.size(), re2::RE2::UNANCHORED, 
                     submatch.data(), nsubmatch);
        }
    }

    return 0;
}