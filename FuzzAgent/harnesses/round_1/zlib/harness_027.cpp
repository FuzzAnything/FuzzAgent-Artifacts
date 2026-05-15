#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_027 {
/*
 * Fuzzing harness for gzip header processing to cover 465 blocked branches in inflate function
 * 
 * Based on coverage analysis guidance at /root/FuzzAgent/output/zlib/coverage_guidance/harness_027_guidance.md
 * 
 * Target: 465 blocked branches in inflate function (74% of reachable branches)
 * 
 * Focus: Gzip header processing states: FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC
 * 
 * Primary APIs: gzopen (init), gzgets (exercises gzip header reading), gzread (exercises decompression with headers)
 * Helper API: gzclose (cleanup)
 * Indirect target: inflate (via gzread/gzgets)
 * 
 * Test strategy:
 * 1. Generate gzip files with varied header configurations using fuzzer input
 * 2. Test gzopen with different modes and header flags
 * 3. Exercise gzgets for line-by-line reading with header processing
 * 4. Test gzread with different buffer sizes and seek positions
 * 5. Repeat with varied gzip header flags (text, extra fields, filename, comment, CRC)
 */


// Helper function to create a simple gzip header with specified flags
std::vector<uint8_t> create_gzip_header(FuzzedDataProvider& fdp, bool include_extra, bool include_name, 
                                       bool include_comment, bool include_crc, bool is_text) {
    std::vector<uint8_t> header;
    
    // Gzip magic bytes
    header.push_back(0x1f);  // ID1
    header.push_back(0x8b);  // ID2
    
    // Compression method (8 = DEFLATE)
    header.push_back(8);     // CM
    
    // FLAGS byte
    uint8_t flags = 0;
    if (is_text) flags |= 0x01;      // FTEXT
    if (include_extra) flags |= 0x04; // FEXTRA
    if (include_name) flags |= 0x08;  // FNAME
    if (include_comment) flags |= 0x10; // FCOMMENT
    if (include_crc) flags |= 0x02;   // FHCRC
    header.push_back(flags);
    
    // Modification time (4 bytes)
    uint32_t mtime = fdp.ConsumeIntegral<uint32_t>();
    header.push_back((mtime >> 0) & 0xFF);
    header.push_back((mtime >> 8) & 0xFF);
    header.push_back((mtime >> 16) & 0xFF);
    header.push_back((mtime >> 24) & 0xFF);
    
    // Extra flags and OS
    uint8_t xfl = fdp.ConsumeIntegral<uint8_t>() % 4; // 0-3
    header.push_back(xfl);  // XFL
    
    uint8_t os = fdp.ConsumeIntegral<uint8_t>() % 20; // 0-19 (various OS codes)
    header.push_back(os);   // OS
    
    // Extra field (if FEXTRA is set)
    if (include_extra) {
        // Extra field length (2 bytes, little-endian)
        uint16_t xlen = fdp.ConsumeIntegralInRange<uint16_t>(1, 512);
        header.push_back((xlen >> 0) & 0xFF);
        header.push_back((xlen >> 8) & 0xFF);
        
        // Extra field data
        std::vector<uint8_t> extra_data = fdp.ConsumeBytes<uint8_t>(xlen);
        header.insert(header.end(), extra_data.begin(), extra_data.end());
    }
    
    // File name (if FNAME is set)
    if (include_name) {
        std::string filename = fdp.ConsumeRandomLengthString(100);
        header.insert(header.end(), filename.begin(), filename.end());
        header.push_back(0); // Null terminator
    }
    
    // Comment (if FCOMMENT is set)
    if (include_comment) {
        std::string comment = fdp.ConsumeRandomLengthString(200);
        header.insert(header.end(), comment.begin(), comment.end());
        header.push_back(0); // Null terminator
    }
    
    // Header CRC (if FHCRC is set)
    if (include_crc) {
        // In real gzip, this would be CRC of header mod 2^16
        uint16_t crc16 = fdp.ConsumeIntegral<uint16_t>();
        header.push_back((crc16 >> 0) & 0xFF);
        header.push_back((crc16 >> 8) & 0xFF);
    }
    
    return header;
}

