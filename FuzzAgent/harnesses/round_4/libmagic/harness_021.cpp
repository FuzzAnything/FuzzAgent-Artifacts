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

namespace harness_021 {
/*
 * Fuzzing harness for libmagic library - mget function targeting with custom magic patterns
 * Target: mget() in softmagic.c (191 blocked branches)
 * MODULE: softmagic.c (magic pattern matching module)
 * STRATEGY: Target indirect operation paths through custom magic pattern loading and 
 *           crafted input data to reach the blocked branches in mget function
 * DIFFERENTIATION: Unlike existing 21 harnesses which focus on file detection and 
 *           standard magic databases, this harness specifically targets magic pattern 
 *           matching internals using magic_load_buffers with custom patterns to 
 *           exercise indirect operation paths in mget
 * API SEQUENCE: magic_open() -> magic_load_buffers() with custom magic patterns -> 
 *               magic_buffer() with crafted input data -> magic_close()
 * Goal: Reach untouched mget function with 191 blocked branches through targeted
 *       custom magic pattern generation that triggers indirect operations, complex
 *       offsets, and boundary conditions in softmagic.c
 */


// Include libmagic header

// Generate custom magic pattern data to trigger mget's indirect operations
std::string generate_custom_magic_pattern(FuzzedDataProvider& fdp, uint8_t pattern_type) {
    std::string magic_data;
    
    switch (pattern_type) {
        case 0: { // Indirect pattern targeting mget's INDIR logic
            magic_data += "# Indirect pattern test for mget function\n";
            magic_data += "# Targets: mget INDIR flag, indirect operations\n";
            magic_data += "0\tindirect\t0\t0\n";
            
            // Generate multiple indirect operations with different types
            size_t num_indirects = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < num_indirects && fdp.remaining_bytes() > 0; i++) {
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 256;
                magic_data += std::to_string(offset) + "\t";
                
                // Choose indirect type
                uint8_t indir_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (indir_type) {
                    case 0:
                        magic_data += "byte&0x80\t";
                        magic_data += ">0\t";
                        magic_data += "indirect file\n";
                        break;
                    case 1:
                        magic_data += "beshort&0x8000\t";
                        magic_data += ">0\t";
                        magic_data += "big endian indirect\n";
                        break;
                    case 2:
                        magic_data += "leshort&0x0080\t";
                        magic_data += ">0\t";
                        magic_data += "little endian indirect\n";
                        break;
                    case 3:
                        magic_data += "belong&0x80000000\t";
                        magic_data += ">0\t";
                        magic_data += "big endian 32-bit indirect\n";
                        break;
                    case 4:
                        magic_data += "lelong&0x00000080\t";
                        magic_data += ">0\t";
                        magic_data += "little endian 32-bit indirect\n";
                        break;
                    case 5:
                        magic_data += "quad&0x8000000000000000\t";
                        magic_data += ">0\t";
                        magic_data += "64-bit indirect\n";
                        break;
                    case 6:
                        magic_data += "string\t";
                        magic_data += "=\\x80\t";
                        magic_data += "string indirect marker\n";
                        break;
                    case 7:
                        magic_data += "pstring\t";
                        magic_data += "x\t";
                        magic_data += "pascal string indirect\n";
                        break;
                }
            }
            break;
        }
        
        case 1: { // Complex offset patterns for boundary testing
            magic_data += "# Complex offset patterns for mget boundary testing\n";
            magic_data += "# Tests: OFFSET_OOB macro, offset calculations\n";
            
            // Generate patterns with varying offsets
            size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(3, 10);
            for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
                // Use both positive and negative offsets
                int32_t offset = static_cast<int32_t>(fdp.ConsumeIntegral<uint32_t>());
                if (fdp.ConsumeBool()) {
                    // Generate negative offset in range [-99, -1] safely, avoiding overflow when offset = INT32_MIN
                    int64_t safe_offset = static_cast<int64_t>(offset);
                    int64_t abs_offset = llabs(safe_offset);  // Safe absolute value using int64_t
                    offset = -static_cast<int32_t>(abs_offset % 100);  // Negative offset for testing
                }
                magic_data += std::to_string(offset) + "\t";
                
                // Choose data type and operation
                uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 6;
                switch (op_type) {
                    case 0:
                        magic_data += "byte\t";
                        magic_data += "=" + std::to_string(fdp.ConsumeIntegral<uint8_t>()) + "\t";
                        magic_data += "byte equality\n";
                        break;
                    case 1:
                        magic_data += "beshort\t";
                        magic_data += "<" + std::to_string(fdp.ConsumeIntegral<uint16_t>()) + "\t";
                        magic_data += "big endian less than\n";
                        break;
                    case 2:
                        magic_data += "leshort\t";
                        magic_data += ">" + std::to_string(fdp.ConsumeIntegral<uint16_t>()) + "\t";
                        magic_data += "little endian greater than\n";
                        break;
                    case 3:
                        magic_data += "belong\t";
                        magic_data += "&" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + "\t";
                        magic_data += "big endian bitmask\n";
                        break;
                    case 4:
                        magic_data += "lelong\t";
                        magic_data += "^" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + "\t";
                        magic_data += "little endian xor\n";
                        break;
                    case 5:
                        magic_data += "string\t";
                        magic_data += "=";
                        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        std::string str_val = fdp.ConsumeRandomLengthString(str_len);
                        // Escape special characters
                        for (char c : str_val) {
                            if (c == '\t' || c == '\n' || c == '\\') {
                                magic_data += '\\';
                            }
                            magic_data += c;
                        }
                        magic_data += "\tstring match\n";
                        break;
                }
            }
            break;
        }
        
