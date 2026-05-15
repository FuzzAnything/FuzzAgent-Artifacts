#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/gzip_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include <zlib.h>

namespace harness_007 {
// Fuzzing harness for Google Protocol Buffers Gzip stream operations
// Targets Gzip compression/decompression APIs: GzipInputStream::Skip, GzipInputStream::Next,
// GzipOutputStream::Next, GzipOutputStream::Flush, GzipOutputStream::Close
// Supporting APIs: ArrayInputStream::Skip, StringOutputStream::BackUp
// Focuses on compression/decompression cycles with zlib integration - semantically different from existing harnesses
// Differentiates from existing harnesses:
// - harness_000: message operations (binary parsing/serialization)
// - harness_001: Tokenizer parsing (low-level text parsing)
// - harness_002: CordInputStream/CordOutputStream (I/O streams with Cord)
// - harness_003: CodedInputStream/CodedOutputStream (binary I/O with Cord)
// - harness_004: JSON format conversion
// - harness_005: service/method descriptor reflection
// - harness_006: unknown field handling and arena allocation
// This harness targets Gzip compression/decompression stream APIs



// Gzip stream headers

// zlib constants

using google::protobuf::io::GzipInputStream;
using google::protobuf::io::GzipOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::StringOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful gzip testing
    // Need enough for at least some raw data and configuration
    if (size < 64) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t gzip_format = fdp.ConsumeIntegral<uint8_t>() % 3;
    bool test_error_conditions = fdp.ConsumeBool();
    bool test_partial_operations = fdp.ConsumeBool();
    bool test_backup_operations = fdp.ConsumeBool();
    
    // Consume parameters for compression
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(256, 65536);
    size_t max_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 16384);
    
    // Ensure we have enough data for testing
    if (fdp.remaining_bytes() < max_data_size) {
        return 0;  // Not enough data for meaningful testing
    }
    
    // Consume raw data for compression testing
    std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(max_data_size);
    
    // Create backing streams
    ArrayInputStream array_input(raw_data.data(), raw_data.size());
    std::string output_string;
    StringOutputStream string_output(&output_string);
    
    // Test compression with GzipOutputStream
    {
        GzipOutputStream::Options options;
        
        // Set format based on fuzzed input
        if (gzip_format == 0) {
            options.format = GzipOutputStream::GZIP;
        } else if (gzip_format == 1) {
            options.format = GzipOutputStream::ZLIB;
        } else {
            // Default to GZIP if out of range
            options.format = GzipOutputStream::GZIP;
        }
        
        options.compression_level = compression_level;
        options.buffer_size = buffer_size;
        options.compression_strategy = Z_DEFAULT_STRATEGY;
        
        GzipOutputStream gzip_output(&string_output, options);
        
        // Write data through compression using Next()
        size_t data_written = 0;
        while (data_written < raw_data.size()) {
            void* compressed_buffer = nullptr;
            int compressed_size = 0;
            
            // Call GzipOutputStream::Next to get buffer for compressed data
            if (!gzip_output.Next(&compressed_buffer, &compressed_size)) {
                break;  // Compression error
            }
            
            // Determine how much data to write
            size_t write_size = std::min(static_cast<size_t>(compressed_size), 
                                         raw_data.size() - data_written);
            
            // Copy raw data to compressed buffer
            memcpy(compressed_buffer, raw_data.data() + data_written, write_size);
            data_written += write_size;
            
            // If we didn't fill the entire buffer, back up
            if (write_size < static_cast<size_t>(compressed_size)) {
                gzip_output.BackUp(compressed_size - write_size);
            }
        }
        
        // Test GzipOutputStream::Flush if requested
        if (test_partial_operations) {
            gzip_output.Flush();
        }
        
        // Test GzipOutputStream::Close
        gzip_output.Close();
        
        // Check for zlib errors
        int error_code = gzip_output.ZlibErrorCode();
        const char* error_msg = gzip_output.ZlibErrorMessage();
        (void)error_code;  // Mark as used
        (void)error_msg;   // Mark as used
    }
    
    // Now test decompression with GzipInputStream if we have compressed data
    if (!output_string.empty() && output_string.size() > 0) {
        // Create input stream from compressed data
        ArrayInputStream compressed_input(output_string.data(), output_string.size());
        
        // Set format for decompression
        GzipInputStream::Format decompress_format;
        if (gzip_format == 0) {
            decompress_format = GzipInputStream::GZIP;
        } else if (gzip_format == 1) {
            decompress_format = GzipInputStream::ZLIB;
        } else {
            decompress_format = GzipInputStream::AUTO;
        }
        
        GzipInputStream gzip_input(&compressed_input, decompress_format, buffer_size);
        
        // Read decompressed data using GzipInputStream::Next
        size_t total_decompressed = 0;
        const void* decompressed_buffer = nullptr;
        int decompressed_size = 0;
        
        while (gzip_input.Next(&decompressed_buffer, &decompressed_size)) {
            total_decompressed += decompressed_size;
            
            // Test GzipInputStream::Skip if requested
            if (test_partial_operations && decompressed_size > 10) {
                // Skip half of the decompressed data
                int skip_amount = decompressed_size / 2;
                gzip_input.Skip(skip_amount);
                total_decompressed -= skip_amount;
                
                // Test GzipInputStream::BackUp if requested
                if (test_backup_operations && decompressed_size > 20) {
                    gzip_input.BackUp(10);
                    total_decompressed -= 10;
                }
            }
            
            // Test ArrayInputStream::Skip on the backing stream
            if (test_partial_operations && fdp.ConsumeBool()) {
                // Skip a small amount in the backing stream
                compressed_input.Skip(10);
            }
        }
        
        // Test GzipInputStream::ByteCount
        int64_t bytes_read = gzip_input.ByteCount();
        (void)bytes_read;  // Mark as used
        
        // Check for zlib errors
        int error_code = gzip_input.ZlibErrorCode();
        const char* error_msg = gzip_input.ZlibErrorMessage();
        (void)error_code;  // Mark as used
        (void)error_msg;   // Mark as used
    }
    
    // Test error conditions with malformed data if requested
    if (test_error_conditions && fdp.remaining_bytes() > 0) {
        // Create malformed gzip data
        std::vector<uint8_t> malformed_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!malformed_data.empty()) {
            ArrayInputStream malformed_input(malformed_data.data(), malformed_data.size());
            GzipInputStream malformed_gzip(&malformed_input, GzipInputStream::AUTO);
            
            // Try to read from malformed data
            const void* buffer = nullptr;
            int buf_size = 0;
            while (malformed_gzip.Next(&buffer, &buf_size)) {
                // Just consume the data
                (void)buffer;
                (void)buf_size;
            }
            
            // Check error codes
            int error_code = malformed_gzip.ZlibErrorCode();
            const char* error_msg = malformed_gzip.ZlibErrorMessage();
            (void)error_code;  // Mark as used
            (void)error_msg;   // Mark as used
        }
    }
    
    return 0;
}

} // namespace harness_007
