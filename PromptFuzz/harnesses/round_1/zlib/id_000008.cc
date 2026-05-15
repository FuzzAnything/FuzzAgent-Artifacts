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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","inflateInit_","inflateCopy","inflateMark","inflateCodesUsed","inflateReset2","inflateEnd","deflateInit_","deflateParams","deflateUsed","deflateEnd","uncompress2_z","inflateEnd"],"critical_calls":["zlibVersion","inflateInit_","inflateCopy","inflateMark","inflateCodesUsed","inflateReset2","inflateEnd","deflateInit_","deflateParams","deflateUsed","deflateEnd","uncompress2_z","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version;  // Use version to avoid unused variable warning
    
    // 2. Check if we have enough data for operations
    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // 3. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 4. Read compressed data from input
    std::vector<Bytef> compressed_data(size);
    size_t bytes_read = fread(compressed_data.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 5. Initialize inflate stream for decompression
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Use inflateInit_ from required APIs
    int ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Setup input for inflation
    strm.avail_in = static_cast<uInt>(bytes_read);
    strm.next_in = compressed_data.data();
    
    // 7. Create output buffer for decompression
    uLongf destLen = size * 10;  // Safe estimate for decompressed size
    std::vector<Bytef> decompressed(destLen);
    strm.avail_out = static_cast<uInt>(destLen);
    strm.next_out = decompressed.data();
    
    // 8. Create a copy of the inflate stream using inflateCopy
    z_stream strm_copy;
    strm_copy.zalloc = Z_NULL;
    strm_copy.zfree = Z_NULL;
    strm_copy.opaque = Z_NULL;
    
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // 9. Use inflateMark on the copy
        long mark = inflateMark(&strm_copy);
        (void)mark;  // Use mark to avoid unused variable warning
        
        // 10. Get inflation codes used
        unsigned long codes_used = inflateCodesUsed(&strm_copy);
        (void)codes_used;  // Use codes_used to avoid unused variable warning
        
        // 11. Reset the stream with different window bits
        ret = inflateReset2(&strm_copy, MAX_WBITS + 16);  // Add 16 for gzip decoding
        
        // Clean up copy
        inflateEnd(&strm_copy);
    }
    
    // 12. Initialize a deflate stream for compression testing
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // 13. Set deflate parameters
        ret = deflateParams(&deflate_strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
        
        // 14. Get deflate bits used
        int bits_used = 0;
        ret = deflateUsed(&deflate_strm, &bits_used);
        (void)bits_used;  // Use bits_used to avoid unused variable warning
        
        deflateEnd(&deflate_strm);
    }
    
    // 15. Try uncompress2_z with the original data
    z_size_t destLen_z = size * 10;
    std::vector<Bytef> uncompressed_z(destLen_z);
    z_size_t sourceLen_z = static_cast<z_size_t>(size);
    
    ret = uncompress2_z(uncompressed_z.data(), &destLen_z, 
                       compressed_data.data(), &sourceLen_z);
    (void)ret;  // Use ret to avoid unused variable warning
    
    // 16. Clean up original inflate stream
    inflateEnd(&strm);
    
    // 17. Create output file and write some results
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write decompressed data if we have any
        if (strm.total_out > 0) {
            fwrite(decompressed.data(), 1, std::min((size_t)strm.total_out, decompressed.size()), out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}