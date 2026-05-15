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

namespace harness_003 {
/* Advanced fuzzing harness for zlib gzip formatted I/O and binary operations
 * Targets gzprintf, gzvprintf, gzfwrite, gzfread, gzungetc APIs with 1374+ undiscovered branches:
 * - gzprintf: Formatted output to gzip file (215 branches)
 * - gzvprintf: Variadic formatted output (215 branches) 
 * - gzfwrite: Binary data writing (193 branches)
 * - gzfread: Binary data reading (377 branches)
 * - gzungetc: Character pushback (389 branches)
 *
 * This harness focuses on advanced gzip file I/O with formatted output (printf-style)
 * and binary read/write operations, semantically distinct from existing harnesses:
 * - harness_000: Core compression/decompression APIs
 * - harness_001: Basic gzip file I/O operations
 * - harness_002: Callback-based inflation
 * - harness_003: Advanced formatted I/O and binary operations (this harness)
 *
 * Implementation follows the sequence from coverage guidance:
 * 1. gzopen to create/access gzip file
 * 2. gzprintf/gzvprintf for formatted data output
 * 3. gzfwrite for binary data writing
 * 4. gzflush to ensure data written
 * 5. Reopen/seek for reading
 * 6. gzfread for binary data reading
 * 7. gzungetc for character pushback
 * 8. gzclose for cleanup
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 64;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_DATA_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_FILENAME_LEN = 256;
const size_t MAX_FORMAT_STRING_LEN = 256;

// Helper function to create a temporary file with random name
std::string create_temp_filename(FuzzedDataProvider& fdp) {
    char pattern[] = "/tmp/zlib_fuzz_adv_XXXXXX";
    int fd = mkstemp(pattern);
    if (fd >= 0) {
        close(fd);
        return std::string(pattern);
    }
    // Fallback to generated name if mkstemp fails
    std::string filename = "/tmp/zlib_fuzz_adv_";
    filename += fdp.ConsumeRandomLengthString(16);
    return filename;
}

// Helper function to remove a file if it exists
void safe_remove(const std::string& filename) {
    if (!filename.empty() && access(filename.c_str(), F_OK) == 0) {
        unlink(filename.c_str());
    }
}

// Custom vprintf-like function to test gzvprintf
static int custom_gzvprintf(gzFile file, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = gzvprintf(file, format, args);
    va_end(args);
    return result;
}

// Main fuzzing function for advanced gzip I/O operations
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create temporary filename
    std::string filename = create_temp_filename(fdp);
    
    // Consume parameters for file operations
    uint8_t operation_flags = fdp.ConsumeIntegral<uint8_t>();
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    
    // Determine mode based on operation flags
    const char* write_mode = "wb";
    char mode_with_level[10];
    if (compression_level > 0) {
        snprintf(mode_with_level, sizeof(mode_with_level), "wb%d", compression_level);
        write_mode = mode_with_level;
    }
    
    // ===== PHASE 1: WRITE OPERATIONS =====
    
    // Open gzip file for writing
    gzFile write_file = gzopen(filename.c_str(), write_mode);
    if (write_file == NULL) {
        safe_remove(filename);
        return 0;
    }
    
    // Set buffer size for formatted output
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 8192);
    gzbuffer(write_file, buffer_size);
    
    // 1. Test gzprintf with various format specifiers
    if (fdp.remaining_bytes() > 20) {
        // Consume format string
        std::string format_str = fdp.ConsumeRandomLengthString(MAX_FORMAT_STRING_LEN);
        if (!format_str.empty()) {
            // Consume arguments for format string
            int int_arg = fdp.ConsumeIntegral<int>();
            long long_arg = fdp.ConsumeIntegral<long>();
            double double_arg = fdp.ConsumeFloatingPoint<double>();
            std::string str_arg = fdp.ConsumeRandomLengthString(100);
            
            // Call gzprintf with mixed format specifiers
            int printf_result = gzprintf(write_file, format_str.c_str(), 
                                        int_arg, long_arg, double_arg, str_arg.c_str());
            
            // Result can be negative (error) or positive (bytes written)
            // We don't check result as fuzzing should explore both success and error paths
        }
    }
    
    // 2. Test gzvprintf (variadic version)
    if (fdp.remaining_bytes() > 10) {
        // Simple format string test
        int test_int = fdp.ConsumeIntegral<int>();
        std::string test_str = fdp.ConsumeRandomLengthString(50);
        
        // Use custom wrapper to test gzvprintf
        int vprintf_result = custom_gzvprintf(write_file, "Test: %d, %s", test_int, test_str.c_str());
    }
    
    // 3. Test gzfwrite with binary data
    if (fdp.remaining_bytes() > 100) {
        // Consume binary data for writing
        size_t binary_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_data_size);
        
        if (!binary_data.empty()) {
            // Test with different item sizes (1, 2, 4, 8 bytes)
            size_t item_size = fdp.PickValueInArray({1, 2, 4, 8});
            size_t num_items = binary_data.size() / item_size;
            
            if (num_items > 0) {
                // Write binary data using gzfwrite
                z_size_t write_result = gzfwrite(binary_data.data(), item_size, num_items, write_file);
                
                // Also test gzwrite for comparison
                if (binary_data.size() <= 65535) {  // gzwrite uses unsigned int
                    gzwrite(write_file, binary_data.data(), (unsigned)binary_data.size());
                }
            }
        }
    }
    
    // 4. Flush data to ensure it's written
    int flush_result = gzflush(write_file, Z_SYNC_FLUSH);
    
    // 5. Write some more data after flush
    if (fdp.remaining_bytes() > 10) {
        std::string post_flush_data = fdp.ConsumeRandomLengthString(100);
        gzputs(write_file, post_flush_data.c_str());
    }
    
    // Close the write file
    gzclose(write_file);
    
    // ===== PHASE 2: READ OPERATIONS =====
    
    // Reopen file for reading
    gzFile read_file = gzopen(filename.c_str(), "rb");
    if (read_file == NULL) {
        safe_remove(filename);
        return 0;
    }
    
    // 6. Test gzfread with binary data
    if (fdp.remaining_bytes() > 50) {
        // Create buffer for reading
        size_t read_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        std::vector<uint8_t> read_buffer(read_buffer_size);
        
        // Try different item sizes for reading
        size_t read_item_size = fdp.PickValueInArray({1, 2, 4, 8});
        size_t read_num_items = read_buffer_size / read_item_size;
        
        if (read_num_items > 0) {
            // Read binary data using gzfread
            z_size_t read_result = gzfread(read_buffer.data(), read_item_size, read_num_items, read_file);
            
            // Also test gzread for comparison
            if (read_buffer_size <= 65535) {
                gzread(read_file, read_buffer.data(), (unsigned)read_buffer_size);
            }
        }
    }
    
    // 7. Test gzungetc character pushback
    if (fdp.remaining_bytes() > 5) {
        // Consume character to push back
        int push_char = fdp.ConsumeIntegral<int>();
        
        // Try gzungetc with the character
        int unget_result = gzungetc(push_char, read_file);
        
        // Also test with -1 to force pending seek (documented behavior)
        if (fdp.ConsumeBool()) {
            gzungetc(-1, read_file);
        }
        
        // Read the character back (if unget succeeded)
        int read_char = gzgetc(read_file);
    }
    
    // 8. Test file position operations
    z_off_t current_pos = gztell(read_file);
    
    // Try seeking if we're not at beginning
    if (current_pos > 0 && fdp.remaining_bytes() > 5) {
        // Seek to a random position (within bounds)
        z_off_t seek_offset = fdp.ConsumeIntegralInRange<z_off_t>(0, current_pos);
        int whence = fdp.PickValueInArray({SEEK_SET, SEEK_CUR});
        gzseek(read_file, seek_offset, whence);
    }
    
    // 9. Test error handling
    if (fdp.remaining_bytes() > 5) {
        int err;
        const char* err_msg = gzerror(read_file, &err);
        
        // Clear error if any
        if (err != Z_OK) {
            gzclearerr(read_file);
        }
    }
    
    // Close the read file
    gzclose(read_file);
    
    // ===== PHASE 3: ADDITIONAL TESTING =====
    
    // Test reopening in append mode if enough data
    if (fdp.remaining_bytes() > 20) {
        gzFile append_file = gzopen(filename.c_str(), "ab");
        if (append_file != NULL) {
            std::string append_data = fdp.ConsumeRandomLengthString(100);
            gzputs(append_file, append_data.c_str());
            gzclose(append_file);
        }
    }
    
    // Test reading the entire file sequentially
    gzFile seq_file = gzopen(filename.c_str(), "rb");
    if (seq_file != NULL) {
        std::vector<uint8_t> seq_buffer(4096);
        int total_read = 0;
        
        while (!gzeof(seq_file)) {
            int bytes_read = gzread(seq_file, seq_buffer.data(), seq_buffer.size());
            if (bytes_read <= 0) {
                break;
            }
            total_read += bytes_read;
        }
        gzclose(seq_file);
    }
    
    // Clean up temporary file
    safe_remove(filename);
    
    return 0;
}

} // namespace harness_003
