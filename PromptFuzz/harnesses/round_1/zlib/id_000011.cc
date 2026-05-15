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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["zlibVersion","uncompress2_z","inflateInit_","inflateEnd","inflateMark","inflate","inflateCodesUsed","inflateCopy","inflateReset2","deflateInit_","deflateBound","deflateBound","deflateParams","deflateUsed","deflate","deflateEnd","inflateEnd","inflateEnd","deflateInit_","deflateBound","deflateBound","deflateParams","deflateUsed","deflate","deflateEnd"],"critical_calls":["zlibVersion","uncompress2_z","inflateInit_","inflateMark","inflate","inflateCodesUsed","inflateCopy","inflateReset2","deflateInit_","deflateBound","deflateBound","deflateParams","deflateUsed","deflate","deflateEnd","inflateEnd","inflateEnd"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Check minimum input size requirement
    if (size < 10) {
        return 0;
    }

    // Step 2: Use zlibVersion API
    const char *version = zlibVersion();
    if (!version) {
        return 0;
    }

    // Step 3: Prepare output buffer for decompression
    // Estimate decompressed size - zlib recommends at least 1.001 * size + 12
    z_size_t destLen = size * 2 + 100;  // Conservative estimate
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        return 0;
    }

    // Step 4: Use uncompress2_z API
    z_size_t sourceLen = size;
    int uncompress_result = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    // If uncompress fails, try to use stream APIs
    if (uncompress_result != Z_OK) {
        free(dest);
        
        // Step 5: Create and initialize inflation stream
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        
        // Use inflateInit_ API
        int init_result = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
        if (init_result != Z_OK) {
            return 0;
        }
        
        // Set input data
        strm.next_in = (Bytef *)data;
        strm.avail_in = size;
        
        // Prepare output buffer
        Bytef *out_buffer = (Bytef *)malloc(size * 2);
        if (!out_buffer) {
            inflateEnd(&strm);
            return 0;
        }
        strm.next_out = out_buffer;
        strm.avail_out = size * 2;
        
        // Step 6: Use inflateMark API
        long mark = inflateMark(&strm);
        
        // Step 7: Perform inflation
        int inflate_result = inflate(&strm, Z_NO_FLUSH);
        
        // Step 8: Use inflateCodesUsed API
        unsigned long codes_used = inflateCodesUsed(&strm);
        
        // Step 9: Create a copy of the stream
        z_stream strm_copy;
        memset(&strm_copy, 0, sizeof(strm_copy));
        
        // Use inflateCopy API
        int copy_result = inflateCopy(&strm_copy, &strm);
        
        // Step 10: Use inflateReset2 API with different window bits
        int reset_result = inflateReset2(&strm, 15 + 16);  // Add 16 for gzip
        
        // Step 11: Create a deflation stream for comparison
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(def_strm));
        
        // Initialize deflation
        int def_init_result = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, 
                                         ZLIB_VERSION, sizeof(z_stream));
        if (def_init_result == Z_OK) {
            // Set up deflation input/output
            Bytef *deflate_out = (Bytef *)malloc(deflateBound(&def_strm, size));
            if (deflate_out) {
                def_strm.next_in = (Bytef *)data;
                def_strm.avail_in = size;
                def_strm.next_out = deflate_out;
                def_strm.avail_out = deflateBound(&def_strm, size);
                
                // Step 12: Use deflateParams API
                int param_result = deflateParams(&def_strm, Z_BEST_COMPRESSION, 
                                               Z_DEFAULT_STRATEGY);
                
                // Step 13: Use deflateUsed API
                int bits_used = 0;
                int used_result = deflateUsed(&def_strm, &bits_used);
                
                // Perform deflation
                int deflate_result = deflate(&def_strm, Z_FINISH);
                
                free(deflate_out);
            }
            
            // Clean up deflation stream
            deflateEnd(&def_strm);
        }
        
        // Clean up
        free(out_buffer);
        if (copy_result == Z_OK) {
            inflateEnd(&strm_copy);
        }
        inflateEnd(&strm);
        
    } else {
        // If uncompress succeeded, we still need to call the other APIs
        
        // Create and initialize a deflation stream
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(def_strm));
        
        int def_init_result = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION,
                                         ZLIB_VERSION, sizeof(z_stream));
        if (def_init_result == Z_OK) {
            // Set up input/output for deflation
            Bytef *deflate_out = (Bytef *)malloc(deflateBound(&def_strm, destLen));
            if (deflate_out) {
                def_strm.next_in = dest;  // Use decompressed data as input
                def_strm.avail_in = destLen;
                def_strm.next_out = deflate_out;
                def_strm.avail_out = deflateBound(&def_strm, destLen);
                
                // Use deflateParams API
                int param_result = deflateParams(&def_strm, Z_BEST_SPEED, 
                                               Z_HUFFMAN_ONLY);
                
                // Use deflateUsed API
                int bits_used = 0;
                int used_result = deflateUsed(&def_strm, &bits_used);
                
                // Perform deflation
                int deflate_result = deflate(&def_strm, Z_FINISH);
                
                free(deflate_out);
            }
            
            deflateEnd(&def_strm);
        }
        
        free(dest);
    }
    
    return 0;
}