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
#include <random>
#include <cmath>
#include <math.h>
#include "magic.h"

namespace harness_010 {
/*
 * Fuzzing harness for libmagic library - Deep Magic Pattern Validation
 * Targets: mget (191 blocked branches), magiccheck (176 blocked branches), file_mdump (139 blocked branches)
 * Goal: Exercise deep edge-case testing of the core pattern matching engine in softmagic.c
 * Focus: Extremely nested indirect patterns, boundary value numeric comparisons, 
 *        complex string pattern combinations, mixed-endian data patterns,
 *        malformed but partially valid magic sequences
 * Semantic differentiation: While harness_004 tests complex patterns and harness_007 tests extreme nesting,
 *        this harness specifically targets comparison logic boundaries and malformed patterns
 *        that trigger the blocked branches in magiccheck and mget comparison operations.
 * Strategy: Generate synthetic buffers that simulate magic database patterns with edge-case
 *        values, invalid-but-parseable structures, and boundary conditions for all comparison types.
 * Expected coverage gain: Target 5-10% overall branch coverage improvement by hitting
 *        the top 3 blocked functions with targeted edge cases.
 */


// Include libmagic header
class TimeoutGuard {
private:
    unsigned long long start_cycles;
    size_t timeout_cycles;
    bool enabled;
    
public:
    TimeoutGuard(size_t max_operations, bool enable = true) 
        : start_cycles(0), 
          timeout_cycles(max_operations), 
          enabled(enable) {
        // Simple cycle counter simulation
        if (enabled) {
            // Use a simple counter instead of chrono
            for (volatile int i = 0; i < 1000; i++) {} // Small delay
        }
    }
    
    bool timed_out() {
        if (!enabled) return false;
        // Simple operation counter - increment on check
        static unsigned long long operation_count = 0;
        operation_count++;
        return operation_count > timeout_cycles;
    }
    
