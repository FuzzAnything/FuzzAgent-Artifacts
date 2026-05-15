// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#include "re2/re2.h"

static void FuzzRE2Functions(const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    // Split input into pattern and text parts
    size_t pattern_len = data[0] % (size - 1) + 1;
    size_t text_len = size - pattern_len - 1;
    
    if (pattern_len == 0 || text_len == 0) return;
    
    // Limit sizes to prevent excessive memory usage
    if (pattern_len > 1000 || text_len > 1000) return;
    
    std::string pattern(reinterpret_cast<const char*>(data + 1), pattern_len);
    std::string text(reinterpret_cast<const char*>(data + 1 + pattern_len), text_len);
    
    // Create RE2 object with safe options
    re2::RE2::Options options;
    options.set_max_mem(1 << 20); // Limit memory to 1MB
    options.set_log_errors(false); // Disable error logging
    
    re2::RE2 re(pattern, options);
    
    if (!re.ok()) {
        // Pattern compilation failed, skip further processing
        return;
    }
    
    // Prepare arguments for capture groups
    const int max_args = 3;
    re2::RE2::Arg args[max_args];
    std::string captured_strings[max_args];
    const re2::RE2::Arg* arg_ptrs[max_args];
    
    for (int i = 0; i < max_args; i++) {
        args[i] = re2::RE2::Arg(&captured_strings[i]);
        arg_ptrs[i] = &args[i];
    }
    
    // Test ConsumeN
    {
        absl::string_view input_view = text;
        re2::RE2::ConsumeN(&input_view, re, arg_ptrs, max_args);
    }
    
    // Test PartialMatchN
    {
        absl::string_view text_view = text;
        re2::RE2::PartialMatchN(text_view, re, arg_ptrs, max_args);
    }
    
    // Test FindAndConsumeN
    {
        absl::string_view input_view = text;
        re2::RE2::FindAndConsumeN(&input_view, re, arg_ptrs, max_args);
    }
    
    // Test FullMatchN
    {
        absl::string_view text_view = text;
        re2::RE2::FullMatchN(text_view, re, arg_ptrs, max_args);
    }
    
    // Test Consume (variadic template version)
    {
        absl::string_view input_view = text;
        std::string s1, s2, s3;
        re2::RE2::Consume(&input_view, re, &s1, &s2, &s3);
    }
    
    // Test FindAndConsume (variadic template version)
    {
        absl::string_view input_view = text;
        std::string s1, s2, s3;
        re2::RE2::FindAndConsume(&input_view, re, &s1, &s2, &s3);
    }
    
    // Test with different numbers of arguments
    for (int n = 0; n <= max_args; n++) {
        // Test ConsumeN with different argument counts
        {
            absl::string_view input_view = text;
            const re2::RE2::Arg* local_arg_ptrs[max_args];
            for (int i = 0; i < n; i++) {
                local_arg_ptrs[i] = &args[i];
            }
            // Only call if n > 0 or we have valid pointers
            if (n > 0) {
                re2::RE2::ConsumeN(&input_view, re, local_arg_ptrs, n);
            } else {
                re2::RE2::ConsumeN(&input_view, re, nullptr, 0);
            }
        }
        
        // Test PartialMatchN with different argument counts
        {
            absl::string_view text_view = text;
            const re2::RE2::Arg* local_arg_ptrs[max_args];
            for (int i = 0; i < n; i++) {
                local_arg_ptrs[i] = &args[i];
            }
            // Only call if n > 0 or we have valid pointers
            if (n > 0) {
                re2::RE2::PartialMatchN(text_view, re, local_arg_ptrs, n);
            } else {
                re2::RE2::PartialMatchN(text_view, re, nullptr, 0);
            }
        }
    }
    
    // Test with NULL arguments
    {
        absl::string_view input_view = text;
        re2::RE2::ConsumeN(&input_view, re, nullptr, 0);
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    // Add a simple check to avoid processing obviously bad data
    if (size == 0 || data == nullptr) return 0;
    
    FuzzRE2Functions(data, size);
    return 0;
}