#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/message_lite.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/any.pb.h"
#include "google/protobuf/struct.pb.h"
#include "google/protobuf/timestamp.pb.h"
#include "google/protobuf/duration.pb.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume a small portion to decide which test to run
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Use the remaining data as protobuf serialized data
    std::vector<uint8_t> proto_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Test different protobuf message types and parsing strategies
    switch (test_type) {
        case 0: {
            // Test with FileDescriptorProto - a complex message type
            google::protobuf::FileDescriptorProto message;
            if (proto_data.size() > 0) {
                // Try parsing the fuzzer input as protobuf data
                bool parse_result = message.ParseFromArray(proto_data.data(), proto_data.size());
                
                // If parsing succeeds, test serialization back
                if (parse_result) {
                    std::string serialized;
                    message.SerializeToString(&serialized);
                    
                    // Test ByteSize
                    size_t byte_size = message.ByteSizeLong();
                    
                    // Test IsInitialized
                    bool is_initialized = message.IsInitialized();
                    
                    // Test Clear
                    message.Clear();
                    
                    // Test SpaceUsed
                    size_t space_used = message.SpaceUsedLong();
                }
            }
            break;
        }
        
        case 1: {
            // Test with Any message
            google::protobuf::Any message;
            if (proto_data.size() > 0) {
                bool parse_result = message.ParseFromArray(proto_data.data(), proto_data.size());
                
                if (parse_result) {
                    // Test serialization
                    std::string serialized;
                    message.SerializeToString(&serialized);
                    
                    // Test type URL operations
                    const std::string& type_url = message.type_url();
                    bool has_type_url = !type_url.empty();
                    
                    // Test ByteSize
                    size_t byte_size = message.ByteSizeLong();
                    
                    // Test default instance
                    const google::protobuf::Any& default_instance = google::protobuf::Any::default_instance();
                }
            }
            break;
        }
        
        case 2: {
            // Test with Struct message (nested map structure)
            google::protobuf::Struct message;
            if (proto_data.size() > 0) {
                bool parse_result = message.ParseFromArray(proto_data.data(), proto_data.size());
                
                if (parse_result) {
                    // Test alternative serialization methods
                    std::string serialized1;
                    message.SerializeToString(&serialized1);
                    
                    // Test ByteSize
                    size_t byte_size = message.ByteSizeLong();
                    
                    // Test CopyFrom with another message
                    google::protobuf::Struct another_message;
                    another_message.CopyFrom(message);
                }
            }
            break;
        }
        
        case 3: {
            // Test with Timestamp message
            google::protobuf::Timestamp message;
            if (proto_data.size() > 0) {
                bool parse_result = message.ParseFromArray(proto_data.data(), proto_data.size());
                
                if (parse_result) {
                    // Test various operations
                    std::string serialized;
                    message.SerializeToString(&serialized);
                    
                    // Test copy constructor
                    google::protobuf::Timestamp copied_message(message);
                    
                    // Test assignment using CopyFrom
                    google::protobuf::Timestamp assigned_message;
                    assigned_message.CopyFrom(message);
                    
                    // Test accessors
                    int64_t seconds = message.seconds();
                    int32_t nanos = message.nanos();
                    
                    // Test ByteSize
                    size_t byte_size = message.ByteSizeLong();
                }
            }
            break;
        }
        
        case 4: {
            // Test with Duration message
            google::protobuf::Duration message;
            if (proto_data.size() > 0) {
                bool parse_result = message.ParseFromArray(proto_data.data(), proto_data.size());
                
                if (parse_result) {
                    // Test serialization
                    std::string serialized;
                    message.SerializeToString(&serialized);
                    
                    // Test ByteSize
                    size_t byte_size = message.ByteSizeLong();
                    
                    // Test default instance
                    const google::protobuf::Duration& default_instance = google::protobuf::Duration::default_instance();
                    
                    // Test copy constructor
                    google::protobuf::Duration copied_message(message);
                    
                    // Test accessors
                    int64_t seconds = message.seconds();
                    int32_t nanos = message.nanos();
                }
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_000
