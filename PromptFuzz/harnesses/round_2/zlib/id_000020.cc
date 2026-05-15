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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["zlibCompileFlags","compressBound_z","uncompress2_z","inflateBackInit_","inflatePrime","inflateBack","inflateCodesUsed","inflateUndermine","inflateBackEnd","deflateInit_","compressBound_z","deflate","deflateReset","deflateEnd","gzdopen","gzputc","gzclose"],"critical_calls":["zlibCompileFlags","compressBound_z","uncompress2_z","inflateBackInit_","inflatePrime","inflateBack","inflateCodesUsed","inflateUndermine","inflateBackEnd","deflateInit_","compressBound_z","deflate","deflateReset","deflateEnd","gzdopen","gzputc","gzclose"],"visited":2}
/**/


// Callback functions for inflateBack
static unsigned  id_000020_in_callback(void* desc, unsigned char** buf) {
    // Simple implementation that just returns available data
    size_t* remaining = (size_t*)desc;
    static unsigned char dummy[1] = {0};
    if (*remaining > 0) {
        *buf = dummy;
        *remaining = 0;
        return 1;
    }
    return 0;
}

static int  id_000020_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    // Discard output
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First check zlib compilation flags
    unsigned long compile_flags = zlibCompileFlags();
    
    // 2. Try to uncompress the input data (which should be zlib compressed data)
    z_size_t destLen = compressBound_z(size);
    Bytef* dest = (Bytef*)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    z_size_t sourceLen = size;
    int uncompress_result = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    // 3. Set up a z_stream for inflateBack
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    // Initialize for inflateBack
    unsigned char window[32768]; // 32K window as recommended in zlib docs
    int back_init_result = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init_result == Z_OK) {
        // Prepare input descriptor for callback
        size_t remaining_data = size > 0 ? size - 1 : 0;
        
        // 4. Use inflatePrime to insert bits if we have enough data
        if (size >= 3) {
            int bits = data[0] & 0x1F; // Use 0-31 bits
            int value = data[1];
            inflatePrime(&strm, bits, value);
        }
        
        // 5. Try inflateBack with our callbacks
        int back_result = inflateBack(&strm,  id_000020_in_callback, &remaining_data, 
                                      id_000020_out_callback, NULL);
        
        // 6. Check inflate codes usage
        unsigned long codes_used = inflateCodesUsed(&strm);
        
        // 7. Try to undermine the stream (if supported)
        if (size >= 2) {
            int subvert = data[0] & 1;
            inflateUndermine(&strm, subvert);
        }
        
        inflateBackEnd(&strm);
    }
    
    // 8. Set up for deflate operations
    z_stream def_strm;
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    int deflate_init_result = deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    
    if (deflate_init_result == Z_OK) {
        // 9. Use compressBound_z to get maximum compressed size
        z_size_t max_compressed = compressBound_z(size);
        
        // 10. Try to compress some data if we have input
        if (size > 0) {
            Bytef* compressed = (Bytef*)malloc(max_compressed);
            if (compressed) {
                def_strm.next_in = (Bytef*)data;
                def_strm.avail_in = size;
                def_strm.next_out = compressed;
                def_strm.avail_out = max_compressed;
                
                deflate(&def_strm, Z_FINISH);
                
                free(compressed);
            }
        }
        
        // 11. Reset deflate stream
        deflateReset(&def_strm);
        deflateEnd(&def_strm);
    }
    
    // 12. Try gzputc operation
    FILE* tmp_file = fopen("output_file", "wb");
    if (tmp_file) {
	int fuzz_fd_0 = fuzz_fileno(tmp_file);

        gzFile gz_file = gzdopen(fuzz_fd_0, "wb");
        if (gz_file) {
            if (size > 0) {
                // Write first byte if available
                gzputc(gz_file, data[0]);
            }
            gzclose(gz_file);
        }
        assert_file_closed(&tmp_file);;
        remove("output_file");
    }
    
    // Cleanup
    free(dest);
    
    assert_file_closed(&tmp_file);
	return 0;
}