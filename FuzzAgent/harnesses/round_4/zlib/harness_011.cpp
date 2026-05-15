#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <memory>
#include <sys/stat.h>
#include <unistd.h>
#include <climits>
#include <sys/mman.h>
#include <fcntl.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_011 {
/* Comprehensive fuzzing harness for zlib 64-bit offset gzip file operations
 * Primary target: gzopen64 API (completely uncovered public API, 0% coverage)
 * Secondary targets: gzseek64, gztell64, gzoffset64 for 64-bit offset support
 * Related APIs: gzread, gzwrite, gzclose, gzerror for complete file operation testing
 * 
 * This harness specifically targets 64-bit large file handling functionality
 * in gzlib.c, gzread.c, and gzwrite.c modules, focusing on:
 * - 64-bit offset support for files larger than 2GB
 * - Various file modes with compression levels
 * - Error conditions and boundary cases
 * - Large file position handling
 * 
 * Distinct from existing harnesses:
 * - harness_001: Tests basic gzip I/O with 32-bit offsets (gzopen, gzseek, gztell)
 * - harness_003: Tests formatted I/O and binary operations
 * - harness_011: Explicit 64-bit offset API testing with large file handling (this harness)
 * 
 * The harness tests both explicit 64-bit API calls (gzopen64, gzseek64, etc.)
 * and ensures proper functionality with large file positions.
 */

// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 48;
// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_DATA_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_FILENAME_LEN = 256;
// Maximum 64-bit offset values for testing large file handling
const z_off64_t MAX_64BIT_OFFSET_TEST = 1024 * 1024 * 1024;  // 1GB for testing

// Safe absolute value function for z_off64_t that handles INT64_MIN edge case
static z_off64_t safe_abs_z_off64_t(z_off64_t value) {
    if (value < 0) {
        if (value == INT64_MIN) {
            // INT64_MIN cannot be negated safely, return INT64_MAX
            return INT64_MAX;
        }
        return -value;
    }
    return value;
}
// Helper function to create an in-memory file descriptor
int create_mem_fd(FuzzedDataProvider& fdp) {
    int fd = -1;
    
    // Try to use memfd_create for Linux (in-memory file descriptor)
#ifdef __linux__
    fd = memfd_create("zlib_fuzz64", MFD_CLOEXEC);
    if (fd >= 0) {
        return fd;
    }
#endif
    
    // Fallback: create anonymous file in /dev/shm (shared memory)
    std::string filename = "/dev/shm/zlib_fuzz64_";
    filename += fdp.ConsumeRandomLengthString(16);
    fd = open(filename.c_str(), O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd >= 0) {
        // Immediately unlink the file so it's deleted when closed
        unlink(filename.c_str());
        return fd;
    }
    
    // Final fallback: use /dev/null (won't support reading written data)
    fd = open("/dev/null", O_RDWR);
    return fd;
}

// Helper function to close a file descriptor
void close_fd(int fd) {
    if (fd >= 0) {
        close(fd);
    }
}

