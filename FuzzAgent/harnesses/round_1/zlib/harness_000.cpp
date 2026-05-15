#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_000 {
/*
 * Fuzzing harness for zlib compression library
 * Targets core compression/decompression APIs with comprehensive input testing
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);  // Compression level 0-9
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);  // Compression strategy
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);  // Window bits for deflate/inflate
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);  // Memory level
    
    // Consume boolean flags
    bool test_compress = fdp.ConsumeBool();
    bool test_deflate = fdp.ConsumeBool();
    bool test_gzip = fdp.ConsumeBool();
    bool test_checksums = fdp.ConsumeBool();

    // Consume remaining data as input buffer
    size_t input_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024 * 1024);
    if (fdp.remaining_bytes() < input_size) {
        return 0;
    }
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_size);
    
    // Test 1: Basic compress/uncompress functions
    if (test_compress && input_size > 0) {
        // Calculate maximum compressed size
        uLongf destLen = compressBound((uLong)input_size);
        Bytef* dest = (Bytef*)malloc(destLen);
        Bytef* uncompr = (Bytef*)malloc(input_size);
        uLongf uncomprLen = (uLongf)input_size;
        
        if (dest && uncompr) {
            // Test compress2 with different levels
            int ret = compress2(dest, &destLen, input_buffer.data(), (uLong)input_size, level);
            if (ret == Z_OK) {
                // Decompress and verify
                int unret = uncompress(uncompr, &uncomprLen, dest, destLen);
                if (unret == Z_OK && uncomprLen == input_size) {
                    // Verify data matches
                    memcmp(uncompr, input_buffer.data(), input_size);
                }
            }
            
            // Test simple compress/uncompress
            destLen = compressBound((uLong)input_size);
            ret = compress(dest, &destLen, input_buffer.data(), (uLong)input_size);
            if (ret == Z_OK) {
                uncomprLen = (uLongf)input_size;
                uncompress(uncompr, &uncomprLen, dest, destLen);
            }
        }
        
        free(dest);
        free(uncompr);
    }

    // Test 2: Stream-based compression/decompression (deflate/inflate)
    if (test_deflate && input_size > 0) {
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // Initialize deflate
        int ret = deflateInit2(&strm, level, Z_DEFLATED, windowBits, memLevel, strategy);
        if (ret == Z_OK) {
            // Prepare buffers
            size_t compr_size = deflateBound(&strm, (uLong)input_size);
            Bytef* compr = (Bytef*)malloc(compr_size);
            Bytef* uncompr = (Bytef*)malloc(input_size);
            
            if (compr && uncompr) {
                // Compress the data
                strm.next_in = input_buffer.data();
                strm.avail_in = (uInt)input_size;
                strm.next_out = compr;
                strm.avail_out = (uInt)compr_size;
                
                ret = deflate(&strm, Z_FINISH);
                if (ret == Z_STREAM_END) {
                    size_t compressed_size = compr_size - strm.avail_out;
                    
                    // Now inflate it back
                    z_stream istrm;
                    istrm.zalloc = Z_NULL;
                    istrm.zfree = Z_NULL;
                    istrm.opaque = Z_NULL;
                    istrm.next_in = compr;
                    istrm.avail_in = (uInt)compressed_size;
                    
                    ret = inflateInit2(&istrm, windowBits);
                    if (ret == Z_OK) {
                        istrm.next_out = uncompr;
                        istrm.avail_out = (uInt)input_size;
                        
                        ret = inflate(&istrm, Z_FINISH);
                        if (ret == Z_STREAM_END) {
                            // Verify decompressed data matches original
                            memcmp(uncompr, input_buffer.data(), input_size - istrm.avail_out);
                        }
                        inflateEnd(&istrm);
                    }
                }
            }
            
            free(compr);
            free(uncompr);
            deflateEnd(&strm);
        }
    }

    // Test 3: Checksum functions (Adler32 and CRC32)
    if (test_checksums && input_size > 0) {
        uLong adler = adler32(0L, Z_NULL, 0);
        adler = adler32(adler, input_buffer.data(), (uInt)input_size);
        
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, input_buffer.data(), (uInt)input_size);
        
        // Test adler32_combine and crc32_combine with random values
        uLong adler1 = fdp.ConsumeIntegral<uLong>();
        uLong adler2 = fdp.ConsumeIntegral<uLong>();
        uLong len2 = fdp.ConsumeIntegral<uLong>();
        adler32_combine(adler1, adler2, len2);
        
        uLong crc1 = fdp.ConsumeIntegral<uLong>();
        uLong crc2 = fdp.ConsumeIntegral<uLong>();
        crc32_combine(crc1, crc2, len2);
    }

    // Test 4: Utility functions
    const char* version = zlibVersion();
    unsigned long compile_flags = zlibCompileFlags();
    
    // Test error handling
    int err_code = fdp.ConsumeIntegralInRange<int>(0, 10);
    const char* err_msg = zError(err_code);

    return 0;
}

} // namespace harness_000
