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

namespace harness_015 {
/*
 * Fuzzing harness for libmagic softmagic.c targeting magiccheck function
 * with 204 blocked branches - focusing on unexercised float/double comparison 
 * paths with 'x' relation and invalid relation cases.
 * 
 * Coverage guidance indicates:
 * 1. FILE_FLOAT with 'x' relation (don't care) - 0 hits (lines 2196-2198)
 * 2. FILE_DOUBLE with 'x' relation - 0 hits (lines 2229-2231)
 * 3. Default/invalid relation cases for float/double comparisons - 0 hits
 *    (lines 2216-2219 for float, 2249-2252 for double)
 * 
 * Strategy: Generate custom magic patterns with float/double types and
 * various relations including 'x' and invalid characters to trigger
 * the uncovered default/invalid relation error paths.
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Helper to generate magic pattern targeting uncovered float/double paths
std::string generate_float_double_pattern(FuzzedDataProvider& fdp, bool include_invalid_reln) {
    std::string pattern;
    
    // Generate offset (0-1024)
    uint32_t offset = fdp.ConsumeIntegralInRange<uint32_t>(0, 1024);
    pattern += std::to_string(offset);
    
    // Choose float or double type
    bool is_float = fdp.ConsumeBool();
    
    if (is_float) {
        pattern += " float";
    } else {
        pattern += " double";
    }
    
    // Choose relation character
    char reln;
    if (include_invalid_reln && fdp.ConsumeBool()) {
        // Generate invalid relation character (not '=', '!', '>', '<', 'x')
        // Use characters from printable ASCII range that are not valid relations
        const char invalid_chars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'y', 'z',
                                     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                                     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                     '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
                                     '[', ']', '{', '}', '|', '\\', ':', ';', '"', '\'', ',', '.', '/', '?'};
        reln = invalid_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, sizeof(invalid_chars) - 1)];
    } else {
        // Choose valid relation, including 'x' which is uncovered
        const char valid_reln_chars[] = {'=', '!', '>', '<', 'x'};
        reln = valid_reln_chars[fdp.ConsumeIntegralInRange<uint8_t>(0, 4)];
    }
    
    pattern += reln;
    pattern += ' ';
    
    // Generate floating point value
    if (is_float) {
        float value = fdp.ConsumeFloatingPoint<float>();
        pattern += std::to_string(value);
    } else {
        double value = fdp.ConsumeFloatingPoint<double>();
        pattern += std::to_string(value);
    }
    
    return pattern;
}

// Helper to create temporary magic file with targeted patterns
std::string create_targeted_magic_file(FuzzedDataProvider& fdp) {
    char magic_template[] = "/tmp/magic_targeted_float_double_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        return "";
    }
    
    std::string magic_content;
    
    // Generate multiple magic pattern lines
    int num_patterns = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < num_patterns && fdp.remaining_bytes() > 10; i++) {
        // Sometimes include invalid relations to trigger error paths
        bool include_invalid = fdp.ConsumeBool();
        std::string pattern = generate_float_double_pattern(fdp, include_invalid);
        
        // Add description
        pattern += " Float/Double test pattern ";
        pattern += std::to_string(i);
        pattern += "\n";
        
        magic_content += pattern;
    }
    
    // Write to temporary file
    write(magic_fd, magic_content.c_str(), magic_content.size());
    fsync(magic_fd);
    close(magic_fd);
    
    return std::string(magic_template);
}

// Helper to generate test data with embedded float/double values
std::vector<uint8_t> generate_test_data_with_floats(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> test_data;
    
    // Start with some random bytes
    size_t base_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::vector<uint8_t> base_data = fdp.ConsumeBytes<uint8_t>(base_size);
    test_data.insert(test_data.end(), base_data.begin(), base_data.end());
    
    // Embed float values at various offsets
    int num_floats = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_floats && fdp.remaining_bytes() >= sizeof(float); i++) {
        size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, test_data.size());
        float value = fdp.ConsumeFloatingPoint<float>();
        
        // Ensure we have enough space
        if (test_data.size() < offset + sizeof(float)) {
            test_data.resize(offset + sizeof(float), 0);
        }
        
        // Copy float bytes
        uint8_t* fbytes = reinterpret_cast<uint8_t*>(&value);
        for (size_t j = 0; j < sizeof(float); j++) {
            test_data[offset + j] = fbytes[j];
        }
    }
    
    // Embed double values at various offsets
    int num_doubles = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_doubles && fdp.remaining_bytes() >= sizeof(double); i++) {
        size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, test_data.size());
        double value = fdp.ConsumeFloatingPoint<double>();
        
        // Ensure we have enough space
        if (test_data.size() < offset + sizeof(double)) {
            test_data.resize(offset + sizeof(double), 0);
        }
        
        // Copy double bytes
        uint8_t* dbytes = reinterpret_cast<uint8_t*>(&value);
        for (size_t j = 0; j < sizeof(double); j++) {
            test_data[offset + j] = dbytes[j];
        }
    }
    
    return test_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 20) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Open magic context
    int flags = 0;
    if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Try with default flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Create targeted magic database with float/double patterns
    std::string magic_file_path = create_targeted_magic_file(fdp);
    if (!magic_file_path.empty()) {
        // Load the targeted magic database
        if (magic_load(magic, magic_file_path.c_str()) == 0) {
            // Test with generated data containing float/double values
            int test_iterations = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < test_iterations && fdp.remaining_bytes() > 50; i++) {
                // Generate test data with embedded float/double values
                std::vector<uint8_t> test_data = generate_test_data_with_floats(fdp);
                
                if (!test_data.empty()) {
                    // Test magic_buffer
                    const char* result = magic_buffer(magic, test_data.data(), test_data.size());
                    (void)result; // Result may be NULL
                    
                    // Also test magic_file for file-based paths
                    if (fdp.ConsumeBool()) {
                        char data_template[] = "/tmp/test_data_float_double_XXXXXX";
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
            }
        }
        
        // Clean up magic file
        unlink(magic_file_path.c_str());
    }
    
    // Also test with default database using remaining fuzzed data
    if (magic_load(magic, NULL) == 0) {
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
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_015
