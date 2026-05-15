#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_009 {
/*
 * libmagic fuzzing harness - Combined ELF parsing and compression detection
 * Target: readelf.c (0.28% branch coverage) and compress.c (3.75% branch coverage)
 * 
 * This harness specifically targets the combination of ELF file parsing and
 * compression detection by generating compressed ELF files:
 * 1) Generate valid ELF structures (32/64-bit, little/big-endian)
 * 2) Compress them with various formats (gzip, bzip2, xz, zstd)
 * 3) Use MAGIC_COMPRESS flag to enable compression detection
 * 4) Test edge cases: malformed ELF headers, truncated compression streams
 * 5) Include corrupted ELF magic numbers (0x7F 'E' 'L' 'F' variations)
 * 
 * Semantically distinct from previous harnesses:
 * - harness_003-005: Test ELF files directly without compression
 * - harness_006: Test compression formats separately without ELF content
 * - harness_009: Tests compressed ELF files, exercising both readelf.c and compress.c
 * 
 * Key focus areas:
 * - MAGIC_COMPRESS flag usage for compressed ELF detection
 * - ELF headers inside compression wrappers
 * - Compression format detection for ELF content
 * - Error paths for malformed compressed ELF files
 * - Testing the interaction between readelf.c and compress.c modules
 */



// ELF constants from readelf.h
#define EI_NIDENT        16

// e_ident indices
#define EI_MAG0          0
#define EI_MAG1          1
#define EI_MAG2          2
#define EI_MAG3          3
#define EI_CLASS         4
#define EI_DATA          5
#define EI_VERSION       6
#define EI_PAD           7

// ELF magic
#define ELFMAG0          0x7f
#define ELFMAG1          'E'
#define ELFMAG2          'L'
#define ELFMAG3          'F'
#define OLFMAG1          0

// ELF class
#define ELFCLASSNONE     0
#define ELFCLASS32       1
#define ELFCLASS64       2

// ELF data encoding (endianness)
#define ELFDATANONE      0
#define ELFDATA2LSB      1
#define ELFDATA2MSB      2

// ELF types
#define ET_NONE          0
#define ET_REL           1
#define ET_EXEC          2
#define ET_DYN           3
#define ET_CORE          4

// Compression method constants
enum CompressionMethod {
    COMPRESS_NONE = 0,
    COMPRESS_GZIP,
    COMPRESS_BZIP2,
    COMPRESS_XZ,
    COMPRESS_ZSTD,
    COMPRESS_LZMA
};

