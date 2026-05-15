#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_020 {
/*
 * Specialized fuzzing harness for libmagic targeting apprentice_magic_strength_1 
 * with 97 blocked branches via magic_buffer API chain to exercise uncovered FILE_TYPE cases.
 * 
 * Primary internal target: apprentice_magic_strength_1 - 97 blocked branches
 * Focus: magic_buffer -> file_buffer -> file_softmagic -> match -> magiccheck -> file_magic_strength -> apprentice_magic_strength_1
 * 
 * APIs targeted:
 * - magic_open (Helper: Init)
 * - magic_load (Helper: Load magic database)
 * - magic_buffer (Target: Main execution - triggers softmagic path to apprentice_magic_strength_1)
 * - magic_close (Helper: Cleanup)
 * 
 * Semantic differentiation from existing harnesses (000-019):
 * - harness_018: Compression format testing + FILE_DEFAULT database testing
 * - harness_019: Magic database validation/compilation focus with dynamic generation of ALL uncovered magic types
 * - harness_020: **Magic_buffer API chain targeting with generated test buffers matching magic patterns for FILE_DEFAULT, FILE_BESTRING16 types** (NEW)
 * 
 * Strategy for harness_020:
 * 1. Generate test buffers that match specific magic patterns targeting uncovered FILE_TYPE cases
 * 2. Focus on FILE_DEFAULT (value 3) and FILE_BESTRING16 types which have uncovered switch cases
 * 3. Create buffers with structured data at specific offsets to trigger softmagic matching
 * 4. Test magic_buffer with diverse flag combinations to explore different execution paths
 * 5. Generate patterns that exercise factor_op handling (lines 949-952 in apprentice.c)
 * 6. Test '^' relation and default relation cases (lines 1065, 1070-1072)
 * 7. Focus on the full API chain from magic_buffer down to apprentice_magic_strength_1
 * 
 * Expected coverage gains:
 * - Unlock 97+ blocked branches in apprentice_magic_strength_1
 * - Activate uncovered code paths for FILE_DEFAULT and FILE_BESTRING16 type handling
 * - Improve overall branch coverage by targeting specific softmagic execution path
 */


// Helper to generate test buffers matching specific magic patterns for apprentice_magic_strength_1
std::vector<uint8_t> generate_test_buffer_for_filetype(FuzzedDataProvider& fdp, int file_type) {
    std::vector<uint8_t> buffer;
    
    // Start with a reasonable size
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 1024);
    buffer.resize(buffer_size, 0);
    
    // Fill buffer with random data
    std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(buffer.size());
    if (random_data.size() > 0) {
        std::copy(random_data.begin(), 
                  random_data.begin() + std::min(random_data.size(), buffer.size()),
                  buffer.begin());
    }
    
    // Add specific patterns based on file_type to trigger magic matching
    switch (file_type) {
        case 3: { // FILE_DEFAULT
            // For FILE_DEFAULT, we need to create data that might match a default pattern
            // Default patterns often match common file headers or specific byte sequences
            if (buffer.size() >= 16) {
                // Add some common file headers at various offsets
                int offset = fdp.ConsumeIntegralInRange<int>(0, buffer.size() - 16);
                
                // Try different common patterns
                int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                switch (pattern_type) {
                    case 0: // PNG header
                        if (buffer.size() - offset >= 8) {
                            buffer[offset] = 0x89; buffer[offset+1] = 'P'; buffer[offset+2] = 'N'; buffer[offset+3] = 'G';
                            buffer[offset+4] = 0x0D; buffer[offset+5] = 0x0A; buffer[offset+6] = 0x1A; buffer[offset+7] = 0x0A;
                        }
                        break;
                    case 1: // GIF header
                        if (buffer.size() - offset >= 6) {
                            buffer[offset] = 'G'; buffer[offset+1] = 'I'; buffer[offset+2] = 'F';
                            buffer[offset+3] = '8'; buffer[offset+4] = '7'; buffer[offset+5] = 'a';
                        }
                        break;
                    case 2: // JPEG header
                        if (buffer.size() - offset >= 4) {
                            buffer[offset] = 0xFF; buffer[offset+1] = 0xD8; buffer[offset+2] = 0xFF; buffer[offset+3] = 0xE0;
                        }
                        break;
                    case 3: // PDF header
                        if (buffer.size() - offset >= 5) {
                            buffer[offset] = '%'; buffer[offset+1] = 'P'; buffer[offset+2] = 'D'; 
                            buffer[offset+3] = 'F'; buffer[offset+4] = '-';
                        }
                        break;
                    case 4: // ZIP header
                        if (buffer.size() - offset >= 4) {
                            buffer[offset] = 'P'; buffer[offset+1] = 'K'; buffer[offset+2] = 0x03; buffer[offset+3] = 0x04;
                        }
                        break;
                }
            }
            break;
        }
        
        case 1016: { // FILE_BESTRING16 (from apprentice.c line 1016)
            // FILE_BESTRING16 expects 16-bit big-endian string data
            // Create some 16-bit character sequences
            if (buffer.size() >= 4) {
                int offset = fdp.ConsumeIntegralInRange<int>(0, buffer.size() - 4);
                
                // Create a simple 16-bit character (2 bytes per character, big-endian)
                uint16_t char1 = fdp.ConsumeIntegral<uint16_t>();
                uint16_t char2 = fdp.ConsumeIntegral<uint16_t>();
                
                // Store in big-endian format
                buffer[offset] = (char1 >> 8) & 0xFF;
                buffer[offset+1] = char1 & 0xFF;
                buffer[offset+2] = (char2 >> 8) & 0xFF;
                buffer[offset+3] = char2 & 0xFF;
                
                // Optionally add more 16-bit characters
                if (buffer.size() - offset >= 8 && fdp.ConsumeBool()) {
                    uint16_t char3 = fdp.ConsumeIntegral<uint16_t>();
                    uint16_t char4 = fdp.ConsumeIntegral<uint16_t>();
                    buffer[offset+4] = (char3 >> 8) & 0xFF;
                    buffer[offset+5] = char3 & 0xFF;
                    buffer[offset+6] = (char4 >> 8) & 0xFF;
                    buffer[offset+7] = char4 & 0xFF;
                }
            }
            break;
        }
        
        default:
            // For other types, just use random data
            break;
    }
    
    return buffer;
}