        case 2: { // Nested continuation patterns
            magic_data += "# Nested continuation patterns for mget recursion\n";
            magic_data += "# Tests: cont_level, recursive mget calls\n";
            
            // Start with a base pattern
            magic_data += "0\tbyte\t=0x01\t\\b>\n";
            
            // Add nested continuations
            size_t nest_level = fdp.ConsumeIntegralInRange<size_t>(1, 4);
            for (size_t level = 0; level < nest_level && fdp.remaining_bytes() > 0; level++) {
                for (size_t indent = 0; indent <= level; indent++) {
                    magic_data += ">";
                }
                
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 100;
                magic_data += std::to_string(offset) + "\t";
                
                // Different operations at each level
                uint8_t level_op = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (level_op) {
                    case 0:
                        magic_data += "byte\t";
                        magic_data += "=" + std::to_string(fdp.ConsumeIntegral<uint8_t>()) + "\t";
                        magic_data += "nested byte match level " + std::to_string(level) + "\n";
                        break;
                    case 1:
                        magic_data += "leshort\t";
                        magic_data += "=" + std::to_string(fdp.ConsumeIntegral<uint16_t>()) + "\t";
                        magic_data += "nested short match level " + std::to_string(level) + "\n";
                        break;
                    case 2: {
                        magic_data += "string\t";
                        magic_data += "=";
                        std::string nest_str = fdp.ConsumeRandomLengthString(8);
                        for (char c : nest_str) {
                            if (c == '\t' || c == '\n' || c == '\\') {
                                magic_data += '\\';
                            }
                            magic_data += c;
                        }
                        magic_data += "\tnested string level " + std::to_string(level) + "\n";
                        break;
                    }
                    case 3:
                        magic_data += "indirect\t";
                        magic_data += "0\t";
                        magic_data += "nested indirect level " + std::to_string(level) + "\n";
                        break;
                }
            }
            break;
        }
        
