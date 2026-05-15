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

namespace harness_011 {
/*
 * Specialized fuzzing harness for libmagic targeting database compilation and verification APIs.
 * 
 * Primary targets (4 completely uncovered APIs):
 * 1. magic_getflags - 2 undiscovered branches
 * 2. magic_compile - 4 undiscovered branches  
 * 3. magic_check - 4 undiscovered branches
 * 4. magic_version - 0 branches but still uncovered
 * 
 * Secondary targets: apprentice.c parsing functions with high undiscovered branch complexity:
 * - apprentice_sort: 389 undiscovered branches
 * - parse_extra: 73 undiscovered branches  
 * - parse: 1469 undiscovered branches
 * - getvalue: 512 undiscovered branches
 * 
 * APIs targeted:
 * - magic_open, magic_version, magic_setflags, magic_compile, magic_check, magic_getflags, magic_close
 * 
 * Strategy:
 * 1. Generate dynamic magic database entries from fuzzer input
 * 2. Test database compilation pipeline: magic_compile -> magic_check
 * 3. Verify flag manipulation: magic_setflags -> magic_getflags
 * 4. Test version information: magic_version
 * 5. Focus on semantic diversity from existing file detection fuzzers by targeting database operations
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: General API testing with default magic database
 * - harness_003: Custom magic database compilation with hardcoded float/double values
 * - harness_009: Dynamic rule compilation from fuzzer input for float/double rules
 * - harness_010: Complex multi-type pattern combinations for magiccheck()
 * - harness_011: Database compilation/verification API testing with dynamic magic database generation (NEW)
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + strategy + magic database content
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int initial_flags = fdp.ConsumeIntegral<int>();
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // 0-4 different testing strategies
    bool test_version_first = fdp.ConsumeBool();
    bool test_flag_manipulation = fdp.ConsumeBool();
    bool test_compile_check = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = initial_flags & 0xFFFFFFF;
    
    // Test magic_version API (completely uncovered)
    int version = magic_version();
    (void)version; // Use result to avoid unused variable warning
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Strategy selection for magic database generation and testing
    switch (strategy) {
        case 0: {
            // Strategy 0: Simple magic database compilation with various entry types
            std::string magic_db;
            
            // Generate number of magic entries from input
            int num_entries = fdp.ConsumeIntegralInRange<int>(1, 20);
            
            for (int i = 0; i < num_entries; i++) {
                if (fdp.remaining_bytes() < 16) break;
                
                // Generate offset
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                
                // Generate entry type from input
                uint8_t entry_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                
                // Build magic entry based on type
                std::string entry = std::to_string(offset) + " ";
                
                switch (entry_type) {
                    case 0:
                        entry += "string ";
                        if (fdp.remaining_bytes() > 4) {
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                // Sanitize for magic format
                                std::string sanitized;
                                for (char c : str_val) {
                                    if (c >= 32 && c <= 126) sanitized += c;
                                    else sanitized += 'X';
                                }
                                entry += sanitized;
                            }
                        }
                        break;
                    case 1:
                        entry += "byte ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint8_t>());
                        break;
                    case 2:
                        entry += "short ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint16_t>());
                        break;
                    case 3:
                        entry += "long ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint32_t>());
                        break;
                    case 4:
                        entry += "float ";
                        entry += std::to_string(fdp.ConsumeFloatingPoint<float>());
                        break;
                    case 5:
                        entry += "double ";
                        entry += std::to_string(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 6:
                        entry += "ubyte ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint8_t>());
                        break;
                    case 7:
                        entry += "beshort ";
                        entry += std::to_string(fdp.ConsumeIntegral<uint16_t>());
                        break;
                }
                
                // Add relation if enough bytes
                if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
                    uint8_t rel = fdp.ConsumeIntegral<uint8_t>() % 5;
                    switch (rel) {
                        case 0: entry.insert(entry.find_last_of(' ') + 1, "= "); break;
                        case 1: entry.insert(entry.find_last_of(' ') + 1, "!= "); break;
                        case 2: entry.insert(entry.find_last_of(' ') + 1, "> "); break;
                        case 3: entry.insert(entry.find_last_of(' ') + 1, "< "); break;
                        case 4: entry.insert(entry.find_last_of(' ') + 1, "& "); break;
                    }
                }
                
                magic_db += entry + "\n";
            }
            
            // Write magic database to temp file
            std::string temp_filename = "/tmp/fuzz_magic_db_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(magic_db.c_str(), 1, magic_db.size(), fp);
                fclose(fp);
                
                // Test magic_compile API (primary target)
                if (test_compile_check) {
                    int compile_result = magic_compile(magic, temp_filename.c_str());
                    (void)compile_result;
                }
                
                // Test magic_check API (primary target)
                if (test_compile_check) {
                    int check_result = magic_check(magic, temp_filename.c_str());
                    (void)check_result;
                }
                
                // Clean up temp file
                unlink(temp_filename.c_str());
            }
            break;
        }
        
        case 1: {
            // Strategy 1: Flag manipulation testing
            // Test magic_setflags and magic_getflags APIs (primary targets)
            
            // Get initial flags
            int current_flags = magic_getflags(magic);
            (void)current_flags;
            
            // Generate new flags from input
            int new_flags = fdp.ConsumeIntegral<int>() & 0xFFFFFFF;
            
            // Test magic_setflags
            if (test_flag_manipulation) {
                int set_result = magic_setflags(magic, new_flags);
                (void)set_result;
                
                // Test magic_getflags after setting
                int updated_flags = magic_getflags(magic);
                (void)updated_flags;
            }
            
            // Generate simple magic database for compilation testing
            std::string simple_db = "0 string TEST Test file\n";
            simple_db += "0 byte 0x7F ASCII text\n";
            
            std::string temp_filename = "/tmp/fuzz_magic_simple_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(simple_db.c_str(), 1, simple_db.size(), fp);
                fclose(fp);
                
                // Test compilation with current flags
                int compile_result = magic_compile(magic, temp_filename.c_str());
                (void)compile_result;
                
                unlink(temp_filename.c_str());
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Complex magic database with multiple levels and continuations
            std::string complex_db;
            
            // Generate hierarchical magic entries
            int num_levels = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            for (int level = 0; level < num_levels; level++) {
                if (fdp.remaining_bytes() < 8) break;
                
                // Add level indicator
                std::string indent(level * 2, '>');
                
                // Add entries at this level
                int entries_at_level = fdp.ConsumeIntegralInRange<int>(1, 5);
                
                for (int e = 0; e < entries_at_level; e++) {
                    if (fdp.remaining_bytes() < 8) break;
                    
                    uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 128;
                    uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    
                    std::string entry = indent + std::to_string(offset) + " ";
                    
                    switch (data_type) {
                        case 0:
                            entry += "string ";
                            if (fdp.remaining_bytes() > 4) {
                                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                                if (fdp.remaining_bytes() >= str_len) {
                                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                    // Basic sanitization
                                    std::string sanitized;
                                    for (char c : str_val) {
                                        if (c >= 32 && c <= 126) sanitized += c;
                                        else sanitized += '.';
                                    }
                                    entry += sanitized;
                                }
                            }
                            break;
                        case 1:
                            entry += "byte ";
                            entry += std::to_string(fdp.ConsumeIntegral<uint8_t>());
                            break;
                        case 2:
                            entry += "short ";
                            entry += std::to_string(fdp.ConsumeIntegral<uint16_t>());
                            break;
                        case 3:
                            entry += "long ";
                            entry += std::to_string(fdp.ConsumeIntegral<uint32_t>());
                            break;
                    }
                    
                    // Add message
                    entry += " message \"Level " + std::to_string(level) + " entry " + std::to_string(e) + "\"";
                    
                    complex_db += entry + "\n";
                }
            }
            
            // Write and test complex database
            std::string temp_filename = "/tmp/fuzz_magic_complex_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(complex_db.c_str(), 1, complex_db.size(), fp);
                fclose(fp);
                
                // Test both compilation and checking
                if (test_compile_check) {
                    int compile_result = magic_compile(magic, temp_filename.c_str());
                    (void)compile_result;
                    
                    int check_result = magic_check(magic, temp_filename.c_str());
                    (void)check_result;
                }
                
                unlink(temp_filename.c_str());
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Edge case testing with invalid/malformed magic databases
            // Generate potentially problematic magic entries
            
            std::string edge_db;
            int num_edge_cases = fdp.ConsumeIntegralInRange<int>(1, 10);
            
            for (int i = 0; i < num_edge_cases; i++) {
                if (fdp.remaining_bytes() < 4) break;
                
                uint8_t edge_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                
                switch (edge_type) {
                    case 0:
                        // Very large offset
                        edge_db += "9999999999 byte 0x00\n";
                        break;
                    case 1:
                        // Negative offset
                        edge_db += "-1 string TEST\n";
                        break;
                    case 2:
                        // Empty type
                        edge_db += "0  \n";
                        break;
                    case 3:
                        // Missing value
                        edge_db += "0 byte\n";
                        break;
                    case 4:
                        // Invalid type
                        edge_db += "0 invalidtype 123\n";
                        break;
                    case 5:
                        // Special characters in string
                        edge_db += "0 string \"test\\x00with\\nnewline\"\n";
                        break;
                }
            }
            
            // Add some valid entries too
            edge_db += "0 string VALID Valid entry\n";
            edge_db += "0 byte 0x7F\n";
            
            std::string temp_filename = "/tmp/fuzz_magic_edge_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(edge_db.c_str(), 1, edge_db.size(), fp);
                fclose(fp);
                
                // Test how compilation handles edge cases
                int compile_result = magic_compile(magic, temp_filename.c_str());
                (void)compile_result;
                
                // Test checking of edge cases
                int check_result = magic_check(magic, temp_filename.c_str());
                (void)check_result;
                
                unlink(temp_filename.c_str());
            }
            break;
        }
        
        case 4: {
            // Strategy 4: Comprehensive API sequence testing
            // Follow exact sequence from coverage guidance
            
            // 1. magic_open already called
            
            // 2. magic_version (test at beginning as per guidance)
            if (test_version_first) {
                int lib_version = magic_version();
                (void)lib_version;
            }
            
            // 3. magic_setflags (optional helper)
            if (test_flag_manipulation) {
                int test_flags = fdp.ConsumeIntegral<int>() & 0xFFFFFFF;
                magic_setflags(magic, test_flags);
            }
            
            // 4. magic_compile with custom magic database input
            // Generate a simple but valid magic database
            std::string guidance_db;
            
            // Add various entry types to exercise apprentice.c parsing
            guidance_db += "0 string MZ MS-DOS executable\n";
            guidance_db += "0 beshort 0xFEFF Unicode text\n";
            guidance_db += "0 lelong 0x464c457f ELF executable\n";
            guidance_db += "0 byte 0x7F ASCII text\n";
            guidance_db += "0 string PK\\x03\\x04 ZIP archive\n";
            
            // Add some entries with relations
            guidance_db += "0 byte >0x00 non-zero byte\n";
            guidance_db += "0 byte <0x80 high bit clear\n";
            guidance_db += "0 string !=EMPTY not empty\n";
            
            std::string temp_filename = "/tmp/fuzz_magic_guidance_" + std::to_string(getpid()) + ".mgc";
            FILE* fp = fopen(temp_filename.c_str(), "w");
            if (fp) {
                fwrite(guidance_db.c_str(), 1, guidance_db.size(), fp);
                fclose(fp);
                
                // Compile the database
                int compile_result = magic_compile(magic, temp_filename.c_str());
                (void)compile_result;
                
                // 5. magic_check to verify compiled database
                int check_result = magic_check(magic, temp_filename.c_str());
                (void)check_result;
                
                // 6. magic_getflags to retrieve current flags
                int final_flags = magic_getflags(magic);
                (void)final_flags;
                
                unlink(temp_filename.c_str());
            }
            break;
        }
    }

    // Always test magic_getflags at the end (primary target)
    int final_flags_check = magic_getflags(magic);
    (void)final_flags_check;

    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_011
