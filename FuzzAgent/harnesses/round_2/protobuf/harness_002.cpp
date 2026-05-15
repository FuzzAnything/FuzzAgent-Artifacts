#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/gzip_stream.h"
#include "google/protobuf/io/coded_stream.h"
#include "absl/strings/cord.h"
#include "absl/strings/cord_buffer.h"

namespace harness_002 {
// Harness for Google Protocol Buffers Cord-based I/O streaming with Gzip compression
// Targets I/O streaming operations as identified by coverage analysis
// Semantic differentiation from harness_000 (binary parsing) and harness_001 (text parsing): 
// Focus on Cord I/O streaming and compression infrastructure


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which I/O streaming pattern to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume configuration options
    bool use_gzip_compression = fdp.ConsumeBool();
    bool use_size_hint = fdp.ConsumeBool();
    bool fragmented_cord = fdp.ConsumeBool();
    bool test_write_cord = fdp.ConsumeBool();
    bool test_read_cord = fdp.ConsumeBool();
    
    // Consume size hint if enabled
    size_t size_hint = 0;
    if (use_size_hint && fdp.remaining_bytes() >= sizeof(size_t)) {
        size_hint = fdp.ConsumeIntegralInRange<size_t>(0, 65536);
    }
    
    // Consume Gzip format for GzipInputStream
    google::protobuf::io::GzipInputStream::Format gzip_input_format = 
        static_cast<google::protobuf::io::GzipInputStream::Format>(
            fdp.ConsumeIntegralInRange<int>(0, 2));
    
    // Consume Gzip format for GzipOutputStream (1=GZIP, 2=ZLIB)
    int gzip_output_format_val = fdp.ConsumeIntegralInRange<int>(1, 2);
    
