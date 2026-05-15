#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/descriptor_database.h"

namespace harness_005 {
// Fuzzing harness for Google Protocol Buffers Service and Method Descriptor operations
// Targets ServiceDescriptor and MethodDescriptor APIs: FindMethodByName, method_count, 
// name(), full_name(), input_type(), output_type(), client_streaming(), server_streaming()
// Focuses on service descriptor reflection APIs rather than message operations or I/O streams
// Differentiates from existing harnesses:
// - harness_000: message operations (binary parsing/serialization)
// - harness_001: Tokenizer parsing (low-level text parsing)
// - harness_002: CordInputStream/CordOutputStream (I/O streams with Cord)
// - harness_003: CodedInputStream/CodedOutputStream (binary I/O with Cord)
// - harness_004: JSON format conversion
// This harness targets service and method descriptor reflection APIs



// Descriptor headers

using google::protobuf::Descriptor;
using google::protobuf::DescriptorPool;
using google::protobuf::FileDescriptor;
using google::protobuf::FileDescriptorProto;
using google::protobuf::ServiceDescriptor;
using google::protobuf::MethodDescriptor;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful descriptor testing
    if (size < 32) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    bool use_streaming_methods = fdp.ConsumeBool();
    bool create_multiple_services = fdp.ConsumeBool();
    
    // Create a descriptor pool
    DescriptorPool pool;
    
    // Create file descriptor protos based on fuzzed input
    FileDescriptorProto file_proto;
    file_proto.set_name("test.proto");
    file_proto.set_package("test.fuzz");
    
    // Add some message types
    auto* request_msg = file_proto.add_message_type();
    request_msg->set_name("FuzzRequest");
    
    auto* response_msg = file_proto.add_message_type();
    response_msg->set_name("FuzzResponse");
    
    auto* stream_request_msg = file_proto.add_message_type();
    stream_request_msg->set_name("StreamRequest");
    
    auto* stream_response_msg = file_proto.add_message_type();
    stream_response_msg->set_name("StreamResponse");
    
    // Create first service
    auto* service_proto = file_proto.add_service();
    service_proto->set_name("TestService");
    
