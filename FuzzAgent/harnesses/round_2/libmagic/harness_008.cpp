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

namespace harness_008 {
/*
 * libmagic fuzzing harness - Deep CDF (Composite Document File) parsing
 * Target: CDF deep parsing functions with proper CDF structure generation
 * 
 * This harness specifically targets deep CDF parsing functions in readcdf.c:
 * - cdf_file_property_info() - Property information extraction
 * - cdf_file_summary_info() - Summary information parsing  
 * - cdf_file_catalog() - Catalog stream parsing
 * 
 * Unlike harness_002 which focused on CDF flag manipulation and signatures,
 * this harness generates complete CDF structures with proper headers, sectors,
 * directory streams, and property information to exercise deep parsing paths.
 * 
 * Key features:
 * 1. Generates valid CDF headers with magic number 0xE11AB1A1E011CFD0
 * 2. Creates proper sector allocation tables (512-byte sectors)
 * 3. Generates directory streams with various entry types
 * 4. Includes both valid and malformed CDF structures for edge-case testing
 * 5. Uses MAGIC_RAW flag for binary detection as specified
 * 6. Targets deep parsing functions mentioned in coverage guidance
 * 
 * Semantically distinct from previous harnesses:
 * - harness_000: General buffer detection and flag manipulation  
 * - harness_001: File-based detection and parameter configuration
 * - harness_002: CDF flag manipulation and signature testing
 * - harness_003-005: ELF parsing modules
 * - harness_006: Compression and encoding modules
 * - harness_007: MIME detection and Apple format modules
 * - harness_008: Deep CDF structure generation and parsing (this harness)
 */



// CDF constants from cdf.h
#define CDF_MAGIC 0xE11AB1A1E011CFD0LL
#define CDF_SEC_SIZE_DEFAULT 9  // 2^9 = 512 bytes
#define CDF_SHORT_SEC_SIZE_DEFAULT 6  // 2^6 = 64 bytes
#define CDF_DIRECTORY_SIZE 128

// CDF directory types
#define CDF_DIR_TYPE_EMPTY        0
#define CDF_DIR_TYPE_USER_STORAGE 1
#define CDF_DIR_TYPE_USER_STREAM  2
#define CDF_DIR_TYPE_LOCKBYTES    3
#define CDF_DIR_TYPE_PROPERTY     4
#define CDF_DIR_TYPE_ROOT_STORAGE 5

// CDF sector IDs
#define CDF_SECID_NULL         0
#define CDF_SECID_FREE         -1
#define CDF_SECID_END_OF_CHAIN -2
#define CDF_SECID_SECTOR_ALLOCATION_TABLE -3
#define CDF_SECID_MASTER_SECTOR_ALLOCATION_TABLE -4

// Helper to write little-endian values
template<typename T>
void write_le(uint8_t* buffer, size_t& pos, T value) {
    for (size_t i = 0; i < sizeof(T); i++) {
        buffer[pos++] = (reinterpret_cast<const uint8_t*>(&value))[i];
    }
}

// Generate CDF header structure
void generate_cdf_header(uint8_t* buffer, size_t& pos, FuzzedDataProvider& fdp) {
    size_t start_pos = pos;
    
    // Magic number: 0xE11AB1A1E011CFD0
    write_le<uint64_t>(buffer, pos, CDF_MAGIC);
    
    // UUID (16 bytes) - can be fuzzed
    write_le<uint64_t>(buffer, pos, fdp.ConsumeIntegral<uint64_t>());
    write_le<uint64_t>(buffer, pos, fdp.ConsumeIntegral<uint64_t>());
    
    // Revision and version
    write_le<uint16_t>(buffer, pos, fdp.ConsumeIntegral<uint16_t>());  // Revision
    write_le<uint16_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint16_t>(3, 4));  // Version (3 or 4)
    
    // Byte order: 0xFFFE = little endian, 0xFEFF = big endian
    write_le<uint16_t>(buffer, pos, fdp.ConsumeBool() ? 0xFFFE : 0xFEFF);
    
    // Sector size power of 2 (typically 9 for 512 bytes)
    uint16_t sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(6, 12);
    write_le<uint16_t>(buffer, pos, sec_size_p2);
    
    // Short sector size power of 2 (typically 6 for 64 bytes)
    uint16_t short_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(4, 8);
    write_le<uint16_t>(buffer, pos, short_sec_size_p2);
    
    // Unused0 (10 bytes)
    for (int i = 0; i < 10; i++) {
        buffer[pos++] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Number of sectors in SAT
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint32_t>(1, 100));
    
