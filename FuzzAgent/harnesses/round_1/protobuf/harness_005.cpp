#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "google/protobuf/symbol_checker.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/compiler/parser.h"
#include "google/protobuf/text_format.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_005 {

// Simple ErrorCollector implementation for parsing
class SimpleErrorCollector : public google::protobuf::io::ErrorCollector {
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

// Helper function to parse proto text into FileDescriptorProto
bool ParseProtoText(const std::string& proto_text, 
                    const std::string& file_name,
                    google::protobuf::FileDescriptorProto* proto) {
  google::protobuf::io::ArrayInputStream input_stream(
      proto_text.data(), static_cast<int>(proto_text.size()));
  SimpleErrorCollector error_collector;
  google::protobuf::io::Tokenizer tokenizer(&input_stream, &error_collector);
  google::protobuf::compiler::Parser parser;
  parser.RecordErrorsTo(&error_collector);
  
  bool parse_result = parser.Parse(&tokenizer, proto);
  if (parse_result) {
    proto->set_name(file_name);
  }
  return parse_result && error_collector.GetErrorCount() == 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Need minimum input for configuration
  if (size < 16) {
    return 0;
  }
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume configuration parameters
  uint8_t config = fdp.ConsumeIntegral<uint8_t>();
  bool enforce_visibility = (config & 0x01) != 0;
  uint8_t test_type = (config >> 1) & 0x07; // 0-7 test types
  
  // Consume proto text data
  std::string proto_text = fdp.ConsumeRemainingBytesAsString();
  if (proto_text.empty()) {
    return 0;
  }
  
  // Create a DescriptorPool
  google::protobuf::DescriptorPool pool;
  
  // Parse the proto text
  google::protobuf::FileDescriptorProto file_proto;
  bool parsed = ParseProtoText(proto_text, "test.proto", &file_proto);
  if (!parsed) {
    // If parsing fails, try to build from FileDescriptorProto directly
    // Use the proto text as binary data for FileDescriptorProto
    if (proto_text.size() > 0) {
      file_proto.ParseFromArray(proto_text.data(), static_cast<int>(proto_text.size()));
    } else {
      return 0;
    }
  }
  
  // Enable symbol visibility enforcement if configured
  pool.EnforceSymbolVisibility(enforce_visibility);
  
  // Try to build the file descriptor
  const google::protobuf::FileDescriptor* file_desc = pool.BuildFile(file_proto);
  if (file_desc == nullptr) {
    // Building failed, but we can still test with some default descriptors
    // Create a simple default FileDescriptorProto for testing
    google::protobuf::FileDescriptorProto default_proto;
    default_proto.set_name("default.proto");
    default_proto.set_edition(google::protobuf::EDITION_2024);
    default_proto.set_syntax("proto3");
    
    // Add a simple message
    google::protobuf::DescriptorProto* message = default_proto.add_message_type();
    message->set_name("TestMessage");
    
    // Add an enum
    google::protobuf::EnumDescriptorProto* enum_proto = default_proto.add_enum_type();
    enum_proto->set_name("TestEnum");
    auto* enum_value = enum_proto->add_value();
    enum_value->set_name("UNSPECIFIED");
    enum_value->set_number(0);
    
    file_desc = pool.BuildFile(default_proto);
    if (file_desc == nullptr) {
      return 0; // Couldn't build any descriptor
    }
  }
  
  // Create SymbolChecker instance
  google::protobuf::SymbolChecker checker(file_desc, file_proto);
  
  // Test different SymbolChecker APIs based on test_type
  switch (test_type % 4) {
    case 0: {
      // Test CheckSymbolVisibilityRules
      std::vector<google::protobuf::SymbolCheckerError> errors = 
          checker.CheckSymbolVisibilityRules();
      
      // Process errors if any
      for (const auto& error : errors) {
        // Access error properties to ensure they're evaluated
        absl::string_view symbol_name = error.symbol_name();
        const google::protobuf::Message* descriptor = error.descriptor();
        google::protobuf::SymbolCheckerErrorType type = error.type();
        std::string message = error.message();
      }
      break;
    }
    
    case 1: {
      // Test IsEnumNamespaceMessage on available messages
      int message_count = file_desc->message_type_count();
      if (message_count > 0) {
        for (int i = 0; i < std::min(message_count, 5); i++) {
          const google::protobuf::Descriptor* message_desc = file_desc->message_type(i);
          bool is_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*message_desc);
          (void)is_namespace; // Use the result
        }
      }
      break;
    }
    
    case 2: {
      // Test IsNamespacedEnum on available enums
      int enum_count = file_desc->enum_type_count();
      if (enum_count > 0) {
        for (int i = 0; i < std::min(enum_count, 5); i++) {
          const google::protobuf::EnumDescriptor* enum_desc = file_desc->enum_type(i);
          bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
          (void)is_namespaced; // Use the result
        }
      }
      
      // Also check enums nested in messages
      int message_count = file_desc->message_type_count();
      for (int i = 0; i < std::min(message_count, 3); i++) {
        const google::protobuf::Descriptor* message_desc = file_desc->message_type(i);
        int nested_enum_count = message_desc->enum_type_count();
        for (int j = 0; j < std::min(nested_enum_count, 3); j++) {
          const google::protobuf::EnumDescriptor* enum_desc = message_desc->enum_type(j);
          bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
          (void)is_namespaced; // Use the result
        }
      }
      break;
    }
    
    case 3: {
      // Test all three APIs together
      // First, check visibility rules
      std::vector<google::protobuf::SymbolCheckerError> errors = 
          checker.CheckSymbolVisibilityRules();
      
      // Check messages for namespace status
      int message_count = file_desc->message_type_count();
      for (int i = 0; i < std::min(message_count, 3); i++) {
        const google::protobuf::Descriptor* message_desc = file_desc->message_type(i);
        bool is_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*message_desc);
        (void)is_namespace;
        
        // Check enums within this message
        int nested_enum_count = message_desc->enum_type_count();
        for (int j = 0; j < std::min(nested_enum_count, 3); j++) {
          const google::protobuf::EnumDescriptor* enum_desc = message_desc->enum_type(j);
          bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
          (void)is_namespaced;
        }
      }
      
      // Check top-level enums
      int enum_count = file_desc->enum_type_count();
      for (int i = 0; i < std::min(enum_count, 3); i++) {
        const google::protobuf::EnumDescriptor* enum_desc = file_desc->enum_type(i);
        bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
        (void)is_namespaced;
      }
      break;
    }
  }
  
  return 0;
}

} // namespace harness_005