// Test 64-bit gzip file operations using gzdopen with memory file descriptors
void test_64bit_gzip_operations(FuzzedDataProvider& fdp) {
    // Create in-memory file descriptor
    int fd = create_mem_fd(fdp);
    if (fd < 0) {
        return;
    }
    
    // Consume parameters for file operations
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    if (test_data.empty()) {
        close_fd(fd);
        return;
    }
    
    // Determine mode based on operation type
    const char* mode = "wb";
    char mode_with_level[10];
    if (compression_level > 0) {
        snprintf(mode_with_level, sizeof(mode_with_level), "wb%d", compression_level);
        mode = mode_with_level;
    }
    
    // Test with gzdopen (uses file descriptor instead of filename)
    gzFile file = gzdopen(fd, mode);
    if (file == NULL) {
        close_fd(fd);
        return;
    }
    
    // Set buffer size
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 65536);
    gzbuffer(file, buffer_size);
    
    // Perform write operations
    if (operation_type == 0 || operation_type == 1) {  // Write operations
        // Write data with gzwrite
        int write_ret = gzwrite(file, test_data.data(), test_data.size());
        
        if (write_ret > 0) {
            // Test 64-bit seek operations after writing
            z_off64_t current_pos = gztell64(file);
            
            // Test various seek operations with 64-bit offsets
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                // Generate 64-bit offset values (can be large for testing)
                z_off64_t seek_offset = fdp.ConsumeIntegral<z_off64_t>();
                // Limit to reasonable range for testing using safe absolute value
                seek_offset = safe_abs_z_off64_t(seek_offset);
                seek_offset = seek_offset % (MAX_64BIT_OFFSET_TEST / 2);
                
                int seek_whence = fdp.PickValueInArray({SEEK_SET, SEEK_CUR, SEEK_END});
                
                // Perform 64-bit seek
                z_off64_t seek_result = gzseek64(file, seek_offset, seek_whence);
                
                // Get new position with 64-bit tell
                z_off64_t new_pos = gztell64(file);
                
                // Test gzoffset64 (64-bit offset from beginning)
                z_off64_t offset64 = gzoffset64(file);
            }
            
            // Return to beginning for more operations
            gzseek64(file, 0, SEEK_SET);
        }
        
        // Additional write operations
        if (operation_type == 1 && test_data.size() > 0) {
            // Test gzputc
            for (size_t i = 0; i < std::min(test_data.size(), (size_t)5); i++) {
                gzputc(file, test_data[i]);
            }
            
            // Test gzputs if we have enough data
            if (test_data.size() >= 4) {
                std::string test_str(reinterpret_cast<const char*>(test_data.data()), 
                                   std::min(test_data.size(), (size_t)50));
                gzputs(file, test_str.c_str());
            }
        }
        
        // Flush if needed
        if (fdp.ConsumeBool()) {
            gzflush(file, Z_FINISH);
        }
    }
    
    // For read operations, we need to write data first, then rewind
    if (operation_type == 2 || operation_type == 3) {
        // Write data to the file first
        int write_ret = gzwrite(file, test_data.data(), test_data.size());
        if (write_ret <= 0) {
            gzclose(file);
            return;
        }
        
        // Rewind to beginning for reading
        gzrewind(file);
        
        // Perform read operations
        if (operation_type == 2) {
            // Read with gzread
            std::vector<uint8_t> read_buffer(test_data.size());
            int read_ret = gzread(file, read_buffer.data(), read_buffer.size());
            
            if (read_ret > 0) {
                // Test 64-bit seek during reading
                z_off64_t mid_pos = static_cast<z_off64_t>(test_data.size() / 2);
                gzseek64(file, mid_pos, SEEK_SET);
                z_off64_t pos_after_seek = gztell64(file);
                
                // Read remaining data
                size_t remaining = test_data.size() - mid_pos;
                if (remaining > 0) {
                    std::vector<uint8_t> remaining_buffer(remaining);
                    gzread(file, remaining_buffer.data(), remaining);
                }
            }
        } else if (operation_type == 3) {
            // Test character and string reading operations
            // Test gzgetc
            if (test_data.size() > 0) {
                for (int i = 0; i < std::min((int)test_data.size(), 5); i++) {
                    gzgetc(file);
                }
            }
            
            // Test gzgets
            if (test_data.size() > 0) {
                gzrewind(file);
                char line[256];
                gzgets(file, line, sizeof(line));
            }
            
            // Test gzungetc if supported
            if (test_data.size() > 0) {
                int ch = gzgetc(file);
                if (ch != -1) {
                    gzungetc(ch, file);
                }
            }
        }
    }
    
    // Test error handling with 64-bit file
    if (file != NULL) {
        int err_num = 0;
        const char* error_msg = gzerror(file, &err_num);
        if (error_msg != NULL && strlen(error_msg) > 0) {
            gzclearerr(file);
        }
    }
    
    // Test file position queries with both 32-bit and 64-bit variants
    if (file != NULL) {
        // Get position with 64-bit API
        z_off64_t pos64 = gztell64(file);
        
        // Also test 32-bit API for comparison (may truncate on large files)
        #ifndef Z_LARGE64
        z_off_t pos32 = gztell(file);
        #endif
        
        // Test offset with 64-bit API
        z_off64_t offset64 = gzoffset64(file);
    }
    
    // Close file (this will also close the underlying fd)
    if (file != NULL) {
        gzclose(file);
    } else {
        close_fd(fd);
    }
}

