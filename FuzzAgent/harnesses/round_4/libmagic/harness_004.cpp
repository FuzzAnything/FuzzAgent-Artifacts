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

namespace harness_004 {
/*
 * Fuzzing harness for libmagic library - Advanced Magic Pattern Testing
 * Targets: Complex softmagic pattern matching with indirect operations
 * Specifically exercises: softmagic.c:mget, magiccheck, match, mprint functions
 * Focuses on indirect operations, type conversions, and boundary conditions in softmagic engine
 * Uses FuzzedDataProvider to create diverse test buffers with nested indirection chains
 * Configure MAGIC_PARAM_INDIR_MAX and MAGIC_PARAM_NAME_MAX via magic_setparam
 */


// Include libmagic header

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on debugging flags for better coverage
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging
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
    
    // Configure indirection and name limits via magic_setparam
    // This targets the blocked branches in mget: indir_count >= ms->indir_max and name_count >= ms->name_max
    
    // Configure MAGIC_PARAM_INDIR_MAX (0) - controls maximum indirection depth
    size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(1, 10);  // Keep small to trigger limits
    int set_indir_result = magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
    (void)set_indir_result; // Don't check result - invalid parameters will return -1, that's OK
    
    // Configure MAGIC_PARAM_NAME_MAX (1) - controls maximum name count
    size_t name_max = fdp.ConsumeIntegralInRange<size_t>(1, 10);   // Keep small to trigger limits
    int set_name_result = magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
    (void)set_name_result; // Don't check result
    
    // Configure other parameters that might affect softmagic matching
    // MAGIC_PARAM_BYTES_MAX (6) - controls maximum bytes to check
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
    
    // Consume operation type to test different advanced pattern scenarios
    // 0 = Mixed-endian data structures, 1 = Boundary offset testing
    // 2 = Nested indirection chains, 3 = Type conversion edge cases
    // 4 = Complex magic pattern simulation
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume buffer generation strategy
    uint8_t buffer_strategy = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    std::vector<uint8_t> test_buffer;
    
