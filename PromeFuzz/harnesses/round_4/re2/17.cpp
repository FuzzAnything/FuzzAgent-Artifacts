// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::max_mem at re2.h:696:13 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
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

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Use first byte to control behavior
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    // Create options objects with different canned options
    re2::RE2::Options opt1(re2::RE2::DefaultOptions);
    re2::RE2::Options opt2(re2::RE2::Latin1);
    re2::RE2::Options opt3(re2::RE2::POSIX);
    re2::RE2::Options opt4(re2::RE2::Quiet);
    
    // Test set_case_sensitive with fuzzer data
    bool case_sensitive = (selector & 0x01) != 0;
    opt1.set_case_sensitive(case_sensitive);
    opt2.set_case_sensitive(!case_sensitive);
    
    // Test set_max_mem with fuzzer data
    int64_t max_mem_value = 8 * 1024 * 1024; // Default 8MB
    if (Size >= sizeof(int64_t)) {
        // Use next 8 bytes for max_mem value
        memcpy(&max_mem_value, Data, sizeof(int64_t));
        Data += sizeof(int64_t);
        Size -= sizeof(int64_t);
    }
    
    // Ensure non-negative max_mem
    if (max_mem_value < 0) {
        max_mem_value = -max_mem_value;
    }
    
    opt1.set_max_mem(max_mem_value);
    opt2.set_max_mem(max_mem_value / 2);
    opt3.set_max_mem(max_mem_value * 2);
    
    // Test Copy function
    opt4.Copy(opt1);
    re2::RE2::Options opt5(re2::RE2::DefaultOptions);
    opt5.Copy(opt2);
    
    // Test max_mem() function
    int64_t mem1 = opt1.max_mem();
    int64_t mem2 = opt2.max_mem();
    int64_t mem3 = opt3.max_mem();
    int64_t mem4 = opt4.max_mem();
    int64_t mem5 = opt5.max_mem();
    
    // Test ParseFlags function
    int flags1 = opt1.ParseFlags();
    int flags2 = opt2.ParseFlags();
    int flags3 = opt3.ParseFlags();
    int flags4 = opt4.ParseFlags();
    int flags5 = opt5.ParseFlags();
    
    // Create RE2 objects with different patterns to test options()
    static const char* patterns[] = {
        ".*",
        "\\w+",
        "[a-z]+",
        "\\d{1,3}",
        "^test$"
    };
    
    for (int i = 0; i < 5; i++) {
        // Create RE2 with different options
        re2::RE2 re2_obj1(patterns[i], opt1);
        if (re2_obj1.ok()) {
            const re2::RE2::Options& retrieved_opt1 = re2_obj1.options();
            // Access some properties
            int64_t retrieved_mem = retrieved_opt1.max_mem();
            int retrieved_flags = retrieved_opt1.ParseFlags();
        }
        
        re2::RE2 re2_obj2(patterns[(i + 1) % 5], opt2);
        if (re2_obj2.ok()) {
            const re2::RE2::Options& retrieved_opt2 = re2_obj2.options();
            int64_t retrieved_mem2 = retrieved_opt2.max_mem();
            int retrieved_flags2 = retrieved_opt2.ParseFlags();
        }
        
        // Test with pattern from fuzzer data if available
        if (Size > 0) {
            // Limit pattern length to avoid excessive memory usage
            size_t pattern_len = Size > 100 ? 100 : Size;
            std::string pattern_str(reinterpret_cast<const char*>(Data), pattern_len);
            
            re2::RE2 re2_obj3(pattern_str, opt3);
            if (re2_obj3.ok()) {
                const re2::RE2::Options& retrieved_opt3 = re2_obj3.options();
                int64_t retrieved_mem3 = retrieved_opt3.max_mem();
                int retrieved_flags3 = retrieved_opt3.ParseFlags();
            }
            
            // Use opt4 (which is a copy of opt1)
            re2::RE2 re2_obj4(pattern_str, opt4);
            if (re2_obj4.ok()) {
                const re2::RE2::Options& retrieved_opt4 = re2_obj4.options();
                int64_t retrieved_mem4 = retrieved_opt4.max_mem();
                int retrieved_flags4 = retrieved_opt4.ParseFlags();
            }
        }
    }
    
    // Additional tests with more diverse option configurations
    re2::RE2::Options opt6(re2::RE2::DefaultOptions);
    opt6.set_case_sensitive((selector & 0x02) != 0);
    opt6.set_max_mem(max_mem_value + 1000);
    
    re2::RE2::Options opt7(re2::RE2::Latin1);
    opt7.set_case_sensitive((selector & 0x04) != 0);
    opt7.set_max_mem(max_mem_value - 1000);
    
    // Copy between different option objects
    opt6.Copy(opt7);
    opt7.Copy(opt1);
    
    // Test the functions again on copied options
    int64_t mem6 = opt6.max_mem();
    int64_t mem7 = opt7.max_mem();
    int flags6 = opt6.ParseFlags();
    int flags7 = opt7.ParseFlags();
    
    return 0;
}