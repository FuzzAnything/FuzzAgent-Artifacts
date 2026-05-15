#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <cstdarg>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_025 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing of gzip file I/O lifecycle
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3; // 0: write/read, 1: line ops, 2: char ops
    bool test_formatted_io = fdp.ConsumeBool();
    bool use_different_modes = fdp.ConsumeBool();
    
    // Mode selection for gzopen
    uint8_t mode_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
    const char* modes[] = {"wb", "rb", "ab", "wb+", "rb+", "ab+"};
    const char* mode = modes[mode_idx];
    
    // Compression level
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    
    // Prepare test data for read/write operations
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(256, 4096);
    if (data_size > fdp.remaining_bytes()) {
        data_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> test_data;
    if (data_size > 0) {
        test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    }
    
    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gz_fuzz_lifecycle_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd < 0) {
        return 0;
    }
    
    // Close the file descriptor for gzopen
    close(temp_fd);
    
    // Step 1: Call gzopen to create/open a gzip file
    gzFile gzfile = gzopen(temp_filename, mode);
    if (gzfile == NULL) {
        unlink(temp_filename);
        return 0;
    }
    
    // Set compression level if in write mode
    if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
        gzsetparams(gzfile, compression_level, Z_DEFAULT_STRATEGY);
    }
    
    // Main testing based on operation mode
    switch (operation_mode) {
        case 0: {
            // Mixed read/write operations: gzwrite data, gzseek to beginning, gzread back
            // --------------------------------------------------------------------------
            if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
                // Write data to file
                size_t write_size = test_data.size();
                if (write_size > 0) {
                    int written = gzwrite(gzfile, test_data.data(), write_size);
                    (void)written; // Result not critical for fuzzing
                    
                    // Flush to ensure data is written
                    gzflush(gzfile, Z_SYNC_FLUSH);
                    
                    // Get current position
                    z_off_t pos = gztell(gzfile);
                    (void)pos;
                    
                    // Seek to beginning for reading
                    if (gzseek(gzfile, 0L, SEEK_SET) >= 0) {
                        // Read back the data
                        std::vector<uint8_t> read_buffer(write_size);
                        int bytes_read = gzread(gzfile, read_buffer.data(), write_size);
                        (void)bytes_read;
                        
                        // Check EOF status
                        int eof_status = gzeof(gzfile);
                        (void)eof_status;
                        
                        // Get position after read
                        pos = gztell(gzfile);
                        (void)pos;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test line operations: gzputs followed by gzgets
            // -------------------------------------------------
            if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
                // Generate lines from fuzzer input
                size_t line1_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (line1_len > fdp.remaining_bytes()) {
                    line1_len = fdp.remaining_bytes();
                }
                std::string line1 = fdp.ConsumeBytesAsString(line1_len);
                line1 += "\n"; // Add newline for line-oriented reading
                
                size_t line2_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (line2_len > fdp.remaining_bytes()) {
                    line2_len = fdp.remaining_bytes();
                }
                std::string line2 = fdp.ConsumeBytesAsString(line2_len);
                line2 += "\n"; // Add newline
                
                // Write lines
                gzputs(gzfile, line1.c_str());
                gzputs(gzfile, line2.c_str());
                
                gzflush(gzfile, Z_SYNC_FLUSH);
                
                // Seek to beginning for reading lines
                if (gzseek(gzfile, 0L, SEEK_SET) >= 0) {
                    char buffer[256];
                    // Read lines back
                    char* line_read = gzgets(gzfile, buffer, sizeof(buffer));
                    (void)line_read;
                    
                    line_read = gzgets(gzfile, buffer, sizeof(buffer));
                    (void)line_read;
                }
            }
            break;
        }
        
        case 2: {
            // Test character operations: gzputc followed by gzgetc
            // -----------------------------------------------------
            if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
                // Write some characters from test data
                size_t num_chars = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                if (num_chars > test_data.size()) {
                    num_chars = test_data.size();
                }
                for (size_t i = 0; i < num_chars; i++) {
                    gzputc(gzfile, test_data[i]);
                }
                
                gzflush(gzfile, Z_SYNC_FLUSH);
                
                // Seek to beginning for reading characters
                if (gzseek(gzfile, 0L, SEEK_SET) >= 0) {
                    for (size_t i = 0; i < num_chars; i++) {
                        int ch = gzgetc(gzfile);
                        (void)ch;
                    }
                }
            }
            break;
        }
    }
    
    // Test formatted I/O: gzprintf followed by gzfread (if enabled)
    if (test_formatted_io && (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+'))) {
        // Generate format string and values from fuzzer input
        size_t format_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        if (format_len > fdp.remaining_bytes()) {
            format_len = fdp.remaining_bytes();
        }
        std::string format_str = fdp.ConsumeBytesAsString(format_len);
        
        // Add basic format specifiers to ensure valid format
        format_str = "%d %f %s " + format_str;
        
        int int_val = fdp.ConsumeIntegral<int>();
        double double_val = fdp.ConsumeFloatingPoint<double>();
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        if (str_len > fdp.remaining_bytes()) {
            str_len = fdp.remaining_bytes();
        }
        std::string str_val = fdp.ConsumeBytesAsString(str_len);
        
        // Write formatted output
        gzprintf(gzfile, format_str.c_str(), int_val, double_val, str_val.c_str());
        
        gzflush(gzfile, Z_SYNC_FLUSH);
        
        // Seek to beginning for formatted read
        if (gzseek(gzfile, 0L, SEEK_SET) >= 0) {
            // Read with gzfread
            char read_buffer[1024];
            z_size_t items_read = gzfread(read_buffer, 1, sizeof(read_buffer), gzfile);
            (void)items_read;
        }
    }
    
    // Verify positions with gztell and EOF with gzeof
    z_off_t current_pos = gztell(gzfile);
    (void)current_pos;
    
    int eof_status = gzeof(gzfile);
    (void)eof_status;
    
    // Step 7: Call gzclose to clean up
    gzclose(gzfile);
    
    // Clean up temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_025
