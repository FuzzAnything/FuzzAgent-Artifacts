#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include <sstream>
#include <limits>
#include <algorithm>

namespace harness_001 {
// Protocol Buffers fuzzing harness for Tokenizer parsing operations
// This harness targets Tokenizer APIs identified as having high coverage potential
// with 300+ undiscovered branches across 7 key Tokenizer parsing functions




using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ErrorCollector;
using google::protobuf::io::Tokenizer;

// Simple error collector for Tokenizer
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
    // Need minimum input for meaningful tokenization
    if (size < 32) return 0;  // Need enough for multiple parameters
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into multiple parts:
    // 1. For ArrayInputStream data
    // 2. For backup amount and other parameters
    // 3. For generating additional test strings
    
    // First, determine how much data to use for tokenization
    size_t tokenization_size = fdp.ConsumeIntegralInRange<size_t>(16, std::min(size, (size_t)4096));
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(tokenization_size);
    if (input_data.empty()) return 0;
    
    // Get block size from fuzzer input
    int block_size = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Create ArrayInputStream with the input data
    ArrayInputStream array_stream(input_data.data(), input_data.size(), block_size);
    
    // Test BackUp method on ArrayInputStream BEFORE passing it to Tokenizer
    // This avoids violating the stream ownership contract
    const void* buffer;
    int buffer_size;
    if (array_stream.Next(&buffer, &buffer_size)) {
        // Get backup amount from fuzzer input
        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
        array_stream.BackUp(backup_amount);
    }
    
    // Create error collector
    SimpleErrorCollector error_collector;
    
    // Create Tokenizer instance with the stream and error collector
    // After this point, Tokenizer manages the stream lifecycle
    Tokenizer tokenizer(&array_stream, &error_collector);
    
    // Track token types for different parsing operations
    int token_count = 0;
    const int max_tokens = 1000; // Safety limit
    
