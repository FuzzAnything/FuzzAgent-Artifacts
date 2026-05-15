#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_011 {
/*
 * Fuzzing harness for libmagic apprentice.c parsing functions (300+ uncovered branches)
 * Target: Magic file validation and parsing logic in apprentice.c
 * 
 * Critical coverage gap functions (0% coverage, high complexity):
 * - check_format_type: 134 branches, 0% covered
 * - parse_string_modifier: 84 branches, 0% covered  
 * - parse_extra: 38 branches, 0% covered
 * - parse_apple: 0% covered
 * - parse_mime: 0% covered
 * Total: ~300+ undiscovered branches
 * 
 * Strategy: Generate diverse magic pattern files with complex syntax to exercise
 * apprentice.c parsing functions through magic_check API and related compilation APIs.
 * Focus on pattern types, string modifiers, extra fields, apple/mime annotations.
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Helper function to generate magic pattern with various format types
std::string generate_magic_pattern(FuzzedDataProvider& fdp) {
    std::string magic_pattern;
    
    // Generate multiple magic pattern lines
    size_t num_lines = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_lines && fdp.remaining_bytes() > 10; i++) {
        // Start with offset
        int offset = fdp.ConsumeIntegralInRange<int>(0, 100);
        magic_pattern += std::to_string(offset) + " ";
        
        // Choose data type to trigger different parsing paths
        int type_choice = fdp.ConsumeIntegralInRange<int>(0, 30);
        
        switch (type_choice) {
            case 0:  // byte
                magic_pattern += "byte";
                break;
            case 1:  // short
                magic_pattern += "short";
                break;
            case 2:  // long  
                magic_pattern += "long";
                break;
            case 3:  // quad
                magic_pattern += "quad";
                break;
            case 4:  // string
                magic_pattern += "string";
                // Add string modifiers to trigger parse_string_modifier
                if (fdp.ConsumeBool()) {
                    int modifier_choice = fdp.ConsumeIntegralInRange<int>(0, 5);
                    switch (modifier_choice) {
                        case 0: magic_pattern += "/c"; break;  // case-insensitive
                        case 1: magic_pattern += "/b"; break;  // binary string
                        case 2: magic_pattern += "/s"; break;  // search
                        case 3: magic_pattern += "/t"; break;  // trim whitespace
                        case 4: magic_pattern += "/w"; break;  // wide char
                        case 5: magic_pattern += "/l"; break;  // lowercase
                    }
                }
                break;
            case 5:  // beshort (big-endian short)
                magic_pattern += "beshort";
                break;
            case 6:  // belong (big-endian long)
                magic_pattern += "belong";
                break;
            case 7:  // bedate (big-endian date)
                magic_pattern += "bedate";
                break;
            case 8:  // beldate (big-endian long date)
                magic_pattern += "beldate";
                break;
            case 9:  // bequad (big-endian quad)
                magic_pattern += "bequad";
                break;
            case 10: // leshort (little-endian short)
                magic_pattern += "leshort";
                break;
            case 11: // lelong (little-endian long)
                magic_pattern += "lelong";
                break;
            case 12: // ledate (little-endian date)
                magic_pattern += "ledate";
                break;
            case 13: // leldate (little-endian long date)
                magic_pattern += "leldate";
                break;
            case 14: // lequad (little-endian quad)
                magic_pattern += "lequad";
                break;
            case 15: // indirect (file offset)
                magic_pattern += "indirect";
                break;
            case 16: // name (named magic)
                magic_pattern += "name";
                break;
            case 17: // use (use another magic entry)
                magic_pattern += "use";
                break;
            case 18: // regex (regular expression)
                magic_pattern += "regex";
                break;
            case 19: // search (search for string)
                magic_pattern += "search";
                break;
            default: // default to byte
                magic_pattern += "byte";
                break;
        }
        
        magic_pattern += " ";
        
        // Add test value with various formats to trigger check_format_type
        int format_choice = fdp.ConsumeIntegralInRange<int>(0, 10);
        switch (format_choice) {
            case 0:
                magic_pattern += "=" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 1:
                magic_pattern += "!=" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 2:
                magic_pattern += "&" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 3:
                magic_pattern += "^" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 4:
                magic_pattern += "<" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 5:
                magic_pattern += ">" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 6:
                magic_pattern += "&=" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 7:
                magic_pattern += "^=" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
            case 8:
                magic_pattern += "x";  // any value
                break;
            case 9:
                // String test value
                if (fdp.remaining_bytes() > 10) {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                    std::string str_val = fdp.ConsumeBytesAsString(str_len);
                    // Escape special characters
                    std::string escaped;
                    for (char c : str_val) {
                        if (c == '\\' || c == '"' || c == '\n' || c == '\r' || c == '\t') {
                            escaped += '\\';
                            escaped += c;
                        } else if (isprint(static_cast<unsigned char>(c))) {
                            escaped += c;
                        } else {
                            char hex[5];
                            snprintf(hex, sizeof(hex), "\\x%02x", static_cast<unsigned char>(c));
                            escaped += hex;
                        }
                    }
                    magic_pattern += "=\"" + escaped + "\"";
                } else {
                    magic_pattern += "=\"test\"";
                }
                break;
            case 10:
                // Hex value
                magic_pattern += "=0x" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                break;
        }
        
        magic_pattern += " ";
        
        // Add description with potential MIME/APPLE/EXTENSION annotations
        // to trigger parse_mime, parse_apple, parse_extra
        std::string description;
        size_t desc_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
        if (fdp.remaining_bytes() >= desc_len) {
            description = fdp.ConsumeBytesAsString(desc_len);
            // Clean up description (remove newlines)
            std::replace(description.begin(), description.end(), '\n', ' ');
            std::replace(description.begin(), description.end(), '\r', ' ');
        } else {
            description = "test data";
        }
        
        // Add annotations based on probability
        if (fdp.ConsumeProbability<double>() < 0.3) {
            // Add MIME type annotation
            description += " \\012MIME: ";
            std::vector<std::string> mime_types = {
                "text/plain", "application/octet-stream", "image/png",
                "image/jpeg", "text/html", "application/json"
            };
            int mime_idx = fdp.ConsumeIntegralInRange<int>(0, mime_types.size() - 1);
            description += mime_types[mime_idx];
        }
        
        if (fdp.ConsumeProbability<double>() < 0.2) {
            // Add APPLE type annotation  
            description += " \\012!:apple ";
            description += fdp.ConsumeBool() ? "TEXT" : "BINA";
        }
        
        if (fdp.ConsumeProbability<double>() < 0.2) {
            // Add EXTENSION annotation
            description += " \\012!:ext ";
            std::vector<std::string> extensions = {
                "txt", "bin", "png", "jpg", "html", "json", "pdf"
            };
            int ext_idx = fdp.ConsumeIntegralInRange<int>(0, extensions.size() - 1);
            description += extensions[ext_idx];
        }
        
        magic_pattern += description + "\n";
    }
    
    return magic_pattern;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful magic pattern generation
    // We need enough bytes for pattern generation and API testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PHASE 1: Setup - Create magic handle with fuzzed flags
    // ============================================================
    
    int flags = 0;
    
    // Consume flags from fuzzed input - include MAGIC_CHECK for validation
    if (fdp.ConsumeBool()) flags |= MAGIC_NONE;
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_DEVICES;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    if (fdp.ConsumeBool()) flags |= MAGIC_ERROR;
    // Include MAGIC_CHECK to enable validation warnings
    if (fdp.ConsumeBool()) flags |= MAGIC_CHECK;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // If magic_open fails with complex flags, try with simple flags
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // ============================================================
    // PHASE 2: Generate complex magic patterns to trigger apprentice.c parsing
    // ============================================================
    
    // Generate magic pattern content with diverse syntax
    std::string magic_content = generate_magic_pattern(fdp);
    
    // If we couldn't generate enough content, use a fallback pattern
    if (magic_content.empty() || magic_content.length() < 10) {
        // Fallback pattern with various syntax elements
        magic_content = 
            "0 string \\x89PNG\\r\\n\\x1a\\n PNG image data \\012MIME: image/png\n"
            "0 byte 0x7f ELF executable\n"
            "0 beshort 0xfeff Unicode text\n"
            "0 string/c GIF GIF image data \\012!:apple BINA \\012!:ext gif\n"
            "4 string/c RIFF Microsoft RIFF data\n"
            "0 belong 0x464c457f ELF executable\n"
            "0 leshort 0x5a4d DOS executable\n"
            "0 regex/c \\\\[.*\\\\] PDF document\n"
            "0 indirect 0 use \\012!:ext txt\n"
            "0 search/1 test test data with \\012MIME: text/plain\n";
    }
    
    // Write magic database to a temporary file
    char magic_template[] = "/tmp/magic_apprentice_fuzz_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Write the magic content
    ssize_t written = write(magic_fd, magic_content.c_str(), magic_content.size());
    if (written < 0) {
        close(magic_fd);
        unlink(magic_template);
        magic_close(magic);
        return 0;
    }
    fsync(magic_fd);
    close(magic_fd);
    
    // ============================================================
    // PHASE 3: Exercise apprentice.c parsing through compilation APIs
    // ============================================================
    
    // 1. magic_load - Load the magic file (triggers apprentice.c parsing)
    int load_result = magic_load(magic, magic_template);
    // Result can be -1 (error) or 0 (success) - both are valid
    
    // 2. magic_check - Validate the magic file (primary target API)
    // This specifically exercises the validation logic in apprentice.c
    int check_result = magic_check(magic, magic_template);
    // check_result triggers check_format_type and other validation functions
    
    // 3. magic_compile - Compile the magic file
    int compile_result = magic_compile(magic, magic_template);
    
    // 4. magic_list - List magic entries
    int list_result = magic_list(magic, magic_template);
    
    // ============================================================
    // PHASE 4: Test with actual file detection using the parsed patterns
    // ============================================================
    
    // Create test file with fuzzed data
    char test_template[] = "/tmp/test_apprentice_fuzz_XXXXXX";
    int test_fd = mkstemp(test_template);
    if (test_fd != -1) {
        // Consume test data from remaining fuzzed input
        size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        if (test_data_size > 0) {
            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
            write(test_fd, test_data.data(), test_data.size());
            fsync(test_fd);
            
            // Test magic_file with the parsed magic database
            const char* file_result = magic_file(magic, test_template);
            (void)file_result;
            
            // Test magic_descriptor
            lseek(test_fd, 0, SEEK_SET);
            const char* desc_result = magic_descriptor(magic, test_fd);
            (void)desc_result;
            
            // Test magic_buffer with the same data
            const char* buffer_result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)buffer_result;
        }
        close(test_fd);
        unlink(test_template);
    }
    
    // Clean up temporary magic file
    unlink(magic_template);
    
    // ============================================================
    // PHASE 5: Additional parsing tests with different flag combinations
    // ============================================================
    
    // Try loading with different flags to trigger different parsing paths
    if (fdp.remaining_bytes() > 0) {
        int alt_flags = fdp.ConsumeIntegral<int>();
        magic_setflags(magic, alt_flags);
        
        // Try reloading with alternative flags
        if (fdp.ConsumeBool()) {
            // Create another magic file with different syntax
            std::string alt_magic_content = generate_magic_pattern(fdp);
            if (!alt_magic_content.empty()) {
                char alt_template[] = "/tmp/magic_alt_fuzz_XXXXXX";
                int alt_fd = mkstemp(alt_template);
                if (alt_fd != -1) {
                    write(alt_fd, alt_magic_content.c_str(), alt_magic_content.size());
                    fsync(alt_fd);
                    close(alt_fd);
                    
                    magic_load(magic, alt_template);
                    magic_check(magic, alt_template);
                    
                    unlink(alt_template);
                }
            }
        }
    }
    
    // ============================================================
    // PHASE 6: Cleanup
    // ============================================================
    
    magic_close(magic);
    
    return 0;
}

} // namespace harness_011