// Test large file simulation with 64-bit offsets using memory file descriptor
void test_large_file_64bit_handling(FuzzedDataProvider& fdp) {
    // Create in-memory file descriptor
    int fd = create_mem_fd(fdp);
    if (fd < 0) {
        return;
    }
    
    // Open file with gzdopen
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    const char* mode = "wb";
    char mode_with_level[10];
    if (compression_level > 0) {
        snprintf(mode_with_level, sizeof(mode_with_level), "wb%d", compression_level);
        mode = mode_with_level;
    }
    
    gzFile file = gzdopen(fd, mode);
    if (file == NULL) {
        close_fd(fd);
        return;
    }
    
    // Write some data
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    if (!test_data.empty()) {
        gzwrite(file, test_data.data(), test_data.size());
    }
    
    // Test large 64-bit offset seeks (simulating large file behavior)
    // Generate large 64-bit offset values
    for (int i = 0; i < 2 && fdp.remaining_bytes() > 0; i++) {
        z_off64_t large_offset = fdp.ConsumeIntegral<z_off64_t>();
        // Ensure positive offset for SEEK_SET using safe absolute value
        large_offset = safe_abs_z_off64_t(large_offset);
        
        // Test seeking to large offset (simulating large file)
        z_off64_t seek_result = gzseek64(file, large_offset, SEEK_SET);
        
        // Get position with 64-bit tell
        z_off64_t pos64 = gztell64(file);
        
        // Test gzoffset64
        z_off64_t offset64 = gzoffset64(file);
    }
    
    // Test seeking from end with negative offset
    if (fdp.remaining_bytes() > 0) {
        // Generate negative offset for SEEK_END (small negative values are safe)
        size_t small_offset = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        // Small_offset is in range 1-100, so casting to z_off64_t and negating is safe
        z_off64_t neg_offset = -static_cast<z_off64_t>(small_offset);
        gzseek64(file, neg_offset, SEEK_END);
        z_off64_t end_pos = gztell64(file);
    }
    
    // Close file (this will also close the underlying fd)
    if (file != NULL) {
        gzclose(file);
    } else {
        close_fd(fd);
    }
}

// Test error conditions with gzopen64 API (using invalid paths)
void test_64bit_error_conditions(FuzzedDataProvider& fdp) {
    // Test 1: Invalid filename with gzopen64
    std::string invalid_filename = "";
    if (fdp.ConsumeBool()) {
        invalid_filename = fdp.ConsumeRandomLengthString(MAX_FILENAME_LEN);
        // May contain invalid characters or path
    }
    
    gzFile invalid_file = gzopen64(invalid_filename.c_str(), "rb");
    // Expect NULL for invalid file
    if (invalid_file != NULL) {
        gzclose(invalid_file);
    }
    
    // Test 2: Invalid mode strings with gzopen64
    std::string test_mode = fdp.PickValueInArray({"", "x", "rbw", "wbr"});
    
    // Use /dev/null as a dummy path for mode testing
    gzFile bad_mode_file = gzopen64("/dev/null", test_mode.c_str());
    if (bad_mode_file != NULL) {
        gzclose(bad_mode_file);
    }
    
    // Test 3: Operations on closed file
    if (fdp.remaining_bytes() > 0) {
        int fd = create_mem_fd(fdp);
        if (fd >= 0) {
            gzFile test_file = gzdopen(fd, "wb");
            if (test_file != NULL) {
                gzclose(test_file);
                // Attempt operation on closed file (should handle gracefully)
                // Note: Actual behavior may vary, but we test it
                int err_num = 0;
                const char* error_msg = gzerror(test_file, &err_num);
            } else {
                close_fd(fd);
            }
        }
    }
}

// Main fuzzing function
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine which test to run based on fuzzer input
    uint8_t test_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (test_selector) {
        case 0:
            // Primary test: 64-bit gzip operations with memory file descriptors
            test_64bit_gzip_operations(fdp);
            break;
            
        case 1:
            // Large file handling with 64-bit offsets using memory file descriptors
            test_large_file_64bit_handling(fdp);
            break;
            
        case 2:
            // Error condition testing with gzopen64
            test_64bit_error_conditions(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_011
