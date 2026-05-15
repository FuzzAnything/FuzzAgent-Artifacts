#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/symbol_checker.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"

namespace harness_008 {
// Fuzzing harness for Google Protocol Buffers SymbolChecker validation operations
// Targets uncovered SymbolChecker APIs: CheckSymbolVisibilityRules (46 branches), 
// IsNamespacedEnum (34 branches), IsEnumNamespaceMessage (20 branches)
// 
// Invocation sequence from coverage guidance:
// 1. Load protobuf definitions using DescriptorPool/FileDescriptor
// 2. Create SymbolChecker instance  
// 3. Retrieve Descriptor/EnumDescriptor objects
// 4. Call IsNamespacedEnum with EnumDescriptor
// 5. Call IsEnumNamespaceMessage with Descriptor
// 6. Call CheckSymbolVisibilityRules to validate symbol graph
//
// Semantic differentiation from existing harnesses:
// - harness_000: message operations (binary parsing/serialization)
// - harness_001: Tokenizer parsing (low-level text parsing)
// - harness_002: CordInputStream/CordOutputStream (I/O streams with Cord)
// - harness_003: CodedInputStream/CodedOutputStream (binary I/O with Cord)
// - harness_004: JSON format conversion
// - harness_005: service/method descriptor reflection
// - harness_006: unknown field handling and arena allocation
// - harness_007: Gzip compression/decompression stream APIs
// This harness targets SymbolChecker symbol visibility validation - a new area



// SymbolChecker and descriptor headers

using google::protobuf::SymbolChecker;
using google::protobuf::Descriptor;
using google::protobuf::EnumDescriptor;
using google::protobuf::FileDescriptor;
using google::protobuf::FileDescriptorProto;
using google::protobuf::DescriptorPool;
using google::protobuf::DescriptorDatabase;
using google::protobuf::TextFormat;

// Simple error collector for descriptor building
class FuzzErrorCollector : public DescriptorPool::ErrorCollector {
public:
    FuzzErrorCollector() = default;
    ~FuzzErrorCollector() override = default;
    
    void RecordError(absl::string_view filename, absl::string_view element_name,
                     const google::protobuf::Message* descriptor,
                     DescriptorPool::ErrorCollector::ErrorLocation location,
                     absl::string_view message) override {
        // Just track that an error occurred
        error_count_++;
    }
    
    void RecordWarning(absl::string_view filename, absl::string_view element_name,
                       const google::protobuf::Message* descriptor,
                       DescriptorPool::ErrorCollector::ErrorLocation location,
                       absl::string_view message) override {
        warning_count_++;
    }
    
    int error_count_ = 0;
    int warning_count_ = 0;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful symbol checking
    // Need enough for proto text generation and configuration
    if (size < 64) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t visibility_config = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool include_enum_namespace = fdp.ConsumeBool();
    bool include_nested_messages = fdp.ConsumeBool();
    bool include_nested_enums = fdp.ConsumeBool();
    
    // Generate proto file name
    std::string file_name = fdp.ConsumeRandomLengthString(16);
    if (file_name.empty()) file_name = "test.proto";
    
    // Generate package name
    std::string package_name = fdp.ConsumeRandomLengthString(16);
    if (package_name.empty()) package_name = "test.package";
    
    // Generate proto text content based on fuzzed configuration
    std::string proto_text;
    proto_text.reserve(1024);
    
    // Start with edition and package
    proto_text = "edition = \"2024\";\n";
    proto_text += "package " + package_name + ";\n\n";
    
    // Add visibility configuration
    if (visibility_config == 0) {
        proto_text += "option features.default_symbol_visibility = STRICT;\n\n";
    } else if (visibility_config == 1) {
        proto_text += "option features.default_symbol_visibility = LOCAL_ALL;\n\n";
    } else if (visibility_config == 2) {
        proto_text += "option features.default_symbol_visibility = EXPORT_TOP_LEVEL;\n\n";
    }
    // visibility_config == 3: no explicit option, use default
    
    // Add enum namespace message if requested
    if (include_enum_namespace) {
        proto_text += "local message EnumNamespaceMessage {\n";
        proto_text += "  export enum NamespacedEnum {\n";
        proto_text += "    ENUM_UNSPECIFIED = 0;\n";
        
        // Add some enum values from fuzzer input
        int num_enum_values = fdp.ConsumeIntegralInRange<int>(0, 5);
        for (int i = 0; i < num_enum_values && fdp.remaining_bytes() > 0; i++) {
            std::string enum_name = fdp.ConsumeRandomLengthString(16);
            if (enum_name.empty()) enum_name = "VALUE_" + std::to_string(i + 1);
            proto_text += "    " + enum_name + " = " + std::to_string(i + 1) + ";\n";
        }
        
        proto_text += "  }\n";
        proto_text += "  reserved 1 to max;\n";
        proto_text += "}\n\n";
    }
    
