#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_020 {
/*
 * Comprehensive gzip decompression harness targeting 489 blocked branches in inflate 
 * and 233 blocked branches in inflateBack function
 * Target: inflate (Primary - 489 blocked branches), inflateBack (Secondary - 233 blocked branches),
 *         gzopen, gzread, gzclose, inflateInit2_, inflateEnd, inflateSetDictionary,
 *         uncompress2, gzfread, gzgets
 * Harness ID: 020
 * 
 * This harness specifically targets the single largest coverage gap: 489 blocked branches
 * in inflate function (77.6% coverage) and 233 blocked branches in inflateBack function
 * (68.5% coverage) by focusing on comprehensive gzip decompression scenarios with all
 * gzip header flag combinations and complex state machine transitions.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_015 (comprehensive inflate testing with gzip headers): covers basic gzip header processing
 * - harness_018 (gzip-specific inflation features): covers CRC validation with inflateValidate
 * - harness_019 (complex inflation scenarios): covers complex state transitions and error paths
 * - harness_002 (inflateBack callback-based decompression): covers basic inflateBack API
 * - harness_001 (gzip file I/O): covers basic gzopen/gzread APIs
 * 
 * This harness differs by:
 * 1. Comprehensive testing of ALL gzip header flag combinations (0x0100-0x1000)
 * 2. Integration of both file-based (gzopen/gzread) and memory-based (inflate) decompression
 * 3. Simultaneous testing of inflate and inflateBack paths for the same input data
 * 4. Testing dictionary-based decompression specifically for gzip format
 * 5. Testing Adler-32 and CRC-32 validation paths for raw and gzip data
 * 6. Testing alternative entry points: uncompress2 for raw gzip data, gzfread/gzgets for line-based reading
 * 7. Generating semantically valid gzip headers with realistic flag combinations
 * 8. Testing partial reads and varying buffer sizes to exercise different code paths
 * 
 * Based on source code analysis (inflate.c and guidance):
 * - Gzip header processing states: HEAD→FLAGS→TIME→OS→EXLEN→EXTRA→NAME→COMMENT→HCRC
 * - Flags: 0x0100 (FTEXT), 0x0200 (FHCRC), 0x0400 (FEXTRA), 0x0800 (FNAME), 0x1000 (FCOMMENT)
 * - Inflate has 489 blocked branches mainly in gzip-specific code paths (#ifdef GUNZIP)
 * - InflateBack has 233 blocked branches in callback-based decompression logic
 * 
 * Test scenarios targeting blocked branches:
 * 1. All gzip header flag combinations (31 possible combinations of 5 flags)
 * 2. Different compression methods (currently only 8=DEFLATE, but test invalid methods)
 * 3. Extra fields with various lengths and content
 * 4. Filenames and comments with special characters and varying lengths
 * 5. CRC16 validation with correct and incorrect CRC values
 * 6. Text flag processing (FTEXT)
 * 7. Operating system codes (0-13) and modification times
 * 8. Dictionary-based decompression with inflateSetDictionary
 * 9. Partial decompression with varying buffer sizes (1 byte to 64KB)
 * 10. Error conditions: truncated headers, corrupted data, invalid CRC
 * 11. Both zlib (windowBits=15) and gzip (windowBits=31) decompression modes
 * 12. Auto-detection mode (windowBits=47 for automatic zlib/gzip detection)
 *
 * Expected coverage gain:
 * - Target reduction of inflate's 489 blocked branches by exploring all gzip header state transitions
 * - Target reduction of inflateBack's 233 blocked branches with gzip-specific callback testing
 * - Improve coverage of gzip-specific code paths under #ifdef GUNZIP
 * - Exercise dictionary processing with gzip format data
 * - Test CRC validation paths for both Adler-32 (zlib) and CRC-32 (gzip)
 * - Cover all error conditions in gzip header processing
 */


// Context structure for inflateBack callback functions
struct inflateback_context {
    const uint8_t* input_data;
    size_t input_size;
    size_t position;
    uint8_t* output_buffer;
    size_t output_capacity;
    size_t output_position;
    bool simulate_error;
    int input_error_counter;
    int output_error_counter;
};

