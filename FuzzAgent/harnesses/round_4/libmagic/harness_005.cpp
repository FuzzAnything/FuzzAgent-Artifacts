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

namespace harness_005 {
/*
 * Fuzzing harness for libmagic library - Deep Softmagic Recursion Testing
 * Targets: mget function in softmagic.c with specific focus on indirection limits
 * Goal: Trigger the blocked branches in mget (191 out of 390) by exceeding indir_max
 * Strategy: Create deeply nested indirect patterns with complex offset calculations
 * Differentiated from harness_004: More aggressive indirection depth, dynamic pattern
 * generation from fuzzer input, testing edge cases for all indirect operation types
 */


// Include libmagic header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex indirection patterns
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on flags that affect softmagic
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging for better coverage
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;       // Return all matches
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // CRITICAL: Set indir_max to very low values (1-5) to force hitting recursion limits
    // This targets the primary blocked branch in mget: if (*indir_count >= ms->indir_max)
    size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(1, 5);  // Very low to trigger limits
    int set_indir_result = magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
    (void)set_indir_result; // Don't check result
    
    // Also set name_max low to potentially trigger name_count limit
    size_t name_max = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    int set_name_result = magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
    (void)set_name_result;
    
    // Set bytes_max to control how much data is examined
    size_t bytes_max = fdp.ConsumeIntegralInRange<size_t>(100, 10000);
    int set_bytes_result = magic_setparam(magic, MAGIC_PARAM_BYTES_MAX, &bytes_max);
    (void)set_bytes_result;
    
    // Try to load magic database - use default path
    const char* magic_path = NULL; // NULL means use default
    int load_result = magic_load(magic, magic_path);
    
