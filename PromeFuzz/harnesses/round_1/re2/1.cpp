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
#include <cstddef>
#include <vector>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    std::string pattern(reinterpret_cast<const char*>(Data), Size);
    re2::RE2 regex(pattern);

    if (!regex.ok()) {
        return 0;
    }

    // 1. ReverseProgramSize
    int reverse_size = regex.ReverseProgramSize();

    // 2. ProgramFanout
    std::vector<int> histogram1;
    int fanout1 = regex.ProgramFanout(&histogram1);

    // 3. ReverseProgramFanout
    std::vector<int> histogram2;
    int fanout2 = regex.ReverseProgramFanout(&histogram2);

    // 4. NumberOfCapturingGroups
    int capturing_groups = regex.NumberOfCapturingGroups();

    // Use the results to avoid dead code elimination
    (void)reverse_size;
    (void)fanout1;
    (void)fanout2;
    (void)capturing_groups;

    return 0;
}