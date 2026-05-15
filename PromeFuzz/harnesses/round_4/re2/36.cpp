// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_longest_match at re2.h:706:10 in re2.h
// re2::RE2::Options::longest_match at re2.h:705:10 in re2.h
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::longest_match at re2.h:705:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_longest_match at re2.h:706:10 in re2.h
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::set_perl_classes at re2.h:727:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
// re2::RE2::Options::longest_match at re2.h:705:10 in re2.h
// re2::RE2::Options::posix_syntax at re2.h:702:10 in re2.h
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
#include <cstddef>
#include "re2/re2.h"

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use first byte to determine which canned option to use
    uint8_t canned_option = Data[0] % 4;
    re2::RE2::CannedOptions canned;
    switch (canned_option) {
        case 0: canned = re2::RE2::DefaultOptions; break;
        case 1: canned = re2::RE2::Latin1; break;
        case 2: canned = re2::RE2::POSIX; break;
        case 3: canned = re2::RE2::Quiet; break;
        default: canned = re2::RE2::DefaultOptions; break;
    }

    // Create Options object with canned option
    re2::RE2::Options options(canned);

    // Use remaining bytes to set various options and test getters
    for (size_t i = 1; i < Size; i++) {
        uint8_t byte = Data[i];
        uint8_t action = byte & 0x07; // Use lower 3 bits for action
        bool value = (byte >> 3) & 0x01; // Use next bit for boolean value

        switch (action) {
            case 0:
                options.set_dot_nl(value);
                break;
            case 1:
                options.set_longest_match(value);
                // Test the getter
                static_cast<void>(options.longest_match());
                break;
            case 2:
                options.set_posix_syntax(value);
                // Test the getter
                static_cast<void>(options.posix_syntax());
                break;
            case 3:
                options.set_perl_classes(value);
                break;
            case 4:
                // Just test getters without setting
                static_cast<void>(options.longest_match());
                static_cast<void>(options.posix_syntax());
                break;
            case 5:
                // Set multiple options
                options.set_dot_nl(value);
                options.set_longest_match(!value);
                break;
            case 6:
                // Set and test in combination
                options.set_posix_syntax(value);
                options.set_perl_classes(value);
                static_cast<void>(options.posix_syntax());
                break;
            case 7:
                // Test all getters
                static_cast<void>(options.longest_match());
                static_cast<void>(options.posix_syntax());
                break;
        }
    }

    return 0;
}