    // Consume data for Cord content
    std::string cord_content;
    if (fdp.remaining_bytes() > 0) {
        size_t content_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(fdp.remaining_bytes(), 65536));
        cord_content = fdp.ConsumeBytesAsString(content_size);
    } else {
        return 0;
    }
    
    // Create source Cord from consumed data
    absl::Cord source_cord;
    if (fragmented_cord && cord_content.size() > 4) {
        // Create fragmented Cord by appending in chunks
        size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, cord_content.size() / 4 + 1);
        size_t pos = 0;
        while (pos < cord_content.size()) {
            size_t chunk_len = std::min(chunk_size, cord_content.size() - pos);
            source_cord.Append(absl::string_view(cord_content.data() + pos, chunk_len));
            pos += chunk_len;
        }
    } else {
        source_cord = absl::Cord(cord_content);
    }
    
    // Test different I/O operations based on operation type
    switch (operation) {
        case 0: {
            // Basic CordOutputStream write and CordInputStream read
            google::protobuf::io::CordOutputStream cord_output(size_hint);
            
            // Write data to CordOutputStream using Next() and BackUp()
            void* output_buffer;
            int output_size;
            if (cord_output.Next(&output_buffer, &output_size)) {
                size_t bytes_to_write = std::min<size_t>(output_size, cord_content.size());
                memcpy(output_buffer, cord_content.data(), bytes_to_write);
                
                // Test BackUp() with random amount
                if (bytes_to_write > 0 && fdp.ConsumeBool()) {
                    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                    cord_output.BackUp(backup_amount);
                }
            }
            
            // Test WriteCord() if enabled
            if (test_write_cord && fdp.ConsumeBool()) {
                cord_output.WriteCord(source_cord);
            }
            
            // Consume the written Cord
            absl::Cord written_cord = cord_output.Consume();
            
            // Read back using CordInputStream
            const absl::Cord* cord_to_read = &written_cord;
            google::protobuf::io::CordInputStream cord_input(cord_to_read);
            
            // Read data using Next()
            const void* input_buffer;
            int input_size;
            while (cord_input.Next(&input_buffer, &input_size)) {
                // Test BackUp() on input
                if (input_size > 0 && fdp.ConsumeBool()) {
                    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, input_size);
                    cord_input.BackUp(backup_amount);
                }
                
                // Test Skip()
                if (input_size > 0 && fdp.ConsumeBool()) {
                    int skip_amount = fdp.ConsumeIntegralInRange<int>(0, input_size);
                    cord_input.Skip(skip_amount);
                }
            }
            
            break;
        }
        
        case 1: {
            // Test ReadCord() from CordInputStream
            const absl::Cord* cord_to_read = &source_cord;
            google::protobuf::io::CordInputStream cord_input(cord_to_read);
            
            if (test_read_cord && cord_content.size() > 4) {
                // Skip some bytes first
                size_t skip_amount = fdp.ConsumeIntegralInRange<size_t>(0, cord_content.size() / 2);
                cord_input.Skip(skip_amount);
                
                // Read a Cord segment
                absl::Cord read_cord;
                size_t read_amount = fdp.ConsumeIntegralInRange<size_t>(
                    1, cord_content.size() - skip_amount);
                cord_input.ReadCord(&read_cord, read_amount);
            }
            
            break;
        }
        
        case 2: {
            // Test GzipOutputStream compression followed by GzipInputStream decompression
            // Create a CordOutputStream to hold compressed data
            google::protobuf::io::CordOutputStream compressed_output(size_hint);
            // Wrap with GzipOutputStream for compression
            google::protobuf::io::GzipOutputStream::Options gzip_options;
            gzip_options.format = static_cast<google::protobuf::io::GzipOutputStream::Format>(gzip_output_format_val);
            google::protobuf::io::GzipOutputStream gzip_output(
                &compressed_output,
                gzip_options);
            
            // Write data through GzipOutputStream
            void* gzip_buffer;
            int gzip_size;
            if (gzip_output.Next(&gzip_buffer, &gzip_size)) {
                size_t bytes_to_write = std::min<size_t>(gzip_size, cord_content.size());
                memcpy(gzip_buffer, cord_content.data(), bytes_to_write);
                
                // Test BackUp() on GzipOutputStream
                if (bytes_to_write > 0 && fdp.ConsumeBool()) {
                    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, bytes_to_write);
                    gzip_output.BackUp(backup_amount);
                }
            }
            
            // Get compressed Cord
            absl::Cord compressed_cord = compressed_output.Consume();
            
            // Decompress using GzipInputStream
            if (compressed_cord.size() > 0) {
                const absl::Cord* compressed_cord_ptr = &compressed_cord;
                google::protobuf::io::CordInputStream compressed_input(compressed_cord_ptr);
                
                google::protobuf::io::GzipInputStream gzip_input(
                    &compressed_input,
                    gzip_input_format);
                // Read decompressed data
                const void* decompressed_buffer;
                int decompressed_size;
                while (gzip_input.Next(&decompressed_buffer, &decompressed_size)) {
                    // Process decompressed data
                    if (decompressed_size > 0 && fdp.ConsumeBool()) {
                        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, decompressed_size);
                        gzip_input.BackUp(backup_amount);
                    }
                }
            }
            
            break;
        }
        
        case 3: {
            // Test CordOutputStream with initial Cord
            std::string initial_str = fdp.ConsumeRandomLengthString(32);
            absl::Cord initial_cord(initial_str);
            google::protobuf::io::CordOutputStream cord_output(std::move(initial_cord), size_hint);
            
            // Write additional data
            void* buffer;
            int buffer_size;
            if (cord_output.Next(&buffer, &buffer_size) && buffer_size > 0) {
                size_t write_size = std::min<size_t>(buffer_size, cord_content.size());
                memcpy(buffer, cord_content.data(), write_size);
            }
            
            // Consume the final Cord
            absl::Cord final_cord = cord_output.Consume();
            break;
        }
        
        case 4: {
            // Test CordOutputStream with CordBuffer
            auto cord_buffer = absl::CordBuffer::CreateWithDefaultLimit(1024);
            if (cord_buffer.available_up_to(1024).data() != nullptr) {
                google::protobuf::io::CordOutputStream cord_output(std::move(cord_buffer), size_hint);
                
                // Write data
                void* buffer;
                int buffer_size;
                if (cord_output.Next(&buffer, &buffer_size) && buffer_size > 0) {
                    size_t write_size = std::min<size_t>(buffer_size, cord_content.size());
                    memcpy(buffer, cord_content.data(), write_size);
                }
                
                absl::Cord result_cord = cord_output.Consume();
            }
            break;
        }
        
        case 5: {
            // Test round-trip: write -> compress -> decompress -> read
            // Create original Cord
            absl::Cord original_cord = source_cord;
            
            // Write to CordOutputStream
            google::protobuf::io::CordOutputStream output_stream(size_hint);
            output_stream.WriteCord(original_cord);
            absl::Cord written_cord = output_stream.Consume();
            
            // Compress
            google::protobuf::io::CordOutputStream compression_stream;
            google::protobuf::io::GzipOutputStream::Options gzip_compress_options;
            gzip_compress_options.format = static_cast<google::protobuf::io::GzipOutputStream::Format>(gzip_output_format_val);
            google::protobuf::io::GzipOutputStream gzip_compress(
                &compression_stream,
                gzip_compress_options);
            const absl::Cord* to_compress = &written_cord;
            google::protobuf::io::CordInputStream compress_input(to_compress);
            
            // Copy data through compression
            const void* in_buffer;
            int in_size;
            while (compress_input.Next(&in_buffer, &in_size)) {
                void* out_buffer;
                int out_size;
                if (gzip_compress.Next(&out_buffer, &out_size) && out_size >= in_size) {
                    memcpy(out_buffer, in_buffer, in_size);
                } else {
                    break;
                }
            }
            
            // Close the GzipOutputStream before consuming the underlying stream
            // This prevents heap-use-after-free when destructor tries to write to freed buffers
            gzip_compress.Close();
            
            absl::Cord compressed_cord = compression_stream.Consume();
            
            // Decompress
            if (compressed_cord.size() > 0) {
                const absl::Cord* to_decompress = &compressed_cord;
                google::protobuf::io::CordInputStream decompress_input(to_decompress);
                
                google::protobuf::io::GzipInputStream gzip_decompress(
                    &decompress_input,
                    gzip_input_format);
                google::protobuf::io::CordOutputStream decompression_stream;
                
                // Copy data through decompression
                const void* comp_buffer;
                int comp_size;
                while (gzip_decompress.Next(&comp_buffer, &comp_size)) {
                    void* decomp_buffer;
                    int decomp_size;
                    if (decompression_stream.Next(&decomp_buffer, &decomp_size) && decomp_size >= comp_size) {
                        memcpy(decomp_buffer, comp_buffer, comp_size);
                    } else {
                        break;
                    }
                }
                
                absl::Cord decompressed_cord = decompression_stream.Consume();
                
                // Read decompressed data
                const absl::Cord* final_cord = &decompressed_cord;
                if (final_cord->size() > 0) {
                    google::protobuf::io::CordInputStream final_input(final_cord);
                    
                    // Exercise various read operations
                    const void* final_buffer;
                    int final_size;
                    while (final_input.Next(&final_buffer, &final_size)) {
                        // Test Skip and BackUp
                        if (final_size > 0 && fdp.ConsumeBool()) {
                            int skip_amount = fdp.ConsumeIntegralInRange<int>(0, final_size);
                            final_input.Skip(skip_amount);
                        }
                    }
                }
            }
            break;
        }
        
        case 6: {
            // Test multiple Cord operations in sequence
            const absl::Cord* source = &source_cord;
            google::protobuf::io::CordInputStream input(source);
            
            // Exercise Next() multiple times
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                const void* buffer;
                int size;
                if (input.Next(&buffer, &size)) {
                    // Randomly decide to BackUp or Skip
                    if (size > 0) {
                        if (fdp.ConsumeBool()) {
                            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, size);
                            input.BackUp(backup_amount);
                        } else if (fdp.ConsumeBool()) {
                            int skip_amount = fdp.ConsumeIntegralInRange<int>(0, size);
                            input.Skip(skip_amount);
                        }
                    }
                }
            }
            
            // Test ByteCount()
            int64_t bytes_read = input.ByteCount();
            
            break;
        }
        
        case 7: {
            // Test error handling paths with edge cases
            // Create empty Cord
            absl::Cord empty_cord;
            const absl::Cord* empty_ptr = &empty_cord;
            
            // Try to create CordInputStream with empty cord
            google::protobuf::io::CordInputStream empty_input(empty_ptr);
            
            // Try Next() on empty stream
            const void* buffer;
            int size;
            bool has_data = empty_input.Next(&buffer, &size);
            
            // Create CordOutputStream with very small size hint
            google::protobuf::io::CordOutputStream small_output(1);
            
            // Try to write more data than hint
            void* out_buffer;
            int out_size;
            if (small_output.Next(&out_buffer, &out_size)) {
                if (out_size > 0 && cord_content.size() > 0) {
                    size_t write_size = std::min<size_t>(out_size, cord_content.size());
                    memcpy(out_buffer, cord_content.data(), write_size);
                    
                    // Test BackUp with invalid amount (should be handled internally)
                    if (write_size > 0) {
                        small_output.BackUp(write_size);
                    }
                }
            }
            
            absl::Cord result = small_output.Consume();
            break;
        }
    }
    
    return 0;
}

} // namespace harness_002
