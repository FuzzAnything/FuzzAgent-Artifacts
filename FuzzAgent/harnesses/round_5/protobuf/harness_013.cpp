#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_013 {
// Protocol Buffers fuzzing harness for Tokenizer configuration and lifecycle operations
// This harness targets Tokenizer configuration APIs identified as having 0% coverage
// and TryParseFloat with high undiscovered complexity (22/32 branches)
//
// Coverage guidance targets:
// 1. Tokenizer constructor (_ZN6google8protobuf2io9TokenizerC1EPNS1_14ZeroCopyInputStreamEPNS1_14ErrorCollectorE)
// 2. Configuration APIs (all at 0% coverage):
//    - set_allow_multiline_strings
//    - set_require_space_after_number  
//    - set_report_newlines
//    - set_allow_f_after_float
//    - set_comment_style
//    - set_report_url_chars
// 3. TryParseFloat with 12.5% line coverage and 22/32 branches undiscovered
// 4. Tokenizer destructor



using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ErrorCollector;
using google::protobuf::io::Tokenizer;

// Simple error collector for Tokenizer (same as harness_001.cpp for consistency)
class SimpleErrorCollector : public ErrorCollector {
 public:
  SimpleErrorCollector() = default;
  ~SimpleErrorCollector() override = default;

  void RecordError(int line, int column, absl::string_view message) override {
    // Just ignore errors for fuzzing purposes
  }

  void RecordWarning(int line, int column, absl::string_view message) override {
    // Just ignore warnings for fuzzing purposes
  }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful tokenizer testing:
    // - Enough for ArrayInputStream data
    // - Enough for configuration bits
    // - Enough for test strings for TryParseFloat
    if (size < 64) return 0;  // Conservative minimum
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into multiple parts:
    // 1. Data for ArrayInputStream (tokenization source)
    // 2. Bits for configuration options (6 boolean flags + 1 enum)
    // 3. Strings for TryParseFloat testing
    
    // Step 1: Create ArrayInputStream with fuzz data
    size_t stream_data_size = fdp.ConsumeIntegralInRange<size_t>(32, std::min(size, (size_t)4096));
    std::vector<uint8_t> stream_data = fdp.ConsumeBytes<uint8_t>(stream_data_size);
    if (stream_data.empty()) return 0;
    
    int block_size = fdp.ConsumeIntegralInRange<int>(1, 256);
    ArrayInputStream array_stream(stream_data.data(), stream_data.size(), block_size);
    
    // Step 2: Create error collector
    SimpleErrorCollector error_collector;
    
    // Step 3: Create Tokenizer instance (constructor)
    Tokenizer tokenizer(&array_stream, &error_collector);
    
    // Step 4: Configure tokenizer using fuzzer input bits
    // Consume bits for boolean configuration options
    bool allow_multiline_strings = fdp.ConsumeBool();
    bool require_space_after_number = fdp.ConsumeBool();
    bool report_newlines = fdp.ConsumeBool();
    bool allow_f_after_float = fdp.ConsumeBool();
    bool report_url_chars = fdp.ConsumeBool();
    
    // Set configuration options
    tokenizer.set_allow_multiline_strings(allow_multiline_strings);
    tokenizer.set_require_space_after_number(require_space_after_number);
    tokenizer.set_report_newlines(report_newlines);
    tokenizer.set_allow_f_after_float(allow_f_after_float);
    tokenizer.set_report_url_chars(report_url_chars);
    
    // Set comment style (enum: CPP_COMMENT_STYLE or SH_COMMENT_STYLE)
    Tokenizer::CommentStyle comment_style = fdp.ConsumeBool() ? 
        Tokenizer::CPP_COMMENT_STYLE : Tokenizer::SH_COMMENT_STYLE;
    tokenizer.set_comment_style(comment_style);
    
