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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","uncompress2_z","inflateInit_","inflateCopy","inflateMark","inflateCodesUsed","inflateReset2","deflateInit_","deflateParams","deflateUsed","deflateEnd","inflateEnd","inflateEnd"],"critical_calls":["zlibVersion","uncompress2_z","inflateInit_","inflateCopy","inflateMark","inflateCodesUsed","inflateReset2","deflateInit_","deflateParams","deflateUsed","deflateEnd","inflateEnd","inflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs with the given constraints:

*/



extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        // Too small for meaningful fuzzing
        return 0;
    }

    // Get zlib version first
    const char *version = zlibVersion();
    (void)version; // Use to avoid unused variable warning

    // Use uncompress2_z to decompress input data
    // Note: We're assuming the input is zlib-compressed data
    z_size_t destLen = size * 2; // Start with 2x the input size
    z_size_t sourceLen = size;
    
    Bytef *dest = static_cast<Bytef*>(malloc(destLen));
    if (!dest) {
        return 0;
    }

    int ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    (void)ret; // We'll proceed even if decompression fails

    // Initialize inflation stream
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize inflation with current zlib version
    ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(dest);
        return 0;
    }

    // Create a copy of the inflation stream
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(z_stream));
    ret = inflateCopy(&strm_copy, &strm);
    
    // Mark current position in stream
    long mark = inflateMark(&strm);
    (void)mark; // Use to avoid unused variable warning

    // Get codes used by inflation
    unsigned long codes_used = inflateCodesUsed(&strm);
    (void)codes_used; // Use to avoid unused variable warning

    // Reset inflation with different window bits
    int window_bits = (data[0] % 15) + 8; // Generate valid window bits (8-15)
    ret = inflateReset2(&strm, window_bits);

    // Initialize deflation stream for testing deflateParams
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    
    ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Set deflation parameters
        int level = (data[1] % 10); // 0-9 compression level
        int strategy = (data[2] % 5); // 0-4 strategy
        ret = deflateParams(&def_strm, level, strategy);
        
        // Get bits used by deflation
        int bits_used;
        ret = deflateUsed(&def_strm, &bits_used);
        (void)bits_used; // Use to avoid unused variable warning
        
        // Clean up deflation stream
        deflateEnd(&def_strm);
    }

    // Test with file operations if we have uncompressed data
    if (destLen > 0 && sourceLen < size) {
        FILE *in_file = fmemopen(dest, destLen, "rb");
        if (in_file) {
            // Read some data from the memory file
            uint8_t buffer[1024];
            size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
            (void)bytes_read; // Use to avoid unused variable warning
            
            // Get file descriptor
            int fd = fuzz_fileno(in_file);
            (void)fd; // Use to avoid unused variable warning
            
            assert_file_closed(&in_file);;
        }
        
        // Try writing output to a file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(dest, 1, destLen, out_file);
            
            // Get file descriptor
            int fd = fuzz_fileno(out_file);
            (void)fd; // Use to avoid unused variable warning
            
            assert_file_closed(&out_file);;
            
            // Remove the output file
            remove("output_file");
        }
    }

    // Clean up
    inflateEnd(&strm);
    inflateEnd(&strm_copy);
    free(dest);

    return 0;
}