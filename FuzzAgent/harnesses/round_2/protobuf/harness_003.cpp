#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/symbol_checker.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/compiler/parser.h"
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_003 {
// Harness for Google Protocol Buffers SymbolChecker module targeting symbol visibility validation
// Targets compiler/schema validation logic as identified by coverage analysis
// Semantic differentiation from previous harnesses:
// - harness_000: binary parsing and serialization
// - harness_001: text format parsing and printing
// - harness_002: Cord-based I/O streaming with compression
// - harness_003: SymbolChecker visibility rules validation (compiler infrastructure)


// Include protobuf headers for SymbolChecker and descriptor infrastructure

// Simple error collector for parser
class SimpleErrorCollector : public google::protobuf::io::ErrorCollector {
public:
    void RecordError(int line, int column, absl::string_view message) override {
        last_error_ = absl::StrFormat("%d:%d:%s", line, column, message);
    }
    
    const std::string& last_error() { return last_error_; }
    
private:
    std::string last_error_;
};

// Helper function to parse proto text into FileDescriptorProto
google::protobuf::FileDescriptorProto ParseProtoText(absl::string_view file_name, absl::string_view file_text) {
    google::protobuf::io::ArrayInputStream input_stream(file_text.data(), file_text.size());
    SimpleErrorCollector error_collector;
    google::protobuf::io::Tokenizer tokenizer(&input_stream, &error_collector);
    google::protobuf::compiler::Parser parser;
    parser.RecordErrorsTo(&error_collector);
    google::protobuf::FileDescriptorProto proto;
    if (!parser.Parse(&tokenizer, &proto)) {
        // Return empty proto on parse failure
        return google::protobuf::FileDescriptorProto();
    }
    proto.set_name(std::string(file_name));
    return proto;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which SymbolChecker functions to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume configuration options
    bool enforce_symbol_visibility = fdp.ConsumeBool();
    bool generate_simple_schema = fdp.ConsumeBool();
    
    // Consume schema data
    std::string schema_text;
    if (fdp.remaining_bytes() > 0) {
        size_t schema_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(fdp.remaining_bytes(), 4096));
        schema_text = fdp.ConsumeBytesAsString(schema_size);
    } else {
        return 0;
    }
    
    // Create DescriptorPool
    google::protobuf::DescriptorPool pool;
    pool.EnforceSymbolVisibility(enforce_symbol_visibility);
    
    try {
        // Create FileDescriptorProto from fuzzer input
        google::protobuf::FileDescriptorProto file_proto;
        
        if (generate_simple_schema) {
            // Generate a simple schema with enum namespace message pattern
            std::string simple_schema = R"schema(
syntax = "proto3";
package fuzzed.test;

message TestMessage {
  int32 field1 = 1;
  string field2 = 2;
}

enum TestEnum {
  VALUE0 = 0;
  VALUE1 = 1;
}

// Try to create an enum namespace message pattern
message EnumNamespaceMessage {
  enum NamespacedEnum {
    ENUM_VALUE0 = 0;
  }
  reserved 1 to max;
}
)schema";
            
            // Mix fuzzer data into the schema
            if (schema_text.size() > 10) {
                // Replace parts of schema with fuzzer data
                size_t pos = simple_schema.find("TestMessage");
                if (pos != std::string::npos && schema_text.size() > 5) {
                    std::string msg_name = schema_text.substr(0, std::min<size_t>(10, schema_text.size()));
                    simple_schema.replace(pos, 11, msg_name);
                }
            }
            
            file_proto = ParseProtoText("fuzzed.proto", simple_schema);
        } else {
            // Try to parse fuzzer input as proto text directly
            file_proto = ParseProtoText("fuzzed.proto", schema_text);
        }
        
        if (file_proto.name().empty()) {
            // Parsing failed, but that's OK for fuzzing
            return 0;
        }
        
        // Build FileDescriptor from FileDescriptorProto
        const google::protobuf::FileDescriptor* file_descriptor = pool.BuildFile(file_proto);
        if (!file_descriptor) {
            // Building failed, but that's OK for fuzzing - invalid schemas are valid test cases
            return 0;
        }
        
        // Create SymbolChecker instance
        google::protobuf::SymbolChecker symbol_checker(file_descriptor, file_proto);
        
        // Test different SymbolChecker operations based on operation type
        switch (operation) {
            case 0: {
                // Test IsEnumNamespaceMessage on all messages in the file
                for (int i = 0; i < file_descriptor->message_type_count(); ++i) {
                    const google::protobuf::Descriptor* message = file_descriptor->message_type(i);
                    bool is_namespace_msg = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*message);
                    // Result can be used or ignored - fuzzer cares about code paths taken
                    (void)is_namespace_msg;
                }
                break;
            }
            
            case 1: {
                // Test IsNamespacedEnum on all enums in the file
                for (int i = 0; i < file_descriptor->enum_type_count(); ++i) {
                    const google::protobuf::EnumDescriptor* enum_desc = file_descriptor->enum_type(i);
                    bool is_namespaced_enum = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
                    (void)is_namespaced_enum;
                }
                
                // Also check nested enums in messages
                for (int i = 0; i < file_descriptor->message_type_count(); ++i) {
                    const google::protobuf::Descriptor* message = file_descriptor->message_type(i);
                    for (int j = 0; j < message->enum_type_count(); ++j) {
                        const google::protobuf::EnumDescriptor* nested_enum = message->enum_type(j);
                        bool is_namespaced_enum = google::protobuf::SymbolChecker::IsNamespacedEnum(*nested_enum);
                        (void)is_namespaced_enum;
                    }
                }
                break;
            }
            
            case 2: {
                // Test CheckSymbolVisibilityRules
                std::vector<google::protobuf::SymbolCheckerError> errors = 
                    symbol_checker.CheckSymbolVisibilityRules();
                
                // Process errors - fuzzer can explore error handling paths
                for (const auto& error : errors) {
                    // Access error properties to exercise code
                    std::string symbol_name = std::string(error.symbol_name());
                    auto error_type = error.type();
                    std::string error_message = error.message();
                    (void)symbol_name;
                    (void)error_type;
                    (void)error_message;
                }
                break;
            }
            
            case 3: {
                // Comprehensive test: call all three target APIs
                // First test static methods
                if (file_descriptor->message_type_count() > 0) {
                    const google::protobuf::Descriptor* first_message = file_descriptor->message_type(0);
                    bool is_namespace_msg = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*first_message);
                    (void)is_namespace_msg;
                    
                    if (first_message->enum_type_count() > 0) {
                        const google::protobuf::EnumDescriptor* first_enum = first_message->enum_type(0);
                        bool is_namespaced_enum = google::protobuf::SymbolChecker::IsNamespacedEnum(*first_enum);
                        (void)is_namespaced_enum;
                    }
                }
                
                // Then test instance method
                std::vector<google::protobuf::SymbolCheckerError> errors = 
                    symbol_checker.CheckSymbolVisibilityRules();
                (void)errors;
                break;
            }
        }
        
    } catch (...) {
        // Catch all exceptions - fuzzer should continue even if parsing/building fails
        return 0;
    }
    
    return 0;
}

} // namespace harness_003
