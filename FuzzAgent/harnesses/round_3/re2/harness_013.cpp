#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include "re2/re2.h"

namespace harness_013 {
// Fuzzing harness for RE2 DFA matching engine execution through core RE2::Match API
// Primary target: Exercise 102+ blocked branches in RE2::Match and uncover 0%-covered DFA functions
// Coverage guidance: harness_013_guidance.md
// Semantic differentiation: Focuses on single-pattern DFA execution with comprehensive
// parameter variations, distinct from RE2::Set multi-pattern operations



// Helper to generate diverse regex patterns targeting different DFA execution paths
std::string GeneratePatternForDFA(FuzzedDataProvider& fdp, int pattern_type, bool use_latin1) {
    std::string pattern;
    
    switch (pattern_type % 20) {
        case 0: // Simple literal patterns for prefix matching
            pattern = "prefix" + fdp.ConsumeRandomLengthString(10);
            break;
            
        case 1: // Complex patterns with captures to exercise submatch handling
            pattern = "(" + fdp.ConsumeRandomLengthString(6) + 
                     ")(?:(" + fdp.ConsumeRandomLengthString(6) + 
                     "))?(" + fdp.ConsumeRandomLengthString(6) + ")";
            break;
            
        case 2: // Alternation patterns for DFA state exploration
            pattern = fdp.ConsumeRandomLengthString(4) + "|" + 
                     fdp.ConsumeRandomLengthString(4) + "|" + 
                     fdp.ConsumeRandomLengthString(4);
            break;
            
        case 3: // Quantifiers for DFA loop handling
            pattern = fdp.ConsumeRandomLengthString(3) + "+" + 
                     fdp.ConsumeRandomLengthString(3) + "*" + 
                     fdp.ConsumeRandomLengthString(3) + "?";
            break;
            
        case 4: // Character classes with ranges
            pattern = "[A-Za-z]+[0-9]{2,4}" + fdp.ConsumeRandomLengthString(6);
            break;
            
        case 5: // Dot patterns with newline considerations
            pattern = "." + fdp.ConsumeRandomLengthString(8) + 
                     ".*" + fdp.ConsumeRandomLengthString(8);
            break;
            
        case 6: // Word boundaries for anchor testing
            pattern = "\\b" + fdp.ConsumeRandomLengthString(8) + "\\b";
            break;
            
        case 7: // Complex repetition with explicit bounds
            pattern = fdp.ConsumeRandomLengthString(3) + "{" + 
                     std::to_string(fdp.ConsumeIntegralInRange<int>(0, 3)) + "," + 
                     std::to_string(fdp.ConsumeIntegralInRange<int>(4, 10)) + "}";
            break;
            
        case 8: // Lookahead assertions (one-pass matching)
            pattern = "(?=" + fdp.ConsumeRandomLengthString(5) + 
                     ")" + fdp.ConsumeRandomLengthString(8);
            break;
            
        case 9: // Lookbehind assertions (bit-state matching)
            pattern = "(?<=" + fdp.ConsumeRandomLengthString(5) + 
                     ")" + fdp.ConsumeRandomLengthString(8);
            break;
            
        case 10: // Mixed syntax with embedded flags
            pattern = "(?i)" + fdp.ConsumeRandomLengthString(8) + 
                     "(?-i)" + fdp.ConsumeRandomLengthString(8);
            break;
            
        case 11: // Simple patterns for fast DFA execution
            pattern = fdp.ConsumeRandomLengthString(12);
            break;
            
        case 12: // Empty and minimal patterns (edge cases)
            pattern = fdp.ConsumeBool() ? "" : "()";
            break;
            
        case 13: // Complex nested groups
            pattern = "(((" + fdp.ConsumeRandomLengthString(3) + 
                     "){2})|(" + fdp.ConsumeRandomLengthString(3) + 
                     ")+)*";
            break;
            
        case 14: // Unicode patterns (if not Latin1)
            if (!use_latin1) {
                pattern = "[\u0041-\u005A]+" + fdp.ConsumeRandomLengthString(6);
            } else {
                pattern = "[A-Z]+" + fdp.ConsumeRandomLengthString(6);
            }
            break;
            
        case 15: // Backslash escapes
            pattern = "\\d+\\s*\\w+" + fdp.ConsumeRandomLengthString(6);
            break;
            
        case 16: // Complex alternation with grouping
            pattern = "(" + fdp.ConsumeRandomLengthString(4) + 
                     "|" + fdp.ConsumeRandomLengthString(4) + 
                     "){1,3}";
            break;
            
        case 17: // Patterns with literal prefixes for prefix optimization
            pattern = "literal_" + fdp.ConsumeRandomLengthString(10) + ".*";
            break;
            
        case 18: // Minimal repetition
            pattern = fdp.ConsumeRandomLengthString(2) + "{0,1}";
            break;
            
        case 19: // Mixed quantifiers
            pattern = fdp.ConsumeRandomLengthString(3) + "??" + 
                     fdp.ConsumeRandomLengthString(3) + "+?" + 
                     fdp.ConsumeRandomLengthString(3) + "*?";
            break;
            
        default:
            pattern = ".*";
            break;
    }
    
    return pattern;
}

// Entry point for libFuzzer
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for pattern, text, and configuration
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Configure RE2::Options with comprehensive settings for DFA testing
    RE2::Options options;
    
