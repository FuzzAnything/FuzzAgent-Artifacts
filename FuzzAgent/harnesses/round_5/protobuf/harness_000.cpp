#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/struct.pb.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/reflection.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/arena.h"
#include "absl/status/status.h"

namespace harness_000 {
// Protocol Buffers fuzzing harness for basic message operations
// This harness tests fundamental protobuf APIs: parsing, serialization,
// field manipulation, and reflection operations.



using google::protobuf::Struct;
using google::protobuf::Value;
using google::protobuf::ListValue;
using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;
using google::protobuf::TextFormat;
using google::protobuf::Arena;
using google::protobuf::Arena;
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) return 0;  // Minimum input size
    
    FuzzedDataProvider fdp(data, size);
    
    // Create message instances on heap (simpler for first harness)
    Struct struct_msg;
    Value value_msg;
    ListValue list_msg;
    
    // Test 1: Parse from serialized binary data
    std::string binary_input = fdp.ConsumeRandomLengthString(1024);
    if (!binary_input.empty()) {
        // Try parsing Struct from binary
        struct_msg.ParseFromString(binary_input);
        
        // Try parsing Value from binary
        value_msg.ParseFromString(binary_input);
        
        // Try parsing ListValue from binary  
        list_msg.ParseFromString(binary_input);
    }
    
    // Test 2: Serialize to binary
    std::string serialized_struct = struct_msg.SerializeAsString();
    std::string serialized_value = value_msg.SerializeAsString();
    std::string serialized_list = list_msg.SerializeAsString();
    
    // Test 3: Basic message operations
    // Check if initialized
    struct_msg.IsInitialized();
    value_msg.IsInitialized();
    list_msg.IsInitialized();
    
    // Get byte size
    struct_msg.ByteSizeLong();
    value_msg.ByteSizeLong();
    list_msg.ByteSizeLong();
    
    // Get space used
    struct_msg.SpaceUsedLong();
    value_msg.SpaceUsedLong();
    list_msg.SpaceUsedLong();
    
    // Test 4: Reflection operations
    const Descriptor* struct_descriptor = struct_msg.GetDescriptor();
    const Reflection* struct_reflection = struct_msg.GetReflection();
    
    if (struct_descriptor && struct_reflection) {
        // Get all fields
        int field_count = struct_descriptor->field_count();
        if (field_count > 0) {
            // Consume field index from fuzzer input
            int field_index = fdp.ConsumeIntegralInRange<int>(0, field_count - 1);
            const FieldDescriptor* field = struct_descriptor->field(field_index);
            
            if (field) {
                // Test field operations based on type
                if (field->is_repeated()) {
                    // Get repeated field size
                    int repeated_size = struct_reflection->FieldSize(struct_msg, field);
                    
                    // Try getting elements if available
                    if (repeated_size > 0) {
                        int element_index = fdp.ConsumeIntegralInRange<int>(0, repeated_size - 1);
                        // Different access methods for different types
                        if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT32) {
                            struct_reflection->GetRepeatedInt32(struct_msg, field, element_index);
                        } else if (field->cpp_type() == FieldDescriptor::CPPTYPE_STRING) {
                            struct_reflection->GetRepeatedString(struct_msg, field, element_index);
                        }
                    }
                } else {
                    // Singular field access
                    if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT32) {
                        struct_reflection->GetInt32(struct_msg, field);
                    } else if (field->cpp_type() == FieldDescriptor::CPPTYPE_STRING) {
                        struct_reflection->GetString(struct_msg, field);
                    } else if (field->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE) {
                        const Message& sub_msg = struct_reflection->GetMessage(struct_msg, field);
                        // Recursively access submessage
                        (void)sub_msg.GetDescriptor();
                    }
                }
            }
        }
    }
    
    // Test 5: Copy operations
    if (fdp.ConsumeBool()) {
        Struct struct_copy;
        struct_copy.CopyFrom(struct_msg);
        
        Value value_copy;
        value_copy.CopyFrom(value_msg);
        
        ListValue list_copy;
        list_copy.CopyFrom(list_msg);
    }
    
    // Test 6: Clear operations
    if (fdp.ConsumeBool()) {
        struct_msg.Clear();
        value_msg.Clear();
        list_msg.Clear();
    }
    
    // Test 7: Unknown fields
    struct_msg.DiscardUnknownFields();
    value_msg.DiscardUnknownFields();
    list_msg.DiscardUnknownFields();
    
    // Test 8: Merge operations
    if (fdp.ConsumeBool()) {
        Struct struct_merge;
        struct_merge.MergeFrom(struct_msg);
        
        Value value_merge;
        value_merge.MergeFrom(value_msg);
        
        ListValue list_merge;
        list_merge.MergeFrom(list_msg);
    }
    
    return 0;
}

} // namespace harness_000
