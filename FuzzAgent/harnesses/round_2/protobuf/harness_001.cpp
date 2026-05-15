#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/port_def.inc"

namespace harness_001 {
// Harness for Google Protocol Buffers Tokenizer module
// Targets text parsing operations as identified by coverage analysis
// Semantic differentiation from harness_000.cpp: Focus on text parsing vs. binary message parsing


// Include protobuf headers for Tokenizer and related components

namespace google {
namespace protobuf {
namespace io {

// Custom ErrorCollector implementation for fuzzing
class FuzzingErrorCollector : public ::google::protobuf::io::ErrorCollector {
 public:
  FuzzingErrorCollector() = default;
  ~FuzzingErrorCollector() override = default;

  void RecordError(int line, ::google::protobuf::io::ColumnNumber column,
                   absl::string_view message) override {
    // Record errors for fuzzing feedback, but don't crash
    error_count_++;
    last_error_line_ = line;
    last_error_column_ = column;
    last_error_message_ = std::string(message);
  }

  void RecordWarning(int line, ::google::protobuf::io::ColumnNumber column,
                     absl::string_view message) override {
    // Record warnings for fuzzing feedback
    warning_count_++;
  }

  int error_count() const { return error_count_; }
  int warning_count() const { return warning_count_; }

 private:
  int error_count_ = 0;
  int warning_count_ = 0;
  int last_error_line_ = -1;
  ::google::protobuf::io::ColumnNumber last_error_column_ = -1;
  std::string last_error_message_;
};

}  // namespace io
}  // namespace protobuf
}  // namespace google

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need at least some data for meaningful testing
    if (size < 4) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which Tokenizer feature to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume tokenizer configuration options
    bool allow_f_after_float = fdp.ConsumeBool();
    bool report_whitespace = fdp.ConsumeBool();
    bool report_newlines = fdp.ConsumeBool();
    bool require_space_after_number = fdp.ConsumeBool();
    bool allow_multiline_strings = fdp.ConsumeBool();
    bool report_url_chars = fdp.ConsumeBool();
    
    // Determine comment style
    ::google::protobuf::io::Tokenizer::CommentStyle comment_style = 
        fdp.ConsumeBool() ? ::google::protobuf::io::Tokenizer::CPP_COMMENT_STYLE 
                         : ::google::protobuf::io::Tokenizer::SH_COMMENT_STYLE;
    
    // Consume text data for tokenizer input
    std::string text_data;
    if (fdp.remaining_bytes() > 0) {
        // Use remaining bytes as text input for tokenizer
        text_data = fdp.ConsumeRemainingBytesAsString();
    } else {
        return 0;
    }
    
    // Create ArrayInputStream from text data
    ::google::protobuf::io::ArrayInputStream input_stream(
        text_data.data(), static_cast<int>(text_data.size()));
    
    // Create error collector
    google::protobuf::io::FuzzingErrorCollector error_collector;
    
    // Create tokenizer with input stream and error collector
    ::google::protobuf::io::Tokenizer tokenizer(&input_stream, &error_collector);
    
    // Configure tokenizer based on fuzzer input
    tokenizer.set_allow_f_after_float(allow_f_after_float);
    tokenizer.set_comment_style(comment_style);
    tokenizer.set_require_space_after_number(require_space_after_number);
    tokenizer.set_allow_multiline_strings(allow_multiline_strings);
    tokenizer.set_report_whitespace(report_whitespace);
    tokenizer.set_report_newlines(report_newlines);
    tokenizer.set_report_url_chars(report_url_chars);
    
    // Variables for parsing results
    std::string parsed_string;
    std::string appended_string;
    double parsed_float = 0.0;
    double try_parsed_float = 0.0;
    uint64_t parsed_integer = 0;
    bool parse_integer_success = false;
    
    // Variables for NextWithComments
    std::string prev_trailing_comments;
    std::vector<std::string> detached_comments;
    std::string next_leading_comments;
    
    // Token counters
    int token_count = 0;
    const int max_tokens = 100; // Limit to prevent infinite loops
    
