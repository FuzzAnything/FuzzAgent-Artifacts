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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["inflateInit2_","zError","inflateEnd","inflatePrime","zError","inflate","zError","inflateUndermine","zError","inflateCopy","zError","inflate","zError","inflateEnd","deflateInit2_","deflateSetDictionary","deflateGetDictionary","zError","deflateEnd","inflate","zError","inflateEnd"],"critical_calls":["inflateInit2_","inflatePrime","zError","inflate","zError","inflateUndermine","zError","inflateCopy","inflate","zError","inflateEnd","deflateInit2_","deflateSetDictionary","deflateGetDictionary","zError","deflateEnd","inflate","zError","inflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression with error handling and stream manipulation:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 8) {
	return 0;
	}
	
    
    // Variables for streams
    z_stream strm1, strm2, strm3;
    int ret;
    const char *err_msg;
    
    // Initialize decompression stream
    memset(&strm1, 0, sizeof(z_stream));
    strm1.zalloc = Z_NULL;
    strm1.zfree = Z_NULL;
    strm1.opaque = Z_NULL;
    strm1.next_in = Z_NULL;
    strm1.avail_in = 0;
    
    ret = inflateInit2(&strm1, 15 + 32); // 15 window bits + 32 for gzip detection
    if (ret != Z_OK) {
        err_msg = zError(ret);
        return 0;
    }
    
    // Set input data
    strm1.next_in = (Bytef *)data;
    strm1.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    // Allocate output buffer
    const size_t out_buf_size = 4096;
    Bytef *out_buf = (Bytef *)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&strm1);
        return 0;
    }
    
    strm1.next_out = out_buf;
    strm1.avail_out = out_buf_size;
    
    // Use inflatePrime to insert bits into the bit buffer
    // We'll use the first byte of data as bits to prime
    if (size > 0) {
        int bits = 8;
        int value = data[0];
        ret = inflatePrime(&strm1, fuzz_int32_t_1, value);
        if (ret != Z_OK) {
            err_msg = zError(ret);
        }
    }
    
    // Perform partial inflation
    ret = inflate(&strm1, fuzz_int32_t_2);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        err_msg = zError(ret);
    }
    
    // Use inflateUndermine to test error conditions
    // This can be used to undermine the integrity of the stream for testing
    ret = inflateUndermine(&strm1, fuzz_int32_t_3); // Pass 1 to undermine
    if (ret != Z_OK) {
        err_msg = zError(ret);
    }
    
    // Create a copy of the inflation stream
    memset(&strm2, 0, sizeof(z_stream));
    ret = inflateCopy(&strm2, &strm1);
    if (ret != Z_OK) {
        err_msg = zError(ret);
    } else {
        // Try to continue inflation with the copy
        strm2.next_out = out_buf + (strm1.next_out - out_buf);
        strm2.avail_out = out_buf_size - (strm1.next_out - out_buf);
        
        ret = inflate(&strm2, fuzz_int32_t_4);
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            err_msg = zError(ret);
        }
        
        // Clean up the copied stream
        inflateEnd(&strm2);
    }
    
    // For deflateGetDictionary, we need a deflate stream
    // Create a temporary deflate stream with a dictionary
    memset(&strm3, 0, sizeof(z_stream));
    strm3.zalloc = Z_NULL;
    strm3.zfree = Z_NULL;
    strm3.opaque = Z_NULL;
    
    ret = deflateInit2(&strm3, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15, 8, Z_DEFAULT_STRATEGY);
    if (ret == Z_OK) {
        // Set a dictionary from input data (use first 256 bytes if available)
        uInt dict_len = (uInt)(size > 256 ? 256 : size);
        if (dict_len > 0) {
            ret = deflateSetDictionary(&strm3, data, dict_len);
            if (ret == Z_OK) {
                // Now get the dictionary back
                Bytef *dict_buf = (Bytef *)malloc(32768); // Max dictionary size
                uInt dict_buf_len = 32768;
                
                if (dict_buf) {
                    ret = deflateGetDictionary(&strm3, dict_buf, &dict_buf_len);
                    if (ret != Z_OK) {
                        err_msg = zError(ret);
                    }
                    free(dict_buf);
                }
            }
        }
        deflateEnd(&strm3);
    }
    
    // Continue with original inflation stream
    // Reset output buffer position if needed
    if (strm1.avail_out == 0) {
        strm1.next_out = out_buf;
        strm1.avail_out = out_buf_size;
    }
    
    // Try to finish inflation
    ret = inflate(&strm1, fuzz_int32_t_5);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        err_msg = zError(ret);
    }
    
    // Clean up
    free(out_buf);
    inflateEnd(&strm1);
    
    return 0;
}