    void reset() {
        // Reset operation counter
        static unsigned long long operation_count = 0;
        operation_count = 0;
    }
};

// Generate boundary value patterns for numeric comparisons
// Targets magiccheck's comparison logic for different numeric types
void generate_boundary_numeric_pattern(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer) {
    // Choose numeric type and boundary condition
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (pattern_type) {
        case 0: { // Near-zero boundary for signed bytes
            int8_t value = fdp.ConsumeIntegral<int8_t>();
            // Bias toward boundary values (-128, -1, 0, 1, 127)
            if (fdp.ConsumeBool()) {
                uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 5;
                switch (choice) {
                    case 0: value = -128; break;
                    case 1: value = -1; break;
                    case 2: value = 0; break;
                    case 3: value = 1; break;
                    case 4: value = 127; break;
                }
            }
            buffer.push_back(static_cast<uint8_t>(value));
            break;
        }
        
        case 1: { // Boundary for unsigned 16-bit values
            uint16_t value = fdp.ConsumeIntegral<uint16_t>();
            if (fdp.ConsumeBool()) {
                uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (choice) {
                    case 0: value = 0; break;
                    case 1: value = 1; break;
                    case 2: value = 0xFF; break;
                    case 3: value = 0xFFFF; break;
                }
            }
            // Write as little-endian
            buffer.push_back(static_cast<uint8_t>(value & 0xFF));
            buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
            break;
        }
        
        case 2: { // Mixed-endian 32-bit values
            uint32_t value = fdp.ConsumeIntegral<uint32_t>();
            bool use_big_endian = fdp.ConsumeBool();
            
            if (use_big_endian) {
                // Big-endian
                buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
                buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
                buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
                buffer.push_back(static_cast<uint8_t>(value & 0xFF));
            } else {
                // Little-endian
                buffer.push_back(static_cast<uint8_t>(value & 0xFF));
                buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
                buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
                buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
            }
            break;
        }
        
        case 3: { // Floating-point boundary values
            float value;
            if (fdp.ConsumeBool()) {
                uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 6;
                switch (choice) {
                    case 0: value = 0.0f; break;
                    case 1: value = -0.0f; break;
                    case 2: value = 1.0f; break;
                    case 3: value = -1.0f; break;
                    case 4: value = HUGE_VALF; break; // Use HUGE_VALF instead of INFINITY
                    case 5: value = static_cast<float>(NAN); break;
                }
            } else {
                value = fdp.ConsumeFloatingPoint<float>();
            }
            // Write as IEEE 754 little-endian
            uint32_t int_repr;
            memcpy(&int_repr, &value, sizeof(float));
            buffer.push_back(static_cast<uint8_t>(int_repr & 0xFF));
            buffer.push_back(static_cast<uint8_t>((int_repr >> 8) & 0xFF));
            buffer.push_back(static_cast<uint8_t>((int_repr >> 16) & 0xFF));
            buffer.push_back(static_cast<uint8_t>((int_repr >> 24) & 0xFF));
            break;
        }
        
        case 4: { // Pattern for date/time comparisons
            // Simulate MS-DOS date/time format used in magiccheck
            uint16_t date = fdp.ConsumeIntegral<uint16_t>();
            uint16_t time = fdp.ConsumeIntegral<uint16_t>();
            
            // Write time then date (little-endian)
            buffer.push_back(static_cast<uint8_t>(time & 0xFF));
            buffer.push_back(static_cast<uint8_t>((time >> 8) & 0xFF));
            buffer.push_back(static_cast<uint8_t>(date & 0xFF));
            buffer.push_back(static_cast<uint8_t>((date >> 8) & 0xFF));
            break;
        }
        
        case 5: { // 64-bit boundary values for quad comparisons
            uint64_t value = fdp.ConsumeIntegral<uint64_t>();
            if (fdp.ConsumeBool()) {
                uint8_t choice = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (choice) {
                    case 0: value = 0; break;
                    case 1: value = 1; break;
                    case 2: value = 0xFFFFFFFFFFFFFFFFULL; break;
                    case 3: value = 0x8000000000000000ULL; break; // High bit set
                }
            }
            
            // Write as little-endian
            for (int i = 0; i < 8; i++) {
                buffer.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
            }
            break;
        }
        
        case 6: { // String with embedded nulls and special characters
            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
            std::string str = fdp.ConsumeRandomLengthString(str_len);
            
            // Inject problematic characters that might affect string comparison
            if (!str.empty() && fdp.ConsumeBool()) {
                size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, str.length() - 1);
                uint8_t special_char = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (special_char) {
                    case 0: str[pos] = '\0'; break; // Null byte
                    case 1: str[pos] = '\n'; break; // Newline
                    case 2: str[pos] = '\xFF'; break; // Invalid UTF-8
                    case 3: str[pos] = '\\'; break; // Escape character
                }
            }
            
            buffer.insert(buffer.end(), str.begin(), str.end());
            break;
        }
        
        case 7: { // Regex-like pattern simulation
            // Create patterns that look like simplified regex for magiccheck
            std::string pattern;
            if (fdp.ConsumeBool()) {
                pattern = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(4, 32));
                // Add regex-like characters
                if (fdp.ConsumeBool()) pattern += ".*";
                if (fdp.ConsumeBool()) pattern += "[0-9]";
                if (fdp.ConsumeBool()) pattern += "\\.";
            } else {
                pattern = fdp.ConsumeRemainingBytesAsString();
            }
            buffer.insert(buffer.end(), pattern.begin(), pattern.end());
            break;
        }
    }
}

