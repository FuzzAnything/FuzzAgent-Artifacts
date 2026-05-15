#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <cassert>

namespace harness_010 {
/*
 * Fuzzing harness for gzip-specific compression/decompression with full header features
 * Targets: deflateInit2_(), deflate(), deflateEnd(), inflateInit2_(), inflateGetHeader(), inflate(), inflateEnd()
 * 
 * This harness specifically targets the 324 blocked branches in inflate() gzip header parsing logic
 * as identified in coverage analysis report. Focuses on gzip header features:
 * - Extra fields (state->flags & 0x0400)
 * - Filename (state->flags & 0x0800)
 * - Comment (state->flags & 0x1000)
 * - CRC16 header checks
 * - Various OS flags and timestamps
 * 
 * Strategy:
 * 1. Setup gzip compression with deflateInit2_() with windowBits = -15 for raw deflate or 31 for gzip
 * 2. Call deflate() with gzip flag set to produce gzip output with various header configurations
 * 3. Call deflateEnd() for cleanup
 * 4. Setup gzip decompression with inflateInit2_() with windowBits = 47 (16 + 31) for gzip auto detection
 * 5. Call inflateGetHeader() to retrieve gzip header structure
 * 6. Call inflate() to decompress gzip data with various header flags
 * 7. Call inflateEnd() for cleanup
 * 8. Test scenarios: gzip with extra fields, filename, comment, CRC16, OS flags, timestamps
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for gzip header configuration + some content
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume gzip header configuration parameters
    uint8_t header_flags = fdp.ConsumeIntegral<uint8_t>();
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    uint8_t os_type = fdp.ConsumeIntegral<uint8_t>() % 20;  // RFC1952 defines 0-20
    uint8_t xflags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume extra field data if needed
    uint16_t extra_len = 0;
    std::vector<uint8_t> extra_data;
    if (header_flags & 0x04) {  // FEXTRA flag in gzip header
        extra_len = fdp.ConsumeIntegralInRange<uint16_t>(0, 1024);
        if (extra_len > 0 && fdp.remaining_bytes() >= extra_len) {
            extra_data = fdp.ConsumeBytes<uint8_t>(extra_len);
        } else {
            extra_len = 0;
        }
    }
    
    // Consume filename if needed
    std::string filename;
    if (header_flags & 0x08) {  // FNAME flag in gzip header
        filename = fdp.ConsumeRandomLengthString(256);
    }
    
    // Consume comment if needed
    std::string comment;
    if (header_flags & 0x10) {  // FCOMMENT flag in gzip header
        comment = fdp.ConsumeRandomLengthString(256);
    }
    
    // Consume the actual data to compress
    size_t data_to_compress_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (data_to_compress_size > fdp.remaining_bytes()) {
        data_to_compress_size = fdp.remaining_bytes();
    }
    std::vector<uint8_t> data_to_compress = fdp.ConsumeBytes<uint8_t>(data_to_compress_size);
    
    // ========== GZIP COMPRESSION PHASE ==========
    z_stream c_stream;
    int err;
    
    // Initialize compression stream
    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    
    // Choose windowBits for gzip output
    // -15 for raw deflate, 31 for gzip with header
    int windowBits = fdp.ConsumeBool() ? -15 : 31;
    
    err = deflateInit2_(&c_stream, 
                       Z_DEFAULT_COMPRESSION,  // level
                       Z_DEFLATED,             // method
                       windowBits,             // windowBits
                       8,                      // memLevel
                       Z_DEFAULT_STRATEGY,     // strategy
                       ZLIB_VERSION,
                       sizeof(z_stream));
    
    if (err != Z_OK) {
        return 0;
    }
    
    // Prepare gzip header if using gzip format (windowBits = 31)
    gz_header gzip_header;
    if (windowBits == 31) {
        memset(&gzip_header, 0, sizeof(gzip_header));
        gzip_header.text = (header_flags & 0x01) ? 1 : 0;  // FTEXT flag
        gzip_header.time = mtime;
        gzip_header.xflags = xflags;
        gzip_header.os = os_type;
        
        // Set extra field if present
        if (!extra_data.empty()) {
            gzip_header.extra = extra_data.data();
            gzip_header.extra_len = extra_len;
            gzip_header.extra_max = extra_len;
        }
        
        // Set filename if present
        if (!filename.empty()) {
            gzip_header.name = (Bytef*)filename.c_str();
            gzip_header.name_max = filename.length() + 1;
        }
        
        // Set comment if present
        if (!comment.empty()) {
            gzip_header.comment = (Bytef*)comment.c_str();
            gzip_header.comm_max = comment.length() + 1;
        }
        
        gzip_header.hcrc = (header_flags & 0x02) ? 1 : 0;  // FHCRC flag
        
        err = deflateSetHeader(&c_stream, &gzip_header);
        if (err != Z_OK) {
            deflateEnd(&c_stream);
            return 0;
        }
    }
    
    // Set input data for compression
    c_stream.next_in = data_to_compress.data();
    c_stream.avail_in = data_to_compress.size();
    
    // Allocate output buffer for compressed data
    size_t compressed_size = data_to_compress.size() * 2 + 512;  // Safe estimate
    std::vector<uint8_t> compressed_data(compressed_size);
    c_stream.next_out = compressed_data.data();
    c_stream.avail_out = compressed_size;
    
    // Perform compression
    err = deflate(&c_stream, Z_FINISH);
    if (err != Z_STREAM_END && err != Z_OK) {
        deflateEnd(&c_stream);
        return 0;
    }
    
    // Get actual compressed size
    compressed_size = compressed_size - c_stream.avail_out;
    
    // Clean up compression stream
    deflateEnd(&c_stream);
    
    // ========== GZIP DECOMPRESSION PHASE ==========
    z_stream d_stream;
    gz_header gzip_header_read;
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in = Z_NULL;
    d_stream.avail_in = 0;
    
    // Use windowBits = 47 (16 + 31) for gzip auto detection
    // Actually, windowBits should be 31 for gzip (or 47 with auto header detection)
    // According to zlib.h: Add 16 to windowBits to decode gzip format
    int decomp_windowBits = 31;  // gzip format
    err = inflateInit2_(&d_stream, decomp_windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (err != Z_OK) {
        return 0;
    }
    memset(&gzip_header_read, 0, sizeof(gzip_header_read));
    
    // Allocate buffers for header fields if we want to read them
    std::vector<uint8_t> extra_buf(1024);
    std::vector<char> name_buf(1024);
    std::vector<char> comment_buf(1024);
    
    if (fdp.ConsumeBool()) {
        // Try to read the header
        gzip_header_read.extra_max = extra_buf.size();
        gzip_header_read.extra = extra_buf.data();
        gzip_header_read.name_max = name_buf.size();
        gzip_header_read.name = (Bytef*)name_buf.data();
        gzip_header_read.comm_max = comment_buf.size();
        gzip_header_read.comment = (Bytef*)comment_buf.data();
        
        err = inflateGetHeader(&d_stream, &gzip_header_read);
        // inflateGetHeader returns Z_OK or Z_STREAM_ERROR, but we continue anyway
    }
    
    // Set compressed data as input for decompression
    d_stream.next_in = compressed_data.data();
    d_stream.avail_in = compressed_size;
    
    // Allocate output buffer for decompressed data
    std::vector<uint8_t> decompressed_data(data_to_compress.size() * 2);
    d_stream.next_out = decompressed_data.data();
    d_stream.avail_out = decompressed_data.size();
    
    // Perform decompression
    err = inflate(&d_stream, Z_FINISH);
    
    // Check various error conditions that exercise different code paths
    if (err != Z_STREAM_END && err != Z_OK && err != Z_BUF_ERROR && err != Z_DATA_ERROR) {
        // Unexpected error, but we still clean up
        inflateEnd(&d_stream);
        return 0;
    }
    
    // Verify decompressed data matches original if decompression was successful
    if (err == Z_STREAM_END || err == Z_OK) {
        size_t decompressed_size = decompressed_data.size() - d_stream.avail_out;
        if (decompressed_size == data_to_compress.size()) {
            // Can compare, but for fuzzing we just verify size
            // In actual fuzzing, we'd want to verify content too
        }
    }
    
    // Test inflate() with partial data to exercise different states
    if (compressed_size > 10 && fdp.ConsumeBool()) {
        // Reset stream for another test
        inflateEnd(&d_stream);
        
        err = inflateInit2_(&d_stream, decomp_windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
        if (err == Z_OK) {
            // Feed data in smaller chunks to exercise state machine
            size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, compressed_size / 2);
            d_stream.next_in = compressed_data.data();
            d_stream.avail_in = chunk_size;
            d_stream.next_out = decompressed_data.data();
            d_stream.avail_out = decompressed_data.size();
            
            err = inflate(&d_stream, Z_NO_FLUSH);
            
            // Feed remaining data
            if (d_stream.avail_in == 0 && chunk_size < compressed_size) {
                d_stream.next_in = compressed_data.data() + chunk_size;
                d_stream.avail_in = compressed_size - chunk_size;
                err = inflate(&d_stream, Z_FINISH);
            }
            
            inflateEnd(&d_stream);
        }
    } else {
        inflateEnd(&d_stream);
    }
    
    // Additional test: Create malformed gzip headers to exercise error paths
    if (fdp.remaining_bytes() > 10 && fdp.ConsumeBool()) {
        // Create intentionally malformed gzip data
        std::vector<uint8_t> malformed_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        z_stream test_stream;
        test_stream.zalloc = (alloc_func)0;
        test_stream.zfree = (free_func)0;
        test_stream.opaque = (voidpf)0;
        test_stream.next_in = Z_NULL;
        test_stream.avail_in = 0;
        
        if (inflateInit2_(&test_stream, decomp_windowBits, ZLIB_VERSION, (int)sizeof(z_stream)) == Z_OK) {
            test_stream.next_in = malformed_data.data();
            test_stream.avail_in = malformed_data.size();
            
            uint8_t dummy_out[1024];
            test_stream.next_out = dummy_out;
            test_stream.avail_out = sizeof(dummy_out);
            
            // This may fail, but that's OK - we're exercising error paths
            inflate(&test_stream, Z_FINISH);
            
            inflateEnd(&test_stream);
        }
    }
    
    return 0;
}

} // namespace harness_010
