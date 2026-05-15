#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/reflection.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/duration.pb.h"
#include "google/protobuf/timestamp.pb.h"
#include "google/protobuf/struct.pb.h"
#include "google/protobuf/any.pb.h"
#include "google/protobuf/wrappers.pb.h"

namespace harness_000 {
// Fuzzing harness for Google Protocol Buffers
// Targets core protobuf operations: parsing, serialization, reflection, and field access
// Uses only available headers in the build directory



// Use only available headers

// Use built .pb.h files

using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::Reflection;
using google::protobuf::Arena;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::ArrayInputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0;  // Insufficient input
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    bool use_arena = fdp.ConsumeBool();
    uint32_t max_string_length = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    
    // Consume variable-size data
    std::string serialized_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());

    try {
        switch (test_type) {
            case 0: {
                // Test 1: Duration message parsing and serialization
                google::protobuf::Duration duration_msg;
                
                // Try to parse from serialized data
                bool parse_success = duration_msg.ParseFromString(serialized_data);
                
                if (parse_success) {
                    // Serialize back
                    std::string serialized;
                    duration_msg.SerializeToString(&serialized);
                    
                    // Byte size calculation
                    duration_msg.ByteSizeLong();
                    
                    // Try partial serialization
                    std::vector<uint8_t> buffer(duration_msg.ByteSizeLong());
                    duration_msg.SerializeToArray(buffer.data(), buffer.size());
                    
                    // Check if initialized
                    duration_msg.IsInitialized();
                }
                break;
            }
            
            case 1: {
                // Test 2: Timestamp message with arena
                std::unique_ptr<Arena> arena;
                if (use_arena) {
                    arena.reset(new Arena());
                }
                
                google::protobuf::Timestamp* timestamp_msg;
                if (use_arena) {
                    timestamp_msg = Arena::Create<google::protobuf::Timestamp>(arena.get());
                } else {
                    timestamp_msg = new google::protobuf::Timestamp();
                }
                
                bool parse_success = timestamp_msg->ParseFromString(serialized_data);
                
                if (parse_success) {
                    // Get descriptor and reflection
                    const Descriptor* descriptor = timestamp_msg->GetDescriptor();
                    const Reflection* reflection = timestamp_msg->GetReflection();
                    
                    if (descriptor && reflection) {
                        // List fields
                        std::vector<const FieldDescriptor*> fields;
                        reflection->ListFields(*timestamp_msg, &fields);
                        
                        // Byte size
                        timestamp_msg->ByteSizeLong();
                        
                        // Serialize
                        std::string serialized;
                        timestamp_msg->SerializeToString(&serialized);
                        
                        // Check initialization
                        timestamp_msg->IsInitialized();
                    }
                }
                
                if (!use_arena) {
                    delete timestamp_msg;
                }
                break;
            }
            
            case 2: {
                // Test 3: Struct message (nested message)
                google::protobuf::Struct struct_msg;
                
                bool parse_success = struct_msg.ParseFromString(serialized_data);
                
                if (parse_success) {
                    // Get descriptor and reflection
                    const Descriptor* descriptor = struct_msg.GetDescriptor();
                    const Reflection* reflection = struct_msg.GetReflection();
                    
                    if (descriptor && reflection) {
                        // Try to access fields via reflection
                        for (int i = 0; i < descriptor->field_count(); ++i) {
                            const FieldDescriptor* field = descriptor->field(i);
                            
                            if (field && fdp.remaining_bytes() > 0) {
                                if (field->is_repeated()) {
                                    // Check repeated field size
                                    int field_size = reflection->FieldSize(struct_msg, field);
                                    
                                    if (field_size > 0 && fdp.remaining_bytes() > 0) {
                                        // Consume some data
                                        std::string field_data = fdp.ConsumeRandomLengthString(
                                            std::min(static_cast<size_t>(max_string_length), 
                                                    fdp.remaining_bytes()));
                                    }
                                }
                            }
                            
                            if (fdp.remaining_bytes() == 0) {
                                break;
                            }
                        }
                        
                        // Serialize
                        std::string serialized;
                        struct_msg.SerializeToString(&serialized);
                    }
                }
                break;
            }
            
            case 3: {
                // Test 4: Any message (type embedding)
                google::protobuf::Any any_msg;
                
                bool parse_success = any_msg.ParseFromString(serialized_data);
                
                if (parse_success) {
                    // Check type URL
                    const std::string& type_url = any_msg.type_url();
                    
                    // Byte size
                    any_msg.ByteSizeLong();
                    
                    // Serialize
                    std::string serialized;
                    any_msg.SerializeToString(&serialized);
                    
                    // Check if initialized
                    any_msg.IsInitialized();
                }
                break;
            }
            
            case 4: {
                // Test 5: Wrapper messages (BoolValue, Int32Value, etc.)
                uint8_t wrapper_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (wrapper_type) {
                    case 0: {
                        google::protobuf::BoolValue bool_msg;
                        bool_msg.ParseFromString(serialized_data);
                        break;
                    }
                    case 1: {
                        google::protobuf::Int32Value int32_msg;
                        int32_msg.ParseFromString(serialized_data);
                        break;
                    }
                    case 2: {
                        google::protobuf::StringValue string_msg;
                        string_msg.ParseFromString(serialized_data);
                        break;
                    }
                    case 3: {
                        google::protobuf::DoubleValue double_msg;
                        double_msg.ParseFromString(serialized_data);
                        break;
                    }
                }
                break;
            }
        }
        
        // Additional test: Try stream-based parsing with any message
        if (!serialized_data.empty()) {
            ArrayInputStream input_stream(serialized_data.data(), serialized_data.size());
            CodedInputStream coded_stream(&input_stream);
            
            // Set some limits
            coded_stream.SetTotalBytesLimit(1024 * 1024);  // 1MB limit
            
            google::protobuf::Duration stream_msg;
            bool stream_parse_success = stream_msg.ParseFromCodedStream(&coded_stream);
            
            if (stream_parse_success) {
                // Verify the message
                stream_msg.IsInitialized();
            }
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
        // libFuzzer will handle the inputs that cause exceptions
    }

    return 0;
}

} // namespace harness_000
