#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace harness_003 {
/*
 * libmagic fuzzing harness - ELF file parsing module testing
 * Target: ELF-specific detection (readelf.c) which has only 0.28% branch coverage
 * 
 * This harness specifically targets the ELF parsing module with:
 * - ELF magic signature detection (0x7F 'E' 'L' 'F')
 * - MAGIC_NO_CHECK_ELF flag manipulation to enable/disable ELF checking
 * - MAGIC_DEBUG flag for debugging output
 * - Testing ELF header patterns and variations
 * - ELF class detection (ELFCLASS32, ELFCLASS64)
 * 
 * Semantically distinct from previous harnesses:
 * - harness_000: General buffer detection and flag manipulation  
 * - harness_001: File-based detection and parameter configuration
 * - harness_002: CDF-specific detection and pattern validation
 * - harness_003: ELF-specific detection, ELF magic signatures, readelf.c targeting
 */



// ELF magic signature constants from readelf.h
#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
#define OLFMAG1 0

// ELF class constants
#define EI_CLASS      4
#define ELFCLASS32    1
#define ELFCLASS64    2

// ELF identification indices
#define EI_MAG0       0
#define EI_MAG1       1
#define EI_MAG2       2
#define EI_MAG3       3

// Utility to create ELF file signature with optional variations
void create_elf_signature(uint8_t* buffer, size_t size, FuzzedDataProvider& fdp) {
    if (size >= 16) {
        // Basic ELF magic signature
        buffer[EI_MAG0] = ELFMAG0;  // 0x7F
        
        // EI_MAG1 can be ELFMAG1 ('E') or OLFMAG1 (0) based on fuzzer input
        bool use_olfmag = fdp.ConsumeBool();
        buffer[EI_MAG1] = use_olfmag ? OLFMAG1 : ELFMAG1;
        
        buffer[EI_MAG2] = ELFMAG2;  // 'L'
        buffer[EI_MAG3] = ELFMAG3;  // 'F'
        
        // Set ELF class based on fuzzer input
        int elf_class = fdp.ConsumeIntegralInRange<int>(1, 3);
        if (elf_class == 1) {
            buffer[EI_CLASS] = ELFCLASS32;
        } else if (elf_class == 2) {
            buffer[EI_CLASS] = ELFCLASS64;
        } else {
            // Invalid class to test error paths
            buffer[EI_CLASS] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Fill remaining header with fuzzed data
        for (size_t i = 5; i < 16 && i < size; i++) {
            buffer[i] = fdp.ConsumeIntegral<uint8_t>();
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 20) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int base_flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 5);
    bool enable_elf_check = fdp.ConsumeBool();
    bool enable_debug = fdp.ConsumeBool();
    
    // Mask flags to valid MAGIC_* values
    int flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Add or remove ELF check flag based on fuzzer input
    if (enable_elf_check) {
        flags &= ~MAGIC_NO_CHECK_ELF;  // Do check for ELF files
    } else {
        flags |= MAGIC_NO_CHECK_ELF;   // Don't check for ELF files
    }
    
    // Add debug flag if requested
    if (enable_debug) {
        flags |= MAGIC_DEBUG;
    }
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Create test buffer with possible ELF signature
    std::vector<uint8_t> buffer_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (buffer_data.size() < 64) {
        buffer_data.resize(64);
    }
    
    // Potentially inject ELF signature based on fuzzer input
    bool inject_elf_sig = fdp.ConsumeBool();
    if (inject_elf_sig && buffer_data.size() >= 16) {
        create_elf_signature(buffer_data.data(), buffer_data.size(), fdp);
    }
    
    // Try to load standard magic database (ignore failure)
    magic_load(magic, NULL);
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test basic buffer detection with ELF data
            {
                const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result; // Use result to avoid unused variable warning
                
                // Check if ELF detection worked
                const char* error_msg = magic_error(magic);
                (void)error_msg;
            }
            break;
            
        case 1:
            // Test with toggled ELF check flag during runtime
            {
                int current_flags = magic_getflags(magic);
                
                // Toggle ELF check flag
                int new_flags = current_flags;
                if (new_flags & MAGIC_NO_CHECK_ELF) {
                    new_flags &= ~MAGIC_NO_CHECK_ELF;
                } else {
                    new_flags |= MAGIC_NO_CHECK_ELF;
                }
                magic_setflags(magic, new_flags);
                
                // Test detection with new flags
                const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result;
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 2:
            // Test with debug flag enabled
            {
                int current_flags = magic_getflags(magic);
                magic_setflags(magic, current_flags | MAGIC_DEBUG);
                
                const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result;
                
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 3:
            // Test multiple detection calls with different buffer sizes
            {
                // Test with full buffer
                const char* result1 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result1;
                
                // Test with minimal ELF header size
                if (buffer_data.size() >= 16) {
                    const char* result2 = magic_buffer(magic, buffer_data.data(), 16);
                    (void)result2;
                }
                
                // Test with partial header
                if (buffer_data.size() >= 8) {
                    const char* result3 = magic_buffer(magic, buffer_data.data(), 8);
                    (void)result3;
                }
            }
            break;
            
        case 4:
            // Test with various flag combinations targeting ELF detection
            {
                // Test with MIME flags (should affect ELF output format)
                int current_flags = magic_getflags(magic);
                
                // Test with MIME type flag
                magic_setflags(magic, current_flags | MAGIC_MIME_TYPE);
                const char* result1 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result1;
                
                // Test with MIME encoding flag
                magic_setflags(magic, (current_flags & ~MAGIC_MIME_TYPE) | MAGIC_MIME_ENCODING);
                const char* result2 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result2;
                
                // Test with both MIME flags
                magic_setflags(magic, current_flags | MAGIC_MIME);
                const char* result3 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result3;
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 5:
            // Test error handling with corrupted ELF data
            {
                // Create intentionally corrupted ELF signature
                if (buffer_data.size() >= 16) {
                    // Corrupt magic bytes
                    buffer_data[EI_MAG0] = fdp.ConsumeIntegral<uint8_t>();
                    buffer_data[EI_MAG1] = fdp.ConsumeIntegral<uint8_t>();
                    buffer_data[EI_MAG2] = fdp.ConsumeIntegral<uint8_t>();
                    buffer_data[EI_MAG3] = fdp.ConsumeIntegral<uint8_t>();
                    
                    const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                    (void)result;
                    
                    // Check error state
                    const char* error_msg = magic_error(magic);
                    (void)error_msg;
                    int err = magic_errno(magic);
                    (void)err;
                }
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_003
