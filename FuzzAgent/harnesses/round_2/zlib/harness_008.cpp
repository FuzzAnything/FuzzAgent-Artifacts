#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_008 {
/*
 * Fuzzing harness for comprehensive gz file reading operations in zlib
 * Target: gzgetc, gzgets, gzread, gzungetc, gzeof, gzerror, gztell, gzseek, gzrewind
 * Harness ID: 008
 * 
 * This harness focuses specifically on comprehensive gz file reading operations
 * with emphasis on gzgetc which has 346+ undiscovered branches.
 * 
 * Different from existing harnesses:
 * - harness_001 (basic gzip file I/O): covers basic read/write operations
 * - harness_004 (advanced gzip I/O with dynamic parameter control): focuses on parameter changes
 * - harness_005 (gzip formatted I/O): focuses on formatted I/O (gzfread, gzfwrite)
 * 
 * This harness specifically targets reading operations with semantic diversity:
 * 1. Character-by-character reading patterns with gzgetc
 * 2. Line-oriented reading with gzgets and newline handling
 * 3. Buffer reading with gzread and partial/complete reads
 * 4. Pushback operations with gzungetc and pushback limits
 * 5. Position tracking with gztell during various read operations
 * 6. Seeking operations with gzseek and gzrewind
 * 7. EOF detection and error handling scenarios
 * 8. Mixed operation sequences and state transitions
 * 
 * The harness creates diverse reading scenarios:
 * - Sequential reading vs random access reading
 * - Reading from files with different content patterns
 * - Error recovery and state consistency checks
 * - Boundary cases for buffer sizes and file positions
 * - Concurrent read/unget/seek operations
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool test_char_reading = fdp.ConsumeBool();
    bool test_line_reading = fdp.ConsumeBool();
    bool test_buffer_reading = fdp.ConsumeBool();
    bool test_unget_operations = fdp.ConsumeBool();
    bool test_seek_operations = fdp.ConsumeBool();
    bool test_mixed_operations = fdp.ConsumeBool();
    bool test_error_scenarios = fdp.ConsumeBool();
    bool test_large_file = fdp.ConsumeBool();
    
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
    char temp_filename[] = "/tmp/gzip_read_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return 0;
    }
    
    // Write some content to the file for reading tests
    // Generate file content from fuzzer input
    size_t file_size = test_large_file ? 
        fdp.ConsumeIntegralInRange<size_t>(1024, 8192) :
        fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    if (fdp.remaining_bytes() < file_size) {
        close(fd);
        remove(temp_filename);
        return 0;
    }
    
    std::vector<uint8_t> file_content = fdp.ConsumeBytes<uint8_t>(file_size);
    
    // Add some newlines for gzgets testing
    if (test_line_reading && file_content.size() > 10) {
        // Insert some newlines at random positions
        for (int i = 0; i < 5 && file_content.size() > 2; i++) {
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
    close(fd);
    
    // Open gzip file for reading
    gzFile gzfile = gzopen(temp_filename, mode.c_str());
    if (gzfile == NULL) {
        // Try with default mode if opening failed
        mode = "rb";
        gzfile = gzopen(temp_filename, mode.c_str());
        if (gzfile == NULL) {
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
    uint8_t num_operations = fdp.ConsumeIntegral<uint8_t>() % 32 + 1;
    
    for (uint8_t i = 0; i < num_operations && !error_occurred && fdp.remaining_bytes() > 0; i++) {
        // Choose operation type based on test scenario and remaining input
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 16;
        
        switch (operation) {
            case 0: {  // gzgetc - read single character
                if (test_char_reading) {
                    int c = gzgetc(gzfile);
                    if (c != -1) {
                        current_pos++;
                    } else {
                        // Check if it's EOF or error
                        if (gzeof(gzfile)) {
                            // EOF is expected, not an error
                        } else {
                            last_error_msg = gzerror(gzfile, &last_error);
                            if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                                error_occurred = true;
                            }
                        }
                    }
                }
                break;
            }
            
            case 1: {  // gzgets - read a line
                if (test_line_reading) {
                    size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(2, 256);
                    std::vector<char> buffer(buf_size);
                    char* result = gzgets(gzfile, buffer.data(), buf_size);
                    if (result != NULL) {
                        // Successfully read a line
                        size_t len = strlen(buffer.data());
                        if (len > 0 && buffer[len-1] == '\n') {
                            // Full line read
                            current_pos += len;
                        } else if (len == buf_size - 1) {
                            // Buffer filled without newline
                            current_pos += len;
                        }
                    } else {
                        // Check if it's EOF or error
                        if (gzeof(gzfile)) {
                            // EOF is expected
                        } else {
                            last_error_msg = gzerror(gzfile, &last_error);
                            if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                                error_occurred = true;
                            }
                        }
                    }
                }
                break;
            }
            
            case 2: {  // gzread - read buffer
                if (test_buffer_reading) {
                    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    std::vector<uint8_t> buffer(read_size);
                    int bytes_read = gzread(gzfile, buffer.data(), read_size);
                    if (bytes_read > 0) {
                        current_pos += bytes_read;
                    } else if (bytes_read == 0) {
                        // Could be EOF
                        if (!gzeof(gzfile)) {
                            last_error_msg = gzerror(gzfile, &last_error);
                            if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                                error_occurred = true;
                            }
                        }
                    } else {
                        // Error (should be -1 for read error)
                        last_error_msg = gzerror(gzfile, &last_error);
                        if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                            error_occurred = true;
                        }
                    }
                }
                break;
            }
            
            case 3: {  // gzungetc - push back character
                if (test_unget_operations && current_pos > 0) {
                    // Can only unget if we've read something
                    int c = fdp.ConsumeIntegral<uint8_t>();
                    int result = gzungetc(c, gzfile);
                    if (result == c) {
                        // Successful unget
                        current_pos--;
                    } else if (result == -1) {
                        // unget failed - could be expected (e.g., too many ungotten chars)
                        // Check error but don't treat as fatal
                        last_error_msg = gzerror(gzfile, &last_error);
                    }
                }
                break;
            }
            
            case 4: {  // gztell - get current position
                z_off_t pos = gztell(gzfile);
                if (pos == -1) {
                    last_error_msg = gzerror(gzfile, &last_error);
                    if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                        error_occurred = true;
                    }
                } else {
                    // Update our tracked position
                    current_pos = pos;
                }
                break;
            }
            
            case 5: {  // gzseek - seek to position
                if (test_seek_operations) {
                    int whence = fdp.ConsumeIntegral<uint8_t>() % 3;
                    z_off_t offset;
                    
                    switch (whence) {
                        case 0:  // SEEK_SET
                            offset = fdp.ConsumeIntegralInRange<z_off_t>(0, file_content.size());
                            break;
                        case 1:  // SEEK_CUR
                            offset = fdp.ConsumeIntegralInRange<z_off_t>(-100, 100);
                            break;
                        case 2:  // SEEK_END
                            offset = fdp.ConsumeIntegralInRange<z_off_t>(-file_content.size(), 0);
                            break;
                        default:
                            offset = 0;
                    }
                    
                    z_off_t result = gzseek(gzfile, offset, whence);
                    if (result == -1) {
                        last_error_msg = gzerror(gzfile, &last_error);
                        // Seek errors are often expected (invalid positions)
                        if (last_error != Z_OK && last_error != Z_BUF_ERROR && 
                            last_error != Z_STREAM_ERROR) {
                            error_occurred = true;
                        }
                    } else {
                        // Successful seek
                        current_pos = result;
                    }
                }
                break;
            }
            
            case 6: {  // gzrewind - rewind to beginning
                if (test_seek_operations) {
                    int result = gzrewind(gzfile);
                    if (result == -1) {
                        last_error_msg = gzerror(gzfile, &last_error);
                        if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                            error_occurred = true;
                        }
                    } else {
                        current_pos = 0;
                    }
                }
                break;
            }
            
            case 7: {  // gzeof - check for end of file
                int eof = gzeof(gzfile);
                // EOF check shouldn't cause errors
                (void)eof; // Use result to avoid unused variable warning
                break;
            }
            
            case 8: {  // gzerror - check for errors
                if (test_error_scenarios) {
                    const char* err_msg;
                    int err;
                    err_msg = gzerror(gzfile, &err);
                    if (err != Z_OK && err != Z_BUF_ERROR && err != Z_STREAM_ERROR) {
                        // Only treat serious errors as fatal
                        error_occurred = true;
                    }
                }
                break;
            }
            
            case 9: {  // Mixed: read some chars then unget
                if (test_mixed_operations && test_char_reading && test_unget_operations) {
                    // Read a few characters
                    int chars_to_read = fdp.ConsumeIntegralInRange<int>(1, 5);
                    for (int j = 0; j < chars_to_read; j++) {
                        int c = gzgetc(gzfile);
                        if (c == -1) break;
                        current_pos++;
                    }
                    
                    // Try to unget one
                    if (current_pos > 0) {
                        int c = fdp.ConsumeIntegral<uint8_t>();
                        gzungetc(c, gzfile);
                        if (current_pos > 0) current_pos--;
                    }
                }
                break;
            }
            
            case 10: {  // Mixed: seek then read
                if (test_mixed_operations && test_seek_operations && test_buffer_reading) {
                    // Seek to a position
                    z_off_t offset = fdp.ConsumeIntegralInRange<z_off_t>(0, file_content.size() / 2);
                    gzseek(gzfile, offset, SEEK_SET);
                    current_pos = offset;
                    
                    // Read from new position
                    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    std::vector<uint8_t> buffer(read_size);
                    int bytes_read = gzread(gzfile, buffer.data(), read_size);
                    if (bytes_read > 0) {
                        current_pos += bytes_read;
                    }
                }
                break;
            }
            
            case 11: {  // Test read until EOF with gzgetc
                if (test_char_reading) {
                    int c;
                    int max_reads = fdp.ConsumeIntegralInRange<int>(1, 100);
                    for (int j = 0; j < max_reads; j++) {
                        c = gzgetc(gzfile);
                        if (c == -1) {
                            if (gzeof(gzfile)) {
                                // Reached EOF naturally
                                break;
                            } else {
                                last_error_msg = gzerror(gzfile, &last_error);
                                if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                                    error_occurred = true;
                                }
                                break;
                            }
                        }
                        current_pos++;
                    }
                }
                break;
            }
            
            case 12: {  // Test partial line reading
                if (test_line_reading) {
                    // Read with small buffer to get partial lines
                    size_t small_buf = fdp.ConsumeIntegralInRange<size_t>(2, 10);
                    std::vector<char> buffer(small_buf);
                    char* result = gzgets(gzfile, buffer.data(), small_buf);
                    if (result != NULL) {
                        size_t len = strlen(buffer.data());
                        current_pos += len;
                    }
                }
                break;
            }
            
            case 13: {  // Test read with zero length
                if (test_buffer_reading) {
                    // gzread with zero length should return 0
                    std::vector<uint8_t> buffer(1);
                    int bytes_read = gzread(gzfile, buffer.data(), 0);
                    if (bytes_read != 0) {
                        // This should not happen
                        last_error_msg = gzerror(gzfile, &last_error);
                        if (last_error != Z_OK && last_error != Z_BUF_ERROR) {
                            error_occurred = true;
                        }
                    }
                }
                break;
            }
            
            case 14: {  // Test multiple ungotten characters
                if (test_unget_operations && current_pos > 2) {
                    // Try to push back multiple characters
                    int num_unget = fdp.ConsumeIntegralInRange<int>(1, 3);
                    for (int j = 0; j < num_unget && current_pos > 0; j++) {
                        int c = fdp.ConsumeIntegral<uint8_t>();
                        int result = gzungetc(c, gzfile);
                        if (result == c) {
                            current_pos--;
                        } else {
                            // May fail after first unget due to buffer limits
                            break;
                        }
                    }
                }
                break;
            }
            
            case 15: {  // Test error scenarios
                if (test_error_scenarios) {
                    // Try invalid operations to trigger errors
                    // Seek to invalid position
                    gzseek(gzfile, -1, SEEK_SET);
                    last_error_msg = gzerror(gzfile, &last_error);
                    // Don't treat as fatal - invalid seeks are expected to fail
                }
                break;
            }
        }
        
        // Periodically check file position consistency
        if (i % 5 == 0 && !error_occurred) {
            z_off_t reported_pos = gztell(gzfile);
            if (reported_pos != -1 && reported_pos != current_pos) {
                // Position mismatch - could be due to buffering
                // Update our tracked position
                current_pos = reported_pos;
            }
        }
    }
    
    // Clean up
    gzclose(gzfile);
    remove(temp_filename);
    
    return 0;
}

} // namespace harness_008
