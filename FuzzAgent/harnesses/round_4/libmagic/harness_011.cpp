#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "magic.h"

namespace harness_011 {
/*
 * Fuzzing harness for libmagic library - Uncovered API Functions
 * Targets: magic_getpath, magic_descriptor, magic_error, magic_version, 
 *          magic_load_buffers, magic_compile, magic_check, magic_list, 
 *          magic_errno, magic_setparam, magic_getparam
 * Focus: Parameter manipulation, error handling, version queries, 
 *        specialized loading/compilation functions, MAGIC_NO_CHECK_* flags
 * Semantic differentiation: While existing harnesses test basic file/buffer operations,
 *        this harness specifically targets uncovered APIs and parameter manipulation
 *        with diverse parameter combinations and error scenarios.
 * Strategy: Use FuzzedDataProvider to generate diverse parameter combinations for 
 *        magic_setparam/magic_getparam with different parameter types, test 
 *        magic_load_buffers with synthetic magic database buffers, test magic_compile 
 *        and magic_check with malformed magic patterns.
 */

// Include libmagic header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test magic_version() - simple version query
    int version = magic_version();
    (void)version; // Avoid unused variable warning
    
    // Test magic_getpath() - get magic database path
    // Consume a path suffix string
    std::string path_suffix = fdp.ConsumeRandomLengthString(32);
    int action = fdp.ConsumeIntegral<int>() % 3;
    const char* magic_path = magic_getpath(path_suffix.c_str(), action);
    (void)magic_path; // Result can be NULL, that's OK
    
    // Consume flags for magic_open - include MAGIC_NO_CHECK_* flags
    int flags = 0;
    
    // Consume two bytes to determine which flags to set
    uint16_t flag_word = fdp.ConsumeIntegral<uint16_t>();
    
    // Standard flags
    if (flag_word & 0x0001) flags |= MAGIC_DEBUG;
    if (flag_word & 0x0002) flags |= MAGIC_SYMLINK;
    if (flag_word & 0x0004) flags |= MAGIC_COMPRESS;
    if (flag_word & 0x0008) flags |= MAGIC_DEVICES;
    if (flag_word & 0x0010) flags |= MAGIC_MIME_TYPE;
    if (flag_word & 0x0020) flags |= MAGIC_CONTINUE;
    if (flag_word & 0x0040) flags |= MAGIC_CHECK;
    if (flag_word & 0x0080) flags |= MAGIC_RAW;
    if (flag_word & 0x0100) flags |= MAGIC_ERROR;
    if (flag_word & 0x0200) flags |= MAGIC_MIME_ENCODING;
    if (flag_word & 0x0400) flags |= MAGIC_APPLE;
    if (flag_word & 0x0800) flags |= MAGIC_EXTENSION;
    if (flag_word & 0x1000) flags |= MAGIC_COMPRESS_TRANSP;
    if (flag_word & 0x2000) flags |= MAGIC_NO_COMPRESS_FORK;
    
    // MAGIC_NO_CHECK_* flags - target these specifically
    if (flag_word & 0x0001000) flags |= MAGIC_NO_CHECK_COMPRESS;
    if (flag_word & 0x0002000) flags |= MAGIC_NO_CHECK_TAR;
    if (flag_word & 0x0004000) flags |= MAGIC_NO_CHECK_SOFT;
    if (flag_word & 0x0008000) flags |= MAGIC_NO_CHECK_APPTYPE;
    if (flag_word & 0x0010000) flags |= MAGIC_NO_CHECK_ELF;
    if (flag_word & 0x0020000) flags |= MAGIC_NO_CHECK_TEXT;
    if (flag_word & 0x0040000) flags |= MAGIC_NO_CHECK_CDF;
    if (flag_word & 0x0080000) flags |= MAGIC_NO_CHECK_CSV;
    if (flag_word & 0x0100000) flags |= MAGIC_NO_CHECK_TOKENS;
    if (flag_word & 0x0200000) flags |= MAGIC_NO_CHECK_ENCODING;
    if (flag_word & 0x0400000) flags |= MAGIC_NO_CHECK_JSON;
    if (flag_word & 0x0800000) flags |= MAGIC_NO_CHECK_SIMH;
    