    // If loading fails, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Determine test strategy based on fuzzer input
    // 0 = Deep nested indirection chains
    // 1 = Mixed data types with different endianness  
    // 2 = Complex offset calculations
    // 3 = Recursive pattern simulation
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Determine buffer size
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < buffer_size) {
        buffer_size = fdp.remaining_bytes();
    }
    
    // Create test buffer
    std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Apply strategy-specific modifications to create complex patterns
    switch (strategy) {
        case 0: {
            // Deep nested indirection chains
            // Create multiple levels of pointer-like structures
            if (test_buffer.size() >= 32) {
                // Determine indirection depth (2-10 levels)
                size_t depth = fdp.ConsumeIntegralInRange<size_t>(2, 10);
                
                // Create chain of offsets pointing to each other
                for (size_t level = 0; level < depth && (level * 4 + 4) < test_buffer.size(); level++) {
                    // Calculate next offset (points to next level or wraps)
                    size_t next_offset = (level + 1) * 4;
                    if (next_offset >= test_buffer.size()) {
                        next_offset = test_buffer.size() - 4;
                    }
                    
                    // Write offset as 32-bit value (big-endian for variety)
                    size_t current_pos = level * 4;
                    test_buffer[current_pos] = (next_offset >> 24) & 0xFF;
                    test_buffer[current_pos + 1] = (next_offset >> 16) & 0xFF;
                    test_buffer[current_pos + 2] = (next_offset >> 8) & 0xFF;
                    test_buffer[current_pos + 3] = next_offset & 0xFF;
                    
                    // At target offset, write a pattern that might match magic entries
                    if (next_offset + 8 < test_buffer.size()) {
                        // Common magic pattern markers
                        test_buffer[next_offset] = 0x7F;
                        test_buffer[next_offset + 1] = 'E';
                        test_buffer[next_offset + 2] = 'L';
                        test_buffer[next_offset + 3] = 'F';
                        
                        // Another common pattern
                        test_buffer[next_offset + 4] = 0x25; // '%'
                        test_buffer[next_offset + 5] = 'P';
                        test_buffer[next_offset + 6] = 'D';
                        test_buffer[next_offset + 7] = 'F';
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Mixed data types with different endianness
            // Create buffer with various data type representations
            if (test_buffer.size() >= 64) {
                // Byte, short, long, quad types with both endianness
                size_t pos = 0;
                
                // Byte values (various signed/unsigned boundary cases)
                if (pos < test_buffer.size()) test_buffer[pos++] = 0x00;
                if (pos < test_buffer.size()) test_buffer[pos++] = 0x7F; // max positive signed
                if (pos < test_buffer.size()) test_buffer[pos++] = 0x80; // min negative signed  
                if (pos < test_buffer.size()) test_buffer[pos++] = 0xFF; // -1 signed / 255 unsigned
                
                // Little-endian 16-bit values
                if (pos + 1 < test_buffer.size()) {
                    uint16_t le_short = fdp.ConsumeIntegral<uint16_t>();
                    test_buffer[pos++] = le_short & 0xFF;
                    test_buffer[pos++] = (le_short >> 8) & 0xFF;
                }
                
                // Big-endian 16-bit values  
                if (pos + 1 < test_buffer.size()) {
                    uint16_t be_short = fdp.ConsumeIntegral<uint16_t>();
                    test_buffer[pos++] = (be_short >> 8) & 0xFF;
                    test_buffer[pos++] = be_short & 0xFF;
                }
                
                // Little-endian 32-bit values
                if (pos + 3 < test_buffer.size()) {
                    uint32_t le_long = fdp.ConsumeIntegral<uint32_t>();
                    test_buffer[pos++] = le_long & 0xFF;
                    test_buffer[pos++] = (le_long >> 8) & 0xFF;
                    test_buffer[pos++] = (le_long >> 16) & 0xFF;
                    test_buffer[pos++] = (le_long >> 24) & 0xFF;
                }
                
                // Big-endian 32-bit values
                if (pos + 3 < test_buffer.size()) {
                    uint32_t be_long = fdp.ConsumeIntegral<uint32_t>();
                    test_buffer[pos++] = (be_long >> 24) & 0xFF;
                    test_buffer[pos++] = (be_long >> 16) & 0xFF;
                    test_buffer[pos++] = (be_long >> 8) & 0xFF;
                    test_buffer[pos++] = be_long & 0xFF;
                }
                
                // Mixed endianness patterns
                if (pos + 7 < test_buffer.size()) {
                    // Create pattern: LE16 followed by BE16 followed by LE32
                    test_buffer[pos++] = 0x34; test_buffer[pos++] = 0x12; // LE: 0x1234
                    test_buffer[pos++] = 0x12; test_buffer[pos++] = 0x34; // BE: 0x1234
                    test_buffer[pos++] = 0x78; test_buffer[pos++] = 0x56; // LE32: 0x...
                    test_buffer[pos++] = 0x34; test_buffer[pos++] = 0x12; // ...0x12345678
                }
            }
            break;
        }
        
        case 2: {
            // Complex offset calculations
            // Create buffer with offset values that require arithmetic operations
            if (test_buffer.size() >= 48) {
                // Write base offset values at various positions
                for (size_t i = 0; i + 8 < test_buffer.size() && i < 32; i += 8) {
                    // Consume offset and operation type
                    uint32_t offset = fdp.ConsumeIntegral<uint32_t>() % test_buffer.size();
                    uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                    
                    // Write offset value
                    test_buffer[i] = (offset >> 24) & 0xFF;
                    test_buffer[i + 1] = (offset >> 16) & 0xFF;
                    test_buffer[i + 2] = (offset >> 8) & 0xFF;
                    test_buffer[i + 3] = offset & 0xFF;
                    
                    // Write operation type (simulating FILE_OP* values)
                    test_buffer[i + 4] = op_type;
                    
                    // Write operand for operation
                    uint32_t operand = fdp.ConsumeIntegral<uint32_t>() % 256;
                    test_buffer[i + 5] = operand & 0xFF;
                    if (i + 6 < test_buffer.size()) test_buffer[i + 6] = (operand >> 8) & 0xFF;
                    if (i + 7 < test_buffer.size()) test_buffer[i + 7] = (operand >> 16) & 0xFF;
                }
                
                // Add known file signatures at calculated offsets
                if (test_buffer.size() > 16) {
                    // ELF signature at offset 0
                    test_buffer[0] = 0x7F; test_buffer[1] = 'E'; test_buffer[2] = 'L'; test_buffer[3] = 'F';
                    
                    // PDF signature somewhere in buffer
                    size_t pdf_pos = test_buffer.size() / 2;
                    if (pdf_pos + 4 < test_buffer.size()) {
                        test_buffer[pdf_pos] = '%'; test_buffer[pdf_pos + 1] = 'P';
                        test_buffer[pdf_pos + 2] = 'D'; test_buffer[pdf_pos + 3] = 'F';
                    }
                    
                    // ZIP signature
                    if (test_buffer.size() > 2) {
                        test_buffer[test_buffer.size() - 10] = 'P'; test_buffer[test_buffer.size() - 9] = 'K';
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Recursive pattern simulation
            // Create buffer with self-referential structures
            if (test_buffer.size() >= 128) {
                // Create header with magic number
                test_buffer[0] = 0xCA; test_buffer[1] = 0xFE; test_buffer[2] = 0xBA; test_buffer[3] = 0xBE;
                
                // Create recursive structure: offset at position 4 points to position 20
                // which contains offset pointing back or forward
                size_t offset1 = 20;
                test_buffer[4] = (offset1 >> 24) & 0xFF;
                test_buffer[5] = (offset1 >> 16) & 0xFF;
                test_buffer[6] = (offset1 >> 8) & 0xFF;
                test_buffer[7] = offset1 & 0xFF;
                
                // At offset 20, create another offset
                if (offset1 + 4 < test_buffer.size()) {
                    size_t offset2 = 40;
                    test_buffer[offset1] = (offset2 >> 24) & 0xFF;
                    test_buffer[offset1 + 1] = (offset2 >> 16) & 0xFF;
                    test_buffer[offset1 + 2] = (offset2 >> 8) & 0xFF;
                    test_buffer[offset1 + 3] = offset2 & 0xFF;
                    
                    // At offset 40, create yet another offset (could point back to create cycle)
                    if (offset2 + 4 < test_buffer.size()) {
                        size_t offset3 = 60;
                        test_buffer[offset2] = (offset3 >> 24) & 0xFF;
                        test_buffer[offset2 + 1] = (offset3 >> 16) & 0xFF;
                        test_buffer[offset2 + 2] = (offset3 >> 8) & 0xFF;
                        test_buffer[offset2 + 3] = offset3 & 0xFF;
                        
                        // At offset 60, create final data
                        if (offset3 + 8 < test_buffer.size()) {
                            test_buffer[offset3] = 0x89; test_buffer[offset3 + 1] = 'P';
                            test_buffer[offset3 + 2] = 'N'; test_buffer[offset3 + 3] = 'G';
                        }
                    }
                }
                
                // Fill remaining buffer with random pattern data
                for (size_t i = 80; i + 2 < test_buffer.size(); i += 2) {
                    uint16_t pattern = fdp.ConsumeIntegral<uint16_t>();
                    test_buffer[i] = pattern & 0xFF;
                    if (i + 1 < test_buffer.size()) test_buffer[i + 1] = (pattern >> 8) & 0xFF;
                }
            }
            break;
        }
    }
    
    // If buffer is empty (shouldn't happen), use remaining bytes
    if (test_buffer.empty() && fdp.remaining_bytes() > 0) {
        test_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    
    // CRITICAL: Test the buffer multiple times with different approaches
    // to maximize chance of hitting the indirection limit in mget
    if (!test_buffer.empty()) {
        // Test 1: Full buffer
        const char* result1 = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        (void)result1;
        
        // Test 2: Different starting offsets to test boundary conditions
        if (test_buffer.size() > 32) {
            // Test from offset 8
            size_t offset1 = 8;
            if (offset1 < test_buffer.size()) {
                size_t size1 = test_buffer.size() - offset1;
                const char* result2 = magic_buffer(magic, test_buffer.data() + offset1, size1);
                (void)result2;
            }
            
            // Test from offset 16  
            size_t offset2 = 16;
            if (offset2 < test_buffer.size()) {
                size_t size2 = test_buffer.size() - offset2;
                const char* result3 = magic_buffer(magic, test_buffer.data() + offset2, size2);
                (void)result3;
            }
            
            // Test middle section
            size_t mid_offset = test_buffer.size() / 3;
            size_t mid_size = test_buffer.size() / 3;
            if (mid_offset + mid_size <= test_buffer.size()) {
                const char* result4 = magic_buffer(magic, test_buffer.data() + mid_offset, mid_size);
                (void)result4;
            }
        }
        
        // Test 3: Small chunks to test granular pattern matching
        if (test_buffer.size() > 64) {
            for (size_t chunk = 0; chunk < 4 && (chunk * 16) < test_buffer.size(); chunk++) {
                size_t chunk_offset = chunk * 16;
                size_t chunk_size = 16;
                if (chunk_offset + chunk_size > test_buffer.size()) {
                    chunk_size = test_buffer.size() - chunk_offset;
                }
                const char* result = magic_buffer(magic, test_buffer.data() + chunk_offset, chunk_size);
                (void)result;
            }
        }
    }
    
    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_005
