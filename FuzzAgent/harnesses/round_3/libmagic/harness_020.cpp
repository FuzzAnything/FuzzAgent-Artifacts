#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_020 {
/*
 * Fuzzing harness for libmagic library targeting CDF (Compound Document Format) 
 * and ELF parsing modules which are underexplored.
 * 
 * BACKGROUND: After 20 harnesses, magiccheck function has plateaued (206 blocked 
 * branches despite 162M hits). Coverage analysis recommends shifting focus to 
 * underexplored modules: cdf.c (21.30% coverage), readelf.c (18.80% coverage), 
 * readcdf.c (6.58% coverage), fsmagic.c (26.38% coverage).
 * 
 * TARGET: CDF/ELF parsing modules to explore new code paths and potentially 
 * uncover new file format parsing bugs.
 * 
 * APPROACH: Create harness that fuzzes CDF/ELF file parsing by generating 
 * test files that trigger these modules.
 * 
 * APIs to target:
 * - magic_buffer (Primary entry point - 100% covered)
 * - magic_file (Alternative entry point - 50% branch coverage)
 * - magic_open (Initialization)
 * - magic_load (Database initialization - 50% branch coverage)
 * - magic_setflags (Set appropriate flags for file analysis)
 * - magic_close (Cleanup)
 * 
 * INVOCATION SEQUENCE:
 * 1. magic_open() + magic_load() - Create magic_set instance with database
 * 2. magic_setflags() - Set appropriate flags for file analysis
 * 3. magic_buffer() with CDF/ELF file data - Target cdf.c parsing logic
 * 4. magic_close() - Proper resource cleanup
 * 
 * FILE TYPES TO FUZZ:
 * - Microsoft Office documents (.doc, .xls, .ppt) - CDF format
 * - PDF files with CDF structures
 * - Compound file binary format samples
 * - ELF executable files
 * - Archive formats handled by readelf module
 * 
 * SPECIFIC GOALS:
 * 1. Unlock 20+ functions in cdf.c (currently 79% uncovered)
 * 2. Reach cdf_time.c functions (currently 0% covered)
 * 3. Explore readcdf.c integration (6.58% coverage)
 * 4. Potentially trigger new code paths in softmagic via file type detection
 * 
 * Project: $SRC (/root/src/libmagic)
 * Output directory: $OUTPUT/harnesses
 * Existing harnesses: 000-019 (analyze for semantic diversity)
 * 
 * Generate harness_020.cpp that focuses on CDF/ELF file format parsing, 
 * semantically different from existing harnesses which focused on magiccheck. 
 * This is a strategic pivot to explore new attack surfaces.
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// CDF magic number from cdf.h
#define CDF_MAGIC 0xE11AB1A1E011CFD0ULL

// ELF magic number
#define ELF_MAGIC 0x7f454c46  // 0x7f 'E' 'L' 'F'

// Helper function to generate CDF-like data
std::vector<uint8_t> generate_cdf_data(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> data;
    
    // Minimum CDF header size is 512 bytes
    size_t min_size = 512;
    size_t max_size = fdp.ConsumeIntegralInRange<size_t>(min_size, 65536);
    
    // Reserve space
    data.reserve(max_size);
    
    // Add CDF magic number (little-endian)
    uint64_t magic = CDF_MAGIC;
    for (size_t i = 0; i < sizeof(magic); i++) {
        data.push_back(static_cast<uint8_t>(magic >> (i * 8)));
    }
    
    // Add CDF header fields with fuzzed values
    // h_uuid[2] (16 bytes)
    for (int i = 0; i < 16 && data.size() < max_size; i++) {
        data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // h_revision (2 bytes)
    uint16_t revision = fdp.ConsumeIntegral<uint16_t>();
    data.push_back(static_cast<uint8_t>(revision));
    data.push_back(static_cast<uint8_t>(revision >> 8));
    
    // h_version (2 bytes)
    uint16_t version = fdp.ConsumeIntegral<uint16_t>();
    data.push_back(static_cast<uint8_t>(version));
    data.push_back(static_cast<uint8_t>(version >> 8));
    
    // h_byte_order (2 bytes) - often 0xFEFF or 0xFFFE
    uint16_t byte_order = fdp.ConsumeBool() ? 0xFEFF : 0xFFFE;
    data.push_back(static_cast<uint8_t>(byte_order));
    data.push_back(static_cast<uint8_t>(byte_order >> 8));
    
    // h_sec_size_p2 (2 bytes) - sector size power of 2 (typically 9 for 512 bytes)
    uint16_t sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(6, 12);
    data.push_back(static_cast<uint8_t>(sec_size_p2));
    data.push_back(static_cast<uint8_t>(sec_size_p2 >> 8));
    
    // Fill rest with fuzzed data
    while (data.size() < max_size && fdp.remaining_bytes() > 0) {
        data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return data;
}

// Helper function to generate ELF-like data
std::vector<uint8_t> generate_elf_data(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> data;
    
    // Minimum ELF header size
    size_t min_size = 64;
    size_t max_size = fdp.ConsumeIntegralInRange<size_t>(min_size, 65536);
    
    // Reserve space
    data.reserve(max_size);
    
    // Add ELF magic number
    data.push_back(0x7f);  // ELFMAG0
    data.push_back('E');   // ELFMAG1
    data.push_back('L');   // ELFMAG2
    data.push_back('F');   // ELFMAG3
    
    // EI_CLASS (1 byte) - 32-bit or 64-bit
    data.push_back(fdp.ConsumeBool() ? 1 : 2);  // 1=32-bit, 2=64-bit
    
    // EI_DATA (1 byte) - endianness
    data.push_back(fdp.ConsumeBool() ? 1 : 2);  // 1=little, 2=big
    
    // EI_VERSION (1 byte) - usually 1
    data.push_back(1);
    
    // EI_OSABI (1 byte) - OS ABI
    data.push_back(fdp.ConsumeIntegral<uint8_t>());
    
    // EI_ABIVERSION (1 byte)
    data.push_back(fdp.ConsumeIntegral<uint8_t>());
    
    // 7 bytes padding
    for (int i = 0; i < 7; i++) {
        data.push_back(0);
    }
    
    // e_type (2 bytes) - object file type
    uint16_t e_type = fdp.ConsumeIntegral<uint16_t>();
    data.push_back(static_cast<uint8_t>(e_type));
    data.push_back(static_cast<uint8_t>(e_type >> 8));
    
    // e_machine (2 bytes) - architecture
    uint16_t e_machine = fdp.ConsumeIntegral<uint16_t>();
    data.push_back(static_cast<uint8_t>(e_machine));
    data.push_back(static_cast<uint8_t>(e_machine >> 8));
    
    // e_version (4 bytes) - usually 1
    uint32_t e_version = 1;
    for (size_t i = 0; i < sizeof(e_version); i++) {
        data.push_back(static_cast<uint8_t>(e_version >> (i * 8)));
    }
    
    // Fill rest with fuzzed data to simulate program headers, section headers, etc.
    while (data.size() < max_size && fdp.remaining_bytes() > 0) {
        data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return data;
}

// Helper function to generate generic binary data (for other file types)
std::vector<uint8_t> generate_generic_data(FuzzedDataProvider& fdp) {
    size_t size = fdp.ConsumeIntegralInRange<size_t>(1, 65536);
    return fdp.ConsumeBytes<uint8_t>(size);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create magic handle with appropriate flags for CDF/ELF parsing
    int flags = MAGIC_NONE;
    
    // Randomly set/clear flags that affect CDF/ELF parsing
    if (fdp.ConsumeBool()) flags |= MAGIC_SYMLINK;
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    if (fdp.ConsumeBool()) flags |= MAGIC_CONTINUE;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_TYPE;
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME_ENCODING;
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    
    // IMPORTANT: Do NOT disable CDF or ELF checking
    // Clear flags that would disable the modules we want to test
    flags &= ~MAGIC_NO_CHECK_CDF;
    flags &= ~MAGIC_NO_CHECK_ELF;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        // Try with default flags if complex flags fail
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0; // Can't proceed without magic handle
        }
    }
    
    // Load default magic database
    if (magic_load(magic, nullptr) == -1) {
        // If default database load fails, try to continue anyway
        // magic_buffer may still work with some functionality
    }
    
    // Set additional flags for file analysis
    int extra_flags = 0;
    if (fdp.ConsumeBool()) extra_flags |= MAGIC_MIME;
    if (fdp.ConsumeBool()) extra_flags |= MAGIC_APPLE;
    if (fdp.ConsumeBool()) extra_flags |= MAGIC_EXTENSION;
    
    magic_setflags(magic, flags | extra_flags);
    
    // Generate multiple test cases from the fuzzer input
    size_t num_tests = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t test_idx = 0; test_idx < num_tests && fdp.remaining_bytes() > 0; test_idx++) {
        // Choose which type of test data to generate
        uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        std::vector<uint8_t> test_data;
        
        switch (test_type) {
            case 0:
                // CDF format test
                test_data = generate_cdf_data(fdp);
                break;
            case 1:
                // ELF format test
                test_data = generate_elf_data(fdp);
                break;
            case 2:
                // Mixed CDF/ELF data (potential malformed)
                if (fdp.ConsumeBool()) {
                    test_data = generate_cdf_data(fdp);
                } else {
                    test_data = generate_elf_data(fdp);
                }
                // Corrupt some bytes
                if (!test_data.empty() && fdp.ConsumeBool()) {
                    size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(0, test_data.size() - 1);
                    if (corrupt_pos < test_data.size()) {
                        test_data[corrupt_pos] = fdp.ConsumeIntegral<uint8_t>();
                    }
                }
                break;
            case 3:
            default:
                // Generic binary data (could trigger other parsers)
                test_data = generate_generic_data(fdp);
                break;
        }
        
        if (test_data.empty()) {
            continue;
        }
        
        // Test magic_buffer with the generated data
        const char* result = magic_buffer(magic, test_data.data(), test_data.size());
        
        // Note: We don't need to use the result, just calling magic_buffer
        // exercises the parsing code paths in cdf.c, readelf.c, readcdf.c, etc.
        (void)result;
        
        // Also test magic_file if we have enough data to write to a temp file
        if (test_data.size() > 0 && fdp.ConsumeBool()) {
            char temp_template[] = "/tmp/magic_fuzz_cdf_elf_XXXXXX";
            int temp_fd = mkstemp(temp_template);
            if (temp_fd != -1) {
                write(temp_fd, test_data.data(), test_data.size());
                fsync(temp_fd);
                close(temp_fd);
                
                const char* file_result = magic_file(magic, temp_template);
                (void)file_result;
                
                // Clean up temporary file
                unlink(temp_template);
            }
        }
        
        // Test with different flag combinations
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            int test_flags = fdp.ConsumeIntegral<int>();
            magic_setflags(magic, test_flags & ~(MAGIC_NO_CHECK_CDF | MAGIC_NO_CHECK_ELF));
            
            const char* result2 = magic_buffer(magic, test_data.data(), test_data.size());
            (void)result2;
            
            // Restore original flags
            magic_setflags(magic, flags | extra_flags);
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_020
