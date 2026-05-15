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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"deflateCopy":[[1327,9,1327,34,0,0,4,0]]},"library_calls":["uncompress","crc32_z","crc32_z","gzdopen","gzgetc_","gzclose","inflateInit2_","inflate","inflateSetDictionary","inflateReset2","deflateInit_","deflateCopy","deflateEnd","inflateEnd","uncompress2"],"critical_calls":["uncompress","crc32_z","crc32_z","gzdopen","gzgetc_","gzclose","inflateInit2_","inflate","inflateSetDictionary","inflateReset2","deflateInit_","deflateCopy","deflateEnd","inflateEnd","uncompress2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 12) {
        return 0;
    }

    // Initialize variables for decompression
    Bytef *dest = nullptr;
    uLongf destLen = 0;
    uLong sourceLen = size;
    int ret;
    
    // First, try to decompress using uncompress (traditional)
    destLen = size * 10;  // Initial estimate
    dest = (Bytef*)malloc(destLen);
    if (!dest) {
        return 0;
    }
    
    ret = uncompress(dest, &destLen, data, sourceLen);
    if (ret == Z_OK) {
        // Calculate CRC32 of the uncompressed data using crc32_z
        uLong crc = crc32_z(0L, Z_NULL, 0);
        crc = crc32_z(crc, dest, destLen);
        (void)crc; // Use result to avoid optimization
    }
    free(dest);
    
    // Create a FILE* for reading compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to read a single character from the compressed file using gzgetc_
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (gz_in) {
        int ch = gzgetc_(gz_in);
        (void)ch; // Use result to avoid optimization
        gzclose(gz_in);
    } else {
        // gzdopen may fail, but we don't want to close the file descriptor
        // since it will be closed when we close in_file
    }
    
    assert_file_closed(&in_file);;
    
    // Create z_stream structures for inflation
    z_stream strm1, strm2;
    Bytef *dict = nullptr;
    uInt dictLen = 0;
    
    // Initialize first inflation stream
    strm1.zalloc = Z_NULL;
    strm1.zfree = Z_NULL;
    strm1.opaque = Z_NULL;
    strm1.avail_in = 0;
    strm1.next_in = Z_NULL;
    
    ret = inflateInit2(&strm1, 15 + 32);  // Automatic header detection
    if (ret != Z_OK) {
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set up input data
    strm1.avail_in = (uInt)size;
    strm1.next_in = (Bytef*)data;
    
    // Try to inflate a small amount
    Bytef out_buf[1024];
    strm1.avail_out = sizeof(out_buf);
    strm1.next_out = out_buf;
    
    ret = inflate(&strm1, Z_NO_FLUSH);
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Try setting a dictionary using inflateSetDictionary
        // Use first few bytes of input as dictionary (this may not be correct)
        if (size > 256) {
            dictLen = 256;
        } else {
            dictLen = (uInt)size;
        }
        
        if (dictLen > 0) {
            dict = (Bytef*)malloc(dictLen);
            if (dict) {
                memcpy(dict, data, dictLen);
                ret = inflateSetDictionary(&strm1, dict, dictLen);
                (void)ret; // Use result to avoid optimization
                free(dict);
            }
        }
        
        // Reset inflation stream with different window bits using inflateReset2
        ret = inflateReset2(&strm1, 15);  // Raw deflate without headers
        (void)ret; // Use result to avoid optimization
    }
    
    // Create a second stream for deflateCopy
    ret = deflateInit(&strm2, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Try to copy the deflate stream (though strm1 is inflate)
        ret = deflateCopy(&strm2, (z_streamp)&strm1);
        (void)ret; // Use result to avoid optimization
        deflateEnd(&strm2);
    }
    
    // Clean up first stream
    inflateEnd(&strm1);
    
    // Now try uncompress2 with partial input
    destLen = size * 2;
    dest = (Bytef*)malloc(destLen);
    if (dest) {
        uLongf actualDestLen = destLen;
        uLong actualSourceLen = size;
        
        ret = uncompress2(dest, &actualDestLen, data, &actualSourceLen);
        (void)ret; // Use result to avoid optimization
        free(dest);
    }
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}