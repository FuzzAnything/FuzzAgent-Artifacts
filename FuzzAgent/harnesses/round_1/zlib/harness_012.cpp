#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <algorithm>

namespace harness_012 {
/*
 * Fuzzing harness for zlib gzdopen API - file descriptor-based gzip operations
 * Target: gzdopen with 0% coverage, 4 undiscovered branches
 * 
 * This harness specifically targets gzdopen API for associating gzFiles with
 * existing file descriptors. Semantically different from existing harnesses:
 * - harness_001: Tests gzopen/gzclose with file paths
 * - harness_002: Tests gzopen/gzclose with memory buffers  
 * - This harness: Tests gzdopen with file descriptors (valid, pipe, invalid)
 * 
 * API sequence as specified by CoverageAnalyzerAgent:
 * 1. Create test file descriptors (valid fd, pipe fd, invalid fd)
 * 2. Call gzdopen with different mode strings ("r", "w", "a", "rb", "wb", invalid)
 * 3. Validate return with gzdirect() when appropriate
 * 4. Perform read/write operations based on mode
 * 5. Test error handling with gzerror()
 * 6. Cleanup with gzclose() and close()
 * 
 * Focus on error paths: invalid fd (-1), closed fd, NULL mode string
 * Related APIs: gzdopen, gzclose, gzdirect, gzerror, gztell
 * 
 * Strategy: Generate various file descriptor scenarios and mode strings
 * from fuzzer input to exercise gzdopen's initialization and error paths.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ==============================================
    // Consume fixed-size configuration parameters
    // ==============================================
    
    // Test scenario selection (0-7)
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // File descriptor type selection
    uint8_t fd_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Mode string configuration
    bool test_valid_modes = fdp.ConsumeBool();
    bool test_invalid_modes = fdp.ConsumeBool();
    bool test_null_mode = fdp.ConsumeBool();
    
    // Operation flags
    bool perform_read_write = fdp.ConsumeBool();
    bool test_error_handling = fdp.ConsumeBool();
    bool test_gzdirect = fdp.ConsumeBool();
    bool test_gztell = fdp.ConsumeBool();
    
    // Data size for read/write operations
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(16, 1024);
    if (fdp.remaining_bytes() < data_size) {
        return 0;
    }
    
    // Consume test data for read/write operations
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // ==============================================
    // Create file descriptors based on fd_type
    // ==============================================
    
    int fd = -1;
    int pipe_fds[2] = {-1, -1};
    bool pipe_created = false;
    bool regular_file_created = false;
    char temp_filename[] = "/tmp/gzdopen_fuzz_XXXXXX";
    
    switch (fd_type) {
        case 0:  // Valid file descriptor (regular file)
            fd = mkstemp(temp_filename);
            if (fd >= 0) {
                regular_file_created = true;
                // Write some initial data for reading tests
                if (test_data.size() > 0) {
                    write(fd, test_data.data(), std::min(test_data.size(), (size_t)256));
                    lseek(fd, 0, SEEK_SET);
                }
            }
            break;
            
        case 1:  // Pipe file descriptor (read end)
            if (pipe(pipe_fds) == 0) {
                pipe_created = true;
                fd = pipe_fds[0];  // Read end
                // Write some data to pipe for reading
                if (test_data.size() > 0) {
                    write(pipe_fds[1], test_data.data(), std::min(test_data.size(), (size_t)256));
                }
            }
            break;
            
        case 2:  // Pipe file descriptor (write end)
            if (pipe(pipe_fds) == 0) {
                pipe_created = true;
                fd = pipe_fds[1];  // Write end
            }
            break;
            
        case 3:  // Invalid file descriptor (-1)
            fd = -1;
            break;
            
        case 4:  // Closed file descriptor
            {
                int temp_fd = open("/dev/null", O_RDONLY);
                if (temp_fd >= 0) {
                    close(temp_fd);
                    fd = temp_fd;  // Now closed, but numeric value still exists
                }
            }
            break;
            
        case 5:  // Standard file descriptors (stdin/stdout/stderr)
            fd = fdp.ConsumeIntegralInRange<int>(0, 2);
            break;
    }
    
    // ==============================================
    // Test gzdopen with various mode strings
    // ==============================================
    
    // Valid mode strings from zlib documentation
    const char* valid_modes[] = {
        "r", "rb", "rt",
        "w", "wb", "wt", 
        "a", "ab", "at",
        "r+", "rb+", "r+b",
        "w+", "wb+", "w+b",
        "a+", "ab+", "a+b"
    };
    const size_t num_valid_modes = sizeof(valid_modes) / sizeof(valid_modes[0]);
    
    // Potentially invalid mode strings
    const char* invalid_modes[] = {
        "", "+", "x", "rx", "wx", "ax", "r++", "w++", "a++"
    };
    const size_t num_invalid_modes = sizeof(invalid_modes) / sizeof(invalid_modes[0]);
    
    gzFile gzfile = NULL;
    const char* mode_to_use = NULL;
    bool mode_is_valid = true;
    
    if (test_valid_modes && test_scenario < 3) {
        // Use a valid mode string
        uint8_t mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_valid_modes - 1);
        mode_to_use = valid_modes[mode_idx];
        mode_is_valid = true;
    }
    else if (test_invalid_modes && test_scenario >= 3 && test_scenario < 6) {
        // Use an invalid mode string
        uint8_t mode_idx = fdp.ConsumeIntegralInRange<uint8_t>(0, num_invalid_modes - 1);
        mode_to_use = invalid_modes[mode_idx];
        mode_is_valid = false;
    }
    else if (test_null_mode && test_scenario == 7) {
        // Test with NULL mode string (should fail)
        mode_to_use = NULL;
        mode_is_valid = false;
    }
    else {
        // Default to a valid mode
        mode_to_use = "rb";
        mode_is_valid = true;
    }
    
    // Call gzdopen with the selected file descriptor and mode
    if (mode_to_use != NULL) {
        gzfile = gzdopen(fd, mode_to_use);
    } else {
        // Explicitly test with NULL mode
        gzfile = gzdopen(fd, NULL);
    }
    
    // ==============================================
    // Validate and operate on the gzFile
    // ==============================================
    
    if (gzfile != NULL) {
        // gzdopen succeeded
        
        // Test gzdirect() - check if file is in direct (uncompressed) mode
        if (test_gzdirect) {
            int is_direct = gzdirect(gzfile);
            // Note: gzdirect returns 1 for direct mode, 0 for gzip mode
            // Returns -1 if not valid or error
        }
        
        // Test gztell() - get current position
        if (test_gztell) {
            z_off_t pos = gztell(gzfile);
            // Position could be -1 on error
        }
        
        // Perform read/write operations based on mode
        if (perform_read_write && gzfile != NULL) {
            // Determine if mode indicates reading or writing
            bool is_read_mode = (mode_to_use != NULL && mode_to_use[0] == 'r');
            bool is_write_mode = (mode_to_use != NULL && mode_to_use[0] == 'w');
            bool is_append_mode = (mode_to_use != NULL && mode_to_use[0] == 'a');
            
            if (is_read_mode || (mode_to_use != NULL && strchr(mode_to_use, '+') != NULL)) {
                // Try reading
                char read_buffer[256];
                int bytes_read = gzread(gzfile, read_buffer, sizeof(read_buffer));
                
                // If we read something, try seeking back
                if (bytes_read > 0) {
                    // Try to seek back to beginning
                    gzrewind(gzfile);
                }
            }
            
            if (is_write_mode || is_append_mode || (mode_to_use != NULL && strchr(mode_to_use, '+') != NULL)) {
                // Try writing
                if (test_data.size() > 0) {
                    size_t write_size = std::min(test_data.size(), (size_t)128);
                    int bytes_written = gzwrite(gzfile, test_data.data(), write_size);
                    
                    // Flush if anything was written
                    if (bytes_written > 0) {
                        gzflush(gzfile, Z_SYNC_FLUSH);
                    }
                }
            }
        }
        
        // Test error handling
        if (test_error_handling) {
            int errnum = 0;
            const char* errmsg = gzerror(gzfile, &errnum);
            
            // Clear any errors
            if (errnum != Z_OK) {
                gzclearerr(gzfile);
            }
        }
        
        // Close the gzFile (will also close the file descriptor)
        if (gzfile != NULL) {
            int close_result = gzclose(gzfile);
            gzfile = NULL;
            
            // Don't use fd after gzclose - it's closed
            if (close_result == Z_OK) {
                fd = -1;  // Mark as closed
            }
        }
    } else {
        // gzdopen failed - test expected error cases
        
        // fd == -1 should always fail
        // invalid mode strings should fail
        // NULL mode should fail
        
        // Get error information if possible (though gzfile is NULL)
        errno = 0;  // Clear errno
        
        // Don't try to close NULL gzfile
    }
    
    // ==============================================
    // Cleanup remaining file descriptors
    // ==============================================
    
    // Close pipe file descriptors if we created them
    if (pipe_created) {
        if (pipe_fds[0] >= 0 && pipe_fds[0] != fd) {
            close(pipe_fds[0]);
        }
        if (pipe_fds[1] >= 0 && pipe_fds[1] != fd) {
            close(pipe_fds[1]);
        }
    }
    
    // Close regular file descriptor if not already closed by gzclose
    if (fd >= 0 && fd != pipe_fds[0] && fd != pipe_fds[1]) {
        close(fd);
        fd = -1;
    }
    
    // Remove temporary file if we created one
    if (regular_file_created) {
        unlink(temp_filename);
    }
    
    // ==============================================
    // Additional test scenarios for coverage
    // ==============================================
    
    // Test scenario variations
    switch (test_scenario) {
        case 0:  // Basic valid fd with valid mode
            // Already covered above
            break;
            
        case 1:  // Test with dup()'d file descriptor
            {
                int temp_fd = open("/dev/null", O_RDONLY);
                if (temp_fd >= 0) {
                    int dup_fd = dup(temp_fd);
                    if (dup_fd >= 0) {
                        gzFile dup_gzfile = gzdopen(dup_fd, "rb");
                        if (dup_gzfile != NULL) {
                            // Quick operation and close
                            char buf[16];
                            gzread(dup_gzfile, buf, sizeof(buf));
                            gzclose(dup_gzfile);
                        } else {
                            close(dup_fd);
                        }
                    }
                    close(temp_fd);
                }
            }
            break;
            
        case 2:  // Test with O_NONBLOCK flag (non-blocking mode)
            {
                int nonblock_fd = open("/dev/null", O_RDONLY | O_NONBLOCK);
                if (nonblock_fd >= 0) {
                    // According to zlib.h, 'N' in mode sets non-blocking flag
                    gzFile nonblock_gzfile = gzdopen(nonblock_fd, "rN");
                    if (nonblock_gzfile != NULL) {
                        gzclose(nonblock_gzfile);
                    } else {
                        close(nonblock_fd);
                    }
                }
            }
            break;
            
        case 3:  // Test multiple gzdopen calls on same descriptor (should fail)
            if (fd >= 0 && fd != pipe_fds[0] && fd != pipe_fds[1]) {
                gzFile gz1 = gzdopen(fd, "rb");
                if (gz1 != NULL) {
                    // Second call should work but creates independent state
                    gzFile gz2 = gzdopen(fd, "rb");
                    if (gz2 != NULL) {
                        gzclose(gz2);
                    }
                    gzclose(gz1);
                }
            }
            break;
            
        case 4:  // Test with 'e' flag (close on exec)
            {
                int exec_fd = open("/dev/null", O_RDONLY);
                if (exec_fd >= 0) {
                    // 'e' in mode sets close-on-exec flag
                    gzFile exec_gzfile = gzdopen(exec_fd, "rbe");
                    if (exec_gzfile != NULL) {
                        gzclose(exec_gzfile);
                    } else {
                        close(exec_fd);
                    }
                }
            }
            break;
            
        case 5:  // Test gzbuffer() after gzdopen
            if (fd >= 0) {
                gzFile buffer_gzfile = gzdopen(fd, "wb");
                if (buffer_gzfile != NULL) {
                    // Set buffer size (must be called before any read/write)
                    int buffer_result = gzbuffer(buffer_gzfile, 16384);
                    // Write some data
                    if (test_data.size() > 0) {
                        gzwrite(buffer_gzfile, test_data.data(), 
                               std::min(test_data.size(), (size_t)64));
                    }
                    gzclose(buffer_gzfile);
                }
            }
            break;
            
        case 6:  // Test gzsetparams() after gzdopen for writing
            if (fd >= 0) {
                gzFile params_gzfile = gzdopen(fd, "wb");
                if (params_gzfile != NULL) {
                    // Set compression parameters
                    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
                    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
                    int params_result = gzsetparams(params_gzfile, level, strategy);
                    
                    // Write with new parameters
                    if (test_data.size() > 0) {
                        gzwrite(params_gzfile, test_data.data(), 
                               std::min(test_data.size(), (size_t)64));
                    }
                    gzclose(params_gzfile);
                }
            }
            break;
            
        case 7:  // Comprehensive error testing
            // Test various error conditions
            gzdopen(-1, "rb");                     // Invalid fd
            gzdopen(999999, "rb");                 // Very large (likely invalid) fd
            gzdopen(fd, "");                       // Empty mode
            gzdopen(fd, "+");                      // Invalid mode
            gzdopen(fd, NULL);                     // NULL mode
            
            // Test gzdirect on NULL
            if (gzfile == NULL) {
                // gzdirect should handle NULL
                gzdirect(NULL);
            }
            
            // Test gzerror on NULL
            if (gzfile == NULL) {
                int errnum = 0;
                gzerror(NULL, &errnum);
            }
            break;
    }
    
    return 0;
}

} // namespace harness_012
