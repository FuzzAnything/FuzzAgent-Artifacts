// This fuzz driver is generated for library re2, aiming to fuzz the following functions:
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::FindAndConsume at re2.h:463:15 in re2.h
// re2::RE2::ok at re2.h:307:8 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::PartialMatch at re2.h:427:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::Consume at re2.h:445:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::FullMatch at re2.h:411:15 in re2.h
// re2::RE2::NumberOfCapturingGroups at re2.h:551:7 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
// re2::RE2::Match at re2.cc:658:11 in re2.h
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
#include "re2/re2.h"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

static void test_partial_match(const RE2& re, absl::string_view text) {
    // Test with no capture arguments
    RE2::PartialMatch(text, re);
    
    // Test with some capture arguments
    std::string capture1, capture2;
    absl::string_view capture3;
    int capture4;
    RE2::PartialMatch(text, re, &capture1);
    RE2::PartialMatch(text, re, &capture1, &capture2);
    RE2::PartialMatch(text, re, &capture1, &capture3);
    RE2::PartialMatch(text, re, &capture1, &capture4);
    RE2::PartialMatch(text, re, static_cast<void*>(nullptr));
}

static void test_consume(const RE2& re, absl::string_view text) {
    // Test with no capture arguments
    absl::string_view input1 = text;
    RE2::Consume(&input1, re);
    
    // Test with capture arguments
    absl::string_view input2 = text;
    std::string capture1;
    absl::string_view capture2;
    RE2::Consume(&input2, re, &capture1);
    
    absl::string_view input3 = text;
    RE2::Consume(&input3, re, &capture1, &capture2);
    
    absl::string_view input4 = text;
    RE2::Consume(&input4, re, static_cast<void*>(nullptr));
}

static void test_full_match(const RE2& re, absl::string_view text) {
    // Test with no capture arguments
    RE2::FullMatch(text, re);
    
    // Test with capture arguments
    std::string capture1;
    absl::string_view capture2;
    int capture3;
    RE2::FullMatch(text, re, &capture1);
    RE2::FullMatch(text, re, &capture1, &capture2);
    RE2::FullMatch(text, re, &capture1, &capture3);
    RE2::FullMatch(text, re, static_cast<void*>(nullptr));
}

static void test_match(const RE2& re, absl::string_view text) {
    // Test with different numbers of submatches
    int nsubmatch = re.NumberOfCapturingGroups() + 1;
    if (nsubmatch < 0) nsubmatch = 0;
    
    // Limit to reasonable number to avoid excessive allocation
    if (nsubmatch > 10) nsubmatch = 10;
    
    std::vector<absl::string_view> submatch(nsubmatch);
    
    // Test with different anchors
    re.Match(text, 0, text.size(), RE2::UNANCHORED, submatch.data(), nsubmatch);
    re.Match(text, 0, text.size(), RE2::ANCHOR_START, submatch.data(), nsubmatch);
    re.Match(text, 0, text.size(), RE2::ANCHOR_BOTH, submatch.data(), nsubmatch);
    
    // Test with different start positions
    if (text.size() > 0) {
        size_t startpos = text.size() / 2;
        size_t endpos = text.size();
        re.Match(text, startpos, endpos, RE2::UNANCHORED, submatch.data(), nsubmatch);
    }
    
    // Test with zero submatches (should be faster)
    re.Match(text, 0, text.size(), RE2::UNANCHORED, nullptr, 0);
    
    // Test with one submatch
    if (nsubmatch >= 1) {
        absl::string_view single_submatch;
        re.Match(text, 0, text.size(), RE2::UNANCHORED, &single_submatch, 1);
    }
}

static void test_find_and_consume(const RE2& re, absl::string_view text) {
    // Test with no capture arguments
    absl::string_view input1 = text;
    RE2::FindAndConsume(&input1, re);
    
    // Test with capture arguments
    absl::string_view input2 = text;
    std::string capture1;
    absl::string_view capture2;
    RE2::FindAndConsume(&input2, re, &capture1);
    
    absl::string_view input3 = text;
    RE2::FindAndConsume(&input3, re, &capture1, &capture2);
    
    absl::string_view input4 = text;
    RE2::FindAndConsume(&input4, re, static_cast<void*>(nullptr));
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    // Split input into pattern and text
    size_t pattern_len = Size / 2;
    size_t text_len = Size - pattern_len;
    
    // Create pattern string (might contain null bytes)
    std::string pattern(reinterpret_cast<const char*>(Data), pattern_len);
    absl::string_view text(reinterpret_cast<const char*>(Data + pattern_len), text_len);
    
    // Create RE2 object with the pattern
    RE2 re(pattern);
    
    // If regex is invalid, skip further tests
    if (!re.ok()) {
        return 0;
    }
    
    // Test NumberOfCapturingGroups
    int num_groups = re.NumberOfCapturingGroups();
    (void)num_groups; // Use result to avoid unused variable warning
    
    // Test all API functions
    test_partial_match(re, text);
    test_consume(re, text);
    test_full_match(re, text);
    test_match(re, text);
    test_find_and_consume(re, text);
    
    return 0;
}