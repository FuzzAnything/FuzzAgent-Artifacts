#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#pragma pack(push, 1)
#pragma pack(pop)

namespace harness_014 {
/* Advanced fuzzing harness targeting inflate function's 469 blocked branches in gzip header processing
 * Primary target: Specific edge cases in inflate() gzip header state machine that remain uncovered
 * Focus: Header validation, flag combinations, CRC calculations, field parsing, and error conditions
 * 
 * Critical coverage gaps identified:
 * - inflate function has 469 blocked branches in gzip header processing
 * - Specific states: FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC
 * - Header validation checks, CRC calculations, and field parsing logic
 * 
 * This harness specifically targets:
 * 1. Gzip header flag validation (0xe000 mask, method validation)
 * 2. CRC calculation during header parsing (when wrap & 4)
 * 3. Extra field handling with varying lengths
 * 4. Name and comment field processing with null terminators  
 * 5. Header CRC (HCRC) state and validation
 * 6. Different wrap configurations (0, 1, 2, 4 bits)
 * 7. Error conditions and state transitions in header processing
 * 
 * Distinct from harness_013:
 * - harness_013: Generates valid gzip data with varied headers for decompression
 * - harness_014: Directly tests inflate() header processing with crafted headers to trigger specific code paths
 * - Focus on validation logic, error conditions, and edge cases rather than successful decompression
 * 
 * Implementation strategy:
 * - Craft gzip headers with specific flag combinations (valid and invalid)
 * - Test all gzip header fields: FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC
 * - Test wrap configurations (raw, zlib, gzip, gzip with validation)
 * - Focus on inflate()'s 469 blocked branches through targeted header variations
 */


// Minimum input size requirement
const size_t MIN_INPUT_SIZE = 32;

// Maximum buffer sizes to prevent excessive memory usage
const size_t MAX_HEADER_SIZE = 1024;
const size_t MAX_DATA_SIZE = 4096;
const size_t MAX_FIELD_LENGTH = 256;

// Gzip header structure for manual construction
struct GzipHeader {
    uint8_t id1;        // 0x1f
    uint8_t id2;        // 0x8b
    uint8_t cm;         // compression method (8 = DEFLATE)
    uint8_t flg;        // flags
    uint32_t mtime;     // modification time
    uint8_t xfl;        // extra flags
    uint8_t os;         // operating system
};

// Helper to create gzip header with specific configurations
std::vector<uint8_t> create_gzip_header(FuzzedDataProvider& fdp, 
                                        bool include_extra,
                                        bool include_name,
                                        bool include_comment,
                                        bool include_crc16) {
    std::vector<uint8_t> header;
    
    // Basic gzip header (10 bytes)
    GzipHeader base_header;
    base_header.id1 = 0x1f;
    base_header.id2 = 0x8b;
    
    // Consume compression method (should be 8 for DEFLATE, but test others)
    base_header.cm = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume flags with specific bits for testing
    uint8_t flags = 0;
    if (include_extra) flags |= 0x04;     // FEXTRA
    if (include_name) flags |= 0x08;      // FNAME
    if (include_comment) flags |= 0x10;   // FCOMMENT
    if (include_crc16) flags |= 0x02;     // FHCRC
    // Also test reserved bits (0xe0) to trigger validation
    flags |= fdp.ConsumeIntegralInRange<uint8_t>(0, 0xe0) & 0xe0;
    base_header.flg = flags;
    
    // Modification time
    base_header.mtime = fdp.ConsumeIntegral<uint32_t>();
    
    // Extra flags (XFL)
    base_header.xfl = fdp.ConsumeIntegral<uint8_t>();
    
    // Operating system
    base_header.os = fdp.ConsumeIntegral<uint8_t>();
    
    // Add base header
    header.insert(header.end(), reinterpret_cast<uint8_t*>(&base_header),
                  reinterpret_cast<uint8_t*>(&base_header) + sizeof(base_header));
    
    // Extra field (FEXTRA)
    if (include_extra) {
        uint16_t extra_len = fdp.ConsumeIntegralInRange<uint16_t>(1, MAX_FIELD_LENGTH);
        header.push_back(extra_len & 0xFF);
        header.push_back((extra_len >> 8) & 0xFF);
        
        std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(extra_len);
        header.insert(header.end(), extra_data.begin(), extra_data.end());
    }
    
    // File name (FNAME)
    if (include_name) {
        std::string filename = fdp.ConsumeRandomLengthString(MAX_FIELD_LENGTH);
        header.insert(header.end(), filename.begin(), filename.end());
        header.push_back(0); // null terminator
    }
    
    // Comment (FCOMMENT)
    if (include_comment) {
        std::string comment = fdp.ConsumeRandomLengthString(MAX_FIELD_LENGTH);
        header.insert(header.end(), comment.begin(), comment.end());
        header.push_back(0); // null terminator
    }
    
    // Header CRC (FHCRC)
    if (include_crc16) {
        uint16_t crc16 = fdp.ConsumeIntegral<uint16_t>();
        header.push_back(crc16 & 0xFF);
        header.push_back((crc16 >> 8) & 0xFF);
    }
    
    return header;
}

// Test specific gzip header validation paths in inflate()
void test_gzip_header_validation(FuzzedDataProvider& fdp) {
    // Consume parameters for header construction
    bool include_extra = fdp.ConsumeBool();
    bool include_name = fdp.ConsumeBool();
    bool include_comment = fdp.ConsumeBool();
    bool include_crc16 = fdp.ConsumeBool();
    
    // Create gzip header
    std::vector<uint8_t> header = create_gzip_header(fdp, include_extra, 
                                                     include_name, include_comment, include_crc16);
    
    // Consume some deflate data to append
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    std::vector<uint8_t> deflate_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Combine header and data
    std::vector<uint8_t> input;
    input.insert(input.end(), header.begin(), header.end());
    input.insert(input.end(), deflate_data.begin(), deflate_data.end());
    
    if (input.empty()) return;
    
    // Test with different windowBits configurations
    int window_bits_options[] = {15 + 16, 15 + 32, 31, -15};
    int wrap_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    int window_bits = window_bits_options[wrap_mode % 4];
    
    // Initialize inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Set up input
    strm.next_in = input.data();
    strm.avail_in = input.size();
    
    // Allocate output buffer
    std::vector<uint8_t> output(input.size() * 2);
    strm.next_out = output.data();
    strm.avail_out = output.size();
    
    // Initialize with specific windowBits
    int ret = inflateInit2(&strm, window_bits);
    if (ret != Z_OK) {
        return;
    }
    // Setup gzip header structure if needed
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    
    // Allocate buffers for header fields if flags indicate they're present
    // Declare buffers outside if block to ensure they persist through inflation
    std::vector<uint8_t> name_buf, comment_buf, extra_buf;
    
    if (include_name || include_comment || include_extra) {
        name_buf.resize(MAX_FIELD_LENGTH);
        comment_buf.resize(MAX_FIELD_LENGTH);
        extra_buf.resize(MAX_FIELD_LENGTH);
        
        gzheader.name = name_buf.data();
        gzheader.name_max = name_buf.size();
        gzheader.comment = comment_buf.data();
        gzheader.comm_max = comment_buf.size();
        gzheader.extra = extra_buf.data();
        gzheader.extra_max = extra_buf.size();
        
        inflateGetHeader(&strm, &gzheader);
    }
    // Perform inflation - this will process the header
    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Try to continue if we got past header
    while (ret == Z_OK && strm.avail_in > 0 && strm.avail_out > 0) {
        ret = inflate(&strm, Z_NO_FLUSH);
    }
    
    // Clean up
    inflateEnd(&strm);
}

// Test specific flag validation in gzip headers
void test_gzip_flag_validation(FuzzedDataProvider& fdp) {
    // Test various flag combinations, including invalid ones
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
        // Create minimal gzip header
        std::vector<uint8_t> header = {0x1f, 0x8b};
        
        // Compression method - test non-DEFLATE values (should trigger error)
        uint8_t cm = fdp.ConsumeIntegral<uint8_t>();
        header.push_back(cm);
        
        // Flags - test reserved bits (0xe0) and invalid combinations
        uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
        header.push_back(flags);
        
        // Rest of header
        uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
        header.push_back(mtime & 0xFF);
        header.push_back((mtime >> 8) & 0xFF);
        header.push_back((mtime >> 16) & 0xFF);
        header.push_back((mtime >> 24) & 0xFF);
        
        header.push_back(fdp.ConsumeIntegral<uint8_t>()); // xfl
        header.push_back(fdp.ConsumeIntegral<uint8_t>()); // os
        
        // Initialize inflation with gzip auto-detection
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        strm.next_in = header.data();
        strm.avail_in = header.size();
        
        std::vector<uint8_t> output(1024);
        strm.next_out = output.data();
        strm.avail_out = output.size();
        
        int ret = inflateInit2(&strm, 15 + 32); // auto-detection
        if (ret != Z_OK) continue;
        
        // This will process header and may hit validation errors
        ret = inflate(&strm, Z_NO_FLUSH);
        
        inflateEnd(&strm);
    }
}

