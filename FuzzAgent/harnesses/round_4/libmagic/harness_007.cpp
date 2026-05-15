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
#include <chrono>
#include <thread>
#include "magic.h"

namespace harness_007 {
/*
 * Fuzzing harness for libmagic library - Extreme Nested Indirect Pattern Matching
 * Targets: softmagic.c:mget function with specific focus on indir_count and name_count limits
 * Goal: Trigger the 191 blocked branches in mget by creating inputs with extreme indirection depth
 *       and excessive name reuse to exceed default limits (indir_max=50, name_max=50)
 * Strategy: Generate synthetic data with deeply nested pointer-like structures and
 *           complex patterns that cause recursive indirection in softmagic engine
 * Differentiated from harness_004: Focuses on extreme depth to naturally exceed limits
 * Differentiated from harness_005: Uses default/controlled limits, not artificially low ones
 * New semantic domain: "Limit-exceeding pattern matching through natural depth"
 * Performance considerations: Includes timeout mechanisms, incremental complexity
 */


// Include libmagic header

// Timeout mechanism to prevent hangs
class TimeoutGuard {
private:
    std::chrono::steady_clock::time_point start;
    size_t timeout_ms;
    bool enabled;
    
public:
    TimeoutGuard(size_t ms, bool enable = true) 
        : start(std::chrono::steady_clock::now()), 
          timeout_ms(ms), 
          enabled(enable) {}
    
