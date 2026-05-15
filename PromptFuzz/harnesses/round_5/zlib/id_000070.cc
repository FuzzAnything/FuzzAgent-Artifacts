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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["zlibVersion","inflateInit_","inflateGetHeader","inflateEnd","inflate","inflateCopy","inflateEnd","inflateSetDictionary","inflateEnd","gzdopen","gzclose","uncompress2_z","crc32_z","deflateInit2_","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","inflateInit_","inflateGetHeader","inflate","inflateCopy","inflateEnd","inflateSetDictionary","gzdopen","gzclose","uncompress2_z","crc32_z","deflateInit2_","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Initialize variables
    z_stream strm_inflate, strm_inflate_copy;
    gz_header header;
    Bytef *dest = nullptr;
    z_size_t destLen = 0;
    uLong crc = 0;
    const char *version = zlibVersion();
    
    // Step 2: Setup inflation stream with header reading
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&header, 0, sizeof(gz_header));
    
    if (inflateInit_(&strm_inflate, version, sizeof(z_stream)) != Z_OK) {
        return 0;
    }
    
    // Step 3: Set header for inflation
    inflateGetHeader(&strm_inflate, &header);
    
    // Step 4: Prepare input for inflation
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    
    // Step 5: Create output buffer for inflation
    destLen = size * 4;  // Reasonable initial guess
    dest = (Bytef*)malloc(destLen);
    if (!dest) {
        inflateEnd(&strm_inflate);
        return 0;
    }
    strm_inflate.next_out = dest;
    strm_inflate.avail_out = destLen;
    
    // Step 6: Perform partial inflation
    int inflate_result = inflate(&strm_inflate, Z_SYNC_FLUSH);
    
    // Step 7: Copy inflation stream state
    memset(&strm_inflate_copy, 0, sizeof(z_stream));
    if (inflateCopy(&strm_inflate_copy, &strm_inflate) == Z_OK) {
        // Clean up the copy
        inflateEnd(&strm_inflate_copy);
    }
    
    // Step 8: Set dictionary for inflation if we have some data
    if (strm_inflate.avail_in > 0) {
        // Use first 256 bytes or whatever is available as dictionary
        uInt dictSize = (strm_inflate.avail_in > 256) ? 256 : strm_inflate.avail_in;
        inflateSetDictionary(&strm_inflate, strm_inflate.next_in, dictSize);
    }
    
    // Step 9: Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(dest);
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 10: Create gzFile and read one character
    int fd = fuzz_fileno(in_file);
    gzFile gzfile = gzdopen(fd, "rb");
    if (gzfile) {
        gzgetc(gzfile);  // Read one character
        gzclose(gzfile);
    }
    assert_file_closed(&in_file);;
    
    // Step 11: Use uncompress2_z with the original data
    z_size_t uncompressed_len = 0;
    z_size_t source_len = size;
    Bytef* uncompressed = (Bytef*)malloc(size * 4);
    if (uncompressed) {
        // Note: source_len is both input and output parameter
        z_size_t source_len_copy = size;
        uncompress2_z(uncompressed, &uncompressed_len, data, &source_len_copy);
        free(uncompressed);
    }
    
    // Step 12: Calculate CRC32 of inflated data
    if (strm_inflate.total_out > 0) {
        crc = crc32_z(0L, dest, strm_inflate.total_out);
    }
    
    // Step 13: Setup deflation stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // Use default parameters for deflation
    int windowBits = 15 + 16;  // Add 16 for gzip header
    int memLevel = 8;
    int level = Z_DEFAULT_COMPRESSION;
    int method = Z_DEFLATED;
    int strategy = Z_DEFAULT_STRATEGY;
    
    deflateInit2_(&strm_deflate, level, method, windowBits, memLevel, 
                  strategy, version, sizeof(z_stream));
    
    // Clean up deflation stream
    deflateEnd(&strm_deflate);
    
    // Step 14: Clean up all resources
    inflateEnd(&strm_inflate);
    free(dest);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}