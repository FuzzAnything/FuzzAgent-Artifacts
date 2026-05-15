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

namespace harness_002 {
/*
 * libmagic fuzzing harness - CDF (Compound Document Format) module testing
 * Target: CDF-specific detection, magic flags, and file signatures
 * 
 * This harness specifically targets the CDF module which has only 6.09% branch coverage.
 * Focus on CDF-specific magic APIs and patterns:
 * - MAGIC_NO_CHECK_CDF flag manipulation
 * - CDF file signature detection (0xE11AB1A1E011CFD0 magic)
 * - magic_check() and magic_compile() for CDF pattern validation
 * 
 * Semantically distinct from previous harnesses:
 * - harness_000: General buffer detection and flag manipulation  
 * - harness_001: File-based detection and parameter configuration
 * - harness_002: CDF-specific detection, CDF magic signatures, pattern validation
 */



// Utility to create CDF file signature (first 8 bytes of CDF header)
void create_cdf_signature(uint8_t* buffer, size_t size) {
    if (size >= 8) {
        // CDF magic signature: 0xE11AB1A1E011CFD0
        buffer[0] = 0xD0;
        buffer[1] = 0xCF;
        buffer[2] = 0x11;
        buffer[3] = 0xE0;
        buffer[4] = 0xA1;
        buffer[5] = 0xB1;
        buffer[6] = 0x1A;
        buffer[7] = 0xE1;
    }
}

// Create simple magic database with CDF pattern
std::string create_cdf_magic_pattern() {
    return std::string(
        "0 belong 0xD0CF11E0 composite document file v2 (or later) Microsoft Office Document\n"
        "0 belong 0xE11AB1A1E011CFD0 composite document file v2 Microsoft Office Document\n"
        "0 string \\xd0\\xcf\\x11\\xe0\\xa1\\xb1\\x1a\\xe1 composite document file v2 Microsoft Office Document\n"
    );
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int base_flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 6);
    int cdf_flag_toggle = fdp.ConsumeBool() ? MAGIC_NO_CHECK_CDF : 0;
    
    // Mask flags to valid MAGIC_* values, specifically including/excluding CDF check
    int flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Add or remove CDF check flag based on fuzzer input
    if (cdf_flag_toggle) {
        flags |= MAGIC_NO_CHECK_CDF;  // Don't check for CDF files
    } else {
        flags &= ~MAGIC_NO_CHECK_CDF; // Do check for CDF files
    }
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Create test buffer with possible CDF signature
    std::vector<uint8_t> buffer_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (buffer_data.size() < 8) {
        buffer_data.resize(8);
    }
    
    // Potentially inject CDF signature based on fuzzer input
    bool inject_cdf_sig = fdp.ConsumeBool();
    if (inject_cdf_sig && buffer_data.size() >= 8) {
        create_cdf_signature(buffer_data.data(), buffer_data.size());
    }
    
    // Try to load standard magic database (ignore failure)
    magic_load(magic, NULL);
    
    // Test different CDF-related operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test basic buffer detection with CDF signature
            {
                const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result; // Use result to avoid unused variable warning
            }
            break;
            
        case 1:
            // Test CDF flag toggling impact on detection
            {
                int current_flags = magic_getflags(magic);
                
                // Toggle CDF check flag
                int new_flags = current_flags;
                if (current_flags & MAGIC_NO_CHECK_CDF) {
                    new_flags &= ~MAGIC_NO_CHECK_CDF;  // Enable CDF checking
                } else {
                    new_flags |= MAGIC_NO_CHECK_CDF;   // Disable CDF checking
                }
                
                magic_setflags(magic, new_flags);
                
                // Test detection with toggled flags
                const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result;
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 2:
            // Test magic_check() with CDF patterns
            {
                // Create temporary magic file with CDF pattern
                std::string magic_pattern = create_cdf_magic_pattern();
                FILE* tmp = tmpfile();
                if (tmp) {
                    fwrite(magic_pattern.c_str(), 1, magic_pattern.size(), tmp);
                    fflush(tmp);
                    rewind(tmp);
                    
                    // Use magic_check to validate pattern
                    char tmpname[] = "/tmp/magic_cdf_XXXXXX";
                    int fd = mkstemp(tmpname);
                    if (fd >= 0) {
                        write(fd, magic_pattern.c_str(), magic_pattern.size());
                        close(fd);
                        
                        int check_result = magic_check(magic, tmpname);
                        (void)check_result;
                        
                        unlink(tmpname);
                    }
                    
                    fclose(tmp);
                }
            }
            break;
            
        case 3:
            // Test magic_compile() with CDF patterns
            {
                // Create temporary magic file
                std::string magic_pattern = create_cdf_magic_pattern();
                char tmpname[] = "/tmp/magic_cdf_compile_XXXXXX";
                int fd = mkstemp(tmpname);
                if (fd >= 0) {
                    write(fd, magic_pattern.c_str(), magic_pattern.size());
                    close(fd);
                    
                    // Try to compile the magic file
                    int compile_result = magic_compile(magic, tmpname);
                    (void)compile_result;
                    
                    // If compilation succeeded, test with buffer
                    if (compile_result == 0) {
                        const char* result = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                        (void)result;
                    }
                    
                    unlink(tmpname);
                }
            }
            break;
            
        case 4:
            // Test CDF detection with various buffer sizes and patterns
            {
                // Create multiple test buffers with different characteristics
                for (size_t test_size = 8; test_size <= 64 && test_size <= buffer_data.size(); test_size *= 2) {
                    const char* result = magic_buffer(magic, buffer_data.data(), test_size);
                    (void)result;
                }
                
                // Test with intentionally malformed CDF signature
                if (buffer_data.size() >= 8) {
                    uint8_t malformed_buffer[8];
                    memcpy(malformed_buffer, buffer_data.data(), 8);
                    // Corrupt the CDF signature
                    malformed_buffer[0] ^= 0xFF;
                    const char* result = magic_buffer(magic, malformed_buffer, 8);
                    (void)result;
                }
            }
            break;
            
        case 5:
            // Test combination of CDF flag with other relevant flags
            {
                int current_flags = magic_getflags(magic);
                
                // Test with COMPRESS flag (CDF files might be compressed)
                int test_flags = current_flags | MAGIC_COMPRESS;
                magic_setflags(magic, test_flags);
                
                const char* result1 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result1;
                
                // Test with MIME flag
                test_flags = (current_flags & ~MAGIC_COMPRESS) | MAGIC_MIME_TYPE;
                magic_setflags(magic, test_flags);
                
                const char* result2 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                (void)result2;
                
                // Restore original flags
                magic_setflags(magic, current_flags);
            }
            break;
            
        case 6:
            // Test error handling with CDF-related operations
            {
                // Test with NULL buffer
                const char* result1 = magic_buffer(magic, NULL, 0);
                (void)result1;
                
                // Test error APIs
                const char* error_msg = magic_error(magic);
                (void)error_msg;
                
                int err = magic_errno(magic);
                (void)err;
                
                // Test with valid buffer again
                if (!buffer_data.empty()) {
                    const char* result2 = magic_buffer(magic, buffer_data.data(), buffer_data.size());
                    (void)result2;
                }
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_002
