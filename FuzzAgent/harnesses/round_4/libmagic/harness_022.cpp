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
#include <algorithm>
#include "magic.h"

namespace harness_022 {
/*
 * Fuzzing harness for libmagic library - parse() function error condition targeting
 * Target: parse() in apprentice.c with 178 blocked branches
 * MODULE: apprentice.c (magic file parser module)
 * STRATEGY: Target parse function's error conditions through malformed magic pattern generation
 *           using magic_open, magic_load_buffers, magic_check, magic_compile APIs
 * DIFFERENTIATION: Focus on deep coverage needs (Phase 2) since API coverage is already 100% -
 *           target error conditions and malformed patterns rather than normal API usage
 * API SEQUENCE: magic_open() -> magic_load_buffers() with malformed magic patterns -> 
 *               magic_check() -> magic_compile() -> magic_close()
 * Goal: Reach untouched parse() function with 178 blocked branches through targeted
 *       malformed magic pattern generation that triggers syntax errors, invalid types,
 *       and boundary conditions in apprentice.c parse() function
 */


// Include libmagic header

// Helper function to generate malformed magic patterns targeting parse() error conditions
std::string generate_malformed_magic_pattern(FuzzedDataProvider& fdp, uint8_t pattern_type) {
    std::string magic_data;
    
    switch (pattern_type) {
        case 0: { // Invalid offset patterns
            magic_data += "# Invalid offset patterns for parse() testing\n";
            magic_data += "# Targets: offset parsing, strtol failures, offset validation\n";
            
            size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                // Generate invalid offset patterns
                uint8_t offset_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (offset_type) {
                    case 0: // Missing offset
                        magic_data += "     \t"; // Empty offset with tabs
                        break;
                    case 1: // Non-numeric offset
                        magic_data += "abc\t"; // Invalid characters
                        break;
                    case 2: // Offset with invalid prefix
                        magic_data += "0xZZ\t"; // Invalid hex
                        break;
                    case 3: // Huge offset
                        magic_data += "999999999999999999999999999999\t"; // Overflow
                        break;
                    case 4: // Negative offset without sign flag
                        magic_data += "-123\t";
                        break;
                    case 5: // Offset with embedded spaces
                        magic_data += "12 34\t"; // Space in middle
                        break;
                    case 6: // Offset with special characters
                        magic_data += "12@34\t"; // Invalid char
                        break;
                    case 7: // Just operator without offset
                        magic_data += ">\t"; // Continuation without offset
                        break;
                }
                
                // Add type and value
                magic_data += "string\t";
                magic_data += "\"test\"\t";
                magic_data += "test_line_" + std::to_string(i) + "\n";
            }
            break;
        }
        
        case 1: { // Invalid type patterns
            magic_data += "# Invalid type patterns for parse() testing\n";
            magic_data += "# Targets: get_type() failures, FILE_INVALID returns\n";
            
            size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                // Valid offset first
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                magic_data += std::to_string(offset) + "\t";
                
                // Generate invalid type patterns
                uint8_t type_error = fdp.ConsumeIntegral<uint8_t>() % 6;
                switch (type_error) {
                    case 0: // Empty type
                        magic_data += "\t"; // No type
                        break;
                    case 1: // Invalid type string
                        magic_data += "xyz\t"; // Not a valid type
                        break;
                    case 2: // Type with invalid suffix
                        magic_data += "stringxyz\t"; // Invalid suffix
                        break;
                    case 3: // Type with embedded spaces
                        magic_data += "str ing\t"; // Space in type
                        break;
                    case 4: // Special characters in type
                        magic_data += "@type\t"; // Invalid start char
                        break;
                    case 5: // Very long invalid type
                        magic_data += "invalid_type_name_that_is_way_too_long_and_not_valid\t";
                        break;
                }
                
                magic_data += "\"test\"\t";
                magic_data += "test_line_" + std::to_string(i) + "\n";
            }
            break;
        }
        