    switch (operation) {
        case 0: {
            // Test basic Next() tokenization
            while (tokenizer.Next() && token_count < max_tokens) {
                const auto& token = tokenizer.current();
                token_count++;
                
                // Based on token type, call appropriate parsing functions
                switch (token.type) {
                    case ::google::protobuf::io::Tokenizer::TYPE_STRING:
                        ::google::protobuf::io::Tokenizer::ParseString(token.text, &parsed_string);
                        ::google::protobuf::io::Tokenizer::ParseStringAppend(token.text, &appended_string);
                        break;
                        
                    case ::google::protobuf::io::Tokenizer::TYPE_FLOAT:
                        parsed_float = ::google::protobuf::io::Tokenizer::ParseFloat(token.text);
                        ::google::protobuf::io::Tokenizer::TryParseFloat(token.text, &try_parsed_float);
                        break;
                        
                    case ::google::protobuf::io::Tokenizer::TYPE_INTEGER:
                        parse_integer_success = ::google::protobuf::io::Tokenizer::ParseInteger(
                            token.text, UINT64_MAX, &parsed_integer);
                        break;
                        
                    default:
                        // For other token types, just count them
                        break;
                }
            }
            break;
        }
        
        case 1: {
            // Test NextWithComments() for comment parsing
            while (tokenizer.NextWithComments(&prev_trailing_comments, &detached_comments, 
                                             &next_leading_comments) && token_count < max_tokens) {
                const auto& token = tokenizer.current();
                token_count++;
                
                // Store comments for coverage (even if empty)
                std::string comment_storage = prev_trailing_comments + next_leading_comments;
                for (const auto& det : detached_comments) {
                    comment_storage += det;
                }
                
                // Parse based on token type
                if (token.type == ::google::protobuf::io::Tokenizer::TYPE_STRING) {
                    ::google::protobuf::io::Tokenizer::ParseString(token.text, &parsed_string);
                }
            }
            break;
        }
        
        case 2: {
            // Test string parsing functions extensively
            while (tokenizer.Next() && token_count < max_tokens) {
                const auto& token = tokenizer.current();
                token_count++;
                
                if (token.type == ::google::protobuf::io::Tokenizer::TYPE_STRING) {
                    // Test both ParseString and ParseStringAppend
                    parsed_string.clear();
                    appended_string.clear();
                    
                    ::google::protobuf::io::Tokenizer::ParseString(token.text, &parsed_string);
                    ::google::protobuf::io::Tokenizer::ParseStringAppend(token.text, &appended_string);
                    
                    // Verify ParseStringAppend actually appends
                    // Use fuzzer input for prefix if available
                    std::string test_string;
                    if (fdp.remaining_bytes() > 0) {
                        size_t prefix_len = std::min(fdp.remaining_bytes(), static_cast<size_t>(20));
                        test_string = fdp.ConsumeBytesAsString(prefix_len);
                    }
                    ::google::protobuf::io::Tokenizer::ParseStringAppend(token.text, &test_string);
                }
            }
            break;
        }
        
        case 3: {
            // Test numeric parsing functions
            while (tokenizer.Next() && token_count < max_tokens) {
                const auto& token = tokenizer.current();
                token_count++;
                
                if (token.type == ::google::protobuf::io::Tokenizer::TYPE_FLOAT) {
                    // Test both ParseFloat and TryParseFloat
                    parsed_float = ::google::protobuf::io::Tokenizer::ParseFloat(token.text);
                    
                    double temp_float;
                    bool try_success = ::google::protobuf::io::Tokenizer::TryParseFloat(token.text, &temp_float);
                    if (try_success) {
                        try_parsed_float = temp_float;
                    }
                } else if (token.type == ::google::protobuf::io::Tokenizer::TYPE_INTEGER) {
                    // Test ParseInteger with different max values
                    uint64_t temp_int;
                    
                    // Try with maximum value
                    parse_integer_success = ::google::protobuf::io::Tokenizer::ParseInteger(
                        token.text, UINT64_MAX, &temp_int);
                    
                    // Try with smaller max value
                    if (fdp.remaining_bytes() > 0) {
                        uint64_t small_max = fdp.ConsumeIntegralInRange<uint64_t>(1, 1000);
                        ::google::protobuf::io::Tokenizer::ParseInteger(token.text, small_max, &temp_int);
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Test tokenizer with URL chars mode
            // First pass with URL chars disabled
            while (tokenizer.Next() && token_count < max_tokens / 2) {
                token_count++;
            }
            
            // Recreate tokenizer with same input but different settings
            ::google::protobuf::io::ArrayInputStream input_stream2(
                text_data.data(), static_cast<int>(text_data.size()));
            google::protobuf::io::FuzzingErrorCollector error_collector2;
            ::google::protobuf::io::Tokenizer tokenizer2(&input_stream2, &error_collector2);
            
            // Enable URL chars reporting
            tokenizer2.set_report_url_chars(true);
            tokenizer2.set_report_whitespace(true);
            
            // Second pass with URL chars enabled
            int token_count2 = 0;
            while (tokenizer2.Next() && token_count2 < max_tokens / 2) {
                token_count2++;
            }
            break;
        }
        
        case 5: {
            // Test tokenizer state transitions and current()/previous() methods
            std::vector<::google::protobuf::io::Tokenizer::TokenType> token_types;
            
            while (tokenizer.Next() && token_count < max_tokens) {
                const auto& current_token = tokenizer.current();
                token_types.push_back(current_token.type);
                token_count++;
                
                // Access previous token after we have at least one
                if (token_count > 1) {
                    const auto& previous_token = tokenizer.previous();
                    // Just accessing for coverage
                }
                
                // Parse based on token type
                if (current_token.type == ::google::protobuf::io::Tokenizer::TYPE_STRING) {
                    ::google::protobuf::io::Tokenizer::ParseString(current_token.text, &parsed_string);
                }
            }
            break;
        }
        
        case 6: {
            // Test mixed operations with frequent configuration changes
            for (int i = 0; i < 5 && token_count < max_tokens; i++) {
                // Read a few tokens
                for (int j = 0; j < 5 && tokenizer.Next(); j++) {
                    token_count++;
                    const auto& token = tokenizer.current();
                    
                    // Sample parsing based on token type
                    if (token.type == ::google::protobuf::io::Tokenizer::TYPE_FLOAT) {
                        parsed_float = ::google::protobuf::io::Tokenizer::ParseFloat(token.text);
                    }
                }
                
                // Change a configuration setting
                if (i % 2 == 0) {
                    tokenizer.set_report_whitespace(!tokenizer.report_whitespace());
                } else {
                    tokenizer.set_report_newlines(!tokenizer.report_newlines());
                }
            }
            break;
        }
        
        case 7: {
            // Test edge cases and error conditions
            // Create tokenizer with minimal valid input
            if (text_data.size() > 0) {
                // Test with single character inputs
                std::string single_char = text_data.substr(0, 1);
                ::google::protobuf::io::ArrayInputStream single_stream(
                    single_char.data(), static_cast<int>(single_char.size()));
                google::protobuf::io::FuzzingErrorCollector single_error_collector;
                ::google::protobuf::io::Tokenizer single_tokenizer(&single_stream, &single_error_collector);
                
                // Try to tokenize single character
                while (single_tokenizer.Next() && token_count < 10) {
                    token_count++;
                }
                
                // Test IsIdentifier helper with fuzzer input
                // Use parts of the text_data for identifier testing
                if (text_data.size() > 5) {
                    // Test first few characters as potential identifier
                    std::string test_identifier = text_data.substr(0, std::min(text_data.size(), static_cast<size_t>(20)));
                    bool is_ident = ::google::protobuf::io::Tokenizer::IsIdentifier(test_identifier);
                    
                    // Also test with numeric prefix (should fail)
                    std::string numeric_test = "123" + test_identifier;
                    bool is_not_ident = ::google::protobuf::io::Tokenizer::IsIdentifier(numeric_test);
                }
            }
            break;
        }
        
        default:
            // Should not reach here due to modulo operation
            break;
    }
    
    // The error collector will have recorded any parsing errors/warnings
    // This provides feedback to the fuzzer about error paths taken
    
    return 0;
}

} // namespace harness_001
