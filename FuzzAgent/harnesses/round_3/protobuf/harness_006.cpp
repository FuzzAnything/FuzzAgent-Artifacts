#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/gzip_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "absl/strings/cord.h"

namespace harness_006 {
// Protocol Buffers fuzzing harness - Gzip Stream Compression/Decompression subsystem
// Targets: GzipInputStream::Skip, GzipInputStream::Next, GzipOutputStream::Next,
//          GzipOutputStream::Flush, GzipOutputStream::BackUp, GzipOutputStream::Close,
//          GzipInputStream::ByteCount, GzipOutputStream::ByteCount
// Uses ArrayInputStream and ArrayOutputStream as backing streams
// Uses FuzzedDataProvider for structured input splitting



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful gzip operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create input data and wrap in ArrayInputStream
    // Consume a portion of input for the raw data to process
    size_t raw_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
    
    // Create ArrayInputStream with the raw data
    google::protobuf::io::ArrayInputStream array_input(raw_data.data(), raw_data.size());
    
    // Step 2: Create GzipInputStream for decompression testing
    // Consume format choice: AUTO (0), GZIP (1), ZLIB (2)
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    google::protobuf::io::GzipInputStream::Format format;
    switch (format_choice) {
        case 0: format = google::protobuf::io::GzipInputStream::AUTO; break;
        case 1: format = google::protobuf::io::GzipInputStream::GZIP; break;
        case 2: format = google::protobuf::io::GzipInputStream::ZLIB; break;
        default: format = google::protobuf::io::GzipInputStream::AUTO; break;
    }
    
    // Consume buffer size (use default -1 or positive value)
    int buffer_size = fdp.ConsumeBool() ? -1 : fdp.ConsumeIntegralInRange<int>(64, 65536);
    
    google::protobuf::io::GzipInputStream gzip_input(&array_input, format, buffer_size);
    
    // Step 3: Call GzipInputStream::Next to read compressed data
    const void* in_data;
    int in_size;
    bool next_result = gzip_input.Next(&in_data, &in_size);
    
    // Only proceed if Next succeeded
    if (next_result && in_size > 0) {
        // Step 4: Call GzipInputStream::Skip to test skip functionality
        // Determine skip amount based on available data
        int skip_amount = fdp.ConsumeIntegralInRange<int>(0, in_size);
        bool skip_result = gzip_input.Skip(skip_amount);
        
        // Backup if we have data left after skip
        if (in_size > skip_amount) {
            gzip_input.BackUp(in_size - skip_amount);
        }
        
        // Test ByteCount on GzipInputStream
        int64_t input_byte_count = gzip_input.ByteCount();
        (void)input_byte_count;
        
        // Test Zlib error reporting methods
        const char* zlib_error_msg = gzip_input.ZlibErrorMessage();
        int zlib_error_code = gzip_input.ZlibErrorCode();
        (void)zlib_error_msg;
        (void)zlib_error_code;
    }
    
    // Step 5: Create output buffer and GzipOutputStream for compression testing
    // Create output buffer for compressed data
    size_t output_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
    std::vector<uint8_t> output_buffer(output_buffer_size);
    
    google::protobuf::io::ArrayOutputStream array_output(output_buffer.data(), output_buffer_size);
    
    // Create GzipOutputStream with options
    google::protobuf::io::GzipOutputStream::Options options;
    
    // Set format based on consumed choice
    int output_format_choice = fdp.ConsumeIntegralInRange<int>(1, 2);
    options.format = (output_format_choice == 1) ? 
        google::protobuf::io::GzipOutputStream::GZIP : 
        google::protobuf::io::GzipOutputStream::ZLIB;
    
    // Set compression level (0-9)
    options.compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    
    // Set compression strategy (0-4, but we'll use valid range)
    options.compression_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Set buffer size
    options.buffer_size = fdp.ConsumeIntegralInRange<int>(64, 65536);
    
    google::protobuf::io::GzipOutputStream gzip_output(&array_output, options);
    
    // Step 6: Call GzipOutputStream::Next to write compressed data
    void* out_data;
    int out_size;
    bool output_next_result = gzip_output.Next(&out_data, &out_size);
    
    if (output_next_result && out_size > 0) {
        // Write some data to the output buffer (using remaining fuzzer data)
        size_t write_size = std::min<size_t>(out_size, fdp.remaining_bytes());
        if (write_size > 0) {
            std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
            memcpy(out_data, write_data.data(), write_size);
            
            // Step 7: Call GzipOutputStream::BackUp to test backup functionality
            // Backup a portion of what we wrote
            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, write_size);
            gzip_output.BackUp(backup_amount);
        }
        
        // Step 8: Call GzipOutputStream::Flush to test flush behavior
        bool flush_result = gzip_output.Flush();
        (void)flush_result;
        
        // Test ByteCount on GzipOutputStream
        int64_t output_byte_count = gzip_output.ByteCount();
        (void)output_byte_count;
        
        // Test Zlib error reporting methods
        const char* output_zlib_error_msg = gzip_output.ZlibErrorMessage();
        int output_zlib_error_code = gzip_output.ZlibErrorCode();
        (void)output_zlib_error_msg;
        (void)output_zlib_error_code;
        
        // Step 9: Call GzipOutputStream::Close to properly close stream
        // Only close if we have enough remaining data for a decision
        if (fdp.remaining_bytes() > 0) {
            bool close_result = gzip_output.Close();
            (void)close_result;
        }
    }
    
    // Step 10: Test ZeroCopyInputStream::ReadCord and ZeroCopyOutputStream::WriteCord
    // if we have enough remaining data
    if (fdp.remaining_bytes() > 16) {
        // Create a simple input stream for Cord testing
        std::vector<uint8_t> cord_test_data = fdp.ConsumeBytes<uint8_t>(16);
        google::protobuf::io::ArrayInputStream cord_input(cord_test_data.data(), cord_test_data.size());
        
        absl::Cord cord;
        int read_count = fdp.ConsumeIntegralInRange<int>(1, cord_test_data.size());
        bool read_cord_result = cord_input.ReadCord(&cord, read_count);
        (void)read_cord_result;
        
        // Test WriteCord if we have a valid Cord
        if (!cord.empty()) {
            // Create output stream for Cord writing
            std::vector<uint8_t> cord_output_buffer(1024);
            google::protobuf::io::ArrayOutputStream cord_output(cord_output_buffer.data(), cord_output_buffer.size());
            
            // Try to write the Cord
            bool write_cord_result = cord_output.WriteCord(cord);
            (void)write_cord_result;
        }
    }
    
    return 0;
}

} // namespace harness_006