    bool timed_out() {
        if (!enabled) return false;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        return elapsed.count() > static_cast<long>(timeout_ms);
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need substantial data for deep patterns
    if (size < 256) {
        return 0;
    }
    
    // Setup timeout guard (100ms timeout)
    TimeoutGuard timeout(100, true);
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - enable debugging for better coverage insight
    int flags = 0;
    
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
    
    // Configure indirection and name limits strategically
    // Use fuzzer input to determine whether to use default limits or set specific ones
    // This allows testing both natural limit exceeding and controlled boundary testing
    
    uint8_t limit_strategy = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    if (limit_strategy == 0) {
        // Strategy 0: Use default limits (50) - try to exceed naturally
        // Don't set params, use defaults
    } else if (limit_strategy == 1) {
        // Strategy 1: Set moderate limits (20-40) - easier to hit but still requires depth
        size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(20, 40);
        int set_indir_result = magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
        (void)set_indir_result;
        
        size_t name_max = fdp.ConsumeIntegralInRange<size_t>(20, 40);
        int set_name_result = magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
        (void)set_name_result;
    } else {
        // Strategy 2: Set very high limits (100-200) - test deep recursion without limits
        size_t indir_max = fdp.ConsumeIntegralInRange<size_t>(100, 200);
        int set_indir_result = magic_setparam(magic, MAGIC_PARAM_INDIR_MAX, &indir_max);
        (void)set_indir_result;
        
        size_t name_max = fdp.ConsumeIntegralInRange<size_t>(100, 200);
        int set_name_result = magic_setparam(magic, MAGIC_PARAM_NAME_MAX, &name_max);
        (void)set_name_result;
    }
    
    // Set bytes_max to control how much data is examined
    size_t bytes_max = fdp.ConsumeIntegralInRange<size_t>(1000, 100000);
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
    
    // Determine data generation strategy for extreme indirection
    // 0 = Deep pointer chains (simulate ELF/PE structures)
    // 1 = Nested container formats (simulate ZIP/PDF/TAR)
    // 2 = Recursive pattern simulation with self-references
    // 3 = Mixed strategy with incremental depth
    uint8_t data_strategy = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Determine target buffer size
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(512, 65536);
    if (fdp.remaining_bytes() < buffer_size) {
        buffer_size = fdp.remaining_bytes();
    }
    
    // Create base test buffer
    std::vector<uint8_t> test_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    // Apply strategy-specific modifications to create extreme indirection patterns
    switch (data_strategy) {
        case 0: {
            // Strategy 0: Deep pointer chains
            // Create data that looks like it contains pointer structures
            // Simulate ELF/PE executable format with section tables, symbol tables, etc.
            if (test_buffer.size() >= 128) {
                // Create header-like structures at beginning
                uint8_t* buf = test_buffer.data();
                size_t buf_size = test_buffer.size();
                
                // Mark as potential executable (ELF/PE magic numbers)
                if (buf_size >= 4) {
                    // ELF magic: 0x7F 'E' 'L' 'F'
                    buf[0] = 0x7F;
                    buf[1] = 'E';
                    buf[2] = 'L';
                    buf[3] = 'F';
                }
                
                // Create section header pointer chains
                // Simulate ELF section headers pointing to other sections
                size_t section_offset = 64;
                size_t num_sections = fdp.ConsumeIntegralInRange<size_t>(5, 50);
                
                for (size_t i = 0; i < num_sections && section_offset + 64 < buf_size; i++) {
                    // Create a section entry
                    // Simulate sh_offset field pointing to another section
                    uint32_t next_section = section_offset + 64;
                    if (next_section + 64 < buf_size) {
                        // Write little-endian offset
                        buf[section_offset] = next_section & 0xFF;
                        buf[section_offset + 1] = (next_section >> 8) & 0xFF;
                        buf[section_offset + 2] = (next_section >> 16) & 0xFF;
                        buf[section_offset + 3] = (next_section >> 24) & 0xFF;
                    }
                    
                    // Add section type (could be SHT_SYMTAB, SHT_RELA, etc.)
                    // These often cause further indirection
                    uint32_t section_type = fdp.ConsumeIntegral<uint32_t>() % 20;
                    buf[section_offset + 4] = section_type & 0xFF;
                    buf[section_offset + 5] = (section_type >> 8) & 0xFF;
                    buf[section_offset + 6] = (section_type >> 16) & 0xFF;
                    buf[section_offset + 7] = (section_type >> 24) & 0xFF;
                    
                    section_offset += 64;
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 1: Nested container formats
            // Simulate ZIP/TAR/PDF with nested structures
            if (test_buffer.size() >= 256) {
                uint8_t* buf = test_buffer.data();
                size_t buf_size = test_buffer.size();
                
                // Add container signatures
                if (buf_size >= 4) {
                    // ZIP local file header signature
                    buf[0] = 'P';
                    buf[1] = 'K';
                    buf[2] = 0x03;
                    buf[3] = 0x04;
                }
                
                // Create nested offset chains
                // Simulate ZIP central directory pointing to local file headers
                size_t current_offset = 4;
                size_t max_depth = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                
                for (size_t depth = 0; depth < max_depth && current_offset + 16 < buf_size; depth++) {
                    // Create offset to next structure
                    size_t next_offset = current_offset + 16 + (fdp.ConsumeIntegral<uint8_t>() % 64);
                    if (next_offset + 16 >= buf_size) break;
                    
                    // Write offset (little endian)
                    buf[current_offset] = next_offset & 0xFF;
                    buf[current_offset + 1] = (next_offset >> 8) & 0xFF;
                    buf[current_offset + 2] = (next_offset >> 16) & 0xFF;
                    buf[current_offset + 3] = (next_offset >> 24) & 0xFF;
                    
                    // Write compressed size (could be 0 for directories, causing indirection)
                    uint32_t comp_size = fdp.ConsumeIntegral<uint32_t>() % 1024;
                    buf[current_offset + 4] = comp_size & 0xFF;
                    buf[current_offset + 5] = (comp_size >> 8) & 0xFF;
                    buf[current_offset + 6] = (comp_size >> 16) & 0xFF;
                    buf[current_offset + 7] = (comp_size >> 24) & 0xFF;
                    
                    current_offset = next_offset;
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 2: Recursive pattern simulation
            // Create self-referential structures that point back to themselves
            if (test_buffer.size() >= 128) {
                uint8_t* buf = test_buffer.data();
                size_t buf_size = test_buffer.size();
                
                // Create a chain of structures where each points to the next
                // and the last points back to create a cycle
                size_t struct_size = 32;
                size_t num_structs = buf_size / struct_size;
                if (num_structs > 50) num_structs = 50; // Limit for performance
                
                for (size_t i = 0; i < num_structs && (i * struct_size) + 16 < buf_size; i++) {
                    size_t current_pos = i * struct_size;
                    
                    // Pointer to next structure (could be cyclic)
                    size_t next_pos;
                    if (i == num_structs - 1) {
                        // Last points to first (cycle)
                        next_pos = 0;
                    } else {
                        next_pos = (i + 1) * struct_size;
                    }
                    
                    // Write pointer
                    buf[current_pos] = next_pos & 0xFF;
                    buf[current_pos + 1] = (next_pos >> 8) & 0xFF;
                    buf[current_pos + 2] = (next_pos >> 16) & 0xFF;
                    buf[current_pos + 3] = (next_pos >> 24) & 0xFF;
                    
                    // Add type field that might trigger name reuse
                    // Repeated pattern names to trigger name_count limit
                    uint32_t pattern_type = fdp.ConsumeIntegral<uint32_t>() % 10;
                    buf[current_pos + 4] = pattern_type & 0xFF;
                    buf[current_pos + 5] = (pattern_type >> 8) & 0xFF;
                    buf[current_pos + 6] = (pattern_type >> 16) & 0xFF;
                    buf[current_pos + 7] = (pattern_type >> 24) & 0xFF;
                }
            }
            break;
        }
        
        case 3: {
            // Strategy 3: Mixed approach with incremental depth
            // Start with simple structure and gradually add complexity
            // This helps avoid immediate crashes and explores deeper paths
            
            if (test_buffer.size() >= 512) {
                uint8_t* buf = test_buffer.data();
                size_t buf_size = test_buffer.size();
                
                // Determine recursion depth from fuzzer input
                size_t target_depth = fdp.ConsumeIntegralInRange<size_t>(10, 100);
                
                // Build layered structure
                size_t layer_size = 64;
                size_t max_layers = buf_size / layer_size;
                if (max_layers > target_depth) max_layers = target_depth;
                
                for (size_t layer = 0; layer < max_layers; layer++) {
                    size_t layer_start = layer * layer_size;
                    
                    // Each layer has a pointer to the next layer
                    if (layer < max_layers - 1 && layer_start + 8 < buf_size) {
                        size_t next_layer = (layer + 1) * layer_size;
                        buf[layer_start] = next_layer & 0xFF;
                        buf[layer_start + 1] = (next_layer >> 8) & 0xFF;
                        buf[layer_start + 2] = (next_layer >> 16) & 0xFF;
                        buf[layer_start + 3] = (next_layer >> 24) & 0xFF;
                    }
                    
                    // Add layer-specific markers that might match magic patterns
                    // Use common file format markers
                    const char* markers[] = {"ELF", "PDF", "ZIP", "PNG", "GIF", "JPG", "MP3", "AVI", "DOC", "XLS"};
                    size_t marker_idx = layer % 10;
                    const char* marker = markers[marker_idx];
                    
                    size_t marker_pos = layer_start + 16;
                    for (size_t i = 0; i < 3 && marker_pos + i < buf_size; i++) {
                        buf[marker_pos + i] = marker[i];
                    }
                }
            }
            break;
        }
    }
    
    // Check timeout before calling magic_buffer
    if (timeout.timed_out()) {
        magic_close(magic);
        return 0;
    }
    
    // Call magic_buffer with the crafted input
    if (!test_buffer.empty()) {
        const char* result = magic_buffer(magic, test_buffer.data(), test_buffer.size());
        // Result can be NULL for unknown types, that's OK
        (void)result;
    }
    
    // Check timeout after operation
    if (timeout.timed_out()) {
        // Timeout occurred during processing
        magic_close(magic);
        return 0;
    }
    
    // Optional: Test with magic_file as well if we have enough data
    // Create a temporary file for additional testing
    if (fdp.remaining_bytes() > 1024) {
        char temp_filename[] = "/tmp/libmagic_extreme_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd >= 0) {
            // Write remaining data to temp file
            size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
            std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
            
            if (write(fd, file_data.data(), file_data.size()) == (ssize_t)file_data.size()) {
                close(fd);
                
                // Check timeout
                if (!timeout.timed_out()) {
                    const char* file_result = magic_file(magic, temp_filename);
                    (void)file_result;
                }
                
                // Clean up temp file
                unlink(temp_filename);
            } else {
                close(fd);
                unlink(temp_filename);
            }
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_007