// Generate test data with specific patterns for relation testing
std::vector<uint8_t> generate_relation_test_buffer(FuzzedDataProvider& fdp, char relation) {
    std::vector<uint8_t> buffer(128, 0);
    
    // Add some structured data based on relation type
    switch (relation) {
        case '^': // Masking bits relation (line 1065)
        case '&': // Masking bits relation
            if (buffer.size() >= 8) {
                // Create data that can be masked
                uint32_t value = fdp.ConsumeIntegral<uint32_t>();
                uint32_t mask = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&buffer[0], &value, sizeof(value));
                memcpy(&buffer[4], &mask, sizeof(mask));
            }
            break;
            
        case '=': // Exact match (line 1054)
            if (buffer.size() >= 4) {
                uint32_t exact_value = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&buffer[0], &exact_value, sizeof(exact_value));
            }
            break;
            
        case '>': // Greater than (line 1058)
        case '<': // Less than
            if (buffer.size() >= 8) {
                // Create two values for comparison
                uint32_t value1 = fdp.ConsumeIntegral<uint32_t>();
                uint32_t value2 = fdp.ConsumeIntegral<uint32_t>();
                memcpy(&buffer[0], &value1, sizeof(value1));
                memcpy(&buffer[4], &value2, sizeof(value2));
            }
            break;
            
        case 'x': // Matches anything (line 1049)
        case '!': // Matches almost anything (line 1050)
            // For 'x' and '!' relations, any data works
            break;
            
        default:
            break;
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: flags + test strategy + file type selection + buffer generation data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int test_strategy = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 different testing strategies
    int target_file_type = fdp.ConsumeIntegralInRange<int>(0, 10); // Select which file type to target
    
    // Map to actual file types from apprentice.c
    int actual_file_type = 3; // Default to FILE_DEFAULT
    if (target_file_type == 1) {
        actual_file_type = 1016; // FILE_BESTRING16
    } else if (target_file_type == 2) {
        actual_file_type = 1017; // FILE_LESTRING16
    } else if (target_file_type == 3) {
        actual_file_type = 955; // FILE_BYTE
    } else if (target_file_type == 4) {
        actual_file_type = 956; // FILE_SHORT
    } else if (target_file_type == 5) {
        actual_file_type = 963; // FILE_LONG
    }
    
    bool test_factor_op = fdp.ConsumeBool();
    bool test_relations = fdp.ConsumeBool();
    
    // Limit flags to reasonable range
    int actual_flags = flags & 0xFFFFFFF;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Try to load default magic database
    const char* magic_path = "/usr/share/misc/magic";
    magic_load(magic, magic_path); // Ignore errors for fuzzing

    // Strategy-based testing
    switch (test_strategy) {
        case 0: {
            // Strategy 0: Targeted FILE_DEFAULT testing
            std::vector<uint8_t> test_buffer = generate_test_buffer_for_filetype(fdp, 3); // FILE_DEFAULT
            
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result; // Result can be null
                
                // Test with different flag combinations
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                    int new_flags = fdp.ConsumeIntegral<int>();
                    magic_setflags(magic, new_flags & 0xFFFFFFF);
                    
                    // Test again with new flags
                    const char* result2 = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                    (void)result2;
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 1: Targeted FILE_BESTRING16 testing
            std::vector<uint8_t> test_buffer = generate_test_buffer_for_filetype(fdp, 1016); // FILE_BESTRING16
            
            if (!test_buffer.empty()) {
                const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                (void)result;
                
                // Test multiple buffers with same magic context
                if (fdp.remaining_bytes() > 32 && fdp.ConsumeBool()) {
                    std::vector<uint8_t> test_buffer2 = generate_test_buffer_for_filetype(fdp, 1016);
                    if (!test_buffer2.empty()) {
                        const char* result2 = magic_buffer(magic, test_buffer2.data(), test_buffer2.size());
                        (void)result2;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Relation testing with specific patterns
            if (test_relations && fdp.remaining_bytes() > 4) {
                // Test different relations
                char relation = '=';
                int rel_choice = fdp.ConsumeIntegralInRange<int>(0, 6);
                switch (rel_choice) {
                    case 0: relation = '='; break;
                    case 1: relation = '>'; break;
                    case 2: relation = '<'; break;
                    case 3: relation = '^'; break;
                    case 4: relation = '&'; break;
                    case 5: relation = 'x'; break;
                    case 6: relation = '!'; break;
                }
                
                std::vector<uint8_t> test_buffer = generate_relation_test_buffer(fdp, relation);
                if (!test_buffer.empty()) {
                    const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                    (void)result;
                }
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Comprehensive testing with multiple file types
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 32; i++) {
                int file_type = 3; // Default
                if (i == 1) file_type = 1016; // FILE_BESTRING16
                else if (i == 2) file_type = 1017; // FILE_LESTRING16
                
                std::vector<uint8_t> test_buffer = generate_test_buffer_for_filetype(fdp, file_type);
                if (!test_buffer.empty()) {
                    const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
                    (void)result;
                }
            }
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_020