    // Add methods to first service based on fuzzed input
    int method_count = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < method_count; i++) {
        auto* method_proto = service_proto->add_method();
        
        // Consume method name from fuzzer input
        size_t method_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        std::string method_name = fdp.ConsumeBytesAsString(method_name_len);
        if (method_name.empty()) method_name = "Method" + std::to_string(i);
        method_proto->set_name(method_name);
        
        // Set input and output types
        method_proto->set_input_type(".test.fuzz.FuzzRequest");
        method_proto->set_output_type(".test.fuzz.FuzzResponse");
        
        // Randomly set streaming flags
        if (use_streaming_methods && fdp.ConsumeBool()) {
            method_proto->set_client_streaming(fdp.ConsumeBool());
            method_proto->set_server_streaming(fdp.ConsumeBool());
        }
    }
    
    // Create second service if requested
    if (create_multiple_services && fdp.remaining_bytes() > 0) {
        auto* service_proto2 = file_proto.add_service();
        service_proto2->set_name("AnotherService");
        
        int method_count2 = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < method_count2 && fdp.remaining_bytes() > 0; i++) {
            auto* method_proto = service_proto2->add_method();
            
            size_t method_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            std::string method_name = fdp.ConsumeBytesAsString(method_name_len);
            if (method_name.empty()) method_name = "AnotherMethod" + std::to_string(i);
            method_proto->set_name(method_name);
            
            method_proto->set_input_type(".test.fuzz.StreamRequest");
            method_proto->set_output_type(".test.fuzz.StreamResponse");
            
            if (use_streaming_methods && fdp.ConsumeBool()) {
                method_proto->set_client_streaming(true);
                method_proto->set_server_streaming(true);
            }
        }
    }
    
    // Build the file descriptor
    const FileDescriptor* file_desc = pool.BuildFile(file_proto);
    if (!file_desc) {
        // Failed to build descriptor, but that's OK for fuzzing
        return 0;
    }
    
    // Test ServiceDescriptor APIs
    int service_count = file_desc->service_count();
    for (int i = 0; i < service_count; i++) {
        const ServiceDescriptor* service = file_desc->service(i);
        
        // Test ServiceDescriptor methods
        std::string service_name = std::string(service->name());
        std::string service_full_name = std::string(service->full_name());
        int service_index = service->index();
        const FileDescriptor* service_file = service->file();
        
        // Test method iteration
        int method_count_in_service = service->method_count();
        for (int j = 0; j < method_count_in_service; j++) {
            const MethodDescriptor* method = service->method(j);
            
            // Test MethodDescriptor methods
            std::string method_name = std::string(method->name());
            std::string method_full_name = std::string(method->full_name());
            int method_index = method->index();
            const ServiceDescriptor* parent_service = method->service();
            const FileDescriptor* method_file = method->file();
            
            // Test type accessors
            const Descriptor* input_type = method->input_type();
            const Descriptor* output_type = method->output_type();
            
            // Test streaming flags
            bool client_streaming = method->client_streaming();
            bool server_streaming = method->server_streaming();
            
            // Test FindMethodByName
            const MethodDescriptor* found_method = service->FindMethodByName(method_name);
            if (found_method) {
                // Verify it's the same method
                bool same_method = (found_method == method);
                std::string found_method_name = std::string(found_method->name());
            }
            
            // Try finding non-existent method
            const MethodDescriptor* not_found = service->FindMethodByName("NonExistentMethodXYZ");
            
            // Test DebugString
            std::string method_debug_string = method->DebugString();
        }
        
        // Test service options (may be null if not set)
        const auto& options = service->options();
        
        // Test DebugString
        std::string service_debug_string = service->DebugString();
        
        // Try finding methods by name that might not exist
        if (fdp.remaining_bytes() > 0) {
            size_t random_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            std::string random_name = fdp.ConsumeBytesAsString(random_name_len);
            const MethodDescriptor* random_found = service->FindMethodByName(random_name);
        }
    }
    
    // Additional tests based on test_mode
    switch (test_mode) {
        case 0: {
            // Test copying service descriptor to proto
            if (service_count > 0) {
                const ServiceDescriptor* first_service = file_desc->service(0);
                google::protobuf::ServiceDescriptorProto service_copy;
                first_service->CopyTo(&service_copy);
                
                // Verify copied fields
                std::string copied_name = service_copy.name();
                int copied_method_count = service_copy.method_size();
            }
            break;
        }
        
        case 1: {
            // Test method descriptor to proto
            if (service_count > 0) {
                const ServiceDescriptor* first_service = file_desc->service(0);
                if (first_service->method_count() > 0) {
                    const MethodDescriptor* first_method = first_service->method(0);
                    google::protobuf::MethodDescriptorProto method_copy;
                    first_method->CopyTo(&method_copy);
                    
                    std::string copied_method_name = method_copy.name();
                    std::string copied_input_type = method_copy.input_type();
                    std::string copied_output_type = method_copy.output_type();
                    bool copied_client_streaming = method_copy.client_streaming();
                    bool copied_server_streaming = method_copy.server_streaming();
                }
            }
            break;
        }
        
        case 2: {
            // Test file descriptor service access
            for (int i = 0; i < service_count; i++) {
                const ServiceDescriptor* service = file_desc->service(i);
                
                // Test service file relationship
                const FileDescriptor* service_file = service->file();
                bool same_file = (service_file == file_desc);
                
                // Test package name
                std::string package_name = std::string(file_desc->package());
                
                // Test that service belongs to this file
                int service_index_in_file = service->index();
            }
            break;
        }
    }
    
    // Test with dynamic descriptor creation using remaining fuzzer data
    if (fdp.remaining_bytes() > 100) {
        FileDescriptorProto dynamic_file_proto;
        dynamic_file_proto.set_name("dynamic.proto");
        dynamic_file_proto.set_package("dynamic.fuzz");
        
        // Add a simple message
        auto* dyn_msg = dynamic_file_proto.add_message_type();
        dyn_msg->set_name("DynMessage");
        
        // Add a service with a single method
        auto* dyn_service = dynamic_file_proto.add_service();
        dyn_service->set_name("DynService");
        
        auto* dyn_method = dyn_service->add_method();
        size_t dyn_method_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
        std::string dyn_method_name = fdp.ConsumeBytesAsString(dyn_method_name_len);
        if (dyn_method_name.empty()) dyn_method_name = "DynMethod";
        dyn_method->set_name(dyn_method_name);
        dyn_method->set_input_type(".dynamic.fuzz.DynMessage");
        dyn_method->set_output_type(".dynamic.fuzz.DynMessage");
        
        // Try to build it
        const FileDescriptor* dyn_file_desc = pool.BuildFile(dynamic_file_proto);
        if (dyn_file_desc) {
            if (dyn_file_desc->service_count() > 0) {
                const ServiceDescriptor* dyn_service_desc = dyn_file_desc->service(0);
                std::string dyn_service_name = std::string(dyn_service_desc->name());
                
                if (dyn_service_desc->method_count() > 0) {
                    const MethodDescriptor* dyn_method_desc = dyn_service_desc->method(0);
                    std::string dyn_method_desc_name = std::string(dyn_method_desc->name());
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_005
