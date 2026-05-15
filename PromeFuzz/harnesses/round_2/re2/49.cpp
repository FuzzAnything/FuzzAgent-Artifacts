// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::MaxSubmatch at re2.cc:1011:10 in re2.h
// re2::RE2::Extract at re2.cc:542:11 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::CheckRewriteString at re2.cc:971:11 in re2.h
// re2::RE2::GlobalReplace at re2.cc:472:10 in re2.h
// re2::RE2::Replace at re2.cc:450:11 in re2.h
// re2::RE2::Options::set_max_mem at re2.h:697:10 in re2.h
// re2::RE2::Options::set_log_errors at re2.h:709:10 in re2.h
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
#include <string>
#include "re2/re2.h"

static void TestMaxSubmatch(absl::string_view rewrite) {
    re2::RE2::MaxSubmatch(rewrite);
}

static void TestExtract(absl::string_view text, const re2::RE2& re,
                       absl::string_view rewrite) {
    std::string out;
    // Ensure text and out don't alias by using separate buffers
    // Also ensure text is valid and not empty
    if (!text.empty() && !rewrite.empty()) {
        // Use try-catch to handle any potential exceptions from bad patterns
        try {
            re2::RE2::Extract(text, re, rewrite, &out);
        } catch (...) {
            // Catch any exceptions to avoid crashes
        }
    }
}

static void TestNumberOfCapturingGroups(const re2::RE2& re) {
    re.NumberOfCapturingGroups();
}

static void TestCheckRewriteString(const re2::RE2& re,
                                  absl::string_view rewrite) {
    std::string error;
    re.CheckRewriteString(rewrite, &error);
}

static void TestGlobalReplace(const re2::RE2& re,
                             absl::string_view rewrite) {
    std::string str = "test string for replacement";
    try {
        re2::RE2::GlobalReplace(&str, re, rewrite);
    } catch (...) {
        // Catch any exceptions to avoid crashes
    }
}

static void TestReplace(const re2::RE2& re,
                       absl::string_view rewrite) {
    std::string str = "test string for replacement";
    try {
        re2::RE2::Replace(&str, re, rewrite);
    } catch (...) {
        // Catch any exceptions to avoid crashes
    }
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Ensure we have valid string views by creating copies
    // First, create a safe buffer with null termination
    std::vector<char> safe_buffer(Data, Data + Size);
    safe_buffer.push_back('\0');
    
    // Use the safe buffer for all string operations
    const char* safe_data = safe_buffer.data();
    
    // Split input into parts for pattern, text, and rewrite
    // Ensure we don't create string_views that go beyond the buffer
    size_t pattern_size = std::min(Size / 3, static_cast<size_t>(50));
    size_t text_size = std::min((Size - pattern_size) / 2, static_cast<size_t>(50));
    size_t rewrite_size = std::min(Size - pattern_size - text_size, static_cast<size_t>(50));
    
    if (pattern_size == 0 || text_size == 0 || rewrite_size == 0) return 0;
    
    // Create safe string views ensuring they point to valid memory
    // Check that we're not creating views beyond the safe buffer
    if (pattern_size + text_size + rewrite_size > Size) return 0;
    
    absl::string_view pattern(safe_data, pattern_size);
    absl::string_view text(safe_data + pattern_size, text_size);
    absl::string_view rewrite(safe_data + pattern_size + text_size, rewrite_size);
    
    // Test MaxSubmatch (static method, doesn't need RE2 object)
    TestMaxSubmatch(rewrite);
    
    // Create RE2 object with the pattern using safe options
    re2::RE2::Options options;
    options.set_max_mem(64 * 1024); // Limit memory usage
    options.set_log_errors(false); // Disable error logging to avoid crashes
    
    // Try to create RE2 object, but handle potential failures gracefully
    re2::RE2 re(pattern, options);
    
    // Test functions that require a valid RE2 object
    if (re.ok()) {
        // Only test Extract with safe inputs and wrap in try-catch
        if (!text.empty() && !rewrite.empty()) {
            try {
                TestExtract(text, re, rewrite);
            } catch (...) {
                // Catch any exceptions to avoid crashes
            }
        }
        TestNumberOfCapturingGroups(re);
        TestCheckRewriteString(re, rewrite);
        TestGlobalReplace(re, rewrite);
        TestReplace(re, rewrite);
    } else {
        // Even if RE2 construction failed, we can still test some functions
        TestNumberOfCapturingGroups(re);  // Should return -1 for invalid regex
        TestCheckRewriteString(re, rewrite);
    }
    
    return 0;
}