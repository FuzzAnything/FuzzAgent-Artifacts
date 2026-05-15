#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/coded_stream.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"

namespace harness_019 {
// Protocol Buffers fuzzing harness for Cord-based IO streaming operations
// This harness targets critical coverage gaps identified by CoverageAnalyzerAgent:
// 40+ undiscovered branch points in APIs: CordInputStream, LimitingInputStream, 
// ZeroCopyOutputStream, and EpsCopyOutputStream.
// Current API coverage: 29.59%
// Focus areas:
// - CordInputStream operations with ReadCord API
// - LimitingInputStream scenarios with stream boundaries and limit enforcement
// - ZeroCopyOutputStream optimizations with buffer management
// - EpsCopyOutputStream epsilon copy operations with error conditions
// Semantic differentiation from existing harnesses:
// - harness_015: Zero-copy stream operations (general)
// - harness_018: Cord lifecycle and stream integration
// - This harness: Cord-based IO streaming with focus on ReadCord, limit enforcement, 
//   epsilon copy optimization, and stream boundary error conditions



using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::StringOutputStream;
using google::protobuf::io::CordInputStream;
using google::protobuf::io::CordOutputStream;
using google::protobuf::io::LimitingInputStream;
using google::protobuf::io::CodedOutputStream;
// Note: EpsCopyOutputStream is used internally by CodedOutputStream
// We'll test it indirectly through CodedOutputStream operations
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for complex streaming operations:
    // Cord data + operation selection + limit values + buffer parameters
    if (size < 128) return 0;

    FuzzedDataProvider fdp(data, size);

    // Step 1: Create test Cord data for streaming operations
    size_t cord_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    std::string cord_data = fdp.ConsumeBytesAsString(cord_data_size);
    
    if (cord_data.empty() && fdp.remaining_bytes() > 32) {
        // Use remaining data if initial consumption didn't yield enough
        cord_data = fdp.ConsumeRemainingBytesAsString();
    }
    
    if (cord_data.empty()) return 0;  // Need some data for meaningful testing
    
    absl::Cord test_cord(cord_data);
    size_t cord_length = test_cord.size();

    // Step 2: Test CordInputStream with comprehensive ReadCord operations
    {
        CordInputStream cord_stream(&test_cord);
        
        // Test basic Next/BackUp operations on CordInputStream
        const void* buffer;
        int buffer_size;
        
        // First Next() call
        if (cord_stream.Next(&buffer, &buffer_size) && buffer_size > 0) {
            // Test BackUp with various amounts
            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
            if (backup_amount >= 0) {
                cord_stream.BackUp(backup_amount);  // Target API: CordInputStream::BackUp
            }
            
            // Test Skip with various amounts
            int skip_amount = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
            cord_stream.Skip(skip_amount);  // Target API: CordInputStream::Skip
        }
        
        // Test ByteCount accuracy
        int64_t bytes_read = cord_stream.ByteCount();
        (void)bytes_read;  // Use to avoid unused variable warning
        
        // Target API: CordInputStream::ReadCord - test with various counts
        if (cord_length > 0) {
            // Reset stream for ReadCord test
            absl::Cord fresh_cord(cord_data);
            CordInputStream fresh_stream(&fresh_cord);
            
            // Test ReadCord with different amounts
            absl::Cord read_cord;
            int read_count = fdp.ConsumeIntegralInRange<int>(1, static_cast<int>(cord_length));
            
            // Target API: CordInputStream::ReadCord (primary coverage gap)
            bool read_success = fresh_stream.ReadCord(&read_cord, read_count);
            (void)read_success;
            
            // Test partial reads and remaining data
            if (read_count < static_cast<int>(cord_length)) {
                // Try reading remaining data
                absl::Cord remaining_cord;
                int remaining_count = static_cast<int>(cord_length) - read_count;
                fresh_stream.ReadCord(&remaining_cord, remaining_count);
            }
        }
    }

