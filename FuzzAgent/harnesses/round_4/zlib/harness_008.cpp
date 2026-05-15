#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_008 {
/* Targeted fuzzing harness for zlib gzip file stream reading operations
 * Primary target: gzgetc_ (completely uncovered, 251 undiscovered branches)
 * Secondary targets: gzgetc, gzread, gzgets, gzfread, gzungetc, gzclearerr, gzdirect
 * Focuses on breaking through plateau state by covering the remaining major uncovered API
 * Targets gzread.c module which has lowest coverage (62.68%)
 * 
 * Implementation sequence:
 * 1. gzopen - Open a gzip file/stream for reading
 * 2. gzread / gzgets / gzfread - Perform bulk reads
 * 3. gzgetc / gzgetc_ - Single character reads
 * 4. gzungetc - Push back characters
 * 5. gzdirect - Check if stream is in direct mode
 * 6. gzclearerr - Clear error indicators
 * 7. gzerror - Get error information
 * 8. gzclose - Close stream and free resources
 */


// Minimum input size requirement for meaningful testing
const size_t MIN_INPUT_SIZE = 32;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_BUFFER_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_STRING_LENGTH = 1024;       // 1KB for strings

// Helper function to create a temporary file with fuzzer data
static int create_temp_file(const uint8_t* data, size_t size, char* temp_path) {
    // Use tmpnam for simplicity in fuzzing context (not for production)
    const char* tmp_dir = getenv("TMPDIR");
    if (!tmp_dir) tmp_dir = "/tmp";
    
    snprintf(temp_path, 256, "%s/zlib_fuzz_XXXXXX", tmp_dir);
    int fd = mkstemp(temp_path);
    if (fd < 0) return -1;
    
    // Write data to temp file
    ssize_t written = write(fd, data, size);
    if (written != (ssize_t)size) {
        close(fd);
        unlink(temp_path);
        return -1;
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Check minimum input size
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary file with the first part of fuzzer data
    // We'll use this as the gzip file to read from
    size_t file_data_size = fdp.ConsumeIntegralInRange<size_t>(16, size / 2);
    std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_data_size);
    
    char temp_path[256];
    if (create_temp_file(file_data.data(), file_data.size(), temp_path) != 0) {
        return 0;  // Failed to create temp file
    }
    
    // Open the file with gzopen for reading
    const char* mode = "rb";  // Read binary mode
    gzFile file = gzopen(temp_path, mode);
    if (file == NULL) {
        unlink(temp_path);
        return 0;  // Failed to open file
    }
    
    // Consume parameters for various read operations
    int read_size = fdp.ConsumeIntegralInRange<int>(1, 1024);
    int string_len = fdp.ConsumeIntegralInRange<int>(1, MAX_STRING_LENGTH);
    int num_items = fdp.ConsumeIntegralInRange<int>(1, 100);
    int item_size = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Test bulk read operations
    
    // Test gzread - read raw bytes
    std::vector<char> buffer(read_size);
    int bytes_read = gzread(file, buffer.data(), read_size);
    // bytes_read could be -1 for error, 0 for EOF, or positive for bytes read
    
    // Test gzfread - structured reading with size_t parameters
    if (bytes_read > 0) {
        // We need to handle the read pointer position, but for fuzzing we'll
        // just test the API call patterns
        std::vector<char> fread_buffer(num_items * item_size);
        z_size_t items_read = gzfread(fread_buffer.data(), item_size, num_items, file);
        // items_read is number of items successfully read
    }
    
    // Test gzgets - read a line
    std::vector<char> line_buffer(string_len + 1);  // +1 for null terminator
    char* line_result = gzgets(file, line_buffer.data(), string_len);
    // line_result is NULL on error/EOF, otherwise pointer to buffer
    
    // Test single character reads
    
    // Test gzgetc - read single character
    int char1 = gzgetc(file);
    // char1 is character read or -1 on error/EOF
    
    // PRIMARY TARGET: Test gzgetc_ - the completely uncovered API
    int char2 = gzgetc_(file);
    // gzgetc_ is backward compatibility wrapper for gzgetc
    
    // Test gzungetc - push back a character if we read one
    if (char1 != -1) {
        int unget_result = gzungetc(char1, file);
        // unget_result is the character pushed back or -1 on error
    }
    
    // Test gzdirect - check if stream is in direct mode
    int direct_mode = gzdirect(file);
    // direct_mode is 1 if transparent (direct), 0 if not
    
    // Test error handling APIs
    
    // Test gzerror - get error information
    const char* error_msg = gzerror(file, NULL);
    // error_msg is error string or "out of memory" if allocation fails
    
    // Test gzclearerr - clear error indicators
    gzclearerr(file);
    
    // Consume remaining data to test various error conditions
    // by trying to read beyond what might be available
    if (fdp.remaining_bytes() < 100) {
        // Try to read more data to potentially trigger errors
        std::vector<char> extra_buffer(1024);
        gzread(file, extra_buffer.data(), 1024);
        
        // Check error state again
        gzerror(file, NULL);
        
        // Clear errors again
        gzclearerr(file);
    }
    
    // Close the file
    gzclose(file);
    
    // Clean up temporary file
    unlink(temp_path);
    
    return 0;
}

} // namespace harness_008
