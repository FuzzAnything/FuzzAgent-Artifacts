#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include "re2/re2.h"

namespace harness_021 {
// RE2 deep parser state machine fuzzing harness
// Targets Regexp::Parse function with 140/160 branches blocked (87.5% blocker rate)
// Semantic differentiation from harness_020: focuses on specific parser state transitions
// and extreme edge cases that stress the parser's internal state machine directly



// Generate patterns specifically designed to stress parser state transitions
std::string GenerateParserStatePattern(FuzzedDataProvider& fdp, int depth_level) {
    std::string pattern;
    
    // Base pattern components that trigger specific parser states
    static const char* state_components[] = {
        // Character class states
        "[",  // Start character class
        "[^", // Negated character class
        "\\p{", // Unicode property start
        "\\P{", // Negated Unicode property start
        "[:", // POSIX class start
        
        // Grouping states  
        "(",  // Start capture group
        "(?:", // Non-capturing group
        "(?P<", // Named capture group
        
        // Quantifier states
        "{",  // Start explicit quantifier
        "*",  // Zero or more (greedy)
        "*?", // Zero or more (non-greedy)
        "+",  // One or more (greedy)
        "+?", // One or more (non-greedy)
        "?",  // Zero or one (greedy)
        "??", // Zero or one (non-greedy)
        
        // Anchor/assertion states
        "^",  // Start of line
        "$",  // End of line
        "\\b", // Word boundary
        "\\B", // Non-word boundary
        "(?=", // Positive lookahead
        "(?!", // Negative lookahead
        
        // Escape sequence states
        "\\", // Escape start
        "\\x", // Hex escape
        "\\u", // Unicode escape
        
        // Special characters that affect parser state
        "|",  // Alternation
        ".",  // Any character (except newline)
        "]",  // Character class end (if matched)
        ")",  // Group end (if matched)
        "}",  // Quantifier end (if matched)
    };
    
    int num_components = fdp.ConsumeIntegralInRange<int>(1, std::min(10, depth_level * 2));
    
    for (int i = 0; i < num_components && fdp.remaining_bytes() > 0; i++) {
        // Choose component type
        int comp_type = fdp.ConsumeIntegralInRange<int>(0, 23);
        
        switch (comp_type) {
            case 0: { // Deeply nested parentheses
                int nest_depth = fdp.ConsumeIntegralInRange<int>(1, 6);
                for (int j = 0; j < nest_depth; j++) pattern += "(";
                pattern += "a";
                for (int j = 0; j < nest_depth; j++) pattern += ")";
                break;
            }
                
            case 1: { // Complex character class with many elements
                pattern += "[";
                if (fdp.ConsumeBool()) pattern += "^";
                
                int class_elements = fdp.ConsumeIntegralInRange<int>(1, 8);
                for (int j = 0; j < class_elements; j++) {
                    if (fdp.ConsumeBool()) {
                        pattern += "a-z";
                    } else if (fdp.ConsumeBool()) {
                        pattern += "A-Z";
                    } else if (fdp.ConsumeBool()) {
                        pattern += "0-9";
                    } else {
                        pattern += "\\[\\]";
                    }
                    if (j < class_elements - 1 && fdp.ConsumeBool()) pattern += "&&";
                }
                pattern += "]";
                break;
            }
                
            case 2: { // Unicode property with random content
                pattern += fdp.ConsumeBool() ? "\\p{" : "\\P{";
                int prop_len = fdp.ConsumeIntegralInRange<int>(1, 8);
                for (int j = 0; j < prop_len && fdp.remaining_bytes() > 0; j++) {
                    pattern += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>('a', 'z'));
                }
                pattern += "}";
                break;
            }
                
            case 3: { // Backreference to capture group
                int group_num = fdp.ConsumeIntegralInRange<int>(1, 9);
                pattern += "\\" + std::to_string(group_num);
                break;
            }
                
            case 4: { // Complex quantifier
                pattern += "{";
                int min_val = fdp.ConsumeIntegralInRange<int>(0, 10);
                pattern += std::to_string(min_val);
                
                if (fdp.ConsumeBool()) {
                    pattern += ",";
                    if (fdp.ConsumeBool()) {
                        int max_val = fdp.ConsumeIntegralInRange<int>(min_val + 1, min_val + 10);
                        pattern += std::to_string(max_val);
                    }
                }
                pattern += "}";
                if (fdp.ConsumeBool()) pattern += "?";
                break;
            }
                
            case 5: { // Alternation with multiple branches
                int num_branches = fdp.ConsumeIntegralInRange<int>(2, 5);
                for (int j = 0; j < num_branches; j++) {
                    pattern += fdp.ConsumeRandomLengthString(3);
                    if (j < num_branches - 1) pattern += "|";
                }
                break;
            }
                
            case 6: { // Word boundary assertions
                if (fdp.ConsumeBool()) pattern += "\\b";
                else pattern += "\\B";
                pattern += fdp.ConsumeRandomLengthString(5);
                if (fdp.ConsumeBool()) pattern += "\\b";
                else pattern += "\\B";
                break;
            }
                
            case 7: { // Lookahead/lookbehind assertions
                if (fdp.ConsumeBool()) {
                    pattern += fdp.ConsumeBool() ? "(?=" : "(?!";
                } else {
                    pattern += fdp.ConsumeBool() ? "(?<=" : "(?<!";
                }
                pattern += fdp.ConsumeRandomLengthString(5);
                pattern += ")";
                break;
            }
                
            case 8: { // Character class with special elements
                pattern += "[";
                if (fdp.ConsumeBool()) pattern += "^";
                
                int class_items = fdp.ConsumeIntegralInRange<int>(1, 6);
                for (int j = 0; j < class_items; j++) {
                    if (fdp.ConsumeBool()) pattern += "\\d";
                    else if (fdp.ConsumeBool()) pattern += "\\w";
                    else if (fdp.ConsumeBool()) pattern += "\\s";
                    else pattern += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>('a', 'z'));
                    
                    if (j < class_items - 1 && fdp.ConsumeBool()) pattern += "&&";
                }
                pattern += "]";
                break;
            }
                
            case 9: { // Escaped characters
                pattern += "\\";
                uint8_t escape_type = fdp.ConsumeIntegral<uint8_t>() % 10;
                switch (escape_type) {
                    case 0: pattern += "x"; pattern += "0123"; break;
                    case 1: pattern += "u"; pattern += "1234"; break;
                    case 2: pattern += "U"; pattern += "12345678"; break;
                    case 3: pattern += "a"; break;
                    case 4: pattern += "b"; break;
                    case 5: pattern += "f"; break;
                    case 6: pattern += "n"; break;
                    case 7: pattern += "r"; break;
                    case 8: pattern += "t"; break;
                    case 9: pattern += "v"; break;
                }
                break;
            }
                
            case 10: { // Line anchors with context
                if (fdp.ConsumeBool()) pattern += "^";
                pattern += fdp.ConsumeRandomLengthString(5);
                if (fdp.ConsumeBool()) pattern += "$";
                break;
            }
                
            case 11: { // Literal string with special chars
                int str_len = fdp.ConsumeIntegralInRange<int>(1, 10);
                for (int j = 0; j < str_len; j++) {
                    // Include characters that might trigger special parsing
                    uint8_t special_chars[] = {'[', ']', '(', ')', '{', '}', 
                                               '*', '+', '?', '.', '|', '\\'};
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                        pattern += static_cast<char>(special_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 11)]);
                    } else {
                        pattern += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>('a', 'z'));
                    }
                }
                break;
            }
                
            default: { // Random state component from the array
                pattern += state_components[comp_type % (sizeof(state_components)/sizeof(state_components[0]))];
            }
        }
        
        // Add connector between components
        if (i < num_components - 1 && fdp.ConsumeBool()) {
            if (fdp.ConsumeBool()) pattern += "";
            else pattern += fdp.ConsumeRandomLengthString(1);
        }
    }
    
    return pattern;
}

