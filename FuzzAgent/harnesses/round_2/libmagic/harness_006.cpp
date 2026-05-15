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

namespace harness_006 {
/*
 * libmagic fuzzing harness - Compression and encoding modules
 * Target: Compression detection (zlib, bzip2, lzma, zstd) and encoding detection
 * 
 * This harness specifically targets compression and encoding modules:
 * 1) MAGIC_COMPRESS flag and compression detection via file_zmagic()
 * 2) Built-in decompressors for zlib, bzip2, lzma, xz, lzip, lz4, zstd
 * 3) Encoding detection via file_encoding() and MIME encoding flags
 * 4) Edge cases for malformed compressed data and encoding detection
 * 
 * Focus areas:
 * - Compression signatures: gzip (\037\213), bzip2 ("BZh"), xz (\3757zXZ\0), 
 *   lzma, lzip, lz4, zstd (\x28\xB5\x2F\xFD), zlib
 * - MAGIC_COMPRESS, MAGIC_COMPRESS_TRANSP, MAGIC_NO_COMPRESS_FORK flags
 * - MAGIC_MIME_ENCODING flag for encoding detection
 * - Malformed/truncated compressed data to trigger error paths
 * - Semantic differentiation from previous harnesses (general buffer, file, CDF, ELF)
 * - Use FuzzedDataProvider to generate complex compression test cases
 * 
 * Compilation requirements: -lz -lbz2 -llzma -lzstd -llz4
 */


// Compression method indices from compress.c
#define METH_GZIP         2
#define METH_BZIP         7
#define METH_XZ           9
#define METH_LZIP         8
#define METH_ZSTD         12
#define METH_LZMA         13
#define METH_ZLIB         14

// Compression signatures
static const struct {
    const char* magic;
    size_t len;
    int method;
} compression_signatures[] = {
    {"\037\213", 2, METH_GZIP},        // gzip
    {"BZh", 3, METH_BZIP},             // bzip2
    {"\3757zXZ\0", 6, METH_XZ},        // xz
    {"LZIP", 4, METH_LZIP},            // lzip
    {"\x28\xB5\x2F\xFD", 4, METH_ZSTD}, // zstd
    {"\037\235", 2, 0},                // compress
    {"PK\3\4", 4, 6},                  // pkzip
    {"\004\"M\030", 4, 11},            // lz4
};

// Helper to generate compressed-like data with signature
static std::vector<uint8_t> generate_compressed_data(FuzzedDataProvider& fdp, int method) {
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Prepend appropriate magic signature based on method
    std::vector<uint8_t> result;
    
    switch (method) {
        case METH_GZIP:
            result.push_back(0x1F);
            result.push_back(0x8B);
            break;
        case METH_BZIP:
            result.push_back('B');
            result.push_back('Z');
            result.push_back('h');
            break;
        case METH_XZ:
            result.push_back(0xFD);
            result.push_back('7');
            result.push_back('z');
            result.push_back('X');
            result.push_back('Z');
            result.push_back(0x00);
            break;
        case METH_ZSTD:
            result.push_back(0x28);
            result.push_back(0xB5);
            result.push_back(0x2F);
            result.push_back(0xFD);
            break;
        case METH_LZMA:
            // LZMA signature: 5D 00 00
            result.push_back(0x5D);
            result.push_back(0x00);
            result.push_back(0x00);
            break;
        default:
            // Generic signature
            result.push_back(0x1F);
            result.push_back(0x9D);
            break;
    }
    
    // Append the actual data
    result.insert(result.end(), data.begin(), data.end());
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        // Too small for meaningful testing
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 5);
    int compression_method = fdp.ConsumeIntegralInRange<int>(0, 7);
    
    // Mask flags to valid compression-related MAGIC_* values
    int compression_flags = flags & (MAGIC_COMPRESS | MAGIC_COMPRESS_TRANSP | 
                                     MAGIC_NO_COMPRESS_FORK | MAGIC_MIME_ENCODING |
                                     MAGIC_MIME_TYPE | MAGIC_RAW);
    
    // Always enable compression detection for this harness
    compression_flags |= MAGIC_COMPRESS;
    
    // Initialize magic cookie with compression flags
    magic_t magic = magic_open(compression_flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load magic database from standard location
    const char* magic_path = "/usr/share/misc/magic.mgc";
    magic_load(magic, magic_path); // Ignore failure
    
    // Get remaining data for testing
    std::vector<uint8_t> test_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (test_data.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0:
            // Test with raw fuzzer data (may contain compression signatures)
            magic_buffer(magic, test_data.data(), test_data.size());
            break;
            
        case 1:
            // Test with generated compressed data
            {
                std::vector<uint8_t> compressed_data = generate_compressed_data(fdp, compression_method);
                if (!compressed_data.empty()) {
                    magic_buffer(magic, compressed_data.data(), compressed_data.size());
                }
            }
            break;
            
        case 2:
            // Test with MIME encoding detection
            {
                int mime_flags = compression_flags | MAGIC_MIME_ENCODING | MAGIC_MIME_TYPE;
                magic_setflags(magic, mime_flags);
                magic_buffer(magic, test_data.data(), test_data.size());
                // Restore flags
                magic_setflags(magic, compression_flags);
            }
            break;
            
        case 3:
            // Test compression transport flag (check but don't report compression)
            {
                int transp_flags = compression_flags | MAGIC_COMPRESS_TRANSP;
                magic_setflags(magic, transp_flags);
                magic_buffer(magic, test_data.data(), test_data.size());
                magic_setflags(magic, compression_flags);
            }
            break;
            
        case 4:
            // Test no compress fork flag
            {
                int nofork_flags = compression_flags | MAGIC_NO_COMPRESS_FORK;
                magic_setflags(magic, nofork_flags);
                magic_buffer(magic, test_data.data(), test_data.size());
                magic_setflags(magic, compression_flags);
            }
            break;
            
        case 5:
            // Test with truncated data (edge cases)
            {
                size_t trunc_size = test_data.size() > 0 ? 
                    fdp.ConsumeIntegralInRange<size_t>(1, test_data.size()) : 1;
                magic_buffer(magic, test_data.data(), trunc_size);
            }
            break;
    }
    
    // Clean up
    magic_close(magic);
    return 0;
}

} // namespace harness_006