    switch (buffer_strategy) {
        case 0: {
            // Strategy 1: Create mixed-endian data structures
            // Build buffers with alternating big-endian and little-endian patterns
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(32, 1024);
            if (fdp.remaining_bytes() < buffer_size) {
                buffer_size = fdp.remaining_bytes();
            }
            
            test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Manipulate buffer to create mixed-endian patterns
            if (test_buffer.size() >= 8) {
                // Create some big-endian 32-bit values
                for (size_t i = 0; i + 4 <= test_buffer.size(); i += 8) {
                    // Swap bytes to create big-endian pattern
                    if (i + 4 < test_buffer.size()) {
                        std::swap(test_buffer[i], test_buffer[i + 3]);
                        std::swap(test_buffer[i + 1], test_buffer[i + 2]);
                    }
                }
                
                // Create some little-endian 16-bit values
                for (size_t i = 4; i + 2 <= test_buffer.size(); i += 8) {
                    if (i + 2 < test_buffer.size()) {
                        std::swap(test_buffer[i], test_buffer[i + 1]);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 2: Boundary offset testing
            // Create buffers with data near boundaries to test offset checks
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
            if (fdp.remaining_bytes() < buffer_size) {
                buffer_size = fdp.remaining_bytes();
            }
            
            test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Insert pattern headers at various offsets
            if (test_buffer.size() >= 16) {
                // Create ELF-like header at offset 0
                if (test_buffer.size() > 0) test_buffer[0] = 0x7F;
                if (test_buffer.size() > 1) test_buffer[1] = 'E';
                if (test_buffer.size() > 2) test_buffer[2] = 'L';
                if (test_buffer.size() > 3) test_buffer[3] = 'F';
                
                // Create PDF-like header near end of buffer
                if (test_buffer.size() > 8) {
                    size_t pdf_offset = test_buffer.size() - 8;
                    if (pdf_offset + 4 < test_buffer.size()) {
                        test_buffer[pdf_offset] = '%';
                        test_buffer[pdf_offset + 1] = 'P';
                        test_buffer[pdf_offset + 2] = 'D';
                        test_buffer[pdf_offset + 3] = 'F';
                    }
                }
                
                // Create ZIP-like header at offset 0
                if (test_buffer.size() > 0) test_buffer[0] = 0x50; // 'P'
                if (test_buffer.size() > 1) test_buffer[1] = 0x4B; // 'K'
                
                // Create nested offset patterns
                for (size_t i = 8; i + 8 <= test_buffer.size(); i += 16) {
                    // Write offset values that point to other parts of buffer
                    uint32_t offset_value = static_cast<uint32_t>(i % test_buffer.size());
                    if (i + 4 < test_buffer.size()) {
                        test_buffer[i] = (offset_value >> 24) & 0xFF;
                        test_buffer[i + 1] = (offset_value >> 16) & 0xFF;
                        test_buffer[i + 2] = (offset_value >> 8) & 0xFF;
                        test_buffer[i + 3] = offset_value & 0xFF;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 3: Nested indirection chains
            // Create buffers with multiple levels of indirection
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
            if (fdp.remaining_bytes() < buffer_size) {
                buffer_size = fdp.remaining_bytes();
            }
            
            test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Create a chain of indirection pointers
            if (test_buffer.size() >= 32) {
                // Level 1 indirection: offset at byte 0 points to byte 16
                if (test_buffer.size() > 3) {
                    test_buffer[0] = 0x00;
                    test_buffer[1] = 0x00;
                    test_buffer[2] = 0x00;
                    test_buffer[3] = 0x10; // Points to offset 16
                }
                
                // Level 2 indirection: offset at byte 16 points to byte 32
                if (test_buffer.size() > 19) {
                    test_buffer[16] = 0x00;
                    test_buffer[17] = 0x00;
                    test_buffer[18] = 0x00;
                    test_buffer[19] = 0x20; // Points to offset 32
                }
                
                // Level 3 indirection: offset at byte 32 points to byte 48
                if (test_buffer.size() > 35) {
                    test_buffer[32] = 0x00;
                    test_buffer[33] = 0x00;
                    test_buffer[34] = 0x00;
                    test_buffer[35] = 0x30; // Points to offset 48
                }
                
                // Add type markers at indirection targets
                if (test_buffer.size() > 48) test_buffer[48] = 0xCA; // Marker
                if (test_buffer.size() > 49) test_buffer[49] = 0xFE;
                if (test_buffer.size() > 50) test_buffer[50] = 0xBA;
                if (test_buffer.size() > 51) test_buffer[51] = 0xBE;
            }
            break;
        }
        
        case 3: {
            // Strategy 4: Type conversion edge cases
            // Create buffers with values that test signed/unsigned conversions
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(48, 512);
            if (fdp.remaining_bytes() < buffer_size) {
                buffer_size = fdp.remaining_bytes();
            }
            
            test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Insert values that test type conversion edge cases
            if (test_buffer.size() >= 16) {
                // Signed byte boundary values
                test_buffer[0] = 0x7F;  // Max positive signed char
                test_buffer[1] = 0x80;  // Min negative signed char
                test_buffer[2] = 0xFF;  // -1 as signed char
                
                // Signed short boundary values (little-endian)
                if (test_buffer.size() >= 8) {
                    test_buffer[4] = 0xFF;  // -1 as signed short (LSB)
                    test_buffer[5] = 0xFF;  // -1 as signed short (MSB)
                    test_buffer[6] = 0x7F;  // Max positive (LSB)
                    test_buffer[7] = 0xFF;  // Max positive (MSB)
                }
                
                // Signed long boundary values (big-endian)
                if (test_buffer.size() >= 16) {
                    test_buffer[8] = 0x7F;   // Max positive (MSB)
                    test_buffer[9] = 0xFF;
                    test_buffer[10] = 0xFF;
                    test_buffer[11] = 0xFF;  // Max positive (LSB)
                    
                    test_buffer[12] = 0x80;  // Min negative (MSB)
                    test_buffer[13] = 0x00;
                    test_buffer[14] = 0x00;
                    test_buffer[15] = 0x00;  // Min negative (LSB)
                }
            }
            break;
        }
    }
    
    // If no buffer was created (shouldn't happen), use remaining bytes
    if (test_buffer.empty() && fdp.remaining_bytes() > 0) {
        test_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    
    // Test the buffer with magic_buffer
    if (!test_buffer.empty()) {
        // Call magic_buffer multiple times with different operations
        // to increase chance of hitting different code paths
        
        // First call with original buffer
        const char* result1 = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        (void)result1;
        
        // If buffer is large enough, test sub-sections to hit different offsets
        if (test_buffer.size() > 16) {
            // Test middle section
            size_t mid_offset = test_buffer.size() / 4;
            size_t mid_size = test_buffer.size() / 2;
            if (mid_offset + mid_size <= test_buffer.size()) {
                const char* result2 = magic_buffer(magic, test_buffer.data() + mid_offset, mid_size);
                (void)result2;
            }
            
            // Test end section (boundary condition)
            size_t end_offset = test_buffer.size() - std::min(test_buffer.size() / 4, static_cast<size_t>(64));
            size_t end_size = test_buffer.size() - end_offset;
            if (end_offset + end_size <= test_buffer.size()) {
                const char* result3 = magic_buffer(magic, test_buffer.data() + end_offset, end_size);
                (void)result3;
            }
        }
        
        // Test with different flags if we have MAGIC_CONTINUE
        if (flags & MAGIC_CONTINUE) {
            // Already using continue flag
        } else {
            // Test with temporary magic handle with continue flag
            magic_t magic2 = magic_open(flags | MAGIC_CONTINUE);
            if (magic2 != NULL) {
                if (magic_load(magic2, NULL) != -1) {
                    const char* result4 = magic_buffer(magic2, test_buffer.data(), test_buffer.size());
                    (void)result4;
                }
                magic_close(magic2);
            }
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_004
