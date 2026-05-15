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

namespace harness_024 {
/*
 * Fuzzing harness for libmagic library - Magic File Annotation Parsing
 * Targets: parse_strength(), parse_apple(), parse_mime(), parse_ext(), parse_extra() in apprentice.c
 * Based on coverage analysis: UNCOVERED FUNCTIONS: parse_strength(), parse_apple(), parse_mime(), parse_ext(), parse_extra() in apprentice.c
 * MODULE: apprentice.c (magic file parser module)
 * STRATEGY: Target magic file compilation and parsing APIs (magic_check, magic_compile) with annotated magic file data
 *           to reach these uncovered parsing logic paths for "!" annotations
 * DIFFERENTIATION: These parsing functions remain uncovered despite 24 existing harnesses - focus on specific 
 *           annotation parsing that previous harnesses missed
 * API SEQUENCE: magic_open() -> magic_check() / magic_compile() with annotated magic file data -> magic_close()
 * Annotation types to target:
 *   !strength: Factor operations (+, -, *, /) with numeric factors
 *   !apple: Apple creator/type annotations
 *   !mime: MIME type specifications  
 *   !ext: File extension lists
 *   Plus combinations and edge cases
 */


// Include libmagic header

// Helper to generate magic file content with various annotations
std::string generate_annotated_magic_file(FuzzedDataProvider& fdp) {
    std::string magic_content;
    
    // Magic file header - minimal valid magic entry to attach annotations to
    // Format: offset type value description
    magic_content = "0 byte 0x42 \"Test file\"\n";
    
    // Determine how many annotations to add (1-10)
    size_t num_annotations = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_annotations && fdp.remaining_bytes() > 0; i++) {
        // Choose annotation type
        uint8_t annotation_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        switch (annotation_type) {
            case 0: { // !strength
                magic_content += "!strength ";
                
                // Factor operation: +, -, *, /, or none
                uint8_t op_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
                switch (op_choice) {
                    case 0: magic_content += ""; break; // No op
                    case 1: magic_content += "+"; break;
                    case 2: magic_content += "-"; break;
                    case 3: magic_content += "*"; break;
                    case 4: magic_content += "/"; break;
                }
                
                // Factor value (0-255, but 0 invalid for division)
                uint8_t factor = fdp.ConsumeIntegral<uint8_t>();
                if (factor == 0 && op_choice == 4) { // Division by zero case
                    factor = fdp.ConsumeIntegralInRange<uint8_t>(1, 255);
                }
                magic_content += std::to_string(static_cast<int>(factor)) + "\n";
                break;
            }
            
            case 1: { // !apple
                magic_content += "!apple ";
                
                // Generate Apple creator/type (4 chars each, traditionally uppercase)
                std::string creator;
                std::string type;
                
                for (int j = 0; j < 4 && fdp.remaining_bytes() > 0; j++) {
                    char c = fdp.ConsumeIntegral<char>();
                    // Apple types often use printable chars, sometimes uppercase
                    if (isprint(c)) creator += c;
                    else creator += 'A' + (static_cast<uint8_t>(c) % 26);
                }
                
                for (int j = 0; j < 4 && fdp.remaining_bytes() > 0; j++) {
                    char c = fdp.ConsumeIntegral<char>();
                    if (isprint(c)) type += c;
                    else type += 'A' + (static_cast<uint8_t>(c) % 26);
                }
                
                if (creator.length() < 4) creator.append(4 - creator.length(), 'A');
                if (type.length() < 4) type.append(4 - type.length(), 'A');
                
                magic_content += creator + " " + type + "\n";
                break;
            }
            
            case 2: { // !mime
                magic_content += "!mime ";
                
                // Generate MIME type (type/subtype with allowed chars: +-/.$?:{};=)
                std::vector<std::string> mime_types = {
                    "text/plain", "application/octet-stream", "image/jpeg", 
                    "application/pdf", "text/html", "application/xml"
                };
                
                // Either use predefined or generate random
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                    // Use predefined
                    size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, mime_types.size() - 1);
                    magic_content += mime_types[idx] + "\n";
                } else {
                    // Generate random MIME-like string
                    std::string mime_type;
                    size_t type_len = fdp.ConsumeIntegralInRange<size_t>(3, 20);
                    
                    for (size_t j = 0; j < type_len && fdp.remaining_bytes() > 0; j++) {
                        if (j == 0) {
                            // First char should be letter
                            char c = 'a' + (fdp.ConsumeIntegral<uint8_t>() % 26);
                            mime_type += c;
                        } else if (j < type_len/2) {
                            // Type part before /
                            char c = fdp.ConsumeIntegral<char>();
                            if (isalnum(c) || strchr("+-.", c)) mime_type += c;
                            else mime_type += 'a';
                        } else if (j == type_len/2) {
                            mime_type += '/';
                        } else {
                            // Subtype part after /
                            char c = fdp.ConsumeIntegral<char>();
                            if (isalnum(c) || strchr("+-.$?:{};=", c)) mime_type += c;
                            else mime_type += 'a';
                        }
                    }
                    
                    // Ensure it has a /
                    if (mime_type.find('/') == std::string::npos) {
                        if (mime_type.length() > 2) {
                            mime_type.insert(mime_type.length()/2, "/");
                        } else {
                            mime_type = "text/plain";
                        }
                    }
                    
                    magic_content += mime_type + "\n";
                }
                break;
            }
            
