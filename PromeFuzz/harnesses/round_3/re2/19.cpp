// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
// re2::RE2::Options::set_case_sensitive at re2.h:724:10 in re2.h
// re2::RE2::Options::set_longest_match at re2.h:706:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
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
#include <stdint.h>
#include <string>
#include <vector>
#include <fstream>

static void WriteToDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Split input into pattern and text parts
    size_t split = size / 2;
    std::string pattern(reinterpret_cast<const char*>(data), split);
    std::string text(reinterpret_cast<const char*>(data + split), size - split);
    
    // Create RE2 object
    re2::RE2 re(pattern);
    if (!re.ok()) return 0;
    
    // Prepare capture strings and Arg objects
    std::string cap1, cap2, cap3;
    re2::RE2::Arg args[] = {
        re2::RE2::Arg(&cap1),
        re2::RE2::Arg(&cap2),
        re2::RE2::Arg(&cap3)
    };
    const re2::RE2::Arg* args_ptr[] = {&args[0], &args[1], &args[2]};
    
    // Test ConsumeN
    {
        absl::string_view input_view(text);
        re2::RE2::ConsumeN(&input_view, re, args_ptr, 3);
    }
    
    // Test FindAndConsume (template version)
    {
        absl::string_view input_view(text);
        re2::RE2::FindAndConsume(&input_view, re, &cap1, &cap2);
    }
    
    // Test PartialMatchN
    {
        re2::RE2::PartialMatchN(text, re, args_ptr, 3);
    }
    
    // Test FullMatchN
    {
        re2::RE2::FullMatchN(text, re, args_ptr, 3);
    }
    
    // Test Consume (template version)
    {
        absl::string_view input_view(text);
        re2::RE2::Consume(&input_view, re, &cap1, &cap2, &cap3);
    }
    
    // Test FindAndConsumeN
    {
        absl::string_view input_view(text);
        re2::RE2::FindAndConsumeN(&input_view, re, args_ptr, 3);
    }
    
    // Test with different options
    {
        re2::RE2::Options options;
        options.set_case_sensitive((size % 2) == 0);
        options.set_longest_match((size % 3) == 0);
        options.set_log_errors(false);
        
        re2::RE2 re2_with_options(pattern, options);
        if (re2_with_options.ok()) {
            absl::string_view input_view(text);
            re2::RE2::ConsumeN(&input_view, re2_with_options, args_ptr, 2);
        }
    }
    
    // Test with empty pattern
    if (pattern.empty()) {
        re2::RE2 empty_re("");
        if (empty_re.ok()) {
            absl::string_view input_view(text);
            re2::RE2::FindAndConsumeN(&input_view, empty_re, args_ptr, 1);
        }
    }
    
    // Test with file input
    WriteToDummyFile(data, size);
    {
        std::ifstream file("./dummy_file", std::ios::binary);
        if (file) {
            std::string file_content((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
            if (!file_content.empty()) {
                absl::string_view file_view(file_content);
                re2::RE2::PartialMatchN(file_view, re, args_ptr, 2);
            }
        }
    }
    
    return 0;
}