// Helper to create simple DEFLATE-compressed data
std::vector<uint8_t> create_deflate_data(FuzzedDataProvider& fdp, size_t max_size) {
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, max_size);
    std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(data_size);
    
    // Simple DEFLATE block: stored block (type 0) with the raw data
    std::vector<uint8_t> deflate_data;
    
    // BFINAL=1 (last block), BTYPE=00 (stored)
    deflate_data.push_back(0x01);
    
    // Length (little-endian, up to 65535)
    uint16_t len = raw_data.size();
    uint16_t nlen = ~len; // One's complement
    
    deflate_data.push_back((len >> 0) & 0xFF);
    deflate_data.push_back((len >> 8) & 0xFF);
    deflate_data.push_back((nlen >> 0) & 0xFF);
    deflate_data.push_back((nlen >> 8) & 0xFF);
    
    // The actual data
    deflate_data.insert(deflate_data.end(), raw_data.begin(), raw_data.end());
    
    return deflate_data;
}

// Helper to create a complete gzip file in memory
std::vector<uint8_t> create_gzip_file(FuzzedDataProvider& fdp, bool include_extra, bool include_name, 
                                     bool include_comment, bool include_crc, bool is_text) {
    std::vector<uint8_t> gzip_file;
    
    // Create header
    std::vector<uint8_t> header = create_gzip_header(fdp, include_extra, include_name, 
                                                    include_comment, include_crc, is_text);
    gzip_file.insert(gzip_file.end(), header.begin(), header.end());
    
    // Create compressed data (simple stored block for fuzzing)
    std::vector<uint8_t> deflate_data = create_deflate_data(fdp, 1024);
    gzip_file.insert(gzip_file.end(), deflate_data.begin(), deflate_data.end());
    
    // CRC32 (4 bytes) - using fuzzed value
    uint32_t crc32 = fdp.ConsumeIntegral<uint32_t>();
    gzip_file.push_back((crc32 >> 0) & 0xFF);
    gzip_file.push_back((crc32 >> 8) & 0xFF);
    gzip_file.push_back((crc32 >> 16) & 0xFF);
    gzip_file.push_back((crc32 >> 24) & 0xFF);
    
    // ISIZE (input size modulo 2^32, 4 bytes)
    uint32_t isize = deflate_data.size() > 4 ? deflate_data.size() - 4 : 0; // Approximate
    gzip_file.push_back((isize >> 0) & 0xFF);
    gzip_file.push_back((isize >> 8) & 0xFF);
    gzip_file.push_back((isize >> 16) & 0xFF);
    gzip_file.push_back((isize >> 24) & 0xFF);
    
    return gzip_file;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for configuration and gzip file generation
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==============================================
    // Consume configuration parameters
    // ==============================================
    
    // Determine which header flags to include
    bool include_extra = fdp.ConsumeBool();
    bool include_name = fdp.ConsumeBool();
    bool include_comment = fdp.ConsumeBool();
    bool include_crc = fdp.ConsumeBool();
    bool is_text = fdp.ConsumeBool();
    
    // Determine gzopen mode
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    const char* mode;
    switch (mode_choice) {
        case 0: mode = "rb"; break;   // Read binary
        case 1: mode = "r"; break;    // Read text
        case 2: mode = "rb9"; break;  // Read with compression level
        case 3: mode = "rT"; break;   // Read text with transparent mode
        default: mode = "rb"; break;
    }
    
    // Determine test scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=gzgets, 1=gzread, 2=both
    
    // Determine buffer sizes for gzread
    size_t read_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    size_t line_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    // Determine if we should test seeking
    bool test_seek = fdp.ConsumeBool();
    
    // ==============================================
    // Create gzip file in memory
    // ==============================================
    
    std::vector<uint8_t> gzip_file = create_gzip_file(fdp, include_extra, include_name, 
                                                     include_comment, include_crc, is_text);
    
    // Write gzip file to temporary file
    char temp_filename[] = "/tmp/gzip_test_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write gzip data to temp file
    ssize_t written = write(fd, gzip_file.data(), gzip_file.size());
    close(fd);
    
    if (written != (ssize_t)gzip_file.size()) {
        unlink(temp_filename);
        return 0;
    }
    
    // ==============================================
    // Test gzopen and gzip header processing
    // ==============================================
    
    gzFile gz = gzopen(temp_filename, mode);
    if (gz == NULL) {
        unlink(temp_filename);
        return 0;
    }
    
    // ==============================================
    // Scenario 0: Test gzgets (line-by-line reading)
    // ==============================================
    if (scenario == 0 || scenario == 2) {
        char* line_buffer = (char*)malloc(line_buffer_size);
        if (line_buffer != NULL) {
            // Read lines until EOF or buffer limit
            int lines_read = 0;
            while (lines_read < 10 && gzgets(gz, line_buffer, line_buffer_size) != NULL) {
                lines_read++;
                
                // Potentially test gztell/gzrewind
                if (test_seek && lines_read % 3 == 0) {
                    z_off_t pos = gztell(gz);
                    if (pos > 0 && fdp.ConsumeBool()) {
                        gzrewind(gz);
                    }
                }
            }
            free(line_buffer);
        }
        
        // Reset to beginning for next test
        gzrewind(gz);
    }
    
    // ==============================================
    // Scenario 1: Test gzread (buffer reading)
    // ==============================================
    if (scenario == 1 || scenario == 2) {
        char* read_buffer = (char*)malloc(read_buffer_size);
        if (read_buffer != NULL) {
            int bytes_read;
            int total_bytes = 0;
            
            do {
                bytes_read = gzread(gz, read_buffer, read_buffer_size);
                if (bytes_read > 0) {
                    total_bytes += bytes_read;
                    
                    // Test gztell during reading
                    if (test_seek && total_bytes % 512 == 0) {
                        z_off_t pos = gztell(gz);
                        
                        // Possibly seek to a different position
                        if (pos > 100 && fdp.ConsumeBool()) {
                            z_off_t new_pos = fdp.ConsumeIntegralInRange<z_off_t>(0, pos);
                            gzseek(gz, new_pos, SEEK_SET);
                        }
                    }
                }
            } while (bytes_read > 0 && total_bytes < 8192);
            
            free(read_buffer);
        }
        
        // Test gzeof and gzerror
        int eof_check = gzeof(gz);
        const char* error_msg = gzerror(gz, NULL);
        (void)eof_check; // Mark as used
        (void)error_msg; // Mark as used
        
        // Reset for potential reuse
        gzrewind(gz);
    }
    
    // ==============================================
    // Test with different gzip header configurations
    // ==============================================
    
    // Test a second gzip file with different header configuration
    if (fdp.remaining_bytes() > 100) {
        // Close current file
        gzclose(gz);
        
        // Create new gzip file with opposite header flags
        std::vector<uint8_t> gzip_file2 = create_gzip_file(fdp, !include_extra, !include_name, 
                                                          !include_comment, !include_crc, !is_text);
        
        // Write to a new temp file
        char temp_filename2[] = "/tmp/gzip_test2_XXXXXX";
        int fd2 = mkstemp(temp_filename2);
        if (fd2 >= 0) {
            write(fd2, gzip_file2.data(), gzip_file2.size());
            close(fd2);
            
            // Open and test briefly
            gzFile gz2 = gzopen(temp_filename2, mode);
            if (gz2 != NULL) {
                // Quick test of header processing
                char test_buf[100];
                int test_read = gzread(gz2, test_buf, sizeof(test_buf));
                (void)test_read; // Mark as used
                
                gzclose(gz2);
            }
            
            unlink(temp_filename2);
        }
        
        // Reopen original for cleanup
        gz = gzopen(temp_filename, mode);
    }
    
    // ==============================================
    // Cleanup
    // ==============================================
    
    if (gz != NULL) {
        gzclose(gz);
    }
    
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_027
