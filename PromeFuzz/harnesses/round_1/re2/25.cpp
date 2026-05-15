// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
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
#include <cstring>
#include <string>
#include <optional>
#include <memory>

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), Size - 1 - pattern_len);
    
    // Try to create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) {
        return 0;
    }

    // Test RE2::Consume
    {
        absl::string_view input_view(text);
        std::string captured_str;
        absl::string_view captured_view;
        int captured_int = 0;
        std::optional<int> captured_opt;
        
        re2::RE2::Consume(&input_view, re, &captured_str);
        
        // Reset and try with multiple capture groups
        input_view = text;
        re2::RE2::Consume(&input_view, re, &captured_str, &captured_view, &captured_int);
        
        // Reset and try with NULL arguments
        input_view = text;
        re2::RE2::Consume(&input_view, re, nullptr, &captured_str);
    }

    // Test RE2::FullMatch
    {
        std::string captured_str;
        absl::string_view captured_view;
        int captured_int = 0;
        long captured_long = 0;
        std::optional<int> captured_opt;
        
        re2::RE2::FullMatch(text, re, &captured_str);
        re2::RE2::FullMatch(text, re, &captured_str, &captured_view, &captured_int);
        re2::RE2::FullMatch(text, re, nullptr, &captured_str);
        
        // Test with numeric parsing
        re2::RE2::FullMatch(text, re, re2::RE2::Hex(&captured_int));
        re2::RE2::FullMatch(text, re, re2::RE2::Octal(&captured_long));
        re2::RE2::FullMatch(text, re, re2::RE2::CRadix(&captured_int));
        
        // Test with optional
        re2::RE2::FullMatch(text, re, &captured_opt);
    }

    // Test RE2::FullMatchN
    {
        std::string captured_str;
        absl::string_view captured_view;
        int captured_int = 0;
        
        // Create Arg objects first
        re2::RE2::Arg arg1(&captured_str);
        re2::RE2::Arg arg2(&captured_view);
        re2::RE2::Arg arg3(&captured_int);
        
        const re2::RE2::Arg* args[] = { &arg1, &arg2, &arg3 };
        
        re2::RE2::FullMatchN(text, re, args, 3);
        
        // Test with fewer arguments
        re2::RE2::Arg arg4(&captured_str);
        const re2::RE2::Arg* args2[] = { &arg4 };
        re2::RE2::FullMatchN(text, re, args2, 1);
        
        // Test with NULL argument
        re2::RE2::Arg arg5(nullptr);
        re2::RE2::Arg arg6(&captured_str);
        const re2::RE2::Arg* args3[] = { &arg5, &arg6 };
        re2::RE2::FullMatchN(text, re, args3, 2);
    }

    // Test RE2::Hex, RE2::Octal, RE2::CRadix as standalone
    {
        int hex_val = 0;
        int oct_val = 0;
        int cradix_val = 0;
        
        re2::RE2::Arg hex_arg = re2::RE2::Hex(&hex_val);
        re2::RE2::Arg oct_arg = re2::RE2::Octal(&oct_val);
        re2::RE2::Arg cradix_arg = re2::RE2::CRadix(&cradix_val);
        
        // Use them with FullMatch
        re2::RE2::FullMatch(text, re, hex_arg, oct_arg);
        re2::RE2::FullMatch(text, re, cradix_arg);
    }

    // Test with different text types
    {
        const char* cstr_text = text.c_str();
        std::string string_text = text;
        absl::string_view view_text = text;
        
        std::string captured;
        re2::RE2::FullMatch(cstr_text, re, &captured);
        re2::RE2::FullMatch(string_text, re, &captured);
        re2::RE2::FullMatch(view_text, re, &captured);
    }

    return 0;
}