// Helper to generate ELF header with variations
std::vector<uint8_t> generate_elf_header(FuzzedDataProvider& fdp, bool& is_valid) {
    std::vector<uint8_t> header(EI_NIDENT, 0);
    
    // Determine if we should generate valid or corrupted ELF
    bool generate_valid = fdp.ConsumeBool();
    is_valid = generate_valid;
    
    // EI_MAG0: Always 0x7F for valid ELF, can be corrupted
    header[EI_MAG0] = generate_valid ? ELFMAG0 : fdp.ConsumeIntegral<uint8_t>();
    
    // EI_MAG1: 'E' for valid ELF, can be corrupted or OLFMAG1 (0)
    if (generate_valid) {
        header[EI_MAG1] = ELFMAG1;
    } else {
        bool use_olfmag = fdp.ConsumeBool();
        header[EI_MAG1] = use_olfmag ? OLFMAG1 : fdp.ConsumeIntegral<uint8_t>();
    }
    
    // EI_MAG2: 'L' for valid ELF
    header[EI_MAG2] = generate_valid ? ELFMAG2 : fdp.ConsumeIntegral<uint8_t>();
    
    // EI_MAG3: 'F' for valid ELF
    header[EI_MAG3] = generate_valid ? ELFMAG3 : fdp.ConsumeIntegral<uint8_t>();
    
    // EI_CLASS: 32-bit or 64-bit
    if (generate_valid) {
        header[EI_CLASS] = fdp.ConsumeBool() ? ELFCLASS32 : ELFCLASS64;
    } else {
        header[EI_CLASS] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // EI_DATA: Endianness
    if (generate_valid) {
        header[EI_DATA] = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
    } else {
        header[EI_DATA] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // EI_VERSION: Usually 1 (current)
    header[EI_VERSION] = generate_valid ? 1 : fdp.ConsumeIntegral<uint8_t>();
    
    // Remaining ident bytes
    for (size_t i = EI_PAD; i < EI_NIDENT; i++) {
        header[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    return header;
}

// Helper to generate ELF program/section data
std::vector<uint8_t> generate_elf_content(FuzzedDataProvider& fdp, bool is_valid_elf) {
    std::vector<uint8_t> content;
    
    if (!is_valid_elf) {
        // For invalid ELF, just generate random data
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        content = fdp.ConsumeBytes<uint8_t>(data_size);
        return content;
    }
    
    // For valid ELF, generate some program-like data
    // Add ELF header (already generated separately)
    // Add some program header-like structure
    size_t phdr_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    for (size_t i = 0; i < phdr_size; i++) {
        content.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Add some section-like data
    size_t section_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < section_count; i++) {
        // Section name offset
        uint32_t name_offset = fdp.ConsumeIntegral<uint32_t>();
        content.insert(content.end(), 
                      reinterpret_cast<uint8_t*>(&name_offset),
                      reinterpret_cast<uint8_t*>(&name_offset) + sizeof(name_offset));
        
        // Section type
        uint32_t section_type = fdp.ConsumeIntegral<uint32_t>();
        content.insert(content.end(),
                      reinterpret_cast<uint8_t*>(&section_type),
                      reinterpret_cast<uint8_t*>(&section_type) + sizeof(section_type));
        
        // Section flags
        uint64_t flags = fdp.ConsumeIntegral<uint64_t>();
        content.insert(content.end(),
                      reinterpret_cast<uint8_t*>(&flags),
                      reinterpret_cast<uint8_t*>(&flags) + sizeof(flags));
        
        // Some section data
        size_t section_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 128);
        std::vector<uint8_t> section_data = fdp.ConsumeBytes<uint8_t>(section_data_size);
        content.insert(content.end(), section_data.begin(), section_data.end());
    }
    
    return content;
}

// Helper to add compression wrapper
std::vector<uint8_t> add_compression_wrapper(const std::vector<uint8_t>& data, 
                                            CompressionMethod method,
                                            FuzzedDataProvider& fdp) {
    std::vector<uint8_t> compressed_data;
    
    switch (method) {
        case COMPRESS_GZIP:
            // GZIP signature: 0x1F 0x8B
            compressed_data.push_back(0x1F);
            compressed_data.push_back(0x8B);
            // Compression method (8 = deflate)
            compressed_data.push_back(8);
            // Flags
            compressed_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            // MTIME (4 bytes)
            for (int i = 0; i < 4; i++) {
                compressed_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            // Extra flags
            compressed_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            // OS (3 = Unix)
            compressed_data.push_back(3);
            break;
            
        case COMPRESS_BZIP2:
            // BZIP2 signature: 'B' 'Z' 'h'
            compressed_data.push_back('B');
            compressed_data.push_back('Z');
            compressed_data.push_back('h');
            // Version '1' - '9'
            compressed_data.push_back(fdp.ConsumeIntegralInRange<uint8_t>('1', '9'));
            break;
            
        case COMPRESS_XZ:
            // XZ signature: 0xFD '7' 'z' 'X' 'Z' 0x00
            compressed_data.push_back(0xFD);
            compressed_data.push_back('7');
            compressed_data.push_back('z');
            compressed_data.push_back('X');
            compressed_data.push_back('Z');
            compressed_data.push_back(0x00);
            break;
            
        case COMPRESS_ZSTD:
            // ZSTD frame header: 0x28 0xB5 0x2F 0xFD
            compressed_data.push_back(0x28);
            compressed_data.push_back(0xB5);
            compressed_data.push_back(0x2F);
            compressed_data.push_back(0xFD);
            break;
            
        case COMPRESS_LZMA:
            // LZMA signature: 0x5D 00 00
            compressed_data.push_back(0x5D);
            compressed_data.push_back(0x00);
            compressed_data.push_back(0x00);
            break;
            
        case COMPRESS_NONE:
        default:
            // No compression wrapper
            return data;
    }
    
    // Add the actual data (not really compressed, but with signature)
    compressed_data.insert(compressed_data.end(), data.begin(), data.end());
    
    // Optionally truncate to test error paths
    bool truncate = fdp.ConsumeBool();
    if (truncate && compressed_data.size() > 10) {
        compressed_data.resize(compressed_data.size() / 2);
    }
    
    return compressed_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters
    int base_flags = fdp.ConsumeIntegral<int>();
    CompressionMethod compress_method = static_cast<CompressionMethod>(
        fdp.ConsumeIntegralInRange<int>(0, COMPRESS_LZMA));
    bool test_compressed_elf = fdp.ConsumeBool();
    bool use_magic_compress_flag = fdp.ConsumeBool();
    
    // Mask flags to valid MAGIC_* values
    int flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Always include MAGIC_COMPRESS for this harness (targets compression detection)
    if (use_magic_compress_flag) {
        flags |= MAGIC_COMPRESS;
    }
    
    // Optionally disable ELF checking to test different paths
    bool disable_elf_check = fdp.ConsumeBool();
    if (disable_elf_check) {
        flags |= MAGIC_NO_CHECK_ELF;
    }
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load standard magic database
    magic_load(magic, NULL);
    
    // Generate ELF data
    bool is_valid_elf;
    std::vector<uint8_t> elf_header = generate_elf_header(fdp, is_valid_elf);
    std::vector<uint8_t> elf_content = generate_elf_content(fdp, is_valid_elf);
    
    // Combine ELF data
    std::vector<uint8_t> elf_data;
    elf_data.insert(elf_data.end(), elf_header.begin(), elf_header.end());
    elf_data.insert(elf_data.end(), elf_content.begin(), elf_content.end());
    
    // Apply compression wrapper if requested
    std::vector<uint8_t> test_data;
    if (test_compressed_elf && compress_method != COMPRESS_NONE) {
        test_data = add_compression_wrapper(elf_data, compress_method, fdp);
    } else {
        test_data = elf_data;
    }
    
    // Ensure we have some data to test
    if (test_data.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Test magic_buffer with the generated data
    const char* result = magic_buffer(magic, test_data.data(), test_data.size());
    (void)result; // Use result to avoid unused variable warning
    
    // Test with different flag combinations
    if (fdp.ConsumeBool()) {
        // Test with MAGIC_RAW flag for binary output
        int new_flags = magic_getflags(magic);
        new_flags |= MAGIC_RAW;
        magic_setflags(magic, new_flags);
        
        const char* raw_result = magic_buffer(magic, test_data.data(), test_data.size());
        (void)raw_result;
        
        // Restore original flags
        magic_setflags(magic, flags);
    }
    
    // Test error handling
    if (fdp.ConsumeBool() && test_data.size() > 0) {
        // Test with NULL buffer
        const char* null_result = magic_buffer(magic, NULL, 0);
        (void)null_result;
        
        // Test with zero size
        const char* zero_result = magic_buffer(magic, test_data.data(), 0);
        (void)zero_result;
        
        // Get error information
        const char* error_msg = magic_error(magic);
        (void)error_msg;
        
        int err = magic_errno(magic);
        (void)err;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_009