        case 2: { // Invalid indirect/offset modifier patterns
            magic_data += "# Invalid indirect/offset modifier patterns\n";
            magic_data += "# Targets: parse_indirect_modifier, parse_op_modifier, parse_string_modifier\n";
            
            size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                magic_data += std::to_string(offset) + "\t";
                magic_data += "string\t";
                
                // Generate invalid modifier patterns
                uint8_t modifier_error = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (modifier_error) {
                    case 0: // Invalid indirect offset
                        magic_data += "(invalid)\t"; // Invalid indirect
                        break;
                    case 1: // Missing closing paren
                        magic_data += "(\t"; // No closing )
                        break;
                    case 2: // Extra closing paren
                        magic_data += "())\t"; // Too many )
                        break;
                    case 3: // Invalid indirect type
                        magic_data += "(xyz)\t"; // Invalid type char
                        break;
                    case 4: // Indirect with missing offset
                        magic_data += "(l)\t"; // No offset value
                        break;
                    case 5: // Invalid op for string type
                        magic_data += "&0x80\t"; // & not allowed for strings
                        break;
                    case 6: // Invalid string modifier
                        magic_data += "/xyz\t"; // Invalid modifier
                        break;
                    case 7: // Complex invalid pattern
                        magic_data += "(l~+123))\t"; // Mixed errors
                        break;
                }
                
                magic_data += "\"test\"\t";
                magic_data += "test_line_" + std::to_string(i) + "\n";
            }
            break;
        }
        
        case 3: { // Invalid relation/value patterns
            magic_data += "# Invalid relation and value patterns\n";
            magic_data += "# Targets: getvalue() failures, relation parsing errors\n";
            
            size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                magic_data += std::to_string(offset) + "\t";
                magic_data += "string\t";
                
                // Generate invalid relation/value patterns
                uint8_t rel_error = fdp.ConsumeIntegral<uint8_t>() % 6;
                switch (rel_error) {
                    case 0: // Invalid relation character
                        magic_data += "@\t"; // Invalid reln
                        break;
                    case 1: // Missing value after relation
                        magic_data += "=\t"; // No value
                        break;
                    case 2: // Invalid numeric value
                        magic_data += "=abc\t"; // Non-numeric
                        break;
                    case 3: // Invalid string value format
                        magic_data += "=test\t"; // String without quotes
                        break;
                    case 4: // Unclosed string quote
                        magic_data += "=\"test\t"; // Missing closing quote
                        break;
                    case 5: // Invalid escaped characters
                        magic_data += "=\"test\\";
                        if (fdp.remaining_bytes() > 0) {
                            std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(1);
                            magic_data += static_cast<char>(extra[0]);
                        }
                        magic_data += "\"\t";
                        break;
                }
                
                magic_data += "test_line_" + std::to_string(i) + "\n";
            }
            break;
        }
        
        case 4: { // Continuation level errors
            magic_data += "# Continuation level errors\n";
            magic_data += "# Targets: cont_level validation, continuation parsing\n";
            
            // First create a base entry
            magic_data += "0\tstring\t\"base\"\tbase_entry\n";
            
            // Generate invalid continuations
            size_t num_continuations = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < num_continuations && fdp.remaining_bytes() > 0; i++) {
                uint8_t cont_error = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (cont_error) {
                    case 0: // Too many > symbols
                        magic_data += ">>>>\t"; // Too deep
                        break;
                    case 1: // Continuation without base
                        magic_data += ">\t"; // Should fail without previous entry
                        // Actually need to reset after first error case
                        if (i == 0) {
                            magic_data = ">\tstring\t\"test\"\tcont_without_base\n";
                            return magic_data;
                        }
                        break;
                    case 2: // Skipped continuation level
                        magic_data += ">>\t"; // Skip level 1
                        break;
                    case 3: // Continuation with invalid offset
                        magic_data += ">abc\t"; // Invalid offset in cont
                        break;
                }
                
                magic_data += "string\t\"cont\"\tcontinuation_" + std::to_string(i) + "\n";
            }
            break;
        }
        
        case 5: { // Mixed multiple error types
            magic_data += "# Mixed multiple error types for comprehensive testing\n";
            magic_data += "# Targets: Multiple parse() error paths in single input\n";
            
            size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(3, 15);
            for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                // Mix different error types
                uint8_t line_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                
                switch (line_type) {
                    case 0: // Valid line (as separator between errors)
                        magic_data += std::to_string(fdp.ConsumeIntegral<uint32_t>() % 256) + "\t";
                        magic_data += "string\t\"valid\"\tvalid_separator\n";
                        break;
                        
                    case 1: // Offset error
                        magic_data += "INVALID\tstring\t\"test\"\toffset_error\n";
                        break;
                        
                    case 2: // Type error  
                        magic_data += "0\tINVALID_TYPE\t\"test\"\ttype_error\n";
                        break;
                        
                    case 3: // Modifier error
                        magic_data += "0\tstring\t(INVALID)\tmodifier_error\n";
                        break;
                        
                    case 4: // Relation error
                        magic_data += "0\tstring\t@\trelation_error\n";
                        break;
                }
            }
            break;
        }
        
        case 6: { // Special edge cases and boundary conditions
            magic_data += "# Special edge cases and boundary conditions\n";
            magic_data += "# Targets: Boundary checks, special keyword handling\n";
            
            // Generate various edge cases
            std::vector<std::string> edge_cases = {
                "0 name \"test\" name_at_wrong_level\n",  // NAME at wrong level
                "0 default test_default\n",  // default keyword
                "0 clear clear_test\n",  // clear keyword  
                "0 regex/re test_regex\n",  // regex with invalid modifier
                "0 search/1 test_search\n",  // search with invalid count
                "0 use invalid_magic\n",  // use with invalid reference
                "0 invalid/xyz test_invalid\n",  // invalid type with modifier
                "0 byte& test_missing_mask\n",  // missing mask value
                "0 byte&0x test_invalid_hex\n",  // incomplete hex
                "0 byte=0x test_invalid_value\n",  // invalid hex value
                "0 string test_no_quotes\n",  // string without quotes
                "0 string \"unclosed test\n",  // unclosed string
                "0 string \"esc\\x\" test_bad_escape\n",  // bad escape
                "0 string \"test\"\bno_space_desc\n",  // backspace instead of tab
            };
            
            // Select random edge cases
            size_t num_cases = fdp.ConsumeIntegralInRange<size_t>(1, edge_cases.size());
            for (size_t i = 0; i < num_cases && i < edge_cases.size(); i++) {
                magic_data += edge_cases[i];
            }
            break;
        }
        
        case 7: { // Very long lines and extreme values
            magic_data += "# Very long lines and extreme values\n";
            magic_data += "# Targets: Buffer limits, integer overflows, extreme offsets\n";
            
            // Generate lines with extreme values
            size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 0; i++) {
                uint8_t extreme_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (extreme_type) {
                    case 0: { // Extreme offset
                        magic_data += "2147483647\t"; // INT32_MAX
                        magic_data += "string\t\"test\"\textreme_offset\n";
                        break;
                    }
                        
                    case 1: { // Very long type name
                        magic_data += "0\t";
                        magic_data += "very_long_type_name_that_exceeds_normal_bounds\t";
                        magic_data += "\"test\"\tlong_type\n";
                        break;
                    }
                        
                    case 2: { // Very long string value
                        magic_data += "0\tstring\t\"";
                        // Generate long string
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(100, 500);
                        if (fdp.remaining_bytes() < str_len) {
                            str_len = fdp.remaining_bytes();
                        }
                        if (str_len > 0) {
                            std::vector<uint8_t> str_data = fdp.ConsumeBytes<uint8_t>(str_len);
                            for (size_t j = 0; j < str_data.size(); j++) {
                                if (str_data[j] >= 32 && str_data[j] < 127 && 
                                    str_data[j] != '\\' && str_data[j] != '"') {
                                    magic_data += static_cast<char>(str_data[j]);
                                } else {
                                    magic_data += '.';
                                }
                            }
                        }
                        magic_data += "\"\tlong_string_value\n";
                        break;
                    }
                        
                    case 3: { // Very long description
                        magic_data += "0\tstring\t\"test\"\t";
                        // Generate long description
                        size_t desc_len = fdp.ConsumeIntegralInRange<size_t>(100, 300);
                        if (fdp.remaining_bytes() < desc_len) {
                            desc_len = fdp.remaining_bytes();
                        }
                        if (desc_len > 0) {
                            std::vector<uint8_t> desc_data = fdp.ConsumeBytes<uint8_t>(desc_len);
                            for (size_t j = 0; j < desc_data.size(); j++) {
                                if (desc_data[j] >= 32 && desc_data[j] < 127 && 
                                    desc_data[j] != '\n' && desc_data[j] != '\r') {
                                    magic_data += static_cast<char>(desc_data[j]);
                                } else {
                                    magic_data += '.';
                                }
                            }
                        }
                        magic_data += "\n";
                        break;
                    }
                } // end switch extreme_type
            } // end for loop
            break;
        } // end case 7
    } // end switch pattern_type
    
    return magic_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - include MAGIC_CHECK to enable parse-time validation
    int flags = MAGIC_CHECK;  // Always enable check mode to trigger parse() validation
    
    // Consume additional flags
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Consume operation type: 0 = magic_load_buffers, 1 = magic_compile, 2 = magic_check
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Generate malformed magic pattern based on fuzzed data
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    std::string malformed_magic = generate_malformed_magic_pattern(fdp, pattern_type);
    
    switch (operation) {
        case 0: {
            // Test magic_load_buffers with malformed patterns
            // Create buffer from malformed magic data
            if (!malformed_magic.empty()) {
                void* buffers[1];
                size_t sizes[1];
                
                buffers[0] = malloc(malformed_magic.size());
                if (buffers[0] != NULL) {
                    memcpy(buffers[0], malformed_magic.data(), malformed_magic.size());
                    sizes[0] = malformed_magic.size();
                    
                    // Try to load malformed magic data - should fail but exercise parse() error paths
                    int load_result = magic_load_buffers(magic, buffers, sizes, 1);
                    (void)load_result; // Expected to fail for malformed data
                    
                    free(buffers[0]);
                }
            }
            break;
        }
        
        case 1: {
            // Test magic_compile with malformed patterns via temporary file
            char temp_filename[] = "/tmp/libmagic_parse_fuzz_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0 && !malformed_magic.empty()) {
                if (write(fd, malformed_magic.data(), malformed_magic.size()) == (ssize_t)malformed_magic.size()) {
                    close(fd);
                    
                    // Try to compile malformed magic file - should fail but exercise parse()
                    int compile_result = magic_compile(magic, temp_filename);
                    (void)compile_result; // Expected to fail
                    
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            } else if (fd >= 0) {
                close(fd);
                unlink(temp_filename);
            }
            break;
        }
        
        case 2: {
            // Test magic_check with malformed patterns
            char temp_filename[] = "/tmp/libmagic_check_fuzz_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0 && !malformed_magic.empty()) {
                if (write(fd, malformed_magic.data(), malformed_magic.size()) == (ssize_t)malformed_magic.size()) {
                    close(fd);
                    
                    // Try to check malformed magic file - should fail but exercise parse()
                    int check_result = magic_check(magic, temp_filename);
                    (void)check_result; // Expected to fail
                    
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            } else if (fd >= 0) {
                close(fd);
                unlink(temp_filename);
            }
            break;
        }
    }
    
    // Always try to also test with a small valid pattern to ensure some API calls succeed
    if (fdp.remaining_bytes() > 10) {
        // Create a minimal valid magic buffer as fallback
        std::string valid_magic = "0 string \"test\" valid_fallback_pattern\n";
        void* valid_buffers[1];
        size_t valid_sizes[1];
        
        valid_buffers[0] = malloc(valid_magic.size());
        if (valid_buffers[0] != NULL) {
            memcpy(valid_buffers[0], valid_magic.data(), valid_magic.size());
            valid_sizes[0] = valid_magic.size();
            
            // Try to load valid magic data (may succeed if previous operation failed)
            int valid_load = magic_load_buffers(magic, valid_buffers, valid_sizes, 1);
            (void)valid_load;
            
            free(valid_buffers[0]);
        }
    }
    
    // Close magic handle
    magic_close(magic);
    
    return 0;
}

} // namespace harness_022
