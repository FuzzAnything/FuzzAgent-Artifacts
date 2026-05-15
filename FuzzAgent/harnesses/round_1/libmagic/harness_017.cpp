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
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_017 {
/*
 * Specialized fuzzing harness for libmagic targeting completely uncovered 
 * magic database compilation and validation APIs with semantic diversity
 * from existing harnesses (000-016).
 * 
 * Primary targets (completely uncovered APIs with 0% coverage):
 * 1. magic_compile - 0% coverage, 4 undiscovered branches  
 * 2. magic_check - 0% coverage, 4 undiscovered branches
 * 3. magic_list - 50% coverage, 3 undiscovered branches
 * 
 * Strategy for semantic diversity from harnesses 011-016:
 * - harness_011: Dynamic magic database generation for compilation/verification
 * - harness_012: Magic file management operations with lifecycle testing  
 * - harness_013: Complex database structures targeting apprentice_sort
 * - harness_014: Varied magic syntax with getpath integration
 * - harness_015: Unknown (need to check)
 * - harness_016: Deep magic matching pipeline targeting ALL FILE_* types
 * 
 * NEW differentiation for harness_017:
 * 1. **Real system database testing**: Use actual system magic files alongside generated ones
 * 2. **Corrupted database testing**: Test error handling with intentionally corrupted magic files
 * 3. **Multi-format testing**: Test both text (.magic) and compiled (.mgc) formats
 * 4. **Extended magic_list testing**: More comprehensive testing of magic_list API
 * 5. **Edge case coverage**: Empty files, huge files, malformed syntax edge cases
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_list, magic_load, magic_close
 * 
 * Expected coverage gains:
 * - Complete API coverage (100% from current 88.89%)
 * - 8+ undiscovered branches in compilation logic
 * - New code paths in apprentice.c error handling
 * - magic_list API improvement from 50% to higher coverage
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + test strategy + magic content + corruption patterns
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int test_strategy = fdp.ConsumeIntegralInRange<int>(0, 6); // 0-6 different testing strategies
    bool use_system_db = fdp.ConsumeBool();
    bool test_corrupted_files = fdp.ConsumeBool();
    bool test_multiple_formats = fdp.ConsumeBool();
    bool extended_list_testing = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Strategy-based testing
    switch (test_strategy) {
        case 0: {
            // Strategy 0: System database compilation and validation
            // Use actual system magic files if available
            if (use_system_db) {
                // Try multiple system magic file locations
                const char* system_paths[] = {
                    "/usr/share/misc/magic",
                    "/usr/share/file/magic",
                    "/usr/lib/file/magic",
                    "magic"  // Local file
                };
                
                for (size_t i = 0; i < sizeof(system_paths)/sizeof(system_paths[0]); i++) {
                    // Check if file exists and is readable
                    if (access(system_paths[i], R_OK) == 0) {
                        // Test magic_compile with system file
                        int compile_result = magic_compile(magic, system_paths[i]);
                        (void)compile_result;
                        
                        // Test magic_check with system file
                        int check_result = magic_check(magic, system_paths[i]);
                        (void)check_result;
                        
                        // Test magic_list with system file
                        if (extended_list_testing) {
                            int list_result = magic_list(magic, system_paths[i]);
                            (void)list_result;
                        }
                        break;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 1: Generated magic database with varied complexity
            std::string magic_content;
            
            // Determine magic entry count
            int num_entries = fdp.ConsumeIntegralInRange<int>(1, 20);
            
            // Generate magic entries with diverse types
            for (int i = 0; i < num_entries; i++) {
                if (fdp.remaining_bytes() < 32) break;
                
                // Generate offset
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 1024;
                
                // Generate data type
                uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 10;
                
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
                                // Sanitize for magic format
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
                        entry += std::to_string(fdp.ConsumeIntegral<uint8_t>());
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
                        // quad type (8-byte)
                        entry += "quad ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint64_t>());
                        break;
                    case 5:
                        // float type
                        entry += "float ";
                        entry += std::to_string(fdp.ConsumeFloatingPoint<float>());
                        break;
                    case 6:
                        // double type
                        entry += "double ";
                        entry += std::to_string(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 7:
                        // beshort (big-endian short)
                        entry += "beshort ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint16_t>());
                        break;
                    case 8:
                        // belong (big-endian long)
                        entry += "belong ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint32_t>());
                        break;
                    case 9:
                        // bequad (big-endian quad)
                        entry += "bequad ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint64_t>());
                        break;
                }
                
                // Add optional relation
                if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
                    uint8_t rel_type = fdp.ConsumeIntegral<uint8_t>() % 7;
                    const char* relations[] = {"=", "!=", ">", "<", "&", "^", "~"};
                    entry.insert(entry.find_last_of(' ') + 1, std::string(relations[rel_type]) + " ");
                }
                
                // Add optional message
                if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
                    entry += " \"Test entry " + std::to_string(i) + "\"";
                }
                
                magic_content += entry + "\n";
            }
            
            // Write to temporary file
            pid_t pid = getpid();
            std::string temp_filename = "/tmp/fuzz_magic_gen_" + std::to_string(pid) + 
                                       (test_multiple_formats ? ".magic" : ".mgc");
            
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_content.c_str(), 1, magic_content.size(), fp);
                fclose(fp);
                
                // Test all target APIs
                int compile_result = magic_compile(magic, temp_filename.c_str());
                (void)compile_result;
                
                int check_result = magic_check(magic, temp_filename.c_str());
                (void)check_result;
                
                if (extended_list_testing) {
                    int list_result = magic_list(magic, temp_filename.c_str());
                    (void)list_result;
                }
                
                // Clean up
                unlink(temp_filename.c_str());
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Corrupted magic file testing
            // Create intentionally corrupted magic files to test error handling
            pid_t pid = getpid();
            std::string corrupt_filename = "/tmp/fuzz_magic_corrupt_" + std::to_string(pid) + ".magic";
            
            // Generate corruption patterns
            std::string corrupt_content;
            int corruption_type = fdp.ConsumeIntegralInRange<int>(0, 5);
            
            switch (corruption_type) {
                case 0:
                    // Empty file
                    corrupt_content = "";
                    break;
                case 1:
                    // File with only invalid characters
                    if (fdp.remaining_bytes() > 0) {
                        size_t junk_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                        if (fdp.remaining_bytes() >= junk_len) {
                            corrupt_content = fdp.ConsumeBytesAsString(junk_len);
                        }
                    }
                    break;
                case 2:
                    // File with invalid offset syntax
                    corrupt_content = "invalid offset string test\n";
                    break;
                case 3:
                    // File with missing type specifier
                    corrupt_content = "0 \n";
                    break;
                case 4:
                    // File with valid syntax but invalid values
                    corrupt_content = "0 byte 999999999999999999999999\n";
                    break;
                case 5:
                    // File with unterminated string
                    corrupt_content = "0 string \"unterminated\n";
                    break;
            }
            
            FILE* fp = fopen(corrupt_filename.c_str(), "w");
            if (fp) {
                fwrite(corrupt_content.c_str(), 1, corrupt_content.size(), fp);
                fclose(fp);
                
                // Test error handling paths
                int compile_result = magic_compile(magic, corrupt_filename.c_str());
                (void)compile_result;
                
                int check_result = magic_check(magic, corrupt_filename.c_str());
                (void)check_result;
                
                if (extended_list_testing) {
                    int list_result = magic_list(magic, corrupt_filename.c_str());
                    (void)list_result;
                }
                
                // Clean up
                unlink(corrupt_filename.c_str());
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Edge case testing with special files
            pid_t pid = getpid();
            
            // Test 1: Empty file
            std::string empty_filename = "/tmp/fuzz_magic_empty_" + std::to_string(pid) + ".magic";
            FILE* fp1 = fopen(empty_filename.c_str(), "w");
            if (fp1) {
                fclose(fp1);
                
                int compile_result = magic_compile(magic, empty_filename.c_str());
                (void)compile_result;
                
                int check_result = magic_check(magic, empty_filename.c_str());
                (void)check_result;
                
                unlink(empty_filename.c_str());
            }
            
            // Test 2: Very large offset (edge case)
            if (fdp.remaining_bytes() > 0) {
                std::string large_offset_filename = "/tmp/fuzz_magic_large_" + std::to_string(pid) + ".magic";
                std::string large_content = "999999999 byte 0xFF\n";
                
                FILE* fp2 = fopen(large_offset_filename.c_str(), "w");
                if (fp2) {
                    fwrite(large_content.c_str(), 1, large_content.size(), fp2);
                    fclose(fp2);
                    
                    int compile_result = magic_compile(magic, large_offset_filename.c_str());
                    (void)compile_result;
                    
                    unlink(large_offset_filename.c_str());
                }
            }
            break;
        }
        
        case 4: {
            // Strategy 4: Format conversion testing (.magic -> .mgc)
            if (fdp.remaining_bytes() > 32) {
                // Create source .magic file
                pid_t pid = getpid();
                std::string source_filename = "/tmp/fuzz_magic_source_" + std::to_string(pid) + ".magic";
                std::string compiled_filename = "/tmp/fuzz_magic_compiled_" + std::to_string(pid) + ".mgc";
                
                // Simple valid magic content
                std::string magic_content = "0 string test Test file\n";
                magic_content += ">0 byte 0x00 continuation\n";
                
                FILE* fp = fopen(source_filename.c_str(), "w");
                if (fp) {
                    fwrite(magic_content.c_str(), 1, magic_content.size(), fp);
                    fclose(fp);
                    
                    // Compile to .mgc format
                    int compile_result = magic_compile(magic, source_filename.c_str());
                    (void)compile_result;
                    
                    // Check both formats
                    int check_source = magic_check(magic, source_filename.c_str());
                    (void)check_source;
                    
                    // Note: Compiled .mgc file would be created by magic_compile
                    // We can try to check it if it exists
                    if (access(compiled_filename.c_str(), R_OK) == 0) {
                        int check_compiled = magic_check(magic, compiled_filename.c_str());
                        (void)check_compiled;
                    }
                    
                    // Clean up
                    unlink(source_filename.c_str());
                    if (access(compiled_filename.c_str(), R_OK) == 0) {
                        unlink(compiled_filename.c_str());
                    }
                }
            }
            break;
        }
        
        case 5: {
            // Strategy 5: Extended magic_list testing
            // Create multiple magic files and test listing functionality
            pid_t pid = getpid();
            
            // Create first magic file
            std::string file1 = "/tmp/fuzz_magic_list1_" + std::to_string(pid) + ".magic";
            std::string content1 = "0 string AAA First file\n";
            
            FILE* fp1 = fopen(file1.c_str(), "w");
            if (fp1) {
                fwrite(content1.c_str(), 1, content1.size(), fp1);
                fclose(fp1);
                
                // Test magic_list with first file
                int list_result1 = magic_list(magic, file1.c_str());
                (void)list_result1;
                
                // Create second magic file
                if (fdp.remaining_bytes() > 0) {
                    std::string file2 = "/tmp/fuzz_magic_list2_" + std::to_string(pid) + ".magic";
                    std::string content2 = "0 string BBB Second file\n";
                    
                    FILE* fp2 = fopen(file2.c_str(), "w");
                    if (fp2) {
                        fwrite(content2.c_str(), 1, content2.size(), fp2);
                        fclose(fp2);
                        
                        // Test magic_list with second file
                        int list_result2 = magic_list(magic, file2.c_str());
                        (void)list_result2;
                        
                        unlink(file2.c_str());
                    }
                }
                
                unlink(file1.c_str());
            }
            break;
        }
        
        case 6: {
            // Strategy 6: Mixed valid and invalid content
            if (fdp.remaining_bytes() > 64) {
                pid_t pid = getpid();
                std::string mixed_filename = "/tmp/fuzz_magic_mixed_" + std::to_string(pid) + ".magic";
                
                // Create mixed valid/invalid content
                std::string mixed_content;
                
                // Start with valid entry
                mixed_content += "0 string valid Valid entry\n";
                
                // Add some invalid content
                mixed_content += "invalid line here\n";
                
                // Add another valid entry
                mixed_content += "10 byte 0x55 Another valid\n";
                
                // Add malformed entry
                mixed_content += "20 \n";  // Missing type
                
                FILE* fp = fopen(mixed_filename.c_str(), "w");
                if (fp) {
                    fwrite(mixed_content.c_str(), 1, mixed_content.size(), fp);
                    fclose(fp);
                    
                    // Test compilation (should handle errors)
                    int compile_result = magic_compile(magic, mixed_filename.c_str());
                    (void)compile_result;
                    
                    // Test checking
                    int check_result = magic_check(magic, mixed_filename.c_str());
                    (void)check_result;
                    
                    unlink(mixed_filename.c_str());
                }
            }
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_017