    // Step 5: Test TryParseFloat with various strings from fuzzer input
    // According to the API documentation:
    // - ParseFloat should only be called with text from TYPE_FLOAT tokens parsed by Tokenizer
    // - TryParseFloat can be called with arbitrary text and returns false for invalid floats
    // Generate multiple test strings for TryParseFloat testing
    int num_test_strings = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int i = 0; i < num_test_strings && fdp.remaining_bytes() > 0; i++) {
        // Consume random length string for TryParseFloat testing
        size_t max_string_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string test_string = fdp.ConsumeRandomLengthString(max_string_len);
        
        if (!test_string.empty()) {
            // Test TryParseFloat (static method) for arbitrary strings
            // This is safe as TryParseFloat validates the input
            double try_result;
            bool try_success = Tokenizer::TryParseFloat(test_string, &try_result);
            (void)try_success;
            
            // Also test with string literals that might represent floats
            // Generate some additional float-like strings
            if (fdp.remaining_bytes() > 10) {
                // Test with numeric patterns
                std::string numeric_pattern;
                if (fdp.ConsumeBool()) {
                    numeric_pattern = fdp.ConsumeRandomLengthString(16);
                } else {
                    // Create float-like patterns
                    std::string int_part = std::to_string(fdp.ConsumeIntegral<int>());
                    std::string frac_part = fdp.ConsumeBool() ? "." + std::to_string(fdp.ConsumeIntegral<int>()) : "";
                    std::string exp_part = fdp.ConsumeBool() ? "e" + std::to_string(fdp.ConsumeIntegralInRange<int>(-10, 10)) : "";
                    numeric_pattern = int_part + frac_part + exp_part;
                }
                
                if (!numeric_pattern.empty()) {
                    // Only use TryParseFloat for arbitrary strings
                    double try_pattern_result;
                    bool try_pattern_success = Tokenizer::TryParseFloat(numeric_pattern, &try_pattern_result);
                    (void)try_pattern_success;
                }
            }
        }
    }
    
    // Step 6: Test tokenizer operations with the configured settings
    // Parse a few tokens to exercise the configuration
    int max_tokens_to_parse = fdp.ConsumeIntegralInRange<int>(0, 50);
    int tokens_parsed = 0;
    
    while (tokens_parsed < max_tokens_to_parse && tokenizer.Next()) {
        tokens_parsed++;
        
        Tokenizer::Token current_token = tokenizer.current();
        
        // Depending on token type, test appropriate parsing functions
        switch (current_token.type) {
            case Tokenizer::TYPE_FLOAT: {
                std::string token_text(current_token.text.data(), current_token.text.size());
                if (!token_text.empty()) {
                    // First validate with TryParseFloat to ensure it's a valid float
                    double try_result;
                    bool is_valid_float = Tokenizer::TryParseFloat(token_text, &try_result);
                    
                    // Only call ParseFloat if TryParseFloat indicates it's a valid float
                    // This ensures we respect the API contract for ParseFloat
                    if (is_valid_float) {
                        double result = Tokenizer::ParseFloat(token_text);
                        (void)result;
                    }
                }
                break;
            }
            
            case Tokenizer::TYPE_INTEGER: {
                // Test ParseInteger with TYPE_INTEGER tokens
                std::string token_text(current_token.text.data(), current_token.text.size());
                if (!token_text.empty() && fdp.remaining_bytes() > 0) {
                    uint64_t integer_result;
                    uint64_t max_value = fdp.ConsumeIntegral<uint64_t>();
                    bool parse_success = Tokenizer::ParseInteger(token_text, max_value, &integer_result);
                    (void)parse_success;
                }
                break;
            }
            
            case Tokenizer::TYPE_STRING: {
                // Test ParseString with TYPE_STRING tokens
                std::string token_text(current_token.text.data(), current_token.text.size());
                if (!token_text.empty()) {
                    std::string parsed_string;
                    Tokenizer::ParseString(token_text, &parsed_string);
                }
                break;
            }
            
            default:
                // For other token types, no special parsing needed
                break;
        }
        
        // Occasionally test NextWithComments to exercise comment parsing with configured style
        if (tokens_parsed % 7 == 0 && fdp.remaining_bytes() > 0) {
            std::string prev_trailing_comments;
            std::vector<std::string> detached_comments;
            std::string next_leading_comments;
            
            bool has_more = tokenizer.NextWithComments(&prev_trailing_comments,
                                                      &detached_comments,
                                                      &next_leading_comments);
            (void)has_more;
        }
    }
    
    // Step 7: Test additional configuration interactions
    // Test report_whitespace() getter (implied by report_newlines setting)
    if (report_newlines) {
        // report_newlines=true implies report_whitespace=true
        // We can't directly call report_whitespace() as it's not in public API
        // But the configuration is set internally
    }
    
    // Step 8: Test IsIdentifier static helper
    if (fdp.remaining_bytes() > 0) {
        std::string identifier_test = fdp.ConsumeRandomLengthString(32);
        bool is_identifier = Tokenizer::IsIdentifier(identifier_test);
        (void)is_identifier;
    }
    
    // Step 9: Cleanup (Tokenizer destructor called automatically when tokenizer goes out of scope)
    // ArrayInputStream destructor called automatically
    
    return 0;
}

} // namespace harness_013
