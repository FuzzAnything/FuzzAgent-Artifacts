#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream.h"

namespace harness_001 {
// Fuzzing harness for Google Protocol Buffers Tokenizer parsing operations
// Targets Tokenizer API functions: NextWithComments, Next, ParseStringAppend, ParseString, ParseFloat, ParseInteger, TryParseFloat
// Focuses on text format parsing rather than binary message operations (differentiates from harness_000.cpp)



// Tokenizer headers

using google::protobuf::io::Tokenizer;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ZeroCopyInputStream;

// Simple error collector for tokenizer
class TestErrorCollector : public google::protobuf::io::ErrorCollector {
public:
    TestErrorCollector() = default;
    ~TestErrorCollector() override = default;
    
    void RecordError(int line, google::protobuf::io::ColumnNumber column,
                     absl::string_view message) override {
        // Record errors but don't fail - just track them for fuzzing
        error_count_++;
        last_error_line_ = line;
        last_error_column_ = column;
        last_error_message_ = std::string(message);
    }
    
    void RecordWarning(int line, google::protobuf::io::ColumnNumber column,
                       absl::string_view message) override {
        warning_count_++;
    }
    
    int error_count_ = 0;
    int warning_count_ = 0;
    int last_error_line_ = 0;
    google::protobuf::io::ColumnNumber last_error_column_ = 0;
    std::string last_error_message_;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful tokenizer testing
    if (size < 16) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_comments = fdp.ConsumeBool();
    bool report_whitespace = fdp.ConsumeBool();
    bool allow_multiline_strings = fdp.ConsumeBool();
    
    // Determine how much input to use for the tokenizer
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, size - 8);
    std::string tokenizer_input = fdp.ConsumeBytesAsString(input_size);
    
    // Create ArrayInputStream with the fuzzed input
    ArrayInputStream array_stream(tokenizer_input.data(), tokenizer_input.size());
    
    // Create error collector
    TestErrorCollector error_collector;
    
    // Create tokenizer
    Tokenizer tokenizer(&array_stream, &error_collector);
    
    // Configure tokenizer based on fuzzed parameters
    tokenizer.set_allow_multiline_strings(allow_multiline_strings);
    tokenizer.set_report_whitespace(report_whitespace);
    if (report_whitespace) {
        tokenizer.set_report_newlines(fdp.ConsumeBool());
    }
    
    // Choose comment style based on fuzzed input
    if (fdp.ConsumeBool()) {
        tokenizer.set_comment_style(Tokenizer::CPP_COMMENT_STYLE);
    } else {
        tokenizer.set_comment_style(Tokenizer::SH_COMMENT_STYLE);
    }
    
    // Track tokens for parsing operations
    std::vector<std::string> token_texts;
    std::vector<Tokenizer::TokenType> token_types;
    
    // Parse tokens from input
    int max_tokens = fdp.ConsumeIntegralInRange<int>(1, 100);
    int tokens_parsed = 0;
    
    while (tokens_parsed < max_tokens) {
        bool has_next = false;
        
        if (use_comments) {
            std::string prev_trailing_comments;
            std::vector<std::string> detached_comments;
            std::string next_leading_comments;
            
            has_next = tokenizer.NextWithComments(
                &prev_trailing_comments, &detached_comments, &next_leading_comments);
            
            // Store comments for coverage (even if empty)
            size_t total_comment_chars = prev_trailing_comments.size() + 
                                         next_leading_comments.size();
            for (const auto& comment : detached_comments) {
                total_comment_chars += comment.size();
            }
        } else {
            // Use regular Next for other tokens
            has_next = tokenizer.Next();
        }
        
        if (!has_next) {
            break;  // End of input
        }
        
        const auto& token = tokenizer.current();
        token_texts.push_back(token.text);
        token_types.push_back(token.type);
        
        tokens_parsed++;
        
        // Break early if we run out of fuzzer input
        if (fdp.remaining_bytes() < 4) {
            break;
        }
    }
    
