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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"deflateStateCheck":[[544,9,544,20,0,0,4,0]],"deflateInit2_":[[435,9,435,23,0,0,4,0]]},"library_calls":["inflateInit_","inflateGetHeader","inflateEnd","inflate","uncompress2_z","crc32_z","crc32_z","inflateCopy","inflateEnd","deflateInit2_","deflateEnd","gzdopen","gzclose","inflateSetDictionary","inflateEnd"],"critical_calls":["inflateInit_","inflateGetHeader","inflate","uncompress2_z","crc32_z","crc32_z","inflateCopy","inflateEnd","deflateInit2_","deflateEnd","gzdopen","gzclose","inflateSetDictionary","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Variables for decompression
    z_stream strm;
    Bytef *dest = nullptr;
    z_size_t destLen = 0;
    z_size_t sourceLen = size;
    
    // Initialize decompression stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    if (inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        return 0;
    }
    
    // Setup for inflateGetHeader
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.extra_max = 65536;
    header.name_max = 256;
    header.comm_max = 256;
    
    // Call inflateGetHeader
    inflateGetHeader(&strm, &header);
    
    // Prepare input for decompression
    strm.avail_in = static_cast<uInt>(size);
    strm.next_in = const_cast<Bytef*>(data);
    
    // Allocate output buffer for decompression
    destLen = size * 4;  // Reasonable initial guess
    dest = static_cast<Bytef*>(malloc(destLen));
    if (!dest) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.avail_out = static_cast<uInt>(destLen);
    strm.next_out = dest;
    
    // Perform decompression
    int ret = inflate(&strm, Z_FINISH);
    
    // Try uncompress2_z on the original data
    Bytef* dest2 = nullptr;
    z_size_t destLen2 = 0;
    z_size_t sourceLen2 = size;
    
    // Allocate buffer for uncompress2_z
    destLen2 = size * 4;
    dest2 = static_cast<Bytef*>(malloc(destLen2));
    if (dest2) {
        // Call uncompress2_z
        uncompress2_z(dest2, &destLen2, data, &sourceLen2);
        free(dest2);
    }
    
    // Calculate CRC32 of decompressed data (if any)
    if (strm.total_out > 0) {
        uLong crc = crc32_z(0L, Z_NULL, 0);
        crc = crc32_z(crc, dest, static_cast<z_size_t>(strm.total_out));
    }
    
    // Create a copy of the inflate stream
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(strm_copy));
    
    // Call inflateCopy
    if (inflateCopy(&strm_copy, &strm) == Z_OK) {
        inflateEnd(&strm_copy);
    }
    
    // Setup for deflateInit2_
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // Call deflateInit2_ with parameters derived from input
    int level = data[0] % 10;  // 0-9
    int method = Z_DEFLATED;
    int windowBits = 15 + (data[1] % 2);  // 15 or 16
    int memLevel = 1 + (data[2] % 9);     // 1-9
    int strategy = data[3] % 4;           // 0-3
    
    deflateInit2_(&deflate_strm, level, method, windowBits, 
                  memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
    deflateEnd(&deflate_strm);
    
    // Open input as FILE* for gzgetc
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Create gzFile from file descriptor
        int fd = fuzz_fileno(in_file);
        gzFile gzfile = gzdopen(fd, "rb");
        if (gzfile) {
            // Call gzgetc
            gzgetc(gzfile);
            gzclose(gzfile);
        }
        assert_file_closed(&in_file);;
    }
    
    // Try inflateSetDictionary with a small dictionary from input
    if (size > 20) {
        uInt dictLength = static_cast<uInt>(data[4] % 16 + 1);
        if (dictLength > size - 5) dictLength = static_cast<uInt>(size - 5);
        
        Bytef* dictionary = const_cast<Bytef*>(data + 5);
        inflateSetDictionary(&strm, dictionary, dictLength);
    }
    
    // Cleanup
    free(dest);
    inflateEnd(&strm);
    
    assert_file_closed(&in_file);
	return 0;
}