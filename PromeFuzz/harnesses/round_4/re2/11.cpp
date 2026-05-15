// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::pattern at re2.h:312:22 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::encoding at re2.h:699:14 in re2.h
// re2::RE2::pattern at re2.h:312:22 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::encoding at re2.h:699:14 in re2.h
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the fuzzer input to create a pattern string
    std::string pattern_str(reinterpret_cast< const char* >(Data), Size);
    
    // Create RE2::Options with default settings
    re2::RE2::Options options;
    
    // Set encoding based on first byte of input
    if (Size > 0) {
        if (Data[0] % 2 == 0) {
            options.set_encoding(re2::RE2::Options::EncodingLatin1);
        } else {
            options.set_encoding(re2::RE2::Options::EncodingUTF8);
        }
    }
    
    // Create RE2 object with the pattern and options
    re2::RE2 re(pattern_str, options);
    
    // Call the functions in the specified order
    // 1. re2::RE2::pattern
    const std::string& pattern1 = re.pattern();
    (void)pattern1; // Use result to avoid unused variable warning
    
    // 2. re2::RE2::options
    const re2::RE2::Options& opts1 = re.options();
    (void)opts1;
    
    // 3. re2::RE2::Options::encoding
    re2::RE2::Options::Encoding enc1 = opts1.encoding();
    (void)enc1;
    
    // 4. re2::RE2::pattern (second call)
    const std::string& pattern2 = re.pattern();
    (void)pattern2;
    
    // 5. re2::RE2::options (second call)
    const re2::RE2::Options& opts2 = re.options();
    (void)opts2;
    
    // 6. re2::RE2::Options::encoding (second call)
    re2::RE2::Options::Encoding enc2 = opts2.encoding();
    (void)enc2;
    
    return 0;
}