    // Add a regular top-level message
    proto_text += "message TopLevelMessage {\n";
    
    // Add nested message if requested
    if (include_nested_messages) {
        std::string nested_visibility = fdp.ConsumeBool() ? "local" : "export";
        proto_text += "  " + nested_visibility + " message NestedMessage {\n";
        proto_text += "    string field = 1;\n";
        proto_text += "  }\n";
        
        // Add field referencing nested message
        proto_text += "  NestedMessage nested = 1;\n";
    }
    
    // Add nested enum if requested
    if (include_nested_enums) {
        std::string enum_visibility = fdp.ConsumeBool() ? "local" : "export";
        proto_text += "  " + enum_visibility + " enum NestedEnum {\n";
        proto_text += "    NESTED_UNSPECIFIED = 0;\n";
        proto_text += "  }\n";
        
        // Add field using nested enum
        proto_text += "  NestedEnum enum_field = 2;\n";
    }
    
    // Add some regular fields
    if (fdp.remaining_bytes() > 0) {
        proto_text += "  string name = 3;\n";
        proto_text += "  int32 count = 4;\n";
        proto_text += "  bool flag = 5;\n";
    }
    
    proto_text += "}\n\n";
    
    // Add a regular top-level enum
    proto_text += "enum TopLevelEnum {\n";
    proto_text += "  TOP_UNSPECIFIED = 0;\n";
    
    // Add some enum values from fuzzer input
    int num_top_values = fdp.ConsumeIntegralInRange<int>(0, 3);
    for (int i = 0; i < num_top_values && fdp.remaining_bytes() > 0; i++) {
        std::string top_name = fdp.ConsumeRandomLengthString(16);
        if (top_name.empty()) top_name = "TOP_VALUE_" + std::to_string(i + 1);
        proto_text += "  " + top_name + " = " + std::to_string(i + 1) + ";\n";
    }
    
    proto_text += "}\n";
    
    try {
        // Create descriptor pool
        DescriptorPool pool;
        FuzzErrorCollector error_collector;
        
        // Parse the proto text
        FileDescriptorProto file_proto;
        google::protobuf::io::ArrayInputStream input_stream(proto_text.data(), proto_text.size());
        
        // Use TextFormat parser to parse proto text
        // If parsing fails, we'll skip this test case
        if (!TextFormat::Parse(&input_stream, &file_proto)) {
            // Parsing failed - skip this test case
            return 0;
        }
        
        file_proto.set_name(file_name);
        
        // Try to build the file descriptor
        const FileDescriptor* file_desc = pool.BuildFileCollectingErrors(file_proto, &error_collector);
        
        if (file_desc != nullptr) {
            // Successfully built file descriptor
            // Create SymbolChecker instance
            SymbolChecker symbol_checker(file_desc, file_proto);
            
            // Test IsEnumNamespaceMessage on all messages
            for (int i = 0; i < file_desc->message_type_count(); i++) {
                const Descriptor* message_desc = file_desc->message_type(i);
                bool is_enum_namespace = SymbolChecker::IsEnumNamespaceMessage(*message_desc);
                (void)is_enum_namespace; // Mark as used
                
                // Test IsNamespacedEnum on all enums within this message
                for (int j = 0; j < message_desc->enum_type_count(); j++) {
                    const EnumDescriptor* enum_desc = message_desc->enum_type(j);
                    bool is_namespaced = SymbolChecker::IsNamespacedEnum(*enum_desc);
                    (void)is_namespaced; // Mark as used
                }
            }
            
            // Test IsNamespacedEnum on top-level enums
            for (int i = 0; i < file_desc->enum_type_count(); i++) {
                const EnumDescriptor* enum_desc = file_desc->enum_type(i);
                bool is_namespaced = SymbolChecker::IsNamespacedEnum(*enum_desc);
                (void)is_namespaced; // Mark as used
            }
            
            // Call CheckSymbolVisibilityRules
            std::vector<google::protobuf::SymbolCheckerError> errors = 
                symbol_checker.CheckSymbolVisibilityRules();
            
            // Errors vector can be examined but we don't need to act on it for fuzzing
            
        } else {
            // File descriptor building failed - this is expected for malformed input
            // We can still test SymbolChecker with null descriptor (though it may crash)
            // Better to skip in this case
        }
        
    } catch (...) {
        // Catch all exceptions - fuzzer should not crash on bad input
    }
    
    return 0;
}

} // namespace harness_008
