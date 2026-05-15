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

namespace harness_005 {
// Protocol Buffers fuzzing harness - Symbol Checker subsystem (enhanced)
// Targets: SymbolChecker::CheckSymbolVisibilityRules, SymbolChecker::IsNamespacedEnum,
//          SymbolChecker::IsEnumNamespaceMessage, SymbolCheckerError::message
// Uses TextFormat parsing for protobuf schema text as recommended
// Uses FuzzedDataProvider for structured input splitting



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic proto structure
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse protobuf schema text using TextFormat::Parse
    google::protobuf::FileDescriptorProto file_desc_proto;
    
    // Create protobuf text from fuzzer input
    // First consume a string for the protobuf schema text
    std::string proto_text = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    if (proto_text.empty()) return 0;
    
    // Try to parse using TextFormat::ParseFromString
    bool parse_success = google::protobuf::TextFormat::ParseFromString(proto_text, &file_desc_proto);
    if (!parse_success) {
        // If TextFormat parsing fails, try with AllowPartialMessage and AllowUnknownField
        google::protobuf::TextFormat::Parser parser;
        parser.AllowPartialMessage(true);
        parser.AllowUnknownField(true);
        parse_success = parser.ParseFromString(proto_text, &file_desc_proto);
        if (!parse_success) {
            return 0; // Couldn't parse as text format
        }
    }
    
    // Ensure we have a file name for the descriptor
    if (file_desc_proto.name().empty()) {
        // Generate a simple file name if not provided
        file_desc_proto.set_name("fuzz.proto");
    }
    
    // Ensure we have an edition set (default to 2024 for visibility features)
    if (file_desc_proto.edition() == google::protobuf::EDITION_UNKNOWN) {
        file_desc_proto.set_edition(google::protobuf::EDITION_2024);
    }
    
    // Step 2: Use DescriptorPool to build FileDescriptor
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
    
    // Step 3: Construct SymbolChecker with FileDescriptor and FileDescriptorProto
    google::protobuf::SymbolChecker symbol_checker(file_desc, file_desc_proto);
    
    // Step 4: Call SymbolChecker::CheckSymbolVisibilityRules
    std::vector<google::protobuf::SymbolCheckerError> errors = 
        symbol_checker.CheckSymbolVisibilityRules();
    
    // Step 5: Process errors via SymbolCheckerError::message()
    for (const auto& error : errors) {
        // Exercise all error reporting APIs
        absl::string_view symbol_name = error.symbol_name();
        const google::protobuf::Message* descriptor = error.descriptor();
        google::protobuf::SymbolCheckerErrorType type = error.type();
        
        // Specifically call message() method to cover this API
        std::string error_message = error.message();
        
        (void)symbol_name;
        (void)descriptor;
        (void)type;
        (void)error_message;
    }
    
    // Step 6: Test IsNamespacedEnum() and IsEnumNamespaceMessage() directly
    // Test IsEnumNamespaceMessage on all message descriptors
    for (int i = 0; i < file_desc->message_type_count(); i++) {
        const google::protobuf::Descriptor* msg_desc = file_desc->message_type(i);
        bool is_enum_namespace_msg = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*msg_desc);
        (void)is_enum_namespace_msg;
        
        // Check nested messages too
        for (int j = 0; j < msg_desc->nested_type_count(); j++) {
            const google::protobuf::Descriptor* nested_msg = msg_desc->nested_type(j);
            bool nested_is_enum_namespace_msg = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*nested_msg);
            (void)nested_is_enum_namespace_msg;
        }
    }
    
    // Test IsNamespacedEnum on all enum descriptors
    for (int i = 0; i < file_desc->enum_type_count(); i++) {
        const google::protobuf::EnumDescriptor* enum_desc = file_desc->enum_type(i);
        bool is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*enum_desc);
        (void)is_namespaced;
    }
    
    // Also check enums nested in messages
    for (int i = 0; i < file_desc->message_type_count(); i++) {
        const google::protobuf::Descriptor* msg_desc = file_desc->message_type(i);
        
        // Test IsEnumNamespaceMessage on the message
        bool msg_is_enum_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*msg_desc);
        (void)msg_is_enum_namespace;
        
        // Test enums within this message
        for (int j = 0; j < msg_desc->enum_type_count(); j++) {
            const google::protobuf::EnumDescriptor* nested_enum = msg_desc->enum_type(j);
            bool nested_is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*nested_enum);
            (void)nested_is_namespaced;
            
            // Also check if the containing message qualifies as enum namespace message
            bool container_is_enum_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*msg_desc);
            (void)container_is_enum_namespace;
        }
        
        // Check nested messages recursively
        for (int j = 0; j < msg_desc->nested_type_count(); j++) {
            const google::protobuf::Descriptor* nested_msg = msg_desc->nested_type(j);
            bool nested_msg_is_enum_namespace = google::protobuf::SymbolChecker::IsEnumNamespaceMessage(*nested_msg);
            (void)nested_msg_is_enum_namespace;
            
            // Check enums in nested messages
            for (int k = 0; k < nested_msg->enum_type_count(); k++) {
                const google::protobuf::EnumDescriptor* nested_msg_enum = nested_msg->enum_type(k);
                bool nested_msg_enum_is_namespaced = google::protobuf::SymbolChecker::IsNamespacedEnum(*nested_msg_enum);
                (void)nested_msg_enum_is_namespaced;
            }
        }
    }
    
    // Additional testing: If we have any errors, test that we can access error details
    if (!errors.empty()) {
        // Test accessing error properties from the first error
        const auto& first_error = errors[0];
        absl::string_view first_symbol = first_error.symbol_name();
        const google::protobuf::Message* first_descriptor = first_error.descriptor();
        google::protobuf::SymbolCheckerErrorType first_type = first_error.type();
        std::string first_message = first_error.message();
        
        (void)first_symbol;
        (void)first_descriptor;
        (void)first_type;
        (void)first_message;
    }
    
    return 0;
}

} // namespace harness_005
