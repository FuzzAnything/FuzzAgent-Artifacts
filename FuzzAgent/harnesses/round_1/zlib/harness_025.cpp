#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_025 {
/*
 * Fuzzing harness for gzip header extraction using low-level inflate APIs
 * Target: Unlock 451 blocked branches in inflate() function related to header processing
 * 
 * Based on coverage analysis guidance at /root/FuzzAgent/output/zlib/coverage_guidance/harness_025_guidance.md
 * 
 * Root Cause Analysis:
 * - Line 525: `if (state->head != Z_NULL)` executed 261 times
 * - Line 526: `state->head->done = -1;` executed 0 times
 * - This indicates the `state->head` field is never initialized, blocking 100+ branches in header processing logic
 * 
 * Target APIs:
 * - `inflateInit2_` with wrap=2 for gzip mode (required helper)
 * - `inflateGetHeader` to set a valid `gz_header` structure (primary target)
 * - `inflate` for main decompression with header processing (primary target)
 * - `inflateEnd` for cleanup (required helper)
 * 
 * Invocation Sequence:
 * 1. Call `inflateInit2_` with `wrap=2` to enable gzip mode
 * 2. Call `inflateGetHeader` to set a valid `gz_header` structure
 * 3. Call `inflate` with gzip-compressed data containing various header fields (extra, name, comment, CRC)
 * 4. Call `inflateEnd` to clean up resources
 * 
 * Header configurations to target:
 * - Extra fields (flags 0x0400) with different lengths and content
 * - Filename (flags 0x0800) with various lengths and encodings
 * - Comment (flags 0x1000) with different content
 * - CRC16 header checks (flags 0x0200)
 * - Different OS types (0-255) and modification times
 * - Text flag (0x0100) for text vs binary
 * 
 * Expected Coverage Impact:
 * - Cover the 451 blocked branches in `inflate()` related to header processing
 * - Cover blocked branches in gzip-specific header parsing paths (FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC cases)
 * - Improve overall branch coverage from 74.73% toward 80%+
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration and compressed data
    if (size < 512) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // ==============================================
    // Consume fixed-size configuration parameters
    // ==============================================
    
    // Header flag configuration (bitmask of gzip header flags)
    uint16_t header_flags = 0;
    
    // Individual flag selections from fuzzer input
    bool has_extra_field = fdp.ConsumeBool();      // FLG.FEXTRA (0x0400)
    bool has_filename = fdp.ConsumeBool();         // FLG.FNAME (0x0800)
    bool has_comment = fdp.ConsumeBool();          // FLG.FCOMMENT (0x1000)
    bool has_header_crc = fdp.ConsumeBool();       // FLG.FHCRC (0x0200)
    bool is_text = fdp.ConsumeBool();              // FLG.FTEXT (0x0100)
    
    if (has_extra_field) header_flags |= 0x0400;
    if (has_filename) header_flags |= 0x0800;
    if (has_comment) header_flags |= 0x1000;
    if (has_header_crc) header_flags |= 0x0200;
    if (is_text) header_flags |= 0x0100;
    
    // OS type (0-255) and modification time
    uint8_t os_type = fdp.ConsumeIntegral<uint8_t>();
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    
    // Extra field data (if flag is set)
    size_t extra_field_size = 0;
    std::vector<uint8_t> extra_field_data;
    if (has_extra_field) {
        extra_field_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        extra_field_data = fdp.ConsumeBytes<uint8_t>(extra_field_size);
    }
    
    // Filename data (if flag is set)
    std::string filename_data;
    if (has_filename) {
        size_t filename_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        filename_data = fdp.ConsumeBytesAsString(filename_len);
    }
    
    // Comment data (if flag is set)
    std::string comment_data;
    if (has_comment) {
        size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        comment_data = fdp.ConsumeBytesAsString(comment_len);
    }
    
    // Compressed data - use remaining input for gzip compressed payload
    std::vector<uint8_t> compressed_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // ==============================================
    // Create gzip header manually
    // ==============================================
    std::vector<uint8_t> gzip_header;
    
    // ID1 and ID2: gzip magic bytes
    gzip_header.push_back(0x1f);  // ID1
    gzip_header.push_back(0x8b);  // ID2
    
    // CM: compression method (8 = DEFLATE)
    gzip_header.push_back(8);
    
    // FLG: flags
    gzip_header.push_back(header_flags & 0xFF);
    
    // MTIME: modification time (4 bytes, little-endian)
    gzip_header.push_back(mtime & 0xFF);
    gzip_header.push_back((mtime >> 8) & 0xFF);
    gzip_header.push_back((mtime >> 16) & 0xFF);
    gzip_header.push_back((mtime >> 24) & 0xFF);
    
    // XFL: extra flags (2 = max compression, 4 = fastest)
    gzip_header.push_back(fdp.ConsumeIntegralInRange<uint8_t>(0, 4));
    
    // OS: operating system
    gzip_header.push_back(os_type);
    
    // Extra field (if present)
    if (has_extra_field && extra_field_data.size() >= 2) {
        // XLEN: extra field length (2 bytes, little-endian)
        uint16_t xlen = extra_field_data.size();
        gzip_header.push_back(xlen & 0xFF);
        gzip_header.push_back((xlen >> 8) & 0xFF);
        
        // Extra field data
        gzip_header.insert(gzip_header.end(), extra_field_data.begin(), extra_field_data.end());
    }
    
    // Filename (if present) - zero-terminated
    if (has_filename && !filename_data.empty()) {
        gzip_header.insert(gzip_header.end(), filename_data.begin(), filename_data.end());
        gzip_header.push_back(0);  // null terminator
    }
    
    // Comment (if present) - zero-terminated
    if (has_comment && !comment_data.empty()) {
        gzip_header.insert(gzip_header.end(), comment_data.begin(), comment_data.end());
        gzip_header.push_back(0);  // null terminator
    }
    
    // Header CRC (if present)
    if (has_header_crc) {
        // Calculate CRC16 of header (simplified - in real implementation would need actual CRC)
        uint16_t header_crc = 0;
        gzip_header.push_back(header_crc & 0xFF);
        gzip_header.push_back((header_crc >> 8) & 0xFF);
    }
    
    // ==============================================
    // Combine header with compressed data
    // ==============================================
    std::vector<uint8_t> gzip_data = gzip_header;
    gzip_data.insert(gzip_data.end(), compressed_data.begin(), compressed_data.end());
    
    // Add CRC32 and ISIZE for gzip trailer (simplified)
    for (int i = 0; i < 8; i++) {
        gzip_data.push_back(0);
    }
    
    // ==============================================
    // Set up zlib stream and header structure
    // ==============================================
    z_stream strm;
    gz_header head;
    
    memset(&strm, 0, sizeof(strm));
    memset(&head, 0, sizeof(head));
    
    // Initialize header structure
    head.text = is_text ? 1 : 0;
    head.time = mtime;
    head.xflags = (header_flags >> 8) & 0xFF;
    head.os = os_type;
    head.hcrc = has_header_crc ? 1 : 0;
    head.done = 0;
    
    // Set up buffers for header fields if present
    std::vector<uint8_t> extra_buffer;
    std::vector<char> name_buffer;
    std::vector<char> comment_buffer;
    
    if (has_extra_field && !extra_field_data.empty()) {
        extra_buffer.resize(extra_field_data.size() + 1);
        memcpy(extra_buffer.data(), extra_field_data.data(), extra_field_data.size());
        head.extra = extra_buffer.data();
        head.extra_max = extra_buffer.size();
    } else {
        head.extra = Z_NULL;
        head.extra_max = 0;
    }
    
    if (has_filename && !filename_data.empty()) {
        name_buffer.resize(filename_data.size() + 1);
        memcpy(name_buffer.data(), filename_data.c_str(), filename_data.size());
        head.name = (Bytef*)name_buffer.data();
        head.name_max = name_buffer.size();
    } else {
        head.name = Z_NULL;
        head.name_max = 0;
    }
    
    if (has_comment && !comment_data.empty()) {
        comment_buffer.resize(comment_data.size() + 1);
        memcpy(comment_buffer.data(), comment_data.c_str(), comment_data.size());
        head.comment = (Bytef*)comment_buffer.data();
        head.comm_max = comment_buffer.size();
    } else {
        head.comment = Z_NULL;
        head.comm_max = 0;
    }
    
    // ==============================================
    // Initialize decompression stream with gzip mode
    // ==============================================
    // Use inflateInit2 with windowBits = 15 + 16 = 31 for gzip auto detection
    // Or use windowBits = 15 + 32 = 47 for gzip with header
    int ret = inflateInit2_(&strm, 47, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Request header extraction
    ret = inflateGetHeader(&strm, &head);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return 0;
    }
    
    // ==============================================
    // Perform decompression with header processing
    // ==============================================
    strm.next_in = gzip_data.data();
    strm.avail_in = gzip_data.size();
    
    // Output buffer for decompressed data
    std::vector<uint8_t> out_buffer(4096);
    strm.next_out = out_buffer.data();
    strm.avail_out = out_buffer.size();
    
    // Decompress with Z_BLOCK to ensure header processing completes
    ret = inflate(&strm, Z_BLOCK);
    
    // Continue decompression if header was processed successfully
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Check if header was processed
        if (head.done == 1) {
            // Header was successfully extracted
            // Continue with normal decompression
            do {
                strm.avail_out = out_buffer.size();
                strm.next_out = out_buffer.data();
                ret = inflate(&strm, Z_NO_FLUSH);
                
                if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || 
                    ret == Z_MEM_ERROR || ret == Z_VERSION_ERROR) {
                    break;
                }
            } while (strm.avail_out == 0 && ret != Z_STREAM_END);
        }
    }
    
    // ==============================================
    // Clean up
    // ==============================================
    inflateEnd(&strm);
    
    return 0;
}

} // namespace harness_025
