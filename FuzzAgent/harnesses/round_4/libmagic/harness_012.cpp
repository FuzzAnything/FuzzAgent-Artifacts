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
#include <cstdio>
#include "magic.h"

namespace harness_012 {
/*
 * Fuzzing harness for libmagic library - Magic Pattern Compilation & Validation
 * Targets: 
 *   1) magic_compile, magic_load_buffers, magic_check APIs to exercise magic database compilation and validation
 *   2) Parsing functions parse_mime, parse_apple, parse_ext, parse_extra, parse_strength in apprentice.c 
 *      which have 110+ uncovered branches
 * Goal: Generate diverse magic pattern files with various field types, strengths, MIME types, 
 *       Apple types, extensions, and extra metadata to trigger deep parsing logic
 * Focus: Complex magic pattern compilation, validation of malformed patterns, 
 *        exercise parsing functions with edge cases
 * Semantic differentiation: While harness_011 tests uncovered APIs generally, this harness specifically 
 *        targets magic database compilation and the parsing functions in apprentice.c with 
 *        synthetic magic patterns generated from fuzzer input
 * Strategy: Use FuzzedDataProvider to create complex magic patterns with:
 *        - Various field types (byte, short, long, string, date, etc.)
 *        - Different strengths with factor operations (+, -, *, /)
 *        - MIME types with valid and invalid characters
 *        - Apple types (creator codes, file types)
 *        - File extensions with special characters
 *        - Extra metadata annotations
 *        - Both valid and malformed patterns to exercise error handling
 * Expected coverage gain: Target the 110+ uncovered branches in parse_* functions 
 *        and exercise deep compilation/validation logic
 */


// Include libmagic header

// Helper to generate magic pattern lines with various field types
std::string generate_magic_pattern_line(FuzzedDataProvider& fdp, int pattern_type) {
    std::string line;
    
    // Consume offset (0-65535)
    uint16_t offset = fdp.ConsumeIntegralInRange<uint16_t>(0, 65535);
    line += std::to_string(offset);
    
    // Consume data type
    uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 20;
    switch (data_type) {
        case 0: line += " byte"; break;
        case 1: line += " short"; break;
        case 2: line += " long"; break;
        case 3: line += " string"; break;
        case 4: line += " pstring"; break;
        case 5: line += " beshort"; break;
        case 6: line += " belong"; break;
        case 7: line += " leshort"; break;
        case 8: line += " lelong"; break;
        case 9: line += " melong"; break;
        case 10: line += " bedate"; break;
        case 11: line += " ledate"; break;
        case 12: line += " medate"; break;
        case 13: line += " beid3"; break;
        case 14: line += " leid3"; break;
        case 15: line += " indirect"; break;
        case 16: line += " name"; break;
        case 17: line += " use"; break;
        case 18: line += " regex"; break;
        case 19: line += " search"; break;
        default: line += " byte"; break;
    }
    
    // Consume comparison operator
    uint8_t op = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (op) {
        case 0: line += " ="; break;
        case 1: line += " <"; break;
        case 2: line += " >"; break;
        case 3: line += " &"; break;
        case 4: line += " ^"; break;
        default: line += " ="; break;
    }
    
    // Consume value based on type
    if (data_type == 2 || data_type == 6 || data_type == 8 || data_type == 9) { // long types
        uint32_t value = fdp.ConsumeIntegral<uint32_t>();
        line += " " + std::to_string(value);
    } else if (data_type == 1 || data_type == 5 || data_type == 7) { // short types
        uint16_t value = fdp.ConsumeIntegral<uint16_t>();
        line += " " + std::to_string(value);
    } else if (data_type == 3 || data_type == 4) { // string types
        size_t max_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string str_value = fdp.ConsumeRandomLengthString(max_len);
        // Escape quotes and backslashes for magic file format
        std::string escaped;
        for (char c : str_value) {
            if (c == '\\' || c == '"') {
                escaped += '\\';
            }
            escaped += c;
        }
        line += " \"" + escaped + "\"";
    } else { // byte and others
        uint8_t value = fdp.ConsumeIntegral<uint8_t>();
        line += " " + std::to_string(value);
    }
    
    // Consume message/description
    line += " ";
    size_t desc_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string desc = fdp.ConsumeRandomLengthString(desc_len);
    // Replace newlines in description
    std::replace(desc.begin(), desc.end(), '\n', ' ');
    std::replace(desc.begin(), desc.end(), '\r', ' ');
    line += desc;
    
    // Based on pattern_type, add annotations
    if (pattern_type == 1 && fdp.ConsumeBool()) {
        // Add strength annotation
        line += "\n>strength ";
        uint8_t factor_op = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (factor_op) {
            case 0: line += "+"; break;
            case 1: line += "-"; break;
            case 2: line += "*"; break;
            case 3: line += "/"; break;
            default: break; // No operator
        }
        if (factor_op < 4) {
            uint8_t factor = fdp.ConsumeIntegralInRange<uint8_t>(1, 255);
            if (factor_op == 3 && factor == 0) factor = 1; // Avoid division by zero
            line += std::to_string(factor);
        }
    } else if (pattern_type == 2 && fdp.ConsumeBool()) {
        // Add MIME type annotation
        line += "\n>mime ";
        std::string mime_type = fdp.ConsumeRandomLengthString(64);
        // MIME types typically have format type/subtype
        if (mime_type.length() > 0 && mime_type.find('/') == std::string::npos) {
            size_t split_pos = fdp.ConsumeIntegralInRange<size_t>(1, mime_type.length());
            mime_type.insert(split_pos, "/");
        }
        line += mime_type;
    } else if (pattern_type == 3 && fdp.ConsumeBool()) {
        // Add Apple type annotation
        line += "\n>apple ";
        // Apple types are 4-character codes
        std::string apple_type;
        for (int i = 0; i < 4; i++) {
            char c = fdp.ConsumeIntegralInRange<char>('A', 'Z');
            apple_type += c;
        }
        line += apple_type;
        if (fdp.ConsumeBool()) {
            line += ",";
            for (int i = 0; i < 4; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('A', 'Z');
                apple_type += c;
            }
            line += apple_type;
        }
    } else if (pattern_type == 4 && fdp.ConsumeBool()) {
        // Add extension annotation
        line += "\n>ext ";
        std::string ext = fdp.ConsumeRandomLengthString(32);
        // Remove problematic characters
        ext.erase(std::remove(ext.begin(), ext.end(), '/'), ext.end());
        ext.erase(std::remove(ext.begin(), ext.end(), '\\'), ext.end());
        ext.erase(std::remove(ext.begin(), ext.end(), ':'), ext.end());
        line += ext;
    }
    
    return line + "\n";
}

// Generate a complete magic file with multiple patterns
std::string generate_magic_file(FuzzedDataProvider& fdp, bool include_malformed) {
    std::string magic_file;
    
    // Magic file header
    magic_file += "# Magic file generated by fuzzer\n";
    magic_file += "# Testing various pattern types and annotations\n";
    
    // Generate number of pattern entries
    size_t num_entries = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_entries; i++) {
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 5);
        std::string line = generate_magic_pattern_line(fdp, pattern_type);
        magic_file += line;
        
