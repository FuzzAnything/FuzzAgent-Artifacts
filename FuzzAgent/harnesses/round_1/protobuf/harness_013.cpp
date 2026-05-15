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
#include "google/protobuf/io/zero_copy_sink.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_013 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive Cord and stream operations
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test type selection - focus on EpsCopyOutputStream Cord operations and stream integration
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (test_type) {
        case 0: {
            // Test 1: EpsCopyOutputStream::WriteString with Cord parameter
            // This is the primary target with 48 branches, 18 undiscovered
            
            // Create output buffer
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 4096);
            std::vector<char> buffer(buffer_size);
            google::protobuf::io::ArrayOutputStream array_output(buffer.data(), buffer.size());
            
            // Create CodedOutputStream which internally uses EpsCopyOutputStream
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Create Cord objects with various data sources
            size_t cord1_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            if (fdp.remaining_bytes() < cord1_size) {
                break;
            }
            std::string cord1_data = fdp.ConsumeBytesAsString(cord1_size);
            absl::Cord cord1(cord1_data);
            
            // Create empty cord
            absl::Cord empty_cord;
            
            // Create cord from string view
            size_t cord2_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() < cord2_size) {
                break;
            }
            std::string cord2_data = fdp.ConsumeBytesAsString(cord2_size);
            absl::Cord cord2(cord2_data);
            
            // Test Cord assignment operation (6 branches)
            absl::Cord cord3;
            cord3 = cord1;  // assignment operator
            
            // Test Cord comparison operation (8 branches, 5 undiscovered)
            bool cords_equal = (cord1 == cord2);
            bool cords_not_equal = (cord1 != cord2);
            
            // Test Cord::TryFlat operation (12 branches, 6 undiscovered)
            auto flat1 = cord1.TryFlat();
            auto flat2 = cord2.TryFlat();
            auto flat_empty = empty_cord.TryFlat();
            
            // Enable aliasing on the stream (4 branches)
            // Note: Need to access EpsCopyOutputStream through CodedOutputStream
            coded_output.EnableAliasing(fdp.ConsumeBool());
            // Write Cord data using CodedOutputStream::WriteCord
            // This internally calls EpsCopyOutputStream::WriteString with Cord parameter
            coded_output.WriteCord(cord1);
            coded_output.WriteCord(cord2);
            coded_output.WriteCord(empty_cord);
            coded_output.WriteCord(cord3);
            
            // Write raw data with aliasing support
            size_t raw_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < raw_data_size) {
                break;
            }
            std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
            
            // Write raw data - WriteAliasedRaw will be called if aliasing is enabled
            coded_output.WriteRaw(raw_data.data(), raw_data.size());
            
            // Flush the stream
            coded_output.Trim();
            
            break;
        }
        
        case 1: {
            // Test 2: ZeroCopyStreamByteSink operations
            // Create StringOutputStream for testing
            std::string output_str;
            google::protobuf::io::StringOutputStream string_output(&output_str);
            
            // Create ZeroCopyStreamByteSink (internal class)
            google::protobuf::io::zc_sink_internal::ZeroCopyStreamByteSink sink(&string_output);
            
            // Write data using ZeroCopyStreamByteSink::Write (8 branches)
            size_t write1_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() < write1_size) {
                break;
            }
            std::string write1_data = fdp.ConsumeBytesAsString(write1_size);
            sink.Write(write1_data);
            
            // Write more data
            size_t write2_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < write2_size) {
                break;
            }
            std::string write2_data = fdp.ConsumeBytesAsString(write2_size);
            sink.Append(write2_data.data(), write2_data.size());
            
            // Check bytes written
            size_t bytes_written = sink.bytes_written();
            bool failed = sink.failed();
            
            // Test with Cord data through CodedOutputStream
            std::string cord_str;
            google::protobuf::io::StringOutputStream cord_output(&cord_str);
            google::protobuf::io::CodedOutputStream coded_cord_output(&cord_output);
            
            size_t cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 384);
            if (fdp.remaining_bytes() < cord_size) {
                break;
            }
            std::string cord_data = fdp.ConsumeBytesAsString(cord_size);
            absl::Cord cord(cord_data);
            
            coded_cord_output.WriteCord(cord);
            coded_cord_output.Trim();
            
            break;
        }
        
        case 2: {
            // Test 3: IstreamInputStream::Skip operations (12 branches)
            // Create string stream from fuzzer data
            size_t stream_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 1024);
            if (fdp.remaining_bytes() < stream_data_size) {
                break;
            }
            std::string stream_data = fdp.ConsumeBytesAsString(stream_data_size);
            
            // Create istringstream and IstreamInputStream
            std::istringstream iss(stream_data);
            google::protobuf::io::IstreamInputStream istream_input(&iss);
            
            // Wrap with CodedInputStream for Skip operations
            google::protobuf::io::CodedInputStream coded_input(&istream_input);
            
            // Test various skip operations
            size_t skip1 = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(100, stream_data_size / 2));
            bool skip1_success = coded_input.Skip(skip1);
            
            // Skip more data
            size_t skip2 = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(50, stream_data_size / 4));
            bool skip2_success = coded_input.Skip(skip2);
            
            // Try to skip beyond available data
            size_t skip3 = fdp.ConsumeIntegralInRange<size_t>(stream_data_size, stream_data_size * 2);
            bool skip3_success = coded_input.Skip(skip3);
            
            // Test reading some data after skipping
            const void* buffer_ptr;
            int buffer_size;
            if (coded_input.GetDirectBufferPointer(&buffer_ptr, &buffer_size)) {
                // Read a varint if available
                uint32_t varint_value;
                bool read_varint = coded_input.ReadVarint32(&varint_value);
                
                // Skip based on consumed value
                if (read_varint && varint_value > 0 && varint_value < 100) {
                    coded_input.Skip(varint_value);
                }
            }
            
            break;
        }
        
        case 3: {
            // Test 4: ZeroCopyOutputStream::WriteAliasedRaw (6 branches)
            // and EpsCopyOutputStream::EnableAliasing combination
            
            // Create ArrayOutputStream
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
            std::vector<char> buffer(buffer_size);
            google::protobuf::io::ArrayOutputStream array_output(buffer.data(), buffer.size());
            
            // Create CodedOutputStream
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Enable aliasing based on fuzzer input
            bool enable_aliasing = fdp.ConsumeBool();
            coded_output.EnableAliasing(enable_aliasing);
            
            // Create data to write
            size_t data1_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() < data1_size) {
                break;
            }
            std::vector<uint8_t> data1 = fdp.ConsumeBytes<uint8_t>(data1_size);
            
            size_t data2_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() < data2_size) {
                break;
            }
            std::vector<uint8_t> data2 = fdp.ConsumeBytes<uint8_t>(data2_size);
            
            // Write data - WriteAliasedRaw may be called internally if aliasing is enabled
            coded_output.WriteRaw(data1.data(), data1.size());
            coded_output.WriteRaw(data2.data(), data2.size());
            
            // Write Cord data as well
            size_t cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 384);
            if (fdp.remaining_bytes() < cord_size) {
                break;
            }
            std::string cord_data = fdp.ConsumeBytesAsString(cord_size);
            absl::Cord cord(cord_data);
            
            coded_output.WriteCord(cord);
            
            // Write string data
            size_t str_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() < str_size) {
                break;
            }
            std::string str_data = fdp.ConsumeBytesAsString(str_size);
            coded_output.WriteString(str_data);
            
            // Flush
            coded_output.Trim();
            
            // Test with StringOutputStream as well
            std::string str_output;
            google::protobuf::io::StringOutputStream string_output(&str_output);
            google::protobuf::io::CodedOutputStream coded_output2(&string_output);
            
            coded_output2.EnableAliasing(fdp.ConsumeBool());
            coded_output2.WriteRaw(data1.data(), data1.size());
            coded_output2.Trim();
            
            break;
        }
        
        case 4: {
            // Test 5: Comprehensive Cord and stream integration
            // Testing all targeted APIs together in a realistic workflow
            
            // Create multiple Cord objects with different characteristics
            absl::Cord cords[4];
            
            // Cord 0: Empty cord
            // Cord 1: Small cord
            size_t cord1_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            if (fdp.remaining_bytes() < cord1_size) {
                break;
            }
            std::string cord1_data = fdp.ConsumeBytesAsString(cord1_size);
            cords[1] = absl::Cord(cord1_data);
            
            // Cord 2: Medium cord  
            size_t cord2_size = fdp.ConsumeIntegralInRange<size_t>(129, 512);
            if (fdp.remaining_bytes() < cord2_size) {
                break;
            }
            std::string cord2_data = fdp.ConsumeBytesAsString(cord2_size);
            cords[2] = absl::Cord(cord2_data);
            
            // Cord 3: Large cord (if enough data)
            size_t cord3_size = fdp.ConsumeIntegralInRange<size_t>(513, 1024);
            if (fdp.remaining_bytes() >= cord3_size) {
                std::string cord3_data = fdp.ConsumeBytesAsString(cord3_size);
                cords[3] = absl::Cord(cord3_data);
            }
            
            // Test Cord operations
            for (int i = 0; i < 4; i++) {
                // TryFlat on each cord
                auto flat = cords[i].TryFlat();
                
                // Compare cords
                for (int j = i + 1; j < 4; j++) {
                    bool equal = (cords[i] == cords[j]);
                    bool not_equal = (cords[i] != cords[j]);
                }
            }
            
            // Test assignment
            absl::Cord assigned_cord;
            assigned_cord = cords[1];
            
            // Create output streams
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(2048, 8192);
            std::vector<char> buffer(buffer_size);
            google::protobuf::io::ArrayOutputStream array_output(buffer.data(), buffer.size());
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Enable aliasing
            coded_output.EnableAliasing(fdp.ConsumeBool());
            
            // Write all cords to stream (testing EpsCopyOutputStream::WriteString with Cord)
            for (int i = 0; i < 4; i++) {
                coded_output.WriteCord(cords[i]);
            }
            
            // Write assigned cord
            coded_output.WriteCord(assigned_cord);
            
            // Write raw data with potential aliasing
            size_t raw_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() >= raw_size) {
                std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_size);
                coded_output.WriteRaw(raw_data.data(), raw_size);
            }
            
            // Create ZeroCopyStreamByteSink for additional testing
            std::string sink_str;
            google::protobuf::io::StringOutputStream sink_output(&sink_str);
            google::protobuf::io::zc_sink_internal::ZeroCopyStreamByteSink sink(&sink_output);
            
            // Write to sink
            if (fdp.remaining_bytes() > 0) {
                size_t sink_data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(512, fdp.remaining_bytes()));
                std::string sink_data = fdp.ConsumeBytesAsString(sink_data_size);
                sink.Write(sink_data);
            }
            
            // Test IstreamInputStream::Skip
            if (fdp.remaining_bytes() > 100) {
                size_t skip_data_size = fdp.ConsumeIntegralInRange<size_t>(100, 500);
                if (fdp.remaining_bytes() >= skip_data_size) {
                    std::string skip_data = fdp.ConsumeBytesAsString(skip_data_size);
                    std::istringstream skip_iss(skip_data);
                    google::protobuf::io::IstreamInputStream skip_istream(&skip_iss);
                    google::protobuf::io::CodedInputStream skip_coded(&skip_istream);
                    
                    size_t skip_amount = fdp.ConsumeIntegralInRange<size_t>(1, skip_data_size / 2);
                    skip_coded.Skip(skip_amount);
                }
            }
            
            coded_output.Trim();
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_013
