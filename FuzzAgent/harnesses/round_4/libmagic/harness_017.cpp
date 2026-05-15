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
#include "magic.h"

namespace harness_017 {
/*
 * Fuzzing harness for libmagic library - Magic File Parser Core Functions
 * Targets: parse_mime(), parse_ext(), parse_apple(), etc. (0% coverage) in apprentice.c
 * MODULE: apprentice.c (magic file parser module)
 * STRATEGY: Target magic_check() and magic_compile() APIs with magic file content as fuzzer input
 * Goal: Reach untouched parser functions and unblock 200+ branches in parse() function
 * DIFFERENTIATION: Unlike existing 17 harnesses, focus on magic file format parsing - use magic file content as input rather than regular file data
 * Differentiated from harness_006: Direct raw magic file content testing vs generated syntax testing
 * Differentiated from other harnesses: Pure magic file parser targeting vs application file format testing
 * API SEQUENCE: magic_open() -> magic_check() -> magic_compile() -> magic_close()
 * Input strategy: Consume fuzzer input as raw magic file content (bytes) to test parser directly
 */


// Include libmagic header - note: magic.h might be in different location
// Based on existing harnesses, they use "magic.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for at least a basic magic rule
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on compilation and checking flags
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging for parser errors
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;      // Important for MIME parsing
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;          // CRITICAL: Enable checking for magic_check()
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Strategy selection: how to test the magic file content
    // 0 = Direct magic_compile with remaining bytes as magic file content
    // 1 = Create temp file with magic content and test magic_check/magic_compile
    // 2 = Use magic_load_buffers with magic content
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    int result = 0;
    
