#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_004 {

// Simple ErrorCollector implementation that counts errors and warnings
class CountingErrorCollector : public google::protobuf::io::ErrorCollector {
public:
  void RecordError(int line, google::protobuf::io::ColumnNumber column,
                   absl::string_view message) override {
    error_count_++;
  }
  
  void RecordWarning(int line, google::protobuf::io::ColumnNumber column,
                     absl::string_view message) override {
    warning_count_++;
  }
  
  int GetErrorCount() const { return error_count_; }
  int GetWarningCount() const { return warning_count_; }
  
private:
  int error_count_ = 0;
  int warning_count_ = 0;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Need minimum input for configuration
  if (size < 8) {
    return 0;
  }
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume configuration parameters first
  uint8_t config = fdp.ConsumeIntegral<uint8_t>();
  uint8_t comment_style = config & 0x01; // 0 = CPP, 1 = SH
  bool test_next_with_comments = (config & 0x02) != 0;
  bool test_basic_next = (config & 0x04) != 0;
  bool test_multiple_styles = (config & 0x08) != 0;
  
  // Use remaining bytes as the proto text to parse
  std::string proto_text = fdp.ConsumeRemainingBytesAsString();
  if (proto_text.empty()) {
    return 0;
  }
  
  // Create input stream - use ArrayInputStream
  google::protobuf::io::ArrayInputStream input_stream(
      proto_text.data(), static_cast<int>(proto_text.size()));
  
  // Create error collector
  CountingErrorCollector error_collector;
  
  // Create tokenizer
  google::protobuf::io::Tokenizer tokenizer(&input_stream, &error_collector);
  
  // Set comment style
  google::protobuf::io::Tokenizer::CommentStyle style;
  if (comment_style == 0) {
    style = google::protobuf::io::Tokenizer::CPP_COMMENT_STYLE;
  } else {
    style = google::protobuf::io::Tokenizer::SH_COMMENT_STYLE;
  }
  tokenizer.set_comment_style(style);
  
  // Test NextWithComments if configured
  if (test_next_with_comments) {
    int max_tokens = 100;  // Safety limit
    int token_count = 0;
    
    std::string prev_trailing_comments;
    std::vector<std::string> detached_comments;
    std::string next_leading_comments;
    
    while (token_count < max_tokens) {
      bool has_next = tokenizer.NextWithComments(
          &prev_trailing_comments, &detached_comments, &next_leading_comments);
      
      if (!has_next) {
        break;  // End of input
      }
      
      // Check if we got any comments
      if (!prev_trailing_comments.empty()) {
        // prev_trailing_comments contains comments from previous token
        // Do something with it to ensure it's processed
        size_t comment_length = prev_trailing_comments.length();
      }
      
      if (!detached_comments.empty()) {
        // detached_comments contains comments not attached to any token
        for (const auto& comment : detached_comments) {
          // Process detached comment
          size_t comment_length = comment.length();
        }
      }
      
      if (!next_leading_comments.empty()) {
        // next_leading_comments contains comments for next token
        size_t comment_length = next_leading_comments.length();
      }
      
      token_count++;
      
      // Clear comment containers for next iteration
      prev_trailing_comments.clear();
      detached_comments.clear();
      next_leading_comments.clear();
    }
  }
  
  // Test basic Next() method if configured
  if (test_basic_next) {
    // Recreate stream and tokenizer for fresh test
    google::protobuf::io::ArrayInputStream input_stream2(
        proto_text.data(), static_cast<int>(proto_text.size()));
    CountingErrorCollector error_collector2;
    google::protobuf::io::Tokenizer tokenizer2(&input_stream2, &error_collector2);
    
    tokenizer2.set_comment_style(style);
    
    int token_count = 0;
    while (token_count < 50) {
      bool has_next = tokenizer2.Next();
      if (!has_next) break;
      token_count++;
    }
  }
  
  // Test multiple comment styles if configured
  if (test_multiple_styles && proto_text.length() > 10) {
    // Test CPP style
    {
      google::protobuf::io::ArrayInputStream cpp_stream(
          proto_text.data(), static_cast<int>(proto_text.size()));
      CountingErrorCollector cpp_collector;
      google::protobuf::io::Tokenizer cpp_tokenizer(&cpp_stream, &cpp_collector);
      
      cpp_tokenizer.set_comment_style(google::protobuf::io::Tokenizer::CPP_COMMENT_STYLE);
      
      std::string ptc, nlc;
      std::vector<std::string> dc;
      
      for (int i = 0; i < 10; i++) {
        bool has_next = cpp_tokenizer.NextWithComments(&ptc, &dc, &nlc);
        if (!has_next) break;
      }
    }
    
    // Test SH style
    {
      google::protobuf::io::ArrayInputStream sh_stream(
          proto_text.data(), static_cast<int>(proto_text.size()));
      CountingErrorCollector sh_collector;
      google::protobuf::io::Tokenizer sh_tokenizer(&sh_stream, &sh_collector);
      
      sh_tokenizer.set_comment_style(google::protobuf::io::Tokenizer::SH_COMMENT_STYLE);
      
      std::string ptc, nlc;
      std::vector<std::string> dc;
      
      for (int i = 0; i < 10; i++) {
        bool has_next = sh_tokenizer.NextWithComments(&ptc, &dc, &nlc);
        if (!has_next) break;
      }
    }
  }
  
  return 0;
}

} // namespace harness_004
