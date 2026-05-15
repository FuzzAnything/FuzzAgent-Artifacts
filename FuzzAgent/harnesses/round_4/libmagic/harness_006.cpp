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

namespace harness_006 {
/*
 * Fuzzing harness for libmagic library - Magic Rule Syntax Parsing and Compilation
 * Targets: magic_compile, magic_check, and internal parse_* functions (parse_ext, parse_extra, 
 *          parse_apple, parse_mime, parse_strength) via magic rule syntax generation
 * Goal: Exercise magic database compilation backend and apprentice.c parsing functions
 * Strategy: Generate diverse magic rule syntax patterns to test compilation and validation
 * Differentiated from harness_001: Focuses on comprehensive rule syntax, not just basic compilation
 * Differentiated from other harnesses: Targets apprentice.c parsing functions (0% coverage)
 * Invocation sequence: magic_open -> magic_load/magic_load_buffers -> magic_compile -> magic_check -> magic_close
 */


// Include libmagic header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful rule generation
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on compilation-related flags
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging for compilation errors
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;          // Important for compilation warnings
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Strategy: 0 = Create temp file with magic rules, 1 = Use magic_load_buffers
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    int load_result = -1;
    std::string temp_filename;
    
    if (strategy == 0) {
        // Strategy 1: Create temporary magic file with diverse rule syntax
        char temp_filename_buffer[] = "/tmp/libmagic_parse_XXXXXX";
        int fd = mkstemp(temp_filename_buffer);
        if (fd < 0) {
            magic_close(magic);
            return 0;
        }
        temp_filename = temp_filename_buffer;
        
        // Generate diverse magic rule syntax to exercise parse_* functions
        std::string magic_rules;
        
        // Generate multiple magic rules (1-10 rules)
        size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(1, 10);
        
        for (size_t rule_idx = 0; rule_idx < num_rules && fdp.remaining_bytes() > 0; rule_idx++) {
            // Consume rule type to target different parse_* functions
            uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 10;
            
            // Start with offset
            uint16_t offset = fdp.ConsumeIntegral<uint16_t>();
            magic_rules += std::to_string(offset % 1024) + "\t";
            
            switch (rule_type) {
                case 0: { // Test parse_ext - extension rules
                    magic_rules += "0\tstring\t";
                    // Generate extension string
                    size_t ext_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    if (fdp.remaining_bytes() < ext_len) {
                        ext_len = fdp.remaining_bytes();
                    }
                    if (ext_len > 0) {
                        std::vector<uint8_t> ext_data = fdp.ConsumeBytes<uint8_t>(ext_len);
                        magic_rules += "\"";
                        for (size_t i = 0; i < ext_data.size(); i++) {
                            if (isalnum(ext_data[i]) || ext_data[i] == '.' || ext_data[i] == '_' || ext_data[i] == '-') {
                                magic_rules += static_cast<char>(ext_data[i]);
                            } else {
                                magic_rules += 'x';
                            }
                        }
                        magic_rules += "\"\t";
                    }
                    magic_rules += "ext\t";
                    // Extension value
                    magic_rules += "\"";
                    size_t ext_val_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                    if (fdp.remaining_bytes() < ext_val_len) {
                        ext_val_len = fdp.remaining_bytes();
                    }
                    if (ext_val_len > 0) {
                        std::vector<uint8_t> ext_val_data = fdp.ConsumeBytes<uint8_t>(ext_val_len);
                        for (size_t i = 0; i < ext_val_data.size(); i++) {
                            if (isalnum(ext_val_data[i]) || ext_val_data[i] == '.' || ext_val_data[i] == '_' || ext_val_data[i] == '-') {
                                magic_rules += static_cast<char>(ext_val_data[i]);
                            } else {
                                magic_rules += 'x';
                            }
                        }
                    }
                    magic_rules += "\"\n";
                    break;
                }
                    
                case 1: { // Test parse_apple - apple creator/type rules
                    magic_rules += "0\tstring\t";
                    // Generate pattern
                    size_t apple_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                    if (fdp.remaining_bytes() < apple_len) {
                        apple_len = fdp.remaining_bytes();
                    }
                    if (apple_len > 0) {
                        std::vector<uint8_t> apple_data = fdp.ConsumeBytes<uint8_t>(apple_len);
                        magic_rules += "\"";
                        for (size_t i = 0; i < apple_data.size(); i++) {
                            magic_rules += static_cast<char>(apple_data[i] % 256);
                        }
                        magic_rules += "\"\t";
                    }
                    magic_rules += "apple\t";
                    // Apple creator/type
                    magic_rules += "\"";
                    for (int i = 0; i < 4; i++) {
                        if (fdp.remaining_bytes() > 0) {
                            uint8_t c = fdp.ConsumeIntegral<uint8_t>();
                            if (isprint(c) && c != '\\' && c != '"') {
                                magic_rules += static_cast<char>(c);
                            } else {
                                magic_rules += 'A' + (c % 26);
                            }
                        } else {
                            magic_rules += 'X';
                        }
                    }
                    magic_rules += "\"\n";
                    break;
                }
                    
                case 2: { // Test parse_mime - MIME type rules
                    magic_rules += "0\tstring\t";
                    // Generate pattern
                    size_t mime_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                    if (fdp.remaining_bytes() < mime_len) {
                        mime_len = fdp.remaining_bytes();
                    }
                    if (mime_len > 0) {
                        std::vector<uint8_t> mime_data = fdp.ConsumeBytes<uint8_t>(mime_len);
                        magic_rules += "\"";
                        for (size_t i = 0; i < mime_data.size(); i++) {
                            magic_rules += static_cast<char>(mime_data[i] % 256);
                        }
                        magic_rules += "\"\t";
                    }
                    magic_rules += "mime\t";
                    // MIME type
                    magic_rules += "\"";
                    std::string mime_types[] = {"text/plain", "application/octet-stream", "image/jpeg", 
                                                "application/pdf", "text/html", "application/xml"};
                    uint8_t mime_idx = fdp.ConsumeIntegral<uint8_t>() % 6;
                    magic_rules += mime_types[mime_idx];
                    magic_rules += "\"\n";
                    break;
                }
                    
                case 3: { // Test parse_strength - strength rules
                    magic_rules += "0\tstring\t";
                    // Generate pattern
                    size_t strength_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                    if (fdp.remaining_bytes() < strength_len) {
                        strength_len = fdp.remaining_bytes();
                    }
                    if (strength_len > 0) {
                        std::vector<uint8_t> strength_data = fdp.ConsumeBytes<uint8_t>(strength_len);
                        magic_rules += "\"";
                        for (size_t i = 0; i < strength_data.size(); i++) {
                            magic_rules += static_cast<char>(strength_data[i] % 256);
                        }
                        magic_rules += "\"\t";
                    }
                    magic_rules += "strength\t";
                    // Strength value (0-255)
                    uint8_t strength_val = fdp.ConsumeIntegral<uint8_t>();
                    magic_rules += std::to_string(strength_val) + "\n";
                    break;
                }
                    
                case 4: { // Test parse_extra - standard magic rule with message
                    magic_rules += "0\t";
                    // Data type
                    uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                    const char* type_names[] = {"byte", "short", "long", "string", "date", "beshort", "belong", "lelong"};
                    magic_rules += type_names[data_type];
                    magic_rules += "\t";
                    
                    // Value/pattern
                    if (data_type == 3) { // string
                        magic_rules += "\"";
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        if (fdp.remaining_bytes() < str_len) {
                            str_len = fdp.remaining_bytes();
                        }
                        if (str_len > 0) {
                            std::vector<uint8_t> str_data = fdp.ConsumeBytes<uint8_t>(str_len);
                            for (size_t i = 0; i < str_data.size(); i++) {
                                if (isprint(str_data[i]) && str_data[i] != '\\' && str_data[i] != '"') {
                                    magic_rules += static_cast<char>(str_data[i]);
                                } else {
                                    magic_rules += '.';
                                }
                            }
                        }
                        magic_rules += "\"\t";
                    } else { // numeric
                        uint32_t num_val = fdp.ConsumeIntegral<uint32_t>();
                        char hex_buf[12];
                        snprintf(hex_buf, sizeof(hex_buf), "0x%08x", num_val);
                        magic_rules += hex_buf;
                        magic_rules += "\t";
                    }
                    
                    // Message
                    magic_rules += "\"Rule ";
                    magic_rules += std::to_string(rule_idx);
                    magic_rules += " generated by fuzzer\"\n";
                    break;
                }
                    
                default: { // Simple rules for other cases
                    magic_rules += "0\tstring\t";
                    size_t simple_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                    if (fdp.remaining_bytes() < simple_len) {
                        simple_len = fdp.remaining_bytes();
                    }
                    if (simple_len > 0) {
                        std::vector<uint8_t> simple_data = fdp.ConsumeBytes<uint8_t>(simple_len);
                        magic_rules += "\"";
                        for (size_t i = 0; i < simple_data.size(); i++) {
                            magic_rules += static_cast<char>(simple_data[i] % 256);
                        }
                        magic_rules += "\"\t";
                    }
                    magic_rules += "\"Test rule\"\n";
                    break;
                }
            }
        }
        
        // Write rules to temp file
        if (write(fd, magic_rules.c_str(), magic_rules.size()) != (ssize_t)magic_rules.size()) {
            close(fd);
            unlink(temp_filename.c_str());
            magic_close(magic);
            return 0;
        }
        close(fd);
        
        // Load magic database from temp file
        load_result = magic_load(magic, temp_filename.c_str());
        
    } else {
        // Strategy 2: Use magic_load_buffers with raw rule data
        // Create 1-3 buffers with magic rule data
        size_t num_buffers = fdp.ConsumeIntegralInRange<size_t>(1, 3);
        
        void** buffers = (void**)malloc(num_buffers * sizeof(void*));
        size_t* sizes = (size_t*)malloc(num_buffers * sizeof(size_t));
        
        if (buffers == NULL || sizes == NULL) {
            if (buffers) free(buffers);
            if (sizes) free(sizes);
            magic_close(magic);
            return 0;
        }
        
        // Create buffers with magic rule syntax
        for (size_t i = 0; i < num_buffers; i++) {
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, 512);
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
        load_result = magic_load_buffers(magic, buffers, sizes, num_buffers);
        
        // Clean up buffers
        for (size_t i = 0; i < num_buffers; i++) {
            if (buffers[i] != NULL) {
                free(buffers[i]);
            }
        }
        free(buffers);
        free(sizes);
    }
    
    // If loading succeeded, test compilation and validation
    if (load_result != -1) {
        // Test magic_compile - compile the loaded database
        int compile_result = magic_compile(magic, NULL); // NULL means compile current database
        (void)compile_result; // Result can be -1, that's OK for fuzzing
        
        // Test magic_check - validate the compiled database
        int check_result = magic_check(magic, NULL); // NULL means check current database
        (void)check_result; // Result can be -1, that's OK for fuzzing
    }
    
    // Clean up temp file if created
    if (!temp_filename.empty()) {
        unlink(temp_filename.c_str());
    }
    
    // Close magic handle
    magic_close(magic);
    
    return 0;
}

} // namespace harness_006
