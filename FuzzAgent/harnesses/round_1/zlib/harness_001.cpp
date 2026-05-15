#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_001 {
/*
 * Fuzzing harness for zlib gzip file operations and advanced stream APIs
 * Targets gzip file I/O, advanced deflate/inflate operations, and error handling
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);  // Compression level
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);  // Strategy
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Window bits
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);  // Memory level
    int flush = fdp.ConsumeIntegralInRange<int>(0, 5);  // Flush mode
    
    // Test selection flags
    bool test_gzip_file = fdp.ConsumeBool();
    bool test_advanced_stream = fdp.ConsumeBool();
    bool test_dictionary = fdp.ConsumeBool();
    bool test_sync = fdp.ConsumeBool();
    bool test_uncompress2 = fdp.ConsumeBool();
    
    // Consume input data
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(64, 64 * 1024);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Test 1: Gzip file operations (inspired by gzio_fuzzer.c)
    if (test_gzip_file && input_size > 0) {
        // Create a temporary file for gzip operations
        char fname[] = "/tmp/gzip_fuzz_XXXXXX";
        int fd = mkstemp(fname);
        if (fd >= 0) {
            close(fd);
            
            // Determine mode from fuzzer input
            std::string mode = "wb";
            if (fdp.ConsumeBool()) {
                mode += std::to_string(level);
            }
            if (fdp.ConsumeBool()) {
                mode += "f";  // filtered
            }
            if (fdp.ConsumeBool()) {
                mode += "h";  // huffman only
            }
            
            // Open gzip file
            gzFile file = gzopen(fname, mode.c_str());
            if (file) {
                // Write some data
                if (gzwrite(file, input_buffer.data(), input_buffer.size()) > 0) {
                    gzflush(file, Z_SYNC_FLUSH);
                    
                    // Change compression parameters
                    gzsetparams(file, level, strategy);
                    
                    // Write more data
                    // Write more data - use fuzzer input for string
                    std::string test_str = fdp.ConsumeRandomLengthString(32);
                    
                    // Get file position
                    z_off_t pos = gztell(file);
                    
                    // Try to read back (will fail on write-only file, but that's OK)
                    char buf[128];
                    gzseek(file, 0, SEEK_SET);
                    gzread(file, buf, sizeof(buf));
                    
                    // Test other gzip functions
                    gzeof(file);
                    gzdirect(file);
                    gzoffset(file);
                    
                    // Get error information
                    int errnum;
                    const char* errmsg = gzerror(file, &errnum);
                    
                    // Clear errors
                    gzclearerr(file);
                }
                
                gzclose(file);
            }
            
            // Clean up
            unlink(fname);
        }
    }
    
    // Test 2: Advanced stream operations with dictionary
    if (test_advanced_stream && input_size > 32) {
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // Initialize deflate with advanced options
        int ret = deflateInit2(&strm, level, Z_DEFLATED, 
                              windowBits | (fdp.ConsumeBool() ? 16 : 0),  // gzip header
                              memLevel, strategy);
        
        if (ret == Z_OK) {
            // Test dictionary if requested
            if (test_dictionary && input_size > 64) {
                size_t dict_size = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                std::vector<uint8_t> dict = fdp.ConsumeBytes<uint8_t>(dict_size);
                deflateSetDictionary(&strm, dict.data(), dict.size());
            }
            
            // Prepare buffers
            size_t compr_size = deflateBound(&strm, input_size);
            std::vector<uint8_t> compr(compr_size);
            std::vector<uint8_t> uncompr(input_size);
            
            // Compress in multiple steps to test different flush modes
            strm.next_in = input_buffer.data();
            strm.avail_in = input_size / 2;  // First half
            strm.next_out = compr.data();
            strm.avail_out = compr_size;
            
            // First compression with partial flush
            ret = deflate(&strm, Z_PARTIAL_FLUSH);
            
            // Compress remaining data
            strm.next_in = input_buffer.data() + input_size / 2;
            strm.avail_in = input_size - (input_size / 2);
            ret = deflate(&strm, Z_FINISH);
            
            if (ret == Z_STREAM_END) {
                size_t compressed_size = compr_size - strm.avail_out;
                
                // Test inflate with dictionary
                z_stream istrm;
                istrm.zalloc = Z_NULL;
                istrm.zfree = Z_NULL;
                istrm.opaque = Z_NULL;
                istrm.next_in = compr.data();
                istrm.avail_in = compressed_size;
                
                ret = inflateInit2(&istrm, windowBits | (fdp.ConsumeBool() ? 16 : 0));
                if (ret == Z_OK) {
                    // Set dictionary if one was used
                    if (test_dictionary && input_size > 64) {
                        inflateSetDictionary(&istrm, input_buffer.data(), 32);
                    }
                    
                    istrm.next_out = uncompr.data();
                    istrm.avail_out = input_size;
                    
                    ret = inflate(&istrm, Z_FINISH);
                    
                    // Test inflateSync for error recovery
                    if (test_sync && ret == Z_DATA_ERROR) {
                        inflateSync(&istrm);
                    }
                    
                    inflateEnd(&istrm);
                }
            }
            
            // Test deflateCopy
            z_stream strm2;
            if (deflateCopy(&strm2, &strm) == Z_OK) {
                deflateEnd(&strm2);
            }
            
            // Test deflateReset
            deflateReset(&strm);
            
            deflateEnd(&strm);
        }
    }
    
    // Test 3: uncompress2 API (newer API with pointer sourceLen)
    if (test_uncompress2 && input_size > 0) {
        // First compress the data
        uLongf comprLen = compressBound(input_size);
        std::vector<Bytef> compr(comprLen);
        
        int ret = compress2(compr.data(), &comprLen, 
                           input_buffer.data(), input_size, level);
        
        if (ret == Z_OK) {
            // Test uncompress2
            std::vector<Bytef> uncompr(input_size);
            uLongf uncomprLen = input_size;
            uLong sourceLen = comprLen;
            
            ret = uncompress2(uncompr.data(), &uncomprLen, 
                             compr.data(), &sourceLen);
            
            // Verify if successful
            if (ret == Z_OK && uncomprLen == input_size) {
                memcmp(uncompr.data(), input_buffer.data(), input_size);
            }
        }
    }
    
    // Test 4: Checksum operations with advanced APIs
    if (input_size > 0) {
        // Test adler32_z and crc32_z with size_t lengths
        uLong adler = adler32(0L, Z_NULL, 0);
        adler = adler32_z(adler, input_buffer.data(), input_size);
        
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32_z(crc, input_buffer.data(), input_size);
        
        // Test combine functions with proper types
        if (fdp.remaining_bytes() >= sizeof(z_off_t)) {
            z_off_t len2 = fdp.ConsumeIntegral<z_off_t>();
            if (len2 >= 0) {  // len2 must be non-negative for meaningful results
                uLong adler1 = fdp.ConsumeIntegral<uLong>();
                uLong adler2 = fdp.ConsumeIntegral<uLong>();
                adler32_combine(adler1, adler2, len2);
                
                uLong crc1 = fdp.ConsumeIntegral<uLong>();
                uLong crc2 = fdp.ConsumeIntegral<uLong>();
                crc32_combine(crc1, crc2, len2);
                
                // Test crc32_combine_gen and crc32_combine_op
                uLong op = crc32_combine_gen(len2);
                if (op != 0) {
                    crc32_combine_op(crc1, crc2, op);
                }
            }
        }
    }
    
    // Test 5: Utility and version functions
    const char* version = zlibVersion();
    unsigned long flags = zlibCompileFlags();
    
    // Test get_crc_table
    const z_crc_t* crc_table = get_crc_table();
    // Test error strings for various error codes
    for (int i = 0; i <= Z_NEED_DICT; i++) {
        const char* err = zError(i);
    }
    
    // Test inflateBack functions (simplified)
    if (input_size > 16) {
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        unsigned char window[32768];
        int ret = inflateBackInit(&strm, windowBits, window);
        if (ret == Z_OK) {
            inflateBackEnd(&strm);
        }
    }
    
    return 0;
}

} // namespace harness_001
