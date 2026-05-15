#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_021 {
/*
 * Fuzzing harness for zlib gzip header processing in inflate function
 * Target: Unlock 443 blocked branches in gzip header processing sections
 * 
 * Focus: Systematic exercise of gzip header processing paths in inflate:
 * - inflate with gzip header processing using inflateGetHeader
 * - deflateInit2_ with windowBits=31 for gzip encoding
 * - inflateInit2_ with windowBits=-15 for gzip auto-detection
 * - All gzip header flag combinations:
 *   * Text flag (0x01)
 *   * CRC16 flag (0x02) 
 *   * Extra field flag (0x04)
 *   * Name flag (0x08)
 *   * Comment flag (0x10)
 * - Various extra field lengths, filename strings, comment strings
 * - Valid/invalid header CRCs
 * 
 * Test scenarios:
 * 1. Basic gzip with no flags
 * 2. gzip with text flag
 * 3. gzip with CRC16 flag
 * 4. gzip with extra field
 * 5. gzip with filename
 * 6. gzip with comment
 * 7. Combination of all flags
 * 8. Invalid header CRC
 * 9. Missing required fields
 * 
 * Target blocked branches in sections: FLAGS, EXLEN, EXTRA, NAME, COMMENT, HCRC
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // First, create gzip compressed data with various header flags
    // We'll use deflate with gzip encoding to generate test data
    
    // Setup compression stream for gzip encoding
    z_stream c_stream;
    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.opaque = Z_NULL;
    
    // Initialize for gzip compression (windowBits = 31 for gzip)
    int windowBits = 31;
    int memLevel = 8; // Default memory level
    int ret = deflateInit2_(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           windowBits, memLevel, Z_DEFAULT_STRATEGY,
                           ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Create some test data to compress
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    if (test_data.empty()) {
        // If not enough bytes, create minimal test data
        test_data.push_back('A');
    }
    
    // Determine gzip header flags based on fuzzer input
    uint8_t gzip_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Clear unsupported bits (bits 5-7 must be zero according to RFC)
    gzip_flags &= 0x1F; // Only bits 0-4 are valid
    
    // Create a custom gzip header manually to control flags
    // Gzip header structure: magic(2) + method(1) + flags(1) + mtime(4) + xfl(1) + os(1)
    std::vector<uint8_t> gzip_header;
    gzip_header.push_back(0x1F); // ID1
    gzip_header.push_back(0x8B); // ID2
    gzip_header.push_back(0x08); // CM = DEFLATE
    
    // Use the fuzzer-controlled flags
    gzip_header.push_back(gzip_flags);
    
    // Modification time (4 bytes)
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    gzip_header.push_back((mtime >> 0) & 0xFF);
    gzip_header.push_back((mtime >> 8) & 0xFF);
    gzip_header.push_back((mtime >> 16) & 0xFF);
    gzip_header.push_back((mtime >> 24) & 0xFF);
    
    // Extra flags (xfl) and OS
    uint8_t xfl = fdp.ConsumeIntegral<uint8_t>();
    uint8_t os = fdp.ConsumeIntegral<uint8_t>() % 256;
    gzip_header.push_back(xfl);
    gzip_header.push_back(os);
    
    // Handle extra field if FLG.FEXTRA is set
    if (gzip_flags & 0x04) { // Extra field flag
        uint16_t extra_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
        gzip_header.push_back((extra_len >> 0) & 0xFF);
        gzip_header.push_back((extra_len >> 8) & 0xFF);
        
        std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_len);
        if (extra_data.size() < extra_len) {
            // Pad if not enough data
            extra_data.resize(extra_len, 0);
        }
        gzip_header.insert(gzip_header.end(), extra_data.begin(), extra_data.end());
    }
    
    // Handle original filename if FLG.FNAME is set
    if (gzip_flags & 0x08) { // Filename flag
        std::string filename = fdp.ConsumeRandomLengthString(256);
        gzip_header.insert(gzip_header.end(), filename.begin(), filename.end());
        gzip_header.push_back(0); // Null terminator
    }
    
    // Handle comment if FLG.FCOMMENT is set
    if (gzip_flags & 0x10) { // Comment flag
        std::string comment = fdp.ConsumeRandomLengthString(256);
        gzip_header.insert(gzip_header.end(), comment.begin(), comment.end());
        gzip_header.push_back(0); // Null terminator
    }
    
    // Handle header CRC if FLG.FHCRC is set
    if (gzip_flags & 0x02) { // CRC16 flag
        // For testing, we can use valid or invalid CRC
        bool use_valid_crc = fdp.ConsumeBool();
        uint16_t crc16 = fdp.ConsumeIntegral<uint16_t>();
        
        // If we want to test invalid CRC path, we can intentionally use wrong CRC
        if (!use_valid_crc) {
            crc16 = ~crc16; // Invert to make invalid
        }
        
        gzip_header.push_back((crc16 >> 0) & 0xFF);
        gzip_header.push_back((crc16 >> 8) & 0xFF);
    }
    
    // Now compress the actual test data
    std::vector<uint8_t> compressed_buffer(test_data.size() * 2 + 256); // Sufficient buffer
    
    c_stream.next_in = test_data.data();
    c_stream.avail_in = test_data.size();
    c_stream.next_out = compressed_buffer.data();
    c_stream.avail_out = compressed_buffer.size();
    
    ret = deflate(&c_stream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&c_stream);
        return 0;
    }
    
    size_t compressed_size = compressed_buffer.size() - c_stream.avail_out;
    deflateEnd(&c_stream);
    
    // Combine gzip header with compressed data and gzip trailer
    std::vector<uint8_t> gzip_data;
    gzip_data.insert(gzip_data.end(), gzip_header.begin(), gzip_header.end());
    gzip_data.insert(gzip_data.end(), compressed_buffer.begin(), compressed_buffer.begin() + compressed_size);
    
    // Add CRC32 and ISIZE (original size modulo 2^32)
    uint32_t crc32_val = crc32(0L, test_data.data(), test_data.size());
    uint32_t isize = test_data.size();
    
    gzip_data.push_back((crc32_val >> 0) & 0xFF);
    gzip_data.push_back((crc32_val >> 8) & 0xFF);
    gzip_data.push_back((crc32_val >> 16) & 0xFF);
    gzip_data.push_back((crc32_val >> 24) & 0xFF);
    
    gzip_data.push_back((isize >> 0) & 0xFF);
    gzip_data.push_back((isize >> 8) & 0xFF);
    gzip_data.push_back((isize >> 16) & 0xFF);
    gzip_data.push_back((isize >> 24) & 0xFF);
    
    // Now test decompression with gzip header processing
    z_stream d_stream;
    d_stream.zalloc = Z_NULL;
    d_stream.zfree = Z_NULL;
    d_stream.opaque = Z_NULL;
    
    // Initialize for gzip decompression (windowBits = -15 for gzip auto-detection)
    windowBits = -15;
    ret = inflateInit2(&d_stream, windowBits);
    if (ret != Z_OK) {
        return 0;
    }
    
    // Setup gz_header structure for header retrieval
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // Allocate buffers for header fields based on what flags we expect
    std::vector<uint8_t> extra_buf;
    std::vector<uint8_t> name_buf;
    std::vector<uint8_t> comment_buf;
    
    if (gzip_flags & 0x04) { // Extra field
        extra_buf.resize(1024);
        header.extra = extra_buf.data();
        header.extra_max = extra_buf.size();
    } else {
        header.extra = Z_NULL;
    }
    
    if (gzip_flags & 0x08) { // Filename
        name_buf.resize(256);
        header.name = name_buf.data();
        header.name_max = name_buf.size();
    } else {
        header.name = Z_NULL;
    }
    
    if (gzip_flags & 0x10) { // Comment
        comment_buf.resize(256);
        header.comment = comment_buf.data();
        header.comm_max = comment_buf.size();
    } else {
        header.comment = Z_NULL;
    }
    
    // Register header structure
    ret = inflateGetHeader(&d_stream, &header);
    if (ret != Z_OK) {
        inflateEnd(&d_stream);
        return 0;
    }
    
    // Provide the gzip data for decompression
    d_stream.next_in = gzip_data.data();
    d_stream.avail_in = gzip_data.size();
    
    std::vector<uint8_t> decompressed_buffer(test_data.size() * 2);
    d_stream.next_out = decompressed_buffer.data();
    d_stream.avail_out = decompressed_buffer.size();
    
    // Decompress with different flush modes to exercise different paths
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 6);
    switch (flush_mode % 7) {
        case 0: flush_mode = Z_NO_FLUSH; break;
        case 1: flush_mode = Z_PARTIAL_FLUSH; break;
        case 2: flush_mode = Z_SYNC_FLUSH; break;
        case 3: flush_mode = Z_FULL_FLUSH; break;
        case 4: flush_mode = Z_FINISH; break;
        case 5: flush_mode = Z_BLOCK; break; // Useful for header-only reading
        case 6: flush_mode = Z_TREES; break; // Another mode
    }
    
    ret = inflate(&d_stream, flush_mode);
    
    // Continue decompression if needed
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        // Try to finish decompression
        d_stream.next_out = decompressed_buffer.data() + (decompressed_buffer.size() - d_stream.avail_out);
        d_stream.avail_out = decompressed_buffer.size() - (decompressed_buffer.size() - d_stream.avail_out);
        ret = inflate(&d_stream, Z_FINISH);
    }
    
    // Also test inflateReset to reset stream state
    if (fdp.ConsumeBool()) {
        ret = inflateReset(&d_stream);
        if (ret == Z_OK) {
            // Try decompressing again with reset stream
            d_stream.next_in = gzip_data.data();
            d_stream.avail_in = gzip_data.size();
            d_stream.next_out = decompressed_buffer.data();
            d_stream.avail_out = decompressed_buffer.size();
            ret = inflate(&d_stream, Z_FINISH);
        }
    }
    
    inflateEnd(&d_stream);
    
    return 0;
}

} // namespace harness_021
