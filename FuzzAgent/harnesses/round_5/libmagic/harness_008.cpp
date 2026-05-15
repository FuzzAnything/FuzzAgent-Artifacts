#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include "magic.h"

namespace harness_008 {
/*
 * Fuzzing harness for libmagic targeting compression format detection and decompression
 * Primary Target: Compression handling functions with 0% coverage in compress.c
 * Focus on generating test data with various compression headers to exercise:
 * - uncompressbzlib: 0% lines, 9/10 branches undiscovered (bzip2 decompression)
 * - uncompresszstd: 0% lines, 7/8 branches undiscovered (zstd decompression)
 * - uncompressxzlib: 0% lines, 7/8 branches undiscovered (xz decompression)
 * - uncompressgzipped: 40.9% lines, 20/36 branches undiscovered (gzip decompression)
 * - uncompresszlib: 50% lines, 7/14 branches undiscovered (zlib decompression)
 * 
 * APIs: magic_open -> magic_load -> magic_buffer with compressed test data -> magic_close
 * Invocation sequence: Initialize libmagic, generate test buffers with compression headers,
 * pass to magic_buffer for detection, which triggers file_zmagic -> uncompressbuf -> specific decompression functions
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000: Basic buffer detection with random operations
 * - harness_001: Configuration management and parameter tuning  
 * - harness_002: Database operations (compile, check, list) with simple magic files
 * - harness_003: Basic file operations and filesystem objects
 * - harness_004: Filesystem magic logic with specific file formats (ELF, CDF, compressed headers only)
 * - harness_005: Deep ELF parsing with complex section structures
 * - harness_006: Magic database parsing internals with diverse magic type testing
 * - harness_007: CDF format detection with Microsoft Office document structures
 * - harness_008: Compression format detection and decompression with various compression algorithms
 */



// Maximum size for compressed data generation
#define MAX_COMPRESSED_DATA_SIZE 1024 * 1024  // 1MB

// Compression format types based on compress.c compr[] array
enum CompressionFormat {
    COMPRESS_GZIP = 0,        // \037\213 (2 bytes)
    COMPRESS_COMPRESS = 1,    // \037\235 (2 bytes) - old compress format
    COMPRESS_FROZEN = 3,      // \037\236 (2 bytes)
    COMPRESS_SCO_LZH = 4,     // \037\240 (2 bytes)
    COMPRESS_PACKED = 5,      // \037\036 (2 bytes)
    COMPRESS_PKZIP = 6,       // PK\3\4 (4 bytes)
    COMPRESS_BZIP2 = 7,       // BZh (3 bytes)
    COMPRESS_LZIP = 8,        // LZIP (4 bytes)
    COMPRESS_XZ = 9,          // \3757zXZ\0 (6 bytes)
    COMPRESS_LRZIP = 10,      // LRZI (4 bytes)
    COMPRESS_LZ4 = 11,        // \004\"M\030 (4 bytes)
    COMPRESS_ZSTD = 12,       // \x28\xB5\x2F\xFD (4 bytes)
    COMPRESS_LZMA = 13,       // uses lzmacmp function check
    COMPRESS_ZLIB = 14,       // uses zlibcmp function check
    COMPRESS_NESTED,          // Nested compression (e.g., tar.gz, tar.bz2)
    COMPRESS_MIXED_HEADERS,   // Mixed/malformed headers
    COMPRESS_ELF_COMPRESSED,  // ELF file compressed
    COMPRESS_CDF_COMPRESSED,  // CDF file compressed
    COMPRESS_MAX
};

// Compression format magic bytes from compress.c
static const uint8_t gzip_magic[] = {0x1f, 0x8b};                       // \037\213
static const uint8_t compress_magic[] = {0x1f, 0x9d};                  // \037\235
static const uint8_t frozen_magic[] = {0x1f, 0x9e};                    // \037\236
static const uint8_t sco_lzh_magic[] = {0x1f, 0xa0};                   // \037\240
static const uint8_t packed_magic[] = {0x1f, 0x1e};                    // \037\036
static const uint8_t pkzip_magic[] = {0x50, 0x4b, 0x03, 0x04};         // PK\3\4
static const uint8_t bzip2_magic[] = {0x42, 0x5a, 0x68};               // BZh
static const uint8_t lzip_magic[] = {0x4c, 0x5a, 0x49, 0x50};          // LZIP
static const uint8_t xz_magic[] = {0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00}; // \3757zXZ\0
static const uint8_t lrzip_magic[] = {0x4c, 0x52, 0x5a, 0x49};         // LRZI
static const uint8_t lz4_magic[] = {0x04, 0x22, 0x4d, 0x18};           // \004\"M\030
static const uint8_t zstd_magic[] = {0x28, 0xb5, 0x2f, 0xfd};          // \x28\xB5\x2F\xFD

// ELF header for compressed ELF tests
static const uint8_t elf64_header[] = {
    0x7f, 'E', 'L', 'F',  // ELF magic
    0x02,                 // 64-bit
    0x01,                 // Little endian
    0x01,                 // ELF version
    0x00,                 // OS ABI
    0x00,                 // ABI version
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Padding
    0x02, 0x00,           // ET_EXEC
    0x3e, 0x00,           // x86-64
    0x01, 0x00, 0x00, 0x00  // ELF version
};

// CDF header for compressed CDF tests
static const uint8_t cdf_header[] = {
    0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1
};

// Generate test buffer with specific compression header
std::vector<uint8_t> generate_compressed_test_data(FuzzedDataProvider& fdp, 
                                                  CompressionFormat format) {
    std::vector<uint8_t> test_data;
    
    // Check if we have enough remaining bytes
    if (fdp.remaining_bytes() < 16) {
        return test_data; // Return empty if insufficient input
    }
    
    // Add compression format header
    switch (format) {
        case COMPRESS_GZIP:
            test_data.insert(test_data.end(), gzip_magic, gzip_magic + sizeof(gzip_magic));
            break;
        case COMPRESS_COMPRESS:
            test_data.insert(test_data.end(), compress_magic, compress_magic + sizeof(compress_magic));
            break;
        case COMPRESS_FROZEN:
            test_data.insert(test_data.end(), frozen_magic, frozen_magic + sizeof(frozen_magic));
            break;
        case COMPRESS_SCO_LZH:
            test_data.insert(test_data.end(), sco_lzh_magic, sco_lzh_magic + sizeof(sco_lzh_magic));
            break;
        case COMPRESS_PACKED:
            test_data.insert(test_data.end(), packed_magic, packed_magic + sizeof(packed_magic));
            break;
        case COMPRESS_PKZIP:
            test_data.insert(test_data.end(), pkzip_magic, pkzip_magic + sizeof(pkzip_magic));
            break;
        case COMPRESS_BZIP2:
            test_data.insert(test_data.end(), bzip2_magic, bzip2_magic + sizeof(bzip2_magic));
            break;
        case COMPRESS_LZIP:
            test_data.insert(test_data.end(), lzip_magic, lzip_magic + sizeof(lzip_magic));
            break;
        case COMPRESS_XZ:
            test_data.insert(test_data.end(), xz_magic, xz_magic + sizeof(xz_magic));
            break;
        case COMPRESS_LRZIP:
            test_data.insert(test_data.end(), lrzip_magic, lrzip_magic + sizeof(lrzip_magic));
            break;
        case COMPRESS_LZ4:
            test_data.insert(test_data.end(), lz4_magic, lz4_magic + sizeof(lz4_magic));
            break;
        case COMPRESS_ZSTD:
            test_data.insert(test_data.end(), zstd_magic, zstd_magic + sizeof(zstd_magic));
            break;
        case COMPRESS_LZMA:
            // LZMA uses function check, but we can add common LZMA header
            test_data.push_back(0x5d); // Common LZMA property byte
            break;
        case COMPRESS_ZLIB:
            // ZLIB uses function check (zlibcmp), but common pattern is 0x78
            test_data.push_back(0x78); // Common zlib CMF byte
            test_data.push_back(fdp.ConsumeIntegral<uint8_t>() & 0x1f); // FLG with valid bits
            break;
        case COMPRESS_NESTED: {
            // Nested compression: e.g., tar.gz (gzip header, then tar-like data)
            test_data.insert(test_data.end(), gzip_magic, gzip_magic + sizeof(gzip_magic));
            // Add fake tar header
            std::string filename = fdp.ConsumeRandomLengthString(100);
            test_data.insert(test_data.end(), filename.begin(), filename.end());
            // Null terminator and tar padding
            for (size_t i = 0; i < 512 - filename.length() - 1; i++) {
                test_data.push_back(0);
            }
            break;
        }
        case COMPRESS_MIXED_HEADERS: {
            // Malformed/mixed headers to test edge cases
            size_t header_len = fdp.ConsumeIntegralInRange<size_t>(2, 10);
            for (size_t i = 0; i < header_len; i++) {
                test_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
            break;
        }
        case COMPRESS_ELF_COMPRESSED: {
            // ELF file compressed with gzip
            test_data.insert(test_data.end(), gzip_magic, gzip_magic + sizeof(gzip_magic));
            // Add compressed ELF header (simulated)
            test_data.insert(test_data.end(), elf64_header, elf64_header + sizeof(elf64_header));
            break;
        }
        case COMPRESS_CDF_COMPRESSED: {
            // CDF file compressed with bzip2
            test_data.insert(test_data.end(), bzip2_magic, bzip2_magic + sizeof(bzip2_magic));
            // Add compressed CDF header (simulated)
            test_data.insert(test_data.end(), cdf_header, cdf_header + sizeof(cdf_header));
            break;
        }
        default:
            // Unknown format, add minimal header
            test_data.push_back(0x1f);
            test_data.push_back(0x8b);
            break;
    }
    
    // Add fuzzer-generated data after the header
    // Use remaining bytes but limit to reasonable size
    size_t remaining = fdp.remaining_bytes();
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(remaining, (size_t)MAX_COMPRESSED_DATA_SIZE));
    
    std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(data_size);
    test_data.insert(test_data.end(), random_data.begin(), random_data.end());
    
    return test_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for format selection and data generation
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libmagic with compression enabled
    // MAGIC_COMPRESS flag is needed for compression detection
    magic_t magic_cookie = magic_open(MAGIC_COMPRESS | MAGIC_ERROR);
    if (magic_cookie == NULL) {
        return 0;
    }
    
    // Load default magic database
    if (magic_load(magic_cookie, NULL) != 0) {
        magic_close(magic_cookie);
        return 0;
    }
    
    // Consume format selection from fuzzer input
    CompressionFormat format = static_cast<CompressionFormat>(
        fdp.ConsumeIntegralInRange<int>(0, COMPRESS_MAX - 1));
    
    // Generate test data with compression header
    std::vector<uint8_t> test_buffer = generate_compressed_test_data(fdp, format);
    
    if (test_buffer.empty()) {
        magic_close(magic_cookie);
        return 0;
    }
    
    // Test magic_buffer on compressed data
    // This should trigger file_zmagic -> uncompressbuf -> specific decompression function
    const char* result = magic_buffer(magic_cookie, test_buffer.data(), test_buffer.size());
    
    // Result can be NULL if detection fails, which is fine
    // We're interested in exercising the decompression code paths, not the result
    
    // Also test with MAGIC_MIME flag to exercise different paths
    magic_setflags(magic_cookie, MAGIC_COMPRESS | MAGIC_MIME | MAGIC_ERROR);
    result = magic_buffer(magic_cookie, test_buffer.data(), test_buffer.size());
    
    // Test with raw buffer detection (no compression flag)
    magic_setflags(magic_cookie, MAGIC_ERROR);
    result = magic_buffer(magic_cookie, test_buffer.data(), test_buffer.size());
    
    // Clean up
    magic_close(magic_cookie);
    
    return 0;
}

} // namespace harness_008