    // Open magic handle with comprehensive flags
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Consume operation type for different uncovered APIs
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (operation) {
        case 0: {
            // Test magic_descriptor with file descriptor
            // Create a temporary file
            char temp_filename[] = "/tmp/libmagic_fuzz_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write some data to the temp file
                size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
                
                if (write(fd, file_data.data(), file_data.size()) == (ssize_t)file_data.size()) {
                    // Test magic_descriptor
                    const char* result = magic_descriptor(magic, fd);
                    (void)result; // Result can be NULL, that's OK
                    
                    // Test magic_errno after operation
                    int errno_val = magic_errno(magic);
                    (void)errno_val;
                }
                
                close(fd);
                unlink(temp_filename);
            }
            break;
        }
        
        case 1: {
            // Test magic_setparam and magic_getparam with different parameter IDs
            // Consume parameter ID (0-9 based on MAGIC_PARAM_* defines)
            int param_id = fdp.ConsumeIntegralInRange<int>(0, 9);
            
            // Consume parameter value - use size_t for proper type alignment
            size_t param_value = fdp.ConsumeIntegral<size_t>();
            
            // Try to set parameter - use size_t* for proper type casting
            int set_result = -1;
            if (param_id == MAGIC_PARAM_INDIR_MAX || param_id == MAGIC_PARAM_NAME_MAX ||
                param_id == MAGIC_PARAM_ELF_PHNUM_MAX || param_id == MAGIC_PARAM_ELF_SHNUM_MAX ||
                param_id == MAGIC_PARAM_ELF_NOTES_MAX || param_id == MAGIC_PARAM_REGEX_MAX ||
                param_id == MAGIC_PARAM_BYTES_MAX || param_id == MAGIC_PARAM_ENCODING_MAX ||
                param_id == MAGIC_PARAM_ELF_SHSIZE_MAX || param_id == MAGIC_PARAM_MAGWARN_MAX) {
                set_result = magic_setparam(magic, param_id, &param_value);
            }
            (void)set_result; // Result can be -1, that's OK
            
            // Try to get parameter - use size_t for proper type casting
            size_t get_value = 0;
            int get_result = magic_getparam(magic, param_id, &get_value);
            (void)get_result; // Result can be -1, that's OK
            
            break;
        }
        
        case 2: {
            // Test magic_load_buffers with synthetic magic database buffers
            // Create 1-3 synthetic magic buffers
            size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(1, 3);
            
            // Allocate arrays for buffers and sizes
            void** buffers = (void**)malloc(num_buffers * sizeof(void*));
            size_t* sizes = (size_t*)malloc(num_buffers * sizeof(size_t));
            
            if (buffers && sizes) {
                for (size_t i = 0; i < num_buffers; i++) {
                    // Create synthetic magic pattern
                    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(16, 128);
                    if (fdp.remaining_bytes() < buffer_size) {
                        buffer_size = fdp.remaining_bytes();
                    }
                    
                    if (buffer_size > 0) {
                        std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
                        
                        // Create a simple magic pattern header
                        const char* magic_header = "# Magic database file - malformed pattern\n0\tstring\tFUZZ";
                        size_t header_len = strlen(magic_header);
                        size_t total_size = buffer_data.size() + header_len + 1; // +1 for safety/null terminator
                        buffers[i] = malloc(total_size);
                        sizes[i] = total_size;
                        
                        if (buffers[i]) {
                            // Add magic pattern header
                            memcpy(buffers[i], magic_header, header_len);
                            memcpy((char*)buffers[i] + header_len, buffer_data.data(), buffer_data.size());
                            // Ensure we don't write beyond allocated buffer
                            // The last byte remains zero (from malloc)
                        } else {
                            sizes[i] = 0;
                        }
                    } else {
                        buffers[i] = NULL;
                        sizes[i] = 0;
                    }
                }
                
                // Try to load from synthetic buffers
                int load_result = magic_load_buffers(magic, buffers, sizes, num_buffers);
                (void)load_result;
                
                // Clean up
                for (size_t i = 0; i < num_buffers; i++) {
                    if (buffers[i]) free(buffers[i]);
                }
                free(buffers);
                free(sizes);
            } else {
                if (buffers) free(buffers);
                if (sizes) free(sizes);
            }
            break;
        }
        
