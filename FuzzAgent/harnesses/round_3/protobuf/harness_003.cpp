#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_003 {
// Protocol Buffers fuzzing harness - Tokenizer text parsing operations
// Targets: Tokenizer::NextWithComments, ParseFloat, ParseString, ParseInteger, TryParseFloat
// Uses FuzzedDataProvider for structured input splitting



// Simple ErrorCollector implementation for fuzzing
class FuzzingErrorCollector : public google::protobuf::io::ErrorCollector {
public:
    void RecordError(int line, google::protobuf::io::ColumnNumber column,
                     absl::string_view message) override {
        // Just ignore errors for fuzzing - we want to test error paths too
        (void)line;
        (void)column;
        (void)message;
    }
    
    void RecordWarning(int line, google::protobuf::io::ColumnNumber column,
                       absl::string_view message) override {
        // Ignore warnings too
        (void)line;
        (void)column;
        (void)message;
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful tokenization
    if (size < 8) return 0; // Increased minimum for configuration bytes
    
    FuzzedDataProvider fdp(data, size);
    
    // Configure tokenizer options based on fuzzed data before consuming input text
    bool allow_f_after_float = fdp.ConsumeBool();
    bool use_cpp_comment_style = fdp.ConsumeBool();
    bool require_space_after_number = fdp.ConsumeBool();
    bool allow_multiline_strings = fdp.ConsumeBool();
    
    // Step 1: Create ArrayInputStream with fuzzer-provided data
    // Use remaining data as input text for tokenizer
    std::string input_text = fdp.ConsumeRemainingBytesAsString();
    if (input_text.empty()) return 0;
    
    google::protobuf::io::ArrayInputStream array_stream(input_text.data(), 
                                                       static_cast<int>(input_text.size()));
    
    // Step 2: Implement ErrorCollector to capture errors
    FuzzingErrorCollector error_collector;
    
    // Step 3: Instantiate Tokenizer with ArrayInputStream and ErrorCollector
    google::protobuf::io::Tokenizer tokenizer(&array_stream, &error_collector);
    
    // Apply configuration
    tokenizer.set_allow_f_after_float(allow_f_after_float);
    tokenizer.set_comment_style(use_cpp_comment_style ? 
        google::protobuf::io::Tokenizer::CPP_COMMENT_STYLE : 
        google::protobuf::io::Tokenizer::SH_COMMENT_STYLE);
    tokenizer.set_require_space_after_number(require_space_after_number);
    tokenizer.set_allow_multiline_strings(allow_multiline_strings);
    
    // Variables for NextWithComments
    std::string prev_trailing_comments;
    std::vector<std::string> detached_comments;
    std::string next_leading_comments;
    
    // Step 4-7: Loop through tokens, parsing them with various methods
    int max_iterations = 100; // Safety limit
    int iteration = 0;
    
    while (iteration++ < max_iterations) {
        // Step 4: Call NextWithComments to parse tokens with comment tracking
        bool has_next = tokenizer.NextWithComments(&prev_trailing_comments, 
                                                  &detached_comments, 
                                                  &next_leading_comments);
        
        if (!has_next) {
            break; // End of input
        }
        
        // Get current token info
        google::protobuf::io::Tokenizer::TokenType token_type = tokenizer.current().type;
        std::string token_text = tokenizer.current().text;
        
        // Step 5: Extract and parse numeric tokens with ParseFloat, ParseInteger, TryParseFloat
        if (token_type == google::protobuf::io::Tokenizer::TYPE_FLOAT) {
            // Test ParseFloat - should work for valid float tokens
            double float_value = google::protobuf::io::Tokenizer::ParseFloat(token_text);
            (void)float_value;
            
            // Test TryParseFloat - should return true for valid float tokens
            double try_float_result;
            bool try_float_success = google::protobuf::io::Tokenizer::TryParseFloat(
                token_text, &try_float_result);
            (void)try_float_success;
            (void)try_float_result;
        }
        
        if (token_type == google::protobuf::io::Tokenizer::TYPE_INTEGER) {
            // Test ParseInteger with various max values
            uint64_t int_result;
            uint64_t max_value = std::numeric_limits<uint64_t>::max();
            
            // Try with different max values to test boundary conditions
            bool parse_int_success = google::protobuf::io::Tokenizer::ParseInteger(
                token_text, max_value, &int_result);
            (void)parse_int_success;
            (void)int_result;
            
            // Also test with smaller max values
            if (token_text.length() < 10) { // Avoid overflow in conversion
                uint64_t smaller_max = 1000;
                bool parse_int_small_success = google::protobuf::io::Tokenizer::ParseInteger(
                    token_text, smaller_max, &int_result);
                (void)parse_int_small_success;
            }
        }
        
        // Step 6: Extract and parse string tokens with ParseString
        if (token_type == google::protobuf::io::Tokenizer::TYPE_STRING) {
            std::string parsed_string;
            
            // Test ParseString
            google::protobuf::io::Tokenizer::ParseString(token_text, &parsed_string);
            (void)parsed_string;
            
            // Test ParseStringAppend
            std::string appended_string;
            google::protobuf::io::Tokenizer::ParseStringAppend(token_text, &appended_string);
            (void)appended_string;
        }
        
        // Also test ParseFloat and TryParseFloat on non-float tokens
        // (should still attempt to parse, may fail or return undefined values)
        if (token_type != google::protobuf::io::Tokenizer::TYPE_FLOAT) {
            double try_float_any;
            bool try_any_success = google::protobuf::io::Tokenizer::TryParseFloat(
                token_text, &try_float_any);
            (void)try_any_success;
            (void)try_float_any;
        }
        
        // Test ParseInteger on non-integer tokens
        if (token_type != google::protobuf::io::Tokenizer::TYPE_INTEGER) {
            uint64_t int_result;
            uint64_t max_value = std::numeric_limits<uint64_t>::max();
            bool parse_int_any = google::protobuf::io::Tokenizer::ParseInteger(
                token_text, max_value, &int_result);
            (void)parse_int_any;
            (void)int_result;
        }
        
        // Clear comment vectors for next iteration
        prev_trailing_comments.clear();
        detached_comments.clear();
        next_leading_comments.clear();
    }
    
    // Step 8: Clean up is automatic (RAII)
    // Tokenizer, ArrayInputStream, and ErrorCollector are destroyed automatically
    
    return 0;
}

} // namespace harness_003
