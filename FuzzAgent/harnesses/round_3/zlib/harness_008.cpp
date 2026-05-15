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

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 6;
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE, Z_FIXED
    bool use_buffer = fdp.ConsumeBool();
    uint32_t buffer_size = fdp.ConsumeIntegralInRange<uint32_t>(128, 65536);
    
    // Determine gzip mode based on selector
    const char* mode;
    bool is_write_mode = false;
    bool is_read_mode = false;
    
    switch (mode_selector) {
        case 0: mode = "rb"; is_read_mode = true; break;
        case 1: mode = "wb"; is_write_mode = true; break;
        case 2: mode = "ab"; is_write_mode = true; break;
        case 3: mode = "wb9"; is_write_mode = true; break; // Maximum compression
        case 4: mode = "wb1"; is_write_mode = true; break; // Fast compression
        case 5: mode = "wT"; is_write_mode = true; break; // No compression
        default: mode = "rb"; is_read_mode = true; break;
    }

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gz_comprehensive_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // If this is a write mode, we need to write some initial data
    // For read modes, we need to create valid gzip data first
    if (is_read_mode) {
        // Create valid gzip-compressed data for reading
        size_t src_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (src_len > fdp.remaining_bytes()) {
            src_len = fdp.remaining_bytes();
        }
        if (src_len == 0) {
            close(fd);
            unlink(temp_filename);
            return 0;
        }
        
        std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(src_len);
        uLongf compr_bound = compressBound(src_len);
        if (compr_bound == 0 || compr_bound > 10 * 1024 * 1024) {
            close(fd);
            unlink(temp_filename);
            return 0;
        }
        
        Bytef* compr = (Bytef*)malloc(compr_bound);
        if (!compr) {
            close(fd);
            unlink(temp_filename);
            return 0;
        }
        
        uLongf compr_len = compr_bound;
        int compress_result = compress(compr, &compr_len, src_data.data(), src_len);
        
        if (compress_result == Z_OK && compr_len > 0) {
            ssize_t written = write(fd, compr, compr_len);
            // Ignore write errors for now
        }
        
        free(compr);
    }
    
    close(fd);

    // Open the gzip file
    gzFile gz_file = gzopen(temp_filename, mode);
    if (gz_file == NULL) {
        unlink(temp_filename);
        return 0;
    }

    // Configure gzip file if requested
    if (use_buffer) {
        gzbuffer(gz_file, buffer_size);
    }
    
    // Set compression parameters for write modes
    if (is_write_mode) {
        gzsetparams(gz_file, compression_level, strategy);
    }

    // Determine number of operations to perform
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 12;
        
        switch (op_type) {
            case 0: {  // gzwrite
                if (is_write_mode) {
                    size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                    if (write_len > fdp.remaining_bytes()) {
                        write_len = fdp.remaining_bytes();
                    }
                    if (write_len > 0) {
                        std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_len);
                        gzwrite(gz_file, write_data.data(), write_len);
                    }
                }
                break;
            }
            
            case 1: {  // gzread
                if (is_read_mode) {
                    size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                    if (read_len > 0) {
                        std::vector<char> buffer(read_len);
                        gzread(gz_file, buffer.data(), read_len);
                    }
                }
                break;
            }
            
            case 2: {  // gzputc
                if (is_write_mode) {
                    int ch = fdp.ConsumeIntegral<uint8_t>();
                    gzputc(gz_file, ch);
                }
                break;
            }
            
            case 3: {  // gzgetc
                if (is_read_mode) {
                    gzgetc(gz_file);
                }
                break;
            }
            
            case 4: {  // gzputs
                if (is_write_mode) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (str_len > fdp.remaining_bytes()) {
                        str_len = fdp.remaining_bytes();
                    }
                    if (str_len > 0) {
                        std::string str = fdp.ConsumeBytesAsString(str_len);
                        gzputs(gz_file, str.c_str());
                    }
                }
                break;
            }
            
            case 5: {  // gzgets
                if (is_read_mode) {
                    size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (buf_size > 0) {
                        std::vector<char> buffer(buf_size);
                        gzgets(gz_file, buffer.data(), buf_size);
                    }
                }
                break;
            }
            
            case 6: {  // gzseek with various whence
                int whence = fdp.ConsumeIntegral<uint8_t>() % 3;
                long offset = fdp.ConsumeIntegral<int32_t>();
                gzseek(gz_file, offset, whence); // SEEK_SET=0, SEEK_CUR=1, SEEK_END=2
                break;
            }
            
            case 7: {  // gztell
                gztell(gz_file);
                break;
            }
            
            case 8: {  // gzrewind
                gzrewind(gz_file);
                break;
            }
            
            case 9: {  // gzerror and gzclearerr
                const char* err_msg = gzerror(gz_file, NULL);
                // Try to trigger an error by doing something invalid
                if (is_read_mode && fdp.ConsumeBool()) {
                    // Try to write to a read-only file (should error)
                    char dummy = 'x';
                    gzwrite(gz_file, &dummy, 1);
                }
                gzclearerr(gz_file);
                break;
            }
            
            case 10: { // gzeof
                gzeof(gz_file);
                break;
            }
            
            case 11: { // Change compression parameters mid-stream
                if (is_write_mode) {
                    int new_level = fdp.ConsumeIntegralInRange<int>(0, 9);
                    int new_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
                    gzsetparams(gz_file, new_level, new_strategy);
                }
                break;
            }
        }
        
        // Check for end of file occasionally
        if (i % 5 == 0) {
            gzeof(gz_file);
        }
    }

    // Try different close functions based on mode
    if (is_read_mode) {
        gzclose_r(gz_file);
    } else if (is_write_mode) {
        gzclose_w(gz_file);
    } else {
        gzclose(gz_file);
    }

    // Clean up temp file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_008
