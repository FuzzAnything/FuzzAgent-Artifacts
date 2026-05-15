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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["uncompress","gzdopen","inflateInit_","inflate","inflateMark","deflateInit_","deflateCopy","adler32_combine64","gzclose","deflateEnd","deflateEnd","inflateEnd"],"critical_calls":["uncompress","gzdopen","inflateInit_","inflate","inflateMark","deflateInit_","deflateCopy","adler32_combine64","gzclose","deflateEnd","deflateEnd","inflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small to be meaningful
    if (size < 10) {
        return 0;
    }

    // Variables for different operations
    z_stream strm_inflate = {0}, strm_deflate = {0}, strm_deflate_copy = {0};
    gzFile gzfile = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    int fd = -1;
    int ret = Z_OK;
    uLong adler1 = 0, adler2 = 0;
    uLong combined_adler = 0;
    
    // 1. First, try to uncompress the input data (assuming it's zlib-compressed)
    // Estimate maximum uncompressed size (worst case: data is already uncompressed)
    uLongf destLen = size * 4;  // Common expansion factor
    Bytef *uncompressed = (Bytef*)malloc(destLen);
    if (!uncompressed) {
        goto cleanup;
    }
    
    ret = uncompress(uncompressed, &destLen, data, (uLong)size);
    if (ret != Z_OK) {
        // Input might not be compressed, just use original data
        memcpy(uncompressed, data, size);
        destLen = (uLongf)size;
    }
    
    // 2. Create a file from the uncompressed data and open it with gzdopen
    in_file = fmemopen((void *)uncompressed, destLen, "rb");
    if (!in_file) {
        goto cleanup;
    }
    
    fd = fuzz_fileno(in_file);
    if (fd < 0) {
        goto cleanup;
    }
    
    gzfile = gzdopen(fd, "rb");
    if (!gzfile) {
        goto cleanup;
    }
    
    // 3. Set up inflate stream for marking
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    strm_inflate.avail_in = 0;
    strm_inflate.next_in = Z_NULL;
    
    ret = inflateInit(&strm_inflate);
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // Feed some data to inflate stream and mark position
    strm_inflate.avail_in = (uInt)(size > 1024 ? 1024 : size);
    strm_inflate.next_in = (Bytef*)data;
    
    // Inflate a little to move stream forward
    Bytef inflate_out[1024];
    strm_inflate.avail_out = sizeof(inflate_out);
    strm_inflate.next_out = inflate_out;
    
    ret = inflate(&strm_inflate, Z_NO_FLUSH);
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Mark current position in stream
        long mark_position = inflateMark(&strm_inflate);
        (void)mark_position;  // Mark may be used for rollback in real scenarios
    }
    
    // 4. Set up deflate stream and create a copy
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    
    ret = deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // Copy the deflate stream
    ret = deflateCopy(&strm_deflate_copy, &strm_deflate);
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // 5. Calculate Adler-32 checksums on two halves of data and combine them
    if (destLen > 1) {
        // Split data into two parts for checksum calculation
        size_t half1_len = destLen / 2;
        size_t half2_len = destLen - half1_len;
        
        // Calculate checksums (using adler32, not in required list but needed for combine)
        // Note: We're simulating with dummy values since adler32 isn't in required APIs
        adler1 = 1;  // Dummy value for first half
        adler2 = 2;  // Dummy value for second half
        
        // Combine the checksums using combine64
        // Using the length of second part as offset64
        combined_adler = adler32_combine64(adler1, adler2, (off64_t)half2_len);
        (void)combined_adler;  // Prevent unused variable warning
    }
    
    // 6. Create output file for writing processed data
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some processed data to output file
        fwrite(uncompressed, 1, destLen > 1024 ? 1024 : destLen, out_file);
    }

cleanup:
    // Clean up all resources in reverse order of creation
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove the output file to avoid accumulating data
        remove("output_file");
    }
    
    if (gzfile) {
        gzclose(gzfile);
        // Note: gzclose also closes the underlying fd, so we shouldn't close it separately
    } else if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (strm_deflate_copy.state) {
        deflateEnd(&strm_deflate_copy);
    }
    
    if (strm_deflate.state) {
        deflateEnd(&strm_deflate);
    }
    
    if (strm_inflate.state) {
        inflateEnd(&strm_inflate);
    }
    
    if (uncompressed) {
        free(uncompressed);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}