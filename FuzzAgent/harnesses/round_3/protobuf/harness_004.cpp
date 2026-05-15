#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/symbol_checker.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/text_format.h"
#include "absl/strings/string_view.h"

namespace harness_004 {
// Protocol Buffers fuzzing harness - Symbol Checker subsystem
// Targets: SymbolChecker::CheckSymbolVisibilityRules, SymbolChecker::IsNamespacedEnum
// Uses FuzzedDataProvider for structured input splitting



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic proto structure
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create FileDescriptorProto with fuzzer-provided data
    google::protobuf::FileDescriptorProto file_desc_proto;
    
    // Try to parse binary proto data
    std::string proto_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    if (proto_data.empty()) return 0;
    
    bool parse_success = file_desc_proto.ParseFromString(proto_data);
    if (!parse_success) {
        // If binary parsing fails, try text format parsing with remaining data
        if (fdp.remaining_bytes() > 0) {
            std::string text_data = fdp.ConsumeRemainingBytesAsString();
            google::protobuf::TextFormat::Parser parser;
            parser.AllowPartialMessage(true);
            parser.AllowUnknownField(true);
            parse_success = parser.ParseFromString(text_data, &file_desc_proto);
            if (!parse_success) {
                return 0; // Couldn't parse any format
            }
        } else {
            return 0;
        }
    }
    
    // Step 2: Create DescriptorPool and build the file descriptor
    google::protobuf::DescriptorPool pool;
    
    const google::protobuf::FileDescriptor* file_desc = nullptr;
    try {
        file_desc = pool.BuildFile(file_desc_proto);
    } catch (...) {
        // BuildFile may throw on invalid descriptors
        return 0;
    }
    
    if (file_desc == nullptr) {
        // Couldn't build a valid file descriptor
        return 0;
    }
    
    // Step 3: Instantiate SymbolChecker with FileDescriptor and its proto
    google::protobuf::SymbolChecker symbol_checker(file_desc, file_desc_proto);
    
    // Step 4: Call SymbolChecker::Initialize (called internally by CheckSymbolVisibilityRules)
    // Initialize is private, but CheckSymbolVisibilityRules calls it internally
    
    // Step 5: Call SymbolChecker::CheckSymbolVisibilityRules
    std::vector<google::protobuf::SymbolCheckerError> errors = 
        symbol_checker.CheckSymbolVisibilityRules();
    
    // Step 6: Process SymbolCheckerError results
    for (const auto& error : errors) {
        // Exercise error reporting APIs
        absl::string_view symbol_name = error.symbol_name();
        const google::protobuf::Message* descriptor = error.descriptor();
        google::protobuf::SymbolCheckerErrorType type = error.type();
        std::string message = error.message();
        
        (void)symbol_name;
        (void)descriptor;
        (void)type;
        (void)message;
    }
    
    // Step 7: For enums in the descriptor, call SymbolChecker::IsNamespacedEnum
    // Iterate through all enum descriptors in the file
    for (int i = 0; i < file_desc->enum_type_count(); i++) {
        const google::protobuf::EnumDescriptor* enum_desc = file_desc->enum_type(i);
        bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
        (void)is_namespaced;
        
        // Also check nested enums within messages
    }
    
    // Check enums nested in messages
    for (int i = 0; i < file_desc->message_type_count(); i++) {
        const google::protobuf::Descriptor* message_desc = file_desc->message_type(i);
        // Recursively check nested messages
        std::vector<const google::protobuf::Descriptor*> message_stack;
        message_stack.push_back(message_desc);
        
        while (!message_stack.empty()) {
            const google::protobuf::Descriptor* current = message_stack.back();
            message_stack.pop_back();
            
            // Check enums in this message
            for (int j = 0; j < current->enum_type_count(); j++) {
                const google::protobuf::EnumDescriptor* enum_desc = current->enum_type(j);
                bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
                (void)is_namespaced;
            }
            
            // Add nested messages to stack
            for (int j = 0; j < current->nested_type_count(); j++) {
                message_stack.push_back(current->nested_type(j));
            }
        }
    }
    
    // Step 8: Also test IsEnumNamespaceMessage static method if accessible
    // Note: IsEnumNamespaceMessage is static public method
    for (int i = 0; i < file_desc->message_type_count(); i++) {
        const google::protobuf::Descriptor* message_desc = file_desc->message_type(i);
        bool is_enum_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*message_desc);
        (void)is_enum_namespace;
        
        // Check nested messages too
        std::vector<const google::protobuf::Descriptor*> message_stack;
        message_stack.push_back(message_desc);
        
        while (!message_stack.empty()) {
            const google::protobuf::Descriptor* current = message_stack.back();
            message_stack.pop_back();
            
            bool nested_is_enum_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*current);
            (void)nested_is_enum_namespace;
            
            // Add nested messages to stack
            for (int j = 0; j < current->nested_type_count(); j++) {
                message_stack.push_back(current->nested_type(j));
            }
        }
    }
    
    return 0;
}

} // namespace harness_004
