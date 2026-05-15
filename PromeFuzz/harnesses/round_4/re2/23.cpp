// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::CRadix at re2.h:950:22 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::Hex at re2.h:957:22 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Octal at re2.h:964:22 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#include "re2/re2.h"

static void FuzzWithNumericParsers(const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    // Split input for pattern and text
    size_t pattern_len = data[0] % (size - 1) + 1;
    std::string pattern(reinterpret_cast<const char*>(data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(data + 1 + pattern_len), 
                     size - 1 - pattern_len);
    
    re2::RE2::Options options;
    re2::RE2 re(pattern, options);
    
    if (!re.ok()) return;
    
    // Test with different numeric types
    {
        int int_val = 0;
        long long_val = 0;
        unsigned int uint_val = 0;
        
        re2::RE2::Arg arg1 = re2::RE2::CRadix(&int_val);
        re2::RE2::Arg arg2 = re2::RE2::Hex(&long_val);
        re2::RE2::Arg arg3 = re2::RE2::Octal(&uint_val);
        
        const re2::RE2::Arg* args[] = { &arg1, &arg2, &arg3 };
        
        re2::RE2::PartialMatchN(text, re, args, 3);
        re2::RE2::FullMatchN(text, re, args, 3);
        
        absl::string_view input_view(text);
        re2::RE2::FindAndConsumeN(&input_view, re, args, 3);
    }
    
    // Test with different combinations of arguments
    {
        short short_val = 0;
        unsigned short ushort_val = 0;
        
        re2::RE2::Arg arg1 = re2::RE2::CRadix(&short_val);
        re2::RE2::Arg arg2 = re2::RE2::Hex(&ushort_val);
        re2::RE2::Arg arg3 = re2::RE2::Octal(&short_val);
        
        const re2::RE2::Arg* args1[] = { &arg1 };
        const re2::RE2::Arg* args2[] = { &arg2, &arg3 };
        
        re2::RE2::PartialMatchN(text, re, args1, 1);
        re2::RE2::FullMatchN(text, re, args2, 2);
        
        absl::string_view input_view(text);
        re2::RE2::FindAndConsumeN(&input_view, re, args1, 1);
    }
    
    // Test with single numeric parser
    {
        int val = 0;
        re2::RE2::Arg arg = re2::RE2::CRadix(&val);
        const re2::RE2::Arg* args[] = { &arg };
        
        re2::RE2::PartialMatchN(text, re, args, 1);
        
        absl::string_view input_view(text);
        re2::RE2::FindAndConsumeN(&input_view, re, args, 1);
    }
    
    // Test with just Hex parser
    {
        unsigned long ulong_val = 0;
        re2::RE2::Arg arg = re2::RE2::Hex(&ulong_val);
        const re2::RE2::Arg* args[] = { &arg };
        
        re2::RE2::FullMatchN(text, re, args, 1);
    }
    
    // Test with just Octal parser
    {
        int octal_val = 0;
        re2::RE2::Arg arg = re2::RE2::Octal(&octal_val);
        const re2::RE2::Arg* args[] = { &arg };
        
        re2::RE2::PartialMatchN(text, re, args, 1);
        
        absl::string_view input_view(text);
        re2::RE2::FindAndConsumeN(&input_view, re, args, 1);
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    FuzzWithNumericParsers(data, size);
    return 0;
}