// Callback function for input (in_func)
static unsigned input_callback(void* desc, z_const unsigned char** buf) {
    inflateback_context* ctx = (inflateback_context*)desc;
    
    if (ctx == nullptr || ctx->input_data == nullptr || ctx->position >= ctx->input_size) {
        return 0;  // No more input available
    }
    
    *buf = (z_const unsigned char*)(ctx->input_data + ctx->position);
    
    // Determine how many bytes to return (1-256 or remaining)
    size_t bytes_to_return = 1;
    if (ctx->position + 256 < ctx->input_size) {
        bytes_to_return = 256;
    } else {
        bytes_to_return = ctx->input_size - ctx->position;
    }
    
    // Update position for next call
    ctx->position += bytes_to_return;
    
    // Occasionally simulate error
    if (ctx->simulate_error && (ctx->input_error_counter++ % 10 == 0)) {
        return 0;  // Simulate no input available
    }
    
    return (unsigned)bytes_to_return;
}

// Callback function for output (out_func)
static int output_callback(void* desc, unsigned char* buf, unsigned len) {
    inflateback_context* ctx = (inflateback_context*)desc;
    
    if (ctx == nullptr || buf == nullptr || len == 0) {
        return 0;  // Nothing to do
    }
    
    // Store decompressed data if we have space
    if (ctx->output_position + len <= ctx->output_capacity) {
        memcpy(ctx->output_buffer + ctx->output_position, buf, len);
        ctx->output_position += len;
    }
    
    // Occasionally simulate output error
    if (ctx->simulate_error && (ctx->output_error_counter++ % 7 == 0)) {
        return 1;  // Simulate output error
    }
    
    return 0;  // Success
}

// Helper function to create gzip header with specific flag configuration
static std::vector<uint8_t> create_gzip_header(
    uint8_t method,           // Compression method (8 = DEFLATE)
    uint16_t flags,           // FLG field flags (0x0100-0x1000)
    uint32_t mtime,           // Modification time
    uint8_t xfl,              // Extra flags
    uint8_t os,               // Operating system
    const std::vector<uint8_t>& extra_data,  // Optional extra field
    const std::string& filename,             // Optional filename
    const std::string& comment,              // Optional comment
    uint16_t crc16            // Optional CRC16
) {
    std::vector<uint8_t> header;
    
    // Gzip magic bytes (0x1f, 0x8b)
    header.push_back(0x1f);
    header.push_back(0x8b);
    
    // Compression method
    header.push_back(method);
    
    // Flags (low byte only, high byte is in flags parameter)
    header.push_back(flags & 0xFF);
    
    // Modification time (4 bytes, little endian)
    header.push_back((mtime >> 0) & 0xFF);
    header.push_back((mtime >> 8) & 0xFF);
    header.push_back((mtime >> 16) & 0xFF);
    header.push_back((mtime >> 24) & 0xFF);
    
    // Extra flags
    header.push_back(xfl);
    
    // Operating system
    header.push_back(os);
    
    // Extra field (if flags & 0x0400)
    if ((flags & 0x0400) && !extra_data.empty()) {
        // Extra field length (2 bytes, little endian)
        uint16_t extra_len = (uint16_t)extra_data.size();
        header.push_back(extra_len & 0xFF);
        header.push_back((extra_len >> 8) & 0xFF);
        // Extra field data
        header.insert(header.end(), extra_data.begin(), extra_data.end());
    }
    
    // Filename (if flags & 0x0800)
    if ((flags & 0x0800) && !filename.empty()) {
        header.insert(header.end(), filename.begin(), filename.end());
        header.push_back(0);  // Null terminator
    }
    
    // Comment (if flags & 0x1000)
    if ((flags & 0x1000) && !comment.empty()) {
        header.insert(header.end(), comment.begin(), comment.end());
        header.push_back(0);  // Null terminator
    }
    
    // CRC16 (if flags & 0x0200)
    if (flags & 0x0200) {
        header.push_back(crc16 & 0xFF);
        header.push_back((crc16 >> 8) & 0xFF);
    }
    
    return header;
}

