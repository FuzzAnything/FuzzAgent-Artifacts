#include <fuzzer/FuzzedDataProvider.h>
#include <cctype>
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

namespace harness_001 {
/*
 * Fuzzing harness for libmagic library - Database Manipulation APIs
 * Targets: magic_load_buffers, magic_compile, magic_check, magic_list, magic_errno
 * Focuses on database manipulation, compilation, and validation operations
 * Uses FuzzedDataProvider to split input for different database operations
 */

// Include libmagic header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on database-related flags
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Consume operation type: 0 = magic_load_buffers, 1 = magic_compile, 2 = magic_check, 3 = magic_list
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {
            // Test magic_load_buffers - load database from in-memory buffers
            // First, determine how many buffers to create (1-4 buffers)
            size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(1, 4);
            
            // Allocate arrays for buffers and sizes
            void** buffers = (void**)malloc(num_buffers * sizeof(void*));
            size_t* sizes = (size_t*)malloc(num_buffers * sizeof(size_t));
            
            if (buffers == NULL || sizes == NULL) {
                if (buffers) free(buffers);
                if (sizes) free(sizes);
                magic_close(magic);
                return 0;
            }
            
            // Create buffers with consumed data
            for (size_t i = 0; i < num_buffers; i++) {
                // Consume random size for each buffer (1-256 bytes)
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                
                // Ensure we have enough remaining bytes
                if (fdp.remaining_bytes() < buffer_size) {
                    buffer_size = fdp.remaining_bytes();
                }
                
                if (buffer_size > 0) {
                    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
                    buffers[i] = malloc(buffer_data.size());
                    sizes[i] = buffer_data.size();
                    
                    if (buffers[i] != NULL) {
                        memcpy(buffers[i], buffer_data.data(), buffer_data.size());
                    } else {
                        sizes[i] = 0;
                    }
                } else {
                    buffers[i] = NULL;
                    sizes[i] = 0;
                }
            }
            
            // Try to load from buffers
            int load_result = magic_load_buffers(magic, buffers, sizes, num_buffers);
            (void)load_result; // Result can be -1 for invalid buffers, that's OK
            
            // Clean up buffers
            for (size_t i = 0; i < num_buffers; i++) {
                if (buffers[i] != NULL) {
                    free(buffers[i]);
                }
            }
            free(buffers);
            free(sizes);
            break;
        }
        
        case 1: {
            // Test magic_compile - compile a magic database file
            // Create a temporary file with magic rules
            char temp_filename[] = "/tmp/libmagic_compile_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write magic rules to the temp file using fuzzed data
                std::string magic_rules;
                
                // Consume bytes to create magic rules
                if (fdp.remaining_bytes() > 0) {
                    // Determine how many rules to create (1-5 rules)
                    size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    
                    for (size_t rule_idx = 0; rule_idx < num_rules && fdp.remaining_bytes() > 0; rule_idx++) {
                        // Consume offset (0-255)
                        uint8_t offset = fdp.ConsumeIntegral<uint8_t>();
                        magic_rules += std::to_string(offset) + " ";
                        
                        // Consume type: 0 = string, 1 = byte, 2 = lelong, 3 = belong
                        uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        
                        switch (rule_type) {
                            case 0: {
                                // string type
                                magic_rules += "string ";
                                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                if (fdp.remaining_bytes() < str_len) {
                                    str_len = fdp.remaining_bytes();
                                }
                                if (str_len > 0) {
                                    std::vector<uint8_t> str_data = fdp.ConsumeBytes<uint8_t>(str_len);
                                    magic_rules += "\"";
                                    for (size_t i = 0; i < str_data.size(); i++) {
                                        if (isprint(str_data[i]) && str_data[i] != '\\' && str_data[i] != '"') {
                                            magic_rules += static_cast<char>(str_data[i]);
                                        } else {
                                            magic_rules += '.';
                                        }
                                    }
                                    magic_rules += "\" ";
                                }
                                break;
                            }
                            case 1: {
                                // byte type with hex values
                                magic_rules += "byte ";
                                size_t byte_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                                if (fdp.remaining_bytes() < byte_count) {
                                    byte_count = fdp.remaining_bytes();
                                }
                                if (byte_count > 0) {
                                    std::vector<uint8_t> byte_data = fdp.ConsumeBytes<uint8_t>(byte_count);
                                    for (size_t i = 0; i < byte_data.size(); i++) {
                                        char hex[4];
                                        snprintf(hex, sizeof(hex), "%02x", byte_data[i]);
                                        magic_rules += hex;
                                        if (i < byte_data.size() - 1) magic_rules += " ";
                                    }
                                    magic_rules += " ";
                                }
                                break;
                            }
                            case 2: {
                                // lelong type
                                magic_rules += "lelong ";
                                uint32_t lelong_val = fdp.ConsumeIntegral<uint32_t>();
                                char hex_val[12];
                                snprintf(hex_val, sizeof(hex_val), "0x%08x ", lelong_val);
                                magic_rules += hex_val;
                                break;
                            }
                            case 3: {
                                // belong type  
                                magic_rules += "belong ";
                                uint32_t belong_val = fdp.ConsumeIntegral<uint32_t>();
                                char hex_val[12];
                                snprintf(hex_val, sizeof(hex_val), "0x%08x ", belong_val);
                                magic_rules += hex_val;
                                break;
                            }
                        }
                        
                        // Add description from fuzzed data
                        magic_rules += "test_rule_" + std::to_string(rule_idx) + "\n";
                    }
                } else {
                    // If no bytes left, create minimal rule
                    magic_rules = "0 string \"test\" test_rule\n";
                }
                
                if (write(fd, magic_rules.c_str(), magic_rules.size()) == (ssize_t)magic_rules.size()) {
                    close(fd);
                    
                    // Try to compile the magic file
                    int compile_result = magic_compile(magic, temp_filename);
                    (void)compile_result; // Result can be -1 for invalid rules, that's OK
                    
                    // Clean up temp file
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            }
            break;
        }
        
