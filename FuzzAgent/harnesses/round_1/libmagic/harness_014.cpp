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

namespace harness_014 {
/*
 * Specialized fuzzing harness for libmagic targeting magic database compilation
 * and validation operations to unlock apprentice.c and compress.c code paths.
 * 
 * Primary targets (completely uncovered public APIs with high downstream complexity):
 * 1. magic_compile - 0% coverage, 4 undiscovered branches  
 * 2. magic_check - 0% coverage, 4 undiscovered branches
 * 
 * Secondary targets (blocked downstream functions):
 * - apprentice.c: apprentice_sort, parse_extra, parse_apple, parse_ext, parse_mime, 
 *   parse_strength, goodchar (all 0% covered)
 * - compress.c: decompression routines (zlib, bzlib, xz, zstd, lzlib integration)
 * 
 * APIs targeted:
 * - magic_open, magic_compile, magic_check, magic_getpath, magic_load, magic_close
 * 
 * Strategy:
 * 1. Generate diverse magic database content with varied syntax and complexity
 * 2. Test compilation and validation workflows with different file extensions
 * 3. Explore flag combinations that affect database processing
 * 4. Test with system database paths via magic_getpath
 * 5. Focus on semantic diversity from existing harnesses by:
 *    - Testing more varied magic rule syntax combinations
 *    - Exploring different file extension behaviors (.magic vs .mgc)
 *    - Testing magic_getpath integration
 *    - Targeting compress.c integration points
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_011: Database compilation/verification with dynamic generation
 * - harness_012: Magic file management operations with lifecycle testing
 * - harness_013: Complex database structures targeting apprentice_sort
 * - harness_014: Varied magic syntax with getpath integration and compression targeting (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + strategy + magic database content + test data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 5); // 0-5 different testing strategies
    bool use_compiled_ext = fdp.ConsumeBool();
    bool test_getpath = fdp.ConsumeBool();
    bool test_with_loaded_db = fdp.ConsumeBool();
    bool use_complex_syntax = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Test magic_getpath if requested (optional helper)
    if (test_getpath && fdp.remaining_bytes() > 4) {
        const char* dbname = use_compiled_ext ? "magic.mgc" : "magic";
        int action = fdp.ConsumeIntegralInRange<int>(0, 2);
        const char* path_result = magic_getpath(dbname, action);
        (void)path_result; // Use result to avoid unused variable warning
    }

    // Generate magic database content with varied syntax
    std::string magic_content;
    
    // Determine number of magic entries
    int num_entries = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    // Array of possible magic types for diverse testing
    const char* magic_types[] = {
        "string", "byte", "short", "long", "quad", "float", "double",
        "beshort", "belong", "bequad", "bedouble", "bestring",
        "leshort", "lelong", "lequad", "ledouble", "lestring"
    };
    
    // Array of possible relations for more diverse syntax
    const char* relations[] = {"", "=", "!=", ">", "<", "&", "^", "~"};
    
    // Array of possible message formats
    const char* message_prefixes[] = {"", "message \"", "message '", ""};
    
    for (int i = 0; i < num_entries; i++) {
        if (fdp.remaining_bytes() < 8) break;
        
        // Generate offset
        uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 1024;
        
        // Choose magic type
        uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(magic_types)/sizeof(magic_types[0]));
        std::string type = magic_types[type_idx];
        
        // Build magic entry
        std::string entry = std::to_string(offset) + " " + type + " ";
        
        // Add value based on type
        if (type == "string" || type == "bestring" || type == "lestring") {
            // String value
            if (fdp.remaining_bytes() > 4) {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() >= str_len) {
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    // Sanitize for magic format
                    std::string sanitized;
                    for (char c : str_val) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '"' && c != '\'') {
                            sanitized += c;
                        } else {
                            sanitized += '.';
                        }
                    }
                    entry += sanitized;
                }
            }
        } else if (type == "byte" || type == "ubyte") {
            // Byte value
            entry += "0x" + std::to_string(fdp.ConsumeIntegral<uint8_t>());
        } else if (type == "short" || type == "ushort" || type == "beshort" || type == "leshort") {
            // Short value
            entry += std::to_string(fdp.ConsumeIntegral<uint16_t>());
        } else if (type == "long" || type == "ulong" || type == "belong" || type == "lelong") {
            // Long value
            entry += std::to_string(fdp.ConsumeIntegral<uint32_t>());
        } else if (type == "quad" || type == "uquad" || type == "bequad" || type == "lequad") {
            // Quad value (use 64-bit if available, otherwise 32-bit)
            if (fdp.remaining_bytes() >= 8) {
                entry += std::to_string(fdp.ConsumeIntegral<uint64_t>());
            } else {
                entry += std::to_string(fdp.ConsumeIntegral<uint32_t>());
            }
        } else if (type == "float" || type == "bedouble" || type == "ledouble") {
            // Floating point value
            float float_val = fdp.ConsumeFloatingPoint<float>();
            entry += std::to_string(float_val);
        } else if (type == "double") {
            // Double value
            double double_val = fdp.ConsumeFloatingPoint<double>();
            entry += std::to_string(double_val);
        }
        
        // Add relation if using complex syntax
        if (use_complex_syntax && fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            uint8_t rel_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(relations)/sizeof(relations[0]));
            if (strlen(relations[rel_idx]) > 0) {
                entry.insert(entry.find_last_of(' ') + 1, std::string(relations[rel_idx]) + " ");
            }
        }
        
        // Add message with varied format
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            uint8_t msg_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(message_prefixes)/sizeof(message_prefixes[0]));
            std::string msg_prefix = message_prefixes[msg_idx];
            std::string msg_suffix = (msg_idx < 2) ? "\"" : ((msg_idx == 2) ? "'" : "");
            
            std::string message = " " + msg_prefix + "Entry " + std::to_string(i);
            if (fdp.remaining_bytes() > 4 && fdp.ConsumeBool()) {
                size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                if (fdp.remaining_bytes() >= extra_len) {
                    std::string extra = fdp.ConsumeBytesAsString(extra_len);
                    // Sanitize extra text
                    for (char c : extra) {
                        if (c >= 32 && c <= 126 && c != '\\' && c != '"' && c != '\'') {
                            message += c;
                        }
                    }
                }
            }
            message += msg_suffix;
            entry += message;
        }
        
        magic_content += entry + "\n";
    }
    
    // Add a continuation entry if enough bytes and complex syntax requested
    if (use_complex_syntax && fdp.remaining_bytes() > 16 && fdp.ConsumeBool()) {
        magic_content += ">0 byte 0x00 continuation test\n";
    }
    
    // Create temporary filename with appropriate extension
    pid_t pid = getpid();
    std::string temp_filename;
    if (use_compiled_ext) {
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
    
    // Strategy-based testing
    switch (strategy) {
        case 0: {
            // Basic compilation and validation
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            break;
        }
        
        case 1: {
            // Validation then compilation (reverse order)
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            break;
        }
        
        case 2: {
            // Multiple compilation attempts with different flags
            int compile_result1 = magic_compile(magic, temp_filename.c_str());
            (void)compile_result1;
            
            // Change flags and try again
            if (fdp.remaining_bytes() > sizeof(int)) {
                int new_flags = fdp.ConsumeIntegral<int>() & 0xFFFFFFF;
                magic_t magic2 = magic_open(new_flags);
                if (magic2 != nullptr) {
                    int compile_result2 = magic_compile(magic2, temp_filename.c_str());
                    (void)compile_result2;
                    
                    int check_result2 = magic_check(magic2, temp_filename.c_str());
                    (void)check_result2;
                    
                    magic_close(magic2);
                }
            }
            break;
        }
        
        case 3: {
            // Test with loaded database after compilation
            int compile_result = magic_compile(magic, temp_filename.c_str());
            (void)compile_result;
            
            if (test_with_loaded_db && compile_result == 0) {
                // Create new instance for loading
                magic_t magic2 = magic_open(actual_flags);
                if (magic2 != nullptr) {
                    int load_result = magic_load(magic2, temp_filename.c_str());
                    (void)load_result;
                    
                    // Test with some sample data if loaded successfully
                    if (load_result == 0 && fdp.remaining_bytes() > 16) {
                        size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() >= test_size) {
                            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                            const char* result = magic_buffer(magic2, test_data.data(), test_size);
                            (void)result;
                        }
                    }
                    
                    magic_close(magic2);
                }
            }
            break;
        }
        
        case 4: {
            // Test validation thoroughly with different operations
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            
            // Also test with the compiled version if extension is .magic
            if (!use_compiled_ext && fdp.remaining_bytes() > 0) {
                std::string compiled_filename = "/tmp/fuzz_magic_compiled_" + std::to_string(pid) + ".mgc";
                
                // Try to compile first
                int compile_result = magic_compile(magic, temp_filename.c_str());
                if (compile_result == 0) {
                    // Rename or copy would be needed here, but for simplicity just test
                    // the original file with check on a new instance
                    magic_t magic2 = magic_open(actual_flags);
                    if (magic2 != nullptr) {
                        int check_result2 = magic_check(magic2, temp_filename.c_str());
                        (void)check_result2;
                        magic_close(magic2);
                    }
                }
            }
            break;
        }
        
        case 5: {
            // Test edge cases and error conditions
            // First test with empty file
            std::string empty_filename = "/tmp/fuzz_magic_empty_" + std::to_string(pid) + 
                (use_compiled_ext ? ".mgc" : ".magic");
            FILE* empty_fp = fopen(empty_filename.c_str(), "w");
            if (empty_fp) {
                fclose(empty_fp);
                
                int empty_check = magic_check(magic, empty_filename.c_str());
                (void)empty_check;
                
                int empty_compile = magic_compile(magic, empty_filename.c_str());
                (void)empty_compile;
                
                unlink(empty_filename.c_str());
            }
            
            // Test with invalid filename
            int invalid_check = magic_check(magic, "/nonexistent/path/to/file.magic");
            (void)invalid_check;
            
            int invalid_compile = magic_compile(magic, "/nonexistent/path/to/file.magic");
            (void)invalid_compile;
            
            // Test with original file
            int check_result = magic_check(magic, temp_filename.c_str());
            (void)check_result;
            break;
        }
    }
    
    // Clean up
    unlink(temp_filename.c_str());
    magic_close(magic);
    
    return 0;
}

} // namespace harness_014
