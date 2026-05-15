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

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gz_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    close(fd);

    // Consume fixed-size parameters first
    // Operation mode: 'r' for reading, 'w' for writing, 'a' for appending
    char mode_char = 'r';
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (mode_choice) {
        case 0: mode_char = 'r'; break;  // read
        case 1: mode_char = 'w'; break;  // write  
        case 2: mode_char = 'a'; break;  // append
    }

    // Build mode string (e.g., "rb", "wb9", "wb6f", etc.)
    std::string mode_str;
    mode_str += mode_char;
    
    // Add 'b' for binary mode (standard for gzip files)
    mode_str += 'b';
    
    // Optional: add compression level for write modes
    if (mode_char == 'w' || mode_char == 'a') {
        uint8_t add_level = fdp.ConsumeBool();
        if (add_level) {
            int level = fdp.ConsumeIntegralInRange<int>(0, 9);
            mode_str += std::to_string(level);
            
            // Optional: add strategy
            uint8_t add_strategy = fdp.ConsumeBool();
            if (add_strategy) {
                char strategy_char = 'f';  // filtered
                uint8_t strategy_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (strategy_choice) {
                    case 0: strategy_char = 'f'; break;  // filtered
                    case 1: strategy_char = 'h'; break;  // huffman only
                    case 2: strategy_char = 'R'; break;  // run-length encoding
                    case 3: strategy_char = 'F'; break;  // fixed code
                }
                mode_str += strategy_char;
            }
        }
    }

    // For read mode, we need to write some gzip-compressed data to the file first
    if (mode_char == 'r') {
        // Create some compressed data to read
        size_t compress_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
        if (fdp.remaining_bytes() < compress_size) {
            unlink(temp_filename);
            return 0;
        }
        
        std::vector<uint8_t> compress_data = fdp.ConsumeBytes<uint8_t>(compress_size);
        
        // Write compressed data to temp file
        FILE* f = fopen(temp_filename, "wb");
        if (f) {
            fwrite(compress_data.data(), 1, compress_data.size(), f);
            fclose(f);
        }
    }

    // Open the gzip file
    gzFile gz_file = gzopen(temp_filename, mode_str.c_str());
    if (gz_file == NULL) {
        unlink(temp_filename);
        return 0;
    }

    // Determine number of operations to perform based on remaining input
    size_t num_ops = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_ops && fdp.remaining_bytes() > 0; i++) {
        uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 7;
        
        switch (op_type) {
            case 0: {  // gzread
                size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
                std::vector<char> buffer(read_size);
                int bytes_read = gzread(gz_file, buffer.data(), read_size);
                // bytes_read can be -1 on error, 0 on EOF, or positive
                break;
            }
            
            case 1: {  // gzgets
                size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(2, 256);
                std::vector<char> buffer(buf_size);
                char* result = gzgets(gz_file, buffer.data(), buf_size);
                // result is NULL on error or EOF, otherwise pointer to buffer
                break;
            }
            
            case 2: {  // gzgetc
                int ch = gzgetc(gz_file);
                // ch is -1 on error or EOF
                break;
            }
            
            case 3: {  // gzfread
                size_t item_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                size_t num_items = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                size_t total_size = item_size * num_items;
                
                if (total_size > 0 && total_size <= 4096) {
                    std::vector<char> buffer(total_size);
                    size_t items_read = gzfread(buffer.data(), item_size, num_items, gz_file);
                    // items_read is number of full items read
                }
                break;
            }
            
            case 4: {  // gzungetc
                char pushback_char = fdp.ConsumeIntegral<char>();
                int result = gzungetc(pushback_char, gz_file);
                // result is the character pushed back, or -1 on error
                break;
            }
            
            case 5: {  // gztell / gzoffset (get current position)
                if (mode_char == 'r') {
                    long pos = gztell(gz_file);
                    // pos is -1 on error
                } else {
                    long long pos = gzoffset(gz_file);
                    // pos is -1 on error
                }
                break;
            }
            
            case 6: {  // gzeof / gzdirect (query file status)
                int is_eof = gzeof(gz_file);  // 1 if at EOF, 0 otherwise
                int is_direct = gzdirect(gz_file);  // 1 if direct I/O, 0 if gzip
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

    // Close the gzip file
    int close_result = gzclose(gz_file);
    // close_result is Z_OK on success, Z_ERRNO on file error, 
    // Z_STREAM_ERROR if stream inconsistent, Z_BUF_ERROR if last gzread ended
    // in middle of gzip stream

    // Clean up temporary file
    unlink(temp_filename);

    return 0;
}

} // namespace harness_001