    if (strategy == 0) {
        // Strategy 0: Direct magic_compile with raw magic file content
        // Use all remaining bytes as magic file content
        std::vector<uint8_t> magic_content = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!magic_content.empty()) {
            // Create a null-terminated copy for magic_compile
            std::string magic_str(magic_content.begin(), magic_content.end());
            
            // Test magic_compile - this should exercise parse_mime, parse_ext, parse_apple
            // magic_compile returns 0 on success, -1 on failure
            result = magic_compile(magic, magic_str.c_str());
            // Don't check result - failures are expected for invalid magic syntax
            (void)result;
        }
        
    } else if (strategy == 1) {
        // Strategy 1: Create temp file with magic content
        // Generate magic file content that specifically targets parse_* functions
        std::string magic_content;
        
        // Consume content type to target different parser functions
        uint8_t content_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (content_type) {
            case 0: {
                // Target parse_mime() - MIME type annotations
                // Format: offset\ttype\tpattern\tMIME\tmime/type
                magic_content += "0\tstring\t";
                
                // Generate pattern
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                if (fdp.remaining_bytes() < pattern_len) {
                    pattern_len = fdp.remaining_bytes();
                }
                if (pattern_len > 0) {
                    std::vector<uint8_t> pattern_data = fdp.ConsumeBytes<uint8_t>(pattern_len);
                    magic_content += "\"";
                    for (size_t i = 0; i < pattern_data.size(); i++) {
                        char c = static_cast<char>(pattern_data[i] % 256);
                        // Ensure printable characters for string pattern
                        if (isprint(c) && c != '\\' && c != '"') {
                            magic_content += c;
                        } else {
                            magic_content += '.';
                        }
                    }
                    magic_content += "\"\t";
                }
                
                magic_content += "MIME\t";
                
                // Generate MIME type (targets parse_mime allowed chars: "+-/.$?:{};=")
                size_t mime_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                if (fdp.remaining_bytes() < mime_len) {
                    mime_len = fdp.remaining_bytes();
                }
                if (mime_len > 0) {
                    std::vector<uint8_t> mime_data = fdp.ConsumeBytes<uint8_t>(mime_len);
                    for (size_t i = 0; i < mime_data.size(); i++) {
                        char c = static_cast<char>(mime_data[i] % 256);
                        // MIME type characters: alnum plus "+-/.$?:{};="
                        if (isalnum(c) || strchr("+-/.$?:{};=", c) != NULL) {
                            magic_content += c;
                        } else {
                            magic_content += 'a'; // fallback
                        }
                    }
                }
                magic_content += "\n";
                break;
            }
                
            case 1: {
                // Target parse_ext() - extension annotations
                // Format: offset\ttype\tpattern\tEXTENSION\textension
                magic_content += "0\tstring\t";
                
                // Generate pattern
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                if (fdp.remaining_bytes() < pattern_len) {
                    pattern_len = fdp.remaining_bytes();
                }
                if (pattern_len > 0) {
                    std::vector<uint8_t> pattern_data = fdp.ConsumeBytes<uint8_t>(pattern_len);
                    magic_content += "\"";
                    for (size_t i = 0; i < pattern_data.size(); i++) {
                        char c = static_cast<char>(pattern_data[i] % 256);
                        if (isprint(c) && c != '\\' && c != '"') {
                            magic_content += c;
                        } else {
                            magic_content += '.';
                        }
                    }
                    magic_content += "\"\t";
                }
                
                magic_content += "EXTENSION\t";
                
                // Generate extension (targets parse_ext allowed chars: ",!+-/@?_$&~.")
                size_t ext_len = fdp.ConsumeIntegralInRange<size_t>(1, 12);
                if (fdp.remaining_bytes() < ext_len) {
                    ext_len = fdp.remaining_bytes();
                }
                if (ext_len > 0) {
                    std::vector<uint8_t> ext_data = fdp.ConsumeBytes<uint8_t>(ext_len);
                    for (size_t i = 0; i < ext_data.size(); i++) {
                        char c = static_cast<char>(ext_data[i] % 256);
                        // Extension characters: alnum plus ",!+-/@?_$&~."
                        if (isalnum(c) || strchr(",!+-/@?_$&~.", c) != NULL) {
                            magic_content += c;
                        } else {
                            magic_content += 'a'; // fallback
                        }
                    }
                }
                magic_content += "\n";
                break;
            }
                
            case 2: {
                // Target parse_apple() - Apple CREATOR/TYPE annotations
                // Format: offset\ttype\tpattern\tAPPLE\tcreator:type
                magic_content += "0\tstring\t";
                
                // Generate pattern
                size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
                if (fdp.remaining_bytes() < pattern_len) {
                    pattern_len = fdp.remaining_bytes();
                }
                if (pattern_len > 0) {
                    std::vector<uint8_t> pattern_data = fdp.ConsumeBytes<uint8_t>(pattern_len);
                    magic_content += "\"";
                    for (size_t i = 0; i < pattern_data.size(); i++) {
                        char c = static_cast<char>(pattern_data[i] % 256);
                        if (isprint(c) && c != '\\' && c != '"') {
                            magic_content += c;
                        } else {
                            magic_content += '.';
                        }
                    }
                    magic_content += "\"\t";
                }
                
                magic_content += "APPLE\t";
                
                // Generate Apple annotation (targets parse_apple allowed chars: "!+-./?")
                // Format: creator:type (each 4 chars)
                size_t creator_len = 4;
                for (size_t i = 0; i < creator_len && fdp.remaining_bytes() > 0; i++) {
                    uint8_t byte = fdp.ConsumeIntegral<uint8_t>();
                    char c = static_cast<char>(byte % 256);
                    // Apple creator characters: alnum plus "!+-./?"
                    if (isalnum(c) || strchr("!+-./?", c) != NULL) {
                        magic_content += c;
                    } else {
                        magic_content += 'A'; // fallback
                    }
                }
                
                magic_content += ":";
                
                size_t type_len = 4;
                for (size_t i = 0; i < type_len && fdp.remaining_bytes() > 0; i++) {
                    uint8_t byte = fdp.ConsumeIntegral<uint8_t>();
                    char c = static_cast<char>(byte % 256);
                    if (isalnum(c) || strchr("!+-./?", c) != NULL) {
                        magic_content += c;
                    } else {
                        magic_content += 'T'; // fallback
                    }
                }
                magic_content += "\n";
                break;
            }
                
            case 3: {
                // Mixed content - multiple lines targeting different parsers
                // Generate 2-5 magic rules
                size_t num_rules = fdp.ConsumeIntegralInRange<size_t>(2, 5);
                for (size_t i = 0; i < num_rules && fdp.remaining_bytes() > 8; i++) {
                    uint8_t rule_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    uint16_t offset = fdp.ConsumeIntegral<uint16_t>() % 1024;
                    
                    magic_content += std::to_string(offset) + "\t";
                    
                    if (rule_type == 0) {
                        magic_content += "string\t\"";
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                        if (fdp.remaining_bytes() < str_len) {
                            str_len = fdp.remaining_bytes();
                        }
                        if (str_len > 0) {
                            std::vector<uint8_t> str_data = fdp.ConsumeBytes<uint8_t>(str_len);
                            for (size_t j = 0; j < str_data.size(); j++) {
                                char c = static_cast<char>(str_data[j] % 256);
                                if (isprint(c) && c != '\\' && c != '"') {
                                    magic_content += c;
                                } else {
                                    magic_content += '.';
                                }
                            }
                        }
                        magic_content += "\"\t";
                        
                        // Random annotation type
                        uint8_t annot_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                        if (annot_type == 0) {
                            magic_content += "MIME\ttext/plain\n";
                        } else if (annot_type == 1) {
                            magic_content += "EXTENSION\t.txt\n";
                        } else {
                            magic_content += "APPLE\tTEXT:ttxt\n";
                        }
                    } else {
                        // Simple numeric rule
                        uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        const char* types[] = {"byte", "short", "long", "beshort"};
                        magic_content += types[num_type] + std::string("\t");
                        
                        uint32_t value = fdp.ConsumeIntegral<uint32_t>();
                        char hex_buf[12];
                        snprintf(hex_buf, sizeof(hex_buf), "0x%08x", value);
                        magic_content += hex_buf;
                        magic_content += "\t\"Test rule ";
                        magic_content += std::to_string(i);
                        magic_content += "\"\n";
                    }
                }
                break;
            }
                
            default: {
                // Raw magic content - use remaining bytes as-is
                std::vector<uint8_t> raw_content = fdp.ConsumeRemainingBytes<uint8_t>();
                magic_content.assign(raw_content.begin(), raw_content.end());
                break;
            }
        }
        
        // Create temporary file
        char temp_filename[] = "/tmp/libmagic_parser_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd < 0) {
            magic_close(magic);
            return 0;
        }
        
        // Write magic content to temp file
        if (write(fd, magic_content.c_str(), magic_content.size()) != (ssize_t)magic_content.size()) {
            close(fd);
            unlink(temp_filename);
            magic_close(magic);
            return 0;
        }
        close(fd);
        
        // Test magic_check() - validates magic file syntax
        // This should exercise the parse() function and all parse_* helpers
        int check_result = magic_check(magic, temp_filename);
        (void)check_result; // Don't check result
        
        // Test magic_compile() - compiles magic file
        int compile_result = magic_compile(magic, temp_filename);
        (void)compile_result; // Don't check result
        
        // Clean up temp file
        unlink(temp_filename);
        
    } else { // strategy == 2
        // Strategy 2: Use magic_load_buffers with magic content
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
        
        // Create buffers with magic content
        for (size_t i = 0; i < num_buffers; i++) {
            // Consume size for this buffer
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
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
        
        // Try to load from buffers - this may invoke compilation/parsing internally
        int load_result = magic_load_buffers(magic, buffers, sizes, num_buffers);
        (void)load_result;
        
        // Free buffers
        for (size_t i = 0; i < num_buffers; i++) {
            if (buffers[i] != NULL) {
                free(buffers[i]);
            }
        }
        free(buffers);
        free(sizes);
    }
    
    // Close magic handle
    magic_close(magic);
    
    return 0;
}

} // namespace harness_017
