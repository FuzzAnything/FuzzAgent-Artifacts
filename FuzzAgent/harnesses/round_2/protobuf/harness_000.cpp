#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/reflection.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/any.pb.h"
#include "google/protobuf/api.pb.h"
#include "google/protobuf/empty.pb.h"
#include "google/protobuf/struct.pb.h"
#include "google/protobuf/timestamp.pb.h"
#include "google/protobuf/duration.pb.h"
#include "google/protobuf/wrappers.pb.h"

namespace harness_000 {
// First fuzzing harness for Google Protocol Buffers library
// Targets main API entry points: Message parsing/serialization APIs


// Include protobuf headers

// Use available protobuf types

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 8) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which API path to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    // Consume test data
    std::string proto_data;
    if (fdp.remaining_bytes() > 0) {
        proto_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    } else {
        return 0;
    }
    
    // Use different available protobuf message types
    google::protobuf::Any any_msg;
    google::protobuf::Api api_msg;
    google::protobuf::Empty empty_msg;
    google::protobuf::Struct struct_msg;
    
    switch (operation) {
        case 0: {
            // Test ParseFromString() and SerializeToString() with Any
            if (proto_data.empty()) return 0;
            
            bool parse_success = any_msg.ParseFromString(proto_data);
            // Continue even if parse fails - invalid input testing is important
            
            std::string serialized;
            any_msg.SerializeToString(&serialized);
            break;
        }
        
        case 1: {
            // Test ParseFromArray() and SerializeToArray() with Api
            if (proto_data.empty()) return 0;
            
            bool parse_success = api_msg.ParseFromArray(proto_data.data(), proto_data.size());
            // Continue testing even if parse fails
            
            // Create buffer for serialization
            size_t buffer_size = api_msg.ByteSizeLong();
            if (buffer_size > 0 && buffer_size < 1024 * 1024) { // Reasonable size limit
                std::vector<char> buffer(buffer_size);
                bool serialize_success = api_msg.SerializeToArray(buffer.data(), buffer.size());
            }
            break;
        }
        
        case 2: {
            // Test MergeFrom() between two Api messages
            if (proto_data.empty()) return 0;
            
            google::protobuf::Api api_msg1;
            google::protobuf::Api api_msg2;
            
            // Try to parse first message
            bool parse_success1 = api_msg1.ParseFromString(proto_data);
            if (!parse_success1) return 0;
            
            // Create second message with some data from fuzzer input
            if (fdp.remaining_bytes() >= 8) {
                // Consume data for string prefix
                std::string prefix = fdp.ConsumeRandomLengthString(4);
                int32_t int_val = fdp.ConsumeIntegral<int32_t>();
                
                // Use reflection to set a field if possible
                const google::protobuf::Descriptor* descriptor = api_msg2.GetDescriptor();
                const google::protobuf::Reflection* reflection = api_msg2.GetReflection();
                if (descriptor && reflection) {
                    const google::protobuf::FieldDescriptor* field = 
                        descriptor->FindFieldByName("name");
                    if (field && field->type() == google::protobuf::FieldDescriptor::TYPE_STRING) {
                        std::string name = prefix + "_" + std::to_string(int_val);
                        reflection->SetString(&api_msg2, field, name);
                    }
                }
            }
            // Merge first into second
            api_msg2.MergeFrom(api_msg1);
            break;
        }
        case 3: {
            // Test CopyFrom() with Struct messages
            if (proto_data.empty()) return 0;
            
            google::protobuf::Struct struct_msg1;
            google::protobuf::Struct struct_msg2;
            
            bool parse_success = struct_msg1.ParseFromString(proto_data);
            if (!parse_success) return 0;
            
            struct_msg2.CopyFrom(struct_msg1);
            
            // Verify copy worked by serializing
            std::string serialized1, serialized2;
            struct_msg1.SerializeToString(&serialized1);
            struct_msg2.SerializeToString(&serialized2);
            break;
        }
        
        case 4: {
            // Test Reflection APIs for dynamic message manipulation with Any
            if (proto_data.empty()) return 0;
            
            bool parse_success = any_msg.ParseFromString(proto_data);
            if (!parse_success) return 0;
            
            const google::protobuf::Descriptor* descriptor = any_msg.GetDescriptor();
            const google::protobuf::Reflection* reflection = any_msg.GetReflection();
            
            if (descriptor && reflection) {
                // Try to get type_url field
                const google::protobuf::FieldDescriptor* type_url_field = 
                    descriptor->FindFieldByName("type_url");
                if (type_url_field && type_url_field->type() == google::protobuf::FieldDescriptor::TYPE_STRING) {
                    // Get current value
                    std::string current_value = reflection->GetString(any_msg, type_url_field);
                    // Set new value from fuzzer input if available
                    if (fdp.remaining_bytes() > 0) {
                        std::string suffix = fdp.ConsumeRandomLengthString(8);
                        std::string new_value = "type.googleapis.com/" + current_value + "_" + suffix;
                        reflection->SetString(&any_msg, type_url_field, new_value);
                    }
                }
                const google::protobuf::FieldDescriptor* value_field = 
                    descriptor->FindFieldByName("value");
                if (value_field && value_field->type() == google::protobuf::FieldDescriptor::TYPE_BYTES) {
                    // Can't easily modify bytes field with reflection, but we can get it
                    std::string value = reflection->GetString(any_msg, value_field);
                    // Could set new value if we had more fuzzer input
                }
            }
            break;
        }
        
        case 5: {
            // Test TextFormat::ParseFromString() for text format parsing
            if (proto_data.empty()) return 0;
            
            google::protobuf::Empty empty_msg_local;
            
            // Try to parse as text format (may fail, which is fine)
            // Note: TextFormat parsing expects text format, not binary
            // We'll try it anyway to exercise the API
            google::protobuf::TextFormat::ParseFromString(proto_data, &empty_msg_local);
            break;
        }
        
        case 6: {
            // Test DynamicMessage with Descriptor
            if (proto_data.empty()) return 0;
            
            // Get Any message descriptor
            const google::protobuf::Descriptor* any_descriptor = google::protobuf::Any::descriptor();
            if (!any_descriptor) return 0;
            
            // Create dynamic message
            std::unique_ptr<google::protobuf::Message> dynamic_msg(
                google::protobuf::MessageFactory::generated_factory()->GetPrototype(any_descriptor)->New());
            
            if (dynamic_msg) {
                // Try to parse into dynamic message
                bool parse_success = dynamic_msg->ParseFromString(proto_data);
                // Continue even if parse fails
                
                // Try to serialize
                std::string serialized;
                dynamic_msg->SerializeToString(&serialized);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_000