            case 3: { // !ext
                magic_content += "!ext ";
                
                // Generate comma-separated extension list
                size_t num_exts = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                for (size_t j = 0; j < num_exts && fdp.remaining_bytes() > 0; j++) {
                    if (j > 0) magic_content += ",";
                    
                    // Extension length (1-10 chars)
                    size_t ext_len = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                    for (size_t k = 0; k < ext_len && fdp.remaining_bytes() > 0; k++) {
                        char c = fdp.ConsumeIntegral<char>();
                        // Extension chars: letters, digits, and some specials: ,!+-/@?_$&~.
                        if (isalnum(c) || strchr(",!+-/@?_$&~.", c)) {
                            magic_content += c;
                        } else {
                            magic_content += 'a' + (static_cast<uint8_t>(c) % 26);
                        }
                    }
                }
                magic_content += "\n";
                break;
            }
            
            case 4: { // Multiple annotations or malformed
                // Try to trigger edge cases by creating malformed or multiple annotations
                if (fdp.ConsumeBool()) {
                    // Multiple annotations in sequence
                    magic_content += "!strength +42\n";
                    magic_content += "!mime text/plain\n";
                    magic_content += "!ext txt,doc,pdf\n";
                } else {
                    // Malformed annotation to test error paths
                    std::string malformed = "!";
                    size_t mal_len = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                    for (size_t j = 0; j < mal_len && fdp.remaining_bytes() > 0; j++) {
                        malformed += fdp.ConsumeIntegral<char>();
                    }
                    magic_content += malformed + "\n";
                }
                break;
            }
        }
    }
    
    return magic_content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful annotation generation
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - include MAGIC_CHECK for validation warnings
    int flags = 0;
    
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;  // Important for annotation validation
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Also consider APPLE and EXTENSION flags which affect annotation handling
    if (fdp.ConsumeBool()) flags |= MAGIC_APPLE;
    if (fdp.ConsumeBool()) flags |= MAGIC_EXTENSION;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Generate annotated magic file content
    std::string magic_content = generate_annotated_magic_file(fdp);
    
    // Choose operation: 0 = magic_compile (to file), 1 = magic_check (validation only)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    int result = -1;
    
    if (operation == 0) {
        // magic_compile - compile the magic file
        // Write content to temporary file
        char temp_filename[] = "/tmp/libmagic_annotation_fuzz_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd >= 0) {
            if (write(fd, magic_content.c_str(), magic_content.size()) == (ssize_t)magic_content.size()) {
                close(fd);
                
                // Try to compile the magic file
                result = magic_compile(magic, temp_filename);
                
                // Clean up temp file
                unlink(temp_filename);
            } else {
                close(fd);
                unlink(temp_filename);
            }
        }
    } else {
        // magic_check - validate the magic file
        // Write content to temporary file
        char temp_filename[] = "/tmp/libmagic_check_fuzz_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd >= 0) {
            if (write(fd, magic_content.c_str(), magic_content.size()) == (ssize_t)magic_content.size()) {
                close(fd);
                
                // Check/validate the magic file
                result = magic_check(magic, temp_filename);
                
                // Clean up temp file
                unlink(temp_filename);
            } else {
                close(fd);
                unlink(temp_filename);
            }
        }
    }
    
    // Also test magic_load_buffers with the annotation data directly
    if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
        // Use remaining bytes as magic buffer data
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
        
        if (!buffer_data.empty()) {
            // Create buffer array for magic_load_buffers
            void* buffers[1] = {buffer_data.data()};
            size_t sizes[1] = {buffer_data.size()};
            
            // Note: This might fail since buffer needs to be compiled .mgc format
            // but we're testing error paths too
            magic_load_buffers(magic, buffers, sizes, 1);
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_024
