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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["zlibVersion","inflateInit_","inflateEnd","inflate","inflateMark","inflateCodesUsed","inflateCopy","inflateEnd","inflateReset2","uncompress2_z","deflateInit_","deflate","deflateParams","deflateUsed","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","inflateInit_","inflate","inflateMark","inflateCodesUsed","inflateCopy","inflateEnd","inflateReset2","uncompress2_z","deflateInit_","deflate","deflateParams","deflateUsed","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	  // Need enough data for meaningful testing

    // 1. Get zlib version first
    const char* version = zlibVersion();
    if (!version) {
	return 0;
	}
	

    // 2. Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use inflateInit_ with explicit version check
    int ret = inflateInit_(&strm, version, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return 0;
    }

    // 3. Set input data for inflation
    strm.avail_in = (uInt)(size > 100 ? 100 : size);  // Use first 100 bytes for stream
    strm.next_in = (Bytef*)data;

    // 4. Create output buffer for inflation
    Bytef out_buffer[4096];
    strm.avail_out = sizeof(out_buffer);
    strm.next_out = out_buffer;

    // 5. Perform initial inflation (partial)
    ret = inflate(&strm, Z_SYNC_FLUSH);
    
    // 6. Get mark position
    long mark_pos = inflateMark(&strm);
    
    // 7. Get code usage statistics
    unsigned long codes_used = inflateCodesUsed(&strm);
    
    // 8. Create a copy of the inflate stream
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(strm_copy));
    
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // Use the copy briefly
        inflateEnd(&strm_copy);
    }

    // 9. Reset inflate stream with different window bits
    ret = inflateReset2(&strm, -15);  // Raw deflate format
    
    // 10. Now test uncompress2_z with remaining data
    if (size > 100) {
        size_t remaining = size - 100;
        const Bytef* remaining_data = data + 100;
        
        // Try to uncompress - note this expects valid compressed data
        Bytef dest_buffer[4096];
        z_size_t destLen = sizeof(dest_buffer);
        z_size_t sourceLen = remaining;
        
        // uncompress2_z modifies sourceLen, so use a copy
        Bytef* source_copy = (Bytef*)malloc(remaining);
        if (source_copy) {
            memcpy(source_copy, remaining_data, remaining);
            z_size_t sourceLen_copy = remaining;
            
            int uncompress_ret = uncompress2_z(dest_buffer, &destLen, 
                                              source_copy, &sourceLen_copy);
            // Check return value but continue regardless
            free(source_copy);
        }
    }

    // 11. Now test deflate-related functions
    // Create a separate deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Set some input data for deflation
        Bytef deflate_in[100];
        Bytef deflate_out[200];
        
        // Use first 100 bytes of input as data to compress
        size_t copy_size = size > 100 ? 100 : size;
        memcpy(deflate_in, data, copy_size);
        
        deflate_strm.avail_in = (uInt)copy_size;
        deflate_strm.next_in = deflate_in;
        deflate_strm.avail_out = sizeof(deflate_out);
        deflate_strm.next_out = deflate_out;
        
        // Perform some deflation
        ret = deflate(&deflate_strm, Z_FINISH);
        
        // 12. Change deflate parameters
        ret = deflateParams(&deflate_strm, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
        
        // 13. Check bits used by deflate
        int bits_used = 0;
        ret = deflateUsed(&deflate_strm, &bits_used);
        
        deflateEnd(&deflate_strm);
    }

    // 14. Clean up inflate stream
    inflateEnd(&strm);

    return 0;
}