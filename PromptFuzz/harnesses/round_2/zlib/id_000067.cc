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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"inflateSyncPoint":[[1323,9,1323,32,0,0,4,0]]},"library_calls":["compressBound_z","compress2_z","gzdopen","gzeof","gzungetc","gzrewind","inflateBackInit_","gzclose","inflatePrime","inflateUndermine","inflateSyncPoint","inflateBack","inflateBackEnd","deflateInit_","gzclose","deflateReset","deflateEnd","gzclose","uncompress2_z","crc32","crc32","crc32","crc32_combine_gen","crc32_combine_op","inflateInit2_","inflateEnd","gzclose"],"critical_calls":["compressBound_z","compress2_z","gzdopen","gzeof","gzungetc","gzrewind","inflateBackInit_","inflatePrime","inflateUndermine","inflateSyncPoint","inflateBack","inflateBackEnd","deflateInit_","deflateReset","deflateEnd","uncompress2_z","crc32","crc32","crc32","crc32_combine_gen","crc32_combine_op","inflateInit2_","inflateEnd","gzclose"],"visited":0}
/**/


// Input callback for inflateBack
static unsigned int  id_000067_inflate_back_input(void* desc, unsigned char** buf) {
    // Simple implementation that returns the entire input at once
    *buf = (unsigned char*)desc;
    return (unsigned int)((size_t)desc >> 32); // Use high 32 bits as size
}

// Output callback for inflateBack  
static int  id_000067_inflate_back_output(void* desc, unsigned char* buf, unsigned int size) {
    // Just discard the output
    (void)desc;
    (void)buf;
    (void)size;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful testing

    // Prepare compressed data (simulate zlib output)
    Bytef* compressed = nullptr;
    z_size_t compressed_len = 0;
    z_size_t uncompressed_len = 0;
    Bytef* uncompressed = nullptr;
    
    // 1. First compress some data to create valid zlib output
    const char* test_string = "Hello, World! This is a test string for zlib fuzzing.";
    uLong source_len = strlen(test_string) + 1;
    
    // Use compressBound_z to get maximum compressed size
    z_size_t max_compressed = compressBound_z((z_size_t)source_len);
    if (max_compressed == 0) {
	return 0;
	}
	
    
    compressed = (Bytef*)malloc(max_compressed);
    if (!compressed) {
	return 0;
	}
	
    
    compressed_len = max_compressed;
    if (compress2_z(compressed, &compressed_len, 
                   (const Bytef*)test_string, (z_size_t)source_len, 
                   Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // 2. Now use the provided APIs with our compressed data
    FILE* in_file = fmemopen((void*)compressed, compressed_len, "rb");
    if (!in_file) {
        free(compressed);
        assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(in_file);

    
    // 3. Use gzdopen to create gzFile from file descriptor
    gzFile gz_file = gzdopen(fuzz_fd_0, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 4. Test gzeof
    (void)gzeof(gz_file);
    
    // 5. Test gzungetc
    int c = gzgetc(gz_file);
    if (c != -1) {
        gzungetc(c, gz_file);
    }
    
    // 6. Test gzrewind
    (void)gzrewind(gz_file);
    
    // 7. Prepare for inflateBack
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // 8. Initialize inflate for back processing
    unsigned char window[32768]; // 32K window
    if (inflateBackInit_(&strm, MAX_WBITS, window, 
                        ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 9. Test inflatePrime
    inflatePrime(&strm, 0, 0);
    
    // 10. Test inflateUndermine
    inflateUndermine(&strm, 0);
    
    // 11. Test inflateSyncPoint
    (void)inflateSyncPoint(&strm);
    
    // 12. Prepare input descriptor for inflateBack
    // We'll use the fuzzer input as test data
    struct {
        const uint8_t* data;
        size_t size;
        size_t pos;
    } input_desc = {data, size, 0};
    
    // 13. Call inflateBack (may fail on random input, but that's OK)
    int ret = inflateBack(&strm,  id_000067_inflate_back_input, &input_desc, 
                          id_000067_inflate_back_output, nullptr);
    (void)ret; // Ignore result for fuzzing
    
    // 14. Clean up inflateBack
    inflateBackEnd(&strm);
    
    // 15. Initialize deflate stream
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    if (deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, 
                    ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 16. Test deflateReset
    deflateReset(&def_strm);
    
    // 17. Test deflateEnd
    deflateEnd(&def_strm);
    
    // 18. Now test with fuzzer input directly using uncompress2_z
    // First allocate output buffer
    uncompressed_len = size * 4; // Reasonable guess
    uncompressed = (Bytef*)malloc(uncompressed_len);
    if (!uncompressed) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    z_size_t source_len_var = (z_size_t)size;
    int uncompress_ret = uncompress2_z(uncompressed, &uncompressed_len, 
                                      data, &source_len_var);
    (void)uncompress_ret; // Result doesn't matter for fuzzing
    
    // 19. Test CRC32 functions
    uLong crc1 = crc32(0L, Z_NULL, 0);
    if (size > 10) {
        // Compute CRC of first half
        size_t half = size / 2;
        crc1 = crc32(crc1, data, (uInt)half);
        
        // Compute CRC of second half
        uLong crc2 = crc32(0L, data + half, (uInt)(size - half));
        
        // Generate combine operation
        uLong op = crc32_combine_gen((off_t)(size - half));
        
        // Combine the two CRCs
        (void)crc32_combine_op(crc1, crc2, op);
    }
    
    // 20. Initialize inflate with different windowBits
    z_stream inf_strm2;
    memset(&inf_strm2, 0, sizeof(inf_strm2));
    
    if (inflateInit2_(&inf_strm2, MAX_WBITS + 16, // gzip mode
                     ZLIB_VERSION, (int)sizeof(z_stream)) == Z_OK) {
        // Clean up properly
        inflateEnd(&inf_strm2);
    }
    
    // 21. Clean up all resources
    free(uncompressed);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    free(compressed);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}