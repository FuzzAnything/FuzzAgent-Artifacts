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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["zlibCompileFlags","compressBound_z","compress","inflateBackInit_","inflatePrime","inflateUndermine","inflateBack","inflateCodesUsed","inflateBackEnd","inflateInit2_","inflate","inflateResetKeep","inflateEnd","deflateInit_","compressBound_z","compressBound_z","deflate","deflateReset","deflateEnd","gzdopen","gzclose","gzdopen","gzputc","gzputs","gzclose","uncompress2_z","uncompress2_z"],"critical_calls":["zlibCompileFlags","compressBound_z","compress","inflateBackInit_","inflatePrime","inflateUndermine","inflateBack","inflateCodesUsed","inflateBackEnd","inflateInit2_","inflate","inflateResetKeep","inflateEnd","deflateInit_","compressBound_z","compressBound_z","deflate","deflateReset","deflateEnd","gzdopen","gzclose","gzdopen","gzputc","gzputs","gzclose","uncompress2_z","uncompress2_z"],"visited":0}
/**/


// Custom data source structure for inflateBack
struct BackSource {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Input callback for inflateBack
static unsigned int  id_000044_back_in_func(void* desc, unsigned char** buf) {
    BackSource* src = (BackSource*)desc;
    if (src->pos >= src->size) {
        return 0; // No more data
    }
    *buf = (unsigned char*)(src->data + src->pos);
    unsigned int avail = src->size - src->pos;
    src->pos = src->size;
    return avail;
}

// Output callback for inflateBack
static int  id_000044_back_out_func(void* desc, unsigned char* buf, unsigned int size) {
    // We'll just discard the output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Get compile flags
    unsigned long flags = zlibCompileFlags();
    (void)flags; // Use to avoid unused variable warning
    
    // 2. Create compressed data from input
    Bytef* compr = nullptr;
    uLongf comprLen = 0;
    z_size_t comprLen_z = 0;
    
    // Calculate bound for compression
    comprLen_z = compressBound_z(size);
    if (comprLen_z == 0) {
	return 0;
	}
	
    
    compr = (Bytef*)malloc(comprLen_z);
    if (!compr) {
	return 0;
	}
	
    
    comprLen = comprLen_z;
    int compress_ret = compress(compr, &comprLen, data, size);
    if (compress_ret != Z_OK) {
        free(compr);
        return 0;
    }
    
    // 3. Setup for inflateBack
    z_stream strm_back;
    memset(&strm_back, 0, sizeof(strm_back));
    
    // Create window for inflateBack
    unsigned char* window = (unsigned char*)malloc(32768); // 32K window
    if (!window) {
        free(compr);
        return 0;
    }
    
    // Initialize inflateBack
    int back_init_ret = inflateBackInit_(&strm_back, 15, window, 
                                         ZLIB_VERSION, sizeof(z_stream));
    if (back_init_ret != Z_OK) {
        free(window);
        free(compr);
        return 0;
    }
    
    // 4. Setup source for inflateBack
    BackSource back_src;
    back_src.data = compr;
    back_src.size = comprLen;
    back_src.pos = 0;
    
    // 5. Try inflatePrime (prime with some bits from input)
    if (size > 10) {
        int prime_bits = data[0] % 16; // 0-15 bits
        int prime_value = 0;
        if (prime_bits > 0) {
            prime_value = data[1] & ((1 << prime_bits) - 1);
        }
        inflatePrime(&strm_back, prime_bits, prime_value);
    }
    
    // 6. Try inflateUndermine (if supported)
    if (size > 20) {
        inflateUndermine(&strm_back, data[2] & 1);
    }
    
    // 7. Perform inflateBack
    int back_ret = inflateBack(&strm_back,  id_000044_back_in_func, &back_src, 
                               id_000044_back_out_func, nullptr);
    
    // 8. Get codes used
    unsigned long codes_used = inflateCodesUsed(&strm_back);
    (void)codes_used;
    
    // 9. Clean up inflateBack
    inflateBackEnd(&strm_back);
    free(window);
    
    // 10. Now setup normal inflate/deflate streams
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // 11. Use inflateResetKeep (requires initialized stream)
    // First initialize with inflateInit2_
    int inflate_init_ret = inflateInit2_(&strm_inflate, 15, 
                                         ZLIB_VERSION, sizeof(z_stream));
    if (inflate_init_ret == Z_OK) {
        // Set up input for inflation
        strm_inflate.next_in = compr;
        strm_inflate.avail_in = comprLen;
        
        // Allocate output buffer
        Bytef* uncompr = (Bytef*)malloc(size * 2);
        if (uncompr) {
            strm_inflate.next_out = uncompr;
            strm_inflate.avail_out = size * 2;
            
            // Inflate a bit
            int inflate_ret = inflate(&strm_inflate, Z_SYNC_FLUSH);
            (void)inflate_ret;
            
            // Try inflateResetKeep
            inflateResetKeep(&strm_inflate);
            
            free(uncompr);
        }
        inflateEnd(&strm_inflate);
    }
    
    // 12. Setup deflate stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    int deflate_init_ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION,
                                        ZLIB_VERSION, sizeof(z_stream));
    if (deflate_init_ret == Z_OK) {
        // Use some input data for deflation
        Bytef* deflate_out = (Bytef*)malloc(compressBound_z(size));
        if (deflate_out) {
            strm_deflate.next_in = (Bytef*)data;
            strm_deflate.avail_in = size > 1024 ? 1024 : size;
            strm_deflate.next_out = deflate_out;
            strm_deflate.avail_out = compressBound_z(size);
            
            // Deflate with flush
            deflate(&strm_deflate, Z_FINISH);
            
            // Try deflateReset
            deflateReset(&strm_deflate);
            
            free(deflate_out);
        }
        deflateEnd(&strm_deflate);
    }
    
    // 13. Test gzFile operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, "rb");
        if (gz) {
            // Try gzputs if we can open for writing
            gzclose(gz);
        }
        
        // Re-open for writing
        assert_file_closed(&in_file);;
        in_file = fopen("output_file", "wb");
        if (in_file) {
            fd = fuzz_fileno(in_file);
            gz = gzdopen(fd, "wb");
            if (gz) {
                // Write some data using gzputc and gzputs
                if (size > 10) {
                    for (int i = 0; i < 5 && i < (int)size; i++) {
                        gzputc(gz, data[i]);
                    }
                    
                    const char* test_str = "test";
                    gzputs(gz, test_str);
                }
                gzclose(gz);
            }
            assert_file_closed(&in_file);;
            unlink("output_file");
        }
    }
    
    // 14. Test uncompress2_z with the compressed data
    z_size_t destLen = 0;
    z_size_t sourceLen = comprLen;
    Bytef* uncompr2 = nullptr;
    
    // First call to get required buffer size
    int uncompress2_ret = uncompress2_z(nullptr, &destLen, compr, &sourceLen);
    if (uncompress2_ret == Z_BUF_ERROR && destLen > 0) {
        uncompr2 = (Bytef*)malloc(destLen);
        if (uncompr2) {
            uncompress2_ret = uncompress2_z(uncompr2, &destLen, compr, &sourceLen);
            free(uncompr2);
        }
    }
    
    // 15. Cleanup
    free(compr);
    
    assert_file_closed(&in_file);
	return 0;
}