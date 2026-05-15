#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "magic.h"

namespace harness_007 {
/*
 * Fuzzing harness for libmagic CDF (Compound Document Format) detection functions
 * This harness specifically targets CDF detection functions that have 0% coverage
 * according to previous analysis.
 * 
 * Target: CDF detection for Microsoft Office documents, OLE2 compound files
 * Functions: file_trycdf, cdf_read_header, cdf_read_sat, cdf_read_dir, etc.
 * 
 * Testing strategy:
 * 1. Generate test buffers with CDF headers and structures (valid and corrupted)
 * 2. Include edge cases: corrupted CDF headers, partial CDF structures, malformed OLE2 streams
 * 3. Test various CDF header fields (byte order, sector sizes, directory chains)
 * 4. Use MAGIC_NONE flag and test with magic_buffer API
 * 
 * Follow sequence: magic_open -> load magic database -> magic_buffer with CDF test data -> magic_close
 * 
 * Based on analysis of cdf.c and readcdf.c which show CDF detection for
 * Microsoft Office documents before they switched to zipped XML.
 */


// Include libmagic headers

// CDF magic number from cdf.h
#define CDF_MAGIC 0xE11AB1A1E011CFD0ULL

// Helper function to create CDF test buffers
std::vector<uint8_t> create_cdf_test_buffer(FuzzedDataProvider& fdp, size_t max_size) {
    std::vector<uint8_t> buffer;
    
    if (max_size < 512) {
        // Minimum size for meaningful CDF testing
        return buffer;
    }
    
    // Determine test type: valid CDF, corrupted, partial, or malformed
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_type) {
        case 0: // Valid CDF structure
            if (max_size >= 512) {
                // Create a basic CDF header (512 bytes minimum)
                buffer.resize(512);
                uint8_t* buf = buffer.data();
                
                // CDF header structure (first 512 bytes)
                // h_magic (8 bytes)
                uint64_t magic = CDF_MAGIC;
                memcpy(buf, &magic, 8);
                
                // h_uuid (16 bytes) - random UUID
                for (int i = 0; i < 16; i++) {
                    buf[8 + i] = fdp.ConsumeIntegral<uint8_t>();
                }
                
                // h_revision (2 bytes) - typically 0x3E
                buf[24] = 0x00;
                buf[25] = 0x3E;
                
                // h_version (2 bytes) - typically 0x03
                buf[26] = 0x00;
                buf[27] = 0x03;
                
                // h_byte_order (2 bytes) - 0xFEFF for little-endian
                buf[28] = 0xFF;
                buf[29] = 0xFE;
                
                // h_sec_size_p2 (2 bytes) - sector size as power of 2 (typically 9 = 512 bytes)
                buf[30] = 0x09;
                buf[31] = 0x00;
                
                // h_short_sec_size_p2 (2 bytes) - typically 6 = 64 bytes
                buf[32] = 0x06;
                buf[33] = 0x00;
                
                // h_unused0 (10 bytes) - zero
                memset(buf + 34, 0, 10);
                
                // h_num_sectors_in_sat (4 bytes)
                uint32_t sat_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
                memcpy(buf + 44, &sat_count, 4);
                
                // h_secid_first_directory (4 bytes)
                uint32_t first_dir = fdp.ConsumeIntegral<uint32_t>();
                memcpy(buf + 48, &first_dir, 4);
                
                // h_unused1 (4 bytes) - zero
                memset(buf + 52, 0, 4);
                
                // h_min_size_standard_stream (4 bytes)
                uint32_t min_stream = fdp.ConsumeIntegralInRange<uint32_t>(4096, 65536);
                memcpy(buf + 56, &min_stream, 4);
                
                // h_secid_first_sector_in_short_sat (4 bytes)
                uint32_t first_short_sat = fdp.ConsumeIntegral<uint32_t>();
                memcpy(buf + 60, &first_short_sat, 4);
                
                // h_num_sectors_in_short_sat (4 bytes)
                uint32_t short_sat_count = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
                memcpy(buf + 64, &short_sat_count, 4);
                
                // h_secid_first_sector_in_master_sat (4 bytes)
                uint32_t first_master_sat = fdp.ConsumeIntegral<uint32_t>();
                memcpy(buf + 68, &first_master_sat, 4);
                
                // h_num_sectors_in_master_sat (4 bytes)
                uint32_t master_sat_count = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
                memcpy(buf + 72, &master_sat_count, 4);
                
                // h_master_sat (436 bytes) - master sector allocation table
                for (size_t i = 0; i < 436; i++) {
                    buf[76 + i] = fdp.ConsumeIntegral<uint8_t>();
                }
                
                // Fill remaining space with sector-like data
                if (max_size > 512) {
                    size_t remaining = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 512);
                    auto extra_data = fdp.ConsumeBytes<uint8_t>(remaining);
                    buffer.insert(buffer.end(), extra_data.begin(), extra_data.end());
                }
            }
            break;
            
