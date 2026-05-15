// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::FullMatchN at re2.cc:418:11 in re2.h
// re2::RE2::Options::set_encoding at re2.h:700:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::ConsumeN at re2.cc:428:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::PartialMatchN at re2.cc:423:11 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsumeN at re2.cc:439:11 in re2.h
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

static void TestConsumeN(absl::string_view input, const re2::RE2& re, int n) {
    if (n < 0 || n > 10) return;
    if (input.empty()) return;
    
    absl::string_view mutable_input = input;
    std::vector<std::string> strings(n);
    std::vector<re2::RE2::Arg> args;
    args.reserve(n);
    std::vector<const re2::RE2::Arg*> arg_ptrs;
    arg_ptrs.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        args.emplace_back(&strings[i]);
        arg_ptrs.push_back(&args[i]);
    }
    
    if (n > 0) {
        re2::RE2::ConsumeN(&mutable_input, re, arg_ptrs.data(), n);
    } else {
        // Don't call with nullptr when n=0 to avoid potential issues
        // Just test with empty args array
        std::vector<const re2::RE2::Arg*> empty_args;
        re2::RE2::ConsumeN(&mutable_input, re, empty_args.data(), 0);
    }
}

static void TestPartialMatchN(absl::string_view text, const re2::RE2& re, int n) {
    if (n < 0 || n > 10) return;
    
    std::vector<std::string> strings(n);
    std::vector<re2::RE2::Arg> args;
    args.reserve(n);
    std::vector<const re2::RE2::Arg*> arg_ptrs;
    arg_ptrs.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        args.emplace_back(&strings[i]);
        arg_ptrs.push_back(&args[i]);
    }
    
    if (n > 0) {
        re2::RE2::PartialMatchN(text, re, arg_ptrs.data(), n);
    } else {
        // Don't call with nullptr when n=0
        std::vector<const re2::RE2::Arg*> empty_args;
        re2::RE2::PartialMatchN(text, re, empty_args.data(), 0);
    }
}

static void TestFindAndConsume(absl::string_view input, const re2::RE2& re) {
    if (input.empty()) return;
    
    absl::string_view mutable_input = input;
    std::string s1, s2;
    re2::RE2::FindAndConsume(&mutable_input, re, &s1, &s2);
}

static void TestFindAndConsumeN(absl::string_view input, const re2::RE2& re, int n) {
    if (n < 0 || n > 10) return;
    if (input.empty()) return;
    
    absl::string_view mutable_input = input;
    std::vector<std::string> strings(n);
    std::vector<re2::RE2::Arg> args;
    args.reserve(n);
    std::vector<const re2::RE2::Arg*> arg_ptrs;
    arg_ptrs.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        args.emplace_back(&strings[i]);
        arg_ptrs.push_back(&args[i]);
    }
    
    if (n > 0) {
        re2::RE2::FindAndConsumeN(&mutable_input, re, arg_ptrs.data(), n);
    } else {
        // Don't call with nullptr when n=0
        std::vector<const re2::RE2::Arg*> empty_args;
        re2::RE2::FindAndConsumeN(&mutable_input, re, empty_args.data(), 0);
    }
}

static void TestConsume(absl::string_view input, const re2::RE2& re) {
    if (input.empty()) return;
    
    absl::string_view mutable_input = input;
    std::string s1, s2;
    re2::RE2::Consume(&mutable_input, re, &s1, &s2);
}

static void TestFullMatchN(absl::string_view text, const re2::RE2& re, int n) {
    if (n < 0 || n > 10) return;
    
    std::vector<std::string> strings(n);
    std::vector<re2::RE2::Arg> args;
    args.reserve(n);
    std::vector<const re2::RE2::Arg*> arg_ptrs;
    arg_ptrs.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        args.emplace_back(&strings[i]);
        arg_ptrs.push_back(&args[i]);
    }
    
    if (n > 0) {
        re2::RE2::FullMatchN(text, re, arg_ptrs.data(), n);
    } else {
        // Don't call with nullptr when n=0
        std::vector<const re2::RE2::Arg*> empty_args;
        re2::RE2::FullMatchN(text, re, empty_args.data(), 0);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Limit the size of pattern to avoid excessive memory usage
    size_t pattern_len = Data[0] % (Size - 1) + 1;
    if (pattern_len >= Size) pattern_len = Size / 2;
    
    // Ensure pattern is not empty and within bounds
    if (pattern_len == 0) pattern_len = 1;
    if (pattern_len > Size) return 0;
    
    // Limit maximum pattern length to avoid pathological cases
    if (pattern_len > 100) pattern_len = 100;
    
    // Ensure we have enough data for text
    size_t text_len = Size - pattern_len;
    if (text_len > 1000) text_len = 1000;  // Limit text length
    
    absl::string_view pattern(reinterpret_cast<const char*>(Data), pattern_len);
    absl::string_view text(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    
    // Create RE2 object with Latin1 encoding to handle binary data
    re2::RE2::Options options;
    options.set_encoding(re2::RE2::Options::EncodingLatin1);
    options.set_log_errors(false);
    options.set_max_mem(64 * 1024 * 1024);  // Limit memory usage
    
    re2::RE2 re(pattern, options);
    
    if (!re.ok()) {
        return 0;
    }
    
    // Test different API functions with varying parameters
    int n = Data[0] % 5;  // 0-4 capture groups
    
    // Only test with non-empty text for functions that modify input
    if (!text.empty()) {
        TestConsumeN(text, re, n);
        TestFindAndConsume(text, re);
        TestFindAndConsumeN(text, re, n);
        TestConsume(text, re);
    }
    
    // These functions can handle empty text
    TestPartialMatchN(text, re, n);
    TestFullMatchN(text, re, n);
    
    // Test with empty string view
    absl::string_view empty;
    TestPartialMatchN(empty, re, 0);
    TestFullMatchN(empty, re, 0);
    
    // Test with different numbers of capture groups (limited range)
    for (int i = 0; i <= 2; ++i) {
        if (!text.empty()) {
            TestConsumeN(text, re, i);
            TestFindAndConsumeN(text, re, i);
        }
        TestPartialMatchN(text, re, i);
        TestFullMatchN(text, re, i);
    }
    
    return 0;
}