        case 3: {
            // Test magic_compile with malformed magic patterns
            // Create a temporary file with malformed magic patterns
            char temp_filename[] = "/tmp/magic_compile_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write malformed magic patterns
                std::string malformed_patterns = fdp.ConsumeRandomLengthString(256);
                
                // Add some intentionally malformed patterns
                malformed_patterns += "\n0\tinvalid\tpattern\tmalformed\n";
                malformed_patterns += "0\tstring\tFUZZ\t>>\tinvalid\n";
                malformed_patterns += "0\tlelong\t0x12345678\t=\tinvalid\tcorrupted\n";
                
                write(fd, malformed_patterns.c_str(), malformed_patterns.size());
                close(fd);
                
                // Try to compile the malformed patterns
                int compile_result = magic_compile(magic, temp_filename);
                (void)compile_result;
                
                // Test magic_error after compilation attempt
                const char* error_msg = magic_error(magic);
                (void)error_msg;
                
                unlink(temp_filename);
            }
            break;
        }
        
        case 4: {
            // Test magic_check with potentially valid/invalid magic files
            // Create a temporary file with mixed valid/invalid patterns
            char temp_filename[] = "/tmp/magic_check_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write mixed patterns
                std::string mixed_patterns = "# Magic database test\n";
                mixed_patterns += "0\tstring\tFUZZ\tfuzz data\n";
                mixed_patterns += "0\tlelong\t0x";
                
                // Add hex pattern from fuzzer input
                size_t hex_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                std::string hex_data = fdp.ConsumeBytesAsString(hex_len);
                for (char c : hex_data) {
                    if (isxdigit(c)) mixed_patterns += c;
                }
                
                mixed_patterns += "\t=\t0x12345678\ttest pattern\n";
                mixed_patterns += "0\tinvalid\tmalformed\tpattern\n"; // Invalid pattern
                
                write(fd, mixed_patterns.c_str(), mixed_patterns.size());
                close(fd);
                
                // Check the magic file
                int check_result = magic_check(magic, temp_filename);
                (void)check_result;
                
                unlink(temp_filename);
            }
            break;
        }
        
        case 5: {
            // Test magic_list to output magic database
            char temp_filename[] = "/tmp/magic_list_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                close(fd);
                
                // Try to list magic database to file
                int list_result = magic_list(magic, temp_filename);
                (void)list_result;
                
                unlink(temp_filename);
            }
            break;
        }
        
        case 6: {
            // Test error handling scenarios
            // Create magic handle with error-prone flags
            int error_flags = MAGIC_ERROR; // Handle ENOENT etc as real errors
            
            // Try operations that might fail
            const char* error_result = magic_error(magic);
            (void)error_result;
            
            // Test with NULL magic handle in some operations
            // (simulated by trying operations that might return errors)
            int errno_val = magic_errno(magic);
            (void)errno_val;
            
            break;
        }
        
        case 7: {
            // Test combination of multiple operations
            // First load database if possible
            const char* default_path = NULL;
            magic_load(magic, default_path);
            
            // Test various operations in sequence
            const char* error_msg = magic_error(magic);
            (void)error_msg;
            
            int errno_val = magic_errno(magic);
            (void)errno_val;
            
            // Try parameter operations
            size_t param_value = 100;
            magic_setparam(magic, MAGIC_PARAM_BYTES_MAX, &param_value);
            
            size_t get_value = 0;
            magic_getparam(magic, MAGIC_PARAM_BYTES_MAX, &get_value);
            
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_011
