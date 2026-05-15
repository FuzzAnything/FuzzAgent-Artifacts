// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_posix_syntax at re2.h:703:10 in re2.h
// re2::RE2::Options::set_longest_match at re2.h:706:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_never_nl at re2.h:715:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
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

static re2::RE2::Options::Encoding getEncodingFromByte(uint8_t byte) {
    return (byte & 1) ? re2::RE2::Options::EncodingLatin1 : re2::RE2::Options::EncodingUTF8;
}

static bool getBoolFromByte(uint8_t byte) {
    return (byte & 1) != 0;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    re2::RE2::Options options;

    uint8_t config_byte = Data[0];
    size_t pattern_start = 1;
    size_t pattern_size = (Size > 1) ? Size - 1 : 0;

    options.set_log_errors(getBoolFromByte(config_byte >> 0));
    options.set_encoding(getEncodingFromByte(config_byte >> 1));
    options.set_posix_syntax(getBoolFromByte(config_byte >> 2));
    options.set_longest_match(getBoolFromByte(config_byte >> 3));
    options.set_literal(getBoolFromByte(config_byte >> 4));
    options.set_never_nl(getBoolFromByte(config_byte >> 5));

    std::string pattern;
    if (pattern_size > 0) {
        pattern.assign(reinterpret_cast<const char*>(Data + pattern_start), pattern_size);
    } else {
        pattern = "";
    }

    re2::RE2 re(pattern, options);
    bool ok = re.ok();

    (void)ok;
    return 0;
}