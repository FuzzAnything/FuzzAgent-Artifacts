#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "absl/strings/cord.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_012 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive stream state manipulation
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test type selection - focus on EpsCopyOutputStream Cord serialization and stream state management
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_type) {
        case 0: {
            // Test 1: EpsCopyOutputStream Cord serialization through CodedOutputStream
            // Initialize ArrayOutputStream for testing
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 4096);
            std::vector<char> buffer(buffer_size);
            google::protobuf::io::ArrayOutputStream array_output(buffer.data(), buffer.size());
            
            // Create CodedOutputStream which internally uses EpsCopyOutputStream
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Create Cord objects from fuzzer input
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
            
            // Write Cord data using CodedOutputStream::WriteCord 
            // This internally calls EpsCopyOutputStream::WriteCord (WriteString for Cord)
            coded_output.WriteCord(cord1);
            coded_output.WriteCord(cord2);
            
            // Also test WriteString for comparison
            size_t str_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() < str_size) {
                break;
            }
            std::string str_data = fdp.ConsumeBytesAsString(str_size);
            coded_output.WriteString(str_data);
            
            // Test ByteCount queries on output stream
            // Note: ArrayOutputStream doesn't have ByteCount, but we can check with StringOutputStream
            std::string output_str;
            google::protobuf::io::StringOutputStream string_output(&output_str);
            google::protobuf::io::CodedOutputStream coded_output2(&string_output);
            
            // Write some data
            coded_output2.WriteCord(cord1);
            
            // Flush to ensure data is written
            coded_output2.Trim();
            
            // Check output size (indirect ByteCount check)
            size_t output_length = output_str.size();
            
            break;
        }
        
        case 1: {
            // Test 2: ConcatenatingInputStream and ArrayInputStream Skip operations
            // Create multiple input sources
            size_t data1_size = fdp.ConsumeIntegralInRange<size_t>(32, 256);
            if (fdp.remaining_bytes() < data1_size) {
                break;
            }
            std::string data1 = fdp.ConsumeBytesAsString(data1_size);
            
            size_t data2_size = fdp.ConsumeIntegralInRange<size_t>(32, 256);
            if (fdp.remaining_bytes() < data2_size) {
                break;
            }
            std::string data2 = fdp.ConsumeBytesAsString(data2_size);
            
            // Create ArrayInputStreams
            google::protobuf::io::ArrayInputStream array_input1(data1.data(), data1.size());
            google::protobuf::io::ArrayInputStream array_input2(data2.data(), data2.size());
            
            // Create a vector of ZeroCopyInputStream* for ConcatenatingInputStream
            std::vector<google::protobuf::io::ZeroCopyInputStream*> streams;
            streams.push_back(&array_input1);
            streams.push_back(&array_input2);
            
            // Create ConcatenatingInputStream
            google::protobuf::io::ConcatenatingInputStream concatenating_input(
                streams.data(), streams.size());
            
            // Wrap with CodedInputStream for Skip operations
            google::protobuf::io::CodedInputStream coded_input(&concatenating_input);
            
            // Test Skip operations with various sizes
            size_t skip1 = fdp.ConsumeIntegralInRange<size_t>(1, data1.size() / 2);
            bool skip1_success = coded_input.Skip(skip1);
            
            // Test ReadVarintSizeAsInt - read a varint to determine size
            uint32_t varint_size = 0;
            bool read_varint_success = coded_input.ReadVarint32(&varint_size);
            
            if (read_varint_success && varint_size > 0 && varint_size < 1024) {
                // Try to skip based on varint size
                coded_input.Skip(varint_size);
            }
            
            // Test BackUp after partial consumption
            const void* buffer_ptr;
            int buffer_size;
            if (coded_input.GetDirectBufferPointer(&buffer_ptr, &buffer_size)) {
                if (buffer_size > 10) {
                    // Consume some data
                    coded_input.Skip(5);
                    // Try to backup (though CodedInputStream doesn't have BackUp directly)
                    // The underlying stream should handle it
                }
            }
            
            break;
        }
        
        case 2: {
            // Test 3: StringOutputStream ByteCount and complex stream state
            std::string output_str;
            google::protobuf::io::StringOutputStream string_output(&output_str);
            
            // Test ByteCount queries during operations
            // Note: StringOutputStream doesn't have ByteCount method, but we can
            // check the underlying string size
            
            // Write some data to the stream
            size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() < write_size) {
                break;
            }
            std::string write_data = fdp.ConsumeBytesAsString(write_size);
            
            // Use CodedOutputStream to write
            google::protobuf::io::CodedOutputStream coded_output(&string_output);
            coded_output.WriteString(write_data);
            coded_output.Trim();
            
            // Check the output size
            size_t byte_count = output_str.size();
            
            // Create another string stream for ZeroCopyStreamByteSink test
            std::string sink_str;
            google::protobuf::io::StringOutputStream sink_output(&sink_str);
            
            // Note: ZeroCopyStreamByteSink is internal, but we can test through CodedOutputStream
            // Write more data with different patterns
            size_t cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < cord_size) {
                break;
            }
            std::string cord_data = fdp.ConsumeBytesAsString(cord_size);
            absl::Cord cord(cord_data);
            
            google::protobuf::io::CodedOutputStream coded_output2(&sink_output);
            coded_output2.WriteCord(cord);
            coded_output2.Trim();
            
            break;
        }
        
        case 3: {
            // Test 4: Complex stream state management with limits and recursion
            // Initialize ArrayInputStream with test data
            size_t stream_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
            if (fdp.remaining_bytes() < stream_data_size) {
                break;
            }
            std::string stream_data = fdp.ConsumeBytesAsString(stream_data_size);
            google::protobuf::io::ArrayInputStream array_input(stream_data.data(), stream_data.size());
            google::protobuf::io::CodedInputStream coded_input(&array_input);
            
            // Set recursion limit
            int recursion_limit = fdp.ConsumeIntegralInRange<int>(1, 50);
            coded_input.SetRecursionLimit(recursion_limit);
            
            // Test PushLimit and PopLimit with various byte limits
            int limit1 = fdp.ConsumeIntegralInRange<int>(1, 100);
            google::protobuf::io::CodedInputStream::Limit pushed_limit1 = 
                coded_input.PushLimit(limit1);
            
            // Read some data within the limit
            uint32_t value1;
            bool read1_success = coded_input.ReadLittleEndian32(&value1);
            
            // Test BytesUntilLimit
            int bytes_until_limit = coded_input.BytesUntilLimit();
            
            // Test ReadLengthAndPushLimit - read a varint and push limit based on it
            google::protobuf::io::CodedInputStream::Limit pushed_limit2 = 
                coded_input.ReadLengthAndPushLimit();
            
            // Read within the new limit
            if (coded_input.BytesUntilLimit() > 0) {
                uint32_t value2;
                coded_input.ReadLittleEndian32(&value2);
            }
            
            // Test recursion depth management
            if (coded_input.IncrementRecursionDepth()) {
                // Read more data within increased recursion depth
                uint32_t value3;
                coded_input.ReadLittleEndian32(&value3);
                
                // Decrement and pop limit
                coded_input.DecrementRecursionDepthAndPopLimit(pushed_limit2);
            }
            
            // Pop the first limit
            coded_input.PopLimit(pushed_limit1);
            
            // Test ReadVarintSizeAsInt by reading varints
            uint32_t varint1, varint2;
            coded_input.ReadVarint32(&varint1);
            coded_input.ReadVarint32(&varint2);
            
            // Test Skip after limits are popped
            size_t final_skip = fdp.ConsumeIntegralInRange<size_t>(1, 50);
            coded_input.Skip(final_skip);
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_012
