#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_013 {
/*
 * Fuzzing harness for libmagic softmagic.c module targeting uncovered magic pattern types
 * Target: magiccheck() with 200+ blocked branches and mget() with 150+ blocked branches
 * 
 * Critical coverage gap functions (high complexity, low coverage):
 * - magiccheck(): Validates magic pattern matches with various data types and relations
 * - mget(): Extracts and processes magic patterns from buffer data
 * 
 * Strategy: Generate custom magic pattern files with diverse data types and relations
 * to exercise softmagic parsing logic through libmagic's magic_buffer API.
 * 
 * Focus on uncovered magic pattern types:
 * 1. FILE_BYTE, FILE_SHORT, FILE_LONG, FILE_QUAD types with various relations
 * 2. FILE_FLOAT, FILE_BEFLOAT, FILE_LEFLOAT floating point comparisons
 * 3. FILE_STRING, FILE_REGEX pattern matching
 * 4. FILE_INDIRECT, FILE_NAME indirect pattern references
 * 5. FILE_DER, FILE_GUID specialized types
 * 6. Various relations: '=', '!', '>', '<', 'x' (don't care)
 * 7. Endian variations: BE/LE versions of numeric types
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Magic pattern type definitions from libmagic internal headers
#define FILE_BYTE         1
#define FILE_SHORT        2
#define FILE_LONG         3
#define FILE_QUAD         4
#define FILE_FLOAT        5
#define FILE_BEFLOAT      6
#define FILE_LEFLOAT      7
#define FILE_STRING       8
#define FILE_REGEX        9
#define FILE_INDIRECT     10
#define FILE_NAME         11
#define FILE_USE          12
#define FILE_DER          13
#define FILE_GUID         14

#define FILE_BESHORT      15
#define FILE_LESHORT      16
#define FILE_BELONG       17
#define FILE_LELONG       18
#define FILE_BEQUAD       19
#define FILE_LEQUAD       20

// Helper to generate magic pattern line
std::string generate_magic_pattern(FuzzedDataProvider& fdp) {
    std::string pattern;
    
    // Generate offset (0-1024)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    pattern += std::to_string(offset);
    
    // Choose data type
    uint8_t type = fdp.ConsumeIntegralInRange<uint8_t>(1, 20);
    
    // Choose relation: '=', '!', '>', '<', 'x'
    char reln_chars[] = {'=', '!', '>', '<', 'x'};
    char reln = reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 4)];
    
    // Generate value based on type
    switch (type) {
        case FILE_BYTE:
            pattern += " byte";
            pattern += reln;
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint8_t>());
            break;
            
        case FILE_SHORT:
        case FILE_BESHORT:
        case FILE_LESHORT:
            pattern += " short";
            pattern += reln;
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint16_t>());
            break;
            
        case FILE_LONG:
        case FILE_BELONG:
        case FILE_LELONG:
            pattern += " long";
            pattern += reln;
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint32_t>());
            break;
            
        case FILE_QUAD:
        case FILE_BEQUAD:
        case FILE_LEQUAD:
            pattern += " quad";
            pattern += reln;
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint64_t>());
            break;
            
        case FILE_FLOAT:
        case FILE_BEFLOAT:
        case FILE_LEFLOAT:
            pattern += " float";
            pattern += reln;
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeFloatingPoint<float>());
            break;
            
        case FILE_STRING:
            pattern += " string";
            pattern += reln;
            pattern += ' ';
            {
                std::string str_val = fdp.ConsumeRandomLengthString(32);
                // Escape special characters
                for (char c : str_val) {
                    if (c == '\\' || c == '\"' || c == '\'') {
                        pattern += '\\';
                    }
                    pattern += c;
                }
            }
            break;
            
        case FILE_REGEX:
            pattern += " regex";
            pattern += reln;
            pattern += ' ';
            pattern += fdp.ConsumeRandomLengthString(32);
            break;
            
        case FILE_INDIRECT:
            pattern += " indirect";
            pattern += reln;
            pattern += ' ';
            pattern += std::to_string(fdp.ConsumeIntegral<uint32_t>());
            break;
            
        case FILE_NAME:
            pattern += " name";
            pattern += reln;
            pattern += ' ';
            pattern += fdp.ConsumeRandomLengthString(32);
            break;
            
        case FILE_USE:
            pattern += " use";
            pattern += reln;
            pattern += ' ';
            pattern += fdp.ConsumeRandomLengthString(32);
            break;
            
        case FILE_DER:
            pattern += " der";
            pattern += reln;
            pattern += ' ';
            pattern += fdp.ConsumeRandomLengthString(16);
            break;
            
        case FILE_GUID:
            pattern += " guid";
            pattern += reln;
            pattern += ' ';
            // Generate GUID-like string
            for (int i = 0; i < 32; i++) {
                pattern += "0123456789abcdef"[fdp.ConsumeIntegralInRange<uint8_t>(0, 15)];
                if (i == 7 || i == 11 || i == 15 || i == 19) pattern += '-';
            }
            break;
            
        default:
            pattern += " byte=x 0";
            break;
    }
    
    // Generate description
    pattern += " ";
    pattern += fdp.ConsumeRandomLengthString(64);
    
    return pattern;
}

// Helper to create temporary magic file
std::string create_magic_file(FuzzedDataProvider& fdp) {
    char magic_template[] = "/tmp/magic_fuzz_softmagic_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        return "";
    }
    
    // Generate multiple magic patterns
    std::string magic_content;
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 50);
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        magic_content += generate_magic_pattern(fdp);
        magic_content += "\n";
    }
    
    // Write to temporary file
    write(magic_fd, magic_content.c_str(), magic_content.length());
    fsync(magic_fd);
    close(magic_fd);
    
    return std::string(magic_template);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: magic_open with flags that affect softmagic processing
    int flags = 0;
    
    // Flags that influence softmagic behavior
    if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    
    // Flags to disable certain checks and focus on softmagic
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TAR;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_ELF;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_TEXT;
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_CDF;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Fallback to basic flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Test 2: Create and load custom magic database
    std::string magic_file_path = create_magic_file(fdp);
    if (!magic_file_path.empty()) {
        // Try to load the custom magic database
        if (magic_load(magic, magic_file_path.c_str()) == 0) {
            // Successfully loaded custom magic patterns
            
            // Test 3: magic_buffer with various fuzzed data
            // Generate test data that should trigger different magiccheck paths
            while (fdp.remaining_bytes() > 0) {
                size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                    std::min<size_t>(fdp.remaining_bytes(), 4096));
                
                if (test_size == 0) break;
                
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                
                // Test magic_buffer with the data
                const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                (void)result; // Result may be NULL, that's OK
                
                // Also test magic_file by writing to temp file
                char data_template[] = "/tmp/data_fuzz_XXXXXX";
                int data_fd = mkstemp(data_template);
                if (data_fd != -1) {
                    write(data_fd, test_data.data(), test_data.size());
                    close(data_fd);
                    
                    const char* file_result = magic_file(magic, data_template);
                    (void)file_result;
                    
                    unlink(data_template);
                }
            }
        }
        
        // Clean up magic file
        unlink(magic_file_path.c_str());
    }
    
    // Test 4: Even if custom magic load failed, test with built-in database
    // Load default magic database if not already loaded
    if (magic_load(magic, NULL) == 0) {
        // Test with remaining fuzzed data
        while (fdp.remaining_bytes() > 0) {
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                std::min<size_t>(fdp.remaining_bytes(), 1024));
            
            if (test_size == 0) break;
            
            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
            
            // Test magic_buffer
            const char* result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)result;
        }
    }
    
    // Test 5: Test parameter configuration that affects softmagic
    // Set parameters that influence magiccheck/mget behavior
    for (int param_id = 0; param_id < 10 && fdp.remaining_bytes() >= sizeof(size_t); param_id++) {
        size_t param_value = fdp.ConsumeIntegral<size_t>();
        magic_setparam(magic, param_id, &param_value);
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_013
