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

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    bool use_compress2 = fdp.ConsumeBool();
    
    // Determine source data size (use remaining bytes)
    size_t src_len = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
    if (src_len == 0) {
        return 0;
    }
    
    std::vector<uint8_t> src_data = fdp.ConsumeBytes<uint8_t>(src_len);
    
    // Calculate compressed size bound
    uLongf compr_bound = compressBound(src_len);
    if (compr_bound == 0 || compr_bound > 10 * 1024 * 1024) {
        return 0;  // Avoid excessive memory usage
    }
    
    // Allocate buffer for compressed data
    Bytef* compr = (Bytef*)malloc(compr_bound);
    if (!compr) {
        return 0;
    }
    
    // Compress the source data to create valid gzip data
    uLongf compr_len = compr_bound;
    int compress_result;
    
    if (use_compress2) {
        compress_result = compress2(compr, &compr_len, src_data.data(), src_len, compression_level);
    } else {
        compress_result = compress(compr, &compr_len, src_data.data(), src_len);
    }
    
    // Only proceed if compression was successful
    if (compress_result != Z_OK || compr_len == 0) {
        free(compr);
        return 0;
    }
    
    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gz_valid_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        free(compr);
        return 0;
    }
    
    // Write valid gzip-compressed data to temp file
    ssize_t written = write(fd, compr, compr_len);
    close(fd);
    free(compr);
    
    if (written != (ssize_t)compr_len) {
        unlink(temp_filename);
        return 0;
    }
    
    // Open the gzip file for reading
    gzFile gz_file = gzopen(temp_filename, "rb");
    if (gz_file == NULL) {
        unlink(temp_filename);
        return 0;
    }
    
    // Determine number of operations to perform based on remaining input
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (op_type) {
            case 0: {  // gzfread
                size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                size_t total_size = item_size * num_items;
                
                if (total_size > 0 && total_size <= 4096) {
                    std::vector<char> buffer(total_size);
                    size_t items_read = gzfread(buffer.data(), item_size, num_items, gz_file);
                    // items_read is number of full items read
                    (void)items_read; // Avoid unused variable warning
                }
                break;
            }
            
            case 1: {  // gzgets
                size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(2, 256);
                std::vector<char> buffer(buf_size);
                char* result = gzgets(gz_file, buffer.data(), buf_size);
                // result is NULL on error or EOF, otherwise pointer to buffer
                (void)result; // Avoid unused variable warning
                break;
            }
            
            case 2: {  // gzread
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
                std::vector<char> buffer(read_size);
                int bytes_read = gzread(gz_file, buffer.data(), read_size);
                // bytes_read can be -1 on error, 0 on EOF, or positive
                (void)bytes_read; // Avoid unused variable warning
                break;
            }
            
            case 3: {  // gzgetc
                int ch = gzgetc(gz_file);
                // ch is -1 on error or EOF
                (void)ch; // Avoid unused variable warning
                break;
            }
            
            case 4: {  // gzungetc
                char pushback_char = fdp.ConsumeIntegral<char>();
                int result = gzungetc(pushback_char, gz_file);
                // result is the character pushed back, or -1 on error
                (void)result; // Avoid unused variable warning
                break;
            }
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
    
    // Close the gzip file - try both close functions for coverage
    int close_result;
    
    // First try gzclose_r (for reading)
    close_result = gzclose_r(gz_file);
    
    // If gzclose_r failed, try gzclose (generic)
    if (close_result != Z_OK) {
        // Reopen file for gzclose test if needed
        gz_file = gzopen(temp_filename, "rb");
        if (gz_file) {
            close_result = gzclose(gz_file);
        }
    }
    
    // Clean up temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_006