        case 2: {
            // Test magic_check - validate a magic database file
            // Create a temporary file with magic rules to check
            char temp_filename[] = "/tmp/libmagic_check_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                // Write some magic rules to the temp file
                std::string magic_rules;
                
                // Consume some bytes to create magic rules
                if (fdp.remaining_bytes() > 0) {
                    size_t rules_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                    std::vector<uint8_t> rules_data = fdp.ConsumeBytes<uint8_t>(rules_size);
                    
                    // Create simple magic rules from consumed data
                    magic_rules = "0 string ";
                    for (size_t i = 0; i < rules_data.size() && i < 32; i++) {
                        if (isprint(rules_data[i]) && rules_data[i] != '\\' && rules_data[i] != '"') {
                            magic_rules += static_cast<char>(rules_data[i]);
                        } else {
                            magic_rules += '.';
                        }
                    }
                    magic_rules += " test string\n";
                } else {
                    magic_rules = "0 string test test file\n";
                }
                
                if (write(fd, magic_rules.c_str(), magic_rules.size()) == (ssize_t)magic_rules.size()) {
                    close(fd);
                    
                    // Try to check/validate the magic file
                    int check_result = magic_check(magic, temp_filename);
                    (void)check_result; // Result can be -1 for invalid rules, that's OK
                    
                    // Clean up temp file
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            }
            break;
        }
        
        case 3: {
            // Test magic_list - list magic database entries
            // First try to load default database
            int load_result = magic_load(magic, NULL);
            
            if (load_result != -1) {
                // Create temporary output file for listing
                char temp_filename[] = "/tmp/libmagic_list_XXXXXX";
                int fd = mkstemp(temp_filename);
                if (fd >= 0) {
                    close(fd);
                    
                    // Try to list magic database to file
                    int list_result = magic_list(magic, temp_filename);
                    (void)list_result; // Result can be -1, that's OK
                    
                    // Clean up temp file
                    unlink(temp_filename);
                }
            }
            
            // Also test magic_errno API
            int errno_value = magic_errno(magic);
            (void)errno_value;
            break;
        }
    }
    
    // Additional database manipulation tests with remaining input
    
    // Test with a simple buffer operation to verify database was loaded/compiled
    if (fdp.remaining_bytes() > 0) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
        
        if (!buffer.empty()) {
            const char* result = magic_buffer(magic, buffer.data(), buffer.size());
            (void)result; // Result can be NULL, that's OK
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_001