    // Call Tokenizer::Next() in a loop to parse tokens
    while (tokenizer.Next() && token_count < max_tokens) {
        token_count++;
        
        // Get current token
        Tokenizer::Token current_token = tokenizer.current();
        
        // Call appropriate parsing functions based on token type
        switch (current_token.type) {
            case Tokenizer::TYPE_FLOAT: {
                // Parse float token - only call ParseFloat with text from TYPE_FLOAT tokens
                std::string token_text(current_token.text.data(), current_token.text.size());
                if (!token_text.empty()) {
                    double float_result = Tokenizer::ParseFloat(token_text);
                    (void)float_result; // Use result to avoid unused variable warning
                    
                    // Also test TryParseFloat
                    double try_result;
                    bool try_success = Tokenizer::TryParseFloat(token_text, &try_result);
                    (void)try_success;
                }
                break;
            }
            
            case Tokenizer::TYPE_INTEGER: {
                // Parse integer token
                std::string token_text(current_token.text.data(), current_token.text.size());
                uint64_t integer_result;
                // Use max_value from fuzzer input
                uint64_t max_value = fdp.ConsumeIntegral<uint64_t>();
                bool parse_success = Tokenizer::ParseInteger(token_text, max_value, &integer_result);
                (void)parse_success;
                break;
            }
            
            case Tokenizer::TYPE_STRING: {
                // Parse string token (both quoted and unquoted strings)
                std::string token_text(current_token.text.data(), current_token.text.size());
                
                // According to API contract and crash analysis, ParseString/ParseStringAppend
                // should not be called with empty strings, even if they come from TYPE_STRING tokens
                if (!token_text.empty()) {
                    std::string parsed_string;
                    
                    // Test ParseString
                    Tokenizer::ParseString(token_text, &parsed_string);
                    
                    // Test ParseStringAppend with empty target
                    std::string append_target;
                    Tokenizer::ParseStringAppend(token_text, &append_target);
                    
                    // Test ParseStringAppend with non-empty target
                    std::string non_empty_target = fdp.ConsumeRandomLengthString(32);
                    Tokenizer::ParseStringAppend(token_text, &non_empty_target);
                }
                break;
                }
            
            case Tokenizer::TYPE_IDENTIFIER: {
                // Identifiers are not strings, so don't call ParseString on them
                // According to API contract, ParseString should only be called
                // with text from TYPE_STRING tokens parsed by Tokenizer
                break;
            }
            
            default:
                // For other token types (SYMBOL, START, END, etc.), no special parsing needed
                break;
        }
        
        // Occasionally test NextWithComments
        if (token_count % 10 == 0 && fdp.remaining_bytes() > 0) {
            std::string prev_trailing_comments;
            std::vector<std::string> detached_comments;
            std::string next_leading_comments;
            
            // Test NextWithComments - this consumes the next token
            bool has_more = tokenizer.NextWithComments(&prev_trailing_comments, 
                                                      &detached_comments, 
                                                      &next_leading_comments);
            
            if (has_more) {
                // Process the token that was consumed by NextWithComments
                Tokenizer::Token comment_token = tokenizer.current();
                std::string comment_token_text(comment_token.text.data(), 
                                              comment_token.text.size());
                
                // Parse based on token type
                switch (comment_token.type) {
                    case Tokenizer::TYPE_FLOAT: {
                        double result = Tokenizer::ParseFloat(comment_token_text);
                        (void)result;
                        break;
                    }
                    case Tokenizer::TYPE_INTEGER: {
                        uint64_t result;
                        uint64_t max_val = fdp.ConsumeIntegral<uint64_t>();
                        Tokenizer::ParseInteger(comment_token_text, max_val, &result);
                        break;
                    }
                    case Tokenizer::TYPE_STRING: {
                        // Check if token text is non-empty before calling ParseString
                        if (!comment_token_text.empty()) {
                            std::string parsed;
                            Tokenizer::ParseString(comment_token_text, &parsed);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        
        // Break early if we're running out of fuzzer input for parameters
        if (fdp.remaining_bytes() < sizeof(uint64_t)) {
            break;
        }
    }
    
    // Test additional edge cases after main parsing loop
    // Use remaining fuzzer input to generate test cases
    
    // Test ParseFloat with various inputs generated from remaining fuzzer data
    // Note: For arbitrary strings not from TYPE_FLOAT tokens, we must only use TryParseFloat
    // as per API documentation: ParseFloat should only be called with text from TYPE_FLOAT tokens
    while (fdp.remaining_bytes() > 10) {
        // Generate float test strings from fuzzer data
        std::string float_str;
        
        // Choose pattern based on fuzzer input
        uint8_t pattern = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (pattern) {
            case 0: {
                // Simple decimal
                int whole = fdp.ConsumeIntegralInRange<int>(0, 10000);
                int frac = fdp.ConsumeIntegralInRange<int>(0, 9999);
                float_str = std::to_string(whole) + "." + std::to_string(frac);
                break;
            }
            case 1: {
                // Scientific notation
                int whole = fdp.ConsumeIntegralInRange<int>(0, 100);
                int frac = fdp.ConsumeIntegralInRange<int>(0, 9);
                int exp = fdp.ConsumeIntegralInRange<int>(0, 99);
                bool positive = fdp.ConsumeBool();
                float_str = std::to_string(whole) + "." + std::to_string(frac) + "e" +
                           (positive ? "+" : "-") + std::to_string(exp);
                break;
            }
            case 2: {
                // Special values: generate from fuzzer input
                uint8_t special = fdp.ConsumeIntegral<uint8_t>() % 3;
                if (special == 0) float_str = "inf";
                else if (special == 1) float_str = "-inf";
                else float_str = "nan";
                break;
            }
            case 3: {
                // Generate valid float literal for TryParseFloat testing
                // Convert integer to float literal by adding ".0"
                float_str = std::to_string(fdp.ConsumeIntegral<int>()) + ".0";
                break;
            }
        }
        
        // For arbitrary strings not from TYPE_FLOAT tokens, only use TryParseFloat
        double try_result;
        bool try_success = Tokenizer::TryParseFloat(float_str, &try_result);
        (void)try_success;
    }
    
    // Test ParseInteger with various bases from remaining fuzzer input
    while (fdp.remaining_bytes() > 5) {
        std::string int_str;
        uint8_t base_pattern = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (base_pattern) {
            case 0: {
                // Decimal
                int value = fdp.ConsumeIntegralInRange<int>(0, 1000000);
                int_str = std::to_string(value);
                break;
            }
            case 1: {
                // Hexadecimal
                int value = fdp.ConsumeIntegralInRange<int>(0, 65535);
                std::stringstream hex_ss;
                hex_ss << "0x" << std::hex << value;
                int_str = hex_ss.str();
                break;
            }
            case 2: {
                // Octal (small values to avoid invalid octal)
                int value = fdp.ConsumeIntegralInRange<int>(0, 7);
                std::stringstream oct_ss;
                oct_ss << "0" << std::oct << value;
                int_str = oct_ss.str();
                break;
            }
            case 3: {
                // Binary
                std::string binary_str = "0b";
                size_t binary_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                for (size_t i = 0; i < binary_len && fdp.remaining_bytes() > 0; i++) {
                    binary_str += fdp.ConsumeBool() ? '1' : '0';
                }
                int_str = binary_str;
                break;
            }
        }
        
        uint64_t int_result;
        uint64_t max_value = fdp.ConsumeIntegral<uint64_t>();
        bool parse_success = Tokenizer::ParseInteger(int_str, max_value, &int_result);
        (void)parse_success;
        
        // Break if we don't have enough data for next iteration
        if (fdp.remaining_bytes() < sizeof(uint64_t) + 5) {
            break;
        }
    }
    
    // Tokenizer destructor is called automatically when tokenizer goes out of scope
    // ArrayInputStream destructor is also called automatically
    
    return 0;
}

} // namespace harness_001
