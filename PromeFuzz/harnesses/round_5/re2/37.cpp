// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to determine configuration options
    uint8_t config = Data[0];
    
    // Create Options object with default constructor
    re2::RE2::Options opt1;
    
    // Set options based on fuzzer input
    opt1.set_max_mem(static_cast<int64_t>(Size) * 100);
    opt1.set_never_nl((config & 0x01) != 0);
    
    // Test ParseFlags
    int flags = opt1.ParseFlags();
    (void)flags; // Use to avoid unused variable warning
    
    // Test max_mem
    int64_t current_max_mem = opt1.max_mem();
    (void)current_max_mem;
    
    // Create another Options object with different settings
    re2::RE2::Options opt2;
    opt2.set_max_mem(static_cast<int64_t>(Size) * 50);
    opt2.set_never_nl((config & 0x02) != 0);
    
    // Test Copy function
    opt1.Copy(opt2);
    
    // Create a pattern from the fuzzer input (skip first byte)
    std::string pattern;
    if (Size > 1) {
        // Use remaining bytes as pattern, but limit length to avoid excessive memory
        size_t pattern_len = Size - 1;
        if (pattern_len > 1000) {
            pattern_len = 1000;
        }
        pattern.assign(reinterpret_cast<const char*>(Data + 1), pattern_len);
    } else {
        pattern = "test";
    }
    
    // Create RE2 object with the pattern and options
    re2::RE2 re(pattern, opt1);
    
    // Test options() function if RE2 object is valid
    if (re.ok()) {
        const re2::RE2::Options& re_options = re.options();
        (void)re_options;
        
        // Test ParseFlags on the returned options
        int re_flags = re_options.ParseFlags();
        (void)re_flags;
        
        // Test max_mem on the returned options
        int64_t re_max_mem = re_options.max_mem();
        (void)re_max_mem;
    }
    
    // Test with different configurations
    re2::RE2::Options opt3;
    
    // Use various bits of config to set different options
    opt3.set_max_mem(static_cast<int64_t>(config) * 1000);
    opt3.set_never_nl((config & 0x04) != 0);
    
    // Test ParseFlags again
    int flags3 = opt3.ParseFlags();
    (void)flags3;
    
    // Create another RE2 object with different options
    if (Size > 10) {
        std::string pattern2(reinterpret_cast<const char*>(Data), 
                            Size > 100 ? 100 : Size);
        re2::RE2 re2(pattern2, opt3);
        
        if (re2.ok()) {
            const re2::RE2::Options& re2_options = re2.options();
            (void)re2_options;
        }
    }
    
    return 0;
}