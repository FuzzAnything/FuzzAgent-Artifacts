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

namespace harness_013 {

// Track internal coverage by exercising all possible code paths
static struct coverage_tracker {
    // File operations
    int files_opened;
    int files_closed;
    int read_operations;
    int write_operations;
    int seek_operations;
    int flush_operations;
    
    // Modes tested
    int read_mode_tested;
    int write_mode_tested;
    int append_mode_tested;
    int transparent_mode_tested;
    
    // Buffer sizes tested
    int small_buffer_tested;
    int large_buffer_tested;
    int default_buffer_tested;
    
    // Error conditions tested
    int null_param_tested;
    int invalid_mode_tested;
    int invalid_seek_tested;
    int eof_tested;
    int error_check_tested;
    
    // Special features
    int gzdopen_tested;
    int gzflush_variants_tested;
    int gzseek_variants_tested;
    int gzgetc_ungetc_tested;
    int gzputs_gets_tested;
    int gzfread_tested;
    int gzoffset_tested;
    int gzdirect_tested;
    int gzclearerr_tested;
    int gzrewind_tested;
    int gzbuffer_tested;
} coverage;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Initialize coverage tracker
    memset(&coverage, 0, sizeof(coverage));
    
    // Minimum size for meaningful testing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Primary test selection
    uint8_t primary_test = fdp.ConsumeIntegral<uint8_t>() % 16;
    
    // Secondary parameters
    bool test_multiple_files = fdp.ConsumeBool();
    bool test_varied_buffers = fdp.ConsumeBool();
    bool test_error_conditions = fdp.ConsumeBool();
    bool test_edge_cases = fdp.ConsumeBool();
    bool test_all_apis = fdp.ConsumeBool() || (primary_test == 8); // Force API test for scenario 8
    
