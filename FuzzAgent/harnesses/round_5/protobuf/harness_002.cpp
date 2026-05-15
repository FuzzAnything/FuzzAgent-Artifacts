#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "absl/strings/cord.h"
#include "absl/strings/cord_buffer.h"

namespace harness_002 {
// Protocol Buffers fuzzing harness for Cord-based I/O operations
// This harness targets CordInputStream and CordOutputStream APIs identified as having
// 196+ undiscovered branches across Cord-based stream operations



using google::protobuf::io::CordInputStream;
using google::protobuf::io::CordOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful Cord operations
    if (size < 32) return 0;  // Need enough for multiple parameters
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into multiple parts:
    // 1. For creating initial test data in source Cord
    // 2. For various parameters (skip amounts, backup amounts, etc.)
    // 3. For additional test data to write
    
    // First, determine how much data to use for source Cord
    size_t cord_data_size = fdp.ConsumeIntegralInRange<size_t>(16, std::min(size, (size_t)4096));
    std::vector<uint8_t> source_data = fdp.ConsumeBytes<uint8_t>(cord_data_size);
    if (source_data.empty()) return 0;
    
    // Step 1: Create test data using absl::CordBuffer and append to absl::Cord
    absl::Cord source_cord;
    
    // Use CordBuffer to efficiently create data
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(8, 1024);
    absl::CordBuffer buffer = absl::CordBuffer::CreateWithDefaultLimit(buffer_size);
    
    if (buffer.capacity() > 0) {
        // Fill buffer with fuzzer data
        size_t fill_size = std::min(buffer.capacity(), fdp.remaining_bytes());
        if (fill_size > 0) {
            std::vector<uint8_t> fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
            memcpy(buffer.data(), fill_data.data(), fill_size);
            buffer.SetLength(fill_size);
            
            // Append buffer to Cord
            source_cord.Append(std::move(buffer));
        }
    }
    
    // Also append the source_data directly to ensure we have content
    if (!source_data.empty()) {
        source_cord.Append(absl::string_view(reinterpret_cast<const char*>(source_data.data()), source_data.size()));
    }
    
    // Step 2: Instantiate CordInputStream from source Cord
    CordInputStream input_stream(&source_cord);
    
    // Step 3: Instantiate CordOutputStream to destination Cord
    CordOutputStream output_stream;
    
    // Variables to track reading/writing
    const void* read_buffer;
    int read_size;
    void* write_buffer;
    int write_size;
    
    // Step 4: Call CordInputStream::Next to read data
    bool has_data = input_stream.Next(&read_buffer, &read_size);
    if (has_data && read_size > 0) {
        // Step 5: Call CordInputStream::ReadCord to read into another Cord
        absl::Cord read_cord;
        int read_amount = fdp.ConsumeIntegralInRange<int>(0, read_size);
        input_stream.ReadCord(&read_cord, read_amount);
        
        // Step 6: Call CordInputStream::Skip (BackUp removed as it's not allowed after ReadCord)
        // According to ZeroCopyInputStream API, BackUp() is only allowed on last returned chunk from Next()
        // ReadCord consumes data from stream, making BackUp() invalid
        int skip_amount = fdp.ConsumeIntegralInRange<int>(0, 128);
        input_stream.Skip(skip_amount);

        // Step 7: Call CordOutputStream::Next to get write buffer
        bool can_write = output_stream.Next(&write_buffer, &write_size);
        if (can_write && write_size > 0) {
            // Write some data from fuzzer input
            size_t write_data_size = std::min(static_cast<size_t>(write_size), fdp.remaining_bytes());
            if (write_data_size > 0) {
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_data_size);
                memcpy(write_buffer, write_data.data(), write_data_size);
                
                // Step 8: Call CordOutputStream::BackUp to return unused buffer space
                // MUST be called directly after Next() according to API contract
                int backup_amount = fdp.ConsumeIntegralInRange<int>(0, write_data_size);
                output_stream.BackUp(backup_amount);
                
                // Step 9: Call CordOutputStream::WriteCord to write data
                // This must be after BackUp() since WriteCord() changes buffer state
                if (!read_cord.empty()) {
                    output_stream.WriteCord(read_cord);
                }
                
                // Try to write more data (after BackUp/WriteCord sequence is complete)
                if (output_stream.Next(&write_buffer, &write_size)) {
                    if (write_size > 0) {
                        size_t more_write_size = std::min(static_cast<size_t>(write_size), fdp.remaining_bytes());
                        if (more_write_size > 0) {
                            std::vector<uint8_t> more_data = fdp.ConsumeBytes<uint8_t>(more_write_size);
                            memcpy(write_buffer, more_data.data(), more_write_size);
                        }
                    }
                }
                
                // Consume the output
                absl::Cord output_cord = output_stream.Consume();
                (void)output_cord; // Use to avoid unused variable warning
            }
        }
    }
    // Additional testing: Create another CordOutputStream with size hint
    size_t size_hint = fdp.ConsumeIntegral<size_t>();
    CordOutputStream output_stream_with_hint(size_hint);
    
    // Test ByteCount on both streams
    int64_t input_byte_count = input_stream.ByteCount();
    int64_t output_byte_count = output_stream.ByteCount();
    int64_t output_with_hint_byte_count = output_stream_with_hint.ByteCount();
    
    (void)input_byte_count;
    (void)output_byte_count;
    (void)output_with_hint_byte_count;
    
    // Test edge cases with small Cords
    if (fdp.remaining_bytes() > 0) {
        absl::Cord small_cord;
        size_t small_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 16);
        if (small_data_size > 0 && small_data_size <= fdp.remaining_bytes()) {
            std::vector<uint8_t> small_data = fdp.ConsumeBytes<uint8_t>(small_data_size);
            small_cord.Append(absl::string_view(reinterpret_cast<const char*>(small_data.data()), small_data.size()));
            
            CordInputStream small_input(&small_cord);
            CordOutputStream small_output;
            
            // Try to read/write with small cord
            const void* small_buffer;
            int small_buffer_size;
            if (small_input.Next(&small_buffer, &small_buffer_size)) {
                void* small_out_buffer;
                int small_out_size;
                if (small_output.Next(&small_out_buffer, &small_out_size) && small_out_size > 0) {
                    size_t copy_size = std::min(static_cast<size_t>(small_buffer_size), static_cast<size_t>(small_out_size));
                    if (copy_size > 0) {
                        memcpy(small_out_buffer, small_buffer, copy_size);
                        small_output.BackUp(small_out_size - copy_size);
                    }
                }
            }
        }
    }
    
    return 0;
}

} // namespace harness_002
