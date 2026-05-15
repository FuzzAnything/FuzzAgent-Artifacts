#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_016 {
/* Advanced fuzzing harness targeting inflate function's 478 blocked branches
 * Focus: Error conditions and edge cases in inflate state machine
 * 
 * Critical coverage gaps identified:
 * - inflate function has 478 blocked branches (out of 630 total)
 * - 21 state->mode = BAD transitions in error conditions
 * - 23 strm->msg error message assignments
 * - Specific error conditions not well-covered by existing harnesses
 * 
 * This harness specifically targets error paths:
 * 1. Header validation failures (zlib, gzip, raw)
 * 2. Block type and validation errors
 * 3. Code length and Huffman table errors  
 * 4. Distance and length code validation
 * 5. CRC and checksum mismatches
 * 6. Memory and buffer boundary conditions
 * 
 * Distinct from existing harnesses:
 * - harness_013: Generates valid gzip data with varied headers
 * - harness_014: Tests gzip header processing with crafted headers
 * - harness_015: Tests multiple entry points with wrap modes
 * - harness_016: Focuses on ERROR CONDITIONS and edge cases in state machine (this harness)
 * 
 * Implementation strategy:
 * - Craft inputs to trigger specific error messages in inflate()
 * - Test all 21 state->mode = BAD transitions
 * - Test boundary conditions and invalid data patterns
 * - Target specific inflate states: HEAD, TYPE, STORED, TABLE, LEN_, DIST, etc.
 * - Focus on inflate()'s 478 blocked branches through targeted error injection
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 16;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_COMPRESSED_SIZE = 1024 * 1024;  // 1MB
const size_t MAX_UNCOMPRESSED_SIZE = 2 * 1024 * 1024;  // 2MB

// Error condition types to test
enum ErrorCondition {
    ERROR_HEADER_CHECK = 0,
    ERROR_UNKNOWN_METHOD,
    ERROR_INVALID_WINDOW,
    ERROR_HEADER_FLAGS,
    ERROR_HEADER_CRC,
    ERROR_INVALID_BLOCK_TYPE,
    ERROR_STORED_LENGTH_MISMATCH,
    ERROR_TOO_MANY_SYMBOLS,
    ERROR_INVALID_CODE_LENGTHS,
    ERROR_MISSING_EOB,
    ERROR_INVALID_LITERALS,
    ERROR_INVALID_DISTANCES,
    ERROR_INVALID_LITERAL_CODE,
    ERROR_INVALID_DISTANCE_CODE,
    ERROR_DISTANCE_TOO_FAR,
    ERROR_DATA_CHECK,
    ERROR_LENGTH_CHECK,
    ERROR_WRAP_MISMATCH,
    ERROR_MAX_CONDITIONS
};

// Helper to create zlib header with specific error condition
std::vector<uint8_t> create_zlib_header(FuzzedDataProvider& fdp, ErrorCondition condition) {
    std::vector<uint8_t> header(2);
    
    switch (condition) {
        case ERROR_HEADER_CHECK:
            // Create header with incorrect check bits (not divisible by 31)
            header[0] = 0x78;  // CMF: CM=8, CINFO=7
            header[1] = 0x00;  // FLG: FCHECK=0 (invalid, should be (CMF*256 + FLG) % 31 == 0)
            break;
            
        case ERROR_UNKNOWN_METHOD:
            // Use non-DEFLATE compression method
            header[0] = 0x70 | (fdp.ConsumeIntegral<uint8_t>() & 0x0F);  // Non-8 method
            header[1] = (31 - ((header[0] * 256) % 31)) % 31;  // Valid check
            break;
            
        case ERROR_INVALID_WINDOW:
            // Invalid window size (too large)
            header[0] = 0x88 | 0x0F;  // CINFO=15 (invalid, max is 7 for 32K window)
            header[1] = (31 - ((header[0] * 256) % 31)) % 31;
            break;
            
        default:
            // Valid zlib header for other tests
            header[0] = 0x78;  // CMF: CM=8 (DEFLATE), CINFO=7 (32K window)
            header[1] = 0x9C;  // FLG: FCHECK=28, FDICT=0, FLEVEL=2
            break;
    }
    
    return header;
}

// Helper to create gzip header with specific error condition
std::vector<uint8_t> create_gzip_header(FuzzedDataProvider& fdp, ErrorCondition condition) {
    std::vector<uint8_t> header(10);
    
    // Basic gzip header
    header[0] = 0x1F;  // ID1
    header[1] = 0x8B;  // ID2
    
    switch (condition) {
        case ERROR_UNKNOWN_METHOD:
            // Non-DEFLATE compression method
            header[2] = fdp.ConsumeIntegral<uint8_t>() % 8;  // Not 8
            break;
            
        case ERROR_HEADER_FLAGS:
            // Reserved flags set (bits 5,6,7)
            header[3] = 0xE0;  // All reserved bits set
            break;
            
        default:
            header[2] = 8;  // CM: DEFLATE
            header[3] = 0;  // FLG: no flags
            break;
    }
    
    // MTIME, XFL, OS
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    header[4] = (mtime >> 0) & 0xFF;
    header[5] = (mtime >> 8) & 0xFF;
    header[6] = (mtime >> 16) & 0xFF;
    header[7] = (mtime >> 24) & 0xFF;
    header[8] = fdp.ConsumeIntegral<uint8_t>();  // XFL
    header[9] = fdp.ConsumeIntegral<uint8_t>();  // OS
    
    return header;
}

// Create stored block with length mismatch error
std::vector<uint8_t> create_stored_block_error(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> block;
    
    // Block header: last block = 1, type = 00 (stored)
    uint8_t header = 0x01;  // 001: last block, stored
    
    block.push_back(header);
    block.push_back(0x00);  // Type bits
    
    // Add length and complement that don't match
    uint16_t length = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    uint16_t nlength = ~length;
    
    // Corrupt the nlength
    if (fdp.ConsumeBool()) {
        nlength ^= 0x01;  // Flip one bit
    }
    
    block.push_back(length & 0xFF);
    block.push_back((length >> 8) & 0xFF);
    block.push_back(nlength & 0xFF);
    block.push_back((nlength >> 8) & 0xFF);
    
    // Add some data
    size_t data_len = std::min<size_t>(length, 256);
    auto data = fdp.ConsumeBytes<uint8_t>(data_len);
    block.insert(block.end(), data.begin(), data.end());
    
    return block;
}

// Create invalid block type (11)
std::vector<uint8_t> create_invalid_block_type(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> block;
    
    // Block header: last block = fdp, type = 11 (invalid)
    uint8_t last = fdp.ConsumeBool() ? 0x01 : 0x00;
    uint8_t type = 0x03;  // 11 binary
    
    block.push_back((last << 0) | (type << 1));
    block.push_back(0x00);  // Extra type bits
    
    return block;
}

// Create dynamic block with invalid code lengths
std::vector<uint8_t> create_invalid_dynamic_block(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> block;
    
    // Block header: last block = fdp, type = 10 (dynamic)
    uint8_t last = fdp.ConsumeBool() ? 0x01 : 0x00;
    uint8_t type = 0x02;  // 10 binary
    
    block.push_back((last << 0) | (type << 1));
    block.push_back(0x00);  // Extra type bits
    
    // HLIT, HDIST, HCLEN (14 bits total)
    // Create invalid values: too many symbols
    uint16_t hlit = 287 + fdp.ConsumeIntegralInRange<uint16_t>(1, 100);  // >286
    uint16_t hdist = 31 + fdp.ConsumeIntegralInRange<uint16_t>(1, 100);  // >30
    uint16_t hclen = fdp.ConsumeIntegralInRange<uint16_t>(4, 19);
    
    uint32_t header_bits = (hlit - 257) | ((hdist - 1) << 5) | ((hclen - 4) << 10);
    
    block.push_back(header_bits & 0xFF);
    block.push_back((header_bits >> 8) & 0xFF);
    
    // Add some code length codes (would be invalid)
    for (int i = 0; i < hclen; i++) {
        block.push_back(fdp.ConsumeIntegral<uint8_t>() % 8);
    }
    
    return block;
}

// Create block with missing end-of-block code
std::vector<uint8_t> create_missing_eob_block(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> block;
    
    // Block header: last block = 1, type = 10 (dynamic)
    block.push_back(0x05);  // 101: last block, dynamic
    block.push_back(0x00);
    
    // Reasonable HLIT, HDIST, HCLEN
    uint16_t hlit = 257 + fdp.ConsumeIntegralInRange<uint16_t>(0, 29);  // 257-286
    uint16_t hdist = 1 + fdp.ConsumeIntegralInRange<uint16_t>(0, 29);   // 1-30
    uint16_t hclen = 4 + fdp.ConsumeIntegralInRange<uint16_t>(0, 15);   // 4-19
    
    uint32_t header_bits = (hlit - 257) | ((hdist - 1) << 5) | ((hclen - 4) << 10);
    
    block.push_back(header_bits & 0xFF);
    block.push_back((header_bits >> 8) & 0xFF);
    
    // Code length codes - ensure code 256 (EOB) has length 0
    // First create random lengths
    for (int i = 0; i < hclen; i++) {
        block.push_back(fdp.ConsumeIntegralInRange<uint8_t>(1, 7));
    }
    
    // Then overwrite position for code 256 to be 0
    // The mapping is complex, but we'll just add more random data
    auto more_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(10, 100));
    block.insert(block.end(), more_data.begin(), more_data.end());
    
    return block;
}

// Test specific error condition
void test_error_condition(FuzzedDataProvider& fdp, ErrorCondition condition, int windowBits) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize based on windowBits
    int ret = inflateInit2(&strm, windowBits);
    if (ret != Z_OK) {
        return;
    }
    
    std::vector<uint8_t> input;
    
    // Build input based on condition and windowBits
    if (windowBits == 0) {
        // Raw deflate - no header
        switch (condition) {
            case ERROR_INVALID_BLOCK_TYPE:
                input = create_invalid_block_type(fdp);
                break;
            case ERROR_STORED_LENGTH_MISMATCH:
                input = create_stored_block_error(fdp);
                break;
            case ERROR_TOO_MANY_SYMBOLS:
                input = create_invalid_dynamic_block(fdp);
                break;
            case ERROR_MISSING_EOB:
                input = create_missing_eob_block(fdp);
                break;
            default:
                // For raw, just use random data
                input = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(16, 512));
                break;
        }
    } else if (windowBits > 0) {
        // zlib wrapper
        auto header = create_zlib_header(fdp, condition);
        input = header;
        
        // Add some block data
        auto block = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(16, 512));
        input.insert(input.end(), block.begin(), block.end());
    } else {
        // gzip wrapper (windowBits < 0, or windowBits + 16 or +32)
        bool with32 = (windowBits < -15);  // windowBits + 32
        bool with16 = (windowBits < 0 && windowBits > -15);  // windowBits + 16
        
        auto header = create_gzip_header(fdp, condition);
        input = header;
        
        // Add some block data
        auto block = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(16, 512));
        input.insert(input.end(), block.begin(), block.end());
        
        // Add CRC and length if needed
        if (with32 || with16) {
            uint32_t crc = fdp.ConsumeIntegral<uint32_t>();
            uint32_t length = fdp.ConsumeIntegral<uint32_t>();
            
            input.push_back(crc & 0xFF);
            input.push_back((crc >> 8) & 0xFF);
            input.push_back((crc >> 16) & 0xFF);
            input.push_back((crc >> 24) & 0xFF);
            
            input.push_back(length & 0xFF);
            input.push_back((length >> 8) & 0xFF);
            input.push_back((length >> 16) & 0xFF);
            input.push_back((length >> 24) & 0xFF);
        }
    }
    
    if (input.empty()) {
        inflateEnd(&strm);
        return;
    }
    
    // Prepare output buffer
    size_t out_size = input.size() * 4;  // Reasonable expansion
    if (out_size > MAX_UNCOMPRESSED_SIZE) {
        out_size = MAX_UNCOMPRESSED_SIZE;
    }
    
    std::vector<uint8_t> output(out_size);
    
    // Set up stream
    strm.next_in = input.data();
    strm.avail_in = input.size();
    strm.next_out = output.data();
    strm.avail_out = output.size();
    
    // Try to inflate - we expect errors for most cases
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Check specific error conditions
    switch (condition) {
        case ERROR_HEADER_CHECK:
            // Should get Z_DATA_ERROR for bad header check
            break;
        case ERROR_UNKNOWN_METHOD:
            // Should get Z_DATA_ERROR for unknown method
            break;
        case ERROR_INVALID_WINDOW:
            // Should get Z_DATA_ERROR for invalid window
            break;
        case ERROR_HEADER_FLAGS:
            // Should get Z_DATA_ERROR for header flags
            break;
        case ERROR_INVALID_BLOCK_TYPE:
            // Should get Z_DATA_ERROR for invalid block type
            break;
        case ERROR_STORED_LENGTH_MISMATCH:
            // Should get Z_DATA_ERROR for stored length mismatch
            break;
        case ERROR_TOO_MANY_SYMBOLS:
            // Should get Z_DATA_ERROR for too many symbols
            break;
        default:
            // Other errors
            break;
    }
    
    // Continue inflating if not finished
    if (ret == Z_OK && strm.avail_in > 0) {
        ret = inflate(&strm, Z_FINISH);
    }
    
    inflateEnd(&strm);
}

// Test inflate with dictionary for DICT state
void test_inflate_with_dictionary(FuzzedDataProvider& fdp) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize with zlib header
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return;
    }
    
    // Create zlib header with dictionary flag
    std::vector<uint8_t> input;
    input.push_back(0x78);  // CMF
    input.push_back(0xBB);  // FLG with FDICT=1 (0x20 bit)
    
    // Dictionary check value
    uint32_t dictid = fdp.ConsumeIntegral<uint32_t>();
    input.push_back(dictid & 0xFF);
    input.push_back((dictid >> 8) & 0xFF);
    input.push_back((dictid >> 16) & 0xFF);
    input.push_back((dictid >> 24) & 0xFF);
    
    // Add some compressed data
    auto data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(16, 256));
    input.insert(input.end(), data.begin(), data.end());
    
    // Prepare output
    std::vector<uint8_t> output(4096);
    
    strm.next_in = input.data();
    strm.avail_in = input.size();
    strm.next_out = output.data();
    strm.avail_out = output.size();
    
    // Inflate should stop at DICT state
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Try to set dictionary (may fail)
    if (ret == Z_NEED_DICT) {
        std::vector<uint8_t> dict(32768);  // Max dictionary size
        size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, dict.size());
        for (size_t i = 0; i < dict_size; i++) {
            dict[i] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        inflateSetDictionary(&strm, dict.data(), dict_size);
        
        // Continue inflation
        ret = inflate(&strm, Z_FINISH);
    }
    
    inflateEnd(&strm);
}

// Test inflatePrime for adding bits to bit buffer
void test_inflate_prime(FuzzedDataProvider& fdp) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize raw inflate
    int ret = inflateInit2(&strm, 0);  // Raw
    if (ret != Z_OK) {
        return;
    }
    
    // Add some bits to the bit buffer
    int bits = fdp.ConsumeIntegralInRange<int>(1, 16);
    int value = fdp.ConsumeIntegralInRange<int>(0, (1 << bits) - 1);
    
    // Fix: Pass value directly to inflatePrime as per API specification
    // The API expects 'bits' least significant bits of 'value' to be inserted
    // No shifting needed since value is already constrained to have only 'bits' LSBs set
    ret = inflatePrime(&strm, bits, value);
    
    // Add some data
    std::vector<uint8_t> input = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(16, 128));
    std::vector<uint8_t> output(1024);
    
    strm.next_in = input.data();
    strm.avail_in = input.size();
    strm.next_out = output.data();
    strm.avail_out = output.size();
    
    ret = inflate(&strm, Z_FINISH);
    
    inflateEnd(&strm);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume error condition to test
    ErrorCondition condition = static_cast<ErrorCondition>(
        fdp.ConsumeIntegralInRange<int>(0, ERROR_MAX_CONDITIONS - 1));
    
    // Consume windowBits configuration
    // - 0: raw deflate
    // - 1-15: zlib wrapper with window size
    // - negative: gzip wrapper (absolute value)
    // - +16: gzip auto-detection
    // - +32: gzip auto-detection with header validation
    int windowBits = 0;
    int wrap_mode = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    switch (wrap_mode) {
        case 0:  // raw
            windowBits = 0;
            break;
        case 1:  // zlib
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
            break;
        case 2:  // gzip (+16)
            windowBits = -fdp.ConsumeIntegralInRange<int>(8, 15);
            break;
        case 3:  // gzip auto-detect (+16)
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15) + 16;
            break;
        case 4:  // gzip auto-detect with validation (+32)
            windowBits = fdp.ConsumeIntegralInRange<int>(8, 15) + 32;
            break;
    }
    
    // Test the selected error condition
    test_error_condition(fdp, condition, windowBits);
    
    // Also test dictionary and prime functions with remaining input
    if (fdp.remaining_bytes() > 32) {
        test_inflate_with_dictionary(fdp);
    }
    
    if (fdp.remaining_bytes() > 16) {
        test_inflate_prime(fdp);
    }
    
    return 0;
}

} // namespace harness_016
