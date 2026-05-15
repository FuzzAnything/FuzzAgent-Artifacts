#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "absl/strings/string_view.h"
#include "re2/re2.h"

namespace harness_013 {
// RE2 fuzzing harness - properly tests RE2 library functionality
// Tests basic RE2 regex matching with various options and patterns
// This is an improved version that properly links with RE2 library



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 4) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume various parameters for RE2 options
    RE2::Options options;
    
    // Set encoding (UTF-8 or Latin1)
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                          : RE2::Options::EncodingUTF8);
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log errors during fuzzing
    options.set_max_mem(64 << 20);  // 64MB memory limit
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());  // Invert: true means case-sensitive
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Consume pattern and text strings from fuzzer input
    // Limit size to prevent excessive memory usage
    // Ensure we have at least some bytes for pattern
    if (fdp.remaining_bytes() == 0) return 0;
    size_t pattern_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(size_t(256), fdp.remaining_bytes()));
    std::string pattern = fdp.ConsumeBytesAsString(pattern_size);
    // Use remaining bytes for text
    std::string text = fdp.ConsumeRemainingBytesAsString();
    
    // Skip if pattern is empty after trimming null bytes
    if (pattern.empty()) return 0;
    
    // Create RE2 object with the pattern
    RE2 re(pattern, options);
    
    // If RE2 compilation fails, return early
    if (!re.ok()) return 0;
    
    // Test basic matching functions
    
    // Test 1: FullMatch - requires entire text to match pattern
    RE2::FullMatch(text, re);
    
    // Test 2: PartialMatch - requires only part of text to match pattern
    RE2::PartialMatch(text, re);
    
    // Test 3: Replace - replace matches in text
    std::string replace_text = text;
    RE2::Replace(&replace_text, re, "X");
    
    // Test 4: GlobalReplace - replace all matches in text
    std::string global_replace_text = text;
    RE2::GlobalReplace(&global_replace_text, re, "XX");
    
    // Test 5: Consume - consume matching prefix
    absl::string_view consume_text = text;
    RE2::Consume(&consume_text, re);
    
    // Test 6: FindAndConsume - find and consume all matches
    absl::string_view find_consume_text = text;
    RE2::FindAndConsume(&find_consume_text, re);
    
    // Test 7: QuoteMeta - quote regex metacharacters
    std::string quoted = RE2::QuoteMeta(pattern);
    
    // Test 8: PossibleMatchRange - get possible match range
    std::string min_match, max_match;
    re.PossibleMatchRange(&min_match, &max_match, 10);
    
    // Test 9: Program size queries (doesn't modify state)
    re.ProgramSize();
    re.ReverseProgramSize();
    
    // Test 10: Number of capturing groups
    re.NumberOfCapturingGroups();
    
    // Test 11: Test with capturing groups
    if (re.NumberOfCapturingGroups() > 0) {
        std::string captured;
        RE2::PartialMatch(text, re, &captured);
        
        // Test multiple captures
        std::string cap1, cap2;
        RE2::PartialMatch(text, re, &cap1, &cap2);
    }
    
    return 0;
}

} // namespace harness_013