    int compression_level = fdp.ConsumeIntegralInRange<int>(-1, 9);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1, 131072);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Create test content
    size_t content_size = fdp.ConsumeIntegralInRange<size_t>(256, 16384);
    if (content_size > fdp.remaining_bytes()) {
        content_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> test_content;
    if (content_size > 0) {
        test_content = fdp.ConsumeBytes<uint8_t>(content_size);
    }
    
    // Create temporary files
    const int MAX_FILES = 4;
    char temp_filenames[MAX_FILES][64];
    int temp_fds[MAX_FILES];
    gzFile gz_files[MAX_FILES] = {NULL};
    
    for (int i = 0; i < MAX_FILES; i++) {
        snprintf(temp_filenames[i], sizeof(temp_filenames[i]), "/tmp/gz_test_%d_XXXXXX", i);
        temp_fds[i] = mkstemp(temp_filenames[i]);
        if (temp_fds[i] < 0) {
            // Clean up previously created files
            for (int j = 0; j < i; j++) {
                close(temp_fds[j]);
                unlink(temp_filenames[j]);
            }
            return 0;
        }
        
        // Write different types of content to each file
        if (!test_content.empty()) {
            if (i == 0) {
                // File 0: Compressed data
                uLongf compr_bound = compressBound(test_content.size());
                if (compr_bound > 0 && compr_bound <= 10 * 1024 * 1024) {
                    Bytef* compr = (Bytef*)malloc(compr_bound);
                    if (compr) {
                        uLongf compr_len = compr_bound;
                        if (compress(compr, &compr_len, test_content.data(), test_content.size()) == Z_OK) {
                            write(temp_fds[i], compr, compr_len);
                        }
                        free(compr);
                    }
                }
            } else if (i == 1) {
                // File 1: Raw data for transparent reading
                write(temp_fds[i], test_content.data(), test_content.size());
            } else {
                // Files 2-3: Mixed content
                size_t write_size = test_content.size() / (i + 1);
                if (write_size > 0) {
                    write(temp_fds[i], test_content.data(), write_size);
                }
            }
        }
        
        close(temp_fds[i]);
    }
    
    // Main test scenarios designed to maximize gzip module coverage
    switch (primary_test) {
        case 0: { // Comprehensive read operations (targets gzread.c internals)
            coverage.read_mode_tested++;
            for (int i = 0; i < (test_multiple_files ? 2 : 1); i++) {
                char mode_buf[16];
                const char* mode = "rb";
                if (compression_level >= 0 && compression_level <= 9) {
                    snprintf(mode_buf, sizeof(mode_buf), "rb%d", compression_level);
                    mode = mode_buf;
                }
                
                gz_files[i] = gzopen(temp_filenames[i], mode);
                if (gz_files[i]) {
                    coverage.files_opened++;
                    coverage.default_buffer_tested++;
                    char buffer[8192];
                    size_t total_read = 0;
                    size_t max_read = fdp.ConsumeIntegralInRange<size_t>(1, 65536);
                    
                    while (total_read < max_read) {
                        // Vary read sizes to test different buffer management
                        size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, sizeof(buffer));
                        
                        int bytes = gzread(gz_files[i], buffer, read_size);
                        coverage.read_operations++;
                        
                        if (bytes <= 0) {
                            coverage.eof_tested++;
                            break;
                        }
                        total_read += bytes;
                        
                        // Occasionally test other read functions
                        if (fdp.ConsumeBool()) {
                            int ch = gzgetc(gz_files[i]);
                            coverage.gzgetc_ungetc_tested++;
                            if (ch != -1) {
                                gzungetc(ch, gz_files[i]);
                                coverage.gzgetc_ungetc_tested++;
                            }
                        }
                        
                        if (fdp.ConsumeBool() && total_read > 10) {
                            // Test seeking
                            z_off_t pos = gztell(gz_files[i]);
                            coverage.seek_operations++;
                            
                            int whence = fdp.ConsumeIntegralInRange<int>(0, 2);
                            z_off_t offset = fdp.ConsumeIntegralInRange<z_off_t>(0, pos > 100 ? 100 : pos);
                            gzseek(gz_files[i], offset, whence);
                            coverage.gzseek_variants_tested++;
                        }
                    }
                    
                    // Test remaining read APIs
                    coverage.gzoffset_tested++;
                    z_off_t offset = gzoffset(gz_files[i]);
                    
                    coverage.gzdirect_tested++;
                    int direct = gzdirect(gz_files[i]);
                    
                    coverage.gzclearerr_tested++;
                    gzclearerr(gz_files[i]);
                    
                    coverage.gzrewind_tested++;
                    gzrewind(gz_files[i]);
                    
                    // Test structured read
                    char fread_buf[1024];
                    coverage.gzfread_tested++;
                    gzfread(fread_buf, 1, sizeof(fread_buf), gz_files[i]);
                    
                    // Test line read
                    char line_buf[256];
                    coverage.gzputs_gets_tested++;
                    gzgets(gz_files[i], line_buf, sizeof(line_buf));
                    
                    // Check for errors
                    int errnum;
                    coverage.error_check_tested++;
                    const char* errmsg = gzerror(gz_files[i], &errnum);
                    
                    coverage.eof_tested++;
                    int eof = gzeof(gz_files[i]);
                }
            }
            break;
        }
        
        case 1: { // Comprehensive write operations (targets gzwrite.c internals)
            coverage.write_mode_tested++;
            for (int i = 0; i < (test_multiple_files ? 2 : 1); i++) {
                char mode_buf[16];
                const char* mode = "wb";
                if (strategy == 1) mode = "wf";
                else if (strategy == 2) mode = "wh";
                else if (strategy == 3) mode = "wR";
                else if (strategy == 4) mode = "wF";
                
                if (compression_level >= 0 && compression_level <= 9) {
                    snprintf(mode_buf, sizeof(mode_buf), "w%c%d", mode[1], compression_level);
                    mode = mode_buf;
                }
                
                gz_files[i] = gzopen(temp_filenames[i], mode);
                if (gz_files[i]) {
                    coverage.files_opened++;
                    // Set buffer size if requested
                    if (test_varied_buffers) {
                        if (buffer_size < 1024) coverage.small_buffer_tested++;
                        else if (buffer_size > 65536) coverage.large_buffer_tested++;
                        else coverage.default_buffer_tested++;
                        
                        gzbuffer(gz_files[i], buffer_size);
                        coverage.gzbuffer_tested++;
                    }
                    
                    // Write data in various patterns
                    if (!test_content.empty()) {
                        size_t total_written = 0;
                        while (total_written < test_content.size()) {
                            size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
                            if (write_size > test_content.size() - total_written) {
                                write_size = test_content.size() - total_written;
                            }
                            
                            int bytes = gzwrite(gz_files[i], test_content.data() + total_written, write_size);
                            coverage.write_operations++;
                            if (bytes <= 0) break;
                            total_written += bytes;
                            
                            // Occasionally flush
                            if (fdp.ConsumeBool()) {
                                int flush_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                                gzflush(gz_files[i], flush_type);
                                coverage.flush_operations++;
                                coverage.gzflush_variants_tested++;
                            }
                            
                            // Test character and string writes
                            if (fdp.ConsumeBool()) {
                                gzputc(gz_files[i], 'X');
                                coverage.gzputs_gets_tested++;
                            }
                            if (fdp.ConsumeBool()) {
                                gzputs(gz_files[i], "test string");
                                coverage.gzputs_gets_tested++;
                            }
                        }
                        
                        // Final flush
                        gzflush(gz_files[i], Z_FINISH);
                        coverage.flush_operations++;
                    }
                }
            }
            break;
        }
        
        case 2: { // Append mode testing
            coverage.append_mode_tested++;
            char mode_buf[16];
            const char* mode = "ab";
            if (compression_level >= 0 && compression_level <= 9) {
                snprintf(mode_buf, sizeof(mode_buf), "ab%d", compression_level);
                mode = mode_buf;
            }
            
            gz_files[0] = gzopen(temp_filenames[0], mode);
            if (gz_files[0]) {
                coverage.files_opened++;
                if (!test_content.empty()) {
                    gzwrite(gz_files[0], test_content.data(), test_content.size());
                    coverage.write_operations++;
                    gzflush(gz_files[0], Z_FINISH);
                    coverage.flush_operations++;
                }
            }
            break;
        }

        case 3: { // File descriptor based operations
            coverage.gzdopen_tested++;
            for (int i = 0; i < (test_multiple_files ? 2 : 1); i++) {
                int fd = open(temp_filenames[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
                if (fd >= 0) {
                    if (!test_content.empty()) {
                        write(fd, test_content.data(), test_content.size());
                        lseek(fd, 0, SEEK_SET);
                    }
                    
                    const char* mode = i == 0 ? "rb" : "wb";
                    gz_files[i] = gzdopen(fd, mode);
                    if (gz_files[i]) {
                        coverage.files_opened++;
                        
                        if (i == 0) {
                            char buf[1024];
                            gzread(gz_files[i], buf, sizeof(buf));
                            coverage.read_operations++;
                        } else {
                            if (!test_content.empty()) {
                                gzwrite(gz_files[i], test_content.data(), test_content.size());
                                coverage.write_operations++;
                            }
                        }
                    } else {
                        close(fd);
                    }
                }
            }
            break;
        }
        
        case 4: { // Transparent mode (reading non-gzip files)
            coverage.transparent_mode_tested++;
            gz_files[0] = gzopen(temp_filenames[1], "rb"); // File 1 has raw data
            if (gz_files[0]) {
                coverage.files_opened++;
                char buf[4096];
                int bytes = gzread(gz_files[0], buf, sizeof(buf));
                coverage.read_operations++;
                
                // Test that it's reading directly
                coverage.gzdirect_tested++;
                int direct = gzdirect(gz_files[0]);
            }
            break;
        }
        
        case 5: { // Buffer size variations
            coverage.gzbuffer_tested++;
            const char* modes[] = {"rb", "wb", "ab"};
            for (int i = 0; i < (test_multiple_files ? 3 : 1); i++) {
                gz_files[i] = gzopen(temp_filenames[i], modes[i % 3]);
                if (gz_files[i]) {
                    coverage.files_opened++;
                    
                    // Test different buffer sizes
                    int sizes[] = {1, 128, 1024, 8192, 65536, 131072};
                    for (size_t j = 0; j < sizeof(sizes)/sizeof(sizes[0]); j++) {
                        if (sizes[j] <= buffer_size * 2) { // Limit based on fuzzer input
                            gzbuffer(gz_files[i], sizes[j]);
                            
                            if (sizes[j] < 1024) coverage.small_buffer_tested++;
                            else if (sizes[j] > 65536) coverage.large_buffer_tested++;
                            else coverage.default_buffer_tested++;
                        }
                    }
                    
                    // Perform operation with current buffer
                    if (i % 3 == 0) { // read
                        char buf[1024];
                        gzread(gz_files[i], buf, sizeof(buf));
                        coverage.read_operations++;
                    } else { // write or append
                        if (!test_content.empty()) {
                            gzwrite(gz_files[i], test_content.data(), test_content.size());
                            coverage.write_operations++;
                        }
                    }
                }
            }
            break;
        }
        
        case 6: { // Seek and tell operations
            coverage.gzseek_variants_tested++;
            gz_files[0] = gzopen(temp_filenames[0], "rb");
            if (gz_files[0]) {
                coverage.files_opened++;
                
                // Read some data first
                char buf[4096];
                int bytes = gzread(gz_files[0], buf, sizeof(buf));
                coverage.read_operations++;
                
                if (bytes > 0) {
                    // Test various seek operations
                    z_off_t pos = gztell(gz_files[0]);
                    coverage.seek_operations++;
                    
                    // SEEK_SET
                    gzseek(gz_files[0], 0, SEEK_SET);
                    coverage.seek_operations++;
                    
                    // SEEK_CUR
                    gzseek(gz_files[0], 10, SEEK_CUR);
                    coverage.seek_operations++;
                    
                    // SEEK_END (if supported)
                    gzseek(gz_files[0], -20, SEEK_END);
                    coverage.seek_operations++;
                    
                    // Test gzoffset
                    coverage.gzoffset_tested++;
                    z_off_t offset = gzoffset(gz_files[0]);
                    
                    // Test gzrewind
                    coverage.gzrewind_tested++;
                    gzrewind(gz_files[0]);
                    
                    // Read from beginning
                    gzread(gz_files[0], buf, sizeof(buf));
                    coverage.read_operations++;
                }
            }
            break;
        }
        
        case 7: { // Error condition testing
            coverage.null_param_tested++;
            coverage.invalid_mode_tested++;
            coverage.invalid_seek_tested++;
            
            // Test NULL parameters
            gzopen(NULL, "rb");
            gzopen(temp_filenames[0], NULL);
            gzdopen(-1, "rb");
            
            // Test invalid modes
            gzopen(temp_filenames[0], "invalid");
            gzopen(temp_filenames[0], "r+b"); // + not allowed
            
            // Open a valid file for further error tests
            gz_files[0] = gzopen(temp_filenames[0], "rb");
            if (gz_files[0]) {
                coverage.files_opened++;
                
                // Test invalid operations - only test functions that handle NULL safely
                // gzread and gzwrite require valid buffers, so we don't test with NULL buffers
                // gzflush(NULL, Z_NO_FLUSH);
                // Note: gzgetc, gzungetc, gzputs, gzputc, gzgets, gzfread may be macros
                // and cause compilation errors with NULL, so we skip them
                gzseek(NULL, 0, SEEK_SET);
                gztell(NULL);
                gzeof(NULL);
                gzerror(NULL, NULL);
                gzrewind(NULL);
                gzbuffer(NULL, 1024);
                gzclearerr(NULL);
                gzoffset(NULL);
                gzdirect(NULL);
                
                // Test with zero-length operations (safe with valid buffer)
                char dummy[1];
                gzread(gz_files[0], dummy, 0);
                gzwrite(gz_files[0], dummy, 0);
                
                // Test invalid seek whence
                gzseek(gz_files[0], 0, 999);
                
                // Test error checking
                int errnum;
                coverage.error_check_tested++;
                const char* errmsg = gzerror(gz_files[0], &errnum);
                
                coverage.gzclearerr_tested++;
                gzclearerr(gz_files[0]);
            }
            break;
        }
        
        case 8: { // Complete API coverage test
            test_all_apis = true;
            // Fall through to comprehensive test
        }
        
        default: { // Comprehensive test combining all aspects
            // Test all opening modes
            const char* read_modes[] = {"rb", "rb1", "rb6", "rb9"};
            const char* write_modes[] = {"wb", "wb1", "wb6", "wb9", "wf", "wh", "wR", "wF"};
            const char* append_modes[] = {"ab", "ab1", "ab6", "ab9"};
            
            for (int i = 0; i < 2 && i < MAX_FILES; i++) {
                // Open in different modes
                const char* mode;
                if (i == 0) {
                    mode = read_modes[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(read_modes)/sizeof(read_modes[0]) - 1)];
                    coverage.read_mode_tested++;
                } else {
                    if (fdp.ConsumeBool()) {
                        mode = write_modes[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(write_modes)/sizeof(write_modes[0]) - 1)];
                        coverage.write_mode_tested++;
                    } else {
                        mode = append_modes[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(append_modes)/sizeof(append_modes[0]) - 1)];
                        coverage.append_mode_tested++;
                    }
                }
                
                gz_files[i] = gzopen(temp_filenames[i], mode);
                if (gz_files[i]) {
                    coverage.files_opened++;
                    
                    // Test buffer size
                    if (test_varied_buffers) {
                        gzbuffer(gz_files[i], buffer_size);
                        coverage.gzbuffer_tested++;
                    }
                    
                    // Perform appropriate operations
                    if (strchr(mode, 'r')) {
                        // Reading
                        char buf[2048];
                        int bytes = gzread(gz_files[i], buf, sizeof(buf));
                        coverage.read_operations++;
                        
                        if (bytes > 0) {
                            // Test character I/O
                            int ch = gzgetc(gz_files[i]);
                            coverage.gzgetc_ungetc_tested++;
                            if (ch != -1) {
                                gzungetc(ch, gz_files[i]);
                                coverage.gzgetc_ungetc_tested++;
                            }
                            
                            // Test line reading
                            char line[256];
                            gzgets(gz_files[i], line, sizeof(line));
                            coverage.gzputs_gets_tested++;
                            
                            // Test structured read
                            gzrewind(gz_files[i]);
                            coverage.gzrewind_tested++;
                            char fread_buf[1024];
                            gzfread(fread_buf, 1, sizeof(fread_buf), gz_files[i]);
                            coverage.gzfread_tested++;
                            
                            // Test seeking
                            gzseek(gz_files[i], 0, SEEK_SET);
                            coverage.seek_operations++;
                            
                            // Test position functions
                            gztell(gz_files[i]);
                            coverage.seek_operations++;
                            gzoffset(gz_files[i]);
                            coverage.gzoffset_tested++;
                            gzdirect(gz_files[i]);
                            coverage.gzdirect_tested++;
                        }
                        
                        // Test EOF
                        gzeof(gz_files[i]);
                        coverage.eof_tested++;
                        
                        // Test error functions
                        int errnum;
                        gzerror(gz_files[i], &errnum);
                        coverage.error_check_tested++;
                        gzclearerr(gz_files[i]);
                        coverage.gzclearerr_tested++;
                    } else {
                        // Writing/appending
                        if (!test_content.empty()) {
                            // Write in chunks
                            size_t written = 0;
                            while (written < test_content.size()) {
                                size_t chunk = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                                if (chunk > test_content.size() - written) {
                                    chunk = test_content.size() - written;
                                }
                                
                                gzwrite(gz_files[i], test_content.data() + written, chunk);
                                coverage.write_operations++;
                                written += chunk;
                                
                                // Occasionally flush
                                if (fdp.ConsumeBool()) {
                                    gzflush(gz_files[i], Z_SYNC_FLUSH);
                                    coverage.flush_operations++;
                                    coverage.gzflush_variants_tested++;
                                }
                            }
                            
                            // Test character and string writes
                            gzputc(gz_files[i], 'Z');
                            coverage.gzputs_gets_tested++;
                            gzputs(gz_files[i], "fuzz test");
                            coverage.gzputs_gets_tested++;
                            
                            // Final flush
                            gzflush(gz_files[i], Z_FINISH);
                            coverage.flush_operations++;
                        }
                    }
                }
            }
            break;
        }
    }
    
    // Clean up all opened files
    for (int i = 0; i < MAX_FILES; i++) {
        if (gz_files[i]) {
            gzclose(gz_files[i]);
            coverage.files_closed++;
        }
        unlink(temp_filenames[i]);
    }
    
    // Force compiler to keep coverage tracking (prevent optimization)
    volatile int total_ops = 
        coverage.files_opened + coverage.files_closed +
        coverage.read_operations + coverage.write_operations +
        coverage.seek_operations + coverage.flush_operations;
    (void)total_ops;
    
    return 0;
}

} // namespace harness_013
