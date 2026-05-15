#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_003 {
// Copyright 2024 Fuzzing Harness Generator
// Fourth harness for RE2 library - targeting string manipulation functions with high undiscovered branch complexity
// Based on coverage guidance: RE2::Extract (176/262), GlobalReplace (198/292), Replace (178/268), Rewrite (29/30) undiscovered branches




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Require minimum input size for meaningful testing
    // Need enough for options, patterns, text, and rewrite strings
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume RE2::Options from fuzzer input
    RE2::Options options;
    options.set_encoding(fdp.ConsumeBool() ? RE2::Options::EncodingLatin1
                                          : RE2::Options::EncodingUTF8);
    options.set_posix_syntax(fdp.ConsumeBool());
    options.set_longest_match(fdp.ConsumeBool());
    options.set_log_errors(false);  // Don't log during fuzzing
    options.set_max_mem(64 << 20);  // 64 MB max memory
    options.set_literal(fdp.ConsumeBool());
    options.set_never_nl(fdp.ConsumeBool());
    options.set_dot_nl(fdp.ConsumeBool());
    options.set_never_capture(fdp.ConsumeBool());
    options.set_case_sensitive(!fdp.ConsumeBool());
    options.set_perl_classes(fdp.ConsumeBool());
    options.set_word_boundary(fdp.ConsumeBool());
    options.set_one_line(fdp.ConsumeBool());
    
    // Consume pattern string - try to create patterns with capture groups
    std::string pattern = fdp.ConsumeRandomLengthString(128);
    
    // If pattern is empty, try to get at least something
    if (pattern.empty() && fdp.remaining_bytes() > 0) {
        pattern = fdp.ConsumeRandomLengthString(1);
    }
    
    // Skip if pattern is still empty
    if (pattern.empty()) {
        return 0;
    }
    
    // Create RE2 object
    RE2 re(pattern, options);
    
    // If RE2 construction failed, return early
    if (!re.ok()) {
        return 0;
    }
    
    // Consume text to operate on
    std::string text = fdp.ConsumeRandomLengthString(512);
    
    // Test RE2::Extract - static function
    if (fdp.remaining_bytes() > 0) {
        std::string rewrite_for_extract = fdp.ConsumeRandomLengthString(64);
        std::string extracted;
        bool extract_result = RE2::Extract(text, re, rewrite_for_extract, &extracted);
        (void)extract_result;
        
        // Also test with empty rewrite string
        std::string empty_rewrite = "";
        std::string extracted_empty;
        RE2::Extract(text, re, empty_rewrite, &extracted_empty);
    }
    
    // Test RE2::Replace - static function
    if (fdp.remaining_bytes() > 0) {
        // Create a copy of text for replacement
        std::string text_for_replace = text;
        std::string rewrite_for_replace = fdp.ConsumeRandomLengthString(64);
        
        bool replace_result = RE2::Replace(&text_for_replace, re, rewrite_for_replace);
        (void)replace_result;
        
        // Test with empty rewrite
        std::string text_for_empty_replace = text;
        RE2::Replace(&text_for_empty_replace, re, "");
    }
    
    // Test RE2::GlobalReplace - static function
    if (fdp.remaining_bytes() > 0) {
        // Create a copy of text for global replacement
        std::string text_for_global = text;
        std::string rewrite_for_global = fdp.ConsumeRandomLengthString(64);
        
        int global_replace_count = RE2::GlobalReplace(&text_for_global, re, rewrite_for_global);
        (void)global_replace_count;
        
        // Test with empty rewrite
        std::string text_for_empty_global = text;
        RE2::GlobalReplace(&text_for_empty_global, re, "");
    }
    
    // Test RE2::Rewrite - member function
    // First need to get submatch vector by performing a match
    if (fdp.remaining_bytes() > 0) {
        // Determine how many submatches we need
        int max_submatch_needed = re.NumberOfCapturingGroups() + 1;
        if (max_submatch_needed > 0) {
            // Allocate vector for submatches
            absl::string_view* submatch_vec = new absl::string_view[max_submatch_needed];
            
            // Perform match to get submatches
            bool match_success = re.Match(text, 0, text.size(), RE2::UNANCHORED, 
                                         submatch_vec, max_submatch_needed);
            
            if (match_success) {
                // Consume rewrite string for Rewrite operation
                std::string rewrite_for_member = fdp.ConsumeRandomLengthString(64);
                std::string rewrite_output;
                
                // Test Rewrite member function
                bool rewrite_result = re.Rewrite(&rewrite_output, rewrite_for_member, 
                                                submatch_vec, max_submatch_needed);
                (void)rewrite_result;
                
                // Test with empty rewrite
                std::string empty_output;
                re.Rewrite(&empty_output, "", submatch_vec, max_submatch_needed);
                
                // Test with backreferences in rewrite string
                if (re.NumberOfCapturingGroups() >= 1) {
                    std::string backref_rewrite = "\\1";
                    std::string backref_output;
                    re.Rewrite(&backref_output, backref_rewrite, submatch_vec, max_submatch_needed);
                }
                
                if (re.NumberOfCapturingGroups() >= 2) {
                    std::string backref_rewrite2 = "\\2 and \\1";
                    std::string backref_output2;
                    re.Rewrite(&backref_output2, backref_rewrite2, submatch_vec, max_submatch_needed);
                }
            }
            
            delete[] submatch_vec;
        }
    }
    
    // Test CheckRewriteString helper
    if (fdp.remaining_bytes() > 0) {
        std::string rewrite_to_check = fdp.ConsumeRandomLengthString(64);
        std::string error_msg;
        bool check_result = re.CheckRewriteString(rewrite_to_check, &error_msg);
        (void)check_result;
        (void)error_msg;
    }
    
    // Test MaxSubmatch static function
    if (fdp.remaining_bytes() > 0) {
        std::string rewrite_for_max = fdp.ConsumeRandomLengthString(64);
        int max_submatch = RE2::MaxSubmatch(rewrite_for_max);
        (void)max_submatch;
    }
    
    // Additional testing with different patterns that have capture groups
    if (fdp.remaining_bytes() > 0) {
        // Try to create a pattern with explicit capture groups
        std::string capture_pattern;
        if (fdp.ConsumeBool()) {
            // Pattern with one capture group
            capture_pattern = "(\\w+)";
        } else {
            // Pattern with two capture groups
            capture_pattern = "(\\w+):(\\d+)";
        }
        
        RE2 re_with_captures(capture_pattern, options);
        if (re_with_captures.ok()) {
            // Test Extract with capture group pattern
            std::string rewrite_with_backref = fdp.ConsumeRandomLengthString(32);
            std::string extracted_with_captures;
            RE2::Extract(text, re_with_captures, rewrite_with_backref, &extracted_with_captures);
            
            // Test GlobalReplace with capture group pattern
            std::string text_for_capture_replace = text;
            std::string rewrite_for_capture = fdp.ConsumeRandomLengthString(32);
            RE2::GlobalReplace(&text_for_capture_replace, re_with_captures, rewrite_for_capture);
        }
    }
    
    // Test edge cases with empty text
    std::string empty_text = "";
    std::string dummy_output;
    RE2::Extract(empty_text, re, "test", &dummy_output);
    
    std::string empty_for_replace = "";
    RE2::Replace(&empty_for_replace, re, "test");
    
    std::string empty_for_global = "";
    RE2::GlobalReplace(&empty_for_global, re, "test");
    
    // Test with patterns that might not match
    if (fdp.remaining_bytes() > 0) {
        // Create a pattern that likely won't match
        std::string unlikely_pattern = fdp.ConsumeRandomLengthString(16);
        unlikely_pattern = "[A-Z][A-Z][A-Z]\\d\\d\\d";  // Pattern like "ABC123"
        
        RE2 unlikely_re(unlikely_pattern, options);
        if (unlikely_re.ok()) {
            std::string unlikely_rewrite = fdp.ConsumeRandomLengthString(32);
            std::string unlikely_extract;
            RE2::Extract(text, unlikely_re, unlikely_rewrite, &unlikely_extract);
            
            std::string unlikely_text = text;
            RE2::GlobalReplace(&unlikely_text, unlikely_re, unlikely_rewrite);
        }
    }
    
    // Cleanup happens automatically via destructors
    return 0;
}

} // namespace harness_003
