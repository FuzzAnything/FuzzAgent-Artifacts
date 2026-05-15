#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

namespace harness_013 {
/*
 * Specialized fuzzing harness for libmagic targeting the apprentice subsystem
 * with focus on magic database compilation, checking, and listing operations
 * that exercise the apprentice_sort function (389 undiscovered branches).
 * 
 * Primary targets:
 * 1. magic_compile - 0% coverage, 4 undiscovered branches  
 * 2. magic_check - 0% coverage, 4 undiscovered branches
 * 3. magic_list - 50% coverage, 3 undiscovered branches
 * 4. apprentice_sort - 389 undiscovered branches (called during database compilation)
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_list, magic_load, magic_close
 * 
 * Strategy:
 * 1. Generate complex magic database structures from fuzzer input to trigger apprentice_sort
 * 2. Create databases with multiple continuation levels (nested patterns)
 * 3. Vary magic entry types, offsets, and data to create diverse sorting scenarios
 * 4. Test all three target APIs with the same complex database
 * 5. Focus on semantic diversity by targeting apprentice subsystem internals
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_003: Custom magic database compilation with hardcoded float/double values
 * - harness_012: Magic file management operations with lifecycle testing
 * - harness_013: Complex database structures targeting apprentice_sort and deep apprentice subsystem (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + database complexity + entry count + entry data
    if (size < 512) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int db_complexity = fdp.ConsumeIntegralInRange<int>(0, 4); // 0-4 different complexity levels
    bool test_compile = fdp.ConsumeBool();
    bool test_check = fdp.ConsumeBool();
    bool test_list = fdp.ConsumeBool();
    bool use_nested_continuations = fdp.ConsumeBool();
    bool vary_entry_types = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Generate magic database content with varying complexity
    std::string magic_content;
    
    // Determine magic entry count based on complexity
    int num_entries = 0;
    switch (db_complexity) {
        case 0: num_entries = fdp.ConsumeIntegralInRange<int>(5, 15); break;
        case 1: num_entries = fdp.ConsumeIntegralInRange<int>(15, 30); break;
        case 2: num_entries = fdp.ConsumeIntegralInRange<int>(30, 50); break;
        case 3: num_entries = fdp.ConsumeIntegralInRange<int>(50, 100); break;
        case 4: num_entries = fdp.ConsumeIntegralInRange<int>(100, 200); break;
    }
    
    // Track continuation levels for nested structures
    std::vector<int> continuation_levels;
    int current_level = 0;
    
    // Generate magic entries with diverse patterns
    for (int i = 0; i < num_entries; i++) {
        if (fdp.remaining_bytes() < 32) break;
        
        // Determine if we should change continuation level
        if (use_nested_continuations && i > 0 && fdp.ConsumeBool()) {
            if (fdp.ConsumeBool() && current_level < 3) {
                // Increase continuation level
                current_level++;
                continuation_levels.push_back(current_level);
            } else if (current_level > 0) {
                // Decrease continuation level
                current_level--;
                continuation_levels.push_back(current_level);
            }
        }
        
        // Generate offset (increases to create overlapping patterns for sorting)
        uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 1024;
        
        // Add continuation prefix if needed
        std::string entry;
        if (current_level > 0) {
            for (int l = 0; l < current_level; l++) {
                entry += ">";
            }
            entry += " ";
        }
        
        entry += std::to_string(offset) + " ";
        
        // Choose data type based on variance flag
        uint8_t data_type;
        if (vary_entry_types) {
            data_type = fdp.ConsumeIntegral<uint8_t>() % 12;
        } else {
            data_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        }
        
        // Build magic entry with different data types
        switch (data_type) {
            case 0: {
                // byte type with comparison
                entry += "byte ";
                uint8_t byte_val = fdp.ConsumeIntegral<uint8_t>();
                char rel = fdp.ConsumeIntegralInRange<char>('=', '>');
                if (rel != '=') {
                    entry += rel;
                    entry += " ";
                }
                entry += std::to_string(static_cast<int>(byte_val));
                entry += " test_byte";
                break;
            }
            case 1: {
                // short type (little endian)
                entry += "leshort ";
                uint16_t short_val = fdp.ConsumeIntegral<uint16_t>();
                char rel = fdp.ConsumeIntegralInRange<char>('=', '>');
                if (rel != '=') {
                    entry += rel;
                    entry += " ";
                }
                entry += std::to_string(short_val);
                entry += " test_short";
                break;
            }
            case 2: {
                // long type (big endian)
                entry += "belong ";
                uint32_t long_val = fdp.ConsumeIntegral<uint32_t>();
                char rel = fdp.ConsumeIntegralInRange<char>('=', '>');
                if (rel != '=') {
                    entry += rel;
                    entry += " ";
                }
                entry += std::to_string(long_val);
                entry += " test_long";
                break;
            }
            case 3: {
                // string type
                entry += "string ";
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    // Sanitize for magic format
                    std::string sanitized;
                    for (char c : str_val) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '\n') sanitized += c;
                        else sanitized += '.';
                    }
                    entry += sanitized;
                } else {
                    entry += "test_string";
                }
                break;
            }
            case 4: {
                // float type
                entry += "float ";
                float float_val = fdp.ConsumeFloatingPoint<float>();
                char rel = fdp.ConsumeIntegralInRange<char>('=', '>');
                if (rel != '=') {
                    entry += rel;
                    entry += " ";
                }
                entry += std::to_string(float_val);
                entry += " test_float";
                break;
            }
            case 5: {
                // double type
                entry += "double ";
                double double_val = fdp.ConsumeFloatingPoint<double>();
                char rel = fdp.ConsumeIntegralInRange<char>('=', '>');
                if (rel != '=') {
                    entry += rel;
                    entry += " ";
                }
                entry += std::to_string(double_val);
                entry += " test_double";
                break;
            }
            case 6: {
                // regex type
                entry += "regex ";
                size_t regex_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= regex_len) {
                    std::string regex_val = fdp.ConsumeBytesAsString(regex_len);
                    // Simple regex sanitization
                    std::string sanitized;
                    for (char c : regex_val) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '\n') sanitized += c;
                        else if (c == '\n') sanitized += "\\n";
                        else sanitized += '.';
                    }
                    entry += sanitized;
                } else {
                    entry += "test.*regex";
                }
                break;
            }
            case 7: {
                // search type with offset
                entry += "search/";
                uint16_t search_len = fdp.ConsumeIntegral<uint16_t>() % 16 + 1;
                entry += std::to_string(search_len) + " ";
                if (fdp.remaining_bytes() >= search_len) {
                    std::string search_val = fdp.ConsumeBytesAsString(search_len);
                    std::string sanitized;
                    for (char c : search_val) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '\n') sanitized += c;
                        else sanitized += '.';
                    }
                    entry += sanitized;
                } else {
                    entry += "test_search";
                }
                break;
            }
            case 8: {
                // indirect type (references another offset)
                entry += "indirect ";
                uint16_t indir_offset = fdp.ConsumeIntegral<uint16_t>() % 512;
                entry += std::to_string(indir_offset);
                entry += " test_indirect";
                break;
            }
            case 9: {
                // name type
                entry += "name ";
                size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= name_len) {
                    std::string name_val = fdp.ConsumeBytesAsString(name_len);
                    std::string sanitized;
                    for (char c : name_val) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '\n') sanitized += c;
                        else sanitized += '.';
                    }
                    entry += sanitized;
                } else {
                    entry += "test_name";
                }
                break;
            }
            case 10: {
                // use type (magic from another file)
                entry += "use ";
                size_t use_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= use_len) {
                    std::string use_val = fdp.ConsumeBytesAsString(use_len);
                    std::string sanitized;
                    for (char c : use_val) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '\n' && c != '/') sanitized += c;
                        else sanitized += '.';
                    }
                    entry += sanitized;
                } else {
                    entry += "test_use";
                }
                break;
            }
            case 11: {
                // default type (catch-all)
                entry += "default";
                entry += " test_default";
                break;
            }
        }
        
        // Add MIME type sometimes
        if (fdp.ConsumeBool()) {
            entry += " ";
            entry += fdp.ConsumeBool() ? "application/octet-stream" : "text/plain";
        }
        
        magic_content += entry + "\n";
    }
    
    // Create temporary file for magic database
    pid_t pid = getpid();
    std::string temp_filename = "/tmp/fuzz_magic_" + std::to_string(pid) + ".magic";
    
    // Write magic content to temporary file
    FILE* fp = fopen(temp_filename.c_str(), "w");
    if (fp == nullptr) {
        magic_close(magic);
        return 0;
    }
    
    size_t written = fwrite(magic_content.c_str(), 1, magic_content.size(), fp);
    fclose(fp);
    
    if (written != magic_content.size()) {
        unlink(temp_filename.c_str());
        magic_close(magic);
        return 0;
    }
    
    // Test the three target APIs based on flags
    int compile_result = -1;
    int check_result = -1;
    int list_result = -1;
    
    if (test_check) {
        // Test magic_check first (syntax validation)
        check_result = magic_check(magic, temp_filename.c_str());
        (void)check_result;
    }
    
    if (test_compile) {
        // Test magic_compile (database compilation)
        compile_result = magic_compile(magic, temp_filename.c_str());
        (void)compile_result;
        
        // If compilation succeeded, try to load and use it
        if (compile_result == 0) {
            // Create a new magic instance for testing the compiled database
            magic_t magic2 = magic_open(actual_flags);
            if (magic2 != nullptr) {
                // Try to load the compiled database
                int load_result = magic_load(magic2, temp_filename.c_str());
                (void)load_result;
                
                // Test with some random data if we have input left
                if (fdp.remaining_bytes() > 0) {
                    size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    if (fdp.remaining_bytes() >= test_size) {
                        std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                        const char* buffer_result = magic_buffer(magic2, test_data.data(), test_data.size());
                        (void)buffer_result;
                    }
                }
                
                magic_close(magic2);
            }
        }
    }
    
    if (test_list) {
        // Test magic_list (database listing)
        list_result = magic_list(magic, temp_filename.c_str());
        (void)list_result;
        
        // Also test listing with NULL filename
        int list_null_result = magic_list(magic, NULL);
        (void)list_null_result;
    }
    
    // Test with different flags to trigger different code paths
    if (fdp.remaining_bytes() > sizeof(int)) {
        int alt_flags = fdp.ConsumeIntegral<int>() & 0xFFFFFFF;
        magic_t alt_magic = magic_open(alt_flags);
        if (alt_magic != nullptr) {
            // Test one of the APIs with alternative flags
            if (test_check) {
                int alt_check = magic_check(alt_magic, temp_filename.c_str());
                (void)alt_check;
            }
            
            // Test magic_list with different flags
            if (test_list) {
                int alt_list = magic_list(alt_magic, temp_filename.c_str());
                (void)alt_list;
            }
            
            magic_close(alt_magic);
        }
    }
    
    // Clean up temporary file
    unlink(temp_filename.c_str());
    
    // Clean up magic instance
    magic_close(magic);
    
    return 0;
}

} // namespace harness_013
