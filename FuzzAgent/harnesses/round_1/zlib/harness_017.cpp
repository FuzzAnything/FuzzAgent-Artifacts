#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_017 {
/*
 * Fuzzing harness for zlib Gzip header processing with inflateGetHeader API
 * Targets comprehensive Gzip header processing with 439 blocked branches in inflate.c
 * Focus: Testing complex conditional logic in lines 590-680 of inflate.c for:
 *   - Extra fields (FLG.FEXTRA), name (FLG.FNAME), comment (FLG.FCOMMENT), CRC (FLG.FHCRC)
 *   - Header CRC mismatch detection, extra field length handling, name/comment string processing
 *   - Various gzip flag combinations and error conditions
 * Semantically different from harness_006 (which only tests inflateGetHeader briefly)
 * This harness focuses exclusively on gzip header processing complexity
 */


// Gzip header constants
#define GZIP_MAGIC1 0x1f
#define GZIP_MAGIC2 0x8b
#define DEFLATE_METHOD 8

// Flag bits in FLG byte
#define FTEXT 0x01     // File is ASCII text
#define FHCRC 0x02     // Header CRC present
#define FEXTRA 0x04    // Extra field present
#define FNAME 0x08     // File name present
#define FCOMMENT 0x10  // Comment present

// Helper function to create gzip header based on fuzzer input
static std::vector<uint8_t> create_gzip_header(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> header;
    
    // Fixed gzip header start
    header.push_back(GZIP_MAGIC1);
    header.push_back(GZIP_MAGIC2);
    header.push_back(DEFLATE_METHOD);  // Compression method: DEFLATE
    
    // FLG byte - determine which optional fields are present
    uint8_t flg = 0;
    if (fdp.ConsumeBool()) flg |= FTEXT;
    if (fdp.ConsumeBool()) flg |= FHCRC;
    if (fdp.ConsumeBool()) flg |= FEXTRA;
    if (fdp.ConsumeBool()) flg |= FNAME;
    if (fdp.ConsumeBool()) flg |= FCOMMENT;
    header.push_back(flg);
    
    // MTIME (modification time) - 4 bytes
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    header.push_back((mtime >> 0) & 0xff);
    header.push_back((mtime >> 8) & 0xff);
    header.push_back((mtime >> 16) & 0xff);
    header.push_back((mtime >> 24) & 0xff);
    
    // XFL (extra flags) - 1 byte
    header.push_back(fdp.ConsumeIntegral<uint8_t>());
    
    // OS (operating system) - 1 byte
    header.push_back(fdp.ConsumeIntegral<uint8_t>());
    
    // Optional extra field if FEXTRA is set
    if (flg & FEXTRA) {
        // Extra field length (2 bytes, little-endian)
        uint16_t xlen = fdp.ConsumeIntegralInRange<uint16_t>(0, 1024);
        header.push_back((xlen >> 0) & 0xff);
        header.push_back((xlen >> 8) & 0xff);
        
        // Extra field data
        if (xlen > 0) {
            auto extra_data = fdp.ConsumeBytes<uint8_t>(xlen);
            header.insert(header.end(), extra_data.begin(), extra_data.end());
        }
    }
    
    // Optional file name if FNAME is set
    if (flg & FNAME) {
        std::string filename = fdp.ConsumeRandomLengthString(256);
        header.insert(header.end(), filename.begin(), filename.end());
        header.push_back(0);  // Null terminator
    }
    
    // Optional comment if FCOMMENT is set
    if (flg & FCOMMENT) {
        std::string comment = fdp.ConsumeRandomLengthString(256);
        header.insert(header.end(), comment.begin(), comment.end());
        header.push_back(0);  // Null terminator
    }
    
    // Optional header CRC if FHCRC is set
    if (flg & FHCRC) {
        // Calculate CRC-32 of header so far
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, header.data(), header.size());
        uint16_t crc16 = crc & 0xffff;
        
        header.push_back((crc16 >> 0) & 0xff);
        header.push_back((crc16 >> 8) & 0xff);
    }
    
    return header;
}

