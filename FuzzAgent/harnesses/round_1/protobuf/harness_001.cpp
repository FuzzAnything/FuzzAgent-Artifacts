#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_001 {
class NullErrorCollector : public google::protobuf::io::ErrorCollector {
 public:
  void RecordError(int line, google::protobuf::io::ColumnNumber column,
                   absl::string_view message) override {
    // Ignore errors during fuzzing
  }
  
  void RecordWarning(int line, google::protobuf::io::ColumnNumber column,
                     absl::string_view message) override {
    // Ignore warnings during fuzzing
  }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0;  // Need minimum input for configuration
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters first
    uint8_t tokenizer_flags = fdp.ConsumeIntegral<uint8_t>();
    bool report_whitespace = (tokenizer_flags & 0x01) != 0;
    bool report_newlines = (tokenizer_flags & 0x02) != 0;
    bool allow_f_after_float = (tokenizer_flags & 0x04) != 0;
    uint8_t comment_style = fdp.ConsumeIntegral<uint8_t>() % 2;
    // Note: require_space_after_number and allow_multiline_strings are Google-internal
    // settings that we can't easily set via public API
    
    // Use remaining bytes as the text to tokenize
    std::string text_to_parse = fdp.ConsumeRemainingBytesAsString();
    if (text_to_parse.empty()) {
        return 0;  // Need some text to tokenize
    }
    
    // Create input stream from the text
    google::protobuf::io::ArrayInputStream input_stream(
        text_to_parse.data(), static_cast<int>(text_to_parse.size()));
    
    // Create error collector
    NullErrorCollector error_collector;
    
    // Create tokenizer
    google::protobuf::io::Tokenizer tokenizer(&input_stream, &error_collector);
    
    // Configure tokenizer based on fuzzed parameters
    tokenizer.set_report_whitespace(report_whitespace);
    tokenizer.set_report_newlines(report_newlines);
    tokenizer.set_allow_f_after_float(allow_f_after_float);
    
    // Set comment style (0=CPP_COMMENT_STYLE, 1=SH_COMMENT_STYLE)
    google::protobuf::io::Tokenizer::CommentStyle style;
    switch (comment_style) {
        case 0:
        default:
            style = google::protobuf::io::Tokenizer::CPP_COMMENT_STYLE;
            break;
        case 1:
            style = google::protobuf::io::Tokenizer::SH_COMMENT_STYLE;
            break;
    }
    tokenizer.set_comment_style(style);
    
    // Parse tokens in a loop
    int max_tokens = 1000;  // Safety limit to avoid infinite loops
    int token_count = 0;
    
    while (token_count < max_tokens) {
        // Randomly choose between Next() and NextWithComments()
        bool use_next_with_comments = fdp.ConsumeBool();
        
        if (use_next_with_comments && token_count > 0) {
            // Need containers for comments
            std::string prev_trailing_comments;
            std::vector<std::string> detached_comments;
            std::string next_leading_comments;
            
            bool has_next = tokenizer.NextWithComments(
                &prev_trailing_comments, &detached_comments, &next_leading_comments);
            
            if (!has_next) {
                break;  // End of input
            }
        } else {
            bool has_next = tokenizer.Next();
            if (!has_next) {
                break;  // End of input
            }
        }
        
        token_count++;
        
        // Get current token
        const google::protobuf::io::Tokenizer::Token& current = tokenizer.current();
        
        // Based on token type, call appropriate parsing functions
        switch (current.type) {
            case google::protobuf::io::Tokenizer::TYPE_STRING: {
                std::string parsed_string;
                google::protobuf::io::Tokenizer::ParseString(current.text, &parsed_string);
                
                // Also test ParseStringAppend
                std::string appended_string;
                google::protobuf::io::Tokenizer::ParseStringAppend(current.text, &appended_string);
                break;
            }
            
            case google::protobuf::io::Tokenizer::TYPE_FLOAT: {
                double float_value = google::protobuf::io::Tokenizer::ParseFloat(current.text);
                
                // Also test TryParseFloat
                double try_float_result;
                bool try_success = google::protobuf::io::Tokenizer::TryParseFloat(
                    current.text, &try_float_result);
                break;
            }
            
            case google::protobuf::io::Tokenizer::TYPE_INTEGER: {
                uint64_t integer_result;
                bool parse_success = google::protobuf::io::Tokenizer::ParseInteger(
                    current.text, 
                    std::numeric_limits<uint64_t>::max(), 
                    &integer_result);
                break;
            }
            
            case google::protobuf::io::Tokenizer::TYPE_IDENTIFIER:
            case google::protobuf::io::Tokenizer::TYPE_SYMBOL:
            case google::protobuf::io::Tokenizer::TYPE_WHITESPACE:
            case google::protobuf::io::Tokenizer::TYPE_NEWLINE:
            case google::protobuf::io::Tokenizer::TYPE_URL_CHARS:
            case google::protobuf::io::Tokenizer::TYPE_END:
            case google::protobuf::io::Tokenizer::TYPE_START:
            default:
                // Other token types don't have specific parsing functions
                break;
        }
        
        // Occasionally check previous token
        if (token_count > 1 && (token_count % 10 == 0)) {
            const google::protobuf::io::Tokenizer::Token& prev = tokenizer.previous();
            // Can inspect previous token if needed
        }
    }
    
    return 0;
}

} // namespace harness_001