        case 1: // Corrupted CDF header
            if (max_size >= 512) {
                buffer.resize(512);
                uint8_t* buf = buffer.data();
                
                // Partially correct CDF header with some corruption
                uint64_t magic = CDF_MAGIC;
                memcpy(buf, &magic, 8);
                
                // Corrupt some header fields
                for (size_t i = 8; i < 512; i++) {
                    if (fdp.ConsumeBool()) {
                        buf[i] = fdp.ConsumeIntegral<uint8_t>();
                    } else {
                        buf[i] = 0;
                    }
                }
                
                // Add random trailing data
                if (max_size > 512) {
                    size_t remaining = fdp.ConsumeIntegralInRange<size_t>(0, max_size - 512);
                    auto extra_data = fdp.ConsumeBytes<uint8_t>(remaining);
                    buffer.insert(buffer.end(), extra_data.begin(), extra_data.end());
                }
            }
            break;
            
        case 2: // Partial CDF structure (too small)
            {
                // Create a buffer that's too small to be a full CDF
                size_t size = fdp.ConsumeIntegralInRange<size_t>(8, 511);
                buffer = fdp.ConsumeBytes<uint8_t>(size);
                
                // But ensure it starts with CDF magic (if large enough)
                if (size >= 8) {
                    uint64_t magic = CDF_MAGIC;
                    memcpy(buffer.data(), &magic, 8);
                }
            }
            break;
            
        case 3: // Malformed OLE2 streams
            {
                // Create buffer with CDF magic but malformed structure
                size_t size = fdp.ConsumeIntegralInRange<size_t>(100, max_size);
                buffer.resize(size);
                uint8_t* buf = buffer.data();
                
                // Start with CDF magic
                uint64_t magic = CDF_MAGIC;
                memcpy(buf, &magic, 8);
                
                // Fill with random data that might trigger edge cases
                for (size_t i = 8; i < size; i++) {
                    buf[i] = fdp.ConsumeIntegral<uint8_t>();
                }
                // Inject some pattern that might look like OLE2 structures
                if (size >= 164) {  // Need at least 164 bytes for dir_pos calculation
                    // Add some directory-like structures
                    size_t dir_pos = fdp.ConsumeIntegralInRange<size_t>(100, size - 64);
                    for (size_t i = 0; i < 64 && dir_pos + i < size; i++) {
                        buf[dir_pos + i] = fdp.ConsumeIntegral<uint8_t>();
                    }
                }
            }
            break;
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libmagic
    magic_t magic = magic_open(MAGIC_NONE);
    if (magic == nullptr) {
        return 0;
    }
    
    // Try to load the default magic database
    // In fuzzing environment, we might not have access to system magic files
    // So we'll rely on whatever magic database is available
    if (magic_load(magic, nullptr) == -1) {
        // If load fails, we can still test with the magic handle
        // Some functions might work even without a loaded database
    }
    
    // Test multiple CDF test cases with remaining input
    while (fdp.remaining_bytes() > 0) {
        // Determine how much data to use for generating CDF test buffer
        size_t max_buffer_size = fdp.ConsumeIntegralInRange<size_t>(8, fdp.remaining_bytes());
        if (max_buffer_size == 0) {
            break;
        }
        
        // Create CDF test buffer
        std::vector<uint8_t> cdf_buffer = create_cdf_test_buffer(fdp, max_buffer_size);
        
        if (!cdf_buffer.empty()) {
            // Test with magic_buffer
            const char* result = magic_buffer(magic, cdf_buffer.data(), cdf_buffer.size());
            (void)result; // Result can be nullptr or string, we don't need it
            
            // Also test magic_buffer with offset (if buffer is large enough)
            if (cdf_buffer.size() > 100) {
                size_t offset = fdp.ConsumeIntegralInRange<size_t>(0, cdf_buffer.size() - 100);
                size_t length = fdp.ConsumeIntegralInRange<size_t>(1, cdf_buffer.size() - offset);
                const char* result2 = magic_buffer(magic, cdf_buffer.data() + offset, length);
                (void)result2;
            }
        }
        
        // Break if we don't have enough data for another iteration
        if (fdp.remaining_bytes() < 16) {
            break;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_007