    // Now test parsing functions on the collected tokens
    for (size_t i = 0; i < token_texts.size(); i++) {
        const std::string& text = token_texts[i];
        Tokenizer::TokenType type = token_types[i];
        
        switch (test_mode) {
            case 0: {
                // Test ParseString and ParseStringAppend
                std::string parsed_string;
                std::string appended_string;
                
                Tokenizer::ParseString(text, &parsed_string);
                Tokenizer::ParseStringAppend(text, &appended_string);
                
                // Also test appending to existing string
                std::string base_string = "prefix_";
                Tokenizer::ParseStringAppend(text, &base_string);
                break;
            }
            
            case 1: {
                // Test ParseFloat and TryParseFloat
                if (type == Tokenizer::TYPE_FLOAT) {
                    // Only call ParseFloat on TYPE_FLOAT tokens (API contract requirement)
                    double float_result = Tokenizer::ParseFloat(text);
                    
                    double try_float_result;
                    bool try_success = Tokenizer::TryParseFloat(text, &try_float_result);
                    
                    // Use results to avoid optimization
                    if (try_success) {
                        volatile double dummy = float_result + try_float_result;
                        (void)dummy;
                    }
                } else {
                    // For non-float tokens, only use TryParseFloat to validate text
                    double try_float_result;
                    bool try_success = Tokenizer::TryParseFloat(text, &try_float_result);
                    
                    // Use result to avoid optimization
                    if (try_success) {
                        volatile double dummy = try_float_result;
                        (void)dummy;
                    }
                }
                break;
            }
            
            case 2: {
                // Test ParseInteger
                if (type == Tokenizer::TYPE_INTEGER) {
                    // Only call ParseInteger on TYPE_INTEGER tokens (API contract requirement)
                    uint64_t max_value = fdp.ConsumeIntegral<uint64_t>();
                    uint64_t integer_result;
                    
                    bool parse_success = Tokenizer::ParseInteger(text, max_value, &integer_result);
                    
                    if (parse_success) {
                        volatile uint64_t dummy = integer_result;
                        (void)dummy;
                    }
                } else {
                    // For non-integer tokens, don't call ParseInteger (would violate API contract)
                    // Just consume a value to maintain input consistency
                    uint64_t dummy_value = fdp.ConsumeIntegral<uint64_t>();
                    (void)dummy_value;
                }
                break;
            }
            case 3: {
                // Mixed testing of all parse functions - respecting API contracts
                std::string parsed_str;
                double float_val;
                uint64_t int_val;
                
                // Only call ParseString on TYPE_STRING tokens
                if (type == Tokenizer::TYPE_STRING) {
                    Tokenizer::ParseString(text, &parsed_str);
                }
                
                // TryParseFloat can be called on any text
                bool try_float_success = Tokenizer::TryParseFloat(text, &float_val);
                if (try_float_success) {
                    volatile double dummy = float_val;
                    (void)dummy;
                }
                
                // Only call ParseInteger on TYPE_INTEGER tokens
                if (type == Tokenizer::TYPE_INTEGER) {
                    bool parse_int_success = Tokenizer::ParseInteger(
                        text, std::numeric_limits<uint64_t>::max(), &int_val);
                    if (parse_int_success) {
                        volatile uint64_t dummy = int_val;
                        (void)dummy;
                    }
                }
                
                // Test ParseStringAppend on TYPE_STRING tokens
                if (type == Tokenizer::TYPE_STRING) {
                    std::string append_str = "prefix_";
                    Tokenizer::ParseStringAppend(text, &append_str);
                }
                break;
            }
        }
        
        // Break if we run out of fuzzer input
        if (fdp.remaining_bytes() < 4 && i > token_texts.size() / 2) {
            break;
        }
    }
    
    // Test edge cases with specific strings from remaining fuzzer input
    if (fdp.remaining_bytes() > 0) {
        std::string edge_case_string = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        
        if (!edge_case_string.empty()) {
            // IMPORTANT: For arbitrary text (not validated tokens), we can only use TryParseFloat
            // ParseString and ParseInteger require text from TYPE_STRING and TYPE_INTEGER tokens respectively
            // Calling them on arbitrary text violates API contracts and causes assertion failures
            
            // Only use TryParseFloat for arbitrary text validation
            double float_output;
            bool try_success = Tokenizer::TryParseFloat(edge_case_string, &float_output);
            
            if (try_success) {
                volatile double dummy = float_output;
                (void)dummy;
            }
            
            // Note: We cannot call ParseString or ParseInteger here because:
            // 1. ParseString requires text from TYPE_STRING tokens (API contract)
            // 2. ParseInteger requires text from TYPE_INTEGER tokens (API contract)
            // 3. edge_case_string is arbitrary fuzzer input, not validated tokens
            
            // However, we can test TryParseFloat on the arbitrary text since it's designed
            // to handle any input and return false for invalid float literals
        }
    }
    
    return 0;  // Always return 0 for libFuzzer
}

} // namespace harness_001