    // Step 3: Test LimitingInputStream with various limit scenarios
    {
        // Create base stream from Cord data
        absl::Cord base_cord(cord_data);
        CordInputStream base_stream(&base_cord);
        
        // Test different limit scenarios
        int64_t limit_amount = fdp.ConsumeIntegralInRange<int64_t>(0, static_cast<int64_t>(cord_length * 2));
        
        // Target API: LimitingInputStream constructor
        LimitingInputStream limiting_stream(&base_stream, limit_amount);
        
        // Test LimitingInputStream operations
        const void* limit_buffer;
        int limit_buffer_size;
        
        if (limiting_stream.Next(&limit_buffer, &limit_buffer_size) && limit_buffer_size > 0) {
            // Test BackUp within limit
            int limit_backup = fdp.ConsumeIntegralInRange<int>(0, limit_buffer_size);
            if (limit_backup >= 0) {
                limiting_stream.BackUp(limit_backup);  // Target API: LimitingInputStream::BackUp
            }
            
            // Test Skip within limit
            int limit_skip = fdp.ConsumeIntegralInRange<int>(0, limit_buffer_size);
            limiting_stream.Skip(limit_skip);  // Target API: LimitingInputStream::Skip
            
            // Test ByteCount with limit enforcement
            int64_t limited_bytes = limiting_stream.ByteCount();
            (void)limited_bytes;
        }
        
        // Target API: LimitingInputStream::ReadCord (critical coverage gap)
        if (limit_amount > 0 && cord_length > 0) {
            // Reset streams for ReadCord test
            absl::Cord fresh_cord(cord_data);
            CordInputStream fresh_base_stream(&fresh_cord);
            LimitingInputStream fresh_limiting_stream(&fresh_base_stream, limit_amount);
            
            absl::Cord limited_cord;
            int read_cord_count = fdp.ConsumeIntegralInRange<int>(1, static_cast<int>(limit_amount));
            
            // Target API: LimitingInputStream::ReadCord
            bool limited_read_success = fresh_limiting_stream.ReadCord(&limited_cord, read_cord_count);
            (void)limited_read_success;
        }
    }

