// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ReverseProgramSize at re2.cc:329:10 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
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

    // Create pattern string from fuzz input
    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    
    // Initialize RE2 with default options
    re2::RE2::Options options;
    re2::RE2 re(pattern, options);
    
    // Check if regex is valid before calling functions
    if (!re.ok()) {
        // Still call functions on invalid object to test error paths
        // but don't proceed with histogram operations that might crash
        (void)re.ReverseProgramSize();
        (void)re.NumberOfCapturingGroups();
        return 0;
    }
    
    // 1. Call ReverseProgramSize
    int reverse_size = re.ReverseProgramSize();
    (void)reverse_size; // Use result to avoid optimization
    
    // 2. Call ProgramFanout
    std::vector<int> fanout_histogram;
    int program_fanout = re.ProgramFanout(&fanout_histogram);
    (void)program_fanout;
    
    // 3. Call ReverseProgramFanout
    std::vector<int> reverse_fanout_histogram;
    int reverse_fanout = re.ReverseProgramFanout(&reverse_fanout_histogram);
    (void)reverse_fanout;
    
    // 4. Call NumberOfCapturingGroups
    int capture_groups = re.NumberOfCapturingGroups();
    (void)capture_groups;
    
    return 0;
}