        case 3: { // Mixed pattern types for comprehensive testing
            magic_data += "# Mixed pattern types for comprehensive mget testing\n";
            
            // Generate a mix of different pattern types
            size_t total_patterns = fdp.ConsumeIntegralInRange<size_t>(5, 15);
            for (size_t i = 0; i < total_patterns && fdp.remaining_bytes() > 0; i++) {
                uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % 512;
                magic_data += std::to_string(offset) + "\t";
                
                uint8_t pattern_mix = fdp.ConsumeIntegral<uint8_t>() % 10;
                switch (pattern_mix) {
                    case 0: case 1: // Byte operations
                        magic_data += "byte\t";
                        if (fdp.ConsumeBool()) {
                            magic_data += "=" + std::to_string(fdp.ConsumeIntegral<uint8_t>());
                        } else {
                            magic_data += "&" + std::to_string(fdp.ConsumeIntegral<uint8_t>());
                        }
                        magic_data += "\tbyte op\n";
                        break;
                        
                    case 2: case 3: // Short operations
                        if (fdp.ConsumeBool()) {
                            magic_data += "beshort\t";
                        } else {
                            magic_data += "leshort\t";
                        }
                        magic_data += "=" + std::to_string(fdp.ConsumeIntegral<uint16_t>()) + "\tshort op\n";
                        break;
                        
                    case 4: case 5: // Long operations
                        if (fdp.ConsumeBool()) {
                            magic_data += "belong\t";
                        } else {
                            magic_data += "lelong\t";
                        }
                        magic_data += "=" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + "\tlong op\n";
                        break;
                        
                    case 6: { // String operations
                        magic_data += "string\t";
                        magic_data += "=";
                        std::string mix_str = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 32));
                        for (char c : mix_str) {
                            if (c == '\t' || c == '\n' || c == '\\' || c == '=') {
                                magic_data += '\\';
                            }
                            magic_data += c;
                        }
                        magic_data += "\tstring match\n";
                        break;
                    }
                        
                    case 7: // Indirect operations
                        magic_data += "indirect\t";
                        magic_data += "0\tindirect test\n";
                        break;
                        
                    case 8: { // Regex operations
                        magic_data += "regex\t";
                        magic_data += "=";
                        std::string regex_str = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 16));
                        // Simple regex pattern
                        magic_data += "^";
                        for (char c : regex_str) {
                            if (c == '\t' || c == '\n' || c == '\\' || c == '^' || c == '$' || c == '.') {
                                magic_data += '\\';
                            }
                            magic_data += c;
                        }
                        magic_data += ".*\tregex test\n";
                        break;
                    }
                        
                    case 9: // Default/string operations
                        magic_data += "default\t";
                        magic_data += "default test pattern\n";
                        break;
                }
            }
            break;
        }
    }
    
    return magic_data;
}

