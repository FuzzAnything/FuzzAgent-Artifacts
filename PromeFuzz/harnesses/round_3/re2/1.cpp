// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
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
#include <cstddef>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create pattern string from fuzzer input
    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    
    // Create RE2 object with the pattern
    re2::RE2 re(pattern);
    
    // Call functions in specified order
    
    // 1. ReverseProgramSize
    int reverseSize = re.ReverseProgramSize();
    
    // 2. ProgramFanout
    std::vector<int> fanoutHistogram;
    int fanoutResult = re.ProgramFanout(&fanoutHistogram);
    
    // 3. ReverseProgramFanout
    std::vector<int> reverseFanoutHistogram;
    int reverseFanoutResult = re.ReverseProgramFanout(&reverseFanoutHistogram);
    
    // 4. NumberOfCapturingGroups
    int capturingGroups = re.NumberOfCapturingGroups();
    
    // All return values are intentionally ignored as they're just for fuzzing
    
    return 0;
}