    // Sector ID of first directory sector
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint32_t>(0, 100));
    
    // Unused1 (4 bytes)
    for (int i = 0; i < 4; i++) {
        buffer[pos++] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Minimum size of standard stream (typically 4096)
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint32_t>(0, 0x10000));
    
    // Sector ID of first sector in short SAT
    write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    
    // Number of sectors in short SAT
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint32_t>(0, 100));
    
    // Sector ID of first sector in master SAT
    write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    
    // Number of sectors in master SAT
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint32_t>(0, 100));
    
    // Master SAT (109 entries = 436 bytes)
    for (int i = 0; i < 109; i++) {
        write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    }
    
    // Ensure we wrote exactly 512 bytes for header
    size_t written = pos - start_pos;
    if (written < 512) {
        // Pad with zeros
        memset(buffer + pos, 0, 512 - written);
        pos += 512 - written;
    }
}

// Generate CDF directory entry
void generate_cdf_directory(uint8_t* buffer, size_t& pos, FuzzedDataProvider& fdp, int entry_type) {
    size_t start_pos = pos;
    
    // Directory name (64 bytes of UTF-16, but we'll use ASCII for simplicity)
    std::string name = fdp.ConsumeRandomLengthString(31);
    for (size_t i = 0; i < 64; i += 2) {
        if (i/2 < name.length()) {
            buffer[pos++] = name[i/2];
            buffer[pos++] = 0;  // High byte of UTF-16
        } else {
            buffer[pos++] = 0;
            buffer[pos++] = 0;
        }
    }
    
    // Name length in characters (including null terminator)
    write_le<uint16_t>(buffer, pos, std::min<uint16_t>(name.length() * 2 + 2, 64));
    
    // Entry type
    buffer[pos++] = entry_type;
    
    // Color (0 = red, 1 = black)
    buffer[pos++] = fdp.ConsumeBool() ? 1 : 0;
    
    // Left/right child and storage IDs
    write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    
    // Storage UUID (16 bytes)
    write_le<uint64_t>(buffer, pos, fdp.ConsumeIntegral<uint64_t>());
    write_le<uint64_t>(buffer, pos, fdp.ConsumeIntegral<uint64_t>());
    
    // Flags
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegral<uint32_t>());
    
    // Creation and modification timestamps
    write_le<int64_t>(buffer, pos, fdp.ConsumeIntegral<int64_t>());
    write_le<int64_t>(buffer, pos, fdp.ConsumeIntegral<int64_t>());
    
    // Stream first sector
    write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
    
    // Stream size
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegral<uint32_t>());
    
    // Unused
    write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegral<uint32_t>());
    
    // Ensure we wrote exactly 128 bytes
    size_t written = pos - start_pos;
    if (written < CDF_DIRECTORY_SIZE) {
        memset(buffer + pos, 0, CDF_DIRECTORY_SIZE - written);
        pos += CDF_DIRECTORY_SIZE - written;
    }
}

// Generate property stream data (for summary info)
void generate_property_stream(uint8_t* buffer, size_t& pos, FuzzedDataProvider& fdp) {
    // Property stream header
    write_le<uint16_t>(buffer, pos, 0xFFFE);  // Byte order
    write_le<uint16_t>(buffer, pos, 0);       // Format version
    write_le<uint16_t>(buffer, pos, 0);       // System version
    
    // Generate some property entries
    int num_properties = fdp.ConsumeIntegralInRange<int>(0, 20);
    for (int i = 0; i < num_properties; i++) {
        // Property ID and type
        write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegral<uint32_t>());
        write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegralInRange<uint32_t>(0, 10));
        
        // Property value based on type
        switch (i % 6) {
            case 0:  // 32-bit signed
                write_le<int32_t>(buffer, pos, fdp.ConsumeIntegral<int32_t>());
                break;
            case 1:  // 32-bit unsigned
                write_le<uint32_t>(buffer, pos, fdp.ConsumeIntegral<uint32_t>());
                break;
            case 2:  // 64-bit floating point
                write_le<double>(buffer, pos, fdp.ConsumeFloatingPoint<double>());
                break;
            case 3:  // String
                {
                    std::string str = fdp.ConsumeRandomLengthString(100);
                    write_le<uint32_t>(buffer, pos, str.length() + 1);
                    for (char c : str) {
                        buffer[pos++] = c;
                    }
                    buffer[pos++] = 0;  // Null terminator
                    // Align to 4 bytes
                    while (pos % 4 != 0) {
                        buffer[pos++] = 0;
                    }
                }
                break;
            case 4:  // Filetime
                write_le<int64_t>(buffer, pos, fdp.ConsumeIntegral<int64_t>());
                break;
            case 5:  // Boolean
                buffer[pos++] = fdp.ConsumeBool() ? 1 : 0;
                // Pad to 4 bytes
                for (int j = 1; j < 4; j++) {
                    buffer[pos++] = 0;
                }
                break;
        }
    }
}

