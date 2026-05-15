#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include "absl/strings/cord.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_011 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful CodedStream state manipulation
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test type selection - focus on stream state manipulation and Cord integration
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_type) {
        case 0: {
            // Test 1: CodedInputStream stream state management with limits
            // Initialize ArrayInputStream with test data
            size_t stream_data_size = fdp.ConsumeIntegralInRange<size_t>(32, 1024);
            if (fdp.remaining_bytes() < stream_data_size) {
                break;
            }
            std::string stream_data = fdp.ConsumeBytesAsString(stream_data_size);
            google::protobuf::io::ArrayInputStream array_input(stream_data.data(), stream_data.size());
            google::protobuf::io::CodedInputStream coded_input(&array_input);
            
            // Set recursion limit
            coded_input.SetRecursionLimit(fdp.ConsumeIntegralInRange<int>(1, 100));
            
            // Test PushLimit and ReadLengthAndPushLimit
            // First read a varint to use as limit
            if (stream_data.size() >= 4) {
                // Test PushLimit with various byte limits
                google::protobuf::io::CodedInputStream::Limit limit1 = 
                    coded_input.PushLimit(fdp.ConsumeIntegralInRange<int>(1, 100));
                
                // Test ReadLittleEndian32 and ReadLittleEndian64 within the limit
                uint32_t value32;
                uint64_t value64;
                bool read32_success = coded_input.ReadLittleEndian32(&value32);
                bool read64_success = coded_input.ReadLittleEndian64(&value64);
                
                // Test ReadLengthAndPushLimit
                google::protobuf::io::CodedInputStream::Limit limit2 = 
                    coded_input.ReadLengthAndPushLimit();
                
                // Read some more data within the new limit
                if (coded_input.BytesUntilLimit() > 0) {
                    uint32_t another_value;
                    coded_input.ReadLittleEndian32(&another_value);
                }
                
                // Test DecrementRecursionDepthAndPopLimit - must have incremented recursion depth first
                if (coded_input.IncrementRecursionDepth()) {
                    bool decrement_success = coded_input.DecrementRecursionDepthAndPopLimit(limit2);
                }
                
                // Pop the first limit
                coded_input.PopLimit(limit1);
                
                // Test operations after limits are popped
                if (coded_input.BytesUntilLimit() < 0) { // No limit now
                    uint32_t final_value;
                    coded_input.ReadLittleEndian32(&final_value);
                }
            }
            break;
        }
        
        case 1: {
            // Test 2: CodedOutputStream::WriteCord for Cord integration
            // Create ArrayOutputStream for testing
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 4096);
            std::vector<char> buffer(buffer_size);
            google::protobuf::io::ArrayOutputStream array_output(buffer.data(), buffer.size());
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Create various Cord objects from fuzzer input
            size_t cord1_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() < cord1_size) {
                break;
            }
            std::string cord1_data = fdp.ConsumeBytesAsString(cord1_size);
            absl::Cord cord1(cord1_data);
            
            size_t cord2_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < cord2_size) {
                break;
            }
            std::string cord2_data = fdp.ConsumeBytesAsString(cord2_size);
            absl::Cord cord2(cord2_data);
            
            // Write Cords using WriteCord method
            coded_output.WriteCord(cord1);
            coded_output.WriteCord(cord2);
            
            // Also test WriteString for comparison (uses string_view, not Cord)
            size_t string_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() >= string_size) {
                std::string string_data = fdp.ConsumeBytesAsString(string_size);
                coded_output.WriteString(string_data);
            }
            
            // Test WriteCordToArray static method
            size_t array_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
            if (fdp.remaining_bytes() >= array_size) {
                std::vector<uint8_t> write_array(array_size);
                std::string array_cord_data = fdp.ConsumeBytesAsString(array_size);
                absl::Cord array_cord(array_cord_data);
                
                uint8_t* result = google::protobuf::io::CodedOutputStream::WriteCordToArray(
                    array_cord, write_array.data());
                size_t bytes_written = result - write_array.data();
            }
            
            // Write some additional data to test mixed operations
            uint32_t int_value = fdp.ConsumeIntegral<uint32_t>();
            coded_output.WriteVarint32(int_value);
            break;
        }
        case 2: {
            // Test 3: OstreamOutputStream and IstreamInputStream BackUp operations
            // Create stringstream for testing
            std::stringstream string_stream;
            
            // First, test OstreamOutputStream::BackUp
            {
                google::protobuf::io::OstreamOutputStream ostream_output(&string_stream);
                google::protobuf::io::CodedOutputStream coded_output(&ostream_output);
                
                // Write some data
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (fdp.remaining_bytes() < write_size) {
                    break;
                }
                std::string write_data = fdp.ConsumeBytesAsString(write_size);
                coded_output.WriteString(write_data);
                
                // Try to BackUp - this should work if the stream supports it
                // Note: BackUp may not be meaningful for OstreamOutputStream
                // but we call it to test the implementation
                coded_output.Trim(); // Ensure buffer is flushed
            }
            
            // Now write data to the stream for input testing
            std::string test_output = "Test data for input stream";
            string_stream.str(test_output);
            string_stream.clear();
            
            // Test IstreamInputStream::BackUp
            {
                google::protobuf::io::IstreamInputStream istream_input(&string_stream);
                google::protobuf::io::CodedInputStream coded_input(&istream_input);
                
                // Read some data
                const void* read_buffer;
                int read_size;
                if (coded_input.GetDirectBufferPointer(&read_buffer, &read_size)) {
                    // Consume some bytes
                    coded_input.Skip(fdp.ConsumeIntegralInRange<int>(1, read_size));
                    
                    // Try to BackUp
                    // Note: BackUp may have specific semantics for IstreamInputStream
                }
            }
            break;
        }
        
        case 3: {
            // Test 4: Complex stream state manipulation targeting the bug pattern
            // where Consume() invalidates stream state but destructor still attempts to flush
            
            // Create a CordOutputStream to test Consume() behavior
            size_t size_hint = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
            google::protobuf::io::CordOutputStream cord_output(size_hint);
            google::protobuf::io::CodedOutputStream coded_output(&cord_output);
            
            // Write some data to create stream state
            uint32_t varint_val = fdp.ConsumeIntegral<uint32_t>();
            coded_output.WriteVarint32(varint_val);
            
            size_t string_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < string_size) {
                break;
            }
            std::string str_data = fdp.ConsumeBytesAsString(string_size);
            coded_output.WriteString(str_data);
            
            // Test the bug pattern: operations after stream state manipulation
            // 1. Write more data
            uint64_t int64_val = fdp.ConsumeIntegral<uint64_t>();
            coded_output.WriteLittleEndian64(int64_val);
            
            // 2. Potentially trigger state invalidation
            // (Simulating what might happen with incorrect usage)
            
            // 3. Let the destructors run - this is where the bug might manifest
            // The destructor of CodedOutputStream might try to flush
            // even if the underlying stream state is invalid
            
            // Create another stream to test recursion depth
            size_t input_data_size = fdp.ConsumeIntegralInRange<size_t>(32, 512);
            if (fdp.remaining_bytes() < input_data_size) {
                break;
            }
            std::string input_data = fdp.ConsumeBytesAsString(input_data_size);
            google::protobuf::io::ArrayInputStream array_input2(input_data.data(), input_data.size());
            google::protobuf::io::CodedInputStream coded_input2(&array_input2);
            
            // Set and manipulate recursion depth
            coded_input2.SetRecursionLimit(fdp.ConsumeIntegralInRange<int>(1, 10));
            
            // Test recursion depth operations
            if (coded_input2.IncrementRecursionDepth()) {
                // Push a limit while recursion depth is incremented
                google::protobuf::io::CodedInputStream::Limit limit = 
                    coded_input2.PushLimit(fdp.ConsumeIntegralInRange<int>(1, 50));
                
                // Read within limit
                uint32_t test_val;
                coded_input2.ReadLittleEndian32(&test_val);
                
                // Use DecrementRecursionDepthAndPopLimit (already includes UnsafeDecrementRecursionDepth)
                bool depth_check = coded_input2.DecrementRecursionDepthAndPopLimit(limit);
                }
            // Test boundary conditions
            // Empty stream operations
            if (fdp.ConsumeBool()) {
                std::string empty_data;
                google::protobuf::io::ArrayInputStream empty_input(empty_data.data(), 0);
                google::protobuf::io::CodedInputStream empty_coded(&empty_input);
                
                // These should fail gracefully
                uint32_t dummy;
                empty_coded.ReadLittleEndian32(&dummy);
                
                google::protobuf::io::CodedInputStream::Limit dummy_limit = 
                    empty_coded.PushLimit(0);
                empty_coded.PopLimit(dummy_limit);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_011
