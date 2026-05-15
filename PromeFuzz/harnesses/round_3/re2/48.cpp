// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Options::set_never_capture at re2.h:721:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Options::never_capture at re2.h:720:10 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
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
#include <cstring>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into pattern and text parts
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len >= Size) {
        pattern_len = Size / 2;
    }
    
    std::string pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    size_t text_len = Size - 1 - pattern_len;
    std::string text(reinterpret_cast<const char*>(Data + 1 + pattern_len), text_len);
    
    // Test with different options
    for (int i = 0; i < 2; ++i) {
        re2::RE2::Options options;
        options.set_never_capture(i == 0);
        
        // Create RE2 object
        re2::RE2 re(pattern, options);
        if (!re.ok()) {
            continue;
        }
        
        // Test never_capture option
        static bool never_cap = options.never_capture();
        (void)never_cap;
        
        // Test NumberOfCapturingGroups
        static int num_groups = re.NumberOfCapturingGroups();
        (void)num_groups;
        
        // Test PartialMatch with different capture combinations
        std::string capture1;
        std::string capture2;
        absl::string_view capture_view;
        
        // Test with no captures
        static bool partial1 = re2::RE2::PartialMatch(text, re);
        (void)partial1;
        
        // Test with one capture
        static bool partial2 = re2::RE2::PartialMatch(text, re, &capture1);
        (void)partial2;
        
        // Test with two captures
        static bool partial3 = re2::RE2::PartialMatch(text, re, &capture1, &capture2);
        (void)partial3;
        
        // Test with string_view capture
        static bool partial4 = re2::RE2::PartialMatch(text, re, &capture_view);
        (void)partial4;
        
        // Test FullMatch with different capture combinations
        static bool full1 = re2::RE2::FullMatch(text, re);
        (void)full1;
        
        static bool full2 = re2::RE2::FullMatch(text, re, &capture1);
        (void)full2;
        
        static bool full3 = re2::RE2::FullMatch(text, re, &capture1, &capture2);
        (void)full3;
        
        static bool full4 = re2::RE2::FullMatch(text, re, &capture_view);
        (void)full4;
        
        // Test Consume
        absl::string_view input_copy = text;
        static bool consume1 = re2::RE2::Consume(&input_copy, re);
        (void)consume1;
        
        input_copy = text;
        static bool consume2 = re2::RE2::Consume(&input_copy, re, &capture1);
        (void)consume2;
        
        input_copy = text;
        static bool consume3 = re2::RE2::Consume(&input_copy, re, &capture1, &capture2);
        (void)consume3;
        
        input_copy = text;
        static bool consume4 = re2::RE2::Consume(&input_copy, re, &capture_view);
        (void)consume4;
        
        // Test with NULL captures
        static bool partial_null = re2::RE2::PartialMatch(text, re, static_cast<std::string*>(nullptr));
        (void)partial_null;
        
        static bool full_null = re2::RE2::FullMatch(text, re, static_cast<std::string*>(nullptr));
        (void)full_null;
        
        input_copy = text;
        static bool consume_null = re2::RE2::Consume(&input_copy, re, static_cast<std::string*>(nullptr));
        (void)consume_null;
    }
    
    return 0;
}