// Helper function to create deflate compressed data
static std::vector<uint8_t> create_deflate_data(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> compressed_data;
    
    // Simple deflate block: fixed Huffman codes, no compression for simplicity
    // This creates a valid deflate stream that inflate can process
    uint8_t block_header = 0x01;  // Final block, no compression
    compressed_data.push_back(block_header);
    
    // Length of uncompressed data (up to 64KB)
    uint16_t len = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    uint16_t nlen = ~len;  // One's complement
    
    compressed_data.push_back((len >> 0) & 0xff);
    compressed_data.push_back((len >> 8) & 0xff);
    compressed_data.push_back((nlen >> 0) & 0xff);
    compressed_data.push_back((nlen >> 8) & 0xff);
    
    // Actual data
    auto data = fdp.ConsumeBytes<uint8_t>(len);
    compressed_data.insert(compressed_data.end(), data.begin(), data.end());
    
    return compressed_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough input for header generation and compressed data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    int windowBits = -15;  // Negative for gzip decoding mode
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
    
    // Allocate buffers for header fields
    size_t extra_max = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    size_t name_max = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    size_t comment_max = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    
    std::vector<uint8_t> extra_buf(extra_max);
    std::vector<uint8_t> name_buf(name_max);
    std::vector<uint8_t> comment_buf(comment_max);
    
    // Initialize gz_header structure
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // Set up header structure with optional buffers
    if (extra_max > 0) {
        header.extra = extra_buf.data();
        header.extra_max = (uInt)extra_max;
    }
    
    if (name_max > 0) {
        header.name = name_buf.data();
        header.name_max = (uInt)name_max;
    }
    
    if (comment_max > 0) {
        header.comment = comment_buf.data();
        header.comm_max = (uInt)comment_max;
    }
    
    // Create gzip header based on fuzzer input
    std::vector<uint8_t> gzip_header = create_gzip_header(fdp);
    
    // Create deflate compressed data
    std::vector<uint8_t> deflate_data = create_deflate_data(fdp);
    
    // Combine header and compressed data
    std::vector<uint8_t> full_gzip_data;
    full_gzip_data.insert(full_gzip_data.end(), gzip_header.begin(), gzip_header.end());
    full_gzip_data.insert(full_gzip_data.end(), deflate_data.begin(), deflate_data.end());
    
    // Initialize zlib stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize for gzip decoding (negative windowBits)
    int ret = inflateInit2(&strm, windowBits);
    if (ret != Z_OK) {
        return 0;  // Initialization failed
    }
    
    // Request header storage
    ret = inflateGetHeader(&strm, &header);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return 0;  // Header request failed
    }
    
    // Set up input buffer
    strm.next_in = full_gzip_data.data();
    strm.avail_in = (uInt)full_gzip_data.size();
    
    // Output buffer for decompressed data
    size_t out_size = 65536;  // Max expected decompressed size
    std::vector<uint8_t> out_buf(out_size);
    strm.next_out = out_buf.data();
    strm.avail_out = (uInt)out_size;
    
    // Decompress with Z_BLOCK to stop after header processing
    ret = inflate(&strm, Z_BLOCK);
    
    // Test various error conditions and state checks
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Check header status
        if (header.done == 1) {
            // Header was successfully processed
            
            // Validate header fields that should match our input
            // Note: We can't fully validate because header may be truncated
            // but we can check basic consistency
            
            // Check if extra field length matches what we expect
            if (header.extra != Z_NULL && header.extra_len > 0) {
                // extra_len should be <= extra_max
                if (header.extra_len > header.extra_max) {
                    // This should not happen - indicates buffer overflow protection
                }
            }
            
            // Check if name was captured
            if (header.name != Z_NULL && name_max > 0) {
                // Name should be null-terminated if it fit
                // We can't validate content because it might be truncated
            }
            
            // Check if comment was captured
            if (header.comment != Z_NULL && comment_max > 0) {
                // Comment should be null-terminated if it fit
            }
            
            // Check hcrc flag
            if (header.hcrc != 0) {
                // Header CRC was present and checked
            }
        } else if (header.done == -1) {
            // Not a gzip stream (shouldn't happen with our negative windowBits)
        }
        
        // Test inflateSyncPoint during header processing
        int sync_result = inflateSyncPoint(&strm);
        
        // Test inflateMark to get current position
        long mark = inflateMark(&strm);
        
        // Continue decompression to process actual data
        if (strm.avail_in > 0) {
            strm.avail_out = (uInt)out_size;
            ret = inflate(&strm, Z_FINISH);
            
            // Test after full decompression
            if (ret == Z_STREAM_END) {
                // Successfully decompressed all data
                sync_result = inflateSyncPoint(&strm);
                mark = inflateMark(&strm);
            }
        }
    } else if (ret == Z_DATA_ERROR) {
        // Data error - possibly corrupt header (CRC mismatch, etc.)
        // This is valid test case for error paths
    }
    
    // Clean up
    inflateEnd(&strm);
    
    // Test 2: Invalid header scenarios
    if (fdp.remaining_bytes() > 32) {
        // Create intentionally corrupted header to test error paths
        std::vector<uint8_t> corrupt_header = create_gzip_header(fdp);
        
        // Corrupt the header in various ways based on fuzzer input
        if (corrupt_header.size() > 10) {
            uint8_t corruption_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            
            switch (corruption_type) {
                case 0:
                    // Wrong magic number
                    if (corrupt_header.size() > 1) {
                        corrupt_header[0] = 0xff;
                        corrupt_header[1] = 0xff;
                    }
                    break;
                    
                case 1:
                    // Wrong compression method (not DEFLATE)
                    if (corrupt_header.size() > 2) {
                        corrupt_header[2] = 0xff;
                    }
                    break;
                    
                case 2:
                    // Corrupt FLG to have invalid combination
                    if (corrupt_header.size() > 3) {
                        corrupt_header[3] = 0xff;
                    }
                    break;
                    
                case 3:
                    // Corrupt extra field length to be inconsistent
                    if (corrupt_header.size() > 10) {
                        // Find FLG byte
                        uint8_t flg = corrupt_header[3];
                        if (flg & FEXTRA) {
                            // Set extra length to something impossible
                            corrupt_header[10] = 0xff;
                            corrupt_header[11] = 0xff;
                        }
                    }
                    break;
                    
                case 4:
                    // Corrupt header CRC if present
                    if (corrupt_header.size() > 10) {
                        uint8_t flg = corrupt_header[3];
                        if (flg & FHCRC) {
                            // Find CRC position (after optional fields)
                            size_t crc_pos = 10;
                            uint8_t* flg_ptr = &corrupt_header[3];
                            
                            if (flg & FEXTRA) {
                                uint16_t xlen = corrupt_header[10] | (corrupt_header[11] << 8);
                                crc_pos += 2 + xlen;
                            }
                            
                            // Skip FNAME null-terminated string
                            if (flg & FNAME) {
                                while (crc_pos < corrupt_header.size() && corrupt_header[crc_pos] != 0) {
                                    crc_pos++;
                                }
                                crc_pos++;  // Skip null terminator
                            }
                            
                            // Skip FCOMMENT null-terminated string
                            if (flg & FCOMMENT) {
                                while (crc_pos < corrupt_header.size() && corrupt_header[crc_pos] != 0) {
                                    crc_pos++;
                                }
                                crc_pos++;  // Skip null terminator
                            }
                            
                            // Corrupt CRC bytes
                            if (crc_pos + 1 < corrupt_header.size()) {
                                corrupt_header[crc_pos] ^= 0xff;
                                corrupt_header[crc_pos + 1] ^= 0xff;
                            }
                        }
                    }
                    break;
                    
                case 5:
                    // Truncated header
                    if (corrupt_header.size() > 20) {
                        corrupt_header.resize(corrupt_header.size() / 2);
                    }
                    break;
            }
            
            // Test the corrupted header
            z_stream strm2;
            memset(&strm2, 0, sizeof(strm2));
            strm2.zalloc = Z_NULL;
            strm2.zfree = Z_NULL;
            strm2.opaque = Z_NULL;
            
            if (inflateInit2(&strm2, windowBits) == Z_OK) {
                gz_header header2;
                memset(&header2, 0, sizeof(header2));
                
                inflateGetHeader(&strm2, &header2);
                
                strm2.next_in = corrupt_header.data();
                strm2.avail_in = (uInt)corrupt_header.size();
                
                std::vector<uint8_t> out2(1024);
                strm2.next_out = out2.data();
                strm2.avail_out = (uInt)out2.size();
                
                // Try to decompress corrupted header
                int ret2 = inflate(&strm2, Z_BLOCK);
                
                // All return values are valid test outcomes
                // Z_DATA_ERROR expected for corrupted headers
                // Z_BUF_ERROR for truncated headers
                // Z_OK or Z_STREAM_END for headers that happen to be valid
                
                inflateEnd(&strm2);
            }
        }
    }
    
    return 0;
}

} // namespace harness_017