// Generate test buffer data that will be analyzed by magic_buffer
std::vector<uint8_t> generate_test_buffer(FuzzedDataProvider& fdp, uint8_t buffer_type) {
    std::vector<uint8_t> buffer;
    
    switch (buffer_type) {
        case 0: { // Structured data with markers for pattern matching
            // Add some magic bytes at specific offsets
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
            buffer.resize(buffer_size);
            
            // Fill with random data
            for (size_t i = 0; i < buffer_size && fdp.remaining_bytes() > 0; i++) {
                buffer[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            
            // Insert pattern markers at various offsets
            size_t num_markers = fdp.ConsumeIntegralInRange<size_t>(3, 10);
            for (size_t i = 0; i < num_markers && buffer_size > 0; i++) {
                size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, buffer_size - 4);
                uint8_t marker_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (marker_type) {
                    case 0: // Byte marker
                        buffer[offset] = 0x01;
                        break;
                    case 1: // Short marker (little endian)
                        buffer[offset] = 0x80;
                        buffer[offset + 1] = 0x00;
                        break;
                    case 2: // Short marker (big endian)
                        buffer[offset] = 0x00;
                        buffer[offset + 1] = 0x80;
                        break;
                    case 3: // String marker
                        if (offset + 8 < buffer_size) {
                            buffer[offset] = 'M';
                            buffer[offset + 1] = 'A';
                            buffer[offset + 2] = 'G';
                            buffer[offset + 3] = 'I';
                            buffer[offset + 4] = 'C';
                            buffer[offset + 5] = '_';
                            buffer[offset + 6] = fdp.ConsumeIntegral<uint8_t>();
                            buffer[offset + 7] = fdp.ConsumeIntegral<uint8_t>();
                        }
                        break;
                }
            }
            break;
        }
        
        case 1: { // Boundary value testing buffer
            // Focus on values that trigger boundary conditions in mget
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(128, 512);
            
            // Add boundary values at strategic positions
            for (size_t i = 0; i < buffer_size && fdp.remaining_bytes() > 0; i++) {
                if (i % 32 == 0) {
                    // Insert boundary values
                    uint8_t boundary_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                    switch (boundary_type) {
                        case 0: buffer.push_back(0x00); break; // Zero
                        case 1: buffer.push_back(0x01); break; // One
                        case 2: buffer.push_back(0x7F); break; // Max positive signed byte
                        case 3: buffer.push_back(0x80); break; // Min negative signed byte
                        case 4: buffer.push_back(0xFF); break; // Max unsigned byte
                        case 5: buffer.push_back(0xFE); break; // Almost max
                        case 6: buffer.push_back(0x02); break; // Small positive
                        case 7: buffer.push_back(0xFD); break; // Almost min negative
                    }
                } else {
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
            }
            break;
        }
        
        case 2: { // Indirect operation triggering buffer
            // Create buffer that will trigger indirect operations in mget
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 1024);
            
            // Header with indirect markers
            buffer.push_back(0x80); // INDIR marker
            buffer.push_back(fdp.ConsumeIntegral<uint8_t>()); // Type
            
            // Add offset data
            uint16_t indirect_offset = fdp.ConsumeIntegral<uint16_t>() % 200;
            buffer.push_back(static_cast<uint8_t>(indirect_offset & 0xFF));
            buffer.push_back(static_cast<uint8_t>((indirect_offset >> 8) & 0xFF));
            
            // Fill remaining buffer with structured data
            for (size_t i = 4; i < buffer_size && fdp.remaining_bytes() > 0; i++) {
                // Create patterns that might be referenced indirectly
                if (i == indirect_offset || i == indirect_offset + 1) {
                    // Put recognizable values at indirect offset
                    buffer.push_back(0x42); // 'B'
                } else if (i > indirect_offset && i < indirect_offset + 16) {
                    // Create a small pattern area
                    buffer.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0x20, 0x7E)); // Printable ASCII
                } else {
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
            }
            break;
        }
        
        case 3: { // Mixed content buffer
            // Combination of different data types
            size_t total_size = fdp.ConsumeIntegralInRange<size_t>(512, 2048);
            
            // Add different sections
            size_t section_size = total_size / 4;
            
            // Section 1: Binary data
            for (size_t i = 0; i < section_size && fdp.remaining_bytes() > 0; i++) {
                buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            // Section 2: Text-like data
            for (size_t i = 0; i < section_size && fdp.remaining_bytes() > 0; i++) {
                buffer.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0x20, 0x7E));
            }
            
            // Section 3: Structured data (repeating patterns)
            for (size_t i = 0; i < section_size && fdp.remaining_bytes() > 0; i++) {
                if (i % 16 == 0) {
                    buffer.push_back(0xAA); // Pattern marker
                } else if (i % 16 == 8) {
                    buffer.push_back(0x55); // Alternate marker
                } else {
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
            }
            
            // Section 4: Boundary values
            for (size_t i = 0; i < section_size && fdp.remaining_bytes() > 0; i++) {
                if (i % 8 == 0) {
                    uint8_t boundary = fdp.ConsumeIntegral<uint8_t>() % 4;
                    switch (boundary) {
                        case 0: buffer.push_back(0x00); break;
                        case 1: buffer.push_back(0xFF); break;
                        case 2: buffer.push_back(0x80); break;
                        case 3: buffer.push_back(0x7F); break;
                    }
                } else {
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
            }
            break;
        }
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for pattern generation
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open
    int flags = 0;
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Generate custom magic pattern data
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    std::string magic_pattern = generate_custom_magic_pattern(fdp, pattern_type);
    
    if (magic_pattern.empty() || fdp.remaining_bytes() < 16) {
        magic_close(magic);
        return 0;
    }
    
    // Prepare buffers for magic_load_buffers
    void* magic_buffers[1];
    size_t magic_sizes[1];
    
    magic_buffers[0] = const_cast<char*>(magic_pattern.c_str());
    magic_sizes[0] = magic_pattern.size();
    
    // Load custom magic patterns using magic_load_buffers
    int load_result = magic_load_buffers(magic, magic_buffers, magic_sizes, 1);
    
    // If loading succeeds, test with generated buffer
    if (load_result != -1) {
        // Generate test buffer data
        uint8_t buffer_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        std::vector<uint8_t> test_buffer = generate_test_buffer(fdp, buffer_type);
        
        if (!test_buffer.empty()) {
            // Test the buffer with our custom patterns
            const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
            (void)result; // Avoid unused variable warning
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_021