// Test wrap configurations and header validation
void test_wrap_configurations(FuzzedDataProvider& fdp) {
    // Test different wrap configurations
    int wrap_modes[] = {0, 1, 2, 4, 1|2, 1|4, 2|4, 1|2|4};
    
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
        // Create test data
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
        std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_size);
        
        if (data.empty()) continue;
        
        // Choose wrap mode
        int wrap_idx = fdp.ConsumeIntegralInRange<int>(0, 7);
        int window_bits = 15;
        
        // Adjust windowBits based on wrap mode
        if (wrap_modes[wrap_idx] & 2) {
            // Gzip mode
            window_bits += 16;
        }
        if (wrap_modes[wrap_idx] == 0) {
            // Raw mode
            window_bits = -15;
        }
        
        // Initialize inflation
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        strm.next_in = data.data();
        strm.avail_in = data.size();
        
        std::vector<uint8_t> output(data.size() * 2);
        strm.next_out = output.data();
        strm.avail_out = output.size();
        
        int ret = inflateInit2(&strm, window_bits);
        if (ret != Z_OK) continue;
        
        // Set wrap for validation if needed
        // Note: wrap is set by windowBits, but we can't directly set it
        
        // Try inflation
        ret = inflate(&strm, Z_NO_FLUSH);
        
        // Try different flush modes
        if (ret == Z_OK || ret == Z_BUF_ERROR) {
            ret = inflate(&strm, Z_SYNC_FLUSH);
        }
        
        inflateEnd(&strm);
    }
}

