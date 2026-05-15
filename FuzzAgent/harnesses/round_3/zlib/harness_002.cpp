#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_002 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for gzip writing operations
    char temp_filename[] = "/tmp/gz_write_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    close(fd);

    // Consume fixed-size parameters first
    // Compression level (0-9, with special values)
    int level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    if (level == -1) {
        level = Z_DEFAULT_COMPRESSION;
    }

    // Strategy selection
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Build mode string for gzopen
    std::string mode_str = "wb";
    
    // Add compression level to mode string if not default
    if (level != Z_DEFAULT_COMPRESSION && level >= 0 && level <= 9) {
        mode_str += std::to_string(level);
        
        // Optional: add strategy character
        uint8_t add_strategy = fdp.ConsumeBool();
        if (add_strategy) {
            char strategy_char = 'f';  // filtered
            switch (strategy) {
                case 0: strategy_char = 'f'; break;  // filtered
                case 1: strategy_char = 'h'; break;  // huffman only
                case 2: strategy_char = 'R'; break;  // run-length encoding
                case 3: strategy_char = 'F'; break;  // fixed code
                default: strategy_char = 'f'; break;
            }
            mode_str += strategy_char;
        }
    }

    // Open the gzip file for writing
    gzFile gz_file = gzopen(temp_filename, mode_str.c_str());
    if (gz_file == NULL) {
        unlink(temp_filename);
        return 0;
    }

    // Optional: set parameters (compression level and strategy)
    uint8_t set_params = fdp.ConsumeBool();
    if (set_params) {
        int level2 = fdp.ConsumeIntegralInRange<int>(0, 9);
        int strategy2 = fdp.ConsumeIntegralInRange<int>(0, 4);
        gzsetparams(gz_file, level2, strategy2);
    }

    // Determine number of writing operations to perform
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (op_type) {
            case 0: {  // gzwrite - write binary data
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
                if (fdp.remaining_bytes() < write_size) {
                    break;
                }
                
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(write_size);
                int bytes_written = gzwrite(gz_file, buffer.data(), write_size);
                // bytes_written is number of bytes written, 0 on error
                break;
            }
            
            case 1: {  // gzputs - write string
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (fdp.remaining_bytes() < str_len) {
                    break;
                }
                
                std::string str = fdp.ConsumeBytesAsString(str_len);
                // Ensure null termination for string functions
                if (!str.empty()) {
                    int result = gzputs(gz_file, str.c_str());
                    // result is non-negative on success, -1 on error
                }
                break;
            }
            
            case 2: {  // gzputc - write single character
                if (fdp.remaining_bytes() < 1) {
                    break;
                }
                
                char ch = fdp.ConsumeIntegral<char>();
                int result = gzputc(gz_file, ch);
                // result is character written on success, -1 on error
                break;
            }
            
            case 3: {  // gzfwrite - write formatted data
                size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                size_t total_size = item_size * num_items;
                
                if (total_size > 0 && total_size <= 4096 && fdp.remaining_bytes() >= total_size) {
                    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(total_size);
                    size_t items_written = gzfwrite(buffer.data(), item_size, num_items, gz_file);
                    // items_written is number of full items written
                }
                break;
            }
            
            case 4: {  // gzprintf - formatted output
                // For gzprintf, we need to create a format string and arguments
                // We'll use a simple approach with a few format specifiers
                if (fdp.remaining_bytes() >= 10) {
                    // Create a simple format string
                    const char* format = "%s %d %f";
                    
                    // Consume string for %s
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (fdp.remaining_bytes() < str_len + 8) {
                        break;
                    }
                    
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    
                    // Consume integers for %d
                    int int_val = fdp.ConsumeIntegral<int>();
                    
                    // Consume float for %f
                    float float_val = fdp.ConsumeFloatingPoint<float>();
                    
                    // Call gzprintf
                    int chars_printed = gzprintf(gz_file, format, str_val.c_str(), int_val, float_val);
                    // chars_printed is number of characters printed, negative on error
                }
                break;
            }
            
            case 5: {  // gzflush - flush output
                int flush_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                int flush_val = Z_NO_FLUSH;
                switch (flush_type) {
                    case 0: flush_val = Z_NO_FLUSH; break;
                    case 1: flush_val = Z_SYNC_FLUSH; break;
                    case 2: flush_val = Z_FULL_FLUSH; break;
                    case 3: flush_val = Z_FINISH; break;
                }
                
                int flush_result = gzflush(gz_file, flush_val);
                // flush_result is Z_OK on success, Z_STREAM_ERROR if not writing,
                // Z_BUF_ERROR if no progress is possible
                break;
            }
        }
        
        // Optional: seek between operations (only forward seeks allowed for writing)
        uint8_t do_seek = fdp.ConsumeBool();
        if (do_seek && fdp.remaining_bytes() >= 4) {
            // For writing, only forward seeks are supported
            // We'll use a small forward seek
            long offset = fdp.ConsumeIntegralInRange<long>(0, 1024);
            int whence = SEEK_CUR;  // Always seek from current position for writing
            
            // Note: gzseek for writing compresses zeroes to the new position
            long seek_result = gzseek(gz_file, offset, whence);
            // seek_result is new position or -1 on error
        }
        
        // Consume some bytes to vary the fuzzer input pattern
        if (fdp.remaining_bytes() > 0) {
            size_t skip_bytes = fdp.ConsumeIntegralInRange<size_t>(0, 
                std::min<size_t>(16, fdp.remaining_bytes()));
            if (skip_bytes > 0) {
                fdp.ConsumeBytes<uint8_t>(skip_bytes);
            }
        }
    }

    // Get position before closing
    long current_pos = gztell(gz_file);
    long long compressed_offset = gzoffset(gz_file);

    // Close the gzip file
    int close_result = gzclose(gz_file);
    // close_result is Z_OK on success, Z_ERRNO on file error, 
    // Z_STREAM_ERROR if stream inconsistent, Z_BUF_ERROR if last gzread ended
    // in middle of gzip stream

    // Verify the file was created and has some content
    FILE* check_file = fopen(temp_filename, "rb");
    if (check_file) {
        fseek(check_file, 0, SEEK_END);
        long file_size = ftell(check_file);
        fclose(check_file);
        
        // If file has content, we could optionally try to read it back
        // But for a writing-focused harness, just creating content is sufficient
    }

    // Clean up temporary file
    unlink(temp_filename);

    return 0;
}

} // namespace harness_002
