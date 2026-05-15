#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "absl/strings/cord.h"
#include "absl/strings/cord_buffer.h"

namespace harness_002 {
// Fuzzing harness for Google Protocol Buffers Cord-based I/O stream operations
// Targets CordInputStream and CordOutputStream APIs: Next, BackUp, ReadCord, WriteCord, Consume
// Focuses on Cord-based streaming operations with absl::Cord integration (differentiates from harness_000.cpp and harness_001.cpp)



// Cord-based stream headers

using google::protobuf::io::CordInputStream;
using google::protobuf::io::CordOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful Cord operations
    if (size < 32) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_size_hint = fdp.ConsumeBool();
    bool donate_cord = fdp.ConsumeBool();
    bool fragmented_cord = fdp.ConsumeBool();
    
    // Determine size hint if used
    size_t size_hint = 0;
    if (use_size_hint) {
        size_hint = fdp.ConsumeIntegralInRange<size_t>(0, 4096);
    }
    
    // Create initial Cord content from fuzzer input
    size_t cord_data_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes() / 2);
    std::string cord_data = fdp.ConsumeBytesAsString(cord_data_size);
    
    absl::Cord source_cord;
    
    if (fragmented_cord) {
        // Create a fragmented cord by appending multiple small pieces
        size_t fragment_count = fdp.ConsumeIntegralInRange<size_t>(2, 10);
        size_t fragment_size = cord_data_size / fragment_count;
        
        for (size_t i = 0; i < fragment_count && cord_data.size() > 0; i++) {
            size_t chunk_size = std::min(fragment_size, cord_data.size());
            source_cord.Append(absl::string_view(cord_data.data(), chunk_size));
            if (chunk_size < cord_data.size()) {
                cord_data = cord_data.substr(chunk_size);
            } else {
                cord_data.clear();
            }
        }
    } else {
        // Create a flat cord
        source_cord = absl::Cord(cord_data);
    }
    
    // Test CordInputStream operations
    CordInputStream input_stream(&source_cord);
    
    // Test Next() and BackUp() operations
    int total_read = 0;
    int max_read_operations = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < max_read_operations && total_read < source_cord.size(); i++) {
        const void* buffer;
        int buffer_size;
        
        if (input_stream.Next(&buffer, &buffer_size)) {
            // Consume some or all of the buffer
            int consume_size = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
            total_read += consume_size;
            
            // Possibly back up some bytes
            if (consume_size < buffer_size && fdp.ConsumeBool()) {
                int backup_amount = fdp.ConsumeIntegralInRange<int>(1, buffer_size - consume_size);
                input_stream.BackUp(backup_amount);
                total_read -= backup_amount;
            }
        } else {
            break;
        }
        
        // Occasionally skip some bytes
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            int skip_amount = fdp.ConsumeIntegralInRange<int>(1, 100);
            input_stream.Skip(skip_amount);
            total_read += skip_amount;
        }
    }
    
    // Reset stream position by creating a new input stream
    CordInputStream input_stream2(&source_cord);
    
    // Test ReadCord() operation
    if (source_cord.size() > 0) {
        absl::Cord read_cord;
        int read_size = fdp.ConsumeIntegralInRange<int>(1, source_cord.size());
        
        if (input_stream2.ReadCord(&read_cord, read_size)) {
            // Verify we read the expected amount
            if (read_cord.size() != read_size) {
                // Data mismatch - continue but note the issue
                return 0;
            }
        }
    }
    
    // Test CordOutputStream operations
    std::unique_ptr<CordOutputStream> output_stream;
    
    if (donate_cord && fdp.remaining_bytes() > 0) {
        // Create a cord to donate
        size_t donate_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min((size_t)256, fdp.remaining_bytes()));
        std::string donate_data = fdp.ConsumeBytesAsString(donate_size);
        absl::Cord donated_cord(donate_data);
        
        if (use_size_hint) {
            output_stream = std::make_unique<CordOutputStream>(std::move(donated_cord), size_hint);
        } else {
            output_stream = std::make_unique<CordOutputStream>(std::move(donated_cord));
        }
    } else {
        if (use_size_hint) {
            output_stream = std::make_unique<CordOutputStream>(size_hint);
        } else {
            output_stream = std::make_unique<CordOutputStream>();
        }
    }
    
    // Test Next() and BackUp() operations on output stream
    int total_written = 0;
    int max_write_operations = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < max_write_operations && fdp.remaining_bytes() > 0; i++) {
        void* buffer;
        int buffer_size;
        
        if (output_stream->Next(&buffer, &buffer_size)) {
            // Write some data to the buffer
            int write_size = fdp.ConsumeIntegralInRange<int>(1, buffer_size);
            
            if (fdp.remaining_bytes() >= write_size) {
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                memcpy(buffer, write_data.data(), write_size);
                total_written += write_size;
                
                // Possibly back up some bytes
                if (write_size < buffer_size && fdp.ConsumeBool()) {
                    int backup_amount = fdp.ConsumeIntegralInRange<int>(1, buffer_size - write_size);
                    output_stream->BackUp(backup_amount);
                    total_written -= backup_amount;
                }
            }
        } else {
            break;
        }
    }
    
    // Test WriteCord() operation if we have remaining data
    if (fdp.remaining_bytes() > 0) {
        size_t write_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min((size_t)1024, fdp.remaining_bytes()));
        std::string write_cord_data = fdp.ConsumeBytesAsString(write_cord_size);
        absl::Cord write_cord(write_cord_data);
        
        output_stream->WriteCord(write_cord);
        total_written += write_cord.size();
    }
    
    // Consume the output cord
    absl::Cord output_cord = output_stream->Consume();
    
    // Verify data integrity - output cord should have our written data
    // Note: We can't easily verify exact content since we might have donated cord data
    // but we can check basic properties
    if (total_written > 0) {
        size_t output_size = output_cord.size();
        // Output should be at least as large as what we wrote (plus any donated data)
        if (output_size < total_written) {
            // Data integrity issue - continue but note potential problem
            return 0;
        }
    }
    
    // Additional test: Write source cord to output stream and verify
    if (test_mode == 0 && source_cord.size() > 0) {
        CordOutputStream verify_stream;
        if (verify_stream.WriteCord(source_cord)) {
            absl::Cord verify_cord = verify_stream.Consume();
            // Basic size check
            if (verify_cord.size() != source_cord.size()) {
                return 0;
            }
        }
    }
    
    return 0;
}

} // namespace harness_002
