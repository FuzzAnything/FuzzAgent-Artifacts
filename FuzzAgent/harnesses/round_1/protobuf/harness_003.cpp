#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/io/gzip_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_003 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;  // Need minimum input for meaningful Gzip operations
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test type selection: 0 = GzipInputStream test, 1 = GzipOutputStream test
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    switch (test_type) {
        case 0: {
            // ========================================================
            // GzipInputStream test - decompression operations
            // ========================================================
            
            // Create test data for compression/decompression
            size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
            
            // Create ArrayInputStream with the test data
            google::protobuf::io::ArrayInputStream array_input(
                test_data.data(), static_cast<int>(test_data.size()));
            
            // Choose Gzip format (AUTO, GZIP, or ZLIB)
            uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            google::protobuf::io::GzipInputStream::Format format;
            switch (format_choice) {
                case 0:
                    format = google::protobuf::io::GzipInputStream::AUTO;
                    break;
                case 1:
                    format = google::protobuf::io::GzipInputStream::GZIP;
                    break;
                case 2:
                default:
                    format = google::protobuf::io::GzipInputStream::ZLIB;
                    break;
            }
            
            // Choose buffer size (default = -1, or specific size)
            int buffer_size = -1;
            if (fdp.ConsumeBool()) {
                buffer_size = fdp.ConsumeIntegralInRange<int>(256, 65536);
            }
            
            // Create GzipInputStream wrapping the ArrayInputStream
            google::protobuf::io::GzipInputStream gzip_input(&array_input, format, buffer_size);
            
            // Test Next() method - read decompressed data
            const void* read_buffer = nullptr;
            int read_size = 0;
            int next_calls = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < next_calls; i++) {
                bool has_next = gzip_input.Next(&read_buffer, &read_size);
                if (!has_next) {
                    break;  // End of stream
                }
                
                // Randomly decide to skip some of the data we just read
                if (read_size > 0 && fdp.ConsumeBool()) {
                    // Test Skip() method with random amount
                    size_t skip_amount = fdp.ConsumeIntegralInRange<size_t>(0, read_size);
                    bool skip_result = gzip_input.Skip(static_cast<int>(skip_amount));
                    (void)skip_result;  // Result can vary based on stream state
                    
                    // If we skipped less than the full read size, we can BackUp the remainder
                    if (skip_amount < static_cast<size_t>(read_size)) {
                        gzip_input.BackUp(read_size - static_cast<int>(skip_amount));
                    }
                }
            }
            
            // Test additional Skip() operations
            size_t additional_skip = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
            gzip_input.Skip(static_cast<int>(additional_skip));
            
            // Check ByteCount
            int64_t byte_count = gzip_input.ByteCount();
            (void)byte_count;
            
            // Check for zlib errors (informational, not fatal for fuzzing)
            int zlib_error = gzip_input.ZlibErrorCode();
            const char* error_msg = gzip_input.ZlibErrorMessage();
            (void)zlib_error;
            (void)error_msg;
            
            break;
        }
        
        case 1: {
            // ========================================================
            // GzipOutputStream test - compression operations  
            // ========================================================
            
            // Create output buffer for compressed data
            size_t output_buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 65536);
            std::vector<uint8_t> output_buffer(output_buffer_size);
            
            // Create ArrayOutputStream for the output buffer
            google::protobuf::io::ArrayOutputStream array_output(
                output_buffer.data(), static_cast<int>(output_buffer.size()));
            
            // Configure GzipOutputStream options
            google::protobuf::io::GzipOutputStream::Options options;
            
            // Choose format (GZIP or ZLIB)
            options.format = fdp.ConsumeBool() ? 
                google::protobuf::io::GzipOutputStream::GZIP : 
                google::protobuf::io::GzipOutputStream::ZLIB;
            
            // Random buffer size
            if (fdp.ConsumeBool()) {
                options.buffer_size = fdp.ConsumeIntegralInRange<int>(256, 65536);
            }
            
            // Random compression level
            if (fdp.ConsumeBool()) {
                // Zlib compression levels: 0 = no compression, 1 = best speed, 9 = best compression
                options.compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
            }
            
            // Random compression strategy
            if (fdp.ConsumeBool()) {
                // Zlib strategies: 0 = default, 1 = filtered, 2 = huffman only, 3 = rle, 4 = fixed
                options.compression_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
            }
            
            // Create GzipOutputStream wrapping the ArrayOutputStream
            google::protobuf::io::GzipOutputStream gzip_output(&array_output, options);
            
            // Write test data using Next() and BackUp() pattern
            size_t total_data_to_write = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
            size_t bytes_written = 0;
            
            while (bytes_written < total_data_to_write) {
                // Get buffer for writing
                void* write_buffer = nullptr;
                int buffer_size = 0;
                bool has_next = gzip_output.Next(&write_buffer, &buffer_size);
                
                if (!has_next) {
                    break;  // Compression error or output full
                }
                
                // Determine how much data to write to this buffer
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(
                    1, std::min(static_cast<size_t>(buffer_size), total_data_to_write - bytes_written));
                
                // Generate random data to write
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                if (write_data.empty()) {
                    break;  // No more input data
                }
                
                // Write data to buffer
                memcpy(write_buffer, write_data.data(), write_data.size());
                bytes_written += write_data.size();
                
                // If we didn't use the full buffer, BackUp the unused portion
                if (write_data.size() < static_cast<size_t>(buffer_size)) {
                    gzip_output.BackUp(buffer_size - static_cast<int>(write_data.size()));
                }
                
                // Randomly decide to flush
                if (fdp.ConsumeBool()) {
                    bool flush_result = gzip_output.Flush();
                    (void)flush_result;
                }
            }
            
            // Test Flush() method
            bool flush_result = gzip_output.Flush();
            (void)flush_result;
            
            // Test Close() method
            bool close_result = gzip_output.Close();
            (void)close_result;
            
            // Check ByteCount
            int64_t byte_count = gzip_output.ByteCount();
            (void)byte_count;
            
            // Check for zlib errors
            const char* error_msg = gzip_output.ZlibErrorMessage();
            (void)error_msg;
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
