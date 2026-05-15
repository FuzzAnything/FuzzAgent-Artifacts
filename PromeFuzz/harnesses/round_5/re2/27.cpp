// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
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
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len >= Size) {
        pattern_len = Size / 2;
    }
    
    absl::string_view pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    absl::string_view text(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                          Size - 1 - pattern_len);

    // Create RE2 object with options
    re2::RE2::Options options;
    options.set_log_errors(false);
    re2::RE2 re(pattern, options);

    // Prepare storage for parsed values
    int int_val = 0;
    long long_val = 0;
    std::string str_val;
    
    // Create Arg objects using different parsers
    re2::RE2::Arg int_arg(&int_val);
    re2::RE2::Arg long_arg(&long_val);
    re2::RE2::Arg str_arg(&str_val);
    
    re2::RE2::Arg octal_arg = re2::RE2::Octal(&int_val);
    re2::RE2::Arg cradix_arg = re2::RE2::CRadix(&long_val);
    re2::RE2::Arg hex_arg = re2::RE2::Hex(&int_val);

    // Test PartialMatchN with different Arg combinations
    const re2::RE2::Arg* args1[] = {&int_arg};
    const re2::RE2::Arg* args2[] = {&str_arg, &int_arg};
    const re2::RE2::Arg* args3[] = {&octal_arg, &cradix_arg, &hex_arg};
    
    re2::RE2::PartialMatchN(text, re, args1, 1);
    re2::RE2::PartialMatchN(text, re, args2, 2);
    re2::RE2::PartialMatchN(text, re, args3, 3);

    // Test FindAndConsumeN
    absl::string_view mutable_text = text;
    re2::RE2::FindAndConsumeN(&mutable_text, re, args1, 1);
    
    // Reset mutable_text for another test
    mutable_text = text;
    re2::RE2::FindAndConsumeN(&mutable_text, re, args3, 3);

    // Test FullMatchN
    re2::RE2::FullMatchN(text, re, args1, 1);
    re2::RE2::FullMatchN(text, re, args2, 2);
    re2::RE2::FullMatchN(text, re, args3, 3);

    // Test the parser creation functions individually
    static auto test_parser_creators = []() {
        int test_int = 0;
        long test_long = 0;
        
        re2::RE2::Arg octal_test = re2::RE2::Octal(&test_int);
        re2::RE2::Arg cradix_test = re2::RE2::CRadix(&test_long);
        re2::RE2::Arg hex_test = re2::RE2::Hex(&test_int);
        
        // These are just to create the objects, no need to use them
        (void)octal_test;
        (void)cradix_test;
        (void)hex_test;
    };
    
    test_parser_creators();

    return 0;
}