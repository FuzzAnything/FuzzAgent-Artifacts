#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/message_lite.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"

namespace harness_000 {
// Protocol Buffers fuzzing harness - Basic message parsing and serialization
// Targets core protobuf APIs: ParseFromString, SerializeToString, ParseFromArray, MergeFromString
// Uses FuzzedDataProvider for structured input splitting



// Include some generated test message types for testing
// We'll use FileDescriptorProto as a test message type since it's available
// and exercises many protobuf features

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 4) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a FileDescriptorProto message for testing
    google::protobuf::FileDescriptorProto file_desc_proto;
    
    // Test 1: ParseFromString with random data
    std::string proto_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    if (!proto_data.empty()) {
        // Try parsing the data - this may fail, which is expected
        bool parse_success = file_desc_proto.ParseFromString(proto_data);
        (void)parse_success; // Result doesn't matter for fuzzing
        
        // If parsing succeeded, test serialization
        if (parse_success) {
            std::string serialized;
            bool serialize_success = file_desc_proto.SerializeToString(&serialized);
            (void)serialize_success;
            
            // Test partial parsing variants
            google::protobuf::FileDescriptorProto file_desc_proto2;
            bool merge_success = file_desc_proto2.MergeFromString(proto_data);
            (void)merge_success;
            
            bool parse_partial_success = file_desc_proto2.ParsePartialFromString(proto_data);
            (void)parse_partial_success;
        }
    }
    
    // Test 2: ParseFromArray with remaining data
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (!remaining_data.empty()) {
            google::protobuf::FileDescriptorProto file_desc_proto3;
            
            // Test ParseFromArray
            bool parse_array_success = file_desc_proto3.ParseFromArray(
                remaining_data.data(), static_cast<int>(remaining_data.size()));
            (void)parse_array_success;
            
            // Test ParsePartialFromArray
            bool parse_partial_array_success = file_desc_proto3.ParsePartialFromArray(
                remaining_data.data(), static_cast<int>(remaining_data.size()));
            (void)parse_partial_array_success;
            
            // If parsing succeeded, test reflection APIs
            if (parse_array_success || parse_partial_array_success) {
                // Get descriptor and reflection
                const google::protobuf::Descriptor* descriptor = file_desc_proto3.GetDescriptor();
                const google::protobuf::Reflection* reflection = file_desc_proto3.GetReflection();
                
                if (descriptor && reflection) {
                    // Exercise reflection APIs
                    int field_count = descriptor->field_count();
                    (void)field_count;
                    
                    // Try to get a few field values using reflection
                    for (int i = 0; i < std::min(5, field_count); i++) {
                        const google::protobuf::FieldDescriptor* field = descriptor->field(i);
                        if (field) {
                            // Check if field is set
                            bool is_set = reflection->HasField(file_desc_proto3, field);
                            (void)is_set;
                            
                            // Get field count for repeated fields
                            if (field->is_repeated()) {
                                int field_size = reflection->FieldSize(file_desc_proto3, field);
                                (void)field_size;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Test 3: Dynamic message creation and manipulation
    // Create a simple dynamic message factory
    google::protobuf::DynamicMessageFactory factory;
    
    // Get FileDescriptorProto descriptor
    const google::protobuf::Descriptor* descriptor = 
        google::protobuf::FileDescriptorProto::descriptor();
    
    if (descriptor) {
        // Create a dynamic message
        std::unique_ptr<google::protobuf::Message> dynamic_message(factory.GetPrototype(descriptor)->New());
        
        // Try to parse some data into the dynamic message
        if (size > 10) {
            // Use a subset of the original data
            size_t parse_size = std::min(size, static_cast<size_t>(100));
            bool dynamic_parse_success = dynamic_message->ParseFromArray(data, static_cast<int>(parse_size));
            (void)dynamic_parse_success;
            
            // Test serialization of dynamic message
            if (dynamic_parse_success) {
                std::string dynamic_serialized;
                bool dynamic_serialize_success = dynamic_message->SerializeToString(&dynamic_serialized);
                (void)dynamic_serialize_success;
            }
        }
    }
    
    // Test 4: Message copying and clearing
    google::protobuf::FileDescriptorProto copy_proto;
    copy_proto.CopyFrom(file_desc_proto);
    
    // Clear and check emptiness
    copy_proto.Clear();
    bool is_initialized = copy_proto.IsInitialized();
    (void)is_initialized;
    
    // Byte size calculation
    int byte_size = file_desc_proto.ByteSize();
    (void)byte_size;
    
    int byte_size_long = file_desc_proto.ByteSizeLong();
    (void)byte_size_long;
    
    return 0;
}

} // namespace harness_000