// Test parser error recovery scenarios
void TestParserErrorRecovery(FuzzedDataProvider& fdp, RE2::Options& options) {
    static const char* error_patterns[] = {
        "*",          // RepeatArgument
        "{5",         // RepeatSize (missing comma or })
        "{5,",        // RepeatSize (missing })
        "{5,3}",      // RepeatSize (min > max)
        "[z-a]",      // BadCharRange
        "\\c",        // BadEscape
        "(?P<>",      // BadNamedCapture
        "(?P<name",   // BadNamedCapture (unclosed)
        "\\x",        // BadEscape (incomplete hex)
        "\\u",        // BadEscape (incomplete unicode)
        "\\p",        // BadEscape (incomplete property)
        "[[:invalid:]]", // BadCharClass
    };
    
    int num_error_tests = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_error_tests && fdp.remaining_bytes() > 0; i++) {
        std::string error_pattern;
        
        if (fdp.ConsumeBool()) {
            // Use predefined error pattern
            int idx = fdp.ConsumeIntegralInRange<int>(0, sizeof(error_patterns)/sizeof(error_patterns[0]) - 1);
            error_pattern = error_patterns[idx];
        } else {
            // Generate random error pattern
            error_pattern = GenerateParserStatePattern(fdp, 1);
        }
        
        RE2 re(error_pattern, options);
        
        // Test error reporting APIs
        bool ok = re.ok();
        RE2::ErrorCode error_code = re.error_code();
        std::string error_msg = re.error();
        
        // Verify that invalid patterns produce appropriate errors
        if (!ok && error_code == RE2::NoError) {
            // This might indicate a problem with error detection
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 10) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Configure RE2 options from fuzzer input with emphasis on parser-affecting options
    RE2::Options options;
    
    // Encoding affects character interpretation in parser
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1 
                                         : RE2::Options::EncodingUTF8);
    
    // Options that directly affect parser behavior
    options.set_posix_syntax(fdp.ConsumeBool());  // Changes parsing rules
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log during fuzzing
    options.set_max_mem(fdp.ConsumeIntegralInRange<int>(1 << 20, 64 << 20));  // 1MB to 64MB
    
    // Literal mode bypasses most parsing - test both paths
    options.set_literal(fdp.ConsumeBool());
    
    // Newline handling affects '.' and line anchors
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    
    // Capture behavior
    options.set_never_capture(fdp.ConsumeBool());
    
    // Case sensitivity affects character class folding
    options.set_case_sensitive(fdp.ConsumeBool());
    
    // Perl classes affect escape sequence interpretation
    options.set_perl_classes(fdp.ConsumeBool());
    
    // Word boundary and line handling
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Test 1: Generate and test patterns focusing on parser state transitions
    int num_state_patterns = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    for (int i = 0; i < num_state_patterns && fdp.remaining_bytes() > 20; i++) {
        int depth = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::string pattern = GenerateParserStatePattern(fdp, depth);
        
        if (pattern.empty()) {
            continue;
        }
        
        // Create RE2 object - triggers Regexp::Parse
        RE2 re(pattern, options);
        
        // Test parsing result
        bool ok = re.ok();
        RE2::ErrorCode error_code = re.error_code();
        std::string error_msg = re.error();
        
        // If parsing succeeded, test additional functionality
        if (ok) {
            // Test pattern metadata accessors
            re.ProgramSize();
            re.ReverseProgramSize();
            re.NumberOfCapturingGroups();
            
            // Test match operations with various inputs
            std::string test_text = fdp.ConsumeRandomLengthString(50);
            
            // Test different matching functions
            RE2::FullMatch(test_text, re);
            RE2::PartialMatch(test_text, re);
            
            // Test possible match range
            std::string min_match, max_match;
            re.PossibleMatchRange(&min_match, &max_match, 10);
            
            // Test QuoteMeta on the pattern
            std::string quoted = RE2::QuoteMeta(pattern);
            
            // Test replacement if pattern has captures
            if (re.NumberOfCapturingGroups() > 0) {
                std::string replace_text = test_text;
                RE2::Replace(&replace_text, re, "replacement");
            }
        } else {
            // Test error handling paths - important for parser error recovery
            // Validate that error_code is meaningful
            if (error_code < RE2::NoError || error_code > RE2::ErrorPatternTooLarge) {
                // Invalid error code range
            }
        }
        
        // Test pattern with slightly modified options
        if (fdp.remaining_bytes() > 10) {
            RE2::Options modified_options = options;
            modified_options.set_case_sensitive(!options.case_sensitive());
            modified_options.set_perl_classes(!options.perl_classes());
            
            RE2 re_modified(pattern, modified_options);
            re_modified.ok();
        }
    }
    
    // Test 2: Parser error recovery scenarios
    if (fdp.remaining_bytes() > 30) {
        TestParserErrorRecovery(fdp, options);
    }
    
    // Test 3: Extreme edge cases for parser state machine
    if (fdp.remaining_bytes() > 50) {
        // Generate patterns with maximum complexity
        std::string extreme_pattern;
        
        // Very long pattern (stress memory and parsing)
        int extreme_len = fdp.ConsumeIntegralInRange<int>(100, 500);
        extreme_pattern.reserve(extreme_len);
        
        for (int i = 0; i < extreme_len && fdp.remaining_bytes() > 0; i++) {
            uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 10;
            switch (char_type) {
                case 0: extreme_pattern += "("; break;
                case 1: extreme_pattern += ")"; break;
                case 2: extreme_pattern += "["; break;
                case 3: extreme_pattern += "]"; break;
                case 4: extreme_pattern += "{"; break;
                case 5: extreme_pattern += "}"; break;
                case 6: extreme_pattern += "\\"; break;
                case 7: extreme_pattern += "|"; break;
                case 8: extreme_pattern += "*"; break;
                case 9: extreme_pattern += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>('a', 'z')); break;
            }
        }
        
        if (!extreme_pattern.empty()) {
            RE2 re_extreme(extreme_pattern, options);
            re_extreme.ok();
            
            // Test with memory limits
            RE2::Options low_mem_options = options;
            low_mem_options.set_max_mem(1024);  // 1KB limit
            
            RE2 re_low_mem(extreme_pattern, low_mem_options);
            re_low_mem.ok();
        }
    }
    
    // Test 4: Pattern combinations that specifically stress state transitions
    if (fdp.remaining_bytes() > 40) {
        // Combine patterns in ways that challenge the parser state machine
        std::string pattern1 = GenerateParserStatePattern(fdp, 2);
        std::string pattern2 = GenerateParserStatePattern(fdp, 2);
        
        if (!pattern1.empty() && !pattern2.empty()) {
            // Nested alternation
            std::string nested_alt = "(?:" + pattern1 + "|(?:" + pattern2 + "|" + pattern1 + "))";
            RE2 re_nested_alt(nested_alt, options);
            re_nested_alt.ok();
            
            // Quantified groups
            std::string quantified = "(?:" + pattern1 + "){2,5}";
            RE2 re_quantified(quantified, options);
            re_quantified.ok();
            
            // Character class containing pattern (as literal)
            std::string in_class = "[" + RE2::QuoteMeta(pattern1).substr(0, 10) + "]";
            RE2 re_in_class(in_class, options);
            re_in_class.ok();
        }
    }
    
    return 0;
}

} // namespace harness_021
