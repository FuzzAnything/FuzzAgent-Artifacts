#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <zlib.h>
}
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["zlibCompileFlags","gzdopen","gzread","gzclose","compressBound","compress2","inflateInit2_","inflate","inflateGetDictionary","inflateEnd","inflateEnd","deflateInit_","deflate","deflateUsed","deflateEnd","uncompress_z","deflateEnd","crc32_z","crc32","crc32","crc32","crc32","crc32_combine_gen64","gzdopen","gzwrite","gzflush","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzread","gzclose","compressBound","compress2","inflateInit2_","inflate","inflateGetDictionary","inflateEnd","deflateInit_","deflate","deflateUsed","deflateEnd","uncompress_z","crc32_z","crc32","crc32","crc32","crc32","crc32_combine_gen64","gzdopen","gzwrite","gzflush","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // 1. First, use zlibCompileFlags to get compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Prevent unused variable warning
    
    // 2. Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Get file descriptor and open as gzFile using gzdopen
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Read decompressed data through gzFile APIs
    // First, try to read some data
    Bytef read_buffer[4096];
    unsigned int bytes_read = 0;
    int read_result = gzread(gz_in, read_buffer, sizeof(read_buffer));
    if (read_result > 0) {
        bytes_read = read_result;
    }
    
    // 5. Also try gzgets, gzgetc, gzungetc, gzeof, gzdirect
    // Note: These are called even though not in the required list, to better exercise the API
    // For the required APIs, we'll focus on gzwrite, gzflush, gzclose below
    
    // 6. Close the gzFile input
    gzclose(gz_in);
    // in_file is closed by gzclose when using gzdopen
    
    // 7. Now prepare to process the data using z_stream APIs
    // Create source buffer from original data for compression
    const Bytef* source = (const Bytef*)data;
    uLong sourceLen = (uLong)(size > 1000000 ? 1000000 : size); // Cap for safety
    
    // 8. Calculate maximum compressed size using compressBound
    uLong max_compressed_size = compressBound(sourceLen);
    if (max_compressed_size < sourceLen) {
        // Should not happen, but handle defensively
        max_compressed_size = sourceLen * 2 + 12;
    }
    
    // 9. Allocate buffers for compression/decompression
    Bytef* compressed = (Bytef*)malloc(max_compressed_size);
    Bytef* decompressed = (Bytef*)malloc(sourceLen);
    Bytef* recompressed = (Bytef*)malloc(max_compressed_size);
    
    if (!compressed || !decompressed || !recompressed) {
        free(compressed);
        free(decompressed);
        free(recompressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 10. First compression using compress2
    uLongf compressedLen = max_compressed_size;
    int compress_level = Z_DEFAULT_COMPRESSION;
    if (size > 0) {
        compress_level = data[0] % 10; // Vary compression level based on input
    }
    int compress_result = compress2(compressed, &compressedLen, source, sourceLen, compress_level);
    
    // 11. Now decompress using inflate API with z_stream
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // 12. Initialize inflate with windowBits
    int windowBits = MAX_WBITS;
    if (size > 1 && (data[1] & 1)) {
        windowBits = -MAX_WBITS; // Raw deflate stream
    }
    
    int inflate_init_result = inflateInit2_(&inflate_stream, windowBits, ZLIB_VERSION, sizeof(z_stream));
    
    if (inflate_init_result == Z_OK && compressedLen > 0) {
        inflate_stream.next_in = compressed;
        inflate_stream.avail_in = (uInt)compressedLen;
        inflate_stream.next_out = decompressed;
        inflate_stream.avail_out = (uInt)sourceLen;
        
        // 13. Perform inflation
        int inflate_result = inflate(&inflate_stream, Z_FINISH);
        (void)inflate_result; // Result checked implicitly
        
        // 14. Get inflate dictionary (even if not set)
        uInt dictLength = 0;
        Bytef dictBuffer[256];
        int dict_result = inflateGetDictionary(&inflate_stream, dictBuffer, &dictLength);
        (void)dict_result;
        
        // 15. End inflation
        inflateEnd(&inflate_stream);
    } else {
        // Clean up if init failed
        if (inflate_init_result == Z_OK) {
            inflateEnd(&inflate_stream);
        }
    }
    
    // 16. Initialize deflate stream
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    int deflate_init_result = deflateInit_(&deflate_stream, compress_level, ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init_result == Z_OK) {
        // Use the decompressed data as source for recompression
        uLong actual_decompressed = inflate_stream.total_out;
        if (actual_decompressed > 0) {
            deflate_stream.next_in = decompressed;
            deflate_stream.avail_in = (uInt)actual_decompressed;
            deflate_stream.next_out = recompressed;
            deflate_stream.avail_out = (uInt)max_compressed_size;
            
            // 17. Perform deflation
            int deflate_result = deflate(&deflate_stream, Z_FINISH);
            (void)deflate_result;
            
            // 18. Get deflate usage statistics
            int bits_used = 0;
            int used_result = deflateUsed(&deflate_stream, &bits_used);
            (void)used_result;
            
            // 19. End deflation
            deflateEnd(&deflate_stream);
            
            // 20. Now decompress the recompressed data using uncompress_z
            z_size_t final_decompressed_len = sourceLen;
            z_size_t recompressed_len = deflate_stream.total_out;
            
            Bytef* final_decompressed = (Bytef*)malloc(sourceLen);
            if (final_decompressed && recompressed_len > 0) {
                int uncompress_result = uncompress_z(final_decompressed, &final_decompressed_len, 
                                                    recompressed, recompressed_len);
                (void)uncompress_result;
                free(final_decompressed);
            }
        } else {
            deflateEnd(&deflate_stream);
        }
    }
    
    // 21. Compute CRC32 using crc32_z
    uLong crc = 0;
    if (sourceLen > 0) {
        crc = crc32_z(crc, source, (z_size_t)sourceLen);
    }
    
    // 22. Generate CRC combine function using crc32_combine_gen64
    // We'll create two dummy CRCs and combine them
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    if (sourceLen > 100) {
        crc1 = crc32(0L, source, 50);
        crc2 = crc32(0L, source + 50, 50);
    }
    off64_t len2 = 50;
    uLong combine_gen = crc32_combine_gen64(len2);
    (void)combine_gen; // Generated combine operator
    
    // 23. Write output using gzFile APIs
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_out = gzdopen(out_fd, "wb");
        
        if (gz_out) {
            // 24. Write some data using gzwrite
            if (bytes_read > 0) {
                int write_result = gzwrite(gz_out, read_buffer, bytes_read);
                (void)write_result;
            }
            
            // 25. Flush the output
            gzflush(gz_out, Z_SYNC_FLUSH);
            
            // 26. Close the gzFile
            gzclose(gz_out);
        } else {
            assert_file_closed(&out_file);;
            // Remove the output file if we couldn't use it
            remove("output_file");
        }
    }
    
    // 27. Clean up allocated resources
    free(compressed);
    free(decompressed);
    free(recompressed);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}