    // Encoding: UTF-8 (default) or Latin1 (targets different Parse paths)
    bool use_latin1 = fdp.ConsumeBool();
    options.set_encoding(use_latin1 ? RE2::Options::EncodingLatin1 
                                    : RE2::Options::EncodingUTF8);
    
    // Set various option flags from fuzzer input
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());  // Critical for DFA mode selection
    options.set_log_errors(false);  // Don't log errors during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB memory limit for DFA
    
    // Options that affect DFA behavior
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());      // Never match newline
    options.set_dot_nl(fdp.ConsumeBool());        // Dot matches newline
    options.set_never_capture(fdp.ConsumeBool()); // All parens non-capturing
    options.set_case_sensitive(!fdp.ConsumeBool()); // Invert: default is true
    
    // POSIX syntax options
    if (options.posix_syntax()) {
        options.set_perl_classes(fdp.ConsumeBool());
        options.set_word_boundary(fdp.ConsumeBool());
        options.set_one_line(fdp.ConsumeBool());
    }
    
    // Step 2: Generate pattern targeting DFA execution paths
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 19);
    std::string pattern = GeneratePatternForDFA(fdp, pattern_type, use_latin1);
    
    // Ensure pattern is not empty for RE2 object creation
    if (pattern.empty()) {
        pattern = ".*";
    }
    
    // Step 3: Create RE2 object
    RE2 re(pattern, options);
    
    // Skip if pattern compilation failed
    if (!re.ok()) {
        return 0;
    }
    
    // Skip very complex patterns that might cause timeouts
    int program_size = re.ProgramSize();
    int reverse_program_size = re.ReverseProgramSize();
    if (program_size > 9999 || reverse_program_size > 9999) {
        return 0;
    }
    
    // Step 4: Prepare text for matching
    // Use remaining bytes for text, ensuring we have some input
    if (fdp.remaining_bytes() < 1) {
        return 0;
    }
    
    std::string text = fdp.ConsumeRemainingBytesAsString();
    
    // Step 5: Test RE2::Match with comprehensive parameter variations
    
    // 5.1 Test different anchor types
    RE2::Anchor anchors[] = {RE2::UNANCHORED, RE2::ANCHOR_START, RE2::ANCHOR_BOTH};
    RE2::Anchor anchor = anchors[fdp.ConsumeIntegralInRange<uint8_t>(0, 2) % 3];
    
    // 5.2 Test different startpos/endpos values (including edge cases)
    size_t text_len = text.length();
    size_t startpos = 0;
    size_t endpos = text_len;
    
    if (text_len > 0) {
        // Generate various start positions
        uint8_t startpos_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (startpos_choice) {
            case 0:
                startpos = 0;  // Beginning
                break;
            case 1:
                startpos = text_len / 2;  // Middle
                break;
            case 2:
                startpos = text_len - 1;  // Near end
                break;
            case 3:
                startpos = text_len;  // At end
                break;
            case 4:
                startpos = fdp.ConsumeIntegralInRange<size_t>(0, text_len * 2);  // Potentially out of bounds
                break;
        }
        
        // Generate various end positions
        uint8_t endpos_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (endpos_choice) {
            case 0:
                endpos = text_len;  // Full text
                break;
            case 1:
                endpos = text_len / 2;  // Half text
                break;
            case 2:
                endpos = 1;  // Very short
                break;
            case 3:
                endpos = 0;  // Empty
                break;
            case 4:
                endpos = fdp.ConsumeIntegralInRange<size_t>(0, text_len * 2);  // Potentially out of bounds
                break;
        }
        
        // Ensure endpos >= startpos for meaningful test
        if (endpos < startpos) {
            std::swap(startpos, endpos);
        }
    }
    
    // 5.3 Test different submatch counts
    int max_captures = re.NumberOfCapturingGroups();
    int nsubmatch = 0;
    
    if (max_captures >= 0) {
        uint8_t submatch_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (submatch_choice) {
            case 0:
                nsubmatch = 0;  // Fastest path
                break;
            case 1:
                nsubmatch = 1;  // Single submatch
                break;
            case 2:
                nsubmatch = std::min(max_captures + 1, 10);  // Multiple submatches
                break;
            case 3:
                nsubmatch = fdp.ConsumeIntegralInRange<int>(0, max_captures + 5);  // Possibly more than captures
                break;
        }
    }
    
    // 5.4 Prepare submatch array
    std::vector<absl::string_view> submatch_array;
    if (nsubmatch > 0) {
        submatch_array.resize(nsubmatch);
    }
    
    // 5.5 Call the core RE2::Match function - primary DFA entry point
    bool match_found = false;
    if (nsubmatch > 0) {
        match_found = re.Match(text, startpos, endpos, anchor, 
                              submatch_array.data(), nsubmatch);
    } else {
        match_found = re.Match(text, startpos, endpos, anchor, nullptr, 0);
    }
    
    // Step 6: Additional DFA-specific tests
    
    // 6.1 Test with prefix matching optimization
    // Patterns with literal prefixes trigger different DFA paths
    if (pattern.find("literal_") == 0) {
        // Re-test with ANCHOR_START to exercise prefix matching
        re.Match(text, 0, text_len, RE2::ANCHOR_START, nullptr, 0);
    }
    
    // 6.2 Test one-pass matching fallback
    // Certain patterns trigger one-pass instead of DFA
    if (pattern.find("(?=") != std::string::npos) {
        re.Match(text, 0, text_len, RE2::UNANCHORED, nullptr, 0);
    }
    
    // 6.3 Test bit-state matching fallback  
    // Other patterns trigger bit-state matching
    if (pattern.find("(?<=") != std::string::npos) {
        re.Match(text, 0, text_len, RE2::UNANCHORED, nullptr, 0);
    }
    
    // 6.4 Test empty text edge case
    if (text.empty()) {
        re.Match("", 0, 0, RE2::UNANCHORED, nullptr, 0);
    }
    
    // 6.5 Test with different anchor for same pattern
    if (match_found) {
        // Try opposite anchor to explore different DFA paths
        RE2::Anchor other_anchor = (anchor == RE2::UNANCHORED) ? RE2::ANCHOR_BOTH : RE2::UNANCHORED;
        re.Match(text, startpos, endpos, other_anchor, nullptr, 0);
    }
    
    // Step 7: Cleanup is automatic (RAII)
    
    return 0;
}

} // namespace harness_013
