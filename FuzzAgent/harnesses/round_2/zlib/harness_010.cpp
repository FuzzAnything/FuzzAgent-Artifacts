#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_010 {
/*
 * Fuzzing harness for focused gz file reading with emphasis on gzgetc_ 
 * Target: gzgetc_ (primary - 346 undiscovered branches), gzdopen, gzoffset, 
 *         gzfread, gzclearerr, gzgets, and related functions
 * Harness ID: 010
 * 
 * This harness focuses specifically on single-character reading patterns and 
 * file descriptor operations, targeting gzgetc_ which has 346+ undiscovered branches.
 * Different from harness_008 which covers comprehensive gz file reading operations.
 * 
 * Different from existing harnesses:
 * - harness_001 (basic gzip file I/O): covers basic read/write operations
 * - harness_004 (advanced gzip I/O with dynamic parameter control): focuses on parameter changes
 * - harness_005 (gzip formatted I/O): focuses on formatted I/O (gzfread, gzfwrite)
 * - harness_008 (comprehensive gz file reading): comprehensive coverage of all gz reading operations
 * 
 * This harness specifically targets single-character reading patterns:
 * 1. Intensive gzgetc_ usage with various file positions and states
 * 2. File descriptor operations with gzdopen for different file descriptors
 * 3. Offset tracking with gzoffset during character-by-character reading
 * 4. Buffered reading with gzfread in combination with single-character reads
 * 5. Error clearing with gzclearerr during interrupted reads
 * 6. Line reading with gzgets interspersed with single-character reads
 * 7. Pushback operations with gzungetc after character reads
 * 8. Mixed operation sequences focusing on state transitions
 * 
 * The harness creates diverse single-character reading scenarios:
 * - Sequential character-by-character reading from different file positions
 * - Reading from files opened with different modes and file descriptors
 * - Error recovery and state consistency during character reading
 * - Boundary cases for buffer sizes and file positions during character reads
 * - Concurrent character read/unget/offset operations
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_gzdopen = fdp.ConsumeBool();
    bool intensive_gzgetc = fdp.ConsumeBool();
    bool test_gzungetc = fdp.ConsumeBool();
    bool test_gzfread = fdp.ConsumeBool();
    bool test_gzoffset = fdp.ConsumeBool();
    bool test_gzclearerr = fdp.ConsumeBool();
    bool test_gzgets = fdp.ConsumeBool();
    bool test_mixed_operations = fdp.ConsumeBool();
    
    // Determine gzip mode - always read mode for reading operations
    std::string mode = "rb";
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (mode_selector) {
        case 0: mode = "rb"; break;     // Read binary
        case 1: mode = "rT"; break;     // Read transparent (no decompression)
        case 2: mode = "rb6"; break;    // Read with compression level hint
        case 3: mode = "r"; break;      // Read text mode
    }

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gzgetc_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return 0;
    }
    
    // Write some content to the file for reading tests
    // Generate file content from fuzzer input
    size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    if (fdp.remaining_bytes() < file_size) {
        close(fd);
        remove(temp_filename);
        return 0;
    }
    
    std::vector<uint8_t> file_content = fdp.ConsumeBytes<uint8_t>(file_size);
    
    // Add some newlines for gzgets testing
    if (test_gzgets && file_content.size() > 10) {
        // Insert some newlines at random positions
        for (int i = 0; i < 3 && file_content.size() > 2; i++) {
            size_t pos = fdp.ConsumeIntegralInRange<size_t>(1, file_content.size() - 1);
            file_content[pos] = '\n';
        }
    }
    
    // Write content to temporary file
    ssize_t written = write(fd, file_content.data(), file_content.size());
    if (written != (ssize_t)file_content.size()) {
        close(fd);
        remove(temp_filename);
        return 0;
    }
    
    // Open gzip file using either gzopen or gzdopen
    gzFile gzfile = NULL;
    if (use_gzdopen) {
        // Use gzdopen with file descriptor
        // Need to reopen file descriptor for gzdopen
        close(fd);
        fd = open(temp_filename, O_RDONLY);
        if (fd == -1) {
            remove(temp_filename);
            return 0;
        }
        gzfile = gzdopen(fd, mode.c_str());
    } else {
        // Use gzopen with filename
        close(fd);
        gzfile = gzopen(temp_filename, mode.c_str());
    }
    
    if (gzfile == NULL) {
        // Try with default mode if opening failed
        mode = "rb";
        if (use_gzdopen) {
            gzfile = gzdopen(fd, mode.c_str());
        } else {
            gzfile = gzopen(temp_filename, mode.c_str());
        }
        if (gzfile == NULL) {
            if (use_gzdopen) {
                close(fd);
            }
            remove(temp_filename);
            return 0;
        }
    }

    // Set buffer size for reading operations
    unsigned buffer_size = fdp.ConsumeIntegralInRange<unsigned>(128, 4096);
    gzbuffer(gzfile, buffer_size);
    
    // Track file position and state
    z_off_t current_pos = 0;
    bool error_occurred = false;
    int last_error = Z_OK;
    const char* last_error_msg = NULL;
    
    // Determine number of operations to perform
    uint8_t num_operations = fdp.ConsumeIntegral<uint8_t>() % 48 + 1;
    
    for (uint8_t i = 0; i < num_operations && !error_occurred && fdp.remaining_bytes() > 0; i++) {
        // Choose operation type based on test scenario and remaining input
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 16;
        
        switch (operation) {
            case 0:  // Intensive gzgetc_ testing - primary target
            case 1:
            case 2: {
                if (intensive_gzgetc) {
                    // Perform multiple gzgetc_ calls in sequence
                    uint8_t num_chars = fdp.ConsumeIntegralInRange<uint8_t>(1, 32);
                    for (uint8_t j = 0; j < num_chars && !error_occurred; j++) {
                        int c = gzgetc_(gzfile);  // Primary target API
                        if (c != -1) {
                            current_pos++;
                        } else {
                            // Check if it's EOF or error
                            if (gzeof(gzfile)) {
                                // EOF is expected, not an error
                                // Could rewind or break
                                if (test_mixed_operations) {
                                    gzrewind(gzfile);
                                    current_pos = 0;
                                }
                            } else {
                                last_error_msg = gzerror(gzfile, &last_error);
                                if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                                    error_occurred = true;
                                }
                            }
                        }
                    }
                }
                break;
            }
            
            case 3: {  // gzungetc - pushback after reading
                if (test_gzungetc) {
                    // First read a character to have something to unget
                    int c = gzgetc(gzfile);
                    if (c != -1) {
                        current_pos++;
                        // Now try to unget it
                        int unget_result = gzungetc(c, gzfile);
                        if (unget_result != -1) {
                            current_pos--;  // Position moved back
                        }
                    }
                }
                break;
            }
            
            case 4: {  // gzfread - buffered reading
                if (test_gzfread) {
                    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    std::vector<char> buffer(read_size);
                    size_t items_read = gzfread(buffer.data(), 1, read_size, gzfile);
                    if (items_read > 0) {
                        current_pos += items_read;
                    }
                }
                break;
            }
            
            case 5: {  // gzoffset - get current offset
                if (test_gzoffset) {
                    z_off_t offset = gzoffset(gzfile);
                    // Verify offset matches our tracked position (within reason)
                    // Allow for differences due to buffering
                    if (offset != -1 && offset < current_pos + 1024 && offset > current_pos - 1024) {
                        // Reasonable match
                    }
                }
                break;
            }
            
            case 6: {  // gzclearerr - clear error state
                if (test_gzclearerr) {
                    // Try to cause an error by reading past EOF, then clear it
                    if (gzeof(gzfile)) {
                        gzclearerr(gzfile);
                        // After clearing, we might be able to read again if rewound
                        gzrewind(gzfile);
                        current_pos = 0;
                    }
                }
                break;
            }
            
            case 7: {  // gzgets - line reading
                if (test_gzgets) {
                    size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(2, 256);
                    std::vector<char> buffer(buf_size);
                    char* result = gzgets(gzfile, buffer.data(), buf_size);
                    if (result != NULL) {
                        // Successfully read a line
                        size_t len = strlen(buffer.data());
                        if (len > 0) {
                            current_pos += len;
                        }
                    }
                }
                break;
            }
            
            case 8: {  // Mixed: gzgetc_ followed by gzoffset
                if (test_mixed_operations && intensive_gzgetc && test_gzoffset) {
                    int c = gzgetc_(gzfile);
                    if (c != -1) {
                        current_pos++;
                    }
                    z_off_t offset = gzoffset(gzfile);
                    // No validation, just call the API
                }
                break;
            }
            
            case 9: {  // Mixed: gzungetc followed by gzgetc_
                if (test_mixed_operations && test_gzungetc && intensive_gzgetc) {
                    int c = gzgetc(gzfile);
                    if (c != -1) {
                        current_pos++;
                        gzungetc(c, gzfile);
                        current_pos--;
                        // Read it back
                        int c2 = gzgetc_(gzfile);
                        if (c2 != -1) {
                            current_pos++;
                            // Should be the same character
                        }
                    }
                }
                break;
            }
            
            case 10: { // Check gzeof state
                // Simple check of EOF state
                (void)gzeof(gzfile);
                break;
            }
            
            case 11: { // Check gzerror state
                // Check error state without necessarily having an error
                int err = Z_OK;
                const char* err_msg = gzerror(gzfile, &err);
                (void)err_msg;  // Suppress unused warning
                break;
            }
            
            case 12: { // Get current position with gztell
                z_off_t pos = gztell(gzfile);
                (void)pos;  // Suppress unused warning
                break;
            }
            
            case 13: { // Test gzrewind
                // Rewind to beginning
                gzrewind(gzfile);
                current_pos = 0;
                break;
            }
            
            case 14: { // Test gzdirect
                // Check if file is in direct mode
                int is_direct = gzdirect(gzfile);
                (void)is_direct;  // Suppress unused warning
                break;
            }
            
            case 15: { // Intensive sequential gzgetc_ reading
                if (intensive_gzgetc) {
                    // Read a sequence of characters
                    uint8_t seq_len = fdp.ConsumeIntegralInRange<uint8_t>(1, 64);
                    for (uint8_t j = 0; j < seq_len && !gzeof(gzfile); j++) {
                        int c = gzgetc_(gzfile);
                        if (c != -1) {
                            current_pos++;
                        } else {
                            break;
                        }
                    }
                }
                break;
            }
        }
        
        // Occasionally check and clear errors
        if (test_gzclearerr && (i % 8 == 0)) {
            gzclearerr(gzfile);
        }
    }
    
    // Clean up
    gzclose_r(gzfile);  // Use gzclose_r for reading files
    
    // If we used gzdopen, fd was already closed by gzclose_r
    if (!use_gzdopen) {
        // File was closed by gzclose_r, just remove temp file
        remove(temp_filename);
    } else {
        // gzdopen closes the fd, so just remove temp file
        remove(temp_filename);
    }
    
    return 0;
}

} // namespace harness_010