    // Step 4: Test ZeroCopyOutputStream optimizations with buffer management
    {
        // Create output buffer for ArrayOutputStream
        size_t output_buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
        std::vector<uint8_t> output_buffer(output_buffer_size, 0);
        
        // Target API: ArrayOutputStream constructor
        ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
        
        // Test basic Next/BackUp operations
        void* out_buffer;
        int out_buffer_size;
        
        if (array_output.Next(&out_buffer, &out_buffer_size) && out_buffer_size > 0) {
            // Write some data to the buffer
            size_t write_amount = fdp.ConsumeIntegralInRange<size_t>(1, static_cast<size_t>(out_buffer_size));
            if (write_amount <= static_cast<size_t>(out_buffer_size)) {
                // Consume data for writing
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_amount);
                if (!write_data.empty()) {
                    memcpy(out_buffer, write_data.data(), write_data.size());
                    
                    // Test BackUp after writing
                    int out_backup = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(write_amount));
                    if (out_backup >= 0) {
                        array_output.BackUp(out_backup);  // Target API: ZeroCopyOutputStream::BackUp
                    }
                }
            }
        }
        
        // Test ByteCount accuracy
        int64_t bytes_written = array_output.ByteCount();
        (void)bytes_written;
        
        // Test StringOutputStream as another ZeroCopyOutputStream implementation
        std::string string_output;
        StringOutputStream string_stream(&string_output);
        
        if (string_stream.Next(&out_buffer, &out_buffer_size) && out_buffer_size > 0) {
            // Write to string output
            size_t string_write = fdp.ConsumeIntegralInRange<size_t>(1, static_cast<size_t>(out_buffer_size));
            if (string_write <= static_cast<size_t>(out_buffer_size)) {
                std::vector<uint8_t> string_data = fdp.ConsumeBytes<uint8_t>(string_write);
                if (!string_data.empty()) {
                    memcpy(out_buffer, string_data.data(), string_data.size());
                    
                    // Adjust position with BackUp
                    int string_backup = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(string_write));
                    if (string_backup >= 0) {
                        string_stream.BackUp(string_backup);
                    }
                }
            }
        }
    }

    // Step 5: Test EpsCopyOutputStream epsilon copy operations and error conditions
    {
        // Create output buffer for testing EpsCopyOutputStream
        size_t eps_buffer_size = fdp.ConsumeIntegralInRange<size_t>(128, 2048);
        std::vector<uint8_t> eps_buffer(eps_buffer_size, 0);
        
        // Create ArrayOutputStream as underlying stream
        ArrayOutputStream array_stream(eps_buffer.data(), eps_buffer.size());
        
        // Create CodedOutputStream which internally uses EpsCopyOutputStream
        CodedOutputStream coded_stream(&array_stream);
        
        // Test various write operations that exercise EpsCopyOutputStream internally
        uint8_t* eps_ptr = nullptr;
        
        // Get EpsCopyOutputStream instance from CodedOutputStream
        // Note: EpsCopyOutputStream is typically accessed through CodedOutputStream's internal API
        // We'll test it indirectly through CodedOutputStream operations
        
        // Test writing varints (uses EpsCopyOutputStream internally)
        uint32_t test_varint = fdp.ConsumeIntegral<uint32_t>();
        coded_stream.WriteVarint32(test_varint);
        
        // Test writing raw data (may use WriteRawMaybeAliased which uses EpsCopyOutputStream)
        size_t raw_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
        
        if (!raw_data.empty()) {
            coded_stream.WriteRaw(raw_data.data(), raw_data.size());
        }
        
        // Test writing fixed-size data (uses EpsCopyOutputStream optimizations)
        if (fdp.remaining_bytes() >= sizeof(uint32_t)) {
            uint32_t fixed_data = fdp.ConsumeIntegral<uint32_t>();
            coded_stream.WriteLittleEndian32(fixed_data);
        }
        
        // Test error detection through HadError API
        bool has_error = coded_stream.HadError();
        (void)has_error;
        
        // Test buffer boundary conditions by attempting to write more than capacity
        if (fdp.remaining_bytes() > 0) {
            // Create a small buffer to test overflow conditions
            std::vector<uint8_t> small_buffer(16, 0);
            ArrayOutputStream small_stream(small_buffer.data(), small_buffer.size());
            CodedOutputStream small_coded(&small_stream);
            
            // Try writing data that likely exceeds buffer capacity
            std::vector<uint8_t> large_data(1024, 0xAA);  // Larger than buffer
            small_coded.WriteRaw(large_data.data(), large_data.size());
            
            // Check error state after likely overflow
            bool overflow_error = small_coded.HadError();
            (void)overflow_error;
        }
    }

    // Step 6: Test integrated streaming scenario with Cord input and various outputs
    {
        // Create Cord from remaining data
        std::string remaining_data = fdp.ConsumeRemainingBytesAsString();
        if (!remaining_data.empty()) {
            absl::Cord integrated_cord(remaining_data);
            CordInputStream input_stream(&integrated_cord);
            
            // Create multiple output streams for testing
            std::vector<uint8_t> integrated_buffer(256, 0);
            ArrayOutputStream array_out(integrated_buffer.data(), integrated_buffer.size());
            
            std::string string_out;
            StringOutputStream string_stream(&string_out);
            
            // Test reading from Cord and writing to different outputs
            const void* read_buffer;
            int read_size;
            
            if (input_stream.Next(&read_buffer, &read_size) && read_size > 0) {
                // Write to array output
                void* array_write;
                int array_write_size;
                
                if (array_out.Next(&array_write, &array_write_size) && array_write_size > 0) {
                    size_t copy_size = std::min(static_cast<size_t>(read_size), 
                                                static_cast<size_t>(array_write_size));
                    memcpy(array_write, read_buffer, copy_size);
                    
                    if (copy_size < static_cast<size_t>(read_size)) {
                        // BackUp unused portion in input
                        input_stream.BackUp(read_size - static_cast<int>(copy_size));
                    }
                }
                
                // Also write to string output
                void* string_write;
                int string_write_size;
                
                if (string_stream.Next(&string_write, &string_write_size) && string_write_size > 0) {
                    size_t string_copy = std::min(static_cast<size_t>(read_size), 
                                                  static_cast<size_t>(string_write_size));
                    memcpy(string_write, read_buffer, string_copy);
                }
            }
        }
    }

    return 0;
}

} // namespace harness_019
