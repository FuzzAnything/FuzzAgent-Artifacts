// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
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
#include <string>

static std::string dummy_file = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Split input: first byte determines pattern length, rest is data
    size_t pattern_len = Data[0] % (Size - 1);
    if (pattern_len == 0) pattern_len = 1;
    
    absl::string_view pattern(reinterpret_cast<const char*>(Data + 1), pattern_len);
    absl::string_view text(reinterpret_cast<const char*>(Data + 1 + pattern_len), 
                          Size - 1 - pattern_len);
    
    // Create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;
    
    // Prepare arguments for capture
    std::string captured1, captured2, captured3;
    re2::RE2::Arg args[3];
    args[0] = &captured1;
    args[1] = &captured2;
    args[2] = &captured3;
    const re2::RE2::Arg* args_ptr[3] = {&args[0], &args[1], &args[2]};
    
    // Test Consume (template version)
    {
        absl::string_view consume_input = text;
        re2::RE2::Consume(&consume_input, re, &captured1);
    }
    
    // Test ConsumeN
    {
        absl::string_view consume_input = text;
        re2::RE2::ConsumeN(&consume_input, re, args_ptr, 2);
    }
    
    // Test FindAndConsumeN
    {
        absl::string_view find_input = text;
        re2::RE2::FindAndConsumeN(&find_input, re, args_ptr, 1);
    }
    
    // Test FindAndConsume (template version)
    {
        absl::string_view find_input = text;
        re2::RE2::FindAndConsume(&find_input, re, &captured1, &captured2);
    }
    
    // Test PartialMatchN
    {
        re2::RE2::PartialMatchN(text, re, args_ptr, 3);
    }
    
    // Test FullMatchN
    {
        re2::RE2::FullMatchN(text, re, args_ptr, 1);
    }
    
    return 0;
}