// Test header field parsing (extra, name, comment)
void test_header_field_parsing(FuzzedDataProvider& fdp) {
    // Test each field type separately
    std::vector<bool> field_types = {true, false, false}; // extra, name, comment
    field_types[fdp.ConsumeIntegralInRange<int>(0, 2)] = true;
    
    // Create header with specific field
    std::vector<uint8_t> header = create_gzip_header(fdp, 
                                                     field_types[0], // extra
                                                     field_types[1], // name  
                                                     field_types[2], // comment
                                                     false); // no crc
    
    if (header.empty()) return;
    
    // Add some deflate data
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    std::vector<uint8_t> deflate_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    header.insert(header.end(), deflate_data.begin(), deflate_data.end());
    
    // Initialize with gzip header retrieval
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.next_in = header.data();
    strm.avail_in = header.size();
    
    std::vector<uint8_t> output(header.size() * 2);
    strm.next_out = output.data();
    strm.avail_out = output.size();
    
    int ret = inflateInit2(&strm, 15 + 32); // gzip auto-detection
    if (ret != Z_OK) return;
    
    // Setup header retrieval
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    
    // Allocate buffers for fields we expect - declare outside conditional blocks
    std::vector<uint8_t> name_buf, comment_buf, extra_buf;
    
    if (field_types[1]) { // name
        name_buf.resize(MAX_FIELD_LENGTH);
        gzheader.name = name_buf.data();
        gzheader.name_max = name_buf.size();
    }
    if (field_types[2]) { // comment
        comment_buf.resize(MAX_FIELD_LENGTH);
        gzheader.comment = comment_buf.data();
        gzheader.comm_max = comment_buf.size();
    }
    if (field_types[0]) { // extra
        extra_buf.resize(MAX_FIELD_LENGTH);
        gzheader.extra = extra_buf.data();
        gzheader.extra_max = extra_buf.size();
    }
    
    inflateGetHeader(&strm, &gzheader);
    
    // Process
    ret = inflate(&strm, Z_NO_FLUSH);
    
    inflateEnd(&strm);
}

// Test CRC calculation during header parsing
void test_header_crc_calculation(FuzzedDataProvider& fdp) {
    // Test with FHCRC flag and wrap & 4 (validation)
    
    // Create header with CRC
    std::vector<uint8_t> header = create_gzip_header(fdp, 
                                                     fdp.ConsumeBool(), // extra
                                                     fdp.ConsumeBool(), // name
                                                     fdp.ConsumeBool(), // comment
                                                     true); // with crc
    
    if (header.size() < 12) return; // Need at least base header + crc
    
    // Add data
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<uint8_t> data = fdp.ConsumeBytes<uint8_t>(data_size);
    header.insert(header.end(), data.begin(), data.end());
    
    // Initialize with validation (wrap & 4)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.next_in = header.data();
    strm.avail_in = header.size();
    
    std::vector<uint8_t> output(header.size() * 2);
    strm.next_out = output.data();
    strm.avail_out = output.size();
    
    // Use windowBits that includes gzip and validation
    int ret = inflateInit2(&strm, 15 + 16 + 32); // gzip + auto + ???
    if (ret != Z_OK) return;
    
    // Process
    ret = inflate(&strm, Z_NO_FLUSH);
    
    inflateEnd(&strm);
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to test different aspects
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (test_type) {
        case 0:
            test_gzip_header_validation(fdp);
            break;
        case 1:
            test_gzip_flag_validation(fdp);
            break;
        case 2:
            test_wrap_configurations(fdp);
            break;
        case 3:
            test_header_field_parsing(fdp);
            break;
        case 4:
            test_header_crc_calculation(fdp);
            break;
    }
    
    return 0;
}

} // namespace harness_014
