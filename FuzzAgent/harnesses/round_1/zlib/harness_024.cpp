#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <algorithm>

namespace harness_024 {
/*
 * Fuzzing harness for comprehensive gzip file decompression with various header configurations
 * Target: Unlock 443 blocked branches in inflate function focusing on gzip header processing
 * 
 * Based on coverage analysis guidance at /root/FuzzAgent/output/zlib/coverage_guidance/harness_024_guidance.md
 * 
 * Focus: Unlock deeply nested conditionals in inflate's HEAD, FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT
 * state handlers by testing gzip files with varied header configurations.
 * 
 * Target APIs:
 * - gzopen: Open gzip files with various headers (required helper)
 * - gzgets: 224 undiscovered branches, exercises line-based reading (primary target)
 * - gzread: 214 undiscovered branches, exercises byte-based reading (primary target)  
 * - gzclose: 115 undiscovered branches (cleanup)
 * - inflate: Internal target with 443 blocked branches in gzip processing
 * 
 * Header configurations:
 * - Extra fields (flags 0x0400) with different lengths and content
 * - Filename (flags 0x0800) with various lengths and encodings
 * - Comment (flags 0x1000) with different content
 * - CRC16 header checks (flags 0x0200)
 * - Different OS types (0-255) and modification times
 * - Various compression levels (0-9) and window sizes (8-15)
 * - Text flag (0x0100) for text vs binary
 * 
 * Invocation sequence:
 * 1. Create gzip test files in memory with varied header configurations
 * 2. Call gzopen to open each test file
 * 3. For each opened file:
 *    a. Call gzread with different buffer sizes (partial reads, full reads)
 *    b. Call gzgets to read line-by-line (for text files)
 *    c. Mix reading strategies to exercise buffering logic
 * 4. Call gzclose to properly clean up resources
 * 5. Include error handling paths (corrupted headers, truncated files)
 * 
 * Strategy: Generate gzip files programmatically with precise header configurations,
 * write to temporary files, then exercise gzopen/gzread/gzgets/gzclose APIs.
 * This directly targets the blocked branches in inflate's gzip header processing logic.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 256) {
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
    
    // Compression parameters
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Reading strategy selection
    uint8_t read_strategy = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0=gzread only, 1=gzgets only, 2=mixed, 3=error testing
    
    // Extra field data (if flag is set)
    size_t extra_field_size = 0;
    std::vector<uint8_t> extra_field_data;
    if (has_extra_field) {
        // Consume extra field size (must be even according to gzip spec)
        extra_field_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
        if (extra_field_size % 2 != 0) extra_field_size++;  // Ensure even size
        extra_field_data = fdp.ConsumeBytes<uint8_t>(extra_field_size);
    }
    
    // Filename (if flag is set)
    std::string filename_data;
    if (has_filename) {
        size_t filename_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        filename_data = fdp.ConsumeBytesAsString(filename_size);
        if (filename_size > 0 && filename_data.back() != '\0') {
            filename_data += '\0';  // Null-terminate for gzip header
        }
    }
    
    // Comment (if flag is set)
    std::string comment_data;
    if (has_comment) {
        size_t comment_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        comment_data = fdp.ConsumeBytesAsString(comment_size);
        if (comment_size > 0 && comment_data.back() != '\0') {
            comment_data += '\0';  // Null-terminate for gzip header
        }
    }
    
    // Test data to compress
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    if (fdp.remaining_bytes() < test_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Add some newlines for gzgets testing if text flag is set
    if (is_text && test_data_size > 10) {
        // Insert some newlines at random positions
        for (size_t i = 0; i < std::min(test_data_size / 10, (size_t)10); i++) {
            size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, test_data_size - 1);
            test_data[pos] = '\n';
        }
    }
    
    // ==============================================
    // Step 1: Create gzip file in memory with specified header
    // ==============================================
    
    // Create temporary file for gzip operations
    char temp_filename[] = "/tmp/gzip_fuzz_header_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;  // Failed to create temp file
    }
    
    // Initialize compression stream with gzip wrapper (windowBits = MAX_WBITS + 16)
    z_stream c_stream;
    memset(&c_stream, 0, sizeof(z_stream));
    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.opaque = Z_NULL;
    
    int windowBits_for_gzip = window_bits + 16;  // Add 16 for gzip wrapper
    int ret = deflateInit2(&c_stream, compression_level, Z_DEFLATED, 
                          windowBits_for_gzip, mem_level, strategy);
    if (ret != Z_OK) {
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    
    // Setup gzip header
    gz_header gzip_header;
    memset(&gzip_header, 0, sizeof(gzip_header));
    
    // Set basic header fields
    gzip_header.time = mtime;
    gzip_header.os = os_type;
    gzip_header.xflags = 0;  // Will be set based on flags
    
    // Set text flag if requested
    gzip_header.text = is_text ? 1 : 0;
    
    // Set extra field if flag is set
    if (has_extra_field && extra_field_data.size() > 0) {
        gzip_header.extra = extra_field_data.data();
        gzip_header.extra_len = (uInt)extra_field_data.size();
        gzip_header.extra_max = (uInt)extra_field_data.size();
    }
    
    // Set filename if flag is set
    if (has_filename && !filename_data.empty()) {
        gzip_header.name = (Bytef*)filename_data.c_str();
        gzip_header.name_max = (uInt)filename_data.size();
    }
    
    // Set comment if flag is set
    if (has_comment && !comment_data.empty()) {
        gzip_header.comment = (Bytef*)comment_data.c_str();
        gzip_header.comm_max = (uInt)comment_data.size();
    }
    
    // Set hcrc flag if requested
    gzip_header.hcrc = has_header_crc ? 1 : 0;
    
    // Set the header before starting compression
    ret = deflateSetHeader(&c_stream, &gzip_header);
    if (ret != Z_OK) {
        deflateEnd(&c_stream);
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    
    // Prepare output buffer
    size_t compressed_bound = deflateBound(&c_stream, test_data_size);
    std::vector<uint8_t> compressed_buffer(compressed_bound);
    
    // Compress the data
    c_stream.next_in = test_data.data();
    c_stream.avail_in = test_data_size;
    c_stream.next_out = compressed_buffer.data();
    c_stream.avail_out = compressed_bound;
    
    ret = deflate(&c_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&c_stream);
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    
    size_t compressed_size = compressed_bound - c_stream.avail_out;
    deflateEnd(&c_stream);
    
    // Write compressed data to temp file
    ssize_t written = write(fd, compressed_buffer.data(), compressed_size);
    if (written != (ssize_t)compressed_size) {
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    
    close(fd);
    
    // ==============================================
    // Step 2: Open gzip file with gzopen
    // ==============================================
    
    // Determine mode based on reading strategy
    const char* mode = "rb";  // Read binary mode for gzip
    
    gzFile gzfile = gzopen(temp_filename, mode);
    if (!gzfile) {
        unlink(temp_filename);
        return 0;  // Failed to open, could be due to corrupted header
    }
    
    // ==============================================
    // Step 3: Perform reading operations based on strategy
    // ==============================================
    
    switch (read_strategy) {
        case 0:  // gzread only - byte-based reading
        {
            // Read with different buffer sizes to exercise buffering logic
            size_t total_read = 0;
            std::vector<uint8_t> read_buffer(test_data_size);
            
            // Multiple partial reads
            while (total_read < test_data_size) {
                size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                chunk_size = std::min(chunk_size, test_data_size - total_read);
                
                int bytes_read = gzread(gzfile, read_buffer.data() + total_read, chunk_size);
                if (bytes_read <= 0) {
                    break;  // EOF or error
                }
                total_read += bytes_read;
            }
            
            // Try reading beyond EOF (should return 0)
            char dummy[16];
            gzread(gzfile, dummy, sizeof(dummy));
            break;
        }
        
        case 1:  // gzgets only - line-based reading (for text files)
        {
            if (is_text) {
                char line_buffer[1024];
                int lines_read = 0;
                
                // Read up to 100 lines or until EOF
                while (lines_read < 100 && gzgets(gzfile, line_buffer, sizeof(line_buffer)) != Z_NULL) {
                    lines_read++;
                }
                
                // Try reading after EOF
                gzgets(gzfile, line_buffer, sizeof(line_buffer));
            } else {
                // For binary files, gzgets still works but may not find newlines
                char buffer[256];
                for (int i = 0; i < 10; i++) {
                    gzgets(gzfile, buffer, sizeof(buffer));
                }
            }
            break;
        }
        
        case 2:  // Mixed reading strategy
        {
            // Alternate between gzread and gzgets
            std::vector<uint8_t> buffer(2048);
            char line_buf[512];
            
            for (int i = 0; i < 20; i++) {
                if (fdp.ConsumeBool()) {
                    // Do a gzread
                    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                    gzread(gzfile, buffer.data(), read_size);
                } else {
                    // Do a gzgets
                    gzgets(gzfile, line_buf, sizeof(line_buf));
                }
                
                // Check if we're at EOF
                if (gzeof(gzfile)) {
                    break;
                }
            }
            break;
        }
        
        case 3:  // Error testing and edge cases
        {
            // Test various error conditions
            // 1. Try reading with zero length
            char dummy[1];
            gzread(gzfile, dummy, 0);
            
            // 2. Try reading with NULL buffer (should be handled)
            // gzread(gzfile, NULL, 10);  // This would crash, skip
            
            // 3. Test gzeof and gzerror
            int errnum;
            const char* errmsg = gzerror(gzfile, &errnum);
            
            // 4. Test gzdirect to check if file is in gzip format
            int is_direct = gzdirect(gzfile);
            
            // 5. Test gztell to get current position
            z_off_t pos = gztell(gzfile);
            
            // 6. Try seeking (may fail for gzip files)
            if (pos > 100) {
                gzseek(gzfile, 50, SEEK_SET);
            }
            
            // 7. Test gzrewind
            gzrewind(gzfile);
            
            // 8. Clear any errors
            gzclearerr(gzfile);
            
            // Now do some actual reading
            std::vector<uint8_t> read_buf(1024);
            gzread(gzfile, read_buf.data(), std::min(read_buf.size(), test_data_size));
            break;
        }
    }
    
    // ==============================================
    // Step 4: Close gzip file with gzclose
    // ==============================================
    
    gzclose(gzfile);
    
    // ==============================================
    // Step 5: Additional tests with corrupted/truncated files
    // ==============================================
    
    // Test with truncated file (if we have enough remaining bytes)
    if (fdp.remaining_bytes() > 100 && compressed_size > 50) {
        // Create a truncated version of the file
        char truncated_filename[] = "/tmp/gzip_fuzz_trunc_XXXXXX";
        int trunc_fd = mkstemp(truncated_filename);
        if (trunc_fd >= 0) {
            // Write only first half of compressed data
            size_t trunc_size = compressed_size / 2;
            write(trunc_fd, compressed_buffer.data(), trunc_size);
            close(trunc_fd);
            
            // Try to open truncated file (should fail or give errors)
            gzFile trunc_file = gzopen(truncated_filename, "rb");
            if (trunc_file) {
                char buf[128];
                gzread(trunc_file, buf, sizeof(buf));
                int trunc_errnum;
                const char* trunc_errmsg = gzerror(trunc_file, &trunc_errnum);
                gzclose(trunc_file);
            }
            
            unlink(truncated_filename);
        }
    }
    // Clean up temp file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_024