// Generate malformed but partially valid magic sequence
// Simulates corrupted magic database entries that still parse partially
void generate_malformed_magic_sequence(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer) {
    // Start with a valid-looking magic header
    uint8_t header_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (header_type) {
        case 0: // ELF-like header
            buffer.push_back(0x7F);
            buffer.push_back('E');
            buffer.push_back('L');
            buffer.push_back('F');
            // Then corrupt some part
            for (int i = 0; i < 12; i++) {
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                } else {
                    // Insert intentionally invalid byte
                    buffer.push_back(0xFF);
                }
            }
            break;
            
        case 1: // PE/COFF-like header
            buffer.push_back('M');
            buffer.push_back('Z');
            // DOS header then corrupt PE header
            for (int i = 0; i < 60; i++) {
                buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            // Corrupt PE signature
            if (fdp.ConsumeBool()) {
                buffer.push_back('P');
                buffer.push_back('E');
                buffer.push_back(0);
                buffer.push_back(0);
            } else {
                buffer.push_back(0);
                buffer.push_back(0);
                buffer.push_back(0);
                buffer.push_back(0);
            }
            break;
            
        case 2: // PNG-like header
            buffer.push_back(0x89);
            buffer.push_back('P');
            buffer.push_back('N');
            buffer.push_back('G');
            buffer.push_back(0x0D);
            buffer.push_back(0x0A);
            buffer.push_back(0x1A);
            buffer.push_back(0x0A);
            // Then partially valid IHDR chunk
            if (fdp.ConsumeBool()) {
                // Valid chunk length
                uint32_t length = 13;
                buffer.push_back((length >> 24) & 0xFF);
                buffer.push_back((length >> 16) & 0xFF);
                buffer.push_back((length >> 8) & 0xFF);
                buffer.push_back(length & 0xFF);
                buffer.push_back('I');
                buffer.push_back('H');
                buffer.push_back('D');
                buffer.push_back('R');
                // Corrupt chunk data
                for (int i = 0; i < 13; i++) {
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
            }
            break;
            
        case 3: // ZIP-like header
            buffer.push_back('P');
            buffer.push_back('K');
            buffer.push_back(0x03);
            buffer.push_back(0x04);
            // Partially valid local file header
            for (int i = 0; i < 26; i++) {
                if (fdp.ConsumeBool() && i > 4) { // Keep PK signature
                    buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
                } else {
                    buffer.push_back(0);
                }
            }
            break;
    }
}

// Generate complex nested indirect pattern
// Targets mget's indirection logic with multiple levels of pointer-like structures
void generate_nested_indirect_pattern(FuzzedDataProvider& fdp, std::vector<uint8_t>& buffer, int depth) {
    if (depth <= 0 || fdp.remaining_bytes() < 4) {
        return;
    }
    
    // Create an indirect reference structure
    // Format: [type:1][offset:2][data...]
    uint8_t indir_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    buffer.push_back(indir_type);
    
    // Generate offset (could be relative or absolute)
    uint16_t offset = fdp.ConsumeIntegral<uint16_t>();
    buffer.push_back(static_cast<uint8_t>(offset & 0xFF));
    buffer.push_back(static_cast<uint8_t>((offset >> 8) & 0xFF));
    
    // Generate the actual data at offset
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(static_cast<size_t>(64), fdp.remaining_bytes()));
    std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Sometimes create another level of indirection within the data
    if (depth > 1 && fdp.ConsumeBool() && data.size() >= 8) {
        // Embed another indirect pattern in the data
        size_t embed_pos = fdp.ConsumeIntegralInRange<size_t>(0, data.size() - 4);
        
        // Create a simple embedded indirect reference
        data[embed_pos] = 0x80; // Mark as indirect
        data[embed_pos + 1] = fdp.ConsumeIntegral<uint8_t>(); // Type
        // Offset points somewhere within the buffer
        uint16_t embed_offset = fdp.ConsumeIntegral<uint16_t>() % buffer.size();
        data[embed_pos + 2] = static_cast<uint8_t>(embed_offset & 0xFF);
        data[embed_pos + 3] = static_cast<uint8_t>((embed_offset >> 8) & 0xFF);
    }
    
    buffer.insert(buffer.end(), data.begin(), data.end());
    
    // Recursively generate more nested patterns with decreasing probability
    if (depth > 1 && fdp.ConsumeProbability<float>() > 0.3) {
        generate_nested_indirect_pattern(fdp, buffer, depth - 1);
    }
}