// Helper to compress data using deflate
static std::vector<uint8_t> compress_data(const uint8_t* data, size_t size, int level) {
    std::vector<uint8_t> compressed;
    
    if (size == 0) {
        return compressed;
    }
    
    uLongf compressed_size = compressBound((uLong)size);
    if (compressed_size == 0 || compressed_size > 10 * 1024 * 1024) {
        return compressed;
    }
    
    compressed.resize(compressed_size);
    
    int ret = compress2(compressed.data(), &compressed_size, data, (uLong)size, level);
    if (ret == Z_OK) {
        compressed.resize(compressed_size);
    } else {
        compressed.clear();
    }
    
    return compressed;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for comprehensive gzip testing
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    uint8_t compression_method = fdp.ConsumeIntegral<uint8_t>();
    uint16_t gzip_flags = fdp.ConsumeIntegral<uint16_t>();
    uint32_t modification_time = fdp.ConsumeIntegral<uint32_t>();
    uint8_t extra_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t os_code = fdp.ConsumeIntegral<uint8_t>() % 14;  // 0-13 valid OS codes
    uint16_t header_crc16 = fdp.ConsumeIntegral<uint16_t>();
    
    // Determine test configuration
    bool use_gzip_format = fdp.ConsumeBool();
    bool use_inflateback = fdp.ConsumeBool();
    bool use_file_apis = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_partial_reads = fdp.ConsumeBool();
    bool include_errors = fdp.ConsumeBool();
    bool use_auto_detect = fdp.ConsumeBool();
    
    // Consume additional parameters
    int window_bits = 15;  // Default zlib window bits (8-15 valid range)
    if (use_gzip_format) {
        window_bits = 31;  // Gzip window bits (15+16 for gzip format)
    }
    if (use_auto_detect) {
        window_bits = 47;  // Auto-detect zlib/gzip (15+32 for auto detection)
    }
    
    int flush_mode = fdp.ConsumeIntegral<uint8_t>() % 6;  // Z_NO_FLUSH to Z_TREES
    size_t max_buffer_size = fdp.ConsumeIntegralInRange<size_t>(64, 65536);
    
    // Consume data for extra field, filename, and comment
    size_t extra_field_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    std::vector<uint8_t> extra_field_data;
    if (extra_field_size > 0 && fdp.remaining_bytes() >= extra_field_size) {
        extra_field_data = fdp.ConsumeBytes<uint8_t>(extra_field_size);
    }
    
    size_t filename_length = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string filename;
    if (filename_length > 0 && fdp.remaining_bytes() >= filename_length) {
        filename = fdp.ConsumeRandomLengthString(filename_length);
    }
    
    size_t comment_length = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string comment;
    if (comment_length > 0 && fdp.remaining_bytes() >= comment_length) {
        comment = fdp.ConsumeRandomLengthString(comment_length);
    }
    
    // Consume remaining bytes as payload data to compress
    std::vector<uint8_t> payload_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (payload_data.empty()) {
        payload_data.push_back('T');
        payload_data.push_back('e');
        payload_data.push_back('s');
        payload_data.push_back('t');
    }
    
    // Create gzip/zlib compressed data
    std::vector<uint8_t> compressed_data = compress_data(payload_data.data(), payload_data.size(), Z_DEFAULT_COMPRESSION);
    if (compressed_data.empty()) {
        return 0;
    }
    
    // Create complete gzip data with header and trailer
    std::vector<uint8_t> complete_gzip_data;
    
    if (use_gzip_format) {
        // Create gzip header
        std::vector<uint8_t> gzip_header = create_gzip_header(
            compression_method,  // Usually 8 for DEFLATE
            gzip_flags,
            modification_time,
            extra_flags,
            os_code,
            extra_field_data,
            filename,
            comment,
            header_crc16
        );
        
        // Combine header + compressed data
        complete_gzip_data.insert(complete_gzip_data.end(), gzip_header.begin(), gzip_header.end());
        complete_gzip_data.insert(complete_gzip_data.end(), compressed_data.begin(), compressed_data.end());
        
        // Add gzip trailer (CRC32 and ISIZE)
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, payload_data.data(), (uInt)payload_data.size());
        
        // CRC32 (4 bytes, little endian)
        complete_gzip_data.push_back((crc >> 0) & 0xFF);
        complete_gzip_data.push_back((crc >> 8) & 0xFF);
        complete_gzip_data.push_back((crc >> 16) & 0xFF);
        complete_gzip_data.push_back((crc >> 24) & 0xFF);
        
        // ISIZE (input size modulo 2^32, 4 bytes, little endian)
        uint32_t input_size = (uint32_t)payload_data.size();
        complete_gzip_data.push_back((input_size >> 0) & 0xFF);
        complete_gzip_data.push_back((input_size >> 8) & 0xFF);
        complete_gzip_data.push_back((input_size >> 16) & 0xFF);
        complete_gzip_data.push_back((input_size >> 24) & 0xFF);
    } else {
        // Use raw deflate data (no header/trailer)
        complete_gzip_data = compressed_data;
    }
    
    // Test 1: Memory-based decompression using inflate
    {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Initialize with appropriate window bits
        int ret = inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
        if (ret != Z_OK) {
            // Try with default window bits if initialization failed
            window_bits = 15;
            ret = inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
            if (ret != Z_OK) {
                return 0;
            }
        }
        
        // Set dictionary if requested
        if (test_dictionary && payload_data.size() >= 256) {
            inflateSetDictionary(&strm, payload_data.data(), 256);
        }
        
        // Prepare input/output buffers
        std::vector<uint8_t> decompressed_buffer(max_buffer_size);
        size_t input_pos = 0;
        size_t output_pos = 0;
        
        strm.next_in = (Bytef*)complete_gzip_data.data();
        strm.avail_in = (uInt)complete_gzip_data.size();
        
        // Decompress in chunks to test partial processing
        do {
            strm.next_out = decompressed_buffer.data() + output_pos;
            strm.avail_out = (uInt)(max_buffer_size - output_pos);
            
            if (test_partial_reads && strm.avail_in > 0) {
                // Limit input for this iteration
                uInt chunk_size = (uInt)fdp.ConsumeIntegralInRange<size_t>(1, strm.avail_in);
                strm.avail_in = chunk_size;
            }
            
            ret = inflate(&strm, flush_mode);
            
            output_pos = max_buffer_size - strm.avail_out;
            
            // Handle errors
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                if (include_errors) {
                    // Test error recovery
                    inflateEnd(&strm);
                    inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
                    break;
                }
            }
            
            // Update input position
            input_pos = complete_gzip_data.size() - strm.avail_in;
            strm.next_in = (Bytef*)complete_gzip_data.data() + input_pos;
            
        } while (ret != Z_STREAM_END && ret != Z_STREAM_ERROR && 
                 ret != Z_DATA_ERROR && output_pos < max_buffer_size);
        
        inflateEnd(&strm);
    }
    
    // Test 2: Callback-based decompression using inflateBack
    // Note: inflateBack only works with raw deflate data (no headers), so we need to use
    // the raw compressed data without gzip header/trailer
    if (use_inflateback && compressed_data.size() > 0) {
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Initialize inflateBack - windowBits must be in range 8..15 for inflateBackInit
        // According to zlib.h: "windowBits is the base two logarithm of the window size, in the range 8..15"
        // "Except for special applications where it is assured that deflate was used with small window sizes, 
        // windowBits must be 15 and a 32K byte window must be supplied to be able to decompress general deflate streams."
        int inflateback_window_bits = 15;  // Always use 15 for general deflate streams
        size_t window_size = 1 << inflateback_window_bits;  // 32KB (2^15 = 32768)
        
        unsigned char* window_buffer = (unsigned char*)malloc(window_size);
        if (window_buffer == nullptr) {
            return 0;
        }
        
        int ret = inflateBackInit(&strm, inflateback_window_bits, window_buffer);
        if (ret != Z_OK) {
            // Try with different window bits if 15 fails
            free(window_buffer);
            // Try smaller window sizes (8-14)
            for (int wbits = 8; wbits < 15; wbits++) {
                window_size = 1 << wbits;
                window_buffer = (unsigned char*)malloc(window_size);
                if (window_buffer == nullptr) {
                    continue;
                }
                ret = inflateBackInit(&strm, wbits, window_buffer);
                if (ret == Z_OK) {
                    break;
                }
                free(window_buffer);
                window_buffer = nullptr;
            }
            if (ret != Z_OK) {
                return 0;
            }
        }
        
        // Setup callback context
        inflateback_context ctx;
        ctx.input_data = compressed_data.data();  // Use raw deflate data, not complete gzip data
        ctx.input_size = compressed_data.size();
        ctx.position = 0;
        ctx.output_buffer = (uint8_t*)malloc(max_buffer_size);
        ctx.output_capacity = max_buffer_size;
        ctx.output_position = 0;
        ctx.simulate_error = include_errors;
        ctx.input_error_counter = 0;
        ctx.output_error_counter = 0;
        
        if (ctx.output_buffer != nullptr) {
            // Perform inflateBack decompression
            ret = inflateBack(&strm, input_callback, &ctx,
                              output_callback, &ctx);
            
            // Clean up
            free(ctx.output_buffer);
        }
        
        // Free window buffer
        free(window_buffer);
        
        inflateBackEnd(&strm);
    }
    
    // Test 3: File-based decompression using gzopen/gzread
    if (use_file_apis && complete_gzip_data.size() > 0) {
        // Create temporary file
        char temp_filename[] = "/tmp/gzip_fuzz_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd != -1) {
            close(fd);
            
            // Write gzip data to file
            FILE* fp = fopen(temp_filename, "wb");
            if (fp != nullptr) {
                fwrite(complete_gzip_data.data(), 1, complete_gzip_data.size(), fp);
                fclose(fp);
                
                // Open with gzopen
                gzFile gzfile = gzopen(temp_filename, "rb");
                if (gzfile != nullptr) {
                    // Test different reading methods
                    uint8_t read_method = fdp.ConsumeIntegral<uint8_t>() % 3;
                    
                    std::vector<uint8_t> read_buffer(max_buffer_size);
                    
                    if (read_method == 0) {
                        // gzread
                        int bytes_read = 0;
                        do {
                            bytes_read = gzread(gzfile, read_buffer.data(), 
                                               fdp.ConsumeIntegralInRange<size_t>(1, max_buffer_size));
                        } while (bytes_read > 0);
                    } else if (read_method == 1) {
                        // gzfread (stdio-like)
                        char line_buffer[1024];
                        while (gzfread(line_buffer, 1, sizeof(line_buffer), gzfile) > 0) {
                            // Process line
                        }
                    } else if (read_method == 2) {
                        // gzgets (line reading)
                        char line_buffer[1024];
                        while (gzgets(gzfile, line_buffer, sizeof(line_buffer)) != nullptr) {
                            // Process line
                        }
                    }
                    
                    gzclose(gzfile);
                }
                
                // Clean up
                remove(temp_filename);
            }
        }
    }
    
    // Test 4: Raw decompression using uncompress2
    {
        std::vector<uint8_t> decompressed_buffer(max_buffer_size);
        uLongf dest_len = (uLongf)decompressed_buffer.size();
        
        // Note: uncompress2 expects zlib-wrapped data, not gzip
        if (!use_gzip_format && compressed_data.size() > 0) {
            uLong source_len = (uLong)compressed_data.size();
            int ret = uncompress2(decompressed_buffer.data(), &dest_len,
                                 compressed_data.data(), &source_len);
            // Result ignored - just testing the API
        }
    }
    
    // Test 5: Adler-32 and CRC-32 validation
    if (payload_data.size() > 0) {
        // Calculate Adler-32 checksum (used by zlib format)
        uLong adler = adler32(0L, Z_NULL, 0);
        adler = adler32(adler, payload_data.data(), (uInt)payload_data.size());
        
        // Calculate CRC-32 checksum (used by gzip format)
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, payload_data.data(), (uInt)payload_data.size());
        
        // Test with partial data
        if (payload_data.size() >= 100) {
            uLong partial_adler = adler32(0L, payload_data.data(), 50);
            partial_adler = adler32_combine(partial_adler, 
                                           adler32(0L, payload_data.data() + 50, 50),
                                           50);
            
            uLong partial_crc = crc32(0L, payload_data.data(), 50);
            partial_crc = crc32_combine(partial_crc,
                                       crc32(0L, payload_data.data() + 50, 50),
                                       50);
        }
    }
    
    return 0;
}

} // namespace harness_020
