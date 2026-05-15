// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ReverseProgramSize at re2.cc:329:10 in re2.h
// re2::RE2::ProgramFanout at re2.cc:379:10 in re2.h
// re2::RE2::ReverseProgramFanout at re2.cc:385:10 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
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
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    
    try {
        re2::RE2 re(pattern);
        
        if (!re.ok()) {
            return 0;
        }
        
        int reverse_size = re.ReverseProgramSize();
        
        std::vector<int> fanout_histogram;
        int fanout_result = re.ProgramFanout(&fanout_histogram);
        
        std::vector<int> reverse_fanout_histogram;
        int reverse_fanout_result = re.ReverseProgramFanout(&reverse_fanout_histogram);
        
        int capturing_groups = re.NumberOfCapturingGroups();
        
        (void)reverse_size;
        (void)fanout_result;
        (void)reverse_fanout_result;
        (void)capturing_groups;
        
    } catch (...) {
    }
    
    return 0;
}