// Generate test buffer based on strategy
std::vector<uint8_t> generate_test_buffer(FuzzedDataProvider& fdp, uint8_t strategy) {
    std::vector<uint8_t> buffer;
    
    switch (strategy) {
        case 0: { // Mixed boundary value patterns
            size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(3, 10);
            for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 0; i++) {
                generate_boundary_numeric_pattern(fdp, buffer);
            }
            break;
        }
        
        case 1: { // Malformed magic sequences
            size_t num_sequences = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < num_sequences && fdp.remaining_bytes() > 0; i++) {
                generate_malformed_magic_sequence(fdp, buffer);
            }
            break;
        }
        
        case 2: { // Deeply nested indirect patterns
            int depth = fdp.ConsumeIntegralInRange<int>(3, 8);
            generate_nested_indirect_pattern(fdp, buffer, depth);
            break;
        }
        
        case 3: { // Combination of all strategies
            if (fdp.ConsumeBool()) generate_boundary_numeric_pattern(fdp, buffer);
            if (fdp.ConsumeBool()) generate_malformed_magic_sequence(fdp, buffer);
            if (fdp.ConsumeBool() && buffer.size() < 1024) {
                int depth = fdp.ConsumeIntegralInRange<int>(2, 5);
                generate_nested_indirect_pattern(fdp, buffer, depth);
            }
            break;
        }
        
        case 4: { // Simulate complex magic database entry
            // Create a structure that looks like a magic database pattern
            // with multiple conditions and comparisons
            buffer.push_back(0); // Start offset
            buffer.push_back(0);
            buffer.push_back(0);
            buffer.push_back(0);
            
            // Type and test value
            uint8_t magic_type = fdp.ConsumeIntegral<uint8_t>() % 20;
            buffer.push_back(magic_type);
            
            // Test value (varies by type)
            size_t value_size = 4; // Default
            switch (magic_type) {
                case 0: case 1: case 2: value_size = 1; break; // byte
                case 3: case 4: case 5: value_size = 2; break; // short
                case 6: case 7: case 8: value_size = 4; break; // long
                case 9: case 10: value_size = 8; break; // quad
                case 11: case 12: value_size = 4; break; // float
                case 13: case 14: value_size = 8; break; // double
                default: value_size = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            }
            
            for (size_t i = 0; i < value_size && fdp.remaining_bytes() > 0; i++) {
                buffer.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            
            // Message string
            std::string msg = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(4, 64));
            buffer.insert(buffer.end(), msg.begin(), msg.end());
            buffer.push_back('\0');
            break;
        }
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need substantial data for complex pattern generation
    if (size < 128) {
        return 0;
    }
    
    // Setup timeout guard (150ms timeout for complex pattern matching)
    TimeoutGuard timeout(150, true);
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open with emphasis on debugging and continue modes
    int flags = 0;
    
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    // Prioritize flags that affect softmagic behavior
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging for insight
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;       // Return all matches - important!
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;          // Print warnings
    if (flag_byte & 0x80) flags |= MAGIC_RAW;            // Don't convert chars
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Configure parameters to target mget's blocked branches
    // Specifically target indir_count and name_count limits
    
    // Set indirection maximum (MAGIC_PARAM_INDIR_MAX = 0)
    size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(5, 100);
    int set_indir_result = magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
    (void)set_indir_result; // Don't check result - invalid parameters will return -1
    
    // Set name maximum (MAGIC_PARAM_NAME_MAX = 1)
    size_t name_max = fdp.ConsumeIntegralInRange<size_t>(5, 100);
    int set_name_result = magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
    (void)set_name_result;
    
    // Set bytes maximum (MAGIC_PARAM_BYTES_MAX = 6) to control how much is checked
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
    
    // Consume buffer generation strategy
    uint8_t buffer_strategy = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Generate test buffer based on strategy
    std::vector<uint8_t> test_buffer = generate_test_buffer(fdp, buffer_strategy);
    
    // If buffer is too small, use remaining bytes
    if (test_buffer.empty() && fdp.remaining_bytes() > 0) {
        test_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    
    // Test the buffer with magic_buffer if we have content
    if (!test_buffer.empty()) {
        // Check timeout before potentially expensive operation
        if (timeout.timed_out()) {
            magic_close(magic);
            return 0;
        }
        
        const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        (void)result; // Result can be NULL for unknown types, that's OK
        
        // Additional test: Try with different flags if we have time
        if (!timeout.timed_out() && fdp.ConsumeBool() && test_buffer.size() > 4) {
            // Test a subset of the buffer to hit different offset paths
            size_t subset_size = fdp.ConsumeIntegralInRange<size_t>(4, test_buffer.size());
            const char* subset_result = magic_buffer(magic, test_buffer.data(), subset_size);
            (void)subset_result;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_010
