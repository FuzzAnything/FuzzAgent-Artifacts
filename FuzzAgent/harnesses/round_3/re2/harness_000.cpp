#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"
#include "re2/filtered_re2.h"
#include "re2/set.h"

namespace harness_000 {
// Fuzzing harness for RE2 regular expression library
// Targets core API functions: pattern compilation, matching, searching, replacement



// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic pattern and text
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Configure RE2::Options from fuzzer input
    RE2::Options options;
    
    // Set encoding: UTF-8 (default) or Latin1
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                           : RE2::Options::EncodingUTF8);
    
    // Set various option flags from fuzzer input
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log errors during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB memory limit
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());  // Invert: default is true
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());

    // Consume anchor type
    RE2::Anchor anchor = fdp.PickValueInArray<RE2::Anchor>({
        RE2::UNANCHORED,
        RE2::ANCHOR_START,
        RE2::ANCHOR_BOTH,
    });

    // Consume pattern and text strings from remaining input
    // Split remaining input roughly equally for pattern and text
    size_t remaining = fdp.remaining_bytes();
    size_t pattern_size = remaining / 2;
    
    std::string pattern = fdp.ConsumeBytesAsString(pattern_size);
    std::string text = fdp.ConsumeRemainingBytesAsString();

    // If pattern is empty, use a simple default to ensure RE2 object creation
    if (pattern.empty()) {
        pattern = ".*";
    }

    // Create RE2 object with fuzzed pattern and options
    RE2 re(pattern, options);
    
    // Skip if pattern compilation failed
    if (!re.ok()) {
        return 0;
    }

    // Don't waste time on very complex patterns during fuzzing
    // Limit program size to prevent timeouts
    int program_size = re.ProgramSize();
    int reverse_program_size = re.ReverseProgramSize();
    if (program_size > 9999 || reverse_program_size > 9999) {
        return 0;
    }

    // Test 1: FullMatch - exact match of entire text
    RE2::FullMatch(text, re);

    // Test 2: PartialMatch - match substring anywhere in text
    RE2::PartialMatch(text, re);

    // Test 3: Consume - match from beginning and consume matched portion
    absl::string_view text_view = text;
    RE2::Consume(&text_view, re);

    // Test 4: FindAndConsume - find and consume all matches
    text_view = text;  // Reset
    RE2::FindAndConsume(&text_view, re);

    // Test 5: Replace - replace first match
    std::string replace_text = text;
    RE2::Replace(&replace_text, re, "X");

    // Test 6: GlobalReplace - replace all matches
    std::string global_replace_text = text;
    RE2::GlobalReplace(&global_replace_text, re, "Y");

    // Test 7: Extract matched values when there are capturing groups
    if (re.NumberOfCapturingGroups() > 0) {
        // Try to extract various types of captured values
        std::string str_match;
        int int_match;
        float float_match;
        double double_match;
        
        absl::string_view capture_view = text;
        
        // Try matching with different output types
        RE2::FullMatch(text, re, &str_match);
        RE2::PartialMatch(text, re, &int_match);
        RE2::Consume(&capture_view, re, &float_match);
        
        capture_view = text;  // Reset
        RE2::FindAndConsume(&capture_view, re, &double_match);
    }

    // Test 8: PossibleMatchRange - get possible match range
    std::string min_match, max_match;
    re.PossibleMatchRange(&min_match, &max_match, 100);

    // Test 9: RE2::Set - multiple pattern matching
    RE2::Set set(options, anchor);
    int set_index = set.Add(pattern, /*error=*/nullptr);
    if (set_index != -1 && set.Compile()) {
        std::vector<int> matches;
        set.Match(text, &matches);
    }

    // Test 10: FilteredRE2 - filtered multiple pattern matching
    re2::FilteredRE2 filter;
    int filter_index = -1;
    filter.Add(pattern, options, &filter_index);
    if (filter_index != -1) {
        std::vector<std::string> atoms;
        filter.Compile(&atoms);
        
        // Pretend all atoms match to exercise AND-OR tree
        std::vector<int> matched_atoms;
        matched_atoms.reserve(atoms.size());
        for (size_t i = 0; i < atoms.size(); ++i) {
            matched_atoms.push_back(static_cast<int>(i));
        }
        
        std::vector<int> filter_matches;
        filter.AllMatches(text, matched_atoms, &filter_matches);
    }

    // Test 11: QuoteMeta - escape regex metacharacters
    std::string quoted = RE2::QuoteMeta(pattern);

    // Test 12: Access pattern information
    std::string error_string = re.error();
    re.NamedCapturingGroups();
    re.CapturingGroupNames();

    return 0;
}

} // namespace harness_000
