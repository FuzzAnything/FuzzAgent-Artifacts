// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static std::string ToString(const uint8_t* data, size_t size) {
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    // Split input into pattern and text
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len >= Size) pattern_len = Size / 2;
    
    size_t text_len = Size - pattern_len;
    const uint8_t* pattern_data = Data;
    const uint8_t* text_data = Data + pattern_len;

    std::string pattern = ToString(pattern_data, pattern_len);
    std::string text = ToString(text_data, text_len);
    
    // Create RE2 object - may fail if pattern is invalid
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // Prepare arguments for capture groups
    std::vector<std::string> captures;
    std::vector<re2::RE2::Arg> args;
    std::vector<const re2::RE2::Arg*> arg_ptrs;
    
    // Use some bytes to determine number of capture groups to try
    int num_captures = (pattern_len > 0 ? pattern_data[0] : 1) % 5;
    
    for (int i = 0; i < num_captures; ++i) {
        captures.emplace_back();
        args.emplace_back(&captures.back());
        arg_ptrs.push_back(&args.back());
    }

    // Test FindAndConsume (template version) - handle 0 captures differently
    {
        re2::StringPiece input(text);
        if (num_captures == 0) {
            // For 0 captures, we need to avoid calling the variadic template with no arguments
            // Use FindAndConsumeN instead
            re2::RE2::FindAndConsumeN(&input, re, nullptr, 0);
        } else {
            switch (num_captures) {
                case 1: {
                    std::string cap1;
                    re2::RE2::FindAndConsume(&input, re, &cap1);
                    break;
                }
                case 2: {
                    std::string cap1, cap2;
                    re2::RE2::FindAndConsume(&input, re, &cap1, &cap2);
                    break;
                }
                case 3: {
                    std::string cap1, cap2, cap3;
                    re2::RE2::FindAndConsume(&input, re, &cap1, &cap2, &cap3);
                    break;
                }
                case 4: {
                    std::string cap1, cap2, cap3, cap4;
                    re2::RE2::FindAndConsume(&input, re, &cap1, &cap2, &cap3, &cap4);
                    break;
                }
            }
        }
    }

    // Test FullMatchN
    {
        re2::StringPiece sp_text(text);
        if (num_captures > 0) {
            re2::RE2::FullMatchN(sp_text, re, arg_ptrs.data(), num_captures);
        } else {
            // For 0 captures, use NULL pointer
            re2::RE2::FullMatchN(sp_text, re, nullptr, 0);
        }
    }

    // Test ConsumeN
    {
        re2::StringPiece input(text);
        if (num_captures > 0) {
            re2::RE2::ConsumeN(&input, re, arg_ptrs.data(), num_captures);
        } else {
            re2::RE2::ConsumeN(&input, re, nullptr, 0);
        }
    }

    // Test FindAndConsumeN
    {
        re2::StringPiece input(text);
        if (num_captures > 0) {
            re2::RE2::FindAndConsumeN(&input, re, arg_ptrs.data(), num_captures);
        } else {
            re2::RE2::FindAndConsumeN(&input, re, nullptr, 0);
        }
    }

    // Test PartialMatchN
    {
        re2::StringPiece sp_text(text);
        if (num_captures > 0) {
            re2::RE2::PartialMatchN(sp_text, re, arg_ptrs.data(), num_captures);
        } else {
            re2::RE2::PartialMatchN(sp_text, re, nullptr, 0);
        }
    }

    // Test Consume (template version) - handle 0 captures differently
    {
        re2::StringPiece input(text);
        if (num_captures == 0) {
            // For 0 captures, use ConsumeN instead
            re2::RE2::ConsumeN(&input, re, nullptr, 0);
        } else {
            switch (num_captures) {
                case 1: {
                    std::string cap1;
                    re2::RE2::Consume(&input, re, &cap1);
                    break;
                }
                case 2: {
                    std::string cap1, cap2;
                    re2::RE2::Consume(&input, re, &cap1, &cap2);
                    break;
                }
                case 3: {
                    std::string cap1, cap2, cap3;
                    re2::RE2::Consume(&input, re, &cap1, &cap2, &cap3);
                    break;
                }
                case 4: {
                    std::string cap1, cap2, cap3, cap4;
                    re2::RE2::Consume(&input, re, &cap1, &cap2, &cap3, &cap4);
                    break;
                }
            }
        }
    }

    // Test with NULL arguments for some capture groups
    if (num_captures >= 2) {
        std::vector<const re2::RE2::Arg*> mixed_ptrs;
        mixed_ptrs.push_back(nullptr);
        mixed_ptrs.push_back(&args[1]);
        if (num_captures >= 3) mixed_ptrs.push_back(nullptr);
        
        re2::StringPiece input(text);
        re2::RE2::FindAndConsumeN(&input, re, mixed_ptrs.data(), mixed_ptrs.size());
        
        re2::StringPiece sp_text(text);
        re2::RE2::PartialMatchN(sp_text, re, mixed_ptrs.data(), mixed_ptrs.size());
    }

    // Test with empty text
    {
        re2::StringPiece empty_text("");
        if (num_captures > 0) {
            re2::RE2::PartialMatchN(empty_text, re, arg_ptrs.data(), num_captures);
        } else {
            re2::RE2::PartialMatchN(empty_text, re, nullptr, 0);
        }
        
        re2::StringPiece empty_input("");
        if (num_captures > 0) {
            re2::RE2::FindAndConsumeN(&empty_input, re, arg_ptrs.data(), num_captures);
        } else {
            re2::RE2::FindAndConsumeN(&empty_input, re, nullptr, 0);
        }
    }

    return 0;
}