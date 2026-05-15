// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_dot_nl at re2.h:718:10 in re2.h
// re2::RE2::Options::set_literal at re2.h:712:10 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::options at re2.h:758:18 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::Copy at re2.h:735:10 in re2.h
// re2::RE2::Options::ParseFlags at re2.cc:166:19 in re2.h
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
#include <cstring>

static void test_options_functions(const uint8_t* data, size_t size) {
    // Create Options objects with different configurations
    re2::RE2::Options opts1(re2::RE2::DefaultOptions);
    re2::RE2::Options opts2(re2::RE2::Latin1);
    re2::RE2::Options opts3(re2::RE2::POSIX);
    re2::RE2::Options opts4(re2::RE2::Quiet);
    
    // Use fuzzer data to set parameters
    if (size > 0) {
        opts1.set_max_mem(static_cast<int64_t>(data[0]) * 1024);
        opts1.set_dot_nl((data[0] & 0x01) != 0);
        opts1.set_literal((data[0] & 0x02) != 0);
    }
    
    if (size > 1) {
        opts2.set_max_mem(static_cast<int64_t>(data[1]) * 1024);
        opts2.set_dot_nl((data[1] & 0x01) != 0);
        opts2.set_literal((data[1] & 0x02) != 0);
    }
    
    // Test ParseFlags on different options
    (void)opts1.ParseFlags();
    (void)opts2.ParseFlags();
    (void)opts3.ParseFlags();
    (void)opts4.ParseFlags();
    
    // Test Copy function
    re2::RE2::Options opts_copy(re2::RE2::DefaultOptions);
    opts_copy.Copy(opts1);
    opts_copy.Copy(opts2);
    opts_copy.Copy(opts3);
    opts_copy.Copy(opts4);
    
    // Test ParseFlags on copied options
    (void)opts_copy.ParseFlags();
}

static void test_re2_options_function(const uint8_t* data, size_t size) {
    // Create a pattern from fuzzer data (or use default if too small)
    std::string pattern;
    if (size > 0) {
        // Use up to 100 bytes for pattern to avoid overly long patterns
        size_t pattern_len = size < 100 ? size : 100;
        pattern.assign(reinterpret_cast<const char*>(data), pattern_len);
    } else {
        pattern = "test";
    }
    
    // Create RE2 objects with different options
    re2::RE2::Options opts1(re2::RE2::DefaultOptions);
    re2::RE2::Options opts2(re2::RE2::Latin1);
    
    if (size > 0) {
        opts1.set_max_mem(static_cast<int64_t>(data[0]) * 1024);
        opts2.set_max_mem(static_cast<int64_t>(data[0]) * 1024);
    }
    
    // Create RE2 objects
    re2::RE2 re2_1(pattern, opts1);
    re2::RE2 re2_2(pattern, opts2);
    
    // Test options() function
    const re2::RE2::Options& retrieved_opts1 = re2_1.options();
    const re2::RE2::Options& retrieved_opts2 = re2_2.options();
    
    // Use the retrieved options
    (void)retrieved_opts1.ParseFlags();
    (void)retrieved_opts2.ParseFlags();
    
    // Test copying between retrieved options
    re2::RE2::Options new_opts(re2::RE2::DefaultOptions);
    new_opts.Copy(retrieved_opts1);
    new_opts.Copy(retrieved_opts2);
    
    // Test ParseFlags on the new options
    (void)new_opts.ParseFlags();
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
    // Test the Options class functions
    test_options_functions(data, size);
    
    // Test the RE2 class options() function
    test_re2_options_function(data, size);
    
    return 0;
}