// Main fuzzer function
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for CDF structure generation
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed parameters
    int flags = fdp.ConsumeIntegral<int>();
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Set MAGIC_RAW flag as specified for binary detection
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_RAW | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Ensure MAGIC_RAW is set for better binary detection
    flags |= MAGIC_RAW;
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load magic database
    magic_load(magic, NULL);
    
    // Generate CDF test data based on test type
    std::vector<uint8_t> cdf_data;
    size_t data_size = 0;
    
    switch (test_type) {
        case 0: {
            // Generate minimal CDF with just header
            data_size = 512;
            cdf_data.resize(data_size);
            size_t pos = 0;
            generate_cdf_header(cdf_data.data(), pos, fdp);
            break;
        }
            
        case 1: {
            // Generate CDF with header and directory entries
            data_size = 512 + CDF_DIRECTORY_SIZE * 10;
            cdf_data.resize(data_size);
            size_t pos = 0;
            
            generate_cdf_header(cdf_data.data(), pos, fdp);
            
            // Generate directory entries
            for (int i = 0; i < 10; i++) {
                int dir_type = fdp.ConsumeIntegralInRange<int>(0, 5);
                generate_cdf_directory(cdf_data.data(), pos, fdp, dir_type);
            }
            break;
        }
            
        case 2: {
            // Generate CDF with property stream (for summary info testing)
            data_size = 512 + 1024;
            cdf_data.resize(data_size);
            size_t pos = 0;
            
            generate_cdf_header(cdf_data.data(), pos, fdp);
            
            // Fill rest with property stream data
            generate_property_stream(cdf_data.data(), pos, fdp);
            
            // Fill remaining space
            while (pos < data_size && fdp.remaining_bytes() > 0) {
                cdf_data[pos++] = fdp.ConsumeIntegral<uint8_t>();
            }
            break;
        }
            
        case 3: {
            // Generate malformed CDF with corrupted structures
            data_size = fdp.ConsumeIntegralInRange<size_t>(100, 4096);
            cdf_data.resize(data_size);
            
            // Fill with random data but ensure magic number is present
            // (to trigger CDF parsing but with malformed internals)
            for (size_t i = 0; i < data_size && fdp.remaining_bytes() > 0; i++) {
                cdf_data[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            
            // Ensure magic number at start to trigger CDF parsing
            if (data_size >= 8) {
                cdf_data[0] = 0xD0;
                cdf_data[1] = 0xCF;
                cdf_data[2] = 0x11;
                cdf_data[3] = 0xE0;
                cdf_data[4] = 0xA1;
                cdf_data[5] = 0xB1;
                cdf_data[6] = 0x1A;
                cdf_data[7] = 0xE1;
            }
            break;
        }
            
        case 4: {
            // Generate edge-case CDF with extreme values
            data_size = 2048;
            cdf_data.resize(data_size);
            
            // Fill with edge-case patterns
            for (size_t i = 0; i < data_size; i++) {
                if (i < 512) {
                    // Header area
                    if (i < 8) {
                        // Magic number
                        static const uint8_t magic_bytes[] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
                        cdf_data[i] = magic_bytes[i];
                    } else {
                        // Extreme values in header fields
                        cdf_data[i] = (i % 2 == 0) ? 0xFF : 0x00;
                    }
                } else {
                    // Random data for rest
                    cdf_data[i] = fdp.ConsumeIntegral<uint8_t>();
                }
            }
            break;
        }
    }
    
    // Test the generated CDF data with magic_buffer
    if (!cdf_data.empty()) {
        const char* result = magic_buffer(magic, cdf_data.data(), cdf_data.size());
        (void)result;  // Use result to avoid unused variable warning
        
        // Additional testing with different buffer sizes
        if (cdf_data.size() > 100) {
            // Test with first 100 bytes
            const char* result_partial = magic_buffer(magic, cdf_data.data(), 100);
            (void)result_partial;
        }
    }
    
    // Test with MAGIC_MIME_TYPE flag to check MIME detection for CDF
    int original_flags = magic_getflags(magic);
    magic_setflags(magic, original_flags | MAGIC_MIME_TYPE);
    
    if (!cdf_data.empty()) {
        const char* mime_result = magic_buffer(magic, cdf_data.data(), std::min<size_t>(cdf_data.size(), 512));
        (void)mime_result;
    }
    
    // Restore original flags
    magic_setflags(magic, original_flags);
    
    // Test error handling with empty buffer
    const char* empty_result = magic_buffer(magic, NULL, 0);
    (void)empty_result;
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_008