        // Sometimes add continuation lines
        if (fdp.ConsumeBool() && i < num_entries - 1) {
            magic_file += ">";
            line = generate_magic_pattern_line(fdp, pattern_type);
            // Remove offset from continuation line
            size_t space_pos = line.find(' ');
            if (space_pos != std::string::npos) {
                line = line.substr(space_pos + 1);
            }
            magic_file += line;
        }
    }
    
    // Optionally add malformed patterns
    if (include_malformed && fdp.ConsumeBool()) {
        magic_file += "\n# Malformed patterns section\n";
        
        // Various malformed pattern types
        uint8_t malformed_type = fdp.ConsumeIntegral<uint8_t>() % 10;
        switch (malformed_type) {
            case 0:
                // Missing offset
                magic_file += "byte = 0x41 Missing offset\n";
                break;
            case 1:
                // Invalid data type
                magic_file += "0 invalidtype = 123 Bad type\n";
                break;
            case 2:
                // Unclosed string
                magic_file += "0 string = \"unclosed string\n";
                break;
            case 3:
                // Invalid operator
                magic_file += "0 byte ? 65 Invalid operator\n";
                break;
            case 4:
                // Negative offset
                magic_file += "-10 byte = 65 Negative offset\n";
                break;
            case 5:
                // Huge offset
                magic_file += "999999999 byte = 65 Huge offset\n";
                break;
            case 6:
                // Malformed strength
                magic_file += "0 byte = 65 Test\n";
                magic_file += ">strength &invalid\n";
                break;
            case 7:
                // Malformed MIME
                magic_file += "0 byte = 65 Test\n";
                magic_file += ">mime invalid/mime/type/here\n";
                break;
            case 8:
                // Empty annotation
                magic_file += "0 byte = 65 Test\n";
                magic_file += ">\n";
                break;
            case 9:
                // Nested continuations
                magic_file += "0 byte = 65 Level1\n";
                magic_file += ">>byte = 66 Level2\n";
                magic_file += ">>>byte = 67 Level3\n";
                break;
        }
    }
    
    return magic_file;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful pattern generation
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open
    int flags = 0;
    uint16_t flag_word = fdp.ConsumeIntegral<uint16_t>();
    
    // Standard flags
    if (flag_word & 0x0001) flags |= MAGIC_DEBUG;
    if (flag_word & 0x0002) flags |= MAGIC_SYMLINK;
    if (flag_word & 0x0004) flags |= MAGIC_COMPRESS;
    if (flag_word & 0x0008) flags |= MAGIC_DEVICES;
    if (flag_word & 0x0010) flags |= MAGIC_MIME_TYPE;
    if (flag_word & 0x0020) flags |= MAGIC_CONTINUE;
    if (flag_word & 0x0040) flags |= MAGIC_CHECK;
    if (flag_word & 0x0080) flags |= MAGIC_RAW;
    if (flag_word & 0x0100) flags |= MAGIC_ERROR;
    if (flag_word & 0x0200) flags |= MAGIC_MIME_ENCODING;
    if (flag_word & 0x0400) flags |= MAGIC_APPLE;
    if (flag_word & 0x0800) flags |= MAGIC_EXTENSION;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Consume operation type
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    int result = 0;
    
    switch (operation) {
        case 0: {
            // Test magic_compile with temporary magic file
            // Generate magic file content
            bool include_malformed = fdp.ConsumeBool();
            std::string magic_content = generate_magic_file(fdp, include_malformed);
            
            // Write to temporary file
            char temp_filename[] = "/tmp/libmagic_compile_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                if (write(fd, magic_content.c_str(), magic_content.size()) == (ssize_t)magic_content.size()) {
                    close(fd);
                    
                    // Try to compile the magic file
                    result = magic_compile(magic, temp_filename);
                    // Result can be -1 (error) or 0 (success), both are valid test cases
                    
                    // Clean up temp file
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            }
            break;
        }
        
        case 1: {
            // Test magic_load_buffers with generated magic buffer
            // Generate magic file content
            std::string magic_content = generate_magic_file(fdp, false);
            
            // Prepare buffer array for magic_load_buffers
            size_t nbufs = 1;
            void* buffers[1];
            size_t sizes[1];
            
            // Allocate buffer and copy content
            buffers[0] = malloc(magic_content.size());
            if (buffers[0] != NULL) {
                memcpy(buffers[0], magic_content.c_str(), magic_content.size());
                sizes[0] = magic_content.size();
                
                // Try to load from buffer
                result = magic_load_buffers(magic, buffers, sizes, nbufs);
                
                free(buffers[0]);
            }
            break;
        }
        
        case 2: {
            // Test magic_check with temporary magic file
            // Generate magic file content (mix of valid and malformed)
            std::string magic_content = generate_magic_file(fdp, true);
            
            // Write to temporary file
            char temp_filename[] = "/tmp/libmagic_check_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                if (write(fd, magic_content.c_str(), magic_content.size()) == (ssize_t)magic_content.size()) {
                    close(fd);
                    
                    // Check the magic file
                    result = magic_check(magic, temp_filename);
                    // Result indicates if magic file is valid
                    
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
            // Combined test: compile, check, and load
            // Generate magic file content
            std::string magic_content = generate_magic_file(fdp, fdp.ConsumeBool());
            
            // Write to temporary file
            char temp_filename[] = "/tmp/libmagic_combined_XXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd >= 0) {
                if (write(fd, magic_content.c_str(), magic_content.size()) == (ssize_t)magic_content.size()) {
                    close(fd);
                    
                    // Try compilation
                    int compile_result = magic_compile(magic, temp_filename);
                    (void)compile_result; // Use result
                    
                    // Try checking
                    int check_result = magic_check(magic, temp_filename);
                    (void)check_result; // Use result
                    
                    // If compilation succeeded, also try to load and use it
                    if (compile_result == 0) {
                        // Re-open magic handle for fresh load
                        magic_close(magic);
                        magic = magic_open(flags);
                        if (magic != NULL) {
                            int load_result = magic_load(magic, temp_filename);
                            if (load_result == 0) {
                                // Test with some data
                                size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_size);
                                if (!test_data.empty()) {
                                    const char* type_result = magic_buffer(magic, test_data.data(), test_data.size());
                                    (void)type_result;
                                }
                            }
                        }
                    }
                    
                    // Clean up temp file
                    unlink(temp_filename);
                } else {
                    close(fd);
                    unlink(temp_filename);
                }
            }
            break;
        }
    }
    
    // Close magic handle
    if (magic != NULL) {
        magic_close(magic);
    }
    
    return 0;
}

} // namespace harness_012
