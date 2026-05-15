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

namespace harness_012 {
/*
 * Specialized fuzzing harness for libmagic targeting magic file management operations.
 * 
 * Primary targets (3 APIs with 11+ undiscovered branches):
 * 1. magic_compile - 0% coverage, 4 undiscovered branches  
 * 2. magic_check - 0% coverage, 4 undiscovered branches
 * 3. magic_list - 50% coverage, 3 undiscovered branches
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_list, magic_load, magic_close
 * 
 * Strategy:
 * 1. Generate dynamic magic database content from fuzzer input
 * 2. Test complete magic file lifecycle: compile -> check -> list
 * 3. Test with various magic file formats and edge cases
 * 4. Focus on semantic diversity from existing harnesses by:
 *    - Testing magic_list API more comprehensively
 *    - Using different file extensions (.magic vs .mgc)
 *    - Testing sequence variations of the three target APIs
 *    - Including magic_load for loaded database testing
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_011: Database compilation/verification with dynamic generation
 * - harness_009: Dynamic rule compilation for float/double rules
 * - harness_003: Custom magic database compilation with hardcoded values
 * - harness_012: Comprehensive magic file management operations with lifecycle testing (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + operation sequence + magic database content
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int operation_sequence = fdp.ConsumeIntegralInRange<int>(0, 5); // 0-5 different testing sequences
    bool use_compiled_extension = fdp.ConsumeBool();
    bool test_load_after_ops = fdp.ConsumeBool();
    bool test_multiple_files = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Generate magic database content from fuzzer input
    std::string magic_content;
    
    // Determine magic entry count
    int num_entries = fdp.ConsumeIntegralInRange<int>(1, 15);
    
    // Generate magic entries
    for (int i = 0; i < num_entries; i++) {
        if (fdp.remaining_bytes() < 16) break;
        
        // Generate offset
        uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 1024;
        
        // Generate data type
        uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 7;
        
        // Build magic entry
        std::string entry = std::to_string(offset) + " ";
        
        switch (data_type) {
            case 0:
                // string type
                entry += "string ";
                if (fdp.remaining_bytes() > 4) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                    if (fdp.remaining_bytes() >= str_len) {
                        std::string str_val = fdp.ConsumeBytesAsString(str_len);
                        // Sanitize string for magic format
                        std::string sanitized;
                        for (char c : str_val) {
                            if (c >= 32 && c <= 126 && c != '\\') sanitized += c;
                            else sanitized += '.';
                        }
                        entry += sanitized;
                    }
                }
                break;
                
            case 1:
                // byte type
                entry += "byte ";
                entry += "0x" + std::to_string(fdp.ConsumeIntegral<uint8_t>());
                break;
                
            case 2:
                // short type
                entry += "short ";
                entry += std::to_string(fdp.ConsumeIntegral<uint16_t>());
                break;
                
            case 3:
                // long type
                entry += "long ";
                entry += std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
                
            case 4:
                // regex type
                entry += "regex ";
                if (fdp.remaining_bytes() > 4) {
                    size_t regex_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    if (fdp.remaining_bytes() >= regex_len) {
                        std::string regex_val = fdp.ConsumeBytesAsString(regex_len);
                        // Basic regex sanitization
                        std::string sanitized;
                        for (char c : regex_val) {
                            if (c >= 32 && c <= 126) sanitized += c;
                            else sanitized += '.';
                        }
                        entry += sanitized;
                    }
                }
                break;
                
            case 5:
                // search type
                entry += "search/1 ";
                if (fdp.remaining_bytes() > 4) {
                    size_t search_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                    if (fdp.remaining_bytes() >= search_len) {
                        std::string search_val = fdp.ConsumeBytesAsString(search_len);
                        // Sanitize search value
                        std::string sanitized;
                        for (char c : search_val) {
                            if (c >= 32 && c <= 126) sanitized += c;
                            else sanitized += '.';
                        }
                        entry += sanitized;
                    }
                }
                break;
                
            case 6:
                // default type
                entry += "default ";
                entry += "x";
                break;
        }
        
        // Add message
        entry += " message \"Entry " + std::to_string(i) + " generated by fuzzer\"";
        
        magic_content += entry + "\n";
    }
    
    // If we have remaining bytes, add some comments and special directives
    if (fdp.remaining_bytes() > 32) {
        magic_content += "\n# Fuzzer-generated magic file\n";
        magic_content += "!:mime application/octet-stream\n";
        magic_content += "!:strength +" + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 100)) + "\n";
    }
    
    // Create temporary filename with appropriate extension
    std::string temp_filename;
    pid_t pid = getpid();
    
    if (use_compiled_extension) {
        temp_filename = "/tmp/fuzz_magic_" + std::to_string(pid) + ".mgc";
    } else {
        temp_filename = "/tmp/fuzz_magic_" + std::to_string(pid) + ".magic";
    }
    
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
    
    // Operation sequence testing
    switch (operation_sequence) {
        case 0: {
            // Sequence 0: compile -> check -> list
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            
            int list_result = magic_list(magic, temp_filename.c_str());
            (void)list_result;
            break;
        }
        
        case 1: {
            // Sequence 1: check -> compile -> list
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            
            int list_result = magic_list(magic, temp_filename.c_str());
            (void)list_result;
            break;
        }
        
        case 2: {
            // Sequence 2: list -> compile -> check
            int list_result = magic_list(magic, temp_filename.c_str());
            (void)list_result;
            
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            break;
        }
        
        case 3: {
            // Sequence 3: compile only (test compilation thoroughly)
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            
            // If compilation succeeded, try loading the compiled database
            if (test_load_after_ops && compile_result == 0) {
                // Create a new magic instance for loading
                magic_t magic2 = magic_open(actual_flags);
                if (magic2 != nullptr) {
                    int load_result = magic_load(magic2, temp_filename.c_str());
                    (void)load_result;
                    magic_close(magic2);
                }
            }
            break;
        }
        
        case 4: {
            // Sequence 4: check only (test validation thoroughly)
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            
            // Also test with different flags
            if (fdp.remaining_bytes() > sizeof(int)) {
                int alt_flags = fdp.ConsumeIntegral<int>() & 0xFFFFFFF;
                magic_t magic2 = magic_open(alt_flags);
                if (magic2 != nullptr) {
                    int check_result2 = magic_check(magic2, temp_filename.c_str());
                    (void)check_result2;
                    magic_close(magic2);
                }
            }
            break;
        }
        
        case 5: {
            // Sequence 5: list only (test listing thoroughly)
            int list_result = magic_list(magic, temp_filename.c_str());
            (void)list_result;
            
            // Test with multiple files if requested
            if (test_multiple_files && fdp.remaining_bytes() > 128) {
                // Create a second magic file
                std::string temp_filename2 = "/tmp/fuzz_magic2_" + std::to_string(pid) + 
                    (use_compiled_extension ? ".mgc" : ".magic");
                
                // Generate additional magic content
                std::string magic_content2;
                int num_entries2 = fdp.ConsumeIntegralInRange<int>(1, 5);
                
                for (int i = 0; i < num_entries2; i++) {
                    if (fdp.remaining_bytes() < 8) break;
                    
                    uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 512;
                    uint8_t type = fdp.ConsumeIntegral<uint8_t>() % 3;
                    
                    magic_content2 += std::to_string(offset) + " ";
                    
                    switch (type) {
                        case 0:
                            magic_content2 += "byte 0x" + std::to_string(fdp.ConsumeIntegral<uint8_t>());
                            break;
                        case 1:
                            magic_content2 += "short " + std::to_string(fdp.ConsumeIntegral<uint16_t>());
                            break;
                        case 2:
                            magic_content2 += "string ";
                            if (fdp.remaining_bytes() > 4) {
                                size_t len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                                if (fdp.remaining_bytes() >= len) {
                                    std::string val = fdp.ConsumeBytesAsString(len);
                                    // Sanitize
                                    std::string sanitized;
                                    for (char c : val) {
                                        if (c >= 32 && c <= 126) sanitized += c;
                                        else sanitized += '.';
                                    }
                                    magic_content2 += sanitized;
                                }
                            }
                            break;
                    }
                    
                    magic_content2 += " message \"Multi-file entry " + std::to_string(i) + "\"\n";
                }
                
                // Write second file
                FILE* fp2 = fopen(temp_filename2.c_str(), "w");
                if (fp2 != nullptr) {
                    fwrite(magic_content2.c_str(), 1, magic_content2.size(), fp2);
                    fclose(fp2);
                    
                    // Test list on second file
                    int list_result2 = magic_list(magic, temp_filename2.c_str());
                    (void)list_result2;
                    
                    unlink(temp_filename2.c_str());
                }
            }
            break;
        }
    }
    
    // Clean up
    unlink(temp_filename.c_str());
    magic_close(magic);
    
    return 0;
}

} // namespace harness_012
