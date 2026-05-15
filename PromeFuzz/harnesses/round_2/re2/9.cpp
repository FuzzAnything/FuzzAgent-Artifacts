// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count at re2.cc:48:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Regexp at re2.h:340:16 in re2.h
// re2::RE2::Regexp at re2.h:340:16 in re2.h
// re2::RE2::ProgramSize at re2.cc:323:10 in re2.h
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Use the input data to create a pattern string
    std::string pattern;
    if (Size > 0) {
        pattern.assign(reinterpret_cast<const char*>(Data), Size);
    } else {
        pattern = "";
    }

    // Set maximum global replace count using first byte (if available)
    int max_replace = -1;
    if (Size > 0) {
        max_replace = static_cast<int>(Data[0]);
    }
    re2::RE2::FUZZING_ONLY_set_maximum_global_replace_count(max_replace);

    // Create RE2 object with default options
    re2::RE2::Options options;
    re2::RE2 re(pattern, options);

    // Call the target functions in specified order
    bool ok_result = re.ok();
    (void)ok_result; // Avoid unused variable warning
    
    re2::Regexp* regexp1 = re.Regexp();
    re2::Regexp* regexp2 = re.Regexp();
    (void)regexp1; // Avoid unused variable warning
    (void)regexp2; // Avoid unused variable warning
    
    int program_size = re.ProgramSize();
    (void)program_size; // Avoid unused variable warning

    return 0;
}