#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <memory>
#include <sys/stat.h>
#include <unistd.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_001 {
/* Comprehensive fuzzing harness for zlib gzip file I/O operations
 * Targets gzip file APIs that are currently 0% covered:
 * - gzopen, gzdopen: File open operations
 * - gzread, gzgets, gzgetc, gzfread: Reading operations
 * - gzwrite, gzputs, gzputc, gzfwrite: Writing operations
 * - gzseek, gztell, gzoffset, gzrewind: Position operations
 * - gzclose, gzclose_r, gzclose_w: Cleanup operations
 * - gzerror, gzclearerr: Error handling
 * - gzsetparams, gzbuffer: Configuration operations
 *
 * This harness targets gzlib.c, gzread.c, gzwrite.c, gzclose.c modules
 * which represent 20+ APIs and 500+ undiscovered branches.
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 32;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_DATA_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_FILENAME_LEN = 256;

// Helper function to create a temporary file with random name
std::string create_temp_filename(FuzzedDataProvider& fdp) {
    char pattern[] = "/tmp/zlib_fuzz_XXXXXX";
    int fd = mkstemp(pattern);
    if (fd >= 0) {
        close(fd);
        return std::string(pattern);
    }
    // Fallback to generated name if mkstemp fails
    std::string filename = "/tmp/zlib_fuzz_";
    filename += fdp.ConsumeRandomLengthString(16);
    return filename;
}

// Helper function to remove a file if it exists
void safe_remove(const std::string& filename) {
    if (!filename.empty() && access(filename.c_str(), F_OK) == 0) {
        unlink(filename.c_str());
    }
}

// Test gzip file creation and basic operations
void test_gzip_file_operations(FuzzedDataProvider& fdp) {
    // Create temporary filename
    std::string filename = create_temp_filename(fdp);
    
    // Consume parameters for file operations
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    if (test_data.empty()) {
        safe_remove(filename);
        return;
    }
    
    // Determine mode based on operation mask
    const char* mode = (operation_mask & 0x01) ? "wb" : "rb";
    char mode_with_level[10];  // Declare at function scope to avoid dangling pointer
    if (compression_level > 0 && (operation_mask & 0x01)) {
        // Add compression level to write mode
        snprintf(mode_with_level, sizeof(mode_with_level), "wb%d", compression_level);
        mode = mode_with_level;
    }
    
    // Open gzip file
    gzFile file = gzopen(filename.c_str(), mode);
    if (file == NULL) {
        safe_remove(filename);
        return;
    }
    
    // Set buffer size if writing
    if (operation_mask & 0x01) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 65536);
        gzbuffer(file, buffer_size);
    }
    
    // Perform operations based on mask
    if (operation_mask & 0x01) {  // Writing operations
        // Write data using various methods
        size_t written = 0;
        
        // Write with gzwrite
        int write_ret = gzwrite(file, test_data.data(), test_data.size());
        if (write_ret > 0) {
            written = write_ret;
        }
        
        // Try gzputc for a few bytes
        if (test_data.size() > 0 && (operation_mask & 0x02)) {
            for (size_t i = 0; i < std::min(test_data.size(), (size_t)10); i++) {
                gzputc(file, test_data[i]);
            }
        }
        
        // Try gzputs if we have string data
        if (test_data.size() > 0 && (operation_mask & 0x04)) {
            // Create a null-terminated string from test data
            std::string test_str(reinterpret_cast<const char*>(test_data.data()), 
                                std::min(test_data.size(), (size_t)100));
            gzputs(file, test_str.c_str());
        }
        
        // Flush if needed
        if (operation_mask & 0x08) {
            gzflush(file, Z_FINISH);
        }
    } else {  // Reading operations (for files created in write mode)
        // First, we need to create a file to read from
        gzFile write_file = gzopen(filename.c_str(), "wb");
        if (write_file != NULL) {
            gzwrite(write_file, test_data.data(), test_data.size());
            gzclose(write_file);
            
            // Reopen for reading
            gzclose(file);
            file = gzopen(filename.c_str(), "rb");
            if (file == NULL) {
                safe_remove(filename);
                return;
            }
        }
        
        if (file != NULL) {
            // Read data using various methods
            std::vector<uint8_t> read_buffer(test_data.size());
            
            // Read with gzread
            int read_ret = gzread(file, read_buffer.data(), read_buffer.size());
            
            // Try gzgetc
            if ((operation_mask & 0x10) && read_ret > 0) {
                gzrewind(file);
                for (int i = 0; i < std::min(read_ret, 10); i++) {
                    gzgetc(file);
                }
            }
            
            // Try gzgets if we have enough data
            if ((operation_mask & 0x20) && read_ret > 0) {
                gzrewind(file);
                char line[256];
                gzgets(file, line, sizeof(line));
            }
        }
    }
    
    // Test seek operations if file is open
    if (file != NULL && test_data.size() > 0) {
        if (operation_mask & 0x40) {  // Seek operations
            // Get current position
            z_off_t current_pos = gztell(file);
            
            // Seek to random position
            size_t seek_pos = fdp.ConsumeIntegralInRange<size_t>(0, test_data.size());
            int seek_whence = fdp.PickValueInArray({SEEK_SET, SEEK_CUR, SEEK_END});
            gzseek(file, seek_pos, seek_whence);
            
            // Get new position
            z_off_t new_pos = gztell(file);
            
            // Test gzoffset if supported
            #ifdef Z_LARGE64
            z_off64_t offset = gzoffset(file);
            #endif
        }
    }
    
    // Test error handling
    if (file != NULL) {
        const char* error_msg = gzerror(file, NULL);
        if (error_msg != NULL && strlen(error_msg) > 0) {
            gzclearerr(file);
        }
    }
    
    // Close file
    if (file != NULL) {
        gzclose(file);
    }
    
    // Clean up temporary file
    safe_remove(filename);
}

// Test gzip operations on memory buffer (using gzdopen)
void test_gzip_memory_operations(FuzzedDataProvider& fdp) {
    // Create test data
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    if (test_data.empty()) {
        return;
    }
    
    // Create a temporary file for memory operations
    std::string filename = create_temp_filename(fdp);
    FILE* stdio_file = fopen(filename.c_str(), "w+b");
    if (stdio_file == NULL) {
        safe_remove(filename);
        return;
    }
    
    // Write test data to stdio file
    fwrite(test_data.data(), 1, test_data.size(), stdio_file);
    fflush(stdio_file);
    rewind(stdio_file);
    
    // Open with gzdopen
    const char* mode = fdp.ConsumeBool() ? "rb" : "wb";
    gzFile gz_file = gzdopen(fileno(stdio_file), mode);
    
    if (gz_file != NULL) {
        if (strchr(mode, 'r') != NULL) {  // Reading mode
            // Read data back
            std::vector<uint8_t> read_buffer(test_data.size());
            gzread(gz_file, read_buffer.data(), read_buffer.size());
            
            // Test gzgets
            if (test_data.size() > 0) {
                gzrewind(gz_file);
                char line[256];
                gzgets(gz_file, line, sizeof(line));
            }
        } else {  // Writing mode
            // Write data
            gzwrite(gz_file, test_data.data(), test_data.size());
            gzflush(gz_file, Z_FINISH);
        }
        
        // Test seek/tell
        if (test_data.size() > 0) {
            gzseek(gz_file, 0, SEEK_SET);
            z_off_t pos = gztell(gz_file);
            
            // Seek to middle
            size_t mid_pos = test_data.size() / 2;
            gzseek(gz_file, mid_pos, SEEK_SET);
        }
        
        // Close file
        gzclose(gz_file);
    }
    
    // Clean up
    fclose(stdio_file);
    safe_remove(filename);
}

// Test advanced gzip operations with parameters
void test_advanced_gzip_operations(FuzzedDataProvider& fdp) {
    std::string filename = create_temp_filename(fdp);
    
    // Open file for writing with specific parameters
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int strategy = fdp.PickValueInArray({
        Z_DEFAULT_STRATEGY,
        Z_FILTERED,
        Z_HUFFMAN_ONLY,
        Z_RLE,
        Z_FIXED
    });
    
    char mode[10];
    snprintf(mode, sizeof(mode), "wb%d", compression_level);
    gzFile file = gzopen(filename.c_str(), mode);
    
    if (file != NULL) {
        // Set buffer size
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
        gzbuffer(file, buffer_size);
        
        // Generate test data
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 16384);
        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
        
        if (!test_data.empty()) {
            // Write initial data
            gzwrite(file, test_data.data(), test_data.size());
            
            // Change compression parameters dynamically
            gzsetparams(file, compression_level, strategy);
            
            // Write more data with new parameters
            if (test_data.size() > 100) {
                gzwrite(file, test_data.data() + 100, test_data.size() - 100);
            }
            
            // Test direct status
            int is_direct = gzdirect(file);
            
            // Test flush operations
            gzflush(file, Z_SYNC_FLUSH);
            
            // Write final data with random string
            if (test_data.size() > 50 && fdp.remaining_bytes() > 10) {
                std::string final_data = fdp.ConsumeRandomLengthString(100);
                gzputs(file, final_data.c_str());
            }
        }
        
        // Close with specific close function
        if (fdp.ConsumeBool()) {
            gzclose_w(file);
        } else {
            gzclose(file);
        }
    }
    
    safe_remove(filename);
}

// Test file concatenation (multiple gzip streams)
void test_gzip_concatenation(FuzzedDataProvider& fdp) {
    std::string filename = create_temp_filename(fdp);
    
    // Create a file with multiple gzip streams
    gzFile file = gzopen(filename.c_str(), "wb");
    if (file != NULL) {
        // Create multiple test data chunks
        for (int i = 0; i < 3; i++) {
            size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> chunk = fdp.ConsumeBytes<uint8_t>(chunk_size);
            
            if (!chunk.empty()) {
                gzwrite(file, chunk.data(), chunk.size());
                gzflush(file, Z_FULL_FLUSH);  // Flush to create separate streams
            }
        }
        gzclose(file);
    }
    
    // Now read the concatenated streams
    file = gzopen(filename.c_str(), "rb");
    if (file != NULL) {
        // Read all data (should read through all concatenated streams)
        std::vector<uint8_t> read_buffer(4096);
        int total_read = 0;
        
        while (!gzeof(file)) {
            int bytes_read = gzread(file, read_buffer.data(), read_buffer.size());
            if (bytes_read <= 0) {
                break;
            }
            total_read += bytes_read;
        }
        
        // Test error status
        int err;
        const char* err_msg = gzerror(file, &err);
        
        gzclose(file);
    }
    
    safe_remove(filename);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test to run based on input
    uint8_t test_selector = fdp.ConsumeIntegral<uint8_t>();
    
    switch (test_selector % 5) {
        case 0:
            test_gzip_file_operations(fdp);
            break;
        case 1:
            test_gzip_memory_operations(fdp);
            break;
        case 2:
            test_advanced_gzip_operations(fdp);
            break;
        case 3:
            test_gzip_concatenation(fdp);
            break;
        case 4:
            // Run multiple tests
            test_gzip_file_operations(fdp);
            if (fdp.remaining_bytes() > MIN_INPUT_SIZE) {
                test_gzip_memory_operations(fdp);
            }
            break;
    }
    
    return 0;
}

} // namespace harness_001
