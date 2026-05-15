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

namespace harness_009 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for error testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_malformed_data = fdp.ConsumeBool();
    bool use_gzdopen = fdp.ConsumeBool();
    bool trigger_errors = fdp.ConsumeBool();
    int buffer_size = fdp.ConsumeIntegralInRange<int>(128, 65536);
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9); // Include Z_DEFAULT_COMPRESSION (-1)
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gz_error_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Option 1: Write malformed/corrupted gzip data to file
    if (use_malformed_data) {
        size_t corrupt_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        if (corrupt_len > fdp.remaining_bytes()) {
            corrupt_len = fdp.remaining_bytes();
        }
        if (corrupt_len > 0) {
            std::vector<uint8_t> corrupt_data = fdp.ConsumeBytes<uint8_t>(corrupt_len);
            // Intentionally corrupt some bytes
            for (size_t i = 0; i < corrupt_data.size() && i < 10; i++) {
                corrupt_data[i] = 0xFF; // Invalid gzip header bytes
            }
            write(fd, corrupt_data.data(), corrupt_data.size());
        }
    } else {
        // Option 2: Write valid gzip data for testing
        size_t src_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (src_len > fdp.remaining_bytes()) {
            src_len = fdp.remaining_bytes();
        }
        if (src_len > 0) {
            std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(src_len);
            uLongf compr_bound = compressBound(src_len);
            if (compr_bound > 0 && compr_bound <= 10 * 1024 * 1024) {
                Bytef* compr = (Bytef*)malloc(compr_bound);
                if (compr) {
                    uLongf compr_len = compr_bound;
                    int compress_result = compress(compr, &compr_len, src_data.data(), src_len);
                    if (compress_result == Z_OK && compr_len > 0) {
                        write(fd, compr, compr_len);
                    }
                    free(compr);
                }
            }
        }
    }
    
    close(fd);
    
    // Open the gzip file using either gzopen or gzdopen
    gzFile gz_file = NULL;
    const char* mode = NULL;
    
    if (test_type == 0) {
        mode = "rb"; // Read only
    } else if (test_type == 1) {
        mode = "wb"; // Write only  
    } else if (test_type == 2) {
        mode = "ab"; // Append
    } else if (test_type == 3) {
        mode = "wT"; // Write transparent (no compression)
    } else if (test_type == 4) {
        mode = "wb9"; // Maximum compression
    } else if (test_type == 5) {
        mode = "wb1"; // Fast compression
    } else if (test_type == 6) {
        mode = "rT"; // Read transparent
    } else {
        mode = "rb"; // Default
    }
    
    if (use_gzdopen) {
        // Re-open file descriptor for gzdopen
        fd = open(temp_filename, O_RDWR);
        if (fd >= 0) {
            gz_file = gzdopen(fd, mode);
        }
    } else {
        gz_file = gzopen(temp_filename, mode);
    }
    
    if (gz_file == NULL) {
        if (use_gzdopen && fd >= 0) {
            close(fd);
        }
        unlink(temp_filename);
        return 0;
    }
    
    // Test gzbuffer (should be called early)
    if (fdp.ConsumeBool()) {
        gzbuffer(gz_file, buffer_size);
    }
    
    // Test gzdirect - check if file is in gzip format
    int is_direct = gzdirect(gz_file);
    
    // Test gzsetparams for write modes
    if (strchr(mode, 'w') || strchr(mode, 'a')) {
        if (fdp.ConsumeBool()) {
            gzsetparams(gz_file, compression_level, strategy);
        }
    }
    
    // Perform a sequence of operations with potential error conditions
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 15);
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 16;
        
        switch (op_type) {
            case 0: {  // gzwrite (potentially on read-only file to trigger error)
                size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (write_len > fdp.remaining_bytes()) {
                    write_len = fdp.remaining_bytes();
                }
                if (write_len > 0) {
                    std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_len);
                    gzwrite(gz_file, write_data.data(), write_len);
                    // Check for error
                    if (trigger_errors) {
                        int err_num;
                        const char* err_msg = gzerror(gz_file, &err_num);
                    }
                }
                break;
            }
            
            case 1: {  // gzread (potentially on write-only file to trigger error)
                size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                if (read_len > 0) {
                    std::vector<char> buffer(read_len);
                    gzread(gz_file, buffer.data(), read_len);
                    // Check for error
                    if (trigger_errors) {
                        int err_num;
                        const char* err_msg = gzerror(gz_file, &err_num);
                    }
                }
                break;
            }
            
            case 2: {  // gzputc
                int ch = fdp.ConsumeIntegral<uint8_t>();
                gzputc(gz_file, ch);
                break;
            }
            
            case 3: {  // gzgetc
                int result = gzgetc(gz_file);
                // gzgetc returns -1 on error or EOF
                if (result == -1 && trigger_errors) {
                    int err_num;
                    const char* err_msg = gzerror(gz_file, &err_num);
                }
                break;
            }
            
            case 4: {  // gzungetc (not in harness_008)
                int ch = fdp.ConsumeIntegral<uint8_t>();
                gzungetc(ch, gz_file);
                break;
            }
            
            case 5: {  // gzputs
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (str_len > fdp.remaining_bytes()) {
                    str_len = fdp.remaining_bytes();
                }
                if (str_len > 0) {
                    std::string str = fdp.ConsumeBytesAsString(str_len);
                    gzputs(gz_file, str.c_str());
                }
                break;
            }
            
            case 6: {  // gzgets
                size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (buf_size > 0) {
                    std::vector<char> buffer(buf_size);
                    gzgets(gz_file, buffer.data(), buf_size);
                }
                break;
            }
            
            case 7: {  // Complex gzseek with various SEEK modes and edge cases
                int whence = fdp.ConsumeIntegral<uint8_t>() % 4; // 0-3, includes invalid
                long offset = fdp.ConsumeIntegral<int32_t>();
                
                // Test edge cases: very large offsets, negative offsets
                if (fdp.ConsumeBool()) {
                    offset = -1; // Invalid
                } else if (fdp.ConsumeBool()) {
                    offset = 0x7FFFFFFF; // Large positive
                }
                
                gzseek(gz_file, offset, whence);
                break;
            }
            
            case 8: {  // gztell
                gztell(gz_file);
                break;
            }
            
            case 9: {  // gzoffset (not in harness_008)
                gzoffset(gz_file);
                break;
            }
            
            case 10: { // gzrewind
                gzrewind(gz_file);
                break;
            }
            
            case 11: { // gzeof
                int eof_result = gzeof(gz_file);
                // eof_result can be 1 (true) or 0 (false)
                break;
            }
            
            case 12: { // gzflush with various flush modes
                int flush = fdp.ConsumeIntegral<uint8_t>() % 4;
                // Map to valid flush values: Z_NO_FLUSH(0), Z_SYNC_FLUSH(2), Z_FULL_FLUSH(3), Z_FINISH(4)
                if (flush == 0) flush = Z_NO_FLUSH;
                else if (flush == 1) flush = Z_SYNC_FLUSH;
                else if (flush == 2) flush = Z_FULL_FLUSH;
                else flush = Z_FINISH;
                
                gzflush(gz_file, flush);
                break;
            }
            
            case 13: { // gzerror with error number retrieval
                int err_num;
                const char* err_msg = gzerror(gz_file, &err_num);
                // Force an error by doing something invalid
                if (trigger_errors && fdp.ConsumeBool()) {
                    // Try invalid operation based on mode
                    if (strchr(mode, 'r')) {
                        // Try to write to read-only
                        char dummy = 'x';
                        gzwrite(gz_file, &dummy, 1);
                    } else {
                        // Try to read from write-only
                        char dummy;
                        gzread(gz_file, &dummy, 1);
                    }
                    // Check error again
                    err_msg = gzerror(gz_file, &err_num);
                }
                break;
            }
            
            case 14: { // gzclearerr
                gzclearerr(gz_file);
                break;
            }
            
            case 15: { // Change compression parameters mid-stream with edge cases
                if (strchr(mode, 'w') || strchr(mode, 'a')) {
                    int new_level = fdp.ConsumeIntegralInRange<int>(-2, 10); // Include invalid values
                    int new_strategy = fdp.ConsumeIntegralInRange<int>(-1, 5); // Include invalid values
                    gzsetparams(gz_file, new_level, new_strategy);
                }
                break;
            }
        }
        
        // Periodically check gzdirect status (it can change after reading)
        if (i % 3 == 0) {
            gzdirect(gz_file);
        }
        
        // Periodically check gzeof status
        if (i % 4 == 0) {
            gzeof(gz_file);
        }
    }
    
    // FIX: The double-closing bug - only close once and properly handle gzdopen
    if (use_gzdopen) {
        // For gzdopen, the underlying file descriptor is closed by gzclose functions
        // We should NOT close the fd separately
        if (strchr(mode, 'r')) {
            gzclose_r(gz_file);
        } else if (strchr(mode, 'w') || strchr(mode, 'a')) {
            gzclose_w(gz_file);
        } else {
            gzclose(gz_file);
        }
    } else {
        // For gzopen, use the appropriate close function
        if (strchr(mode, 'r')) {
            gzclose_r(gz_file);
        } else if (strchr(mode, 'w') || strchr(mode, 'a')) {
            gzclose_w(gz_file);
        } else {
            gzclose(gz_file);
        }
    }
    
    // Clean up temp file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_009
